/****************************************************************/
//Author: 	高泽宇
//Version: 	1.0
//Date:	    2019.4.18
//Description:	绝缘子L串参数及模型显示对话框
/****************************************************************/

#pragma  once
#include <QDialog>
#include <QHeaderView>
#include "StTableWidget.h"
#include "Mdl_InsultorPara_L.h"
#include "Mdl_InsulaGeoPara_L.h"
#include "Cl_Stright_L.h"

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

class InsulatorParaDlg_L :public QDialog
{
	Q_OBJECT
public:
	InsulatorParaDlg_L(HdlCl_Stright_L cl_Stright_L);
	~InsulatorParaDlg_L();

protected:
	void initDialog_L();//初始化L串对话框

private slots:
    void slotEditAngleTable();//L 串夹角取值表格显示
	void slotCurrentIndexChanged();//分裂数切换
	void slotOk();
	void slotOpenInsulatorLibDlg();//绝缘子串库查看

private:
	void readInsultorParaData_L();
	void readInsulaGeoParaData_L();
	void writeInsultorParaData_L();
	void writeInsulaGeoParaData_L();

	void updateUIData();//刷新界面数据
	std::vector<double> getLAngleRecommend();//L 串夹角推荐值
	double calcInsultorLen_L();//根据绝缘子串几何参数计算绝缘子串总长度，此长度为等效长度，为垂直面上的投影长度，存在误差，但影响很小

	QGroupBox* initParameterGbx();//绝缘子串参数框
	QGroupBox* initAngleValueGbx();//L串夹角取值
	QGroupBox* initGeoParameterGbx();//几何参数框

private:
	bool                        _isReadData;//是否正在读入数据
	QWidget                    *_leftAreaWidget;
	QLabel                     *_rightAreaLbl;

	QGroupBox                  *_parameterGbx;
	StTableWidget              *_parameterTable;

	QGroupBox                  *_angleValueGbx;
	StTableWidget	           *_angleValueTable;
	QLineEdit                  *_alphaCorrectEdit;
	QLineEdit                  *_betaCorrectEdit;

	QGroupBox                  *_geoParameterGbx;
	StTableWidget	           *_geoParameterTable;
	QComboBox                  *_splitNumCmb;

	HdlCl_Stright_L             _cl_Stright_L;
	HdlMdl_InsultorPara_L       _insultorPara_L;
	HdlMdl_InsulaGeoPara_L		_insulaGeoPara_L;
	VoltType::Type              _voltType;
};