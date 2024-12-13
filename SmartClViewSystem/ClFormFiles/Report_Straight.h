/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.8.1
//Description:	���ɱ������
/****************************************************************/
#pragma once
#include "Report.h"
#include "Mdl_BasicPara.h"
#include "Mdl_CalcRelePara.h"
#include "Mdl_SpanPara.h"
#include "Mdl_VSpanCalSet.h"
#include "Mdl_MetaAreaPara.h"
#include "Mdl_WireRelatPara.h"
#include "Mdl_InsultorPara_I.h"
#include "DataUI_Stright.h"
#include "MechCalProPara.h"
#include "SwingAnglePara.h"

using namespace SmartCl;
using namespace std;

class Report_Straight:public Report
{
public:
	Report_Straight(const string & clName);
	virtual ~Report_Straight();

private:
	virtual void writeContent(QString &html) const;//д��������
	void getWireCalaResPara();//���IVL��ͬ��϶Բ���ͻ�ȡ���߼�����ز�����

	void baseInfo(QString &html) const;//�����ſ�
	void meteInfo(QString &html) const;//��������Ϣ
	virtual void wireInfo(QString &html) const = 0;//������Ϣ
	virtual void insulatorInfo(QString &html) const = 0;//��Ե�Ӵ���Ϣ
	virtual void gapInfo(QString &html) const = 0;//��϶ֵ��Ϣ
	void calcInfo(QString &html) const;//���������Ϣ
	void tensionCalcRes_Code2010(QString &html) const;//���������м���̲��� 2010�ɹ淶
	void tensionCalcRes_Code2020(QString &html) const;//���������м���̲��� 2020�¹淶
	virtual void swingAngleRes(QString &html) const = 0;//ҡ�ڽǼ�����

protected:
	HdlDataUI_Stright          _dataUI;//ֱ�����������ݻ���

	HdlMdl_BasicPara           _basicInfo;//������Ϣ
	HdlMdl_CalcRelePara        _calcRelePara;//�������
	HdlMdl_SpanPara            _spanPara;//������Ϣ
	HdlMdl_VSpanCalSet         _vSpanCalSet;//��ֱ�����������
	HdlMdl_MetaAreaPara        _meteAreaPara;//���󹤿���ز���
	HdlMdl_WireRelatPara       _wireRelatPara;//������ز���
	HdlMdl_InsultorPara_I      _insultorPara_I;//��Ե�Ӳ���,�ں��м����ƫ����

	//HdlMechCalProPara          _calProPara;//���й��������غɼ���������ݣ�������IVL��
	HdlMechCalProPara          _tensionCalcProPara;//�������㣬���й����������ݣ�������IVL��
	HdlMechCalProPara          _loadCalcProPara;//���ؼ��㣬���й�������������ݣ�������IVL��
	HdlSwingAngleParaLib       _angleCalcPara;//�������׵���磬ҡ�ڽǼ�����ز����ʣ�����IV��

	HdlSwingAngleParaLib       _angleCalcPara_alpha;//�������׵���磬ҡ�ڽǼ�����ز����ʣ�����IV��
	HdlSwingAngleParaLib       _angleCalcPara_beta;//�������׵���磬ҡ�ڽǼ�����ز����ʣ�����IV��

};