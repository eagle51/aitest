/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.8.1
//Description:	生成报告基类
/****************************************************************/
#pragma once
#include "Report.h"
#include "Mdl_BasicPara.h"
#include "Mdl_CalcRelePara.h"
#include "Mdl_SpanPara.h"
#include "Mdl_VSpanCalSet.h"
#include "Mdl_MetaAreaPara.h"
#include "Mdl_WireRelatPara.h"
#include "Mdl_InsultorPara_I.h"
#include "DataUI_Stright.h"
#include "MechCalProPara.h"
#include "SwingAnglePara.h"

using namespace SmartCl;
using namespace std;

class Report_Straight:public Report
{
public:
	Report_Straight(const string & clName);
	virtual ~Report_Straight();

private:
	virtual void writeContent(QString &html) const;//写具体内容
	void getWireCalaResPara();//针对IVL不同间隙圆类型获取导线计算相关参数类

	void baseInfo(QString &html) const;//杆塔概况
	void meteInfo(QString &html) const;//气象区信息
	virtual void wireInfo(QString &html) const = 0;//导线信息
	virtual void insulatorInfo(QString &html) const = 0;//绝缘子串信息
	virtual void gapInfo(QString &html) const = 0;//间隙值信息
	void calcInfo(QString &html) const;//计算参数信息
	void tensionCalcRes_Code2010(QString &html) const;//张力计算中间过程参数 2010旧规范
	void tensionCalcRes_Code2020(QString &html) const;//张力计算中间过程参数 2020新规范
	virtual void swingAngleRes(QString &html) const = 0;//摇摆角计算结果

protected:
	HdlDataUI_Stright          _dataUI;//直线塔界面数据基类

	HdlMdl_BasicPara           _basicInfo;//基本信息
	HdlMdl_CalcRelePara        _calcRelePara;//计算参数
	HdlMdl_SpanPara            _spanPara;//档距信息
	HdlMdl_VSpanCalSet         _vSpanCalSet;//垂直档距计算设置
	HdlMdl_MetaAreaPara        _meteAreaPara;//气象工况相关参数
	HdlMdl_WireRelatPara       _wireRelatPara;//导线相关参数
	HdlMdl_InsultorPara_I      _insultorPara_I;//绝缘子参数,内核中计算风偏角用

	//HdlMechCalProPara          _calProPara;//所有工况张力载荷计算过程数据，适用于IVL串
	HdlMechCalProPara          _tensionCalcProPara;//张力计算，所有工况过程数据，适用于IVL串
	HdlMechCalProPara          _loadCalcProPara;//荷载计算，所有工况计算过程数据，适用于IVL串
	HdlSwingAngleParaLib       _angleCalcPara;//大风操作雷电带电，摇摆角计算相关参数适，用于IV串

	HdlSwingAngleParaLib       _angleCalcPara_alpha;//大风操作雷电带电，摇摆角计算相关参数适，用于IV串
	HdlSwingAngleParaLib       _angleCalcPara_beta;//大风操作雷电带电，摇摆角计算相关参数适，用于IV串

};