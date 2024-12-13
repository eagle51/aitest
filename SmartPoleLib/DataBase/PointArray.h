#pragma once
#include <vector>
#include "SharedObject.h"
#include "Point.h"
#include "..\Utility\Box3D.h"

namespace SmartPoleCore
{
	//模仿C#的List包装vector
	//几何模型节点数组
	class PointMap;
	class PointArray:public SharedObject
	{
	public:
		PointArray(void);
		~PointArray(void);
		void add(const HandlePoint& rhs)
		{
			_PointVec.push_back(rhs);
		}
		void insert(int iPos, const HandlePoint& rhs)
		{
			_PointVec.insert(_PointVec.begin() + iPos, rhs);
		}
		int remove(const HandlePoint& pPoint);
		void removeAt(int iPos)
		{
			_PointVec.erase(_PointVec.begin()+iPos);
		}
		void removeRange(int iStart, int iCount)
		{
			_PointVec.erase(_PointVec.begin() + iStart, _PointVec.begin() + iStart + iCount);
		}
		//
		void addArray(const PointArray* pTmpArry)
		{
			_PointVec.insert(_PointVec.end(), pTmpArry->_PointVec.begin(), pTmpArry->_PointVec.end());
		}
		int refContains(const HandlePoint& rhs) const;
		HandlePoint hasPoint(int tagNum) const;
		int contains(int tagNum) const;
		int count() const
		{
			return int(_PointVec.size());
		}
		void clear()
		{
			_PointVec.clear();
		}
		const HandlePoint& getAt(int iPos) const
		{
			return _PointVec[iPos];
		}

		bool empty() const
		{
			return _PointVec.empty();
		}
		HandlePoint FindEqual(const CVec3D* pPoint);
		//计算坐标
		void initAndCheck(PointMap* pointMap);

		PointArray* getRangePointArray(int startTag,int endTag) const;

		bool hasFixedPoint() const;

		HandlePoint getFixedPoint() const;

		CBox3D getBox() const;

		HandlePoint findCoincidentPoint() const;

		void setDrawZ(double maxZ);
	private:
		std::vector<HandlePoint> _PointVec;
	};
	typedef SharedHandle<PointArray> HdPointArray;

}