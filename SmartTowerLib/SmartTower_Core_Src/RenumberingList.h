#pragma once
#include <map>
using namespace std;
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月29日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	//包括派生节点号
	class RenumberingList
	{
	public:
		int FindNumTag(int tagnum) const
		{
			std::map<int, int>::const_iterator iter = _NumList.find(tagnum);
			if(iter==_NumList.end())
				return -1;
			return iter->second;
		} 
		void AddOrChangePair(int iOldTagNum,int iNewTagNum)
		{
			std::map<int,int>::iterator iter=_NumList.find(iOldTagNum);
			if(iter==_NumList.end())
				_NumList.insert(pair<int,int>(iOldTagNum,iNewTagNum));
			else
				iter->second=iNewTagNum;
		}
		int getCount() const
		{
			return int(_NumList.size());
		}
		int MainCount() const;
		//取方向，新的变旧的，旧的变新的。
		void reversed();
		int newMainMaxNumber() const;
		bool CheckAllEqual() const;
		void removeRange(int startNum,int endNum);
		void clear()
		{
			_NumList.clear();
		}
		RenumberingList(void);
		~RenumberingList(void);
	private:
		std::map<int,int> _NumList;
	};

}
