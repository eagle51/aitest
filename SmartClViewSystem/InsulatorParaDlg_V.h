/****************************************************************/
//Author: 	������
//Version: 	1.0
//Date:	    2019.4.4
//Description:	��Ե��V��������ģ����ʾ�Ի���
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
	void initDialog();//��ʼ��V���Ի���
	
private slots:
    void slotEditAngleTable();//V���н�ȡֵ�����ʾ
	void slotCurrentIndexChanged();//�������л�
	void slotOk();
	void slotOpenInsulatorLibDlg();//��Ե�Ӵ���鿴
private:
	void readInsultorParaData_V();
	void readInsulaGeoParaData_V();
	void writeInsultorParaData_V();
	void writeInsulaGeoParaData_V();

	void updateUIData();//ˢ�½�������
	double getVAngleRecommend();//V ���н��Ƽ�ֵ
	double calcInsultorLen_V();//���ݾ�Ե�Ӵ����β��������Ե�Ӵ��ܳ��ȣ��˳���Ϊ��Ч���ȣ�Ϊ��ֱ���ϵ�ͶӰ����

	QGroupBox* initParameterGbx();//��Ե�Ӵ�������
	QGroupBox* initAngleValueGbx();//V���н�ȡֵ
	QGroupBox* initGeoParameterGbx();//���β�����

private:
	bool                      _isReadData;//�Ƿ����ڶ�������
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