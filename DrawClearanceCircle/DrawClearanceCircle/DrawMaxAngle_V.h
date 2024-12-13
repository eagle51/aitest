/****************************************************************/
//Author: 马梓淇
//Version: 	1.0
//Date:		2019.12.31
//Description:	直线塔V串 最大摇摆角绘图
/****************************************************************/
#pragma once
#include "DrawAutility.h"
#include "DrawCl.h"
#include "qstring.h"
#include "Cl_MaxAngle_V.h"
#include "DrawMaxAngle.h"

using namespace SmartCl;

class DrawMaxAngle_V : public DrawMaxAngle
{
public:
	DrawMaxAngle_V();
	~DrawMaxAngle_V();

	//绘制主流程
	void drawMaxAngle_V(const QString& clName);

private:
	void draw(HdlDataUI_MaxAngle_V dataUI_MaxAngle_V);

	//计算放电点位置
	std::list<OdGePoint3d> calcElecPoint(HdlMdl_InsulaHangPara_V insulaHangPara_V,/*绝缘子串几何参数*/
										 OdGePoint3d shiftDirection,/*平移点向量，用于区分绘制不同工况*/
										 double moveDownDis/*绝缘子串绘制起始点相比两个挂点中点位置的下移距离*/);

	//绘制绝缘子串几何模型及间隙圆，返回V串夹角
	double drawInsulatorAndCircle(HdlMdl_InsulaHangPara_V insulaHangPara_V,/*绝缘子串几何参数*/
								 HdlMdl_GapValuePara_VL gapValuePara_V,/*间隙信息*/
					             OdGePoint3d shiftDirection,/*平移点向量，用于区分绘制不同工况*/
					             double moveDownDis/*绝缘子串绘制起始点相比两个挂点中点位置的下移距离*/);

	//筛选铁塔范围，在铁塔高度方向切割，用于计算上移相切与下移相切
	std::list<OdDbObjectId> filtTowerModel(const std::list<OdDbObjectId> &towerLineList,/*铁塔线 list*/
						                   OdGePoint3d breakPoint,/*分割点*/
										   bool isdeleteDown/*是否为删除分割点下方的铁塔线*/);

};