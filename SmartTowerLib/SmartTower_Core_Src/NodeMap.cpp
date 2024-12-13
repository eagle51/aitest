//#include "StdAfx.h"
#include "NodeMap.h"
#include "ExceptionMessage.h"

namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月29日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	NodeMap::NodeMap(void)
	{
	} 

	NodeMap::~NodeMap(void)
	{
	}

	void NodeMap::InsertSingle(const HandleNode &addNode,Symetry::Type iIsym )
	{
		if(addNode.isNull())
			return;
		pair<std::map<int, HandleNode>::iterator, bool> existFlag = _pNodeMap.insert(pair<int, HandleNode>(addNode->TagNum(), addNode));
		if(existFlag.second==false)
		{
			HandleTowerPoint errObject=addNode->GetOriginNum();
			HandleTowerPoint pOldObject=existFlag.first->second->GetOriginNum();
			PointErrException::ErrType tmpType=PointErrException::IsymErr;
			if(pOldObject.GetPointee()!=errObject.GetPointee())
				tmpType=PointErrException::DuplicateNum;
			HandleTException ERRObj=new PointErrException(errObject,tmpType);
			CExceptionMessageRegister::Instance()->SendErrObject(ERRObj);
		}
	}

	void NodeMap::InsertFull(const HandleNode &addNode)
	{
		if (addNode.isNull())
			return;
		HandleTowerPoint insertPoint = addNode->GetOriginNum();
		this->InsertSingle(addNode, Symetry::None);
		if (insertPoint->Isym() == Symetry::Xsym || insertPoint->Isym() == Symetry::XYsym)
		{
			HandleNode tempAddHDNode = addNode->getISNode(Symetry::Xsym);
			this->InsertSingle(tempAddHDNode, Symetry::Xsym);
		}
		if (insertPoint->Isym() == Symetry::Ysym || insertPoint->Isym() == Symetry::XYsym)
		{
			HandleNode tempAddHDNode = addNode->getISNode(Symetry::Ysym);
			this->InsertSingle(tempAddHDNode, Symetry::Ysym);
		}
		if (insertPoint->Isym() == Symetry::Osym || insertPoint->Isym() == Symetry::XYsym)
		{
			HandleNode tempAddHDNode = addNode->getISNode(Symetry::Osym);
			this->InsertSingle(tempAddHDNode, Symetry::Osym);
		}
	}


	//function ---by raoyi  2022/01/26 18:03
	void NodeMap::AddToNodeArray(NodeArray* pNodes)
	{
		if (pNodes == NULL)
			return;

		for (std::map<int, HandleNode>::const_iterator iter = _pNodeMap.begin(); iter != _pNodeMap.end(); ++iter)
			pNodes->push_back(iter->second);
	}

}