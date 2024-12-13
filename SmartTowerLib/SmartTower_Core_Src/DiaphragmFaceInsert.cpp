//#include "StdAfx.h"
#include "DiaphragmFaceInsert.h"
#include "TTAType.h"
#include "CodeRegister.h"
#include "TowerPoint.h"

//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	
	 DiaphragmFaceInfo::DiaphragmFaceInfo(void)
	 {
		 _TP=0;
		 _NI=0;
		 _M1=0;
		 _IF1=0;
		 _IF2=0;
		 _M2=0;
		 _IS1=0;
		 _IS2=0;
		 _N0=0;
		 _N1=0;
		 _N2=0;
		 _N3=0;
		 _N4=0;
	 };
	 DiaphragmFaceInfo::~DiaphragmFaceInfo(void)
	 {	 
	 };

	 void DiaphragmFaceInfo::InsertToModel( CTowerModel* TheModel )
	 {
		 this->createPointsAndMembers();
		 HdTowerPointArray pGSecPoints=TheModel->getSecondaryPointArray();
		 pGSecPoints->addArray(_pSecPointArray.GetPointee());
		 HdMemberArray pGMemberArray=TheModel->getMemberArray();
		 pGMemberArray->addArray(_pMemberArray.GetPointee());
		 _pSecPointArray=NULL;
		 _pMemberArray=NULL;
	 }

	 void DiaphragmFaceInfo::MKindTransfer()
	 {
	 }

	 bool DiaphragmFaceInfo::CompareToMe(HandleNode cornerNode,std::map<int,HandleNode>* tmpNodeMap)
	 {
		 this->createPointsAndMembers(); 
		 MetaModel metaModel;
		 HdTowerPointArray PriPointArray=new TowerPointArray();
		 PriPointArray->push_back(new CPrimaryPoint(cornerNode->TagNum(),Symetry::XYsym,cornerNode->x(),cornerNode->y(),cornerNode->z()));
		 metaModel.setPrimaryPointArray(PriPointArray);
		 metaModel.setSecondaryPointArray(_pSecPointArray);
		 metaModel.setMemberArray(_pMemberArray);
		 metaModel.CheckAndUpdata();
		 HdNodeArray pmetaNodeArray=metaModel.getNodeArray();
		  //检查杆件是否一一对应
		 HdElemArray pmetaElms=metaModel.getElemArray();
		 _pSecPointArray=NULL;
		 _pMemberArray=NULL;
		 if(tmpNodeMap->size()!=pmetaNodeArray->size())
			 return false;
		 //检查点的坐标和节点编号是否相同		
		 for (std::map<int,HandleNode>::iterator Iter=tmpNodeMap->begin();Iter!=tmpNodeMap->end();++Iter)
		 {
			 bool FlagHasElm=false;
			 for (int iloop = 0; iloop < pmetaNodeArray->size(); ++iloop)
			 {
				 HandleNode pTmptNode = pmetaNodeArray->at(iloop);
				 if (Iter->second->CoordEqual(pTmptNode.GetPointee()))
				 {
					 FlagHasElm=true;
					 break;
				 }
			 }
			 if(!FlagHasElm)
				 return false;
		 }
		 return true;
	 }

	 void DiaphragmFaceInfo::CreateNodesAndElms( HandleNode cornerNode )
	 {
		 
	 }
	 int DiaphragmFaceInfo::intstep=10;

	 /////////////////////
    ///////TP30/////////
	////////////////////

	 CDiaphragmFaceInsertTP30::CDiaphragmFaceInsertTP30()
	 {
		 this->TP(30);
	 }

	CDiaphragmFaceInsertTP30::~CDiaphragmFaceInsertTP30(void)
	{

	}


	void CDiaphragmFaceInsertTP30::createPointsAndMembers()
	{
		//创建节点
		int curNum=this->NI();
		int n1=this->N1();
		if (n1<1)
		{
			n1=curNum;
			curNum+=intstep;
		}
		_pSecPointArray=new TowerPointArray();
		CSecondaryPoint tmpN1(n1,Symetry::Ysym,this->N0(),this->N0()+1,2);
		HandleTowerPoint tmpN1SecPoint=tmpN1.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpN1SecPoint);
		int n2=this->N2();
		if (n2<1)
		{
			n2=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpN2(n2,Symetry::Xsym,this->N0(),this->N0()+2,2);
		HandleTowerPoint tmpN2SecPoint=tmpN2.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpN2SecPoint);
		int n3=this->N3();
		if (n3<1)
		{
			n3=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpn3(n3,Symetry::XYsym,this->N0(),n1,2);
		HandleTowerPoint tmpn3SecPoint=tmpn3.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn3SecPoint);
		int n4=this->N4();
		if (n4<1)
		{
			n4=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpn4(n4,Symetry::XYsym,this->N0(),n2,2);
		HandleTowerPoint tmpn4SecPoint=tmpn4.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn4SecPoint);
		int n5=curNum;	
		int n6=n5+intstep;
		int n7=n6+intstep;
		CSecondaryPoint tmpn5(n5,Symetry::XYsym,n4,n3+1,4);
		HandleTowerPoint tmpn5SecPoint=tmpn5.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn5SecPoint);
		CSecondaryPoint tmpn6(n6,Symetry::Ysym,n3+1,n4,3);
		HandleTowerPoint tmpn6SecPoint=tmpn6.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn6SecPoint);
		CSecondaryPoint tmpn7(n7,Symetry::Xsym,n4+2,n3,3);
		HandleTowerPoint tmpn7SecPoint=tmpn7.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn7SecPoint);


		//创建杆件
		_pMemberArray=new MemberArray();
		_pMemberArray->push_back(new MemberInf(this->N0(),n3,0,Symetry::XYsym,3,20,this->IF1(),this->IF2(),0));
		_pMemberArray->push_back(new MemberInf(n1,n3,0,Symetry::XYsym,3,20,this->IF1(),this->IF2(),0));
		_pMemberArray->push_back(new MemberInf(this->N0(),n4,0,Symetry::XYsym,3,20,this->IS1(),this->IS2(),0));
		_pMemberArray->push_back(new MemberInf(n2,n4,0,Symetry::XYsym,3,20,this->IS1(),this->IS2(),0));
		_pMemberArray->push_back(new MemberInf(n4,n5,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n5,n6,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n3,n6,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n3,n5,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n5,n7,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n4,n7,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n1,n6,0,Symetry::Ysym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n2,n7,0,Symetry::Xsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(this->N0(),n5,0,Symetry::XYsym,3,20,1,0,0));
		this->MKindTransfer();
	}
	/////////////////////
    ///////TP31/////////
	////////////////////

	CDiaphragmFaceInsertTP31::CDiaphragmFaceInsertTP31()
	{
		this->TP(31);
	}
	CDiaphragmFaceInsertTP31::~CDiaphragmFaceInsertTP31(void)
	{

	}

	void CDiaphragmFaceInsertTP31::createPointsAndMembers()
	{   //////////////////////////////////////////////////////////////////////
		/////////////////////创建节点/////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		int curNum=this->NI();
		int n1=this->N1();
		if (n1<1)
		{
			n1=curNum;
			curNum+=intstep;
		}
		_pSecPointArray=new TowerPointArray();
		CSecondaryPoint tmpN1(n1,Symetry::Ysym,this->N0(),this->N0()+1,2);
		HandleTowerPoint tmpN1SecPoint=tmpN1.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpN1SecPoint);
		int n2=this->N2();
		if (n2<1)
		{
			n2=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpN2(n2,Symetry::Xsym,this->N0(),this->N0()+2,2);
		HandleTowerPoint tmpN2SecPoint=tmpN2.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpN2SecPoint);
		int n3=this->N3();
		if (n3<1)
		{
			n3=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpn3(n3,Symetry::XYsym,this->N0(),n1,2);
		HandleTowerPoint tmpn3SecPoint=tmpn3.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn3SecPoint);
		int n4=this->N4();
		if (n4<1)
		{
			n4=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpn4(n4,Symetry::XYsym,this->N0(),n2,2);
		HandleTowerPoint tmpn4SecPoint=tmpn4.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn4SecPoint);
		int n5=curNum;	
		int n6=n5+intstep;
		int n7=n6+intstep;
		int n8=n7+intstep;
		int n9=n8+intstep;
		CSecondaryPoint tmpn5(n5,Symetry::XYsym,n4,n1,3);
		HandleTowerPoint tmpn5SecPoint=tmpn5.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn5SecPoint);
		CSecondaryPoint tmpn6(n6,Symetry::XYsym,n1,n4,3);
		HandleTowerPoint tmpn6SecPoint=tmpn6.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn6SecPoint);
		CSecondaryPoint tmpn7(n7,Symetry::XYsym,n2,n3,3);
		HandleTowerPoint tmpn7SecPoint=tmpn7.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn7SecPoint);
		CSecondaryPoint tmpn8(n8,Symetry::Ysym,n6,n6+1,2);
		HandleTowerPoint tmpn8SecPoint=tmpn8.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn8SecPoint);
		CSecondaryPoint tmpn9(n9,Symetry::Xsym,n7,n7+2,2);
		HandleTowerPoint tmpn9SecPoint=tmpn9.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn9SecPoint);



		//////////////////////////////////////////////////////////////////////////////////
		////////////////////////////创建杆件//////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		_pMemberArray=new MemberArray();
		size_t MenbCount=_pMemberArray->size();
		_pMemberArray->push_back(new MemberInf(this->N0(),n3,0,Symetry::XYsym,3,20,this->IF1(),this->IF2(),0));
		_pMemberArray->push_back(new MemberInf(n1,n3,0,Symetry::XYsym,3,20,this->IF1(),this->IF2(),0));
		_pMemberArray->push_back(new MemberInf(this->N0(),n4,0,Symetry::XYsym,3,20,this->IS1(),this->IS2(),0));
		_pMemberArray->push_back(new MemberInf(n2,n4,0,Symetry::XYsym,3,20,this->IS1(),this->IS2(),0));
		_pMemberArray->push_back(new MemberInf(n1,n6,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n5,n6,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n4,n5,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n3,n5,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n5,n7,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n2,n7,0,Symetry::XYsym,3,20,0,200,0));

		_pMemberArray->push_back(new MemberInf(n6,n8,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n1,n8,0,Symetry::Ysym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n7,n9,0,Symetry::XYsym,3,20,0,200,0));	
		_pMemberArray->push_back(new MemberInf(n2,n9,0,Symetry::Xsym,3,20,1,0,0));

		_pMemberArray->push_back(new MemberInf(n3,n6,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n4,n7,0,Symetry::XYsym,3,20,1,0,0));

		_pMemberArray->push_back(new MemberInf(this->N0(),n5,0,Symetry::XYsym,3,20,1,0,0));
		this->MKindTransfer();
	}


	/////////////////////
    ///////TP32/////////
	////////////////////

	CDiaphragmFaceInsertTP32::CDiaphragmFaceInsertTP32()
	{
		this->TP(32);
	}
	CDiaphragmFaceInsertTP32::~CDiaphragmFaceInsertTP32(void)
	{

	}

	void CDiaphragmFaceInsertTP32::createPointsAndMembers()
	{
		//////////////////////////////////////////////////////////////////////////////////
		////////////////////////////创建节点//////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		int curNum=this->NI();
		int n1=this->N1();
		if (n1<1)
		{
			n1=curNum;
			curNum+=intstep;
		}
		_pSecPointArray=new TowerPointArray();
		CSecondaryPoint tmpN1(n1,Symetry::Ysym,this->N0(),this->N0()+1,2);
		HandleTowerPoint tmpN1SecPoint=tmpN1.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpN1SecPoint);
		int n2=this->N2();
		if (n2<1)
		{
			n2=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpN2(n2,Symetry::Xsym,this->N0(),this->N0()+2,2);
		HandleTowerPoint tmpN2SecPoint=tmpN2.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpN2SecPoint);
		int n3=this->N3();
		if (n3<1)
		{
			n3=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpn3(n3,Symetry::XYsym,this->N0(),n1,2);
		HandleTowerPoint tmpn3SecPoint=tmpn3.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn3SecPoint);
		int n4=this->N4();
		if (n4<1)
		{
			n4=curNum;
			curNum+=intstep;
		}
		CSecondaryPoint tmpn4(n4,Symetry::XYsym,this->N0(),n2,2);
		HandleTowerPoint tmpn4SecPoint=tmpn4.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn4SecPoint);
		int n5=curNum;	
		int n6=n5+intstep;
		int n7=n6+intstep;
		int n8=n7+intstep;
		int n9=n8+intstep;

		CSecondaryPoint tmpn5(n5,Symetry::XYsym,n3,n4,2);
		HandleTowerPoint tmpn5SecPoint=tmpn5.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn5SecPoint);
		CSecondaryPoint tmpn6(n6,Symetry::XYsym,n1,n5,2);
		HandleTowerPoint tmpn6SecPoint=tmpn6.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn6SecPoint);
		CSecondaryPoint tmpn7(n7,Symetry::XYsym,n2,n5,2);
		HandleTowerPoint tmpn7SecPoint=tmpn7.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn7SecPoint);
		CSecondaryPoint tmpn8(n8,Symetry::Ysym,n6,n6+1,2);
		HandleTowerPoint tmpn8SecPoint=tmpn8.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn8SecPoint);
		CSecondaryPoint tmpn9(n9,Symetry::Xsym,n7,n7+2,2);
		HandleTowerPoint tmpn9SecPoint=tmpn9.ToXBSecondaryPoint();
		_pSecPointArray->push_back(tmpn9SecPoint);

		//////////////////////////////////////////////////////////////////////////////////
		////////////////////////////创建杆件//////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		_pMemberArray=new MemberArray();
		size_t MenbCount=_pMemberArray->size();
		_pMemberArray->push_back(new MemberInf(this->N0(),n3,0,Symetry::XYsym,3,20,this->IF1(),this->IF2(),0));
		_pMemberArray->push_back(new MemberInf(n1,n3,0,Symetry::XYsym,3,20,this->IF1(),this->IF2(),0));
		_pMemberArray->push_back(new MemberInf(this->N0(),n4,0,Symetry::XYsym,3,20,this->IS1(),this->IS2(),0));
		_pMemberArray->push_back(new MemberInf(n2,n4,0,Symetry::XYsym,3,20,this->IS1(),this->IS2(),0));
		_pMemberArray->push_back(new MemberInf(n1,n6,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n5,n6,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n4,n5,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n3,n5,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n5,n7,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n2,n7,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n6,n8,0,Symetry::XYsym,3,20,0,200,0));
		_pMemberArray->push_back(new MemberInf(n1,n8,0,Symetry::Ysym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n7,n9,0,Symetry::XYsym,3,20,0,200,0));	
		_pMemberArray->push_back(new MemberInf(n2,n9,0,Symetry::Xsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n3,n6,0,Symetry::XYsym,3,20,1,0,0));
		_pMemberArray->push_back(new MemberInf(n4,n7,0,Symetry::XYsym,3,20,1,0,0));

		_pMemberArray->push_back(new MemberInf(this->N0(),n5,0,Symetry::XYsym,3,20,1,0,0));
	
		this->MKindTransfer();
	}



	void DiaphragmFaceInsertManager::InsertToModel( CTowerModel* TheModel )
	{
		for(vector<StHandle<DiaphragmFaceInfo>>::iterator Iter=_DiaphragmFaceInfos.begin();Iter<_DiaphragmFaceInfos.end();Iter++)
		{
			(*Iter)->InsertToModel(TheModel);
		}
	}

	void DiaphragmFaceInsertManager::CreatePartDiaphragmFace(HandlePart pPart, HdElemArray partElms,const HdMemberArray& pGMemberArray )
	{
		if(partElms->Empty())
			return;
		//分析模板的分层
		NodeArray cornerNodes;
		cornerNodes.push_back(partElms->front()->StartNode());
		for (int iloop = 0; iloop < partElms->size(); ++iloop)
		{
			HandleElement pPartElm = partElms->at(iloop);
			HandleNode pTmpStartNode = pPartElm->StartNode();
			HandleNode pTmpEndNode = pPartElm->EndNode();
			bool NewArray=true;
			for (int iloop = 0; iloop<cornerNodes.size(); ++iloop)
			{
				HandleNode pNode=cornerNodes.at(iloop);
				if(abs(pNode->z()-pTmpStartNode->z())<Constants::DISTANCE_EPSILON0_001 && abs(pNode->z()-pTmpEndNode->z())<Constants::DISTANCE_EPSILON0_001)
				{
					if(pNode->x()<pTmpStartNode->x() || pNode->y()<pTmpStartNode->y())
					{
						cornerNodes[iloop]=pTmpStartNode;
						pNode=pTmpStartNode;
					}
					if(pNode->x()<pTmpEndNode->x() || pNode->y()<pTmpEndNode->y())
					{
						cornerNodes[iloop]=pTmpEndNode;
						pNode=pTmpEndNode;
					}
					NewArray=false;
					break;
				}
			}
			if(NewArray)
			{
				if(pTmpStartNode->x()<pTmpEndNode->x() || pTmpStartNode->y()< pTmpEndNode->y())
					cornerNodes.push_back(pTmpEndNode);
				else
					cornerNodes.push_back(pTmpStartNode);
			}
		}
		int NumCorn=cornerNodes.size();
		//分析每个层上的模板是否是制定的模板
		for (int iloop = 0; iloop < cornerNodes.size(); ++iloop)
		{
			HandleNode pCornerNode = cornerNodes.at(iloop);
			CVec3D corner2Node(-pCornerNode->x(), -pCornerNode->y(), pCornerNode->z());
			CBox3D box3D;
			box3D.extend(pCornerNode.GetPointee());
			box3D.extend(&corner2Node);
			box3D.extend(Constants::DISTANCE_EPSILON);
			HdElemArray tmplElm = pPart->getAllElmArray()->findInBox3D(box3D);
		}		
	}
	
	SmartTower::HandleNode DiaphragmFaceInsertManager::FindNode( std::map<int,HandleNode>* tmpNodeMap ,HandleNode pCoorNode)
	{
		for (std::map<int,HandleNode>::iterator Iter=tmpNodeMap->begin();Iter!=tmpNodeMap->end();++Iter)
		{
			if(Iter->second->CoordEqual(pCoorNode.GetPointee()))
				return Iter->second;
		}
		return NULL;
	}

	int DiaphragmFaceInsertManager::FindMinNum( std::map<int,HandleTowerPoint>* tmpPointMap,int iN0Number )
	{
		int iResult=Constants::MinAuxTagNum;
		for (std::map<int,HandleTowerPoint>::iterator Iter=tmpPointMap->begin();Iter!=tmpPointMap->end();++Iter)
		{
			if(Iter->second->TagNum()!=iN0Number && iResult>Iter->second->TagNum())
				iResult=Iter->second->TagNum();
		}
		return iResult;
	}
	void DiaphragmFaceInsertManager::AddFace( const char* strInfo )
	{
		CSplitStr subStrs;
		subStrs.Split(strInfo);
		if(subStrs.size()>12)
		{
			int iTPIndex=subStrs.intVal(0);
			StHandle<DiaphragmFaceInfo> pDFInfo;
			if(iTPIndex==30)
				pDFInfo=new CDiaphragmFaceInsertTP30();
			else if(iTPIndex==31)
				pDFInfo=new CDiaphragmFaceInsertTP31();
			else if(iTPIndex==32)
				pDFInfo=new CDiaphragmFaceInsertTP32();
			else
				throw exception("成段生成数据样式不正确，请检查");			
			pDFInfo->TP(subStrs.intVal(0));
			pDFInfo->NI(subStrs.intVal(1));
			pDFInfo->M1(subStrs.intVal(2));
			pDFInfo->IF1(subStrs.intVal(3));
			pDFInfo->IF2(subStrs.intVal(4));
			pDFInfo->M2(subStrs.intVal(5));
			pDFInfo->IS1(subStrs.intVal(6));
			pDFInfo->IS2(subStrs.intVal(7));
			pDFInfo->N0(subStrs.intVal(8));
			pDFInfo->N1(subStrs.intVal(9));
			pDFInfo->N2(subStrs.intVal(10));
			pDFInfo->N3(subStrs.intVal(11));
			pDFInfo->N4(subStrs.intVal(12));
			this->push_back(pDFInfo);
		}
	}

	DiaphragmFaceInsertManager::DiaphragmFaceInsertManager()
	{

	}

	DiaphragmFaceInsertManager::~DiaphragmFaceInsertManager()
	{

	}



}