#pragma once
#include "osg\Array"
#include "JPoint.h"
#include <array>

using namespace SmartCl;

namespace Measure{

	class Util
	{
	public:
		static double PointToPointDist(const JPoint& pt1, const JPoint& pt2);
		static double PointToSegDist(const JPoint& pt, const JPoint& ptStart, const JPoint& ptEnd);
		static double PointToRegionDist(const JPoint& pt, const std::vector<JPoint>& ptCollection);
		static JPoint OsgPtToJPoint(osg::Vec3 pt);
	private:
		Util();
		~Util();
		static double partitionInElem(const JPoint& pt, const JPoint& ptStart, const JPoint& ptEnd);
		static JPoint getNearestPt(const JPoint& pt, const std::vector<JPoint>& ptCollection, std::vector<int>& unlessIndex);
		static bool isInArea(const JPoint& pt, const std::vector<JPoint>& ptCollection);
		static JPoint getProjectionPoint(const JPoint& pt, const std::vector<JPoint>& ptCollection);
	};
}