#include "Report.h"
#include "Clearence.h"
#include "ClWidget.h"
#include "HtmlAutility.h"
#include "Auxiliary.h"
#include "EnumLib.h"
#include <fstream>
#include "DataBase.h"

#include "qfile.h"
#include "qtextstream.h"
#include <QPrinter>
#include <QTextDocument>
#include <QTextBlock>
#include "qdatetime.h"

Report::Report(const string & clName)
{
	_cl = ClWidget::getCoreInstance()->getClByName(clName);

	_fontSize_s2 = 24;//小二号
	_fontSize_s3 = 20;//小三号
	_fontSize_4 = 18.7;//四号
	_fontSize_s4 = 16;//小四号
	_fontSize_5 = 14;//五号
}

Report::~Report()
{

}

//@function: 生成报告   -- by mzq  2019/7/31 10:58
void Report::wordReport() const
{
	string savePathStr = ClWidget::getCoreInstance()->getPathFolder() + "\\" + "间隙圆计算书" + "\\" + _cl->getClName() + ".pdf";
	std::ofstream writeFile(savePathStr);
	if (!writeFile)
		throw std::exception("计算书生成失败，已经打开或者文件被占用！\n请先关闭或解除占用。");

	QString html = "";

	firstPage(html);
	writeContent(html);

	html += "</body></html>";

	QPrinter printer_html;
	printer_html.setPageSize(QPrinter::A4);
	printer_html.setOutputFormat(QPrinter::PdfFormat);
	QString savePath = QString::fromLocal8Bit(savePathStr.c_str());
	printer_html.setOutputFileName(savePath);
	QTextDocument *text_document = new QTextDocument();
	text_document->setHtml(html);
	text_document->print(&printer_html);
	text_document->end();
	delete text_document;

	/*转换成word格式*/
	string wordSavePathStr = ClWidget::getCoreInstance()->getPathFolder() + "\\" + "间隙圆计算书" + "\\" + _cl->getClName() + ".doc";
	std::ofstream writeFileDoc(wordSavePathStr);
	if (!writeFileDoc)
		throw std::exception("计算书生成失败，已经打开或者文件被占用！\n请先关闭或解除占用。");
	QString wordSavePath = QString::fromLocal8Bit(wordSavePathStr.c_str());
	QFile outFile(wordSavePath);
	outFile.open(QIODevice::WriteOnly);
	QTextStream ts(&outFile);
	ts << html << endl;
}

//@function: 封皮第一页   -- by mzq  2019/8/2 11:17
void Report::firstPage(QString &html) const
{
	html += HtmlAutility::insertEnter(3, 48);
	html += HtmlAutility::insertText("间隙圆计算书", 48, 1, true);//字体大小为小初
	html += HtmlAutility::insertEnter(1, 48);

	html += HtmlAutility::insertText("工程名称：" + ClWidget::getCoreInstance()->getProjectName(), _fontSize_s2, 1, false);
	html += HtmlAutility::insertText("代号：" + ClWidget::getCoreInstance()->getProjectNO(), _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(3, _fontSize_s2);

	html += HtmlAutility::insertText("批&nbsp;&nbsp;准：__________", _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(1, _fontSize_s2);
	html += HtmlAutility::insertText("审&nbsp;&nbsp;核：__________", _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(1, _fontSize_s2);
	html += HtmlAutility::insertText("校&nbsp;&nbsp;核：__________", _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(1, _fontSize_s2);
	html += HtmlAutility::insertText("设&nbsp;&nbsp;计：__________", _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(3, _fontSize_s2);

	string current_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd").toLocal8Bit().toStdString();
	html += HtmlAutility::insertText(current_Time, _fontSize_s2, 1, false);

	html += HtmlAutility::newPage();
}