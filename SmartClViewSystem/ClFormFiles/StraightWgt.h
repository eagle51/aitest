/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.2.6
//Description:	I����������
/****************************************************************/
#pragma once
#include "ClWidget.h"
#include "Mdl_BasicPara.h"
#include "ClTableWgt.h"
#include "table_view.h"
#include "SingleMeteRegionModel.h"
#include "Mdl_CalcRelePara.h"
#include "Mdl_SpanPara.h"
#include "Mdl_VSpanCalSet.h"
#include "Mdl_WireRelatPara.h"
#include "Mdl_InsultorPara_I.h"
#include "Mdl_InsulaGeoPara_I.h"
#include "DataBase.h"

using namespace SmartCl;

class StraightWgt : public ClWidget
{
	Q_OBJECT

public:
	StraightWgt(int clType, 
		        std::string clName);
	virtual ~StraightWgt();
	virtual void initWgt();//��ʼ������
	virtual void readDataToForm();//�����ݶ��������
	virtual void writeDataFromForm();//�ӽ��������д�����ݿ�
	virtual void drawClAndCreatReport();//���Ƽ�϶Բ�Լ����ɼ����飬����һ������ͼֽ�Լ������鹦��

	virtual void writeReport() = 0;//���ɱ���
	virtual void readChildClDataToForm() = 0;//��Ե�Ӵ�����
	virtual void writeChildClDataFromForm() = 0;//��Ե�Ӵ�����
	virtual QGroupBox* initGapValueGbx() = 0;//���ֹ�����϶ֵ��Ͽ�
	virtual QGroupBox* initSwingAngleGbx() = 0;//I��ҡ�ڽ���Ϣ�����I����VL��������
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap) = 0;//�����Ƽ���϶ֵ

	QToolBar* getCadToolBar(QWidget *parent = 0/*������*/);//����OdqView���ȡ����ʾ

private:
	void initCadToolBar();//��ʼCAD��ͼ������
	QWidget*   initClDesignWgt();//��ʼ����϶Բ����
	QGroupBox* initBasicInfoGbx();//��ʼ��������Ϣ��Ͽ�
	QGroupBox* initCalcParaGbx();//��ʼ�����������Ϣ��Ͽ�
	QGroupBox* initWireRelatParaGbx();//������Ϣ��Ͽ�
	QGroupBox* initMeteAreaGbx();//��������Ϣ��Ͽ�
	QGroupBox* initSpanParaGbx();//������Ϣ��Ͽ�
	QGroupBox* initVSpanCalSetGbx();//��ֱ����������÷�ʽ��Ͽ�

	void readBasicInfoData(HdlMdl_BasicPara pBasicPara);//������Ϣ
	void writeBasicInfoData(HdlMdl_BasicPara pBasicPara);//������Ϣ
	void readCalcReleParaData(HdlMdl_CalcRelePara pCalcRelePara);//��ѹ�߶���Ϣ
	void writeCalcReleParaData(HdlMdl_CalcRelePara pCalcRelePara);//��ѹ�߶���Ϣ
	void readSpanParaData(HdlMdl_SpanPara pSpanPara);//������Ϣ
	void writeSpanParaData(HdlMdl_SpanPara pSpanPara);//������Ϣ
	void readVSpanCalSetData(HdlMdl_VSpanCalSet pVSpanCalSet,
							 HdlMdl_SpanPara pSpanPara);//��ֱ�����������
	void writeVSpanCalSetData(HdlMdl_VSpanCalSet pVSpanCalSet,
							  HdlMdl_SpanPara pSpanPara);//��ֱ�����������
	void readMeteAreaData(HdlMdl_MetaAreaPara pMeteArea);//���󹤿���زβ���
	void writeMeteAreaData(HdlMdl_MetaAreaPara pMeteArea);//���󹤿���زβ���
	void readWireRelatData(HdlMdl_WireRelatPara pWireRelatPara);//������ز���
	void writeWireRelatData(HdlMdl_WireRelatPara pWireRelatPara);//������ز���

private slots:
    void slotSgstGapChanged();//����ͼ�϶�Ƽ���ز��������˸ı�
	void slotVoltChanged();//��ѹ�ȼ������˸ı�
    void slotOpenWireAvgHLib_Dlg();//�鿴����ƽ���߶ȿ�Ի���
	void slotselectMeteDlg(); //ѡ���������Ի���
	void slotOnClickSetVerSpan();//��ֱ��������
	void slotCodeTypeChanged();//�淶���ı�
	void slotOnClickAlphaAutoCalc();//����Զ������ѹ������ϵ��
	void slotOnClickAlphaInput();//����ֶ������ѹ������ϵ��
	
	void slotCalcPrincipleSet();//����ԭ������
	void slotCalcProcess();//����
	void slotCreatReport(bool isOpenReportMsg = true);//���ɱ���
	void slotDrawCommand();//���Ƽ�϶Բ
	void slotErrDisplay();//��ʾ����

protected:
	QPushButton               *_insulatorParaBtn;//��Ե�Ӵ�����	
	HdlDataUI_Stright          _dataUI_Stright;//ֱ��������������

	/*CAD��ͼ������ģ��*/
	QToolBar                  *_cadToolBar;//CAD��ͼ��������
	QComboBox                 *_leftOrRightCmb;//�����Ҳ�
	QLineEdit                 *_plottingScaleEdt;//��ͼ������
	QComboBox                 *_isDimensionCbx;//������ע

	/*������Ϣģ��*/
	QLineEdit                 *_towNameEdt;//����
	//QComboBox                 *_voltTypeCBx;//��ѹ�ȼ����
	QPushButton               *_voltTypeBtn;//��ѹ�ȼ����
	QComboBox                 *_loopTypeCBx;//��·����
	QLineEdit                 *_altitudeEdt;//���θ߶�
	QLineEdit                 *_rotatAngleEdt;//ת�Ƕ���

	/*��϶ֵģ��*/
	ClTableWgt                *_gapValueTbl;//��϶ֵ����
	QGroupBox                 *_hotLineWorkGBox;//�Ƿ��Ǵ�����ҵ
	QLineEdit                 *_hotLineWorkRangeEdit;//������ҵ������Χ
	QGroupBox                 *_marginUpGBox;//�Ƿ��Ƕ�����ԣ��
	QLineEdit                 *_marginUpEdit;//����ԣ��ֵ
	QGroupBox                 *_marginDownGBox;//�Ƿ��Ƕ�����ԣ��
	QLineEdit                 *_marginDownEdit;//����ԣ��ֵ

private:
	/*�������ģ��*/
	QLineEdit                 *_wireAvgHEdit;//���
	QLineEdit                 *_towerCallHEdit;//���������

	QComboBox                 *_terrRoughtCmb;//���δֲڶ����
	QComboBox                 *_codeTypeCmb;//��ƹ涨���
	QGroupBox                 *_alphaGBox;//�Ƿ��ֶ����÷�ѹ������ϵ��

	QRadioButton              *_alphaAutoCalcRbtn;//�Զ������ѹ������ϵ��
	QRadioButton              *_alphaInputRbtn;//�ֶ������ѹ������ϵ��
	QLineEdit                 *_alphaEdit;//��ѹ������ϵ��

	/*���߲���ģ��*/
	ClTableWgt                *_pWireParaTbl;//���߲���

	/*������ģ��*/
	StTable_View              *_meteInfoView;//������view
	SingleMeteRegionModel     *_meteInfoModel;//������model
	QLabel                    *_meteName; //����������
	QPushButton               *_selectMete; //ѡ����������ť
	QLineEdit                 *_windRefH_Edt; //���ٻ�׼�߶�

	/*�������ģ��*/
	ClTableWgt                *_spanParaTbl;//�������

	/*��ֱ��������ģ��*/
	QRadioButton              *_autoSetVerSpanRBtn;//�Զ����ô�ֱ����
	QRadioButton              *_sameVerSpanRBtn;//��������ֱ������ͬ
	QRadioButton              *_partSetVerSpanRBtn;//�������ֱ����ü�Ȩϵ��
	QLabel                    *_maxVerSpanLabel;//��󻡴����ƹ���label
	QComboBox                 *_maxVerSpanComboxbox;//��󴹵����ƹ���
	QWidget                   *_maxVerSpanWidget;
	ClTableWgt                *_partSetVerSpanWidget;//��ֱ��������Ȩ��

};
typedef QPointer<StraightWgt> HdlStraightIWgt;