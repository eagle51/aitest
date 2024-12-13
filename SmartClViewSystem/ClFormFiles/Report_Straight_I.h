/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.8.1
//Description:	���ɱ���-ֱ����I��
/****************************************************************/
#pragma once
#include "Report_Straight.h"
#include "Mdl_GapValuePara_I.h"
#include "Mdl_InsulaGeoPara_I.h"

using namespace SmartCl;

class Report_Straight_I : public Report_Straight
{
public:
	Report_Straight_I(const string & clName);
	~Report_Straight_I();

	virtual void wireInfo(QString &html) const;//������Ϣ
	virtual void insulatorInfo(QString &html) const;//��Ե�Ӵ���Ϣ
	virtual void gapInfo(QString &html) const;//��϶ֵ��Ϣ
	virtual void swingAngleRes(QString &html) const;//ҡ�ڽǼ�����

private:
	void swingAngleRes_Code2010(QString &html) const;//ҡ�ڽǼ����� 2010�淶
	void swingAngleRes_Code2020(QString &html) const;//ҡ�ڽǼ����� 2020�淶

private:
	HdlMdl_GapValuePara_I      _gapValuePara_I;//I �����ֹ�����Ӧ�ļ�϶ֵ����
	HdlMdl_InsulaGeoPara_I     _insulaGeoPara_I;//��Ե�Ӵ����β���
};