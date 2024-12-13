/****************************************************************/
//Author: 马梓淇
//Version: 	1.0
//Date:		2019.9.11
//Description:	直线塔 最大摇摆角绘图
/****************************************************************/
#pragma once
#include "DrawAutility.h"
#include "DrawCl.h"
#include "qstring.h"
#include "Cl_MaxAngle_I.h"

using namespace SmartCl;

class DrawMaxAngle : public DrawCl
{
public:
	DrawMaxAngle();
	~DrawMaxAngle();

protected:
	//绘制铁塔模型
	std::list<OdDbObjectId> drawTowerModel(const HdlJ3DModelData towerModelData,/*铁塔模型数据*/
										   OdGePoint3d shiftDirection/*平移点向量*/);

	//计算点集到 线段集的最小距离
	double calcDistance(const std::list<OdDbObjectId> &towerLineList, /*铁塔线段 ID list*/
						const std::list<OdGePoint3d> &elecPointList /*放电点 list*/);

protected:
	std::list<OdDbObjectId>   _towerLineList;//铁塔杆件list
	HdlCl                     _Cl;//间隙圆
};