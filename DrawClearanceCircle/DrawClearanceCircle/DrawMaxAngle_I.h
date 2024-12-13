/****************************************************************/
//Author: �����
//Version: 	1.0
//Date:		2019.9.11
//Description:	ֱ����I�� ���ҡ�ڽǻ�ͼ
/****************************************************************/
#pragma once
#include "Cl_MaxAngle_I.h"
#include "DrawMaxAngle.h"

using namespace SmartCl;

class DrawMaxAngle_I : public DrawMaxAngle
{
public:
	DrawMaxAngle_I();
	~DrawMaxAngle_I();

	//����������
	void drawMaxAngle_I(const QString& clName);


private:
	void draw(HdlDataUI_MaxAngle_I dataUI_MaxAngle_I);

	//����ŵ���λ��
	ElecPointAndRotaCenter calcElecPoint(HdlMdl_InsulaHangPara_I insulaHangPara_I,/*��Ե�Ӵ����β���*/
										 OdGePoint3d shiftDirection/*ƽ�Ƶ�����*/);

	//���ƾ�Ե�Ӵ�
	void drawInsulator(HdlMdl_InsulaHangPara_I insulaHangPara_I,/*��Ե�Ӵ����β���*/
					   double angle,/*ת���Ƕ�*/
					   OdGePoint3d shiftDirection,/*ƽ�Ƶ�����*/
					   GapCate::Type gapCateType/*��������*/);

};