#include "PointArray.h"
#include <list>
#include "PointMap.h"
#include "ExceptionMessage.h"
#include "PointErrException.h"

namespace SmartPoleCore
{
	int PointArray::remove(const HandlePoint& pPoint)
	{
		int iPos = this->refContains(pPoint);
		if (iPos < 0)
			return -1;
		_PointVec.erase(_PointVec.begin() + iPos);
		return iPos;
	}

	int PointArray::refContains(const HandlePoint& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandlePoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end();++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter-_PointVec.begin());
		}
		return -1;
	}

	SmartPoleCore::HandlePoint PointArray::hasPoint(int tagNum) const
	{
		if (tagNum<0 || tagNum>Constants::SysMaxTagNum)
			return NULL;
		for (std::vector<HandlePoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if ((*cIter)->TagNum() == tagNum)
				return *cIter;
		}
		return NULL;
	}

	int PointArray::contains(int tagNum) const
	{
		if (tagNum<0 || tagNum>Constants::SysMaxTagNum)
			return -1;
		for (std::vector<HandlePoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if ((*cIter)->TagNum() == tagNum)
				return int(cIter - _PointVec.begin());
		}
		return -1;
	}

	void PointArray::initAndCheck(PointMap* pointMap)
	{
		std::list<HandlePoint> tempList;
		tempList.insert(tempList.end(), _PointVec.begin(), _PointVec.end());
		size_t loopflag = 0;
		while (!tempList.empty())
		{
			HandlePoint temppPoint2 = tempList.front();
			tempList.pop_front();
			if (temppPoint2->CalcCoor(pointMap))
			{
				loopflag = 0; //已经加入的标记
				pointMap->Insert(temppPoint2);
			}
			else
			{
				//没有计算出结果，放在最后计算
				tempList.push_back(temppPoint2);
				loopflag++;
				if (loopflag >= tempList.size())
				{
					for (std::list<HandlePoint>::iterator errIter = tempList.begin(); errIter != tempList.end(); errIter++)
						ExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(*errIter, PointErrException::RefPointErr));
					break;
				}
			}
		}
	}

	PointArray::PointArray(void)
	{

	}

	PointArray::~PointArray(void)
	{

	}

	SmartPoleCore::HandlePoint PointArray::FindEqual(const CVec3D* pPoint)
	{
		for (std::vector<HandlePoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if (pPoint->CoordEqual(cIter->GetPointee()))
				return *cIter;
		}
		return NULL;
	}

	SmartPoleCore::CBox3D PointArray::getBox() const
	{
		CBox3D result;
		for (std::vector<HandlePoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
			result.extend(cIter->GetPointee());
		return result;
	}

	PointArray* PointArray::getRangePointArray(int startTag, int endTag) const
	{
		PointArray* pResultArray = new PointArray();
		for (std::vector<HandlePoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if ((*cIter)->TagNum() >= startTag && (*cIter)->TagNum() <= endTag)
				pResultArray->add(*cIter);
		}
		return pResultArray;
	}

	bool PointArray::hasFixedPoint() const
	{
		for (std::vector<HandlePoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if ((*cIter)->isFixed())
				return true;
		}
		return false;
	}

	SmartPoleCore::HandlePoint PointArray::getFixedPoint() const
	{
		for (std::vector<HandlePoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if ((*cIter)->isFixed())
				return *cIter;
		}
		return NULL;
	}

	SmartPoleCore::HandlePoint PointArray::findCoincidentPoint() const
	{
		for (std::vector<HandlePoint>::const_iterator iIter = _PointVec.begin(); iIter < _PointVec.end(); ++iIter)
		{
			for (std::vector<HandlePoint>::const_iterator jIter = iIter+1; jIter < _PointVec.end(); ++jIter)
			{
				if ((*iIter)->CoordEqual(jIter->GetPointee()))
					return *iIter;
			}
		}
		return NULL;
	}
	// @ function:   --- by lsq 2020/4/17 15:42
	void PointArray::setDrawZ(double maxZ)
	{
		for (std::vector<HandlePoint>::const_iterator iIter = _PointVec.begin(); iIter < _PointVec.end(); ++iIter)
		{
			(*iIter)->setDrawZ(maxZ - (*iIter)->z());
		}
	}


}