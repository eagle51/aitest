/****************************************************************/
//Author: �����
//Version: 	1.0
//Date:		2019.9.11
//Description:	ֱ���� ���ҡ�ڽǻ�ͼ
/****************************************************************/
#pragma once
#include "DrawAutility.h"
#include "DrawCl.h"
#include "qstring.h"
#include "Cl_MaxAngle_I.h"

using namespace SmartCl;

class DrawMaxAngle : public DrawCl
{
public:
	DrawMaxAngle();
	~DrawMaxAngle();

protected:
	//��������ģ��
	std::list<OdDbObjectId> drawTowerModel(const HdlJ3DModelData towerModelData,/*����ģ������*/
										   OdGePoint3d shiftDirection/*ƽ�Ƶ�����*/);

	//����㼯�� �߶μ�����С����
	double calcDistance(const std::list<OdDbObjectId> &towerLineList, /*�����߶� ID list*/
						const std::list<OdGePoint3d> &elecPointList /*�ŵ�� list*/);

protected:
	std::list<OdDbObjectId>   _towerLineList;//�����˼�list
	HdlCl                     _Cl;//��϶Բ
};