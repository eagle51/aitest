/****************************************************************/
//Author: �����
//Version: 	1.0
//Date:		2019.12.31
//Description:	ֱ����V�� ���ҡ�ڽǻ�ͼ
/****************************************************************/
#pragma once
#include "DrawAutility.h"
#include "DrawCl.h"
#include "qstring.h"
#include "Cl_MaxAngle_V.h"
#include "DrawMaxAngle.h"

using namespace SmartCl;

class DrawMaxAngle_V : public DrawMaxAngle
{
public:
	DrawMaxAngle_V();
	~DrawMaxAngle_V();

	//����������
	void drawMaxAngle_V(const QString& clName);

private:
	void draw(HdlDataUI_MaxAngle_V dataUI_MaxAngle_V);

	//����ŵ��λ��
	std::list<OdGePoint3d> calcElecPoint(HdlMdl_InsulaHangPara_V insulaHangPara_V,/*��Ե�Ӵ����β���*/
										 OdGePoint3d shiftDirection,/*ƽ�Ƶ��������������ֻ��Ʋ�ͬ����*/
										 double moveDownDis/*��Ե�Ӵ�������ʼ����������ҵ��е�λ�õ����ƾ���*/);

	//���ƾ�Ե�Ӵ�����ģ�ͼ���϶Բ������V���н�
	double drawInsulatorAndCircle(HdlMdl_InsulaHangPara_V insulaHangPara_V,/*��Ե�Ӵ����β���*/
								 HdlMdl_GapValuePara_VL gapValuePara_V,/*��϶��Ϣ*/
					             OdGePoint3d shiftDirection,/*ƽ�Ƶ��������������ֻ��Ʋ�ͬ����*/
					             double moveDownDis/*��Ե�Ӵ�������ʼ����������ҵ��е�λ�õ����ƾ���*/);

	//ɸѡ������Χ���������߶ȷ����и���ڼ���������������������
	std::list<OdDbObjectId> filtTowerModel(const std::list<OdDbObjectId> &towerLineList,/*������ list*/
						                   OdGePoint3d breakPoint,/*�ָ��*/
										   bool isdeleteDown/*�Ƿ�Ϊɾ���ָ���·���������*/);

};