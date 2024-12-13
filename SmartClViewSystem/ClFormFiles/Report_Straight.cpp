#include "Report_Straight.h"
#include "Clearence.h"
#include "ClWidget.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"
#include "EnumLib.h"
#include <fstream>

#include "qfile.h"
#include "qtextstream.h"
#include <QPrinter>
#include <QTextDocument>
#include <QTextBlock>
#include "qdatetime.h"
#include "Cl_Stright_I.h"
#include "Cl_Stright_V.h"
#include "Cl_Stright_L.h"


Report_Straight::Report_Straight(const string & clName) :Report(clName)
{
	_dataUI = dynamic_cast<DataUI_Stright*>(_cl->getDataUI().GetPointee());

	_basicInfo = _dataUI->getBasicInfo();//基本信息
	_calcRelePara = _dataUI->getCalcRelePara();//计算参数
	_spanPara = _dataUI->getSpanPara();//档距信息
	_vSpanCalSet = _dataUI->getVSpanCalSet();//垂直档距计算设置
	_meteAreaPara = _dataUI->getMeteAreaPara();//气象工况相关参数
	_wireRelatPara = _dataUI->getWireRelatPara();//导线相关参数
	_insultorPara_I = _dataUI->getInsultorPara_I();//绝缘子参数,内核中计算风偏角用

	getWireCalaResPara();
}

Report_Straight::~Report_Straight()
{

}

//@function: 写具体内容   -- by mzq  2019/12/27 18:38
void Report_Straight::writeContent(QString &html) const
{
	baseInfo(html);
	meteInfo(html);
	wireInfo(html);
	insulatorInfo(html);
	gapInfo(html);
	calcInfo(html);

	if (CodeCate::DLT5582_2020 != _calcRelePara->getCodeType())
		tensionCalcRes_Code2010(html);
	else
		tensionCalcRes_Code2020(html);

	swingAngleRes(html);
}

//@function: 杆塔概况   -- by mzq  2019/8/2 11:35
void Report_Straight::baseInfo(QString &html) const
{
	/*正文文本*/
	html += HtmlAutility::insertText("1、杆塔概况", _fontSize_s3, 0, true);
	html += HtmlAutility::insertText("&nbsp;&nbsp;杆塔概况信息如表1.1所示。", _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("表1.1 杆塔概况信息表", _fontSize_s4, 1, false);

	vector<string> headerRow;
	headerRow.push_back("参数名称");
	headerRow.push_back("参数信息");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("杆塔名称");
	singlerow.push_back(_basicInfo->getTowName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("杆塔类型");
	singlerow.push_back(TowerType::typeToStr(_basicInfo->getTowType()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("电压等级 (kV)");
	singlerow.push_back(VoltType::typeToStr(_basicInfo->getVoltType()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("杆塔转角度数 (°)");
	singlerow.push_back(Auxiliary::d2s(_basicInfo->getRotatAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("杆塔计算相呼高 (m)");
	singlerow.push_back(Auxiliary::d2s(_calcRelePara->getTowerCallH()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("代表档距 (m)");
	singlerow.push_back(Auxiliary::d2s(_spanPara->getEqvSpan()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("水平档距 (m)");
	singlerow.push_back(Auxiliary::d2s(_spanPara->getHSpan()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("Kv系数");
	singlerow.push_back(Auxiliary::d2s(_spanPara->getKvCoef()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 气象区信息   -- by mzq  2019/8/3 18:09
void Report_Straight::meteInfo(QString &html) const
{
	html += HtmlAutility::insertText("2、气象区信息", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;气象区信息如表2.1所示，大风风速基准高度为：" + Auxiliary::d2s(_meteAreaPara->getWindRefH()) + "m。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("表2.1 气象区信息表。", _fontSize_s4, 1, false);

	/*气象区表格表头*/
	vector<string> headerRow;
	headerRow.push_back("工况类别");
	headerRow.push_back("温度 (℃)");
	headerRow.push_back("风速 (m/s)");
	headerRow.push_back("冰厚 (mm)");

	HdlSingleMeteArea singleMeteArea = _meteAreaPara->getSingleMeteArea();
	/*气象区表格表内容*/
	vector<vector<string>> contentVec;
	vector<string> singlerow;
	for (int meteLoop = 0; meteLoop < singleMeteArea->getCount(); ++meteLoop)
	{
		singlerow.clear();
		singlerow.push_back(MeteCate::typeToStr(singleMeteArea->at(meteLoop)->getMtCate()));
		singlerow.push_back(Auxiliary::d2s(singleMeteArea->at(meteLoop)->getTemp()));
		singlerow.push_back(Auxiliary::d2s(singleMeteArea->at(meteLoop)->getWindSpd()));
		singlerow.push_back(Auxiliary::d2s(singleMeteArea->at(meteLoop)->getIceDep()));
		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 计算参数信息   -- by mzq  2019/8/30 17:19
void Report_Straight::calcInfo(QString &html) const
{
	html += HtmlAutility::insertText("6、计算参数", _fontSize_s3, 0, true);

	html += HtmlAutility::insertText("&nbsp;&nbsp;计算参数信息如表6.1所示。", _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("表6.1 计算参数信息表", _fontSize_s4, 1, false);

	/*表头*/
	vector<string> headerRow;
	headerRow.push_back("参数名称");
	headerRow.push_back("参数信息");

	/*表内容*/
	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("导线平均高 (m)");
	singlerow.push_back(Auxiliary::d2s(_calcRelePara->getWireAvgH()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("杆塔计算相呼高 (m)");
	singlerow.push_back(Auxiliary::d2s(_calcRelePara->getTowerCallH()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("地面粗糙度类别");
	singlerow.push_back(TerrainRough::typeToStr(_calcRelePara->getTerrRought()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("规范类别");
	singlerow.push_back(CodeCate::typeToStr(_calcRelePara->getCodeType()));
	contentVec.push_back(singlerow);

	if (CodeCate::GB50545_2010 == _calcRelePara->getCodeType())
	{
		singlerow.clear();
		singlerow.push_back("风压不均匀系数<i>α</i>");

		if (_calcRelePara->getIsInputAlpha())
			singlerow.push_back(Auxiliary::d2s(_calcRelePara->getAlpha()));
		else
			singlerow.push_back("根据风速自动计算");

		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: 张力计算中间过程参数 2010旧规范   -- by mzq  2019/8/30 17:59
void Report_Straight::tensionCalcRes_Code2010(QString &html) const
{
	{
		html += HtmlAutility::insertText("7、张力计算结果", _fontSize_s3, 0, true);
		html += HtmlAutility::insertText("&nbsp;&nbsp;张力计算中间过程参数如表7.1所示。", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("表7.1 张力计算中间过程参数表", _fontSize_s4, 1, false);

		/*表头*/
		vector<string> headerRow;
		headerRow.push_back("工况类别");
		if (CodeCate::GB50061_2010 == _calcRelePara->getCodeType())
		{
			headerRow.push_back("风荷载档距系数<i>α</i>");
			headerRow.push_back("风荷载体型系数<i>μ</i><sub>S</sub>");
		}
		else if (CodeCate::GB50545_2010 == _calcRelePara->getCodeType())
		{
			headerRow.push_back("风压不均匀系数<i>α</i>");
			headerRow.push_back("体型系数<i>μ</i><sub>sc</sub>");
		}
		headerRow.push_back("允许张力 (N)");

		/*表内容*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);

			singlerow.clear();
			singlerow.push_back(MeteCate::typeToStr(meteRelCalProPara->getMtCate()));

			if (CodeCate::GB50061_2010 == _calcRelePara->getCodeType())
			{
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAlpha()));
			}
			else if (CodeCate::GB50545_2010 == _calcRelePara->getCodeType())
			{
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAlpha()));
			}

			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getMu_sc()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAllowTension()));
			contentVec.push_back(singlerow);
		}

		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}
	


	{
		html += HtmlAutility::insertText("&nbsp;&nbsp;张力计算结果如表7.2所示。", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("表7.2 张力计算结果表", _fontSize_s4, 1, false);

		/*表头*/
		vector<string> headerRow;
		headerRow.push_back("工况类别");
		headerRow.push_back("水平荷载<br/>(N/m)");
		headerRow.push_back("垂直荷载<br/>(N/m)");
		headerRow.push_back("综合荷载<br/>(N/m)");
		headerRow.push_back("导线张力<br/>(N)");
		headerRow.push_back("垂直档距<br/>(m)");

		/*表内容*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);

			singlerow.clear();
			singlerow.push_back(MeteCate::typeToStr(meteRelCalProPara->getMtCate()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgHUnitLoad()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getVUnitLoad()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgCplxRatLoad()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getTensionLoad()));
			singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getVSpan()));
			contentVec.push_back(singlerow);
		}

		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}
}

//@function: 张力计算中间过程参数 2020新规范   -- by mzq 2020/12/12 18:32
void Report_Straight::tensionCalcRes_Code2020(QString &html) const
{
	{
		html += HtmlAutility::insertText("7、张力计算结果", _fontSize_s3, 0, true);
		html += HtmlAutility::insertText("7.1、控制工况及张力计算结果", _fontSize_s4, 0, true);
		html += HtmlAutility::insertText("&nbsp;&nbsp;根据《DL/T 5582-2020 架空输电线路电气设计规程》要求计算控制工况及张力，中间过程参数如表7.1所示。", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("表7.1 风荷载计算中间过程参数表", _fontSize_s4, 1, false);

		/*表头*/
		vector<string> headerRow;
		headerRow.push_back("工况类别");
		headerRow.push_back("导地线风荷载折减系数<i>γ</i><sub>c</sub>");
		headerRow.push_back("风压高度变化系数<i>μ</i><sub>z</sub>");
		headerRow.push_back("体型系数<i>μ</i><sub>sc</sub>");

		/*表内容*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);
			MeteCate::Type meteCate = meteRelCalProPara->getMtCate();
			if (MeteCate::Wind_Case2 != meteCate && MeteCate::InOverVolt_Case2 != meteCate
				&& MeteCate::OutOverVolt_Case2 != meteCate && MeteCate::HotLineWork_Case2 != meteCate)
			{
				singlerow.clear();
				singlerow.push_back(MeteCate::typeToStr(meteCate));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getGamma_c()));

				if (MeteCate::Wind == meteCate || MeteCate::Wind_Case2 == meteCate)
					singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getWireMu_z()));
				else
					singlerow.push_back("/");

				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getMu_sc()));
				contentVec.push_back(singlerow);
			}
		}
		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}
	


	{
		html += HtmlAutility::insertText("&nbsp;&nbsp;荷载和张力计算结果如表7.2所示。", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("表7.2 荷载和张力计算结果表", _fontSize_s4, 1, false);

		/*表头*/
		vector<string> headerRow;
		headerRow.push_back("工况类别");
		headerRow.push_back("水平荷载<br/>(N/m)");
		headerRow.push_back("垂直荷载<br/>(N/m)");
		headerRow.push_back("综合荷载<br/>(N/m)");
		headerRow.push_back("导线张力<br/>(N)");

		/*表内容*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);
			MeteCate::Type meteCate = meteRelCalProPara->getMtCate();
			if (MeteCate::Wind_Case2 != meteCate && MeteCate::InOverVolt_Case2 != meteCate
				&& MeteCate::OutOverVolt_Case2 != meteCate && MeteCate::HotLineWork_Case2 != meteCate)
			{
				singlerow.clear();
				singlerow.push_back(MeteCate::typeToStr(meteCate));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgHUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getVUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgCplxRatLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getTensionLoad()));
				contentVec.push_back(singlerow);
			}
		}
		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}

	



	{
		html += HtmlAutility::insertText("7.2、摇摆角计算所需工况张力计算结果", _fontSize_s4, 0, true);
		html += HtmlAutility::insertText("&nbsp;&nbsp;以第7.1节中所计算的控制工况为准，按照《DL/T 5582-2020 架空输电线路电气设计规程》计算大风、操作、雷电以及带电作业工况下的张力，并计算垂直档距。", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("表7.3 风荷载计算中间过程参数表", _fontSize_s4, 1, false);

		/*表头*/
		vector<string> headerRow;
		headerRow.push_back("工况类别");
		headerRow.push_back("导地线风荷载折减系数<i>γ</i><sub>c</sub>");
		headerRow.push_back("风压高度变化系数<i>μ</i><sub>z</sub>");
		headerRow.push_back("体型系数<i>μ</i><sub>sc</sub>");

		/*表内容*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);
			MeteCate::Type meteCate = meteRelCalProPara->getMtCate();
			if (MeteCate::Wind_Case2 == meteCate || MeteCate::InOverVolt_Case2 == meteCate
				|| MeteCate::OutOverVolt_Case2 == meteCate || MeteCate::HotLineWork_Case2 == meteCate)
			{
				singlerow.clear();
				singlerow.push_back(MeteCate::typeCase2ToStr(meteCate));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getGamma_c()));
			
				if (MeteCate::Wind == meteCate || MeteCate::Wind_Case2 == meteCate)
					singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getWireMu_z()));
				else
					singlerow.push_back("/");

				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getMu_sc()));
				contentVec.push_back(singlerow);
			}
		}
		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}



	{
		html += HtmlAutility::insertText("&nbsp;&nbsp;荷载和张力计算结果如表7.4所示。", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("表7.4 荷载和张力计算结果表", _fontSize_s4, 1, false);

		/*表头*/
		vector<string> headerRow;
		headerRow.push_back("工况类别");
		headerRow.push_back("水平荷载<br/>(N/m)");
		headerRow.push_back("垂直荷载<br/>(N/m)");
		headerRow.push_back("综合荷载<br/>(N/m)");
		headerRow.push_back("导线张力<br/>(N)");

		/*表内容*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _tensionCalcProPara->getCalProParaArraySize(); ++meteLoop)
		{
			HdlMeteRelCalProPara meteRelCalProPara = _tensionCalcProPara->getCalProParaArrayAt(meteLoop);
			MeteCate::Type meteCate = meteRelCalProPara->getMtCate();
			if (MeteCate::Wind_Case2 == meteCate || MeteCate::InOverVolt_Case2 == meteCate
				|| MeteCate::OutOverVolt_Case2 == meteCate || MeteCate::HotLineWork_Case2 == meteCate)
			{
				singlerow.clear();
				singlerow.push_back(MeteCate::typeCase2ToStr(meteCate));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgHUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getVUnitLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getAvgCplxRatLoad()));
				singlerow.push_back(Auxiliary::d2s(meteRelCalProPara->getTensionLoad()));
				contentVec.push_back(singlerow);
			}
		}

		html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
	}
	
}

//@function: 针对IVL不同间隙圆类型获取导线计算相关参数类   -- by mzq  2019/8/30 18:38
void Report_Straight::getWireCalaResPara()
{
	ClCate::Type clType = _cl->getClType();

	switch (clType)
	{
	case SmartCl::ClCate::StraightIStr:
	{
										  HdlCl_Stright_I cl_Stright_I = static_cast<Cl_Stright_I*>(_cl.GetPointee());
										 // _calProPara = cl_Stright_I->getElecLoadCalc()->getWire()->getCalProPara();//所有工况载荷计算过程数据
										  _tensionCalcProPara = cl_Stright_I->getElecLoadCalc()->getWire()->getTensionCalcProPara();//张力计算结果
										  _loadCalcProPara = cl_Stright_I->getElecLoadCalc()->getWire()->getLoadCalcProPara();//荷载计算结果
										  _angleCalcPara = cl_Stright_I->getElecLoadCalc()->getWire()->getAngleCalcPara();//大风操作雷电带电，摇摆角计算相关参数
										  break;
	}
	case SmartCl::ClCate::StraightVStr:
	{
										  HdlCl_Stright_V cl_Stright_V = static_cast<Cl_Stright_V*>(_cl.GetPointee());
										 // _calProPara = cl_Stright_V->getElecLoadCalc()->getWire()->getCalProPara();//所有工况载荷计算过程数据
										  _tensionCalcProPara = cl_Stright_V->getElecLoadCalc()->getWire()->getTensionCalcProPara();//张力计算结果
										  _loadCalcProPara = cl_Stright_V->getElecLoadCalc()->getWire()->getLoadCalcProPara();//荷载计算结果
										  _angleCalcPara = cl_Stright_V->getElecLoadCalc()->getWire()->getAngleCalcPara();//大风操作雷电带电，摇摆角计算相关参数
										  break;
	}
	case SmartCl::ClCate::StraightLStr:
	{
										  /*对于L串的，因为左肢和右肢张力计算结果都一致，所以这里张力计算结果默认取左肢，即计算alpha角度的*/
										  HdlCl_Stright_L cl_Stright_L = static_cast<Cl_Stright_L*>(_cl.GetPointee());
										 // _calProPara = cl_Stright_L->getElecLoadCalc_alpha()->getWire()->getCalProPara();//所有工况载荷计算过程数据，alpha侧和beta侧相同，这里取左肢
										  _tensionCalcProPara = cl_Stright_L->getElecLoadCalc_alpha()->getWire()->getTensionCalcProPara();//张力计算结果，所有工况载荷计算过程数据，alpha侧和beta侧相同，这里取左肢
										  _loadCalcProPara = cl_Stright_L->getElecLoadCalc_alpha()->getWire()->getLoadCalcProPara();//荷载计算结果，所有工况载荷计算过程数据，alpha侧和beta侧相同，这里取左肢
										  _angleCalcPara_alpha = cl_Stright_L->getElecLoadCalc_alpha()->getWire()->getAngleCalcPara();//alpha侧，大风操作雷电带电，摇摆角计算相关参数
										  _angleCalcPara_beta = cl_Stright_L->getElecLoadCalc_beta()->getWire()->getAngleCalcPara();//alpha侧，大风操作雷电带电，摇摆角计算相关参数
										  break;
	}
	default:
		break;
	}
}