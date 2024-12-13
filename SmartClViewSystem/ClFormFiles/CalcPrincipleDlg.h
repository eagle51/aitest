/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2022.12.03
//Description:	��������
/****************************************************************/
#pragma once
#include "qdialog.h"
#include "Mdl_CalcRelePara.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
class QLabel;
QT_END_NAMESPACE

using namespace SmartCl;
class CalcPrincipleDlg :public QDialog
{
	Q_OBJECT

public:
	CalcPrincipleDlg(HdlMdl_CalcRelePara calcRelePara, 
					 SoftWareType::Type softWareType,/*����汾����*/
					 QWidget *parent = 0);
	~CalcPrincipleDlg();

private:
	void initDialog();//��ʼ���Ի���
	void readData();//��ȡ����
	void closeEvent(QCloseEvent * event);

private slots:
	void slotSaveData(); //��������
	void slotCurrentIndexChanged_wire(); //����ԭ�������л�

private:
	QComboBox                   *_calcPrincipleTypeCbx_wire;//����ԭ������-����
	QComboBox                   *_calcPrincipleTypeCbx_insulator;//����ԭ������-��Ե�Ӵ�
	QLabel                      *_IzRefHeightLable;//���ڼ���Iz�Ĳο��߶� lable
	QLineEdit                   *_IzRefHeightEdt;//���ڼ���Iz�Ĳο��߶�

	HdlMdl_CalcRelePara          _calcRelePara;//�������

	SoftWareType::Type           _softWareType;//����汾����
};	