#include "measureUtil.h"
#include "OsgDrawEnt.h"
#include "J3DModelData.h"
#include <math.h>

namespace Measure{

	Util::Util()
	{
	}

	Util::~Util()
	{
	}

	double Util::PointToSegDist(const JPoint& pt, const JPoint& ptStart, const JPoint& ptEnd)
	{
		double k = partitionInElem(pt, ptStart, ptEnd); // 杆件上的分点: ptInElem=_startPt+(_endPt-_start)*k
		JPoint tempPt;
		if (k <= 0 || k >= 1){ // 点在杆件之外
			tempPt = k <= 0 ? (ptStart) : (ptEnd);
		}
		else{ // 点在杆件上
			Vec3D vec = ptEnd - ptStart;
			tempPt = Vec3D(ptStart) + (vec.multiNum(k));
		}
		return tempPt.distanceTo(&pt);
	}



	double Util::partitionInElem(const JPoint& point, const JPoint& ptStart, const JPoint& ptEnd)
	{ // 参考:https://blog.csdn.net/theArcticOcean/article/details/50616240

		JPoint vec31 = point - (ptStart);
		JPoint vec21 = ptEnd - ptStart;

		return vec31.dot(vec21) / vec21.dot(vec21);
	}

	// @ funtion:    --- by lt 2019/8/22 14:28
	double Util::PointToRegionDist(const JPoint& pt, const std::vector<JPoint>& ptCollection)
	{
		if (ptCollection.size() != 4) return -1;
		JPoint projectionPt = getProjectionPoint(pt, ptCollection);
		if (isInArea(projectionPt, ptCollection))
		{
			return projectionPt.distanceTo(&pt);
		}
		else
		{
			std::vector<int> unlessIndexCollection;
			JPoint nearestPt = getNearestPt(projectionPt, ptCollection, unlessIndexCollection);
			JPoint secondNearPt = getNearestPt(projectionPt, ptCollection, unlessIndexCollection); // 排除最近点取第二近的点
			return PointToSegDist(projectionPt, nearestPt, secondNearPt);
		}
	}
	// @ funtion:    --- by lt 2019/8/22 15:15
	SmartCl::JPoint Util::getNearestPt(const JPoint& pt, const std::vector<JPoint>& ptCollection, std::vector<int>& unlessIndex)
	{
		int nearestIndex = 0;
		for (size_t i = 0; i < ptCollection.size(); i++)
		{
			if (std::find(unlessIndex.begin(), unlessIndex.end(), i) != unlessIndex.end()){
				nearestIndex = i;
				break;
			}
		}

		JPoint nearestPt = ptCollection[nearestIndex];
		double nearestDist = pt.distanceTo(&nearestPt);

		for (int i = 0; i < ptCollection.size(); i++)
		{
			if (std::find(unlessIndex.begin(), unlessIndex.end(), i) != unlessIndex.end() &&
				pt.distanceTo(&ptCollection[i]) < nearestDist){
				nearestDist = pt.distanceTo(&ptCollection[i]);
				nearestIndex = i;
			}
		}

		unlessIndex.push_back(nearestIndex);
		nearestPt = ptCollection[nearestIndex];
		return nearestPt;
	}
	// @ funtion:    --- by lt 2019/8/22 15:33
	bool Util::isInArea(const JPoint& pt, const std::vector<JPoint>& ptCollection)
	{
		Vec3D P1_P = pt - ptCollection[0];
		Vec3D P1_P2 = ptCollection[1] - ptCollection[0];
		Vec3D P3_P4 = ptCollection[3] - ptCollection[2];
		Vec3D P3_P = pt - ptCollection[2];
		Vec3D P1_P4 = ptCollection[3] - ptCollection[0];
		Vec3D P3_P2 = ptCollection[1] - ptCollection[2];

		return P1_P.cross(P1_P2).dot(P3_P4.cross(P3_P)) >= 0 && P1_P4.cross(P1_P).dot(P3_P2.cross(P3_P)) >= 0;
	}
	// @ funtion:    --- by lt 2019/8/22 15:33
	SmartCl::JPoint Util::getProjectionPoint(const JPoint& pt, const std::vector<JPoint>& ptCollection)
	{//参考文档 https://www.cnblogs.com/nobodyzhou/p/6145030.html
		/*平面由法向量n→(a, b, c), 平面上的一点O(x0, y0, z0) 所确定，只要n→ = / 0 ，确定的平面就是唯一的。
			任一点Vi(x, y, z), 在平面的投影点为 Vi′(x′, y′, z′) */
		//t = ax0 + by0 + cz0−(ax + by + cz)/a2 + b2 + c2
		JPoint projectionPt;
		Vec3D P1_P2 = ptCollection[1] - ptCollection[0];
		Vec3D P1_P4 = ptCollection[3] - ptCollection[0];

		Vec3D normal = P1_P2.cross(P1_P4);

		double t = (normal.getX()*ptCollection[0].getX() + normal.getY()*ptCollection[0].getY() + normal.getZ()*ptCollection[0].getZ()
			- (normal.getX()*pt.getX() + normal.getY()*pt.getY() + normal.getZ()*pt.getZ()))
			/ (pow(normal.getX(), 2) + pow(normal.getY(), 2), pow(normal.getZ(), 2));

		projectionPt = JPoint(pt.getX() + normal.getX()*t, pt.getY() + normal.getY()*t, pt.getZ() + normal.getZ()*t);
		return projectionPt;
	}
	// @ funtion:    --- by lt 2019/9/6 9:59
	double Util::PointToPointDist(const JPoint& pt1, const JPoint& pt2)
	{
		return pt1.distanceTo(&pt2);
	}
	// @ funtion:    --- by lt 2019/9/6 10:51
	SmartCl::JPoint Util::OsgPtToJPoint(osg::Vec3 pt)
	{
		return JPoint(pt.x(), pt.y(), pt.z());
	}
}