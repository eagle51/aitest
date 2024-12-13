//#include "StdAfx.h"
#include "TowerPoint.h"
#include "config.hpp"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	CTowerPoint::CTowerPoint(int iPointNum,Symetry::Type isym)
		:m_iPointNum(iPointNum),m_Isym(isym)
	{
	};
	CTowerPoint::~CTowerPoint(void)
	{
	} 

	bool CTowerPoint::bDxfDimMark() const
	{
		return false;
	}

	bool CTowerPoint::isFoundationPoint() const
	{
		return false;
	}

	void CTowerPoint::setFoundationPointFlag(bool flag)
	{

	}

}