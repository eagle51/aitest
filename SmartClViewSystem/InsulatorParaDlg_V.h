/****************************************************************/
//Author: 	高泽宇
//Version: 	1.0
//Date:	    2019.4.4
//Description:	绝缘子V串参数及模型显示对话框
/****************************************************************/

#pragma  once
#include <QDialog>
#include <QHeaderView>
#include "StTableWidget.h"
#include "Mdl_InsultorPara_V.h"
#include "Mdl_InsulaGeoPara_V.h"
#include "Cl_Stright_V.h"

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

class InsulatorParaDlg_V :public QDialog
{
	Q_OBJECT
public:
	InsulatorParaDlg_V(HdlCl_Stright_V cl_Stright_V);
	~InsulatorParaDlg_V();

protected:
	void initDialog();//初始化V串对话框
	
private slots:
    void slotEditAngleTable();//V串夹角取值表格显示
	void slotCurrentIndexChanged();//分裂数切换
	void slotOk();
	void slotOpenInsulatorLibDlg();//绝缘子串库查看
private:
	void readInsultorParaData_V();
	void readInsulaGeoParaData_V();
	void writeInsultorParaData_V();
	void writeInsulaGeoParaData_V();

	void updateUIData();//刷新界面数据
	double getVAngleRecommend();//V 串夹角推荐值
	double calcInsultorLen_V();//根据绝缘子串几何参数计算绝缘子串总长度，此长度为等效长度，为垂直面上的投影长度

	QGroupBox* initParameterGbx();//绝缘子串参数框
	QGroupBox* initAngleValueGbx();//V串夹角取值
	QGroupBox* initGeoParameterGbx();//几何参数框

private:
	bool                      _isReadData;//是否正在读入数据
	QWidget                  *_leftAreaWidget;
	QLabel                   *_rightAreaLbl;

	QGroupBox                *_parameterGbx;
	StTableWidget            *_parameterTable;

	QGroupBox                *_angleValueGbx;
	StTableWidget          	 *_angleValueTable;
	QLineEdit                *_alphaCorrectEdit;

	QGroupBox                *_geoParameterGbx;
	StTableWidget	         *_geoParameterTable;
	QComboBox                *_splitNumCmb;

	HdlCl_Stright_V           _cl_Stright_V;
	HdlMdl_InsultorPara_V     _insultorPara_V;
	HdlMdl_InsulaGeoPara_V    _insulaGeoPara_V;
	VoltType::Type            _voltType;
};