/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.8.1
//Description:	生成报告-直线塔V串
/****************************************************************/
#pragma once
#include "Report_Straight.h"
#include "Mdl_GapValuePara_VL.h"
#include "Mdl_InsultorPara_V.h"
#include "Mdl_InsulaGeoPara_V.h"

using namespace SmartCl;

class Report_Straight_V : public Report_Straight
{
public:
	Report_Straight_V(const string & clName);
	~Report_Straight_V();

	virtual void wireInfo(QString &html) const;//导线信息
	virtual void insulatorInfo(QString &html) const;//绝缘子串信息
	virtual void gapInfo(QString &html) const;//间隙值信息
	virtual void swingAngleRes(QString &html) const;//摇摆角计算结果

private:
	void swingAngleRes_Code2010(QString &html) const;//摇摆角计算结果 2010规范
	void swingAngleRes_Code2020(QString &html) const;//摇摆角计算结果 2020规范

private:
	HdlMdl_GapValuePara_VL     _gapValuePara_VL;//V 串四种工况对应的间隙值参数
	HdlMdl_InsultorPara_V      _insultorPara_V;//绝缘子串参数
	HdlMdl_InsulaGeoPara_V     _insulaGeoPara_V;//绝缘子串几何参数
};