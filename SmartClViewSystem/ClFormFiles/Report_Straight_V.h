/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.8.1
//Description:	���ɱ���-ֱ����V��
/****************************************************************/
#pragma once
#include "Report_Straight.h"
#include "Mdl_GapValuePara_VL.h"
#include "Mdl_InsultorPara_V.h"
#include "Mdl_InsulaGeoPara_V.h"

using namespace SmartCl;

class Report_Straight_V : public Report_Straight
{
public:
	Report_Straight_V(const string & clName);
	~Report_Straight_V();

	virtual void wireInfo(QString &html) const;//������Ϣ
	virtual void insulatorInfo(QString &html) const;//��Ե�Ӵ���Ϣ
	virtual void gapInfo(QString &html) const;//��϶ֵ��Ϣ
	virtual void swingAngleRes(QString &html) const;//ҡ�ڽǼ�����

private:
	void swingAngleRes_Code2010(QString &html) const;//ҡ�ڽǼ����� 2010�淶
	void swingAngleRes_Code2020(QString &html) const;//ҡ�ڽǼ����� 2020�淶

private:
	HdlMdl_GapValuePara_VL     _gapValuePara_VL;//V �����ֹ�����Ӧ�ļ�϶ֵ����
	HdlMdl_InsultorPara_V      _insultorPara_V;//��Ե�Ӵ�����
	HdlMdl_InsulaGeoPara_V     _insulaGeoPara_V;//��Ե�Ӵ����β���
};