/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.8.1
//Description:	此类为Html需要用的一些工具函数
/****************************************************************/
#pragma once
#include "qstring.h"
#include <vector>

class HtmlAutility
{
public:
	HtmlAutility();
	~HtmlAutility();

	//插入分页符
	static QString newPage();

	//插入多行回车
	static QString insertEnter(int EnterNum, //回车数量
		                             double fontSize);//字体大小

	//插入正文
	static QString insertText(const std::string& text, //正文文本内容
		                      double fontSize, //字体大小
		                      int position, //文本位置，左0中1右2
		                      bool isBold);//是否粗体

	//插入图片
	static QString insertPic(const std::string& picPath, //图片路径
		                     double picWidth,//图片宽度
		                     double picheight);//图片高度

	//插入表格(整体插入，适合无合并单元格功情况)
	static QString insertTable(const std::vector<std::string>& headerVec, //表头内容
		                       const std::vector<std::vector<std::string>>& contentVec, //表中内容，内层为列，外层为行
		                       double fontSize,//字体大小
		                       int contentPosition);//表中文本位置，左0中1右2

	//表格样式
	static QString tableStyle(double fontSize); //字体大小

	//插入一个表格表头单元格
	static QString insertTableHeaderCell(const std::string& cellText, //单元格内容
		                                 int rowspan = 1, //行合并单元格
		                                 int colspan = 1);//列合并单元格

	//插入一个表格中内容单元格
	static QString insertTableTextCell(const std::string& cellText, //单元格内容
		                               int rowspan = 1, //行合并单元格
		                               int colspan = 1);//列合并单元格
};
