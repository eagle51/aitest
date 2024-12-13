/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.9.9
//Description:	ֱ����V�����ҡ�ڽ� ��������
/****************************************************************/
#pragma once
#include "MaxAngleWgt.h"
#include "DataUI_MaxAngle_V.h"

using namespace SmartCl;

class MaxAngleWgt_V : public MaxAngleWgt
{
	Q_OBJECT

public:
	MaxAngleWgt_V(int clType, std::string clName);
	virtual ~MaxAngleWgt_V();

	virtual void readChildClDataToForm();
	virtual void writeChildClDataFromForm();
	virtual QGroupBox* initGapValueGbx();//���ֹ�����϶ֵ��Ͽ�
	virtual QGroupBox* initInsulatorParaGbx();//��Ե�Ӵ���Ϣ��Ͽ�
	virtual void writeReport();//���ɱ���
	virtual void initialHangPointTableData();//�µ�����ģ��ʱˢ�³�ʼ���ҵ������

	void readGapValueParaData(HdlMdl_GapValuePara_VL gapValuePara_VL);//���ں˶�ȡ��϶ֵ����
	void writeGapValueParaData(HdlMdl_GapValuePara_VL gapValuePara_VL);//д���϶ֵ�������ں�

	void readInsulaHangParaData_V(HdlMdl_InsulaHangPara_V insulaHangPara_V);//��ȡ��Ե�Ӵ����ݣ�����������Ϣ�͹ҵ���Ϣ
	void readInsulaGeoParaData_V(HdlMdl_InsulaGeoPara_V insulaGeoPara_V);//��ȡ��Ե�Ӵ����β�������
	void writeInsulaHangParaData_V(HdlMdl_InsulaHangPara_V insulaHangPara_V);//д���Ե�Ӵ����ݣ�����������Ϣ�͹ҵ���Ϣ

public slots:
	virtual void upDateSgstGap(const std::vector<std::string>& fourSugestGap);//���¼�϶�Ƽ�ֵ

private slots:
	void slotCurrentIndexChanged();//�������л�
	void slotConnectPoint(int index, /*���*/
						  osg::Vec3 pt/*�������*/);//���ӽڵ�
	void slotReverseHangPoint();//��ת�ҵ�����
	void slotOpenInsulatorLibDlg();//��Ե�Ӵ���鿴

private:
	QLabel                   *_rightAreaLbl;//ʾ��ͼlabel
	QComboBox                *_splitNumCmb;//������combox

	QGroupBox                 *_marginUpGBox;//�Ƿ��Ƕ�����ԣ��
	QLineEdit                 *_marginUpEdit;//����ԣ��ֵ

	StTableWidget            *_geoParaTable;//��Ե�Ӵ����β�����
	StTableWidget            *_hangPointTable;//��Ե�Ӵ��ҵ������

	HdlDataUI_MaxAngle_V      _dataUI_MaxAngle_V;//��������
	
};
typedef QPointer<MaxAngleWgt_V> HdlMaxAngleWgt_V;