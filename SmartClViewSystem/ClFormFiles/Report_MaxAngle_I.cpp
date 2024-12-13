#include "Report_MaxAngle_I.h"
#include "Auxiliary.h"
#include "HtmlAutility.h"

Report_MaxAngle_I::Report_MaxAngle_I(const string &clName) :Report_MaxAngle(clName)
{
	_dataUI_MaxAngle_I = dynamic_cast<DataUI_MaxAngle_I*>(_dataUI.GetPointee());
	_gapValuePara_I = _dataUI_MaxAngle_I->getGapValuePara_I();//I �����ֹ�����Ӧ�ļ�϶ֵ����
	_insulaHangPara_I = _dataUI_MaxAngle_I->getInsulaHangPara_I();//��Ե�Ӵ����β���
}

Report_MaxAngle_I::~Report_MaxAngle_I()
{

}

//@function: ��϶ֵ��Ϣ   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_I::gapInfo(QString &html) const
{
	html += HtmlAutility::insertText("2.������϶", _fontSize_s3, 0, true);
	string text = "&nbsp;&nbsp;��϶ֵ��Ϣ��� 2.1��ʾ��";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("��2.1 ������϶��", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("��Ƶ��ѹ��϶ (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getPowerFreq_Gap()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��������ѹ��϶ (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getInOverVolt_Gap()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("�׵����ѹ��϶ (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getOutOverVolt_Gap()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������ҵ��С��϶ (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getHotLineWork_Gap()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������ҵ������Χ (m)");
	singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getHotLineWorkRange()));
	contentVec.push_back(singlerow);

	/*�Ƿ���ԣ��*/
	if (_gapValuePara_I->getIsMargin_down())
	{
		singlerow.clear();
		singlerow.push_back("ԣ��ֵ (m)");
		singlerow.push_back(Auxiliary::d2s(_gapValuePara_I->getMarginValue_down()));
		contentVec.push_back(singlerow);
	}

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: ��Ե�Ӵ���Ϣ   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_I::insulatorInfo(QString &html) const
{
	//3.��Ե�Ӵ�����
	html += HtmlAutility::insertText("3.��Ե�Ӵ�����", _fontSize_s3, 0, true);

	HdlJPoint hangPoint = _insulaHangPara_I->getHangPoint();
	string text = QString::fromLocal8Bit("&nbsp;&nbsp;��Ե��I���ҵ��Ϊ:%1������Ϊ��%2��%3, %4����")
		.arg(QString::number(hangPoint->getTagNum()))
		.arg(QString::number(hangPoint->getX()))
		.arg(QString::number(hangPoint->getY()))
		.arg(QString::number(hangPoint->getZ())).toLocal8Bit();

	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	string text1 = "&nbsp;&nbsp;��Ե�Ӵ����β���ʾ��ͼ��ͼ3.1��ʾ�����������3.1��ʾ";
	html += HtmlAutility::insertText(text1, _fontSize_s4, 0, false);

	HdlMdl_InsulaGeoPara_I insulaGeoPara_I = _insulaHangPara_I->getInsulaGeoPara_I();
	/*����I��ʾ��ͼƬ*/
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
	html += HtmlAutility::insertText("ͼ4.1 I����Ե�Ӵ�ʾ��ͼ", _fontSize_s4, 1, false);

	/*�����Ե�Ӵ��������*/
	html += HtmlAutility::insertText("��4.1 ��Ե�Ӵ�������Ϣ��", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("������");
	singlerow.push_back(SplitNum::typeToStr(insulaGeoPara_I->getSplitNum()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("���Ѽ�� (m)");
	singlerow.push_back(insulaGeoPara_I->getSplitShow());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L1 �����ҵ�-��ߵ�һת���� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL1()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2 �����ҵ�-���ӵ����߼йҵ� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL2()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3 ���ӵ����߼йҵ�-��ѹ�� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL3()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L4 ��ѹ����� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL4()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L5 �ӵ����߼г��� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL5()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L6 С���� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_I->getL6()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: �Ƕȼ�����   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_I::angleRes(QString &html) const
{
	html += HtmlAutility::insertText("4.�������ҡ�ڽ�", _fontSize_s3, 0, true);
	html += HtmlAutility::insertText("�������������ҡ�ڽ����4.1��ʾ��", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("�������");
	headerRow.push_back("�������ҡ�ڽ� (��)");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("��Ƶ");
	singlerow.push_back(Auxiliary::d2s(_dataUI_MaxAngle_I->getPowerFreqAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("����");
	singlerow.push_back(Auxiliary::d2s(_dataUI_MaxAngle_I->getInOverVoltAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("�׵�");
	singlerow.push_back(Auxiliary::d2s(_dataUI_MaxAngle_I->getOutOverVoltAngle()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("������ҵ");
	singlerow.push_back(Auxiliary::d2s(_dataUI_MaxAngle_I->getHotLineWorkAngle()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

