/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2022.12.03
//Description:	参数设置
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
					 SoftWareType::Type softWareType,/*软件版本类型*/
					 QWidget *parent = 0);
	~CalcPrincipleDlg();

private:
	void initDialog();//初始化对话框
	void readData();//读取数据
	void closeEvent(QCloseEvent * event);

private slots:
	void slotSaveData(); //保存数据
	void slotCurrentIndexChanged_wire(); //计算原则类型切换

private:
	QComboBox                   *_calcPrincipleTypeCbx_wire;//计算原则类型-导线
	QComboBox                   *_calcPrincipleTypeCbx_insulator;//计算原则类型-绝缘子串
	QLabel                      *_IzRefHeightLable;//用于计算Iz的参考高度 lable
	QLineEdit                   *_IzRefHeightEdt;//用于计算Iz的参考高度

	HdlMdl_CalcRelePara          _calcRelePara;//计算参数

	SoftWareType::Type           _softWareType;//软件版本类型
};	