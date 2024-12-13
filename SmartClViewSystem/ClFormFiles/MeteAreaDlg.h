/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2020.03.28
//Description:	�������Ի���
/****************************************************************/
#pragma once
#include <QDialog>
#include "SingleMeteRegionModel.h"
#include "SingleMeteArea.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
QT_END_NAMESPACE

using namespace std;
using namespace SmartCl;

class AddMeteAreaDialog :public QDialog
{
	Q_OBJECT

public:
	AddMeteAreaDialog(QWidget *parent = 0);
	~AddMeteAreaDialog(){};

	std::string getMeteAreaName() const;

private slots:
	void slotokBtn();

private:
	QVBoxLayout  *initDialogLyt();

private:
	QLabel          *_addMtAreaNameLabel;
	QLineEdit       *_addMtAreaNameLineEdit;
	QPushButton     *_okBtn;
	QPushButton     *_cancelBtn;
	QLabel          *_errLabel;
};


class MeteAreaDlg : public QDialog
{
	Q_OBJECT

public:
	explicit MeteAreaDlg(const QString &curMeteAreaLblName = 0, QWidget *parent = 0);

	HdlSingleMeteArea getCurMtAreaTpl() const;
	void hideText();//���ر�ע��Ӧ�õ��������Ϣ

private:
	QHBoxLayout* initDialogLyt();
	void changeEvent(QEvent * e);
	void changeLanguage();
	void meteAreaNameList();
	int curMeteAreaLblIndex(const QString &curMeteAreaLblName);

private slots:
	void slotAddBtn_Click();
	void slotDeleteBtn_Click();
	void slotSaveBtn_Click();
	void slotSetCurrentMeteArea(int Index);
	void slotOk();
	void slotEditingFinished();

signals:
	void MeteAreaChange(HdlSingleMeteArea pMeteAreaCopy);

private:
	QListWidget                 *_meteArealist;
	QLabel                      *_curMeteAreaLbl;
	QLabel			            *_noteLabel;
	QLineEdit                   *_curMeteAreaEdt;
	StTable_View                *_pMeteAreaView;

	QPushButton                 *_deleteBtn;//ɾ��������ģ������
	QPushButton                 *_saveBtn;//����������ģ������
	QPushButton                 *_addBtn;//����������ģ������
	QPushButton                 *_affirmBtn;//ȷ��
	QPushButton                 *_cancelBtn;//ȡ��
	QLabel                      *_meteAreaTpllabel;//����ǩ��������
	SingleMeteRegionModel       *_pMeteRegionModel;
	HdlSingleMeteArea            _pMeteArea;
};