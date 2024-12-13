/****************************************************************/
//Author: 	    马梓淇
//Version: 	    1.0
//Date:		    2019.02.18
//Description:	新建工程
/****************************************************************/

#pragma once
#include <QDialog>
QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE

class NewProject_Dlg :
	public QDialog
{
	Q_OBJECT
public:
	NewProject_Dlg();
	~NewProject_Dlg();
public:
	static void existPro();
	static bool checkProExisted(QString& name, QString& path);//检查该项目名是否已经存在
	static void createProFileFolder(QString& name, QString& path);//创建工程文件夹
	static void checkAndCreateFolder(QString& name, QString& path);//检查并创建文件夹
private:
	void initDlg();
	static bool delDir(const QString &path);//删除文件夹
private slots :
	void slotClickBrowsBtn();
	void slotClickOk();
	void slotClickCancel();
signals:
	void crateProTreew();
	void clearPro();
	void writeRecentFiles(const QString& filePath);
	void loadParaLibFile(bool isNewProject/*是否为新建工程*/);

private:
	QPushButton    *_browsBtn;
	QLineEdit      *_pathEdt;
	QLineEdit      *_proNameEdt;
	QLineEdit      *_proNumEdt;
	QLineEdit      *_volNumEdt;

};