#include "Report_Straight_I.h"
#include "DataUI_Stright_I.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"

//@function:    -- by mzq  2019/8/2 14:47
Report_Straight_I::Report_Straight_I(const string & clName) :Report_Straight(clName)
{
	HdlDataUI_Stright_I  dataUI_Stright_I = dynamic_cast<DataUI_Stright_I*>(_dataUI.GetPointee());
	_gapValuePara_I = dataUI_Stright_I->getGapValuePara_I();//I 串四种工况对应的间隙值参数
	_insulaGeoPara_I = dataUI_Stright_I->getInsulaGeoPara_I();//绝缘子串几何参数
}

//@function:    -- by mzq  2019/8/2 14:48
Report_Straight_I::~Report_Straight_I()
{

}

//@function: 导线信息   -- by mzq  2019/8/3 23:15
void Report_Straight_I::wireInfo(QString &html) const
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
	singlerow.push_back(SplitNum::typeToStr(_insulaGeoPara_I->getSplitNum()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("分裂间距 (m)");
	singlerow.push_back(_insulaGeoPara_I->getSplitShow());
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

//@function: 绝缘子串信息   -- by mzq  2019/8/4 11:07
void Report_Straight_I::insulatorInfo(QString &html) const
{
	html += HtmlAutility::insertText("4、绝缘子串参数信息", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;I串绝缘子串示意图如图4.1所示，绝缘子串参数信息如表4.1所示。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	/*插入I串示例图片*/
	string picPath;
	switch (_insulaGeoPara_I->getSplitNum())
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
	singlerow.push_back("绝缘子串型号");
	singlerow.push_back(_insultorPara_I->getInsultorName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("绝缘子串总重力 (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_I->getInsultorWeight()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("垂直风向绝缘子串联数 (联)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_I->getNumSubs_V()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("顺风向绝缘子串联数 (联)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_I->getNumSubs_H()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("单联绝缘子串片数 (片)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_I->getNumPlate()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("单片绝缘子受风面积 (m<sup>2</sup>)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_I->getShieldArea()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("重锤重力 (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_I->getHammerWeight()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L0 绝缘子串长度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_I->getInsultorLen()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L1 铁塔挂点-金具第一转动点 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_I->getL1()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2 铁塔挂点-上子导线线夹挂点 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_I->getL2()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3 上子导线线夹挂点-均压环 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_I->getL3()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L4 均压环宽度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_I->getL4()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L5 子导线线夹长度 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_I->getL5()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L6 小弧垂 (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_I->getL6()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("θ 线夹及小弧垂偏角 (度)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_I->getTheta()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 间隙值信息   -- by mzq  2019/8/4 17:10
void Report_Straight_I::gapInfo(QString &html) const
{
	html += HtmlAutility::insertText("5、空气间隙", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;海拔高度及相应的间隙值如表5.1所示。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	/*插入空气间隙表格*/
	html += HtmlAutility::insertText("表5.1 空气间隙表", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("参数名称");
	headerRow.push_back("参数信息");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("海拔高度 (m)");
	singlerow.push_back(Auxiliary::d2s(_basicInfo->getAltitude()));
	contentVec.push_back(singlerow);

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

	if (_gapValuePara_I->getIsHotLineWork())
	{
		singlerow.clear();
		singlerow.push_back("带电作业最小间隙 (m)");
		singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getHotLineWork_Gap()));
		contentVec.push_back(singlerow);

		singlerow.clear();
		singlerow.push_back("带电作业人体活动范围 (m)");
		singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getHotLineWorkRange()));
		contentVec.push_back(singlerow);
	}

	/*是否考虑对塔顶裕度*/
	if (_gapValuePara_I->getIsMargin_up())
	{
		singlerow.clear();
		singlerow.push_back("对塔顶裕度 (m)");
		singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getMarginValue_up()));
		contentVec.push_back(singlerow);
	}

	/*是否考虑对塔身裕度*/
	if (_gapValuePara_I->getIsMargin_down())
	{
		singlerow.clear();
		singlerow.push_back("对塔身裕度 (m)");
		singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getMarginValue_down()));
		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 摇摆角计算结果   -- by mzq  2019/8/30 21:02
void Report_Straight_I::swingAngleRes(QString &html) const
{
	if (CodeCate::DLT5582_2020 != _calcRelePara->getCodeType())
		swingAngleRes_Code2010(html);
	else
		swingAngleRes_Code2020(html);
}

//@function: 摇摆角计算结果 2010规范   -- by mzq 2020/12/14 11:02
void Report_Straight_I::swingAngleRes_Code2010(QString &html) const
{
	html += HtmlAutility::insertText("8、摇摆角计算结果", _fontSize_s3, 0, true);
	html += HtmlAutility::insertText("&nbsp;&nbsp;摇摆角计算结果如表8.1所示。", _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("表8.1 摇摆角计算结果表", _fontSize_s4, 1, false);

	/*表头*/
	vector<string> headerRow;
	headerRow.push_back("工况类别");
	headerRow.push_back("导线水平<br/>荷载<br/>(N/m)");
	headerRow.push_back("导线垂直<br/>荷载<br/>(N/m)");
	headerRow.push_back("导线<br/>风偏角<br/>(°)");
	headerRow.push_back("绝缘子串<br/>风压<br/>(N)");
	headerRow.push_back("绝缘子串<br/>摇摆角<br/>(°)");

	/*表内容*/
	vector<vector<string>> contentVec;
	vector<string> singlerow;
	for (int meteLoop = 0; meteLoop < _angleCalcPara->getSize(); ++meteLoop)
	{
		HdlSwingAnglePara swingAnglePara = _angleCalcPara->getAt(meteLoop);
		MeteCate::Type meteCate = swingAnglePara->getMeteCate();
		HdlMeteRelCalProPara loadCurMtCalPara = _loadCalcProPara->find(meteCate);//荷载计算结果

		singlerow.clear();
		singlerow.push_back(MeteCate::typeToStr(swingAnglePara->getMeteCate()));
		singlerow.push_back(Auxiliary::d2s(swingAnglePara->getHRatLoad()));
		singlerow.push_back(Auxiliary::d2s(loadCurMtCalPara->getVUnitLoad()));
		singlerow.push_back(Auxiliary::d2s(std::atan(swingAnglePara->getHRatLoad() / loadCurMtCalPara->getVUnitLoad()) * 180 / Consts::PI));
		singlerow.push_back(Auxiliary::d2s(swingAnglePara->getInsuHLoad()));
		singlerow.push_back(Auxiliary::d2s(swingAnglePara->getSwingAngle()));
		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 摇摆角计算结果 2020规范   -- by mzq 2020/12/14 11:02
void Report_Straight_I::swingAngleRes_Code2020(QString &html) const
{
	{
		html += HtmlAutility::insertEnter(3, _fontSize_s3);
		html += HtmlAutility::insertText("8、摇摆角计算结果", _fontSize_s3, 0, true);
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
		for (int meteLoop = 0; meteLoop < _angleCalcPara->getSize(); ++meteLoop)
		{
			HdlSwingAnglePara swingAnglePara = _angleCalcPara->getAt(meteLoop);
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
			singlerow.push_back(Auxiliary::d2s(std::atan(swingAnglePara->getHRatLoad()/loadCurMtCalPara->getVUnitLoad())*180/Consts::PI));
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
		for (int meteLoop = 0; meteLoop < _angleCalcPara->getSize(); ++meteLoop)
		{
			HdlSwingAnglePara swingAnglePara = _angleCalcPara->getAt(meteLoop);
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