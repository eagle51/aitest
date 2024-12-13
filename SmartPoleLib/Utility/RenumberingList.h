#pragma once
#include <map>
using namespace std;
namespace SmartPoleCore
{
	//包括派生节点号
	class RenumberingList
	{
	public:
		int FindNumTag(int tagnum)
		{
			std::map<int,int>::iterator Iter=_NumList.find(tagnum);
			if(Iter==_NumList.end())
				return -1;
			return Iter->second;
		}
		void AddOrChangePair(int OldTagNum,int NewTagNum)
		{
			std::map<int,int>::iterator Iter=_NumList.find(OldTagNum);
			if(Iter==_NumList.end())
				_NumList.insert(pair<int,int>(OldTagNum,NewTagNum));
			else
				Iter->second=NewTagNum;
		}

		int GetCount() const
		{
			return int(_NumList.size());
		}
		//取方向，新的变旧的，旧的变新的。
		void Reversed();

		int GetMainMaxNumber() const;

		bool CheckAllEqual() const;

		void RemoveRange(int startNum,int endNum);

		RenumberingList(void);
		~RenumberingList(void);
	private:
		std::map<int,int> _NumList;
	};

}
