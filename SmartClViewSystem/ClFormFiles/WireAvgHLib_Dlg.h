/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2020.01.19
//Description:	����ƽ���߿����
/****************************************************************/

#pragma once
#include <QDialog>
#include "EnumLib.h"
#include "WireAvgHLib.h"
#include "StTableWidget.h"

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

using namespace SmartCl;

class WireAvgHLib_Dlg :public QDialog
{
	Q_OBJECT

public:
	WireAvgHLib_Dlg(bool isCanEdit/*����Ƿ���Ա༭*/);
	~WireAvgHLib_Dlg();
	const QString& getSelectedVoltType() const { return _selectedVoltType; }
	void setSelectedVoltType(const QString& val) { _selectedVoltType = val; }
	void setCurrentTableRow(const QString& voltTypeQstr);//���ñ��ǰѡ����

private:
	void initDialog();//��ʼ���Ի���
	void readDataToForm();//�����ݶ��������

private slots:
	void writeDataFromForm();//�ӽ��������д�����ݿ�
	void slotOk();//ȷ����ѡ���ж�Ӧ�ĵ�ѹ�ȼ�

private:
	StTableWidget            *_tableWidget;//���ؼ�
	bool                      _isCanEdit;//����Ƿ���Ա༭
	QString                   _selectedVoltType;//ѡ�еĵ�ѹ�ȼ�	

	HdlWireAvgHLib            _wireAvgHLib;//����ƽ���߿�
};