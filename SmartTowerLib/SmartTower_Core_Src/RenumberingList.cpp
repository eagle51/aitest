//#include "StdAfx.h"
#include "RenumberingList.h"
#include "Constants.h"

namespace SmartTower
{

	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月29日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	RenumberingList::RenumberingList(void)
	{
	}


	RenumberingList::~RenumberingList(void)
	{
	}

	int RenumberingList::MainCount() const
	{
		if(_NumList.empty())
			return 0;
		int iResult=0;
		int numtmp=-1;
		for (std::map<int,int>::const_iterator iIter=_NumList.begin();iIter!=_NumList.end();++iIter)
		{
			if(iIter->first<Constants::MinAuxTagNum && (iIter->first/10)!=numtmp )
			{	++iResult;
				numtmp=iIter->first/10;
			}
		}
		return iResult;
	}

	void RenumberingList::reversed()
	{
		std::map<int,int> tmpList;
		for (std::map<int,int>::iterator iter=_NumList.begin();iter!=_NumList.end();++iter)
		{
			tmpList.insert(pair<int,int>(iter->second,iter->first));
		}
		_NumList=tmpList;
	}

	int RenumberingList::newMainMaxNumber() const
	{
		int MaxVal=0;
		for (std::map<int,int>::const_iterator iter=_NumList.begin();iter!=_NumList.end();++iter)
		{
			if(iter->second>MaxVal && iter->second<Constants::MinAuxTagNum)
				MaxVal=iter->second;
		}
		return MaxVal;
	}

	bool RenumberingList::CheckAllEqual() const
	{
		for (std::map<int,int>::const_iterator iter=_NumList.begin();iter!=_NumList.end();++iter)
		{
			if(iter->first!=iter->second)
				return false;
		}
		return true;
	}

	void RenumberingList::removeRange( int startNum,int endNum )
	{
		for (std::map<int,int>::iterator Iter=_NumList.begin();Iter!=_NumList.end();++Iter)
		{
			if(Iter->first>=startNum && Iter->first<endNum)
				Iter->second=-1;
		}
	} 

}