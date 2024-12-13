/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2022.12.03
//Description:	��������
/****************************************************************/
#pragma once
#include "qdialog.h"
#include "QLineEdit"
#include "QGroupBox"
#include "QComboBox"

class SettingsDlg :public QDialog
{
	Q_OBJECT

public:
	SettingsDlg(QWidget *parent = 0);
	~SettingsDlg();

private:
	void initDialog();//��ʼ���Ի���
	void readData();//��ȡ����
	void closeEvent(QCloseEvent * event);

private slots:
	void slotSaveData(); //��������

private:
	QLineEdit                   *_plottingScaleEdt;//��ͼ���ű���
	QComboBox                   *_dimStyleTypeCbx;//��ע����
	QComboBox                   *_smallSagStyleCbx;//V/L��С������ʽ
	QGroupBox                   *_isConsiderRingRGBox;//�Ƿ��Ǿ�ѹ������
	QLineEdit                   *_ringREdt;//��ѹ������

};	