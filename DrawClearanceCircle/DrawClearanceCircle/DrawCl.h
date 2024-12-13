/****************************************************************/
//Author: �����
//Version: 	1.0
//Date:		2019.7.22
//Description:	��ͼ����
/****************************************************************/
#pragma once
#include "DataDraw_StrightI.h"
#include "DrawAutility.h"
#include "DataBase.h"
#include "VirBox\VirBoxChecker.h"

class DrawCl
{
public:
	DrawCl();
	virtual ~DrawCl();

	static SmartCl::DataBase* getCoreInstance(); //�ں����ݿ�
	static AboutSoftWare::VirBoxChecker* getVirBoxCheckerInstance(); //��˼���ܼ��

protected:
	OdDbObjectId     _idDimStyle;//��ע��ʽ
	double           _fontScale;//�������ű���
	double           _plottingScale;//�����ߣ�����1:100
	double           _sizeScale;//�ۺ� �� ת��Ϊ ����֮���ٿ��Ǳ����ߺ���������ųߴ�
	double           _insulatorBaseLengh;//��Ե�Ӵ���׼���ȣ�����ȷ��fontScale��

	OdGeVector3d     _vecHorizontalUnit;//ˮƽ��λ����
	OdGeVector3d     _vecVerticalUnit;//��ֱ��λ����
};