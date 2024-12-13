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

	_fontSize_s2 = 24;//С����
	_fontSize_s3 = 20;//С����
	_fontSize_4 = 18.7;//�ĺ�
	_fontSize_s4 = 16;//С�ĺ�
	_fontSize_5 = 14;//���
}

Report::~Report()
{

}

//@function: ���ɱ���   -- by mzq  2019/7/31 10:58
void Report::wordReport() const
{
	string savePathStr = ClWidget::getCoreInstance()->getPathFolder() + "\\" + "��϶Բ������" + "\\" + _cl->getClName() + ".pdf";
	std::ofstream writeFile(savePathStr);
	if (!writeFile)
		throw std::exception("����������ʧ�ܣ��Ѿ��򿪻����ļ���ռ�ã�\n���ȹرջ���ռ�á�");

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

	/*ת����word��ʽ*/
	string wordSavePathStr = ClWidget::getCoreInstance()->getPathFolder() + "\\" + "��϶Բ������" + "\\" + _cl->getClName() + ".doc";
	std::ofstream writeFileDoc(wordSavePathStr);
	if (!writeFileDoc)
		throw std::exception("����������ʧ�ܣ��Ѿ��򿪻����ļ���ռ�ã�\n���ȹرջ���ռ�á�");
	QString wordSavePath = QString::fromLocal8Bit(wordSavePathStr.c_str());
	QFile outFile(wordSavePath);
	outFile.open(QIODevice::WriteOnly);
	QTextStream ts(&outFile);
	ts << html << endl;
}

//@function: ��Ƥ��һҳ   -- by mzq  2019/8/2 11:17
void Report::firstPage(QString &html) const
{
	html += HtmlAutility::insertEnter(3, 48);
	html += HtmlAutility::insertText("��϶Բ������", 48, 1, true);//�����СΪС��
	html += HtmlAutility::insertEnter(1, 48);

	html += HtmlAutility::insertText("�������ƣ�" + ClWidget::getCoreInstance()->getProjectName(), _fontSize_s2, 1, false);
	html += HtmlAutility::insertText("���ţ�" + ClWidget::getCoreInstance()->getProjectNO(), _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(3, _fontSize_s2);

	html += HtmlAutility::insertText("��&nbsp;&nbsp;׼��__________", _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(1, _fontSize_s2);
	html += HtmlAutility::insertText("��&nbsp;&nbsp;�ˣ�__________", _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(1, _fontSize_s2);
	html += HtmlAutility::insertText("У&nbsp;&nbsp;�ˣ�__________", _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(1, _fontSize_s2);
	html += HtmlAutility::insertText("��&nbsp;&nbsp;�ƣ�__________", _fontSize_s2, 1, false);
	html += HtmlAutility::insertEnter(3, _fontSize_s2);

	string current_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd").toLocal8Bit().toStdString();
	html += HtmlAutility::insertText(current_Time, _fontSize_s2, 1, false);

	html += HtmlAutility::newPage();
}