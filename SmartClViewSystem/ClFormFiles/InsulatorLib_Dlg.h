/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.12.30
//Description:	绝缘子串库
/****************************************************************/

#pragma once
#include "InsulatorLibWgt_I.h"
#include "InsulatorLibWgt_L.h"
#include "InsulatorLibWgt_V.h"

#include <QDialog>
#include "qtabwidget.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QComboBox;
QT_END_NAMESPACE

class InsulatorLib_Dlg :public QDialog
{
	Q_OBJECT
public:
	InsulatorLib_Dlg();
	InsulatorLib_Dlg(InsulatorType::Type insulatorType, VoltType::Type voltType);
	~InsulatorLib_Dlg();

	HdlInsulator_I getSelect_I();//获取当前选择的I串
	HdlInsulator_V getSelect_V();//获取当前选择的V串
	HdlInsulator_L getSelect_L();//获取当前选择的L串

private:
	void initDialog();//初始化对话框

private slots:
	void slotApply();
	void slotWriteToJson();

private:
	QTabWidget               *_tabWidget;
	InsulatorLibWgt_I        *_insulatorLibWgt_I;
	InsulatorLibWgt_L        *_insulatorLibWgt_L;
 	InsulatorLibWgt_V        *_insulatorLibWgt_V;

	InsulatorType::Type       _insulatorType;
	VoltType::Type            _voltType;
	bool                      _isShowAll;/*是否显示所有类型绝缘子串*/
};
