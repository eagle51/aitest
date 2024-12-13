#ifndef INSULATORSTRING_L_H
#define INSULATORSTRING_L_H
#include "InsulatorLib_L.h"
#include "ComboDelegate.h"

#include <QWidget>
#include "qfileinfo.h"
#include <QTableWidget>
#include "qtablewidget.h"
#include "qmenu.h"
#include "qlabel.h"
#include "qcombobox.h"

using namespace SmartCl;
class InsulatorLibWgt_L : public QWidget
{
	Q_OBJECT

public:
	InsulatorLibWgt_L(QWidget *parent);
	~InsulatorLibWgt_L();
	void writeToJson();//保存到json文件
	void setTableStatus();//设置表格编辑状态
	HdlInsulator_L getCurrentInsulator();//获取当前绝缘子串
	void setVoltage(VoltType::Type voltType);//设置电压等级
private:
	void initialWgt();//初始化窗口
	void initTableMenu();//初始化表格右击菜单
	void readDataToForm();//将数据读入界面中
	void writeDataFromForm();//从界面把数据写入数据库
	void writeOneInsulator_L(HdlInsulator_L insulator_L,
							 bool writeSpecial/*是否写入线夹及小弧垂信息*/,
							 int index);//写入一列数据

public slots:
    void slotChangeVolt(int index);//改变电压等级

private slots:
	void slotTableMenu(QPoint pos);//打开右键菜单
	void slotAddColumn();//增加列
	void slotDeletColumn();//删除列
	void slotApplyToProject_L();//更新直线塔L串间隙圆串参数
	void slotChangePic();//更改界面示意图

private:
	QComboBox                       *_voltTypeCBx;//电压等级类别
	QTableWidget                    *_tableWidget;
	ComboDelegate	                *_splitNumCmbDeleg;
	QMenu                           *_tableMenu;
	QLabel                          *_picLbl;

	HdlInsulatorLib_L                _insulatorLib_L;
	HdlInsulatorLib_SingleVolt_L     _insulatorLib_SingleVolt_L;
};

#endif // INSULATORSTRING_L_H
