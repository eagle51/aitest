#pragma once
#include <vector>
#include "MemberInf.h"
#include "IntVector.h"

namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月21日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	class MemberArray:public SharedObject
	{ 
	public:
		MemberArray();
		MemberArray(const MemberArray* rhs);
		~MemberArray();
		void push_back(const HandleMemberInf& rhs)
		{
			_MembVec.push_back(rhs);
		}
		void insert(int iPos, const HandleMemberInf& rhs)
		{
			_MembVec.insert(_MembVec.begin() + iPos, rhs);
		}
		int remove(const HandleMemberInf& pMemb);
		void removeAt(int iPos)
		{
			_MembVec.erase(_MembVec.begin() + iPos);
		}
		void removeRange(int iStart, int iCount)
		{
			_MembVec.erase(_MembVec.begin() + iStart, _MembVec.begin() + iStart + iCount);
		}
		int refContains(const HandleMemberInf& rhs) const;
		HandleMemberInf find(int J1, int J2) const;
		//是否交换J1J2
		int contains(int J1,int J2) const;

		int size() const
		{
			return int(_MembVec.size());
		}
		void clear()
		{
			_MembVec.clear();
		}
		const HandleMemberInf& at(int iPos) const
		{
			return _MembVec[iPos];
		}

		const HandleMemberInf& front() const
		{
			return _MembVec.front();
		}

		const HandleMemberInf& back() const
		{
			return _MembVec.back();
		}
		void removeBack()
		{
			_MembVec.pop_back();
		}
		bool empty() const
		{
			return _MembVec.empty();
		}

		void Renumbering(RenumberingList* Renumblist);

		MemberArray* DeepCopy() const;

		MemberArray* getMainArray() const;

		MemberArray* getLegTypeArray() const;

		MemberArray* getOtherArray() const;

		//添加数组
		void addArray(const MemberArray* pArray)
		{
			_MembVec.insert(_MembVec.end(), pArray->_MembVec.begin(), pArray->_MembVec.end());
		}
		//初始输入顺序排序
		void sortByOrderIndex();
        void check();
	private:
		std::vector<HandleMemberInf> _MembVec;
	};
	typedef StHandle<MemberArray> HdMemberArray;
}
