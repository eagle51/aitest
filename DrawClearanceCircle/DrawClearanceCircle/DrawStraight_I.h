/****************************************************************/
//Author: �����
//Version: 	1.0
//Date:		2019.7.15
//Description:	I����ͼ
/****************************************************************/
#pragma once
#include "DataDraw_StrightI.h"
#include "DrawAutility.h"
#include "DrawCl.h"
#include "SmartCADTable.h"

using namespace SmartCl;

class DrawStraight_I : public DrawCl
{
public:
	DrawStraight_I();
	~DrawStraight_I();

	//����I��������
	void drawStraight_I(QString clName/*��϶Բ����*/);

private:
	//���ƾ�Ե�Ӵ����νṹ���Լ���϶Բ
	double drawInsulateGeo_I(HdlDataDraw_StrightI drawData, /*I����ͼ�������*/
							 OdGePoint3d ptStart/*��ͼ���*/);

	//���Ʊ��
	void drawTableInfo_I(HdlDataDraw_StrightI drawData,/*��ͼ�������*/
						 OdGePoint2d insertPt/*������Ͻǵ�*/);

	//��ƫ�Ƕȷ����Ƿ�һ��
	bool isSwingAngleSameDirection(HdlAngleForFourCate angleForFourCate/*ҡ�ڽǲ���*/);

private:
	double _sizeScale;
};