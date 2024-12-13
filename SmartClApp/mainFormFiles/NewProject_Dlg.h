/****************************************************************/
//Author: 	    �����
//Version: 	    1.0
//Date:		    2019.02.18
//Description:	�½�����
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
	static bool checkProExisted(QString& name, QString& path);//������Ŀ���Ƿ��Ѿ�����
	static void createProFileFolder(QString& name, QString& path);//���������ļ���
	static void checkAndCreateFolder(QString& name, QString& path);//��鲢�����ļ���
private:
	void initDlg();
	static bool delDir(const QString &path);//ɾ���ļ���
private slots :
	void slotClickBrowsBtn();
	void slotClickOk();
	void slotClickCancel();
signals:
	void crateProTreew();
	void clearPro();
	void writeRecentFiles(const QString& filePath);
	void loadParaLibFile(bool isNewProject/*�Ƿ�Ϊ�½�����*/);

private:
	QPushButton    *_browsBtn;
	QLineEdit      *_pathEdt;
	QLineEdit      *_proNameEdt;
	QLineEdit      *_proNumEdt;
	QLineEdit      *_volNumEdt;

};