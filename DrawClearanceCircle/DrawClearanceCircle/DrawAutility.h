/****************************************************************/
//Author: �����
//Version: 	1.0
//Date:		2019.7.10
//Description:	����Ϊ��ͼ��Ҫ�õ�һЩ���ߺ���
/****************************************************************/
#pragma once
#include "OdaCommon.h"
#include <list>
#include "DbObjectId.h"
#include "MeteAreaLib.h"
#include "Ge\GePoint3dArray.h"
#include "DbArc.h"
#include "Ed\EdCommandContext.h"
#include "DbDatabase.h"
#include "DbLine.h"
#include "qstring.h"
#include "Clearence.h"
#include "J3DModelData.h"

namespace SmartCl{

	typedef std::pair<OdDbObjectId, OdGePoint3dArray> PointOnCircle;//Բid��Բ����������Բ�����ӳ��
	typedef std::pair<OdDbObjectId, OdDbArcPtr> ArcOnCircle;//Բid��һ�������Բ�ϵĵ�ӳ��
	typedef std::pair<std::list<OdGePoint3d>, OdGePoint3d> ElecPointAndRotaCenter;//�ŵ��ͷŵ����תԲ�ĵ�ӳ��
	//����ɫö��
	class LineColor
	{
	public:
		enum Type
		{
			Wind = 0,
			InOverVolt = 1,
			OutOverVolt = 2,
			HotLineWork = 3,
			None = 4,
		};

		//���ݹ�����ȡ������ɫ
		static int LineColorIndex(LineColor::Type type/*���*/);
	};

	//������Բ��ص�������Ϣ
	class CirclesBoundaryInfo
	{
	public:
		CirclesBoundaryInfo::CirclesBoundaryInfo(const std::list<OdGePoint3d> &centrePoints,/*Բ�ĵ�*/
												 double radius,/*�뾶*/
												 int colorIndex/*����ɫ*/)
												 :_centrePoints(centrePoints),
												 _radius(radius),
												 _colorIndex(colorIndex){}

	public:
		std::list<OdGePoint3d>     _centrePoints;//Բ�ĵ�
		double                     _radius;//�뾶
		int                        _colorIndex;//����ɫ
	};

	class DrawAutility
	{
	public:
		DrawAutility();
		~DrawAutility();

		//���ݼ�϶Բ���ƻ�ȡ��Ӧ��϶Բ
		static HdlCl getCl(QString clName/*��϶Բ����*/);

		//��ȡԲ�Ľ���
		static void getCircleIntersection(std::list<OdDbObjectId> listCircle, /*Բlist*/
										  std::list<PointOnCircle> &listPoint/*Բid��Բ����������Բ�����ӳ�� list*/);

		//����Բ��ɾ������Χ���ڵ�Բ����ɾ���ظ�Բ��
		static void calcArc(const std::list<PointOnCircle>& pointOnCircle, /*Բid��Բ����������Բ�����ӳ�� list*/
							const std::list<OdDbObjectId>& listCircle, /*Բlist����Ҫɾ���ظ�Բ*/
							std::list<ArcOnCircle> & listArcOnCircle /*Բid��һ�������Բ�ϵ�Բ����ӳ�� list*/);

		//ɸѡ��Ҫ��ע��Բ���������б�ע
		static void dimArcs(std::list<OdDbArcPtr> listArc,/*��Ҫ��ע��Բ��*/
							OdDbObjectId g_idDimStyle,/*��ע���*/
							double dimScale/*��ע�ߴ�����*/);
		
		//���ܣ��жϵ��Ƿ�����һ���Ƕȷ�Χ�ڲ�
		/*����ֵ
		-2 ����Ƕȷ�Χ����
		0 �����������
		1 �ڷ�Χ��
		-1 �ڷ�Χ��
		*/
		static int isAngleInside(OdDbLinePtr pLineStart, /*һ���Ƕȵ����ֱ��*/
								 OdDbLinePtr pLineEnd, /*һ���Ƕȵ��յ�ֱ��*/
								 OdGePoint3d pointVertex/*���жϵĵ�*/);
		
		//���ܣ��ü����� I���Գ���ֱ�߲ü�
		static void clipArcOrCircle(std::list<OdDbArcPtr> &listArc,/*��Ҫ�ü���Բ��*/
			                        OdGePoint3d pointBase, /*�ü�ֱ�����*/
									double dLength, /*�ü�ֱ�߳���*/
									bool bLeft/*�ü��ı�*/);
		
		//��ȡ����ߵĵ㼯���Զ������ظ���
		static std::list<OdGePoint3d> getLinePoints(const std::list<OdDbObjectId> &listLineID/*��id list*/);

		//�����ظ���
		static void filterDupPoint(std::list<OdGePoint3d> &pointList/*�� list*/);

		//�����ظ���
		static void filterDupLine(std::list<OdDbObjectId> &lineList/*�� list*/);

		//���ƶ��Բ��ɵ���߽�
		static std::list<OdDbArcPtr> drawCirclesBoundary(const std::list<CirclesBoundaryInfo> &drawInfoList,/*�����Ϣ*/
														 bool isHiddenLine = false/*�Ƿ�Ϊ����*/);

		//ɾ���ظ���Բ
		static void deleteCoincideCircle(std::list<OdDbObjectId>& listCircle/*Բlist*/);

		//���Ʒ��ѵ��ߵ�ʵ��
		static std::list<OdDbObjectId> drawSplitEntity(const std::list<OdGePoint3d> &listPoint3d /*���Ѷ˵�����*/);

		//����������Ķ˵�����
		static std::list<OdGePoint3d> creatNumSplitEntity(OdGePoint3d pointHook,/*�ҵ�����*/
														  double spaceSplit,/*���Ѽ��*/
														  double heightSplit,/*���Ѹ߶ȣ�����Է�����Ϊ3�Ҳ��ǵȱ������ε�ʱ������*/
														  SplitNum::Type splitType/*������*/);

		//���Ʒŵ��ʾ��㣬��С�뾶��Բ��ʾ
		static void drawElecPointCircle(std::list<CirclesBoundaryInfo> drawInfoList,/*�ŵ�㼯*/
										double radius/*Բ�뾶*/);

		//����ֱ����I���߼��Լ�С����
		static std::list<OdDbObjectId> draw_I_L5L6(double dL5,/*L5���ȣ��߼г���*/
										           double dL6,/*L6���ȣ�С����*/
										           const std::list<OdGePoint3d> &listPoint3d/*��ʼ�ο���*/);

		//���ܣ�����I���߼м�С�������֣����ط��������ε��߶�ID
		static std::list<OdDbObjectId> draw_I_L5L6Theta(OdGePoint3d pointHook, /*���ѽ�߹ҵ�*/
														double dL5, /*�߼г���*/
														double dL6, /*С��������*/
														double dAngle, /*С����ƫ��*/
														const std::list<OdGePoint3d> &listPoint3d, /*�������˵�*/
														bool isDrawLine = true/*�Ƿ���ƻ����߶Σ�Ĭ��Ϊ����*/);

		//���ܣ�����V���߼м�С�������֣����ط��������ε��·��˵�����
		static std::list<OdGePoint3d> draw_VL_L6L7(OdGePoint3d pointHook, /*���ѽ�߹ҵ�*/
											       double dL6, /*�߼г���*/
												   double dL7, /*С��������*/
												   double dAngle, /*С����ƫ��*/
												   const std::list<OdGePoint3d> &listPoint3d, /*�������˵�*/
												   bool isDrawLine = true,/*�Ƿ���ƻ����߶Σ�Ĭ��Ϊ����*/
												   bool isRightTriangle = true/*ֱ�������Σ�true�����������Σ�false*/);



		//���ܣ�����V��Բ��
		static std::list<OdDbArcPtr> drawArc_V(const std::list<OdGePoint3d> &listArcCenter,/*����ԭ��list*/
										      double dRadius, /*�뾶*/
											  int color,/*����ɫ*/
											  OdDbLinePtr pLineL1,/*V�ǣ��߶�1*/
											  OdDbLinePtr pLineL2 /*V�ǣ��߶�2*/);

		//���ܣ�����V��Բ��
		static std::list<OdDbArcPtr> drawArc_V(const std::list<CirclesBoundaryInfo>& drawInfoList,/*�����Ϣ*/
											   OdDbLinePtr pLineL1,/*V�ǣ��߶�1*/
											   OdDbLinePtr pLineL2 /*V�ǣ��߶�2*/);


		//���ܣ���������ֱ�ߵļн�
		static double getTwoLineAngle(OdDbLinePtr pLine1,/*��1*/
									  OdDbLinePtr pLine2/*��2*/);

		//���ܣ���Բ�ĺͰ뾶Ϊ����������Բ���ϵĵ�Ч8����
		static std::list<OdGePoint3d> getPointOnCircle(OdGePoint3d centerPoint/*Բ�ĵ�*/, 
													   double radius/*�뾶*/);
	};
}