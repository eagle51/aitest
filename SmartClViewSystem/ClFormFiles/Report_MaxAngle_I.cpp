#include "Report_MaxAngle_I.h"
#include "Auxiliary.h"
#include "HtmlAutility.h"

Report_MaxAngle_I::Report_MaxAngle_I(const string &clName) :Report_MaxAngle(clName)
{
	_dataUI_MaxAngle_I = dynamic_cast<DataUI_MaxAngle_I*>(_dataUI.GetPointee());
	_gapValuePara_I = _dataUI_MaxAngle_I->getGapValuePara_I();//I 串四种工况对应的间隙值参数
	_insulaHangPara_I = _dataUI_MaxAngle_I->getInsulaHangPara_I();//绝缘子串几何参数
}

Report_MaxAngle_I::~Report_MaxAngle_I()
{

}

//@function: 间隙值信息   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_I::gapInfo(QString &html) const
{
	html += HtmlAutility::insertText("2.空气间隙", _fontSize_s3, 0, true);
	string text = "&nbsp;&nbsp;间隙值信息如表 2.1所示。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("表2.1 空气间隙表", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("参数名称");
	headerRow.push_back("参数信息");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("工频电压间隙 (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getPowerFreq_Gap()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("操作过电压间隙 (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getInOverVolt_Gap()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("雷电过电压间隙 (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getOutOverVolt_Gap()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("带电作业最小间隙 (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getHotLineWork_Gap()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("带电作业人体活动范围 (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getHotLineWorkRange()));
	contentVec.push_back(singlerow);

	/*是否考虑裕度*/
	if (_gapValuePara_I->getIsMargin_down())
	{
		singlerow.clear();
		singlerow.push_back("裕度值 (m)");
		singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getMarginValue_down()));
		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 绝缘子串信息   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_I::insulatorInfo(QString &html) const
{
	//3.绝缘子串参数
	html += HtmlAutility::insertText("3.绝缘子串参数", _fontSize_s3, 0, true);

	HdlJPoint hangPoint = _insulaHangPara_I->getHangPoint();
	string text = QString::fromLocal8Bit("&nbsp;&nbsp;绝缘子I串挂点号为:%1，坐标为（%2，%3, %4）。")
		.arg(QString::number(hangPoint->getTagNum()))
		.arg(QString::number(hangPoint->getX()))
		.arg(QString::number(hangPoint->getY()))
		.arg(QString::number(hangPoint->getZ())).toLocal8Bit();

	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	string text1 = "&nbsp;&nbsp;绝缘子串几何参数示意图如图3.1所示，参数表如表3.1所示";
	html += HtmlAutility::insertText(text1, _fontSize_s4, 0, false);

	HdlMdl_InsulaGeoPara_I insulaGeoPara_I = _insulaHangPara_I->getInsulaGeoPara_I();
	/*插入I串示例图片*/
	string picPath;
	switch (insulaGeoPara_I->getSplitNum())
	{
	case SmartCl::SplitNum::One:
		picPath = "./Resources/SplitINum/noneSplit.png";
		break;
	case SmartCl::SplitNum::TwoV:
		picPath = "./Resources/SplitINum/twoSplitVer.png";
		break;
	case SmartCl::SplitNum::TwoH:
		picPath = "./Resources/SplitINum/twoSplitHor.png";
		break;
	case SmartCl::SplitNum::Three:
		picPath = "./Resources/SplitINum/threeSplit.png";
		break;
	case SmartCl::SplitNum::Four:
		picPath = "./Resources/SplitINum/fourSplit.png";
		break;
	case SmartCl::SplitNum::Six:
		picPath = "./Resources/SplitINum/sixSplit.png";
		break;
	case SmartCl::SplitNum::Eight:
		picPath = "./Resources/SplitINum/eightSplit.png";
		break;
	case SmartCl::SplitNum::Ten:
		picPath = "./Resources/SplitINum/tenSplit.png";
		break;
	default:
		break;
	}
	html += HtmlAutility::insertPic(picPath, 190, 360);
	html += HtmlAutility::insertText("图4.1 I串绝缘子串示意图", _fontSize_s4, 1, false);

	/*插入绝缘子串参数表格*/
	html += HtmlAutility::insertText("表4.1 绝缘子串参数信息表", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("参数名称");
	headerRow.push_back("参数信息");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("分裂数");
	singlerow.push_back(SplitNum::typeToStr(insulaGeoPara_I->getSplitNum()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("分裂间距 (m)");
	singlerow.push_back(insulaGeoPara_I->getSplitShow());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L1 铁塔挂点-金具第一转动点 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL1()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2 铁塔挂点-上子导线线夹挂点 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL2()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3 上子导线线夹挂点-均压环 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL3()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L4 均压环宽度 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL4()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L5 子导线线夹长度 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL5()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L6 小弧垂 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL6()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 角度计算结果   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_I::angleRes(QString &html) const
{
	html += HtmlAutility::insertText("4.最大允许摇摆角", _fontSize_s3, 0, true);
	html += HtmlAutility::insertText("各工况最大允许摇摆角如表4.1所示。", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("工况类别");
	headerRow.push_back("最大允许摇摆角 (度)");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("工频");
	singlerow.push_back(Auxiliary::d2s(_dataUI_MaxAngle_I->getPowerFreqAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("操作");
	singlerow.push_back(Auxiliary::d2s(_dataUI_MaxAngle_I->getInOverVoltAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("雷电");
	singlerow.push_back(Auxiliary::d2s(_dataUI_MaxAngle_I->getOutOverVoltAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("带电作业");
	singlerow.push_back(Auxiliary::d2s(_dataUI_MaxAngle_I->getHotLineWorkAngle()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

