#include "Report_MaxAngle.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"

Report_MaxAngle::Report_MaxAngle(const string & clName) :Report(clName)
{
	_dataUI = dynamic_cast<DataUI_MaxAngle*>(_cl->getDataUI().GetPointee());

	_basicInfo = _dataUI->getBasicInfo();//������Ϣ
}

Report_MaxAngle::~Report_MaxAngle()
{

}

//@function: д��������   -- by mzq  2019/12/27 18:38
void Report_MaxAngle::writeContent(QString &html) const
{
	baseInfo(html);
	gapInfo(html);
	insulatorInfo(html);
	angleRes(html);
}

//@function: �����ſ�   -- by mzq  2019/8/2 11:35
void Report_MaxAngle::baseInfo(QString &html) const
{
	/*�����ı�*/
	html += HtmlAutility::insertText("1.������Ϣ", _fontSize_s3, 0, true);
	string text = "&nbsp;&nbsp;������Ϣ���1.1��ʾ��";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("��1.1 ������Ϣ��", _fontSize_s4, 1, false);
	vector<string> headerRow;
	headerRow.push_back("��������");
	headerRow.push_back("������Ϣ");

	vector<vector<string>> contentVec;
	vector<string> singlerow;
	singlerow.clear();
	singlerow.push_back("��������");
	singlerow.push_back(_basicInfo->getTowName());
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��ѹ�ȼ�");
	singlerow.push_back(VoltType::typeToStr(_basicInfo->getVoltType()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("��·����");
	singlerow.push_back(LoopType::typeToStr(_basicInfo->getLoopType()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("���θ߶� (m)");
	singlerow.push_back(Auxiliary::d2s(_basicInfo->getAltitude()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}