#include "SinglePole.h"
#include <iomanip>
#include "Component.h"
#include "AssociatedMemberArray.h"
 #include "PlaneFunction.h"
// #include "ExceptionMessage.h"
// #include "MemberException.h"
// #include "Auxiliary.h"
// #include "WinChinaDLT5551_2018.h"
// #include "ModelDateBase.h"
// #include "FdtReactionCalculator.h"
// #include "CodeChinaDL_T51302001.h"


namespace SmartPoleCore
{
	SinglePole::SinglePole()
	{
		this->clearAll();
	}


	SinglePole::~SinglePole()
	{
	}

	void SinglePole::clearAll()
	{
		_PointArray = new PointArray();
		_MemberArray = new MemberArray();
		_NodeArray = new NodeArray();
		_pElmArray = new ElemArray();

//		_PoleLoadArray = new NodeLoadManager();
// 		_InnerForceArray = new InnerForceArray();
// 		_DisplacementArray = new DisplacementArray();
// 		_ReactionForceArray = new DisplacementArray();
	}


	void SinglePole::addMembArray(const HdPointArray& pPointArray, const HdMemberArray& pMembArray)
	{
		_PointArray->addArray(pPointArray.GetPointee());
		_MemberArray->addArray(pMembArray.GetPointee());
	}


	void SinglePole::addComponent(const Component* pComp)
	{
		HdPointArray pTmpPoints = pComp->getPointArray();
		_PointArray->addArray(pTmpPoints.GetPointee());

		HdMemberArray pTmpMembers = pComp->getMemberArray();
		_MemberArray->addArray(pTmpMembers.GetPointee());
	}
	// @ function: 求解  --- by lsq 2020/4/1 9:37
// 	void SinglePole::solve(const HandleDesign& pDesignPara, const HdWinCondManager& pWinConds, const HdNodeLoadManager& pWireLoad, const HandleNGP& pCurNGP,ostream& fout, const HdProcessBar& pBar)
// 	{
// 		//test();
// 		clearResult();              //清空上一次计算结果
// 		mashSinglePole(pDesignPara);
// 		solveNodeLoad(pWinConds, pWireLoad, pDesignPara, pCurNGP, fout);
// 		solveFEM(pDesignPara, pCurNGP, pBar);                  //FEM建模
// 		//内力检查
// 	}

	// @ function:   --- by lsq 2020/5/13 10:49
// 	void SinglePole::mashSinglePole(const HandleDesign& pDesignPara)
// 	{
// 		findCnnAndCheckCoincident();             //去除多余点
// 		decomposedGeoModel();       //打断
// 		verifyModelIntegrity();     //检查模型完整性
// 		meshElement(pDesignPara);                 //划分网格
// 
// 	}
	// @ function:   --- by lsq 2020/5/13 17:02
// 	void SinglePole::solveNodeLoad(const HdWinCondManager& pWinConds, const HdNodeLoadManager& pWireLoad, const HandleDesign& pDesignPara, const HandleNGP& pCurNGP, ostream& fout)
// 	{
// 		_PoleLoadArray = pWireLoad->DeepCopy();
// 		_PoleLoadArray->appendToFEMModel(_NodeArray.GetPointee());
// 		solveWinLoad(pWinConds, pDesignPara, pCurNGP, fout);      //求解点荷载
// 	}

	void SinglePole::findCnnAndCheckCoincident()
	{
		std::map<int, HandlePoint> pCnnPoint;
		for (int iloop = 0; iloop < _MemberArray->count(); ++iloop)
		{
			HandleMember pElm = _MemberArray->getAt(iloop);
			pCnnPoint.insert(pair<int, HandlePoint>(pElm->getStartNum(), pElm->getStartPt()));
			pCnnPoint.insert(pair<int, HandlePoint>(pElm->getEndNum(), pElm->getEndPt()));
		};
		_PointArray->clear();
		for (std::map<int, HandlePoint>::iterator iIter = pCnnPoint.begin(); iIter != pCnnPoint.end(); ++iIter)
			_PointArray->add(iIter->second);
		HandlePoint pTmpPoint = _PointArray->findCoincidentPoint();
		if (!pTmpPoint.isNull())
		{
			static char ErrorInfo[50];
			sprintf_s(ErrorInfo, "存在两个节点坐标重合，重合节点：%d, 请检查！", pTmpPoint->TagNum());
			throw exception(ErrorInfo);
		}
	}

	void SinglePole::decomposedGeoModel()
	{//先对几何模型进行打断
		int tmpMaxLoop = 0;
		int AppendPointNum = 9850;
		HdMemberArray newMemberArry = new MemberArray();
		while (!_MemberArray->empty())
		{
			HandleMember pMemb1 = _MemberArray->back();
			_MemberArray->removeBack();
			HandlePoint pStartP1 = pMemb1->getStartPt();
			HandlePoint pEndP1 = pMemb1->getEndPt();
			bool b_pushback = true;
			for (int tmploop = 0; tmploop < _MemberArray->count(); ++tmploop)
			{
				HandleMember pMemb2 = _MemberArray->getAt(tmploop);
				HandlePoint pStartP2 = pMemb2->getStartPt();
				HandlePoint pEndP2 = pMemb2->getEndPt();
				if (pStartP1->TagNum() == pStartP2->TagNum() || pStartP1->TagNum() == pEndP2->TagNum())
					continue;//重合杆件忽略
				else if (pEndP1->TagNum() == pStartP2->TagNum() || pEndP1->TagNum() == pEndP2->TagNum())
					continue;//重合杆件忽略
				{
					HandleVec3D crossResult = LineSegmentHelper::SegmentSegmentCrossPoint(pStartP1.GetPointee(), pEndP1.GetPointee(), pStartP2.GetPointee(), pEndP2.GetPointee());
					if (crossResult.isNull())
						continue;
					
					if (crossResult.GetPointee() == pStartP1.GetPointee() || crossResult.GetPointee() == pEndP1.GetPointee())
					{
						HandlePoint pCossPt = static_cast<Point*>(crossResult.GetPointee());
						HandleProfile pNewProfile = pMemb2->solveRatioProfile(pCossPt.GetPointee());
						Member* fatherMemb = pMemb2->getFatherMemb();
						HandleMember pNewMemb1 = new Member(pMemb2->getTYPE(), pStartP2, pMemb2->getStartProfile(), pCossPt, pNewProfile, fatherMemb);
						HandleMember pNewMemb2 = new Member(pMemb2->getTYPE(), pCossPt, pNewProfile, pEndP2, pMemb2->getEndProfile(), fatherMemb);
						_MemberArray->replace(tmploop, pNewMemb1);
						_MemberArray->insert(tmploop, pNewMemb2);
					}
					else if (crossResult.GetPointee() == pStartP2.GetPointee() || crossResult.GetPointee() == pEndP2.GetPointee())
					{
						HandlePoint pCossPt = static_cast<Point*>(crossResult.GetPointee());
						HandleProfile pNewProfile = pMemb1->solveRatioProfile(pCossPt.GetPointee());
						Member* fatherMemb = pMemb1->getFatherMemb();
						HandleMember pNewMemb1 = new Member(pMemb1->getTYPE(), pStartP1, pMemb1->getStartProfile(), pCossPt, pNewProfile, fatherMemb);
						HandleMember pNewMemb2 = new Member(pMemb1->getTYPE(), pCossPt, pNewProfile, pEndP1, pMemb1->getEndProfile(), fatherMemb);
						b_pushback = false;
						_MemberArray->add(pNewMemb1.GetPointee());
						_MemberArray->add(pNewMemb2.GetPointee());
						break;
					}
					else
					{//交叉点
						//还有漏洞，就是可能有共用点
						HandlePoint pCossPt = _PointArray->FindEqual(crossResult.GetPointee());
						if (pCossPt.isNull())
							pCossPt = new Point(crossResult.GetPointee(), ++AppendPointNum);
						HandleProfile pNewProfile1 = pMemb1->solveRatioProfile(pCossPt.GetPointee());
						Member* fatherMemb1 = pMemb1->getFatherMemb();
						HandleMember pNewMemb1 = new Member(pMemb1->getTYPE(), pStartP1, pMemb1->getStartProfile(), pCossPt, pNewProfile1, fatherMemb1);
						HandleMember pNewMemb2 = new Member(pMemb1->getTYPE(), pCossPt, pNewProfile1, pEndP1, pMemb1->getEndProfile(), fatherMemb1);
						_MemberArray->add(pNewMemb1.GetPointee());
						_MemberArray->add(pNewMemb2.GetPointee());


						HandleProfile pNewProfile2 = pMemb2->solveRatioProfile(pCossPt.GetPointee());
						Member* fatherMemb2 = pMemb2->getFatherMemb();
						HandleMember pNewMemb3 = new Member(pMemb2->getTYPE(), pStartP2, pMemb2->getStartProfile(), pCossPt, pNewProfile2, fatherMemb2);
						HandleMember pNewMemb4 = new Member(pMemb2->getTYPE(), pCossPt, pNewProfile2, pEndP2, pMemb2->getEndProfile(), fatherMemb2);
						_MemberArray->replace(tmploop, pNewMemb3);
						_MemberArray->insert(tmploop, pNewMemb4);
						b_pushback = false;
						break;
					}
				}
			}
			if (b_pushback && !pMemb1.isNull())
			{
				newMemberArry->add(pMemb1);
				tmpMaxLoop = 0;
			}
			else if (tmpMaxLoop++ > 100)
				throw exception("Errors in the model, check please!");
		}
		_MemberArray = newMemberArry;
	}

	//这个是校验模型完整性
	void SinglePole::verifyModelIntegrity()
	{
		HandlePoint pFixedPoint = _PointArray->getFixedPoint();
		if (pFixedPoint.isNull())
			throw exception("模型没有设置固定约束点");
		std::set<Member*> membSet;
		std::map<int, HandleNode> pointToNodeMap;
		std::map<int, AssociatedMemberArray> cnnSet;

		this->findCnnMembSet(&cnnSet);
		this->registerCnnMemb(pFixedPoint, &cnnSet, &membSet);
		this->checkCollinear(&cnnSet);
	}

	// @ function: 划分有限元网格  --- by lsq 2020/3/27 9:51
// 	void SinglePole::meshElement(const HandleDesign& pDesignPara)
// 	{//根据高度排个序
// 		_MemberArray->sortByEndZCoor();
// 		int startNodeNum = 0;
// 		for (int iloop = 0; iloop < _MemberArray->count(); ++iloop)
// 		{
// 			HandleMember pMemb = _MemberArray->getAt(iloop);
// 			if (pMemb->getTYPE()==Member::GuyWire)
// 			{
// 				HandleProfile pTmpProfile = pMemb->getStartProfile();
// 				HandleNode pSubStartNode, pNewNode;
// 				pSubStartNode = _NodeArray->findByFatherTagNum(pMemb->getStartNum());//起点
// 				pNewNode = _NodeArray->findByFatherTagNum(pMemb->getEndNum());
// 				if (pSubStartNode.isNull())
// 				{
// 					pSubStartNode = new CNode(pMemb->getStartPt().GetPointee(), startNodeNum++);
// 					_NodeArray->add(pSubStartNode);
// 				}
// 				if (pNewNode.isNull())
// 				{
// 					pNewNode = new CNode(pMemb->getEndPt().GetPointee(), startNodeNum++);
// 					_NodeArray->add(pNewNode);
// 				}
// 				_pElmArray->Add(new BaseElement(pSubStartNode, pNewNode, pMemb->getFatherMemb(), pTmpProfile));
// 			}
// 			else
// 			{
// 				startNodeNum = pMemb->MeshToElms(_NodeArray.GetPointee(), _pElmArray.GetPointee(), pDesignPara->getElemSize(), startNodeNum);
// 			}
// 		}
// 	}
// 
// 	void SinglePole::addNodeToSolver(SixFEM::SixDomain* theDomain)
// 	{
// 		for (int iloop = 0; iloop < _NodeArray->count(); ++iloop)
// 		{
// 			HandleNode pNode = _NodeArray->getAt(iloop);
// 			SixFEM::HandleNode Node1 = new SixFEM::SixNode(pNode->TagNum(), 6, pNode->x(), pNode->y(), pNode->z());
// 			theDomain->addNode(Node1);
// 			if (pNode->isFixed())
// 			{
// 				theDomain->addSPConstraint(pNode->TagNum(), 1);//加约束
// 				HdNodeDisplacementArray ReactionForceArray = new NodeDisplacementArray(pNode, _PoleLoadArray->getCaseSize());
// 				_ReactionForceArray->add(ReactionForceArray);
// 			}
// 			HdNodeDisplacementArray NodeDispArray = new NodeDisplacementArray(pNode, _PoleLoadArray->getCaseSize());
// 			_DisplacementArray->add(NodeDispArray);
// 		}
// 	}
// 	double _tempEps6 = 0.0000010;
// 	double _tempEps12 = 0.0000000000010;
// 	void SinglePole::addElmToSolver(SixFEM::SixDomain* theDomain, const HandleDesign& pDesignPara)
// 	{
// // 		string windFileName = "aaa.test";
// // 		std::ofstream efout(windFileName.c_str());
// // 		if (!efout)
// // 			throw exception("cannot open .Inp file");
// // 		efout.flags(ios::right | ios::dec | ios::fixed);//右边输出，十进制，固定位数。
// 		std::vector<double> vecInLocXZPlane1(3);
// 		int secId = 0;
// 		for (int iloop = 0; iloop < _pElmArray->count(); ++iloop)
// 		{
// 			HandleElement pElement = _pElmArray->getAt(iloop);
// 			pElement->setId(iloop + 1);
// 			//初始化内力结果内存
// 			HdElmInnerForceArray pInnerForceArray = new ElmInnerForceArray(pElement, _PoleLoadArray->getCaseSize());
// 			_InnerForceArray->add(pInnerForceArray);
// 
// 			HandleNode startPt = pElement->StartNode();
// 			HandleNode endPt = pElement->EndNode();
// 			if (startPt->isVertical(endPt.GetPointee()))
// 			{
// 				vecInLocXZPlane1[0] = startPt->x() + 0.5;
// 				vecInLocXZPlane1[1] = startPt->y();
// 				vecInLocXZPlane1[2] = startPt->z();
// 			}
// 			else
// 			{
// 				vecInLocXZPlane1[0] = startPt->x();
// 				vecInLocXZPlane1[1] = startPt->y();
// 				vecInLocXZPlane1[2] = startPt->z() + 0.5;
// 			}
// 
// 			double	Area = pElement->getCurProfile()->getArea()*_tempEps6;//有效面积
// 			HandleMaterial pMat = pElement->getCurMatriel();
// 			HandleProfile pProfile = pElement->getCurProfile();
// 		
// 			// 			HandleMember pFa = pElement->GetMember();
// 			// 			efout << setw(10) << pElement->StartNum() << setw(10) << pElement->EndNum()
// 			// 				<< setw(8) << pFa->getStartNum() << setw(8) << pFa->getEndNum() << setw(12) << setprecision(1) << pMat->getE() *1000.0<< setw(12) << setprecision(1) << pMat->getG()*1000.0
// 			// 				<< setw(12) << setprecision(8) << Area << setw(12) << pProfile->getJxy()*_tempEps12 << setw(12) << pProfile->getIxx()*_tempEps12 
// 			// 				<< setw(12) << pProfile->getIyy()*_tempEps12<< setw(12) << Area*0.5 << setw(12) << Area*0.5 << endl;
// 
// 			if (pElement->GetMember()->getTYPE() == Member::GuyWire)
// 			{
// 				//加杆单元
// 				SixFEM::HandleMaterial mat = new SixFEM::SixMaterial(pElement->getId(), 1, pMat->getE()*1000.0); // 杆单元材料
// 				theDomain->addSpringElement(pElement->getId(), pElement->StartNum(), pElement->EndNum(), Area, mat);
// 			}
// 			else
// 			{
// 				theDomain->addBeamSection(new SixFEM::BeamSection(pMat->getE()*1000.0, pMat->getG()*1000.0,
// 					Area,
// 					pProfile->getJxy()*_tempEps12,
// 					pProfile->getIxx()*_tempEps12,
// 					pProfile->getIyy()*_tempEps12,
// 					Area*0.5,
// 					Area*0.5,
// 					vecInLocXZPlane1));
// 				theDomain->addBeamElement(pElement->getId(), pElement->StartNum(), pElement->EndNum(), secId++, false, pDesignPara->getNonlinearFlag());
// 			}
// 		}
// 	}

	// @ function:   --- by lsq 2020/4/1 11:23
// 	void SinglePole::solveFEM(const HandleDesign& pDesignPara, const HandleNGP& pCurNGP, const HdProcessBar& pBar)
// 	{
// 		//初始化
//  		SixFEM::HandleDomain theDomain = new SixFEM::SixDomain();
// 		this->addNodeToSolver(theDomain.GetPointee());
// 		this->addElmToSolver(theDomain.GetPointee(), pDesignPara);
// 		//以反力点为中心寻找杆件
// 		std::vector<FdtReactionCalculator> fdtCalculator;
// 		for (int iloop = 0; iloop < _ReactionForceArray->count(); ++iloop)
// 		{
// 			fdtCalculator.push_back(FdtReactionCalculator(_ReactionForceArray->getAt(iloop)));
// 			fdtCalculator.back().findCnnElem(_pElmArray);
// 		}
// 		
// 		vector<double> disp(6);
// 		vector<double> innerforce(12);
// 		double Fn = 0;
// 		//对每个工况进行计算
// 		for (int iloop = pCurNGP->getActiveStartCase(); iloop < pCurNGP->getActiveEndCase(); ++iloop)
// 		{
// 			theDomain->addLoadPattern(iloop);//添加工况
// 			for (int jloop = 0; jloop < _PoleLoadArray->size(); ++jloop)
// 			{
// 				HandleLoadVec nodeLoadVec = _PoleLoadArray->at(jloop);
// 				const NodeLoad& nodeLoad = nodeLoadVec->getNCaseForce(iloop);//加风荷载
// 				theDomain->addNodalLoad(iloop, nodeLoadVec->GetFnode()->TagNum(), nodeLoad.Fx(), nodeLoad.Fy(), nodeLoad.Fz(), 0., 0., 0.);
// 			}
// 			theDomain->analyze(pDesignPara->getNonlinearFlag());
// 			//获取位移结果
// 			for (int jloop = 0; jloop < _DisplacementArray->count(); ++jloop)
// 			{
// 				HdNodeDisplacementArray NodeDispArray = _DisplacementArray->getAt(jloop);
// 				theDomain->getDispAtBeamNode(NodeDispArray->getNode()->TagNum(), disp);
// 				NodeDispArray->setDispResult(iloop, disp,0);
// 			}
// 
// 			//获取内力结果
// 			for (int jloop = 0; jloop < _InnerForceArray->count(); ++jloop)
// 			{
// 				HdElmInnerForceArray pElmInnerForceArray = _InnerForceArray->getAt(jloop);
// 				if (pElmInnerForceArray->getElement()->GetMember()->getTYPE()==Member::GuyWire)
// 				{
// 					theDomain->getInnerForceAtSpringElem(jloop + 1, Fn);
// 					innerforce = { Fn, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// 				}
// 				else
// 				{
// 					theDomain->getInnerForceAtBeamElem(jloop + 1, innerforce);
// 				}
// 				pElmInnerForceArray->setForceAtCase(iloop, innerforce);
// 			}
// 
// 			for (std::vector<FdtReactionCalculator>::iterator iIter = fdtCalculator.begin(); iIter < fdtCalculator.end();++iIter)
// 			{//计算基础反力，要注意可能存在多根相连接的情况
// 				iIter->solveForce(theDomain.GetPointee(), iloop);
// 			}
// 
// 			theDomain->removeLoadPattern(iloop);
// 			if (!pBar.isNull())
// 				pBar->addValue(1);
// 		}
// 	}
// 
// 	void SinglePole::solveWinLoad(const HdWinCondManager& pWinConds, const HandleDesign& pDesignPara, const HandleNGP& pCurNGP, ostream& fout)
// 	{
// 		WinChinaDLT5551_2018 winSolver(_pElmArray, pWinConds, pCurNGP);
// 		CBox3D box=_MemberArray->GetBox();
// 		winSolver.initial(pDesignPara, box);
// 		HdNodeLoadManager tmpNodeLoadManager = winSolver.solve(fout);
// 		_PoleLoadArray->SuperPositionNLM(tmpNodeLoadManager.GetPointee());
// 	}

	// @ function: 清空计算结果  --- by lsq 2020/4/1 16:26
	void SinglePole::clearResult()
	{
		_NodeArray = new NodeArray();
		_pElmArray = new ElemArray();

//		_PoleLoadArray = new NodeLoadManager();
// 		_InnerForceArray = new InnerForceArray();
// 		_DisplacementArray = new DisplacementArray();
// 		_ReactionForceArray = new DisplacementArray();
	}

	void SinglePole::findCnnMembSet(std::map<int, AssociatedMemberArray>* pCnnSet)
	{
		for (int iloop = 0; iloop < _MemberArray->count(); ++iloop)
		{
			//集合本段杆件
			HandleMember pElm = _MemberArray->getAt(iloop);
			std::map<int, AssociatedMemberArray>::iterator findResIter = pCnnSet->find(pElm->getStartNum());
			if (findResIter == pCnnSet->end())
			{
				pair<std::map<int, AssociatedMemberArray>::iterator, bool> InsetFlag = pCnnSet->insert(\
					pair<int, AssociatedMemberArray>(pElm->getStartNum(), AssociatedMemberArray(pElm->getStartPt().GetPointee())));
				findResIter = InsetFlag.first;
			}
			findResIter->second.addElm(pElm);

			findResIter = pCnnSet->find(pElm->getEndNum());

			if (findResIter == pCnnSet->end())
			{
				pair<std::map<int, AssociatedMemberArray>::iterator, bool> InsetFlag = pCnnSet->insert(\
					pair<int, AssociatedMemberArray>(pElm->getEndNum(), AssociatedMemberArray(pElm->getEndPt().GetPointee())));
				findResIter = InsetFlag.first;
			}
			findResIter->second.addElm(pElm);
		};
	}

	void SinglePole::registerCnnMemb(const HandlePoint& pCentrlPoint, std::map<int, AssociatedMemberArray>* pCnnSet, std::set<Member*>* pRegisterSet)
	{
		std::map<int, AssociatedMemberArray>::iterator findResIter = pCnnSet->find(pCentrlPoint->TagNum());
		int registerCount= 0;
		for (int iloop = 0; iloop < findResIter->second.getCnnMembCount();++iloop)
		{
			HandleMember pMemb = findResIter->second.getCnnMembAt(iloop);
			pair<std::set<Member*>::iterator, bool> insertRes = pRegisterSet->insert(pMemb.GetPointee());//登记相连接的杆件
			if (!insertRes.second)
				++registerCount;
		}
		if (registerCount == findResIter->second.getCnnMembCount())
			return;

		//递归调用
		for (int iloop = 0; iloop < findResIter->second.getCnnMembCount(); ++iloop)
		{
			HandlePoint pPoint = findResIter->second.getEndPointAt(iloop);
			this->registerCnnMemb(pPoint, pCnnSet, pRegisterSet);
		}
	}

	void SinglePole::checkCollinear(std::map<int, AssociatedMemberArray>* pCnnSet)
	{
		for (std::map<int, AssociatedMemberArray>::iterator iIter = pCnnSet->begin(); iIter != pCnnSet->end(); ++iIter)
		{
			if (iIter->second.bCheckLineOverlap())
			{
				static char ErrorInfo[50];
				sprintf_s(ErrorInfo, "模型存在错误，在节点%d,存在重合线的情况", iIter->first);
				throw exception(ErrorInfo);
			}
			iIter->second.setCrossArmByLegDiameter();
		}
	}
	// @ function: 内力校核  --- by lsq 2020/4/15 14:40
	void SinglePole::checkMember(int legNum)
	{
// 		for (int iloop = 0; iloop < _InnerForceArray->count(); ++iloop)
// 			_InnerForceArray->getAt(iloop)->profileCheck(legNum);
	}
	// @ function: 连接计算 --- by lsq 2020/4/29 19:47
	void SinglePole::ConnectCalc(HandleMember pMemb, bool isStart, int LegNum, bool isOptimization)
	{
		//_InnerForceArray->CalcConn(pMemb, isStart, LegNum, isOptimization);
	}
	// @ function:   --- by lsq 2020/8/28 16:07
	void SinglePole::test()
	{
// 		bool isLargeDeformation = false;
// 		SixFEM::HandleDomain theDomain = new SixFEM::SixDomain();
// 		//加节点
// // 		vector<int> id; Utility::GenerateLinespacedVectorByStep<int>(id, 1, 3, 1);
// // 		vector<double> x(3); x[0] = 0.; x[1] = 1.; x[2] = 1.;
// // 		vector<double> y(3); y[0] = 0.; y[1] = 0.; y[2] = 1.;
// // 		vector<double> z(3, 0.0);
// 		theDomain->addNode(1, 0, 0, 0);
// 		theDomain->addNode(2, 1, 0, 0);
// 		theDomain->addNode(3, 1, 1, 0);
// 		//加截面
// 		double E = 3000000;
// 		double G = 1250000;
// 		double A = 7.854e-3;
// 		double Jx = 9.817e-6;
// 		double Iy = 4.909e-6;
// 		double Iz = 4.909e-6;
// 		double Avy = 7.069e-3;
// 		double Avz = 7.069e-3;
// 		std::vector<double> vecInLocXZPlane1(3); vecInLocXZPlane1[0] = 0.; vecInLocXZPlane1[1] = 0.5; vecInLocXZPlane1[2] = 0.5;
// 	
// 		SixFEM::HandleMaterial mat = new SixFEM::SixMaterial(1, 1, E); // 杆单元材料
// 		theDomain->addSpringElement(3, 2, 3, A, mat);
// 
// 		//加梁单元
// // 		vector<int> id_beam; Utility::GenerateLinespacedVectorByStep<int>(id_beam, 1, 2, 1);
// // 		vector<int> node1(2); node1[0] = 1; node1[1] = 1;
// // 		vector<int> node2(2); node2[0] = 2; node2[1] = 3;
// 		SixFEM::HandleBeamSection sec = new SixFEM::BeamSection(E, G, A, Jx, Iy, Iz, Avy, Avz, vecInLocXZPlane1);
// 		theDomain->addBeamSection(sec);
// 		theDomain->addBeamElement(1, 1, 2, 0);
// 		theDomain->addBeamElement(2, 1, 3, 0);
// 		//加杆单元
// 		//加约束
// 		theDomain->addSPConstraint(1, 1);
// 		//加工况
// 		theDomain->addLoadPattern(1);
// 		//加荷载
// 		theDomain->addNodalLoad(1, 3, 0., 10., 0., 0., 0., 0.);
// 		//删工况
// 		//theDomain->removeLoadPattern(1);
// 		//计算
// 		theDomain->analyze(isLargeDeformation);
// 		//后处理
// 		/*
// 		cout << "单元力：" << endl;
// 		vector<double> innerforce(12);
// 		theDomain->getInnerForceAtBeamElem(1, innerforce);
// 		cout << "Fx_I:" << innerforce[0] << endl
// 		<< "Fy_I:" << innerforce[1] << endl
// 		<< "Fz_I:" << innerforce[2] << endl
// 		<< "Mx_I:" << innerforce[3] << endl
// 		<< "My_I:" << innerforce[4] << endl
// 		<< "Mz_I:" << innerforce[5] << endl
// 		<< "Fx_J:" << innerforce[6] << endl
// 		<< "Fy_J:" << innerforce[7] << endl
// 		<< "Fz_J:" << innerforce[8] << endl
// 		<< "Mx_J:" << innerforce[9] << endl
// 		<< "My_J:" << innerforce[10] << endl
// 		<< "Mz_J:" << innerforce[11] << endl
// 		<< endl;
// 		*/
// 		double Fn = 0;
// 		theDomain->getInnerForceAtSpringElem(3, Fn);
// 		std::string file("../../result_Truss.dat");
// 		theDomain->summary(file);

	}

}
