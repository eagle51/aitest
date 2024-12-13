#include "Report_MaxAngle_V.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"

Report_MaxAngle_V::Report_MaxAngle_V(const string & clName) :Report_MaxAngle(clName)
{
	_dataUI_MaxAngle_V = dynamic_cast<DataUI_MaxAngle_V*>(_dataUI.GetPointee());
	_gapValuePara_V = _dataUI_MaxAngle_V->getGapValuePara_VL();//I �����ֹ�����Ӧ�ļ�϶ֵ����
	_insulaHangPara_V = _dataUI_MaxAngle_V->getInsulaHangPara_V();//��Ե�Ӵ����β���
}

Report_MaxAngle_V::~Report_MaxAngle_V()
{

}

//@function: ��϶ֵ��Ϣ   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_V::gapInfo(QString &html) const
{
	html += HtmlAutility::insertText("2.������϶", _fontSize_s3, 0, true);
	string text = "&nbsp;&nbsp;��϶ֵ��Ϣ���2.1��ʾ��";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("��2.1 ������϶��", _fontSize_s4, 1, false);

	html += HtmlAutility::tableStyle(_fontSize_5);
	html += "<table class=\"gridtable\" border=\"1\" cellspacing=\"0\" width=\"615\">";

	html += "<tr>";
	html += HtmlAutility::insertTableHeaderCell("��������", 1, 2);
	html += HtmlAutility::insertTableHeaderCell("������Ϣ");
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("��Ƶ��ѹ��϶  (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getPowerFreq_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("��������ѹ��϶  (m)", 2, 1);
	html += HtmlAutility::insertTableTextCell("���ࣨ������");
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getInOverVolt_Down_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("���ࣨ��������");
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getInOverVolt_Up_Gap()));
	html += "</tr>";

	html += "<tr>";
	html += HtmlAutility::insertTableTextCell("�׵����ѹ��϶  (m)", 1, 2);
	html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getOutOverVolt_Gap()));
	html += "</tr>";

	/*�Ƿ��Ǵ�����ҵ*/
	if (_gapValuePara_V->getIsHotLineWork())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("������ҵ��С��϶ (m)", 2, 1);
		html += HtmlAutility::insertTableTextCell("���ࣨ������");
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getHotLineWork_Down_Gap()));
		html += "</tr>";

		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("���ࣨ��������");
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getHotLineWork_Up_Gap()));
		html += "</tr>";

		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("������ҵ������Χ (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getHotLineWorkRange()));
		html += "</tr>";
	}

	/*�Ƿ��Ƕ�����ԣ��*/
	if (_gapValuePara_V->getIsMargin_up())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("������ԣ�� (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getMarginValue_up()));
		html += "</tr>";
	}

	/*�Ƿ��Ƕ�����ԣ��*/
	if (_gapValuePara_V->getIsMargin_down())
	{
		html += "<tr>";
		html += HtmlAutility::insertTableTextCell("������ԣ�� (m)", 1, 2);
		html += HtmlAutility::insertTableTextCell(Auxiliary::d2s(_gapValuePara_V->getMarginValue_down()));
		html += "</tr>";
	}

	html += "</table>";
}

//@function: ��Ե�Ӵ���Ϣ   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_V::insulatorInfo(QString &html) const
{
	//3.��Ե�Ӵ�����
	html += HtmlAutility::insertText("3.��Ե�Ӵ�����", _fontSize_s3, 0, true);

	HdlJPoint hangPointLeft = _insulaHangPara_V->getHangPointLeft();
	string textLeftPoint = QString::fromLocal8Bit("&nbsp;&nbsp;V�����ҵ��Ϊ:%1������Ϊ��%2��%3, %4����")
		.arg(QString::number(hangPointLeft->getTagNum()))
		.arg(QString::number(hangPointLeft->getX()))
		.arg(QString::number(hangPointLeft->getY()))
		.arg(QString::number(hangPointLeft->getZ())).toLocal8Bit();
	html += HtmlAutility::insertText(textLeftPoint, _fontSize_s4, 0, false);

	HdlJPoint hangPointRight = _insulaHangPara_V->getHangPointRight();
	string textRightPoint = QString::fromLocal8Bit("&nbsp;&nbsp;V���Ҳ�ҵ��Ϊ:%1������Ϊ��%2��%3, %4����")
		.arg(QString::number(hangPointRight->getTagNum()))
		.arg(QString::number(hangPointRight->getX()))
		.arg(QString::number(hangPointRight->getY()))
		.arg(QString::number(hangPointRight->getZ())).toLocal8Bit();
	html += HtmlAutility::insertText(textRightPoint, _fontSize_s4, 0, false);

	HdlMdl_InsulaGeoPara_V insulaGeoPara_V = _insulaHangPara_V->getInsulaGeoPara_V();
	/*����I��ʾ��ͼƬ*/
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
	html += HtmlAutility::insertText("ͼ4.1 V����Ե�Ӵ�ʾ��ͼ", _fontSize_s4, 1, false);

	/*�����Ե�Ӵ��������*/
	html += HtmlAutility::insertText("��4.1 ��Ե�Ӵ�������Ϣ��", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("������");
	singlerow.push_back(SplitNum::typeToStr(insulaGeoPara_V->getSplitNum()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("���Ѽ�� (m)");
	singlerow.push_back(insulaGeoPara_V->getSplitShow());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L1 V����֫���� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL1()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L2 ��ѹ���߶� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL2()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L3 ��ѹ����� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL3()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L4 ������ (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL4()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L5 �߼йҿ�������ҿ״��� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL5()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L6 �ӵ����߼г��� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL6()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("L7 С���� (m)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getL7()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("�� �߼м�С����ƫ��(��)");
	singlerow.push_back(Auxiliary::d2s(insulaGeoPara_V->getTheta()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}

//@function: �Ƕȼ�����   -- by mzq  2020/1/6 17:07
void Report_MaxAngle_V::angleRes(QString &html) const
{
	html += HtmlAutility::insertText("4.�������ҡ�ڽ�", _fontSize_s3, 0, true);

	if (_dataUI_MaxAngle_V->getMaxAngle() < 360)
	{
		string resStr = QString::fromLocal8Bit("&nbsp;&nbsp;V���������н�Ϊ%1�ȡ�")
			.arg(QString::number(_dataUI_MaxAngle_V->getMaxAngle())).toLocal8Bit();

		html += HtmlAutility::insertText(resStr, _fontSize_s4, 0, false);
	}
	else
	{
		string resStr = "&nbsp;&nbsp;V���������нǼ��㲻������";

		html += HtmlAutility::insertText(resStr, _fontSize_s4, 0, false);
	}

	if (_dataUI_MaxAngle_V->getMinAngle() < 360)
	{
		string resStr = QString::fromLocal8Bit("&nbsp;&nbsp;V����С����н�Ϊ%1�ȡ�")
			.arg(QString::number(_dataUI_MaxAngle_V->getMinAngle())).toLocal8Bit();

		html += HtmlAutility::insertText(resStr, _fontSize_s4, 0, false);
	}
	else
	{
		string resStr = "&nbsp;&nbsp;V����С����нǼ��㲻������";
		html += HtmlAutility::insertText(resStr, _fontSize_s4, 0, false);
	}
}