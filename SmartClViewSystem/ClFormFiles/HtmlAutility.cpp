#include "HtmlAutility.h"
#include <QString>

HtmlAutility::HtmlAutility()
{

}

HtmlAutility::~HtmlAutility()
{

}

//@function: �����ҳ��   -- by mzq  2019/8/1 16:32
QString HtmlAutility::newPage()
{
	QString QStr = "<br clear = all style = \"page-break-before:always\" mce_style = \"page-break-before:always\">";
	return QStr;
}

//@function: ����س�   -- by mzq  2019/8/1 17:31
QString HtmlAutility::insertEnter(
	int EnterNum, //�س�����
	double fontSize)//�����С
{
	QString QStr = "";
	for (int iLoop = 0; iLoop < EnterNum; ++iLoop)
	{
		QStr += "<div style=\"color:red;font-size:" + QString::number(fontSize) + "px\"><p align=\"center\">&nbsp;</p></div>";
	}
	return QStr;
}

//@function: ��������   -- by mzq  2019/8/2 9:31
QString HtmlAutility::insertText(const std::string& text, //�����ı�����
	                             double fontSize, //�����С
	                             int position, //�ı�λ�ã���0��1��2
	                             bool isBold)//�Ƿ����
{
	//htmlʾ��
	//<div style="color:red;font-size:18px"><p align="center"><strong>����һ����divԪ���е��ı���</strong></p></div>

	QString QStr = "<div style=\"color:black;font-size:" + QString::number(fontSize) + "px\">";
	switch (position)
	{
	case 0:
		QStr += "<p align=\"left\">";
		break;
	case 1:
		QStr += "<p align=\"center\">";
		break;
	case 2:
		QStr += "<p align=\"right\">";
		break;
	default:
		QStr += "<p align=\"left\">";
		break;
	}

	if (isBold)
		QStr += "<strong>" + QString::fromLocal8Bit(text.c_str()) + "</strong>";
	else
		QStr += QString::fromLocal8Bit(text.c_str());
	
	QStr += "</p></div>";
	return QStr;
}

//@function: ����ͼƬ   -- by mzq  2019/8/4 14:23
QString HtmlAutility::insertPic(const std::string& picPath, /*ͼƬ·�� */
	                            double picWidth,//ͼƬ���
	                            double picheight)//ͼƬ�߶�

{
	QString QStr = "";
	QStr += "<p align=\"center\"><img src=\"" + QString::fromLocal8Bit(picPath.c_str()) + "\" width=\""
		+ QString::number(picWidth) + "\" height=\"" + QString::number(picheight) + "\"></p>";

	return QStr;
}

//@function: ������(������룬�ʺ��޺ϲ���Ԫ�����)   -- by mzq  2019/8/2 16:13
QString HtmlAutility::insertTable(const std::vector<std::string>& headerVec, //��ͷ����
	                              const std::vector<std::vector<std::string>>& contentVec, //�������ݣ��ڲ�Ϊ�У����Ϊ��
	                              double fontSize,//�����С
	                              int contentPosition)//�����ı�λ�ã���0��1��2
{
	QString QStr = "";

	/*������style��ʽ*/
	QStr += tableStyle(fontSize);

	/*�������������*/
	QStr += "<table class=\"gridtable\" border=\"1\" cellspacing=\"0\" width=\"615\">";
	
	/*�����ͷ����*/
	QStr += "<tr>";
	for (unsigned int headerLoop = 0; headerLoop < headerVec.size(); ++headerLoop)
	{
		QStr += "<th>" + QString::fromLocal8Bit(headerVec.at(headerLoop).c_str()) + "</th>";
	}
	QStr += "</tr>";

	/*�����������*/
	for (unsigned int rowLoop = 0; rowLoop < contentVec.size(); ++rowLoop)
	{
		QStr += "<tr>";
		std::vector<std::string>  singleRow = contentVec.at(rowLoop);
		for (unsigned int columnLoop = 0; columnLoop < singleRow.size(); ++columnLoop)
		{
			/*��������λ�ã���0��1��2*/
			switch (contentPosition)
			{
			case 0:
				QStr += "<td align=\"left\">";
				break;
			case 1:
				QStr += "<td align=\"center\">";
				break;
			case 2:
				QStr += "<td align=\"right\">";
				break;
			default:
				QStr += "<td align=\"left\">";
				break;
			}
			QStr += QString::fromLocal8Bit(singleRow.at(columnLoop).c_str()) + "</td>";
		}
		QStr += "</tr>";
	}

	/*��β*/
	QStr += "</table>";
	return QStr;
}

//@function: �����ʽ   -- by mzq  2019/8/4 21:30
QString HtmlAutility::tableStyle(double fontSize)
{
	QString QStr = "";

	/*������style��ʽ*/
	QStr += "<style type = \"text/css\">";
	QStr += "table.gridtable {font-size:"
		+ QString::number(fontSize)
		+ "px;color:#000000;border-color:#000000; }";
	QStr += "table.gridtable th {padding:4px;border-style:solid;border-color:#000000;background-color:#dedede;}";
	QStr += "table.gridtable td{padding: 4px;border-style: solid;border-color: #000000;background-color: #ffffff;}";
	QStr += "</style>";

	return QStr;
}

//@function: ����һ������ͷ��Ԫ��   -- by mzq  2019/8/3 10:18
QString HtmlAutility::insertTableHeaderCell(const std::string& cellText,//��Ԫ������
	                                        int rowspan,//�кϲ���Ԫ��
	                                        int colspan)//�кϲ���Ԫ��
{
	QString QStr = "";

	/*��������λ�ã���0��1��2*/
	int textPosition = 1;
	switch (textPosition)
	{
	case 0:
		QStr += "<th align=\"left\"";
		break;
	case 1:
		QStr += "<th align=\"center\"";
		break;
	case 2:
		QStr += "<th align=\"right\"";
		break;
	default:
		QStr += "<th align=\"left\"";
		break;
	}

	QStr += " rowspan=\"" + QString::number(rowspan) 
		+ "\" colspan = \"" + QString::number(colspan) + "\" >" 
		+ QString::fromLocal8Bit(cellText.c_str()) + "</th>";
	
	return QStr;
}

//@function: ����һ����������ݵ�Ԫ��   -- by mzq  2019/8/4 22:07
QString HtmlAutility::insertTableTextCell(const std::string& cellText, /*��Ԫ������ */ 
	                                      int rowspan, /*�кϲ���Ԫ�� */ 
	                                      int colspan)/*�кϲ���Ԫ��*/
{
	QString QStr = "";

	/*��������λ�ã���0��1��2*/
	int textPosition = 1;
	switch (textPosition)
	{
	case 0:
		QStr += "<td align=\"left\"";
		break;
	case 1:
		QStr += "<td align=\"center\"";
		break;
	case 2:
		QStr += "<td align=\"right\"";
		break;
	default:
		QStr += "<td align=\"left\"";
		break;
	}

	QStr += " rowspan=\"" + QString::number(rowspan)
		+ "\" colspan = \"" + QString::number(colspan) + "\" >"
		+ QString::fromLocal8Bit(cellText.c_str()) + "</td>";

	return QStr;
}
