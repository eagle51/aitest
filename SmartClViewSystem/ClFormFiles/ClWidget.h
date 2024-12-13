/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.1.2
//Description:	��ͬ��϶Բ���ͽ������
/****************************************************************/
#pragma once
#include <QWidget>
#include <QPointer>
#include "DataBase.h"
#include "DataUI_Stright.h"
#include "ReportErrModel.h"
#include "Clearence.h"
#include "VirBox\VirBoxChecker.h"

QT_BEGIN_NAMESPACE
class QTableView;
class QLabel;
class QTextEdit;
class QGroupBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QRadioButton; 
class QCheckBox;
class QToolBar;
QT_END_NAMESPACE

class ClWidget:public QWidget
{
	Q_OBJECT

public:
	ClWidget(int clType, std::string clName);
	virtual ~ClWidget();

	virtual void initWgt() = 0;//��ʼ������
	virtual void readDataToForm() = 0;//�����ݶ��������
	virtual void writeDataFromForm() = 0;//�ӽ��������д�����ݿ�
	virtual void drawClAndCreatReport() = 0;//���Ƽ�϶Բ�Լ����ɼ����飬����һ������ͼֽ�Լ������鹦��

	static SmartCl::DataBase* getCoreInstance(); //�ں����ݿ�
	static AboutSoftWare::VirBoxChecker* getVirBoxCheckerInstance(); //��˼���ܼ��

protected:
	QWidget* initErrTable();//������

protected:
	SmartCl::HdlCl              _cl;//��϶Բ
	QLabel                     *_clTypeTitle;//��϶Բ���ͱ���

	/*��ťģ��*/
	QPushButton                *_pReportBtn;//���ɼ����鰴ť
	QPushButton                *_pDrawClearanceBtn;//���Ƽ�϶Բ��ť

	/*������ģ��*/
	bool                        _hasErr;//�����Ƿ��д���
	ReportErrModel             *_pErrModel;//������model
	QTableView                 *_pErrTableView;//������view
};
typedef QPointer<ClWidget> HdlClWidget;
