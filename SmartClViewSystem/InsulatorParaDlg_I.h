/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:	    2019.3.27
//Description:	绝缘子串参数及模型显示对话框
/****************************************************************/
#pragma  once
#include <QDialog>
#include "StTableWidget.h"
#include "Mdl_InsulaGeoPara_I.h"
#include "Mdl_InsultorPara_I.h"
#include "Cl_Stright_I.h"

QT_BEGIN_NAMESPACE
class QGroupBox;
class QComboBox;
class QCheckBox;
class QLineEdit;
class QTimer;
class QValidator;
class QRegExp;
class QLabel;
QT_END_NAMESPACE

using namespace SmartCl;

class InsulatorParaDlg_I :public QDialog
{
	Q_OBJECT
public:
	InsulatorParaDlg_I(HdlCl_Stright_I cl_Stright_I);
	~InsulatorParaDlg_I();

protected:
	void initDialog();//初始化对话框
	
private slots:
    void slotUpdateInsultorLen();//更新绝缘子串长度显示
	void slotCurrentIndexChanged();//下拉框切换
	void slotOk();
	void slotOpenInsulatorLibDlg();//绝缘子串库查看

private:
	void readInsultorParaData_I();
	void readInsulaGeoParaData_I();
	void writeInsultorParaData_I();
	void writeInsulaGeoParaData_I();

	void updateUIData();//刷新界面数据
	double calcInsultorLen_I();//根据绝缘子串几何参数自动计算绝缘子串总长度

	QGroupBox* initParameterGbx();//参数表组合框
	QGroupBox* initGeoParameterGbx();//几何参数组合框
	
private:
	bool                      _isReadData;//是否正在读入数据
	QWidget                  *_leftAreaWidget;
	QLabel                   *_rightAreaLbl;
	QComboBox                *_splitNumCmb;
	QTableWidget             *_tableWidgetSee;
	StTableWidget            *_parameterTable;
	StTableWidget            *_geoParameterTable;

	HdlDataUI_Stright_I       _dataUI_Stright_I;
	HdlMdl_InsultorPara_I     _insultorPara_I;
	HdlMdl_InsulaGeoPara_I    _insulaGeoPara_I;
	VoltType::Type            _voltType;
};