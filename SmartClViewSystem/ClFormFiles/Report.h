/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.8.1
//Description:	���ɱ������
/****************************************************************/
#pragma once
#include <string>
#include "Clearence.h"
#include "qstring.h"

using namespace SmartCl;
using namespace std;

class Report
{
public:
	Report(const string &clName);
	virtual ~Report();

	void wordReport() const;//���ɱ���

private:
	virtual void writeContent(QString &html) const = 0;//д������������
	void firstPage(QString &html) const;//��Ƥ��һҳ

protected:
	HdlCl                      _cl;//��϶Բ

	double                     _fontSize_s2;//С����
	double                     _fontSize_s3;//С����
	double                     _fontSize_4;//�ĺ�
	double                     _fontSize_s4;//С�ĺ�
	double                     _fontSize_5;//���
};