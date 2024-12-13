/****************************************************************/
//Author: 马梓淇
//Version: 	1.0
//Date:		2019.7.10
//Description:	VL串绘图
/****************************************************************/
#pragma once
#include "DrawAutility.h"
#include "DataDraw_StrightV.h"
#include "DataDraw_StrightL.h"
#include "DrawCl.h"

using namespace SmartCl;

class DrawStraight_VL : public DrawCl
{
public:
	DrawStraight_VL();
	~DrawStraight_VL();

	//功能：绘制主流程
	void drawStraight_VL(QString clName/*间隙圆名称*/);

private:
	//功能：绘制几何部分
	void drawInsulateGeo_VL(QString clName, /*间隙圆名称*/
							OdGePoint3d ptStart/*起点*/);
	
	//功能：绘制V串表格
	void drawTableInfo_V(HdlDataDraw_StrightV drawData, /*绘图相关数据*/
						 OdGePoint2d insertPt/*表格左上角点*/);

	//功能：绘制L串表格
	void drawTableInfo_L(HdlDataDraw_StrightL drawData, /*绘图相关数据*/
						 OdGePoint2d insertPt/*表格左上角点*/);

};