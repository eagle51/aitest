#pragma once
#include <vector>
#include "Member.h"
#include "..\Utility\Box3D.h"
namespace SmartPoleCore
{
	//几何模型杆件数组
	class PointMap;
	class MemberArray:public SharedObject
	{
	public:
		MemberArray();
		~MemberArray();
		//
		void add(const HandleMember& rhs)
		{
			_MembVec.push_back(rhs);
		}
		void insert(int iPos, const HandleMember& rhs)
		{
			_MembVec.insert(_MembVec.begin() + iPos, rhs);
		}
		void removeBack()
		{
			_MembVec.pop_back();
		}
		int remove(const HandleMember& pMemb);

		void removeAt(int iPos)
		{
			_MembVec.erase(_MembVec.begin() + iPos);
		}
		void removeRange(int iStart, int iCount)
		{
			_MembVec.erase(_MembVec.begin() + iStart, _MembVec.begin() + iStart + iCount);
		}
		void replace(int iPos,const HandleMember& pMemb)
		{
			_MembVec[iPos] = pMemb;
		}
		void addArray(const MemberArray* pTmpArry)
		{
			_MembVec.insert(_MembVec.end(), pTmpArry->_MembVec.begin(), pTmpArry->_MembVec.end());
		}
		int refContains(const HandleMember& rhs) const;
		int contains(int J1,int J2) const;
		int count() const
		{
			return int(_MembVec.size());
		}
		void clear()
		{
			_MembVec.clear();
		}
		const HandleMember& getAt(int iPos) const
		{
			return _MembVec[iPos];
		}
		const HandleMember& front() const
		{
			return _MembVec.front();
		}
		const HandleMember& back() const
		{
			return _MembVec.back();
		}

		bool empty() const
		{
			return _MembVec.empty();
		}
		//初始化并校验
		void initAndCheck(PointMap* pointMap) const;

		CBox3D GetBox() const;

		void clearProfileForce() const;

		MemberArray* getRangeMemberArray(int startTag, int endTag);

		double getMaxZ(bool isDrawZ=false);
		double getMembsLen();

		void sortByEndZCoor();
	private:
		std::vector<HandleMember> _MembVec;

		static bool endZCoorCompare(HandleMember& pMemb1, HandleMember& pMemb2)
		{
			HandlePoint pMemb1Point = pMemb1->getMaxZPoint();
			HandlePoint pMemb2Point = pMemb2->getMaxZPoint();
			return pMemb1Point->z() < pMemb2Point->z();
		};
		double findAssMemsDia(HandlePoint TagNum, HandleMember memb)const;
		void checkMemVec(PointMap* TagNum, HandleMember memb)const;
	};
	typedef SharedHandle<MemberArray> HdMemberArray;
}
