/****************************************************************/
//Author: 马梓淇
//Version: 	1.0
//Date:		2019.7.15
//Description:	I串绘图
/****************************************************************/
#pragma once
#include "DataDraw_StrightI.h"
#include "DrawAutility.h"
#include "DrawCl.h"
#include "SmartCADTable.h"

using namespace SmartCl;

class DrawStraight_I : public DrawCl
{
public:
	DrawStraight_I();
	~DrawStraight_I();

	//绘制I串主流程
	void drawStraight_I(QString clName/*间隙圆名称*/);

private:
	//绘制绝缘子串几何结构及以及间隙圆
	double drawInsulateGeo_I(HdlDataDraw_StrightI drawData, /*I串绘图相关数据*/
							 OdGePoint3d ptStart/*绘图起点*/);

	//绘制表格
	void drawTableInfo_I(HdlDataDraw_StrightI drawData,/*绘图相关数据*/
						 OdGePoint2d insertPt/*表格左上角点*/);

	//风偏角度方向是否一致
	bool isSwingAngleSameDirection(HdlAngleForFourCate angleForFourCate/*摇摆角参数*/);

private:
	double _sizeScale;
};