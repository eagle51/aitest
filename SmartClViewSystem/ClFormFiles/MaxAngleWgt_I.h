/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.9.9
//Description:	ֱ����I�����ҡ�ڽ� ��������
/****************************************************************/
#pragma once
#include "MaxAngleWgt.h"
#include "DataUI_MaxAngle_I.h"
#include "StTableWidget.h"

using namespace SmartCl;

class MaxAngleWgt_I : public MaxAngleWgt
{
	Q_OBJECT

public:
	MaxAngleWgt_I(int clType, 
		          std::string clName);
	virtual ~MaxAngleWgt_I();

	virtual void readChildClDataToForm();
	virtual void writeChildClDataFromForm();
	virtual QGroupBox* initGapValueGbx();//���ֹ�����϶ֵ��Ͽ�
	virtual QGroupBox* initInsulatorParaGbx();//��Ե�Ӵ���Ϣ��Ͽ�
	virtual void writeReport();//���ɱ���
	virtual void initialHangPointTableData();//�µ�����ģ��ʱˢ�³�ʼ���ҵ������

	void readGapValueParaData(HdlMdl_GapValuePara_I gapValuePara_I);//���ں˶�ȡ��϶ֵ����
	void writeGapValueParaData(HdlMdl_GapValuePara_I gapValuePara_I);//д���϶ֵ�������ں�

	void readInsulaHangParaData_I(HdlMdl_InsulaHangPara_I insulaHangPara_I);//��ȡ��Ե�Ӵ����ݣ�����������Ϣ�͹ҵ���Ϣ
	void readInsulaGeoParaData_I(HdlMdl_InsulaGeoPara_I insulaGeoPara_I);//��ȡ��Ե�Ӵ����β�������
	void writeInsulaHangParaData_I(HdlMdl_InsulaHangPara_I insulaHangPara_I);//д���Ե�Ӵ����ݣ�����������Ϣ�͹ҵ���Ϣ

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

	StTableWidget            *_geoParaTable;//��Ե�Ӵ����β�����
	StTableWidget            *_hangPointTable;//��Ե�Ӵ��ҵ������

	HdlDataUI_MaxAngle_I      _dataUI_MaxAngle_I;//��������
};
typedef QPointer<MaxAngleWgt_I> HdlMaxAngleWgt_I;