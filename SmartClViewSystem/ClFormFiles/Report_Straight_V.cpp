#include "Report_Straight_V.h"
#include "DataUI_Stright_V.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"


//@function:    -- by mzq  2019/8/2 14:47
Report_Straight_V::Report_Straight_V(const string & clName) :Report_Straight(clName)
{
	HdlDataUI_Stright_V  dataUI_Stright_V = dynamic_cast<DataUI_Stright_V*>(_dataUI.GetPointee());
	_gapValuePara_VL = dataUI_Stright_V ->getGapValuePara_VL();
	_insultorPara_V = dataUI_Stright_V->getInsultorPara_V();
	_insulaGeoPara_V = dataUI_Stright_V->getInsulaGeoPara_V();
}

//@function:    -- by mzq  2019/8/2 14:48
Report_Straight_V::~Report_Straight_V()
{

}

//@function: ������Ϣ   -- by mzq  2019/8/3 23:16
void Report_Straight_V::wireInfo(QString &html) const
{
	html += HtmlAutility::insertText("3��������Ϣ", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;������Ϣ���3.1��ʾ��";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("��3.1 ������Ϣ��", _fontSize_s4, 1, false);

	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("�����ͺ�");
	singlerow.push_back(_wireRelatPara->getWPA()->getName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������");
	singlerow.push_back(SplitNum::typeToStr(_insulaGeoPara_V->getSplitNum()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("���Ѽ�� (m)");
	singlerow.push_back(_insulaGeoPara_V->getSplitShow());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������� (mm<sup>2</sup>)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getArea()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("ֱ�� (mm)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getDiameter()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������ (N)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getUltimateTension()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��λ���� (kg/m)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getUnitWeight()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("����ϵ�� (MPa)");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getWPA()->getE()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��������ϵ�� (1/��)");
	singlerow.push_back(QString::number(_wireRelatPara->getWPA()->getLineExpansionCof()).toLocal8Bit().toStdString());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��ȫϵ��");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getSaftyCof()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("����ϵ��");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getNewWireCof()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("ƽ����������ϵ��");
	singlerow.push_back(Auxiliary::d2s(_wireRelatPara->getAvgCof()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: ��Ե�Ӵ���Ϣ   -- by mzq  2019/8/4 11:07
void Report_Straight_V::insulatorInfo(QString &html) const
{
	html += HtmlAutility::insertText("4����Ե�Ӵ�������Ϣ", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;V����Ե�Ӵ�ʾ��ͼ��ͼ4.1��ʾ����Ե�Ӵ�������Ϣ���4.1��ʾ��";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	/*����I��ʾ��ͼƬ*/
	string picPath;
	switch (_insulaGeoPara_V->getSplitNum())
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
	html += HtmlAutility::insertText("ͼ4.1 V����Ե�Ӵ�ʾ��ͼ", _fontSize_s4, 1, false);


	/*�����Ե�Ӵ��������*/
	html += HtmlAutility::insertText("��4.1 ��Ե�Ӵ�������Ϣ��", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("��Ե�Ӵ��ͺ�");
	singlerow.push_back(_insultorPara_V->getInsultorName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("V�����ؼ�����֫���� (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_V->getInsultorWeight()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��֫��ֱ�����Ե�Ӵ����� (��)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_V->getSglNumSubs_V()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��֫������Ե�Ӵ�Ƭ�� (Ƭ)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_V->getNumPlate()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��Ƭ��Ե���ܷ���� (m<sup>2</sup>)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_V->getShieldArea()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("�ش����� (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_V->getHammerWeight()));
	contentVec.push_back(singlerow);


	singlerow.clear();
	singlerow.push_back("��ѹ�� (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getAlphaCorrectAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("V���нǦ��Ƽ�ֵ (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getAlphaRecommend()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("V���нǦ�ʵ��ȡֵ (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getAlpha()));
	contentVec.push_back(singlerow);


	singlerow.clear();
	singlerow.push_back("L1 V����֫���� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getL1()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2 ��ѹ���߶� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getL2()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3 ��ѹ����� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getL3()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L4 ������ (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getL4()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L5 �߼йҿ�������ҿ״��� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getL5()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L6 �ӵ����߼г��� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getL6()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L7 С���� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getL7()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("�� �߼м�С����ƫ�� (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_V->getTheta()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: ��϶ֵ��Ϣ   -- by mzq  2019/8/4 17:11
void Report_Straight_V::gapInfo(QString &html) const
{
	html += HtmlAutility::insertText("5��������϶", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;���θ߶ȼ���Ӧ�ļ�϶ֵ���5.1��ʾ��";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	/*���������϶���*/
	html += HtmlAutility::insertText("��5.1 ������϶��", _fontSize_s4, 1, false);

	html += HtmlAutility::tableStyle(_fontSize_5);
	html += "<table class=\"gridtable\" border=\"1\" cellspacing=\"0\" width=\"615\">";

	html += "<tr>";
	html += HtmlAutility::insertTableHeaderCell("��������", 1, 2);
	html += HtmlAutility::insertTableHeaderCell("������Ϣ");
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("���θ߶� (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_basicInfo->getAltitude()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("��Ƶ��ѹ��϶  (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getPowerFreq_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("��������ѹ��϶  (m)", 2, 1);
	html += HtmlAutility::insertTableTextCell("���ࣨ������");
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getInOverVolt_Down_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("���ࣨ��������");
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getInOverVolt_Up_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("�׵����ѹ��϶  (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getOutOverVolt_Gap()));
	html += "</tr>";

	/*�Ƿ��Ǵ�����ҵ*/
	if (_gapValuePara_VL->getIsHotLineWork())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("������ҵ��С��϶ (m)", 2, 1);
		html += HtmlAutility::insertTableTextCell("���ࣨ������");
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getHotLineWork_Down_Gap()));
		html += "</tr>";

		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("���ࣨ��������");
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getHotLineWork_Up_Gap()));
		html += "</tr>";

		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("������ҵ������Χ (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getHotLineWorkRange()));
		html += "</tr>";
	}

	/*�Ƿ��Ƕ�����ԣ��*/
	if (_gapValuePara_VL->getIsMargin_up())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("������ԣ�� (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getMarginValue_up()));
		html += "</tr>";
	}

	/*�Ƿ��Ƕ�����ԣ��*/
	if (_gapValuePara_VL->getIsMargin_down())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("������ԣ�� (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_VL->getMarginValue_down()));
		html += "</tr>";
	}

	html += "</table>";
}

//@function: ҡ�ڽǼ�����   -- by mzq  2019/8/30 21:02
void Report_Straight_V::swingAngleRes(QString &html) const
{
	if (CodeCate::DLT5582_2020 != _calcRelePara->getCodeType())
		swingAngleRes_Code2010(html);
	else
		swingAngleRes_Code2020(html);
}

//@function: ҡ�ڽǼ����� 2010�淶   -- by mzq 2020/12/14 19:19
void Report_Straight_V::swingAngleRes_Code2010(QString &html) const
{
	html += HtmlAutility::insertText("8��ҡ�ڽǼ�����", _fontSize_s3, 0, true);

	double alphaCorrectAngle = _insulaGeoPara_V->getAlphaCorrectAngle();
	double alphaRecommend = _insulaGeoPara_V->getAlphaRecommend();
	double alpha = _insulaGeoPara_V->getAlpha();
	std::string text = "&nbsp;&nbsp;V���н��Ƽ�ֵ����ʱ����V���ĵ�֫������Ч��I������ҡ�ڽǼ��㣬��ЧI�����������8.1��ʾ��V���нǦ��Ƽ�ֵ=2��(��ЧI����繤��ҡ�ڽ�-��ѹ��)����ѹ��Ϊ"
		+ Auxiliary::d2s(alphaCorrectAngle) + "�ȣ���������ɵ�<strong>���Ƽ�ֵΪ" + Auxiliary::d2s(alphaRecommend) + "�ȣ�"
		+ "��ʵ��ȡֵ" + Auxiliary::d2s(alpha) + "�ȡ�</strong>";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("��8.1 ��ЧI��ҡ�ڽǼ�������", _fontSize_s4, 1, false);

	/*��ͷ*/
	vector<string> headerRow;
	headerRow.push_back("�������");
	headerRow.push_back("����ˮƽ����<br/>(N/m)");
	headerRow.push_back("��Ե�Ӵ���ѹ<br/>(N)");
	headerRow.push_back("ҡ�ڽ�<br/>(��)");

	/*������*/
	vector<vector<string>> contentVec;
	vector<string> singlerow;
	for (int meteLoop = 0; meteLoop < _angleCalcPara->getSize(); ++meteLoop)
	{
		HdlSwingAnglePara swingAnglePara = _angleCalcPara->getAt(meteLoop);

		singlerow.clear();
		singlerow.push_back(MeteCate::typeToStr(swingAnglePara->getMeteCate()));
		singlerow.push_back(Auxiliary::d2s(swingAnglePara->getHRatLoad()));
		singlerow.push_back(Auxiliary::d2s(swingAnglePara->getInsuHLoad()));
		singlerow.push_back(Auxiliary::d2s(swingAnglePara->getSwingAngle()));
		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: ҡ�ڽǼ����� 2020�淶   -- by mzq 2020/12/14 19:19
void Report_Straight_V::swingAngleRes_Code2020(QString &html) const
{
	html += HtmlAutility::insertText("8��ҡ�ڽǼ�����", _fontSize_s3, 0, true);

	double alphaCorrectAngle = _insulaGeoPara_V->getAlphaCorrectAngle();
	double alphaRecommend = _insulaGeoPara_V->getAlphaRecommend();
	double alpha = _insulaGeoPara_V->getAlpha();
	std::string text = "&nbsp;&nbsp;V���н��Ƽ�ֵ����ʱ����V���ĵ�֫������Ч��I������ҡ�ڽǼ��㡣V���нǦ��Ƽ�ֵ=2��(��ЧI����繤��ҡ�ڽ�-��ѹ��)����ѹ��Ϊ"
		+ Auxiliary::d2s(alphaCorrectAngle) + "�ȣ���������ɵ�<strong>���Ƽ�ֵΪ" + Auxiliary::d2s(alphaRecommend) + "�ȣ�"
		+ "��ʵ��ȡֵ" + Auxiliary::d2s(alpha) + "�ȡ�</strong>";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);


	{
		html += HtmlAutility::insertText("&nbsp;&nbsp;������ؼ��������8.1��ʾ��", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("��8.1 ������ؼ�������", _fontSize_s4, 1, false);

		/*��ͷ*/
		vector<string> headerRow;
		headerRow.push_back("����<br/>���");
		headerRow.push_back("���<br/>����<br/>��ϵ<br/>��<br/><i>��</i><sub>c</sub>");
		headerRow.push_back("���<br/>ϵ��<br/><i>��</i><sub>c</sub>");
		headerRow.push_back("����<br/>�ۼ�<br/>ϵ��<br/><i>��</i><sub>L</sub>");
		headerRow.push_back("��ѹ<br/>�߶�<br/>�仯<br/>ϵ��<br/><i>��</i><sub>z</sub>");
		headerRow.push_back("����<br/>ˮƽ<br/>����<br/>(N/m)");
		headerRow.push_back("����<br/>��ֱ<br/>����<br/>(N/m)");
		headerRow.push_back("����<br/>��ƫ<br/>��<br/>(��)");
		headerRow.push_back("��ֱ<br/>����<br/>(m)");

		/*������*/
		vector<vector<string>> contentVec;
		vector<string> singlerow;
		for (int meteLoop = 0; meteLoop < _angleCalcPara->getSize(); ++meteLoop)
		{
			HdlSwingAnglePara swingAnglePara = _angleCalcPara->getAt(meteLoop);
			MeteCate::Type meteCate = swingAnglePara->getMeteCate();
			HdlMeteRelCalProPara loadCurMtCalPara = _loadCalcProPara->find(meteCate);//���ؼ�����

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
		html += HtmlAutility::insertText("&nbsp;&nbsp;��Ե�Ӵ���ؼ��������8.2��ʾ��", _fontSize_s4, 0, false);
		html += HtmlAutility::insertText("��8.2 ��Ե�Ӵ���ؼ�������", _fontSize_s4, 1, false);

		/*��ͷ*/
		vector<string> headerRow;
		headerRow.push_back("�������");
		headerRow.push_back("��Ե�Ӵ���ѹ<br/>(N)");
		headerRow.push_back("��Ե�Ӵ�����<br/>(N)");
		headerRow.push_back("�ش�����<br/>(N)");
		headerRow.push_back("ҡ�ڽ�<br/>(��)");

		/*������*/
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
