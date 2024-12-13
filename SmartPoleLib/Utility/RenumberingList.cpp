#include "RenumberingList.h"
#include "Constants.h"

namespace SmartPoleCore
{
	RenumberingList::RenumberingList(void)
	{
	}


	RenumberingList::~RenumberingList(void)
	{
	}

	void RenumberingList::Reversed()
	{
		std::map<int,int> TmpList;
		for (std::map<int,int>::iterator Iter=_NumList.begin();Iter!=_NumList.end();++Iter)
		{
			TmpList.insert(pair<int,int>(Iter->second,Iter->first));
		}
		_NumList=TmpList;
	}

	int RenumberingList::GetMainMaxNumber() const
	{
		int MaxVal=0;
		for (std::map<int,int>::const_iterator iter=_NumList.begin();iter!=_NumList.end();++iter)
		{
			if (iter->second>MaxVal && iter->second<Constants::SysMaxTagNum)
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

	void RenumberingList::RemoveRange( int startNum,int endNum )
	{
		for (std::map<int,int>::iterator Iter=_NumList.begin();Iter!=_NumList.end();++Iter)
		{
			if(Iter->first>=startNum && Iter->first<endNum)
				Iter->second=-1;
		}
	}
}