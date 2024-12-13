#pragma once
#include <vector>
#include "SharedHandle.hpp"
#include "BaseElement.h"
#include "Box3D.h"

namespace SmartPoleCore
{
	//有限元单元数组s
	class ElemArray: public SharedObject
	{
	public:
		ElemArray();
		~ElemArray();
		void Add(const HandleElement& rhs)
		{
			_ElemVec.push_back(rhs);
		}
		void Insert(int iPos, const HandleElement& rhs)
		{
			_ElemVec.insert(_ElemVec.begin() + iPos, rhs);
		}
		int Remove(const HandleElement& pPoint);
		void RemoveAt(int iPos)
		{
			_ElemVec.erase(_ElemVec.begin() + iPos);
		}
		void RemoveRange(int iStart, int iCount)
		{
			_ElemVec.erase(_ElemVec.begin() + iStart, _ElemVec.begin() + iStart + iCount);
		}
		void addArray(const ElemArray* pTmpArry)
		{
			_ElemVec.insert(_ElemVec.end(), pTmpArry->_ElemVec.begin(), pTmpArry->_ElemVec.end());
		}
		int RefContains(const HandleElement& rhs) const;
		int Contains(int j1,int j2) const;
		int count() const
		{
			return int(_ElemVec.size());
		}
		void Clear()
		{
			_ElemVec.clear();
		}
		const HandleElement& getAt(int iPos) const
		{
			return _ElemVec[iPos];
		}
		bool Empty() const
		{
			return _ElemVec.empty();
		}
		CBox3D GetBox() const;
	private:
		std::vector<HandleElement> _ElemVec;

	};

	typedef SharedHandle<ElemArray> HdElemArray;

}