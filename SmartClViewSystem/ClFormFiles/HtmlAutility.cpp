#include "HtmlAutility.h"
#include <QString>

HtmlAutility::HtmlAutility()
{

}

HtmlAutility::~HtmlAutility()
{

}

//@function: 插入分页符   -- by mzq  2019/8/1 16:32
QString HtmlAutility::newPage()
{
	QString QStr = "<br clear = all style = \"page-break-before:always\" mce_style = \"page-break-before:always\">";
	return QStr;
}

//@function: 插入回车   -- by mzq  2019/8/1 17:31
QString HtmlAutility::insertEnter(
	int EnterNum, //回车数量
	double fontSize)//字体大小
{
	QString QStr = "";
	for (int iLoop = 0; iLoop < EnterNum; ++iLoop)
	{
		QStr += "<div style=\"color:red;font-size:" + QString::number(fontSize) + "px\"><p align=\"center\">&nbsp;</p></div>";
	}
	return QStr;
}

//@function: 插入正文   -- by mzq  2019/8/2 9:31
QString HtmlAutility::insertText(const std::string& text, //正文文本内容
	                             double fontSize, //字体大小
	                             int position, //文本位置，左0中1右2
	                             bool isBold)//是否粗体
{
	//html示例
	//<div style="color:red;font-size:18px"><p align="center"><strong>这是一个在div元素中的文本。</strong></p></div>

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

//@function: 插入图片   -- by mzq  2019/8/4 14:23
QString HtmlAutility::insertPic(const std::string& picPath, /*图片路径 */
	                            double picWidth,//图片宽度
	                            double picheight)//图片高度

{
	QString QStr = "";
	QStr += "<p align=\"center\"><img src=\"" + QString::fromLocal8Bit(picPath.c_str()) + "\" width=\""
		+ QString::number(picWidth) + "\" height=\"" + QString::number(picheight) + "\"></p>";

	return QStr;
}

//@function: 插入表格(整体插入，适合无合并单元格功情况)   -- by mzq  2019/8/2 16:13
QString HtmlAutility::insertTable(const std::vector<std::string>& headerVec, //表头内容
	                              const std::vector<std::vector<std::string>>& contentVec, //表中内容，内层为列，外层为行
	                              double fontSize,//字体大小
	                              int contentPosition)//表中文本位置，左0中1右2
{
	QString QStr = "";

	/*定义表格style样式*/
	QStr += tableStyle(fontSize);

	/*定义表格框距等属性*/
	QStr += "<table class=\"gridtable\" border=\"1\" cellspacing=\"0\" width=\"615\">";
	
	/*输入表头内容*/
	QStr += "<tr>";
	for (unsigned int headerLoop = 0; headerLoop < headerVec.size(); ++headerLoop)
	{
		QStr += "<th>" + QString::fromLocal8Bit(headerVec.at(headerLoop).c_str()) + "</th>";
	}
	QStr += "</tr>";

	/*输入表中内容*/
	for (unsigned int rowLoop = 0; rowLoop < contentVec.size(); ++rowLoop)
	{
		QStr += "<tr>";
		std::vector<std::string>  singleRow = contentVec.at(rowLoop);
		for (unsigned int columnLoop = 0; columnLoop < singleRow.size(); ++columnLoop)
		{
			/*表中内容位置，左0中1右2*/
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

	/*结尾*/
	QStr += "</table>";
	return QStr;
}

//@function: 表格样式   -- by mzq  2019/8/4 21:30
QString HtmlAutility::tableStyle(double fontSize)
{
	QString QStr = "";

	/*定义表格style样式*/
	QStr += "<style type = \"text/css\">";
	QStr += "table.gridtable {font-size:"
		+ QString::number(fontSize)
		+ "px;color:#000000;border-color:#000000; }";
	QStr += "table.gridtable th {padding:4px;border-style:solid;border-color:#000000;background-color:#dedede;}";
	QStr += "table.gridtable td{padding: 4px;border-style: solid;border-color: #000000;background-color: #ffffff;}";
	QStr += "</style>";

	return QStr;
}

//@function: 插入一个表格表头单元格   -- by mzq  2019/8/3 10:18
QString HtmlAutility::insertTableHeaderCell(const std::string& cellText,//单元格内容
	                                        int rowspan,//行合并单元格
	                                        int colspan)//列合并单元格
{
	QString QStr = "";

	/*表中内容位置，左0中1右2*/
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

//@function: 插入一个表格中内容单元格   -- by mzq  2019/8/4 22:07
QString HtmlAutility::insertTableTextCell(const std::string& cellText, /*单元格内容 */ 
	                                      int rowspan, /*行合并单元格 */ 
	                                      int colspan)/*列合并单元格*/
{
	QString QStr = "";

	/*表中内容位置，左0中1右2*/
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
