/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2022.12.03
//Description:	参数设置
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
	void initDialog();//初始化对话框
	void readData();//读取数据
	void closeEvent(QCloseEvent * event);

private slots:
	void slotSaveData(); //保存数据

private:
	QLineEdit                   *_plottingScaleEdt;//绘图缩放比例
	QComboBox                   *_dimStyleTypeCbx;//标注设置
	QComboBox                   *_smallSagStyleCbx;//V/L串小弧垂样式
	QGroupBox                   *_isConsiderRingRGBox;//是否考虑均压环环径
	QLineEdit                   *_ringREdt;//均压环环径

};	