#include "DrawMaxAngle.h"
#include "CreateEnt.h"
#include "J3DModelData.h"
#include "Auxiliary.h"
#include "OdqInterfaces.h"

//@function:    -- by mzq  2019/7/15 10:37
DrawMaxAngle::DrawMaxAngle()
{
	
}

//@function:    -- by mzq  2019/7/15 10:38
DrawMaxAngle::~DrawMaxAngle()
{

}

//@function: 绘制铁塔模型   -- by mzq  2019/9/11 18:03
std::list<OdDbObjectId> DrawMaxAngle::drawTowerModel(const HdlJ3DModelData towerModelData,/*铁塔模型数据*/
													 OdGePoint3d shiftDirection/*平移点向量*/)
{
	std::list<OdDbObjectId> towerLineList;//铁塔杆件list

	if (towerModelData->sizeOfTowerModel() > 0)
	{
		for (int iLoop = 0; iLoop < towerModelData->sizeOfTowerModel(); ++iLoop)
		{
			SmartCl::HdlJPoint pStartNode = towerModelData->elemAt(iLoop)->getStartNode();
			SmartCl::HdlJPoint pEndNode = towerModelData->elemAt(iLoop)->getEndNode();

			/*teigha坐标系与osg不同，teigha塔高方向为Y，osg塔高方向为Z，这里需要转换一下坐标系， 同时需要考虑平移*/
			OdGePoint3d ptStart = OdGePoint3d(pStartNode->getX() + shiftDirection.x, pStartNode->getZ() + shiftDirection.y, 0);//起点
			OdGePoint3d ptEnd = OdGePoint3d(pEndNode->getX() + shiftDirection.x, pEndNode->getZ() + shiftDirection.y, 0);//终点
			towerLineList.insert(towerLineList.end(), CreateEnt::drawLine(ptStart, ptEnd));
		}

		DrawAutility::filterDupLine(towerLineList);//过滤掉重复线
	}

	return towerLineList;
}

//@function: 计算点集到 线段集的最小距离   -- by mzq  2019/12/19 20:35
double DrawMaxAngle::calcDistance(const std::list<OdDbObjectId> &towerLineList, /*铁塔线段 ID list*/
								  const std::list<OdGePoint3d> &elecPointList/*放电点 list*/)
{
	HdlJPoint start = new JPoint();
	HdlJPoint end = new JPoint();
	HdlTowerElem towerElem = new TowerElem();
	double minDistance = 999999;

	std::list<OdGePoint3d>::const_iterator iterPoint = elecPointList.begin();
	for (; iterPoint != elecPointList.end(); iterPoint++)//循环所有点
	{
		JPoint point(iterPoint->x, iterPoint->y, iterPoint->z);
		std::list<OdDbObjectId>::const_iterator iterLine = towerLineList.begin();
		for (iterLine; iterLine != towerLineList.end(); iterLine++)//循环所有线，计算点到直线距离
		{
			OdDbLinePtr towerLine = iterLine->safeOpenObject(OdDb::kForRead);
			if (towerLine->isA()->isEqualTo(OdDbLine::desc()))
			{
				OdGePoint3d pointStart, pointEnd;
				towerLine->getStartPoint(pointStart);
				towerLine->getEndPoint(pointEnd);

				/*转化成内核中的线单元类，方便用于计算点到直线距离*/
				start->setX(pointStart.x); start->setY(pointStart.y); start->setZ(pointStart.z);
				end->setX(pointEnd.x); end->setY(pointEnd.y); end->setZ(pointEnd.z);
				towerElem->setStartNode(start);
				towerElem->setEndNode(end);
				JPoint minDisPointInLine = towerElem->pointInElem(point);
				double distance = minDisPointInLine.distanceTo(&point);

				if (minDistance > distance)
					minDistance = distance;
			}
		}
	}
	return minDistance;
}