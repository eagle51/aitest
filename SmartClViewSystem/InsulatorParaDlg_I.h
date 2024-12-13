/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:	    2019.3.27
//Description:	��Ե�Ӵ�������ģ����ʾ�Ի���
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
	void initDialog();//��ʼ���Ի���
	
private slots:
    void slotUpdateInsultorLen();//���¾�Ե�Ӵ�������ʾ
	void slotCurrentIndexChanged();//�������л�
	void slotOk();
	void slotOpenInsulatorLibDlg();//��Ե�Ӵ���鿴

private:
	void readInsultorParaData_I();
	void readInsulaGeoParaData_I();
	void writeInsultorParaData_I();
	void writeInsulaGeoParaData_I();

	void updateUIData();//ˢ�½�������
	double calcInsultorLen_I();//���ݾ�Ե�Ӵ����β����Զ������Ե�Ӵ��ܳ���

	QGroupBox* initParameterGbx();//��������Ͽ�
	QGroupBox* initGeoParameterGbx();//���β�����Ͽ�
	
private:
	bool                      _isReadData;//�Ƿ����ڶ�������
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