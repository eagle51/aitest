#ifndef INSULATORSTRING_I_H
#define INSULATORSTRING_I_H

#include "InsulatorLib_I.h"
#include "StTableWidget.h"
#include "ComboDelegate.h"

#include <QWidget>
#include "qfileinfo.h"
#include <QTableWidget>
#include "qlabel.h"
#include "qcombobox.h"
#include "qtablewidget.h"
#include "qmenu.h"

using namespace SmartCl;
class InsulatorLibWgt_I : public QWidget
{
	Q_OBJECT

public:
	InsulatorLibWgt_I(QWidget *parent);
	~InsulatorLibWgt_I();
	void writeToJson();//保存到json文件
	void setTableStatus();//设置表格编辑状态
	HdlInsulator_I getCurrentInsulator();//获取当前绝缘子串
	void setVoltage(VoltType::Type voltType);//设置电压等级

private:
	void initialWgt();//初始化窗口
	void initTableMenu();//初始化表格右击菜单
	void readDataToForm();//将数据读入界面中
	void writeDataFromForm();//从界面把数据写入数据库
	void writeOneInsulator_I(HdlInsulator_I insulator_I, 
							 bool writeSpecial/*是否写入线夹及小弧垂信息*/, 
							 int index);//写入一列数据
public slots:
	void slotChangeVolt(int index);//改变电压等级

private slots:
	void slotTableMenu(QPoint pos);//打开右键菜单
	void slotAddColumn();//增加列
	void slotDeletColumn();//删除列
	void slotApplyToProject_I();//更新直线塔I串间隙圆串参数
	void slotChangePic();//更改界面示意图

private:
	QComboBox                       *_voltTypeCBx;       //电压等级类别
	QTableWidget                    *_tableWidget;       //表格控件
	ComboDelegate	                *_splitNumCmbDeleg;  //分裂数组合代理
	QMenu                           *_tableMenu;         //右键菜单
	QLabel                          *_picLbl;            //图片标签

	HdlInsulatorLib_I               _insulatorLib_I;               //I型绝缘子串数据库
	HdlInsulatorLib_SingleVolt_I    _insulatorLib_SingleVolt_I;    //电压等级对应的I型绝缘子数据库
	
};
#endif // INSULATORSTRING_I_H