//#include "StdAfx.h"
#include "LegPartLB.h"

//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	
	 
	LegPartLB::LegPartLB( int allIterFlag/*=1*/,int iFoundationNum/*=0*/,int iBodyPartIndex/*=1*/ )
		:_FoundationNum(iFoundationNum),_BodyPartIndex(iBodyPartIndex)
	{
		AllIterFlag(allIterFlag);
	}


	LegPartLB::LegPartLB( int iLBFlag/*=0*/ )
	{
		LBFlag(iLBFlag);
	}


	LegPartLB::~LegPartLB(void)
	{
	}

	int LegPartLB::getCurrentIterFlag(bool isLastLoop, int onlyHLFlag) const
	{
		if (_allIterFlag != 0)
		{
			if (onlyHLFlag > 3)
				return 2;
			else if (isLastLoop)
				return _allIterFlag;
			else
				return 2;
		}
		return 0;
	}

	const char* LegPartLB::getDaoLBFlag(int curBodyIndex) const
	{
		lbCharStr.clear();
		if(_allIterFlag!=1)
			lbCharStr.push_back('0');
		else
			lbCharStr.push_back('1');
		if(_FoundationNum>9)
			lbCharStr.push_back(char('A'+_FoundationNum-10));
		else
			lbCharStr.push_back(char('0'+_FoundationNum));
		if(curBodyIndex>0)
			lbCharStr.push_back(char('0'+curBodyIndex));
		else
			lbCharStr.push_back(char('0'+_BodyPartIndex));
		return lbCharStr.c_str();
	}

	std::string LegPartLB::lbCharStr="";

	bool LegPartLB::BodyGreaterThan10=false;


}