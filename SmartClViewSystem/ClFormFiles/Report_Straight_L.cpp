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

//@function: ������Ϣ   -- by mzq  2019/8/3 23:16
void Report_Straight_L::wireInfo(QString &html) const
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
	singlerow.push_back(SplitNum::typeToStr(_insulaGeoPara_L->getSplitNum()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("���Ѽ�� (m)");
	singlerow.push_back(_insulaGeoPara_L->getSplitShow());
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

//@function: ��Ե�Ӵ���Ϣ   -- by mzq  2019/8/4 11:08
void Report_Straight_L::insulatorInfo(QString &html) const
{
	html += HtmlAutility::insertText("4����Ե�Ӵ�������Ϣ", _fontSize_s3, 0, true);

	string text = "&nbsp;&nbsp;L����Ե�Ӵ�ʾ��ͼ��ͼ4.1��ʾ����Ե�Ӵ�������Ϣ���4.1��ʾ��";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);

	/*����I��ʾ��ͼƬ*/
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
	html += HtmlAutility::insertText("ͼ4.1 L����Ե�Ӵ�ʾ��ͼ", _fontSize_s4, 1, false);


	/*�����Ե�Ӵ��������*/
	html += HtmlAutility::insertText("��4.1 ��Ե�Ӵ�������Ϣ��", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("��Ե�Ӵ��ͺ�");
	singlerow.push_back(_insultorPara_L->getInsultorName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L����������b֫���� (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_L->getInsultorWeight_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L����������a֫���� (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_L->getInsultorWeight_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("a֫��ֱ�����Ե�Ӵ����� (��)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_L->getNumSubs_a_V()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("b֫��ֱ�����Ե�Ӵ����� (��)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_L->getNumSubs_b_V()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("a֫������Ե�Ӵ�Ƭ�� (Ƭ)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_L->getNumPlate_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("b֫������Ե�Ӵ�Ƭ�� (Ƭ)");
	singlerow.push_back(Auxiliary::i2s(_insultorPara_L->getNumPlate_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��Ƭ��Ե���ܷ���� (m<sup>2</sup>)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_L->getShieldArea()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("�ش����� (N)");
	singlerow.push_back(Auxiliary::d2s(_insultorPara_L->getHammerWeight()));
	contentVec.push_back(singlerow);


	singlerow.clear();
	singlerow.push_back("����ѹ�� (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getAlpha_correctAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("���Ƽ�ֵ (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getAlpha_recommend()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��ʵ��ȡֵ (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getAlpha()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("����ѹ�� (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getBeta_correctAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("���Ƽ�ֵ (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getBeta_recommend()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��ʵ��ȡֵ (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getBeta()));
	contentVec.push_back(singlerow);


	singlerow.clear();
	singlerow.push_back("L1_a a֫��֫���� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL1_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L1_b b֫��֫���� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL1_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2_a a֫��ѹ���߶� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL2_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2_b b֫��ѹ���߶� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL2_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3_a a֫��ѹ����� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL3_a()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3_b b֫��ѹ����� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL3_b()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L4 ������ (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL4()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L5 �߼йҿ�������ҿ״��� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL5()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L6 �ӵ����߼г��� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL6()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L7 С���� (m)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getL7()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("�� �߼м�С����ƫ�� (��)");
	singlerow.push_back(Auxiliary::d2s(_insulaGeoPara_L->getTheta()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: ��϶ֵ��Ϣ   -- by mzq  2019/8/4 17:11
void Report_Straight_L::gapInfo(QString &html) const
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
void Report_Straight_L::swingAngleRes(QString &html) const
{
	if (CodeCate::DLT5582_2020 != _calcRelePara->getCodeType())
		swingAngleRes_Code2010(html);
	else
		swingAngleRes_Code2020(html);
}

//@function: ҡ�ڽǼ����� 2010�淶   -- by mzq 2020/12/15 16:19
void Report_Straight_L::swingAngleRes_Code2010(QString &html) const
{
	html += HtmlAutility::insertText("8��ҡ�ڽǼ�����", _fontSize_s3, 0, true);

	/**********************************************L����֫ alpha********************************************************/
	double alphaCorrectAngle = _insulaGeoPara_L->getAlpha_correctAngle();
	double alphaRecommend = _insulaGeoPara_L->getAlpha_recommend();
	double alpha = _insulaGeoPara_L->getAlpha();
	std::string text_alpha = "&nbsp;&nbsp;L����֫�нǦ��Ƽ�ֵ����ʱ����L������֫������Ч��I������ҡ�ڽǼ��㣬ȡת�Ƕ���Ϊ"
		+ Auxiliary::d2s(-_basicInfo->getRotatAngle())
		+ "�ȣ�L����֫��ЧI�����������8.1��ʾ��L����֫�нǦ��Ƽ�ֵ=��ЧI����繤��ҡ�ڽ�-����ѹ�ǣ�����ѹ��Ϊ"
		+ Auxiliary::d2s(alphaCorrectAngle) + "�ȣ���������ɵ�<strong>���Ƽ�ֵΪ" + Auxiliary::d2s(alphaRecommend) + "�ȣ�"
		+ "��ʵ��ȡֵΪ" + Auxiliary::d2s(alpha) + "�ȡ�</strong>";
	html += HtmlAutility::insertText(text_alpha, _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("��8.1 L����֫��ЧI��ҡ�ڽǼ�������", _fontSize_s4, 1, false);

	/*��ͷ*/
	vector<string> headerRow_alpha;
	headerRow_alpha.push_back("�������");
	headerRow_alpha.push_back("����ˮƽ����<br/>(N/m)");
	headerRow_alpha.push_back("��Ե�Ӵ���ѹ<br/>(N)");
	headerRow_alpha.push_back("ҡ�ڽ�<br/>(��)");

	/*������*/
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





	/**********************************************L����֫ beta********************************************************/
	double betaCorrectAngle = _insulaGeoPara_L->getBeta_correctAngle();
	double betaRecommend = _insulaGeoPara_L->getBeta_recommend();
	double beta = _insulaGeoPara_L->getBeta();
	std::string text_beta = "&nbsp;&nbsp;L����֫�нǦ��Ƽ�ֵ����ʱ����L������֫������Ч��I������ҡ�ڽǼ��㣬ȡת�Ƕ���Ϊ"
		+ Auxiliary::d2s(_basicInfo->getRotatAngle())
		+ "�ȣ�L����֫��ЧI�����������8.2��ʾ��L����֫�нǦ��Ƽ�ֵ=��ЧI����繤��ҡ�ڽ�-����ѹ�ǣ�����ѹ��Ϊ"
		+ Auxiliary::d2s(betaCorrectAngle) + "�ȣ���������ɵ�<strong>���Ƽ�ֵΪ" + Auxiliary::d2s(betaRecommend) + "�ȣ�"
		+ "��ʵ��ȡֵΪ" + Auxiliary::d2s(beta) + "�ȡ�</strong>";
	html += HtmlAutility::insertText(text_beta, _fontSize_s4, 0, false);

	html += HtmlAutility::insertText("��8.2 L����֫��ЧI��ҡ�ڽǼ�������", _fontSize_s4, 1, false);

	/*��ͷ*/
	vector<string> headerRow_beta;
	headerRow_beta.push_back("�������");
	headerRow_beta.push_back("����ˮƽ����<br/>(N/m)");
	headerRow_beta.push_back("��Ե�Ӵ���ѹ<br/>(N)");
	headerRow_beta.push_back("ҡ�ڽ�<br/>(��)");

	/*������*/
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

//@function: ҡ�ڽǼ����� 2020�淶   -- by mzq 2020/12/15 16:19
void Report_Straight_L::swingAngleRes_Code2020(QString &html) const
{
	html += HtmlAutility::insertText("8��ҡ�ڽǼ�����", _fontSize_s3, 0, true);

	{
		/**********************************************L����֫ alpha********************************************************/
		html += HtmlAutility::insertText("8.1��L����֫������", _fontSize_s4, 0, true);

		double alphaCorrectAngle = _insulaGeoPara_L->getAlpha_correctAngle();
		double alphaRecommend = _insulaGeoPara_L->getAlpha_recommend();
		double alpha = _insulaGeoPara_L->getAlpha();
		std::string text_alpha = "&nbsp;&nbsp;L����֫�нǦ��Ƽ�ֵ����ʱ����L������֫������Ч��I������ҡ�ڽǼ��㣬ȡת�Ƕ���Ϊ"
			+ Auxiliary::d2s(-_basicInfo->getRotatAngle())
			+ "�ȣ�L����֫��ЧI�����������8.1��8.2��ʾ��L����֫�нǦ��Ƽ�ֵ=��ЧI����繤��ҡ�ڽ�-����ѹ�ǣ�����ѹ��Ϊ"
			+ Auxiliary::d2s(alphaCorrectAngle) + "�ȣ���������ɵ�<strong>���Ƽ�ֵΪ" + Auxiliary::d2s(alphaRecommend) + "�ȣ�"
			+ "��ʵ��ȡֵΪ" + Auxiliary::d2s(alpha) + "�ȡ�</strong>";
		html += HtmlAutility::insertText(text_alpha, _fontSize_s4, 0, false);


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
			for (int meteLoop = 0; meteLoop < _angleCalcPara_alpha->getSize(); ++meteLoop)
			{
				HdlSwingAnglePara swingAnglePara = _angleCalcPara_alpha->getAt(meteLoop);
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
		/**********************************************L����֫ beta********************************************************/
		html += HtmlAutility::insertText("8.2��L����֫������", _fontSize_s4, 0, true);
		double betaCorrectAngle = _insulaGeoPara_L->getBeta_correctAngle();
		double betaRecommend = _insulaGeoPara_L->getBeta_recommend();
		double beta = _insulaGeoPara_L->getBeta();
		std::string text_beta = "&nbsp;&nbsp;L����֫�нǦ��Ƽ�ֵ����ʱ����L������֫������Ч��I������ҡ�ڽǼ��㣬ȡת�Ƕ���Ϊ"
			+ Auxiliary::d2s(_basicInfo->getRotatAngle())
			+ "�ȣ�L����֫��ЧI�����������8.3��8.4��ʾ��L����֫�нǦ��Ƽ�ֵ=��ЧI����繤��ҡ�ڽ�-����ѹ�ǣ�����ѹ��Ϊ"
			+ Auxiliary::d2s(betaCorrectAngle) + "�ȣ���������ɵ�<strong>���Ƽ�ֵΪ" + Auxiliary::d2s(betaRecommend) + "�ȣ�"
			+ "��ʵ��ȡֵΪ" + Auxiliary::d2s(beta) + "�ȡ�</strong>";
		html += HtmlAutility::insertText(text_beta, _fontSize_s4, 0, false);


		{
			html += HtmlAutility::insertText("&nbsp;&nbsp;������ؼ��������8.3��ʾ��", _fontSize_s4, 0, false);
			html += HtmlAutility::insertText("��8.3 ������ؼ�������", _fontSize_s4, 1, false);

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
			for (int meteLoop = 0; meteLoop < _angleCalcPara_beta->getSize(); ++meteLoop)
			{
				HdlSwingAnglePara swingAnglePara = _angleCalcPara_beta->getAt(meteLoop);
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
			html += HtmlAutility::insertText("&nbsp;&nbsp;��Ե�Ӵ���ؼ��������8.4��ʾ��", _fontSize_s4, 0, false);
			html += HtmlAutility::insertText("��8.4 ��Ե�Ӵ���ؼ�������", _fontSize_s4, 1, false);

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
