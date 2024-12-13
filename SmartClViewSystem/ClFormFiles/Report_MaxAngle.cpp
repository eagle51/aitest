#include "Report_MaxAngle.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"

Report_MaxAngle::Report_MaxAngle(const string & clName) :Report(clName)
{
	_dataUI = dynamic_cast<DataUI_MaxAngle*>(_cl->getDataUI().GetPointee());

	_basicInfo = _dataUI->getBasicInfo();//基本信息
}

Report_MaxAngle::~Report_MaxAngle()
{

}

//@function: 写具体内容   -- by mzq  2019/12/27 18:38
void Report_MaxAngle::writeContent(QString &html) const
{
	baseInfo(html);
	gapInfo(html);
	insulatorInfo(html);
	angleRes(html);
}

//@function: 杆塔概况   -- by mzq  2019/8/2 11:35
void Report_MaxAngle::baseInfo(QString &html) const
{
	/*正文文本*/
	html += HtmlAutility::insertText("1.基本信息", _fontSize_s3, 0, true);
	string text = "&nbsp;&nbsp;基本信息如表1.1所示。";
	html += HtmlAutility::insertText(text, _fontSize_s4, 0, false);
	html += HtmlAutility::insertText("表1.1 基本信息表", _fontSize_s4, 1, false);
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
	singlerow.push_back("电压等级");
	singlerow.push_back(VoltType::typeToStr(_basicInfo->getVoltType()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("回路类型");
	singlerow.push_back(LoopType::typeToStr(_basicInfo->getLoopType()));
	contentVec.push_back(singlerow);

	singlerow.clear();
	singlerow.push_back("海拔高度 (m)");
	singlerow.push_back(Auxiliary::d2s(_basicInfo->getAltitude()));
	contentVec.push_back(singlerow);

	html += HtmlAutility::insertTable(headerRow, contentVec, _fontSize_5, 1);
}