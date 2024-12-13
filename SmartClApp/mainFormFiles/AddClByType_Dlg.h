/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		20190218
//Description:	按照间隙圆类型添加间隙圆对话框			
/****************************************************************/
#pragma once
#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE

class AddClByType_Dlg :
	public QDialog
{
	Q_OBJECT
public:
	AddClByType_Dlg();
	~AddClByType_Dlg();
	int test(int);
private:
	void initDlg();
private slots:
	void slotClickOk();
	void slotClickCancel();
signals:
	void createTreeByClName(const QString& clName);
private:
	QComboBox *_clTypeCbx;//间隙圆类型下拉框
	QLineEdit *_clNameEdt;//间隙圆名称

};
