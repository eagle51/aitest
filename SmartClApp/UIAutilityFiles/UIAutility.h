/****************************************************************/
//Author: 	李婷
//Version: 	1.0
//Date:		2019.1.10
//Description:	此类为界面上需要用的一些工具函数
/****************************************************************/
#pragma once
class QString;

class UIAutility
{
public:
	UIAutility();
	~UIAutility();

public:
	/*拆分文件路径，拆分出文件的名字以及文件的路径直到文件夹。
	例如D:\\United\\test.xml;
	path：带文件名的文件路径.D:\\United\\test.xml
	fileName：test
	filePath：D:\\United;
	*/
	static void spliterPath(const QString& path,QString& filePath,QString& fileName);

	//判断是否有空格
	static bool hasBlank(QString qstr);
};
