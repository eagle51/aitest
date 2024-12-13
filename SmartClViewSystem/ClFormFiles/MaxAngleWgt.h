/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.9.9
//Description:	ֱ����I�����ҡ�ڽ� ��������
/****************************************************************/
#pragma once
#include "ClWidget.h"
#include "OsgQt.h"
#include "OsgUIManager.h"
#include "DataUI_MaxAngle.h"
#include "StTableWidget.h"
#include "qcombobox.h"
#include "Mdl_BasicPara.h"

using namespace SmartCl;

class MaxAngleWgt : public ClWidget
{
	Q_OBJECT

public:
	MaxAngleWgt(int clType,
		          std::string clName);
	virtual ~MaxAngleWgt();

	virtual void initWgt();//��ʼ������
	virtual void readDataToForm();//�����ݶ��������
	virtual void writeDataFromForm();//�ӽ��������д�����ݿ�
	virtual void drawClAndCreatReport();//���Ƽ�϶Բ�Լ����ɼ����飬����һ������ͼֽ�Լ������鹦��

	virtual void writeReport() = 0;//���ɱ���
	virtual void readChildClDataToForm() = 0;//��Ե�Ӵ�����
	virtual void writeChildClDataFromForm() = 0;//��Ե�Ӵ�����
	virtual QGroupBox* initGapValueGbx() = 0;//���ֹ�����϶ֵ��Ͽ�
	virtual QGroupBox* initInsulatorParaGbx() = 0;//��Ե�Ӵ���Ϣ��Ͽ�
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap) = 0;
	virtual void initialHangPointTableData() = 0;//�µ�����ģ��ʱˢ�³�ʼ���ҵ������

	OsgQt* getOsgWidget(QWidget *parent = 0/*������*/);//����OdqView���ȡ����ʾ����
	QToolBar* getCadToolBar(QWidget *parent = 0/*������*/);//����OdqView���ȡ����ʾ
	void updateOsgView();//����Osg������ʾ

private:
	void initCadToolBar();//��ʼCAD��ͼ������
	QWidget* initClDesignWgt();//��ʼ����϶Բ����
	QGroupBox* initBasicInfoGbx();//��ʼ��������Ϣ��Ͽ�

	void readBasicInfoData(HdlMdl_BasicPara pBasicPara);//������Ϣ
	void writeBasicInfoData(HdlMdl_BasicPara pBasicPara);//������Ϣ

signals:
	void changeTabWidgetIndex(int index);//�ı�ͼ����ʾ����ǰ��ʾ��tabҳ��

public slots:
	void slotSgstGapChanged();//����ͼ�϶�Ƽ���ز��������˸ı�
	void slotDrawCommand();//���Ƽ�϶Բ

private slots:
	void slotCalcProcess();//����
	void slotCreatReport(bool isOpenReportMsg = true);//���ɱ���
	void slotErrDisplay();//��ʾ����
	void slotOpenImpotrTowerDlg(); //��������ģ�����ݶԻ���

protected:
	HdlDataUI_MaxAngle         _dataUI_MaxAngle;//����������
	QSPtr_OsgUIManager         _osgUIManager;//Osg��ͼ������
	OsgQt                     *_osgWidget;//Osg���ڣ���Ҫ����getOsgWidget()�������г�ʼ��

	/*CAD��ͼ������ģ��*/
	QComboBox                 *_meteCateCmb;//���󹤿��л�cmb

	/*������Ϣģ��*/
	QLineEdit                 *_towNameEdt;//����
	QComboBox                 *_voltTypeCBx;//��ѹ�ȼ����
	QComboBox                 *_loopTypeCBx;//��·����
	QLineEdit                 *_altitudeEdt;//���θ߶�

	/*��϶ֵģ��*/
	StTableWidget             *_gapValueTbl;//��϶ֵ����
	QGroupBox                 *_hotLineWorkGBox;//�Ƿ��Ǵ�����ҵ
	QLineEdit                 *_hotLineWorkRangeEdit;//������ҵ������Χ
	QGroupBox                 *_marginDownGBox;//�Ƿ��Ƕ�����ԣ��
	QLineEdit                 *_marginDownEdit;//����ԣ��ֵ

private:
	QToolBar                  *_cadToolBar;//CAD��ͼ��������
	QPushButton               *_inportTowerBtn; //������������
	bool                       _isReversal; //�Ƿ�ת��

};
typedef QPointer<MaxAngleWgt> HdlMaxAngleWgt;