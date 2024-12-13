/****************************************************************/
//Author: 	������
//Version: 	1.0
//Date:	    2019.4.18
//Description:	��Ե��L��������ģ����ʾ�Ի���
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
	void initDialog_L();//��ʼ��L���Ի���

private slots:
    void slotEditAngleTable();//L ���н�ȡֵ�����ʾ
	void slotCurrentIndexChanged();//�������л�
	void slotOk();
	void slotOpenInsulatorLibDlg();//��Ե�Ӵ���鿴

private:
	void readInsultorParaData_L();
	void readInsulaGeoParaData_L();
	void writeInsultorParaData_L();
	void writeInsulaGeoParaData_L();

	void updateUIData();//ˢ�½�������
	std::vector<double> getLAngleRecommend();//L ���н��Ƽ�ֵ
	double calcInsultorLen_L();//���ݾ�Ե�Ӵ����β��������Ե�Ӵ��ܳ��ȣ��˳���Ϊ��Ч���ȣ�Ϊ��ֱ���ϵ�ͶӰ���ȣ���������Ӱ���С

	QGroupBox* initParameterGbx();//��Ե�Ӵ�������
	QGroupBox* initAngleValueGbx();//L���н�ȡֵ
	QGroupBox* initGeoParameterGbx();//���β�����

private:
	bool                        _isReadData;//�Ƿ����ڶ�������
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