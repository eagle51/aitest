
#include "NodeArray.h"

namespace SmartPoleCore
{
	NodeArray::NodeArray()
	{
	}


	NodeArray::~NodeArray()
	{
	}

	int NodeArray::remove(const HandleNode& pPoint)
	{
		int iPos = this->refContains(pPoint);
		if (iPos < 0)
			return -1;
		_NodeVec.erase(_NodeVec.begin() + iPos);
		return iPos;
	}

	int NodeArray::refContains(const HandleNode& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter - _NodeVec.begin());
		}
		return -1;
	}
	
	//是否包含，若包含，在第几行
	int NodeArray::contains(int tagNum) const
	{
		if (tagNum<0 || tagNum>Constants::SysMaxTagNum)
			return -1;
		for (std::vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
		{
			if ((*cIter)->TagNum() == tagNum)
				return int(cIter - _NodeVec.begin());
		}
		return -1;
	}

	SmartPoleCore::CBox3D NodeArray::getBox() const
	{
		CBox3D result;
		for (std::vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
			result.extend(cIter->GetPointee());
		return result;
	}

	SmartPoleCore::HandleNode NodeArray::findByFatherTagNum(int fatherNum) const
	{
		for (std::vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
		{
			const Point* fatherPoint = (*cIter)->getFather();
			if (fatherPoint!=NULL && fatherPoint->TagNum()==fatherNum)
				return *cIter;
		}
		return NULL;
	}

}