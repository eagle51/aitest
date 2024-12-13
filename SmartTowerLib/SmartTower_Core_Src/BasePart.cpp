//#include "StdAfx.h"
#include "BasePart.h"
#include <algorithm>
#include <functional>
#include "ExceptionMessage.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////


namespace SmartTower
{	
	CBasePart::CBasePart(int iJmin,int iFmin,int iFmax,int iAuxMAX,LegPartLB iLB,int iVar):
		m_Jmin(iJmin), m_Fmin(iFmin), m_Fmax(iFmax), m_AuxMAX(iAuxMAX), m_LB(iLB), m_Var(iVar), m_extendNum(0),
		_isActive(false), m_upFmax(0), _cnnFmin(-1), _cnnFmax(-1), _PartIndex(-1), _cnnPart(NULL)
	{
		m_UpAuxTagMax=Constants::MinAuxTagNum-1;
		_TotalHBetaZNuA=0.0;
		_TotalHNuZ=0.0;
	};

	//初始化
	void CBasePart::initial(NodeArray *pGPointSet,ElemArray *pGLink)
	{ 
		m_extendNum = 0;
		//杆件分离
		_bodyNodeArray = pGPointSet->binaryRange(m_upFmax + 1, m_Fmax + 1);
		_auxNodeArray = pGPointSet->binaryRange(m_UpAuxTagMax + 1, m_AuxMAX + 1);

		_bodyElmArray = pGLink->BinaryRange(m_upFmax + 1, m_Fmax + 1);
		_auxElmArray = pGLink->BinaryRange(m_UpAuxTagMax + 1, m_AuxMAX + 1);

		//如果是塔身，就是交界地方的点或者杆件；如果是塔腿就是基础位置的点和杆件
		_fdtNodeArray = pGPointSet->binaryRange(m_Fmin, m_Fmax + 1);//节点重编号中用到
		_fdtElmArray = pGLink->BinaryRange(m_Fmin, m_Fmax + 1);

		_allNodeArray = _bodyNodeArray;
		_allElmArray = _bodyElmArray;
	};

	bool CBasePart::extendNumErr() const
	{
		return this->m_extendNum < 0 || this->m_extendNum>this->FmaxTagNum() || (this->m_extendNum > 0 && this->m_extendNum < this->JminTagNum());
	} 

	bool CBasePart::extendElmErr() const
	{
// 		if (m_extendNum>10)
// 			double asdf = 0;
		return m_extendNum>10 && (_extendElmsArray.isNull() || _extendElmsArray->Empty());
	}

	CBasePart::~CBasePart(void)
	{
	};

}
