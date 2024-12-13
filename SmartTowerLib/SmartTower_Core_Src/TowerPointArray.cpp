//#include "StdAfx.h"
#include "TowerPointArray.h"
#include "NodeArray.h"

namespace SmartTower
{
	
	TowerPointArray::TowerPointArray()
	{

	}

	TowerPointArray::~TowerPointArray()
	{

	}
	 
	void TowerPointArray::addArray(const TowerPointArray* pArray, bool isInsertBack/* = true*/)
	{
		if (isInsertBack)
			_PointVec.insert(_PointVec.end(), pArray->_PointVec.begin(), pArray->_PointVec.end());
		else
			_PointVec.insert(_PointVec.begin(), pArray->_PointVec.begin(), pArray->_PointVec.end());

	}

	int TowerPointArray::remove(const HandleTowerPoint& pPoint)
	{
		int iPos = this->refContains(pPoint);
		if (iPos < 0)
			return -1;
		_PointVec.erase(_PointVec.begin() + iPos);
		return iPos;
	}

	int TowerPointArray::removeByNum(int tagNum)
	{
		int iPos = this->contains(tagNum);
		if (iPos >=0)
			_PointVec.erase(_PointVec.begin() + iPos);
		return iPos;
	}

	int TowerPointArray::refContains(const HandleTowerPoint& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandleTowerPoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter - _PointVec.begin());
		}
		return -1;
	}

	SmartTower::HandleTowerPoint TowerPointArray::find(int tagNum) const
	{
		if (tagNum<0)
			return NULL;
		for (std::vector<HandleTowerPoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if ((*cIter)->TagNum() == tagNum)
				return *cIter;
		}
		return NULL;
	}

	int TowerPointArray::contains(int tagNum) const
	{
		if (tagNum<0)
			return -1;
		for (std::vector<HandleTowerPoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			if ((*cIter)->TagNum() == tagNum)
				return int(cIter - _PointVec.begin());
		}
		return -1;
	}

	void TowerPointArray::check() const
	{
		for (std::vector<HandleTowerPoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			(*cIter)->Check();
		}
	}

	int TowerPointArray::getAbsPointCount() const
	{
		int icount = 0;
		for (std::vector<HandleTowerPoint>::const_iterator Iter = _PointVec.begin(); Iter < _PointVec.end(); ++Iter)
		{
			if ((*Iter)->GetObjType() == TowBaseObject::PRIPOINT)
			{
				CPrimaryPoint* priPoint = static_cast<CPrimaryPoint*>(Iter->GetPointee());
				if (priPoint->IsAbsoluteCoordinates())
					++icount;
			}
		}
		return icount;
	}

	int TowerPointArray::findMaxTagNum() const
	{
		int maxTagnum = 0;
		for (std::vector<HandleTowerPoint>::const_iterator Iter = _PointVec.begin(); Iter < _PointVec.end(); ++Iter)
		{
			if ((*Iter)->TagNum()>maxTagnum)
				maxTagnum = (*Iter)->TagNum();
		}
		return maxTagnum;
	}

	void TowerPointArray::sortByTagnum()
	{
		std::sort(_PointVec.begin(), _PointVec.end(), TowerPointArray::nodeNumCompare);
	}

	SmartTower::TowerPointArray* TowerPointArray::DeepCopy() const
	{
		TowerPointArray* pNewPointArray = new TowerPointArray();
		pNewPointArray->_PointVec.reserve(_PointVec.size());
		for (std::vector<HandleTowerPoint>::const_iterator Iter = _PointVec.begin(); Iter < _PointVec.end(); ++Iter)
		{
			pNewPointArray->push_back((*Iter)->DeepCopy());
		}
		return pNewPointArray;
	}


	//function ---by raoyi  2022/01/26 18:05
	void TowerPointArray::Renumbering(RenumberingList* Renumblist)
	{
		for (std::vector<HandleTowerPoint>::const_iterator cIter = _PointVec.begin(); cIter < _PointVec.end(); ++cIter)
		{
			(*cIter)->Renumbering(Renumblist);
		}
	}

}