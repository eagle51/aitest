/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.8.1
//Description:	生成报告基类
/****************************************************************/
#pragma once
#include "Report_MaxAngle.h"
#include "Mdl_GapValuePara_VL.h"
#include "Mdl_InsulaGeoPara_V.h"
#include "DataUI_MaxAngle_V.h"

using namespace SmartCl;
using namespace std;

class Report_MaxAngle_V :public Report_MaxAngle
{
public:
	Report_MaxAngle_V(const string &clName);
	virtual ~Report_MaxAngle_V();

private:
	virtual void gapInfo(QString &html) const;//间隙值信息
	virtual void insulatorInfo(QString &html) const;//绝缘子串信息
	virtual void angleRes(QString &html) const;//角度计算结果

private:
	HdlDataUI_MaxAngle_V         _dataUI_MaxAngle_V;//界面数据类

	HdlMdl_GapValuePara_VL       _gapValuePara_V;//V 串四种工况对应的间隙值参数
	HdlMdl_InsulaHangPara_V     _insulaHangPara_V;//绝缘子串几何参数

};