/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2020.1.6
//Description:	�������ҡ�ڽǱ������
/****************************************************************/
#pragma once
#include "Report.h"
#include "DataUI_MaxAngle.h"

using namespace SmartCl;
using namespace std;

class Report_MaxAngle:public Report
{
public:
	Report_MaxAngle(const string & clName);
	virtual ~Report_MaxAngle();

private:
	virtual void writeContent(QString &html) const;//д��������

	void baseInfo(QString &html) const;//�����ſ�
	virtual void insulatorInfo(QString &html) const = 0;//��Ե�Ӵ���Ϣ
	virtual void gapInfo(QString &html) const = 0;//��϶ֵ��Ϣ
	virtual void angleRes(QString &html) const = 0;//ҡ�ڽǼ�����

protected:
	HdlDataUI_MaxAngle          _dataUI;//���ݻ���

	HdlMdl_BasicPara           _basicInfo;//������Ϣ
};