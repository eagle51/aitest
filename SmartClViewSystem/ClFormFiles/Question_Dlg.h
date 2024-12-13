#pragma once
#include <QDialog>
#include <QPushButton>
QT_BEGIN_NAMESPACE
/*class QRadioButton;*/
class QPushButton;
/*class QLabel;*/
QT_END_NAMESPACE
class Question_Dlg :
	public QDialog
{
	Q_OBJECT
public:
	Question_Dlg();
	~Question_Dlg();

private:
	void initDlg();
	void changeEvent(QEvent *e);
	void changeLanguage();

private slots :
	void slotClickOk();
	void slotClickCancel();
private:
// 	QRadioButton *_selectBtn;
// 	QLabel		 *_tipLbl;
	QPushButton  *_okBtn;//确定
	QPushButton  *_cancelBtn;//取消
};
