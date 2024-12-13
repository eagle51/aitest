/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2020.1.6
//Description:	最大允许摇摆角报告基类
/****************************************************************/
#pragma once
#include "Report.h"
#include "DataUI_MaxAngle.h"

using namespace SmartCl;
using namespace std;

class Report_MaxAngle:public Report
{
public:
	Report_MaxAngle(const string & clName);
	virtual ~Report_MaxAngle();

private:
	virtual void writeContent(QString &html) const;//写具体内容

	void baseInfo(QString &html) const;//杆塔概况
	virtual void insulatorInfo(QString &html) const = 0;//绝缘子串信息
	virtual void gapInfo(QString &html) const = 0;//间隙值信息
	virtual void angleRes(QString &html) const = 0;//摇摆角计算结果

protected:
	HdlDataUI_MaxAngle          _dataUI;//数据基类

	HdlMdl_BasicPara           _basicInfo;//基本信息
};