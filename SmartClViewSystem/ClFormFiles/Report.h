/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.8.1
//Description:	生成报告基类
/****************************************************************/
#pragma once
#include <string>
#include "Clearence.h"
#include "qstring.h"

using namespace SmartCl;
using namespace std;

class Report
{
public:
	Report(const string &clName);
	virtual ~Report();

	void wordReport() const;//生成报告

private:
	virtual void writeContent(QString &html) const = 0;//写具体内容内容
	void firstPage(QString &html) const;//封皮第一页

protected:
	HdlCl                      _cl;//间隙圆

	double                     _fontSize_s2;//小二号
	double                     _fontSize_s3;//小三号
	double                     _fontSize_4;//四号
	double                     _fontSize_s4;//小四号
	double                     _fontSize_5;//五号
};