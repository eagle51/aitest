#include "Report_Straight_L.h"
#include "DataUI_Stright_L.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"


//@function:    -- by mzq  2019/8/2 14:47
Report_Straight_L::Report_Straight_L(const string & clName) :Report_Straight(clName)
{
	HdlDataUI_Stright_L  dataUI_Stright_L = dynamic_cast<DataUI_Stright_L*>(_dataUI.GetPointee());
	_gapValuePara_VL = dataUI_Stright_L->getGapValuePara_VL();
	_insultorPara_L = dataUI_Stright_L->getInsultorPara_L();
	_insulaGeoPara_L = dataUI_Stright_L->getInsulaGeoPara_L();
}

//@function:    -- by mzq  2019/8/2 14:48
Report_Straight_L::~Report_Straight_L()
{

}

//@function: 导线信息   -- by mzq  2019/8/3 23:16
void Report_Straight_L::wireInfo(QString &html) const
{
	html += HtmlAutility::insertText("3、导线信息", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;导线信息如表3.1所示。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("表3.1 导线信息表", _fontSize_s4, 1, false);

	vector<string> headerRow;
	headerRow.push_back("参数名称");
	headerRow.push_back("参数信息");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("导线型号");
	singlerow.push_back(_wireRelatPara->getWPA()->getName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("分裂数");
	singlerow.push_back(SplitNum::typeToStr(_insulaGeoPara_L->getSplitNum()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("分裂间距 (m)");
	singlerow.push_back(_insulaGeoPara_L->getSplitShow());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("截面面积 (mm<sup>2</sup>)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getArea()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("直径 (mm)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getDiameter()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("拉断力 (N)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getUltimateTension()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("单位重量 (kg/m)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getUnitWeight()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("弹性系数 (MPa)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getE()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("线性膨胀系数 (1/℃)");
	singlerow.push_back(QString::number(_wireRelatPara->getWPA()->getLineExpansionCof()).toLocal8Bit().toStdString());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("安全系数");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getSaftyCof()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("新线系数");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getNewWireCof()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("平均运行张力系数");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getAvgCof()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 绝缘子串信息   -- by mzq  2019/8/4 11:08
void Report_Straight_L::insulatorInfo(QString &html) const
{
	html += HtmlAutility::insertText("4、绝缘子串参数信息", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;L串绝缘子串示意图如图4.1所示，绝缘子串参数信息如表4.1所示。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	/*插入I串示例图片*/
	string picPath;
	switch (_insulaGeoPara_L->getSplitNum())
	{
	case SmartCl::SplitNum::One:
		picPath = "./Resources/SplitLNum/noneSplit.png";
		break;
	case SmartCl::SplitNum::TwoV:
		picPath = "./Resources/SplitLNum/twoSplitVer.png";
		break;
	case SmartCl::SplitNum::TwoH:
		picPath = "./Resources/SplitLNum/twoSplitHor.png";
		break;
	case SmartCl::SplitNum::Three:
		picPath = "./Resources/SplitLNum/threeSplit.png";
		break;
	case SmartCl::SplitNum::Four:
		picPath = "./Resources/SplitLNum/fourSplit.png";
		break;
	case SmartCl::SplitNum::Six:
		picPath = "./Resources/SplitLNum/sixSplit.png";
		break;
	case SmartCl::SplitNum::Eight:
		picPath = "./Resources/SplitLNum/eightSplit.png";
		break;
	case SmartCl::SplitNum::Ten:
		picPath = "./Resources/SplitLNum/tenSplit.png";
		break;
	default:
		break;
	}
	html += HtmlAutility::insertPic(picPath, 504, 360);
	html += HtmlAutility::insertText("图4.1 L串绝缘子串示意图", _fontSize_s4, 1, false);


	/*插入绝缘子串参数表格*/
	html += HtmlAutility::insertText("表4.1 绝缘子串参数信息表", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("参数名称");
	headerRow.push_back("参数信息");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("绝缘子串型号");
	singlerow.push_back(_insultorPara_L->getInsultorName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L串总重力减b肢重力 (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_L->getInsultorWeight_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L串总重力减a肢重力 (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_L->getInsultorWeight_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("a肢垂直风向绝缘子串联数 (联)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_L->getNumSubs_a_V()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("b肢垂直风向绝缘子串联数 (联)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_L->getNumSubs_b_V()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("a肢单联绝缘子串片数 (片)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_L->getNumPlate_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("b肢单联绝缘子串片数 (片)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_L->getNumPlate_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("单片绝缘子受风面积 (m<sup>2</sup>)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_L->getShieldArea()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("重锤重力 (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_L->getHammerWeight()));
	contentVec.push_back(singlerow);


	singlerow.clear();
	singlerow.push_back("α受压角 (°)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getAlpha_correctAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("α推荐值 (°)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getAlpha_recommend()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("α实际取值 (°)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getAlpha()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("β受压角 (°)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getBeta_correctAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("β推荐值 (°)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getBeta_recommend()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("β实际取值 (°)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getBeta()));
	contentVec.push_back(singlerow);


	singlerow.clear();
	singlerow.push_back("L1_a a肢单肢长度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL1_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L1_b b肢单肢长度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL1_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2_a a肢均压环高度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL2_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2_b b肢均压环高度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL2_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3_a a肢均压环宽度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL3_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3_b b肢均压环宽度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL3_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L4 联板宽度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL4()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L5 线夹挂孔与联板挂孔垂距 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL5()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L6 子导线线夹长度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL6()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L7 小弧垂 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL7()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("θ 线夹及小弧垂偏角 (°)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getTheta()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 间隙值信息   -- by mzq  2019/8/4 17:11
void Report_Straight_L::gapInfo(QString &html) const
{
	html += HtmlAutility::insertText("5、空气间隙", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;海拔高度及相应的间隙值如表5.1所示。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	/*插入空气间隙表格*/
	html += HtmlAutility::insertText("表5.1 空气间隙表", _fontSize_s4, 1, false);

	html += HtmlAutility::tableStyle(_fontSize_5);
	html += "<table class=\"gridtable\" border=\"1\" cellspacing=\"0\" width=\"615\">";

	html += "<tr>";
	html += HtmlAutility::insertTableHeaderCell("参数名称", 1, 2);
	html += HtmlAutility::insertTableHeaderCell("参数信息");
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("海拔高度 (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_basicInfo->getAltitude()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("工频电压间隙  (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getPowerFreq_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("操作过电压间隙  (m)", 2, 1);
	html += HtmlAutility::insertTableTextCell("中相（距塔身）");
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getInOverVolt_Down_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("中相（距塔顶）");
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getInOverVolt_Up_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("雷电过电压间隙  (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getOutOverVolt_Gap()));
	html += "</tr>";

	/*是否考虑带电作业*/
	if (_gapValuePara_VL->getIsHotLineWork())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("带电作业最小间隙 (m)", 2, 1);
		html += HtmlAutility::insertTableTextCell("中相（距塔身）");
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getHotLineWork_Down_Gap()));
		html += "</tr>";

		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("中相（距塔顶）");
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getHotLineWork_Up_Gap()));
		html += "</tr>";

		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("带电作业人体活动范围 (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getHotLineWorkRange()));
		html += "</tr>";
	}

	/*是否考虑对塔顶裕度*/
	if (_gapValuePara_VL->getIsMargin_up())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("对塔顶裕度 (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getMarginValue_up()));
		html += "</tr>";
	}

	/*是否考虑对塔身裕度*/
	if (_gapValuePara_VL->getIsMargin_down())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("对塔身裕度 (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getMarginValue_down()));
		html += "</tr>";
	}

	html += "</table>";
}

//@function: 摇摆角计算结果   -- by mzq  2019/8/30 21:02
void Report_Straight_L::swingAngleRes(QString &html) const
{
	if (CodeCate::DLT5582_2020 != _calcRelePara->getCodeType())
		swingAngleRes_Code2010(html);
	else
		swingAngleRes_Code2020(html);
}

//@function: 摇摆角计算结果 2010规范   -- by mzq 2020/12/15 16:19
void Report_Straight_L::swingAngleRes_Code2010(QString &html) const
{
	html += HtmlAutility::insertText("8、摇摆角计算结果", _fontSize_s3, 0, true);

	/**********************************************L串左肢 alpha********************************************************/
	double alphaCorrectAngle = _insulaGeoPara_L->getAlpha_correctAngle();
	double alphaRecommend = _insulaGeoPara_L->getAlpha_recommend();
	double alpha = _insulaGeoPara_L->getAlpha();
	std::string text_alpha = "&nbsp;&nbsp;L串左肢夹角α推荐值计算时，将L串的左肢参数等效成I串进行摇摆角计算，取转角度数为"
		+ Auxiliary::d2s(-_basicInfo->getRotatAngle())
		+ "度，L串左肢等效I串计算结果如表8.1所示。L串左肢夹角α推荐值=等效I串大风工况摇摆角-α受压角，α受压角为"
		+ Auxiliary::d2s(alphaCorrectAngle) + "度，经过计算可得<strong>α推荐值为" + Auxiliary::d2s(alphaRecommend) + "度，"
		+ "α实际取值为" + Auxiliary::d2s(alpha) + "度。</strong>";
	html += HtmlAutility::insertText(text_alpha, _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("表8.1 L串左肢等效I串摇摆角计算结果表", _fontSize_s4, 1, false);

	/*表头*/
	vector<string> headerRow_alpha;
	headerRow_alpha.push_back("工况类别");
	headerRow_alpha.push_back("导线水平荷载<br/>(N/m)");
	headerRow_alpha.push_back("绝缘子串风压<br/>(N)");
	headerRow_alpha.push_back("摇摆角<br/>(°)");

	/*表内容*/
	vector<vector<string>> contentVec_alpha;
	vector<string> singlerow_alpha;
	for (int meteLoop = 0; meteLoop < _angleCalcPara_alpha->getSize(); ++meteLoop)
	{
		HdlSwingAnglePara swingAnglePara = _angleCalcPara_alpha->getAt(meteLoop);

		singlerow_alpha.clear();
		singlerow_alpha.push_back(MeteCate::typeToStr(swingAnglePara->getMeteCate()));
		singlerow_alpha.push_back(Auxiliary::d2s(swingAnglePara->getHRatLoad()));
		singlerow_alpha.push_back(Auxiliary::d2s(swingAnglePara->getInsuHLoad()));
		singlerow_alpha.push_back(Auxiliary::d2s(swingAnglePara->getSwingAngle()));
		contentVec_alpha.push_back(singlerow_alpha);
	}

	html += HtmlAutility::insertTable(headerRow_alpha, contentVec_alpha, _fontSize_5, 1);





	/**********************************************L串右肢 beta********************************************************/
	double betaCorrectAngle = _insulaGeoPara_L->getBeta_correctAngle();
	double betaRecommend = _insulaGeoPara_L->getBeta_recommend();
	double beta = _insulaGeoPara_L->getBeta();
	std::string text_beta = "&nbsp;&nbsp;L串右肢夹角β推荐值计算时，将L串的右肢参数等效成I串进行摇摆角计算，取转角度数为"
		+ Auxiliary::d2s(_basicInfo->getRotatAngle())
		+ "度，L串右肢等效I串计算结果如表8.2所示。L串右肢夹角β推荐值=等效I串大风工况摇摆角-β受压角，β受压角为"
		+ Auxiliary::d2s(betaCorrectAngle) + "度，经过计算可得<strong>β推荐值为" + Auxiliary::d2s(betaRecommend) + "度，"
		+ "β实际取值为" + Auxiliary::d2s(beta) + "度。</strong>";
	html += HtmlAutility::insertText(text_beta, _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("表8.2 L串右肢等效I串摇摆角计算结果表", _fontSize_s4, 1, false);

	/*表头*/
	vector<string> headerRow_beta;
	headerRow_beta.push_back("工况类别");
	headerRow_beta.push_back("导线水平荷载<br/>(N/m)");
	headerRow_beta.push_back("绝缘子串风压<br/>(N)");
	headerRow_beta.push_back("摇摆角<br/>(°)");

	/*表内容*/
	vector<vector<string>> contentVec_beta;
	vector<string> singlerow_beta;
	for (int meteLoop = 0; meteLoop < _angleCalcPara_beta->getSize(); ++meteLoop)
	{
		HdlSwingAnglePara swingAnglePara = _angleCalcPara_beta->getAt(meteLoop);

		singlerow_beta.clear();
		singlerow_beta.push_back(MeteCate::typeToStr(swingAnglePara->getMeteCate()));
		singlerow_beta.push_back(Auxiliary::d2s(swingAnglePara->getHRatLoad()));
		singlerow_beta.push_back(Auxiliary::d2s(swingAnglePara->getInsuHLoad()));
		singlerow_beta.push_back(Auxiliary::d2s(swingAnglePara->getSwingAngle()));
		contentVec_beta.push_back(singlerow_beta);
	}

	html += HtmlAutility::insertTable(headerRow_beta, contentVec_beta, _fontSize_5, 1);
}

//@function: 摇摆角计算结果 2020规范   -- by mzq 2020/12/15 16:19
void Report_Straight_L::swingAngleRes_Code2020(QString &html) const
{
	html += HtmlAutility::insertText("8、摇摆角计算结果", _fontSize_s3, 0, true);

	{
		/**********************************************L串左肢 alpha********************************************************/
		html += HtmlAutility::insertText("8.1、L串左肢计算结果", _fontSize_s4, 0, true);

		double alphaCorrectAngle = _insulaGeoPara_L->getAlpha_correctAngle();
		double alphaRecommend = _insulaGeoPara_L->getAlpha_recommend();
		double alpha = _insulaGeoPara_L->getAlpha();
		std::string text_alpha = "&nbsp;&nbsp;L串左肢夹角α推荐值计算时，将L串的左肢参数等效成I串进行摇摆角计算，取转角度数为"
			+ Auxiliary::d2s(-_basicInfo->getRotatAngle())
			+ "度，L串左肢等效I串计算结果如表8.1及8.2所示。L串左肢夹角α推荐值=等效I串大风工况摇摆角-α受压角，α受压角为"
			+ Auxiliary::d2s(alphaCorrectAngle) + "度，经过计算可得<strong>α推荐值为" + Auxiliary::d2s(alphaRecommend) + "度，"
			+ "α实际取值为" + Auxiliary::d2s(alpha) + "度。</strong>";
		html += HtmlAutility::insertText(text_alpha, _fontSize_s4, 0, false);


		{
			html += HtmlAutility::insertText("&nbsp;&nbsp;导线相关计算结果如表8.1所示。", _fontSize_s4, 0, false);
			html += HtmlAutility::insertText("表8.1 导线相关计算结果表", _fontSize_s4, 1, false);

			/*表头*/
			vector<string> headerRow;
			headerRow.push_back("工况<br/>类别");
			headerRow.push_back("风荷<br/>载折<br/>减系<br/>数<br/><i>γ</i><sub>c</sub>");
			headerRow.push_back("阵风<br/>系数<br/><i>β</i><sub>c</sub>");
			headerRow.push_back("档距<br/>折减<br/>系数<br/><i>α</i><sub>L</sub>");
			headerRow.push_back("风压<br/>高度<br/>变化<br/>系数<br/><i>μ</i><sub>z</sub>");
			headerRow.push_back("导线<br/>水平<br/>荷载<br/>(N/m)");
			headerRow.push_back("导线<br/>垂直<br/>荷载<br/>(N/m)");
			headerRow.push_back("导线<br/>风偏<br/>角<br/>(°)");
			headerRow.push_back("垂直<br/>档距<br/>(m)");

			/*表内容*/
			vector<vector<string>> contentVec;
			vector<string> singlerow;
			for (int meteLoop = 0; meteLoop < _angleCalcPara_alpha->getSize(); ++meteLoop)
			{
				HdlSwingAnglePara swingAnglePara = _angleCalcPara_alpha->getAt(meteLoop);
				MeteCate::Type meteCate = swingAnglePara->getMeteCate();
				HdlMeteRelCalProPara loadCurMtCalPara = _loadCalcProPara->find(meteCate);//荷载计算结果

				singlerow.clear();
				singlerow.push_back(MeteCate::typeCase2ToStr(swingAnglePara->getMeteCate()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getGamma_c()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getBeta_c()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getAlpha_l()));

				if (MeteCate::InOverVolt_Case2 == meteCate || MeteCate::OutOverVolt_Case2 == meteCate || MeteCate::HotLineWork_Case2 == meteCate)
					singlerow.push_back("/");
				else
					singlerow.push_back(Auxiliary::d2s(loadCurMtCalPara->getWireMu_z()));

				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getHRatLoad()));
				singlerow.push_back(Auxiliary::d2s(loadCurMtCalPara->getVUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(std::atan(swingAnglePara->getHRatLoad() / loadCurMtCalPara->getVUnitLoad()) * 180 / Consts::PI));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getVSpan()));
				contentVec.push_back(singlerow);
			}

			html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
		}

		{
			html += HtmlAutility::insertText("&nbsp;&nbsp;绝缘子串相关计算结果如表8.2所示。", _fontSize_s4, 0, false);
			html += HtmlAutility::insertText("表8.2 绝缘子串相关计算结果表", _fontSize_s4, 1, false);

			/*表头*/
			vector<string> headerRow;
			headerRow.push_back("工况类别");
			headerRow.push_back("绝缘子串风压<br/>(N)");
			headerRow.push_back("绝缘子串重力<br/>(N)");
			headerRow.push_back("重锤重力<br/>(N)");
			headerRow.push_back("摇摆角<br/>(°)");

			/*表内容*/
			vector<vector<string>> contentVec;
			vector<string> singlerow;
			for (int meteLoop = 0; meteLoop < _angleCalcPara_alpha->getSize(); ++meteLoop)
			{
				HdlSwingAnglePara swingAnglePara = _angleCalcPara_alpha->getAt(meteLoop);
				MeteCate::Type meteCate = swingAnglePara->getMeteCate();

				singlerow.clear();
				singlerow.push_back(MeteCate::typeCase2ToStr(swingAnglePara->getMeteCate()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getInsuHLoad()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getInsulVLoad()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getHammerVLoad()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getSwingAngle()));
				contentVec.push_back(singlerow);
			}
			html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
		}
	}
	



	{
		/**********************************************L串右肢 beta********************************************************/
		html += HtmlAutility::insertText("8.2、L串右肢计算结果", _fontSize_s4, 0, true);
		double betaCorrectAngle = _insulaGeoPara_L->getBeta_correctAngle();
		double betaRecommend = _insulaGeoPara_L->getBeta_recommend();
		double beta = _insulaGeoPara_L->getBeta();
		std::string text_beta = "&nbsp;&nbsp;L串右肢夹角β推荐值计算时，将L串的右肢参数等效成I串进行摇摆角计算，取转角度数为"
			+ Auxiliary::d2s(_basicInfo->getRotatAngle())
			+ "度，L串右肢等效I串计算结果如表8.3及8.4所示。L串右肢夹角β推荐值=等效I串大风工况摇摆角-β受压角，β受压角为"
			+ Auxiliary::d2s(betaCorrectAngle) + "度，经过计算可得<strong>β推荐值为" + Auxiliary::d2s(betaRecommend) + "度，"
			+ "β实际取值为" + Auxiliary::d2s(beta) + "度。</strong>";
		html += HtmlAutility::insertText(text_beta, _fontSize_s4, 0, false);


		{
			html += HtmlAutility::insertText("&nbsp;&nbsp;导线相关计算结果如表8.3所示。", _fontSize_s4, 0, false);
			html += HtmlAutility::insertText("表8.3 导线相关计算结果表", _fontSize_s4, 1, false);

			/*表头*/
			vector<string> headerRow;
			headerRow.push_back("工况<br/>类别");
			headerRow.push_back("风荷<br/>载折<br/>减系<br/>数<br/><i>γ</i><sub>c</sub>");
			headerRow.push_back("阵风<br/>系数<br/><i>β</i><sub>c</sub>");
			headerRow.push_back("档距<br/>折减<br/>系数<br/><i>α</i><sub>L</sub>");
			headerRow.push_back("风压<br/>高度<br/>变化<br/>系数<br/><i>μ</i><sub>z</sub>");
			headerRow.push_back("导线<br/>水平<br/>荷载<br/>(N/m)");
			headerRow.push_back("导线<br/>垂直<br/>荷载<br/>(N/m)");
			headerRow.push_back("导线<br/>风偏<br/>角<br/>(°)");
			headerRow.push_back("垂直<br/>档距<br/>(m)");

			/*表内容*/
			vector<vector<string>> contentVec;
			vector<string> singlerow;
			for (int meteLoop = 0; meteLoop < _angleCalcPara_beta->getSize(); ++meteLoop)
			{
				HdlSwingAnglePara swingAnglePara = _angleCalcPara_beta->getAt(meteLoop);
				MeteCate::Type meteCate = swingAnglePara->getMeteCate();
				HdlMeteRelCalProPara loadCurMtCalPara = _loadCalcProPara->find(meteCate);//荷载计算结果

				singlerow.clear();
				singlerow.push_back(MeteCate::typeCase2ToStr(swingAnglePara->getMeteCate()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getGamma_c()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getBeta_c()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getAlpha_l()));

				if (MeteCate::InOverVolt_Case2 == meteCate || MeteCate::OutOverVolt_Case2 == meteCate || MeteCate::HotLineWork_Case2 == meteCate)
					singlerow.push_back("/");
				else
					singlerow.push_back(Auxiliary::d2s(loadCurMtCalPara->getWireMu_z()));

				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getHRatLoad()));
				singlerow.push_back(Auxiliary::d2s(loadCurMtCalPara->getVUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(std::atan(swingAnglePara->getHRatLoad() / loadCurMtCalPara->getVUnitLoad()) * 180 / Consts::PI));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getVSpan()));
				contentVec.push_back(singlerow);
			}

			html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
		}

		{
			html += HtmlAutility::insertText("&nbsp;&nbsp;绝缘子串相关计算结果如表8.4所示。", _fontSize_s4, 0, false);
			html += HtmlAutility::insertText("表8.4 绝缘子串相关计算结果表", _fontSize_s4, 1, false);

			/*表头*/
			vector<string> headerRow;
			headerRow.push_back("工况类别");
			headerRow.push_back("绝缘子串风压<br/>(N)");
			headerRow.push_back("绝缘子串重力<br/>(N)");
			headerRow.push_back("重锤重力<br/>(N)");
			headerRow.push_back("摇摆角<br/>(°)");

			/*表内容*/
			vector<vector<string>> contentVec;
			vector<string> singlerow;
			for (int meteLoop = 0; meteLoop < _angleCalcPara_beta->getSize(); ++meteLoop)
			{
				HdlSwingAnglePara swingAnglePara = _angleCalcPara_beta->getAt(meteLoop);
				MeteCate::Type meteCate = swingAnglePara->getMeteCate();

				singlerow.clear();
				singlerow.push_back(MeteCate::typeCase2ToStr(swingAnglePara->getMeteCate()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getInsuHLoad()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getInsulVLoad()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getHammerVLoad()));
				singlerow.push_back(Auxiliary::d2s(swingAnglePara->getSwingAngle()));
				contentVec.push_back(singlerow);
			}
			html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
		}
	}
	
}
