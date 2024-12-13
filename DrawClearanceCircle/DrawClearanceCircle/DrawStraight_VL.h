/****************************************************************/
//Author: �����
//Version: 	1.0
//Date:		2019.7.10
//Description:	VL����ͼ
/****************************************************************/
#pragma once
#include "DrawAutility.h"
#include "DataDraw_StrightV.h"
#include "DataDraw_StrightL.h"
#include "DrawCl.h"

using namespace SmartCl;

class DrawStraight_VL : public DrawCl
{
public:
	DrawStraight_VL();
	~DrawStraight_VL();

	//���ܣ�����������
	void drawStraight_VL(QString clName/*��϶Բ����*/);

private:
	//���ܣ����Ƽ��β���
	void drawInsulateGeo_VL(QString clName, /*��϶Բ����*/
							OdGePoint3d ptStart/*���*/);
	
	//���ܣ�����V�����
	void drawTableInfo_V(HdlDataDraw_StrightV drawData, /*��ͼ�������*/
						 OdGePoint2d insertPt/*������Ͻǵ�*/);

	//���ܣ�����L�����
	void drawTableInfo_L(HdlDataDraw_StrightL drawData, /*��ͼ�������*/
						 OdGePoint2d insertPt/*������Ͻǵ�*/);

};