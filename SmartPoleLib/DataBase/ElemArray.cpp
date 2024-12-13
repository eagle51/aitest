#include "ElemArray.h"



namespace SmartPoleCore
{
	ElemArray::ElemArray()
	{
	}


	ElemArray::~ElemArray()
	{
	}

	int ElemArray::Remove(const HandleElement& pPoint)
	{
		int iPos = this->RefContains(pPoint);
		if (iPos < 0)
			return -1;
		_ElemVec.erase(_ElemVec.begin() + iPos);
		return iPos;
	}

	int ElemArray::RefContains(const HandleElement& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter - _ElemVec.begin());
		}
		return -1;
	}

	int ElemArray::Contains(int j1, int j2) const
	{
		int minNum = j1;
		int maxNum = j2;
		if (minNum > maxNum)
			std::swap(minNum, maxNum);
		if (minNum<0 || minNum>Constants::SysMaxTagNum || maxNum<0 || maxNum>Constants::SysMaxTagNum)
			return -1;

		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			if ((*cIter)->minNum() == minNum && (*cIter)->maxNum() == maxNum)
				return int(cIter - _ElemVec.begin());
		}
		return -1;
	}

	SmartPoleCore::CBox3D ElemArray::GetBox() const
	{
		CBox3D result;
		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			HandleNode pStartNode = (*cIter)->StartNode();
			HandleNode pEndNode = (*cIter)->EndNode();
			result.extend(pStartNode.GetPointee());
			result.extend(pEndNode.GetPointee());
		}
		return result;
	}



}
