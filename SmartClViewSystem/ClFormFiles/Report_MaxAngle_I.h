/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2020.1.6
//Description:	���ɱ������
/****************************************************************/
#pragma once
#include "Report_MaxAngle.h"
#include "Mdl_GapValuePara_I.h"
#include "Mdl_InsulaGeoPara_I.h"
#include "DataUI_MaxAngle_I.h"

using namespace SmartCl;
using namespace std;

class Report_MaxAngle_I :public Report_MaxAngle
{
public:
	Report_MaxAngle_I(const string &clName);
	virtual ~Report_MaxAngle_I();

private:
	virtual void gapInfo(QString &html) const;          //��϶ֵ��Ϣ
	virtual void insulatorInfo(QString &html) const;    //��Ե�Ӵ���Ϣ
	virtual void angleRes(QString &html) const;         //�Ƕȼ�����

private:
	HdlDataUI_MaxAngle_I         _dataUI_MaxAngle_I;//����������

	HdlMdl_GapValuePara_I       _gapValuePara_I;//I �����ֹ�����Ӧ�ļ�϶ֵ����
	HdlMdl_InsulaHangPara_I     _insulaHangPara_I;//��Ե�Ӵ����β���

};