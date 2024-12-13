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

//@function: ��������ģ��   -- by mzq  2019/9/11 18:03
std::list<OdDbObjectId> DrawMaxAngle::drawTowerModel(const HdlJ3DModelData towerModelData,/*����ģ������*/
													 OdGePoint3d shiftDirection/*ƽ�Ƶ�����*/)
{
	std::list<OdDbObjectId> towerLineList;//�����˼�list

	if (towerModelData->sizeOfTowerModel() > 0)
	{
		for (int iLoop = 0; iLoop < towerModelData->sizeOfTowerModel(); ++iLoop)
		{
			SmartCl::HdlJPoint pStartNode = towerModelData->elemAt(iLoop)->getStartNode();
			SmartCl::HdlJPoint pEndNode = towerModelData->elemAt(iLoop)->getEndNode();

			/*teigha����ϵ��osg��ͬ��teigha���߷���ΪY��osg���߷���ΪZ��������Ҫת��һ������ϵ�� ͬʱ��Ҫ����ƽ��*/
			OdGePoint3d ptStart = OdGePoint3d(pStartNode->getX() + shiftDirection.x, pStartNode->getZ() + shiftDirection.y, 0);//���
			OdGePoint3d ptEnd = OdGePoint3d(pEndNode->getX() + shiftDirection.x, pEndNode->getZ() + shiftDirection.y, 0);//�յ�
			towerLineList.insert(towerLineList.end(), CreateEnt::drawLine(ptStart, ptEnd));
		}

		DrawAutility::filterDupLine(towerLineList);//���˵��ظ���
	}

	return towerLineList;
}

//@function: ����㼯�� �߶μ�����С����   -- by mzq  2019/12/19 20:35
double DrawMaxAngle::calcDistance(const std::list<OdDbObjectId> &towerLineList, /*�����߶� ID list*/
								  const std::list<OdGePoint3d> &elecPointList/*�ŵ�� list*/)
{
	HdlJPoint start = new JPoint();
	HdlJPoint end = new JPoint();
	HdlTowerElem towerElem = new TowerElem();
	double minDistance = 999999;

	std::list<OdGePoint3d>::const_iterator iterPoint = elecPointList.begin();
	for (; iterPoint != elecPointList.end(); iterPoint++)//ѭ�����е�
	{
		JPoint point(iterPoint->x, iterPoint->y, iterPoint->z);
		std::list<OdDbObjectId>::const_iterator iterLine = towerLineList.begin();
		for (iterLine; iterLine != towerLineList.end(); iterLine++)//ѭ�������ߣ�����㵽ֱ�߾���
		{
			OdDbLinePtr towerLine = iterLine->safeOpenObject(OdDb::kForRead);
			if (towerLine->isA()->isEqualTo(OdDbLine::desc()))
			{
				OdGePoint3d pointStart, pointEnd;
				towerLine->getStartPoint(pointStart);
				towerLine->getEndPoint(pointEnd);

				/*ת�����ں��е��ߵ�Ԫ�࣬�������ڼ���㵽ֱ�߾���*/
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