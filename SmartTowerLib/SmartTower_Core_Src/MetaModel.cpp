//#include "StdAfx.h"
#include "MetaModel.h"
#include "ExceptionMessage.h"
#include "NodeMap.h"
#include "ElementMap.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月24日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	MetaModel::MetaModel(void)
	{
		this->ClearAllInfo();
	}

	MetaModel::~MetaModel(void)
	{
	}

	void MetaModel::AddToList(std::list<HandleTowerPoint>* pList, const HdTowerPointArray& pPointArray)
	{
		if (pPointArray.isNull())
			return;
		for (int iloop = 0; iloop < pPointArray->size(); ++iloop)
			pList->push_back(pPointArray->at(iloop));
	}

	void MetaModel::CalcNodeCoor(NodeMap* pNodesMap, bool bAuxIn)
	{
		std::list<HandleTowerPoint> tempPointList;
		this->AddToList(&tempPointList, _PrimaryPointArray.GetPointee());
		this->AddToList(&tempPointList, _SecondaryPointArray.GetPointee());
		size_t loopflag = 0;
		bool addAuxPointFinish = true;
		while (!tempPointList.empty())
		{
			HandleTowerPoint temppPoint2 = tempPointList.front();
			HandleNode tempNode = temppPoint2->CalcCoor(pNodesMap);
			tempPointList.pop_front();//删除第一个元素
			if (tempNode.isNull())
			{
				//没有计算出结果，放在最后计算
				tempPointList.push_back(temppPoint2);
				loopflag++;
				if (loopflag >= tempPointList.size())
				{
					if (addAuxPointFinish && bAuxIn)
					{
						this->AddToList(&tempPointList, _AuxPointArray.GetPointee());
						addAuxPointFinish = false;
						loopflag = 0;
					}
					else
					{
						for (std::list<HandleTowerPoint>::iterator piter = tempPointList.begin(); piter != tempPointList.end(); piter++)
							CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(*piter, PointErrException::CNNPNotExsit));
						break;
					}
				}
			}
			else
			{
				loopflag = 0;//已经加入的标记
				pNodesMap->InsertFull(tempNode);
			}

			if (tempPointList.empty() && addAuxPointFinish && bAuxIn)
			{//如果主要的全部计算完成后添加辅助点计算
				this->AddToList(&tempPointList, _AuxPointArray.GetPointee());
				addAuxPointFinish = false;
			}

		}

		///////////检查重复杆件/////
		_NodeArray =new NodeArray();		
		pNodesMap->AddToNodeArray(_NodeArray.GetPointee());
		_GobalModelBox = _NodeArray->GetBox3D();	
	}

	void MetaModel::CreateElms(NodeMap* pNodesMap, bool bAuxIn)
	{
		_ElemArray = new ElemArray();
		ElementMap elmMap;
		int icount = 0;
		for (int iloop = 0; iloop < _MenberArray->size(); ++iloop)
		{
			HandleMemberInf pMember = _MenberArray->at(iloop);
			pMember->_OrderIndex = ++icount;
			elmMap.InsertFull(pNodesMap, pMember);
		}
		if (bAuxIn)
		{
			for (int iloop = 0; iloop < _AuxMenberArray->size(); ++iloop)
			{
				HandleMemberInf pMember = _AuxMenberArray->at(iloop);
				pMember->_OrderIndex = ++icount;
				elmMap.InsertFull(pNodesMap, pMember);
			}
		}
		elmMap.TransToElmArray(_ElemArray.GetPointee());
	}

	void MetaModel::CreateNodeAndElms(bool bAuxIn)
	{
		NodeMap nodesMap;
		this->CalcNodeCoor(&nodesMap, bAuxIn);//计算坐标
		this->CreateElms(&nodesMap, bAuxIn);//计算杆件
	}

	void MetaModel::CheckAndUpdata(bool bAuxIn)
	{
		CExceptionMessageRegister::Instance()->setErrEnable(false);
		this->CreateNodeAndElms(bAuxIn);
		CExceptionMessageRegister::Instance()->setErrEnable(true);
	}


	//function ---by raoyi  2022/02/09 14:27
	void MetaModel::ClearAllInfo() 
	{
		_PrimaryPointArray = new TowerPointArray();
		_SecondaryPointArray = new TowerPointArray();
		_AuxPointArray = new TowerPointArray();
		
		_MenberArray = new MemberArray();
		_AuxMenberArray = new MemberArray();

		_NodeArray = new NodeArray();
		_ElemArray = new ElemArray();
	}

}