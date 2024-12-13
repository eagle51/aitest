#pragma once
#include <vector>
#include "SharedObject.h"
#include "TowerPoint.h"
#include "PrimaryPoint.h"
#include "SecondaryPoint.h"
#include "XBSecondaryPoint.h"
namespace SmartTower
{
	class NodeArray;
	class TowerPointArray:public SharedObject
	{
	public:
		TowerPointArray();
		~TowerPointArray();
		const HandleTowerPoint& front() const
		{
			return _PointVec.front();
		}
		const HandleTowerPoint& back() const
		{
			return _PointVec.back();
		}
		void push_back(const HandleTowerPoint& rhs)
		{ 
			_PointVec.push_back(rhs);
		}
		void addArray(const TowerPointArray* pArray, bool isInsertBack = true);
		void insert(int iPos, const HandleTowerPoint& rhs)
		{
			_PointVec.insert(_PointVec.begin() + iPos, rhs);
		}
		int remove(const HandleTowerPoint& pPoint);
		int removeByNum(int tagNum);
		void removeAt(int iPos)
		{
			_PointVec.erase(_PointVec.begin() + iPos);
		}
		void removeRange(int iStart, int iCount)
		{
			_PointVec.erase(_PointVec.begin() + iStart, _PointVec.begin() + iStart + iCount);
		}
		int refContains(const HandleTowerPoint& rhs) const;
		HandleTowerPoint find(int tagNum) const;
		int contains(int tagNum) const;
		int size() const
		{
			return int(_PointVec.size());
		}
		void clear()
		{
			_PointVec.clear();
		}
		const HandleTowerPoint& at(int iPos) const
		{
			return _PointVec[iPos];
		}
		bool empty() const
		{
			return _PointVec.empty();
		}
		void Renumbering(RenumberingList* Renumblist);

		void check() const;

		int getAbsPointCount() const;

		int findMaxTagNum() const;

		void sortByTagnum();
		TowerPointArray* DeepCopy() const;
	private:
		static bool nodeNumCompare(HandleTowerPoint &Point1, HandleTowerPoint &Point2)
		{
			return Point1->TagNum() <= Point2->TagNum();
		}
		std::vector<HandleTowerPoint> _PointVec;
	};
	typedef StHandle<TowerPointArray> HdTowerPointArray;
}