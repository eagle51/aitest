/****************************************************************/
//Author: 马梓淇
//Version: 	1.0
//Date:		2019.9.11
//Description:	直线塔I串 最大摇摆角绘图
/****************************************************************/
#pragma once
#include "Cl_MaxAngle_I.h"
#include "DrawMaxAngle.h"

using namespace SmartCl;

class DrawMaxAngle_I : public DrawMaxAngle
{
public:
	DrawMaxAngle_I();
	~DrawMaxAngle_I();

	//绘制主流程
	void drawMaxAngle_I(const QString& clName);


private:
	void draw(HdlDataUI_MaxAngle_I dataUI_MaxAngle_I);

	//计算放电点点位置
	ElecPointAndRotaCenter calcElecPoint(HdlMdl_InsulaHangPara_I insulaHangPara_I,/*绝缘子串几何参数*/
										 OdGePoint3d shiftDirection/*平移点向量*/);

	//绘制绝缘子串
	void drawInsulator(HdlMdl_InsulaHangPara_I insulaHangPara_I,/*绝缘子串几何参数*/
					   double angle,/*转动角度*/
					   OdGePoint3d shiftDirection,/*平移点向量*/
					   GapCate::Type gapCateType/*工况类型*/);

};