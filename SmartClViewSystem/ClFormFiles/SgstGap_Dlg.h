/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.8.8
//Description:	间隙值设置对话框
/****************************************************************/

#pragma once
#include <QDialog>
#include "SgstGapLib.h"

using namespace SmartCl;
using namespace std;

QT_BEGIN_NAMESPACE
class QPushButton;
class StTableWidget;
class QLabel;
class QComboBox;
QT_END_NAMESPACE

class SgstGap_Dlg :public QDialog
{
	Q_OBJECT
public:
	SgstGap_Dlg();
	~SgstGap_Dlg();

private:
	void initWgt();

private slots:
	void slotSave();//保存
	void slotSgstGapChanged();//界面和间隙推荐相关参数发生了改变

private:
	QComboBox                 *_voltTypeCBx;//电压等级类别
	QComboBox                 *_loopTypeCBx;//回路类型
	QComboBox                 *_insulatorTypeCBx;//绝缘子串类型
	QComboBox                 *_altitudeTypeCBx;//海拔高度

	StTableWidget             *_gapValueTbl;//间隙值参数

	QPushButton               *_saveBtn;
	QPushButton	              *_cancelBtn;
};