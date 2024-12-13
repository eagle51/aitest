/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2020.01.19
//Description:	导线平均高库界面
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
	WireAvgHLib_Dlg(bool isCanEdit/*表格是否可以编辑*/);
	~WireAvgHLib_Dlg();
	const QString& getSelectedVoltType() const { return _selectedVoltType; }
	void setSelectedVoltType(const QString& val) { _selectedVoltType = val; }
	void setCurrentTableRow(const QString& voltTypeQstr);//设置表格当前选中行

private:
	void initDialog();//初始化对话框
	void readDataToForm();//将数据读入界面中

private slots:
	void writeDataFromForm();//从界面把数据写入数据库
	void slotOk();//确认所选的行对应的电压等级

private:
	StTableWidget            *_tableWidget;//表格控件
	bool                      _isCanEdit;//表格是否可以编辑
	QString                   _selectedVoltType;//选中的电压等级	

	HdlWireAvgHLib            _wireAvgHLib;//导线平均高库
};