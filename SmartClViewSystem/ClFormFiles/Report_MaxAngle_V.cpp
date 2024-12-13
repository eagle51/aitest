#include "Report_MaxAngle_V.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"

Report_MaxAngle_V::Report_MaxAngle_V(const string & clName) :Report_MaxAngle(clName)
{
	_dataUI_MaxAngle_V = dynamic_cast<DataUI_MaxAngle_V*>(_dataUI.GetPointee());
	_gapValuePara_V = _dataUI_MaxAngle_V->getGapValuePara_VL();//I 串四种工况对应的间隙值参数
	_insulaHangPara_V = _dataUI_MaxAngle_V->getInsulaHangPara_V();//绝缘子串几何参数
}

Report_MaxAngle_V::~Report_MaxAngle_V()
{

}

//@function: 间隙值信息   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_V::gapInfo(QString &html) const
{
	html += HtmlAutility::insertText("2.空气间隙", _fontSize_s3, 0, true);
	string text = "&nbsp;&nbsp;间隙值信息如表2.1所示。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("表2.1 空气间隙表", _fontSize_s4, 1, false);

	html += HtmlAutility::tableStyle(_fontSize_5);
	html += "<table class=\"gridtable\" border=\"1\" cellspacing=\"0\" width=\"615\">";

	html += "<tr>";
	html += HtmlAutility::insertTableHeaderCell("参数名称", 1, 2);
	html += HtmlAutility::insertTableHeaderCell("参数信息");
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("工频电压间隙  (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getPowerFreq_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("操作过电压间隙  (m)", 2, 1);
	html += HtmlAutility::insertTableTextCell("中相（距塔身）");
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getInOverVolt_Down_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("中相（距塔顶）");
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getInOverVolt_Up_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("雷电过电压间隙  (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getOutOverVolt_Gap()));
	html += "</tr>";

	/*是否考虑带电作业*/
	if (_gapValuePara_V->getIsHotLineWork())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("带电作业最小间隙 (m)", 2, 1);
		html += HtmlAutility::insertTableTextCell("中相（距塔身）");
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getHotLineWork_Down_Gap()));
		html += "</tr>";

		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("中相（距塔顶）");
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getHotLineWork_Up_Gap()));
		html += "</tr>";

		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("带电作业人体活动范围 (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getHotLineWorkRange()));
		html += "</tr>";
	}

	/*是否考虑对塔顶裕度*/
	if (_gapValuePara_V->getIsMargin_up())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("对塔顶裕度 (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getMarginValue_up()));
		html += "</tr>";
	}

	/*是否考虑对塔身裕度*/
	if (_gapValuePara_V->getIsMargin_down())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("对塔身裕度 (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getMarginValue_down()));
		html += "</tr>";
	}

	html += "</table>";
}

//@function: 绝缘子串信息   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_V::insulatorInfo(QString &html) const
{
	//3.绝缘子串参数
	html += HtmlAutility::insertText("3.绝缘子串参数", _fontSize_s3, 0, true);

	HdlJPoint hangPointLeft = _insulaHangPara_V->getHangPointLeft();
	string textLeftPoint = QString::fromLocal8Bit("&nbsp;&nbsp;V串左侧挂点号为:%1，坐标为（%2，%3, %4）。")
		.arg(QString::number(hangPointLeft->getTagNum()))
		.arg(QString::number(hangPointLeft->getX()))
		.arg(QString::number(hangPointLeft->getY()))
		.arg(QString::number(hangPointLeft->getZ())).toLocal8Bit();
	html += HtmlAutility::insertText(textLeftPoint, _fontSize_s4, 0, false);

	HdlJPoint hangPointRight = _insulaHangPara_V->getHangPointRight();
	string textRightPoint = QString::fromLocal8Bit("&nbsp;&nbsp;V串右侧挂点号为:%1，坐标为（%2，%3, %4）。")
		.arg(QString::number(hangPointRight->getTagNum()))
		.arg(QString::number(hangPointRight->getX()))
		.arg(QString::number(hangPointRight->getY()))
		.arg(QString::number(hangPointRight->getZ())).toLocal8Bit();
	html += HtmlAutility::insertText(textRightPoint, _fontSize_s4, 0, false);

	HdlMdl_InsulaGeoPara_V insulaGeoPara_V = _insulaHangPara_V->getInsulaGeoPara_V();
	/*插入I串示例图片*/
	string picPath;
	switch (insulaGeoPara_V->getSplitNum())
	{
	case SmartCl::SplitNum::One:
		picPath = "./Resources/SplitVNum/noneSplit.png";
		break;
	case SmartCl::SplitNum::TwoV:
		picPath = "./Resources/SplitVNum/twoSplitVer.png";
		break;
	case SmartCl::SplitNum::TwoH:
		picPath = "./Resources/SplitVNum/twoSplitHor.png";
		break;
	case SmartCl::SplitNum::Three:
		picPath = "./Resources/SplitVNum/threeSplit.png";
		break;
	case SmartCl::SplitNum::Four:
		picPath = "./Resources/SplitVNum/fourSplit.png";
		break;
	case SmartCl::SplitNum::Six:
		picPath = "./Resources/SplitVNum/sixSplit.png";
		break;
	case SmartCl::SplitNum::Eight:
		picPath = "./Resources/SplitVNum/eightSplit.png";
		break;
	case SmartCl::SplitNum::Ten:
		picPath = "./Resources/SplitVNum/tenSplit.png";
		break;
	default:
		break;
	}
	html += HtmlAutility::insertPic(picPath, 408, 360);
	html += HtmlAutility::insertText("图4.1 V串绝缘子串示意图", _fontSize_s4, 1, false);

	/*插入绝缘子串参数表格*/
	html += HtmlAutility::insertText("表4.1 绝缘子串参数信息表", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("参数名称");
	headerRow.push_back("参数信息");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("分裂数");
	singlerow.push_back(SplitNum::typeToStr(insulaGeoPara_V->getSplitNum()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("分裂间距 (m)");
	singlerow.push_back(insulaGeoPara_V->getSplitShow());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L1 V串单肢长度 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL1()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2 均压环高度 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL2()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3 均压环宽度 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL3()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L4 联板宽度 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL4()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L5 线夹挂孔与连板挂孔垂距 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL5()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L6 子导线线夹长度 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL6()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L7 小弧垂 (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL7()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("θ 线夹及小弧垂偏角(度)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getTheta()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 角度计算结果   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_V::angleRes(QString &html) const
{
	html += HtmlAutility::insertText("4.最大允许摇摆角", _fontSize_s3, 0, true);

	if (_dataUI_MaxAngle_V->getMaxAngle() < 360)
	{
		string resStr = QString::fromLocal8Bit("&nbsp;&nbsp;V串最大允许夹角为%1度。")
			.arg(QString::number(_dataUI_MaxAngle_V->getMaxAngle())).toLocal8Bit();

		html += HtmlAutility::insertText(resStr, _fontSize_s4, 0, false);
	}
	else
	{
		string resStr = "&nbsp;&nbsp;V串最大允许夹角计算不收敛。";

		html += HtmlAutility::insertText(resStr, _fontSize_s4, 0, false);
	}

	if (_dataUI_MaxAngle_V->getMinAngle() < 360)
	{
		string resStr = QString::fromLocal8Bit("&nbsp;&nbsp;V串最小允许夹角为%1度。")
			.arg(QString::number(_dataUI_MaxAngle_V->getMinAngle())).toLocal8Bit();

		html += HtmlAutility::insertText(resStr, _fontSize_s4, 0, false);
	}
	else
	{
		string resStr = "&nbsp;&nbsp;V串最小允许夹角计算不收敛。";
		html += HtmlAutility::insertText(resStr, _fontSize_s4, 0, false);
	}
}