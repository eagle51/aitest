/****************************************************************/
//Author: 马梓淇
//Version: 	1.0
//Date:		2019.7.22
//Description:	绘图基类
/****************************************************************/
#pragma once
#include "DataDraw_StrightI.h"
#include "DrawAutility.h"
#include "DataBase.h"
#include "VirBox\VirBoxChecker.h"

class DrawCl
{
public:
	DrawCl();
	virtual ~DrawCl();

	static SmartCl::DataBase* getCoreInstance(); //内核数据库
	static AboutSoftWare::VirBoxChecker* getVirBoxCheckerInstance(); //深思加密检查

protected:
	OdDbObjectId     _idDimStyle;//标注样式
	double           _fontScale;//字体缩放比例
	double           _plottingScale;//比例尺，例如1:100
	double           _sizeScale;//综合 米 转化为 毫米之后，再考虑比例尺后的最终缩放尺寸
	double           _insulatorBaseLengh;//绝缘子串基准长度，用于确定fontScale用

	OdGeVector3d     _vecHorizontalUnit;//水平单位向量
	OdGeVector3d     _vecVerticalUnit;//垂直单位向量
};