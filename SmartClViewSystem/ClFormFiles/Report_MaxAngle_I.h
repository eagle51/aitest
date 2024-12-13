/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2020.1.6
//Description:	生成报告基类
/****************************************************************/
#pragma once
#include "Report_MaxAngle.h"
#include "Mdl_GapValuePara_I.h"
#include "Mdl_InsulaGeoPara_I.h"
#include "DataUI_MaxAngle_I.h"

using namespace SmartCl;
using namespace std;

class Report_MaxAngle_I :public Report_MaxAngle
{
public:
	Report_MaxAngle_I(const string &clName);
	virtual ~Report_MaxAngle_I();

private:
	virtual void gapInfo(QString &html) const;          //间隙值信息
	virtual void insulatorInfo(QString &html) const;    //绝缘子串信息
	virtual void angleRes(QString &html) const;         //角度计算结果

private:
	HdlDataUI_MaxAngle_I         _dataUI_MaxAngle_I;//界面数据类

	HdlMdl_GapValuePara_I       _gapValuePara_I;//I 串四种工况对应的间隙值参数
	HdlMdl_InsulaHangPara_I     _insulaHangPara_I;//绝缘子串几何参数

};