/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.8.1
//Description:	���ɱ������
/****************************************************************/
#pragma once
#include "Report_MaxAngle.h"
#include "Mdl_GapValuePara_VL.h"
#include "Mdl_InsulaGeoPara_V.h"
#include "DataUI_MaxAngle_V.h"

using namespace SmartCl;
using namespace std;

class Report_MaxAngle_V :public Report_MaxAngle
{
public:
	Report_MaxAngle_V(const string &clName);
	virtual ~Report_MaxAngle_V();

private:
	virtual void gapInfo(QString &html) const;//��϶ֵ��Ϣ
	virtual void insulatorInfo(QString &html) const;//��Ե�Ӵ���Ϣ
	virtual void angleRes(QString &html) const;//�Ƕȼ�����

private:
	HdlDataUI_MaxAngle_V         _dataUI_MaxAngle_V;//����������

	HdlMdl_GapValuePara_VL       _gapValuePara_V;//V �����ֹ�����Ӧ�ļ�϶ֵ����
	HdlMdl_InsulaHangPara_V     _insulaHangPara_V;//��Ե�Ӵ����β���

};