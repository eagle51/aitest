//#include "StdAfx.h"
#include "LegPart.h"
#include <algorithm>
#include <functional>
#include "ExceptionMessage.h"
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月27日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有 
	/////////////////////////////////////
	/////////////////////////////////////

	CLegPart::CLegPart(int iJmin, int iFmin, int iFmax, int iAuxMAX, LegPartLB iLB, int iVar, int startCaseNum, int endCaseNum) :
		  CBasePart(iJmin,iFmin,iFmax,iAuxMAX,iLB,iVar),m_iStartCaseNum(startCaseNum),m_iEndCaseNum(endCaseNum)
	{
		m_Jmin=iJmin;
		m_Var.SetIntOrigin(iVar);
	}
	CLegPart::~CLegPart(void)
	{
	};


	void CLegPart::initial(NodeArray *pGPointSet, ElemArray *pGLink)
	{
		int comMaxNum = m_Fmin;
		if (m_extendNum > m_Jmin && m_extendNum < m_Fmin)
			comMaxNum = m_extendNum;
		//杆件分离
		_bodyNodeArray = pGPointSet->binaryRange(m_upFmax + 1, comMaxNum);
		_bodyElmArray = pGLink->BinaryRange(m_upFmax + 1, comMaxNum);

		_fdtNodeArray = pGPointSet->binaryRange(m_Fmin, m_Fmax + 1);
		_fdtElmArray = pGLink->BinaryRange(m_Fmin, m_Fmax + 1);

		if (m_extendNum > m_Jmin && m_extendNum < m_Fmin)
		{//公共段
			_extendNodeArray=pGPointSet->binaryRange(comMaxNum, m_Fmin);
			_extendElmsArray = pGLink->BinaryRange(comMaxNum, m_Fmin);
		}
		_auxNodeArray = pGPointSet->binaryRange(m_UpAuxTagMax + 1, m_AuxMAX + 1);
		_auxElmArray = pGLink->BinaryRange(m_UpAuxTagMax + 1, m_AuxMAX + 1);

		//全部节点，全部杆件
		_allNodeArray = pGPointSet->binaryRange(m_upFmax + 1, m_Fmax + 1);
		_allElmArray = pGLink->BinaryRange(m_upFmax + 1, m_Fmax + 1);
	}

	int CLegPart::getMaxZTag() const
	{
		double dMaxZ=-1000.0;
		int maxTag=-1;
		for (int iloop = 0; iloop < _fdtNodeArray->size();++iloop)
		{
			HandleNode pTpNode = _fdtNodeArray->at(iloop);
			if (dMaxZ<pTpNode->z())
			{
				dMaxZ = pTpNode->z();
				maxTag = pTpNode->TagNum();
			}
		}
		return maxTag;
	}

	double CLegPart::GetMaxZ() const
	{
		double dMaxZ=-1000.0;
		if (_fdtNodeArray.isNull() || _fdtNodeArray->empty())
			return dMaxZ;
		for (int iloop = 0; iloop < _fdtNodeArray->size(); ++iloop)
		{
			HandleNode pTpNode = _fdtNodeArray->at(iloop);
			if (dMaxZ<pTpNode->z())
				dMaxZ = pTpNode->z();
		}
		return dMaxZ;
	}

	int CLegPart::MaxActiveAuxNum() const
	{
		int iMinNum=this->UpAuxTagMax()+20;
		if (!_auxNodeArray->empty())
			iMinNum = _auxNodeArray->back()->TagNum();
		return iMinNum;
	}

	HandleNode CLegPart::getUpFdtPoint( double dZCoor )
	{
		if (_fdtNodeArray->empty())
			return NULL;
		HandleNode pNode = _fdtNodeArray->front();
		double disZ = pNode->z() - dZCoor;
		if(disZ>0)
			disZ=-1000.0;
		for (int iloop = 0; iloop < _fdtNodeArray->size(); ++iloop)
		{
			HandleNode pTpNode = _fdtNodeArray->at(iloop);
			double tmpZDis = pTpNode->z() - dZCoor;//点距离期望的Z坐标差
			if( tmpZDis<Constants::DISTANCE_EPSILON0_001 && tmpZDis>disZ)//必须小于期望坐标，大于初值
			{
				disZ=tmpZDis;
				pNode = pTpNode;
			}
		}
		return pNode;
	}

	HandleNode CLegPart::getDownFdtPoint( double dZCoor )
	{
		if (_fdtNodeArray->empty())
			return NULL;
		HandleNode pNode = _fdtNodeArray->front();
		double disZ = pNode->z() - dZCoor;
		if(disZ<0)
			disZ=1000.0;
		for (int iloop = 0; iloop < _fdtNodeArray->size(); ++iloop)
		{
			HandleNode pTpNode = _fdtNodeArray->at(iloop);
			double tmpZDis = pTpNode->z() - dZCoor;//点距离期望的Z坐标差
			if( tmpZDis>-Constants::DISTANCE_EPSILON0_001 && tmpZDis<disZ)//必须小于期望坐标，大于初值
			{
				disZ=tmpZDis;
				pNode = pTpNode;
			}
		}
		return pNode;
	}

	SmartTower::HandleNode CLegPart::getExistLegUpNode(int fdtNum) const
	{
		if (_fdtElmArray.isNull() || _fdtElmArray->Empty())
			return NULL;
		HandleElement pElm = _fdtElmArray->getFdtLegElm(fdtNum);
		if (pElm.isNull())
			return NULL;
		return pElm->StartNode();
	}

	SmartTower::HandleNode CLegPart::getMayLegUpNode() const
	{
		if (!_fdtElmArray.isNull() && !_fdtElmArray->Empty())
		{
			HandleElement pElm = _fdtElmArray->getFdtLegElm();
			if (!pElm.isNull())
				return pElm->StartNode();
		}

		HandleNode resNode;
		if (!_bodyElmArray.isNull() && !_bodyElmArray->Empty())
			resNode = _bodyElmArray->findMayLegUpNode(m_Fmin);

		if (!resNode.isNull() || _cnnPart==NULL)
			return resNode;

		HdElemArray pCnnBodyElms = _cnnPart->getAllElmArray();
		if (!pCnnBodyElms.isNull() && !pCnnBodyElms->Empty())
			resNode = pCnnBodyElms->findMayLegUpNode(m_Fmin);

		return resNode;
	}

	SmartTower::HandleNode CLegPart::findMayBracingUpNode(const HandleNode& upKeyNode, bool bFornt) const
	{
		if (upKeyNode.isNull())
			return NULL;
		HandleNode pResNode = this->getAllElmArray()->findMidAxisNode(upKeyNode, bFornt);
		if (pResNode.isNull() && _cnnPart!=NULL)
			pResNode = _cnnPart->getAllElmArray()->findMidAxisNode(upKeyNode, bFornt);
		return pResNode;
	}

}
