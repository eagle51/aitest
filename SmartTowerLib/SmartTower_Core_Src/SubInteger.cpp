//#include "StdAfx.h"
#include "SubInteger.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月30日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	long CSubInteger::a[]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};  
	void CSubInteger::SetSubInt(const size_t iPosition,const size_t must0_9)
	{ 
		int oInt=GetSubInt(iPosition);
		m_Int+=(must0_9-oInt)*a[iPosition];
	}
	CSubInteger::~CSubInteger(void)
	{
	}
}