/****************************************************************/
//Author: �����
//Version: 	1.0
//Date:		2019.9.11
//Description: ��ͼʵ�������
/****************************************************************/
#pragma once
#include "Ge\GePoint3d.h"
#include "DbEntity.h"
#include "Gs\GsFiler.h"
#include "DbDimStyleTableRecord.h"
#include "Ed\EdCommandContext.h"
#include "qstring.h"
#include <list>
#include "DbCurve.h"
#include "DbArc.h"
#include "DbLine.h"

class CreateEnt
{
public:
	//���������� ��ע��ʹ�ñ���ǰ�뽫��ָ�븳ֵ
	static OdEdCommandContext* _pCmdCtx;

	//���ܣ���ȡ���ݿ�
	static OdDbDatabasePtr getDataBase();

	//���ܣ���ȡ����ʵ��
	static std::list<OdDbObjectId> getAllEntity();

	//���ܣ���ӵ�ģ�Ϳռ�
	static OdDbObjectId addToModelSpace(OdDbEntityPtr entity);//��Ҫ��ӵ�ʵ��

	//���ܣ����ͼֽ
	static void clearModelSpace();

	//���ܣ�ɾ��ʵ��
	static void deletEntity(const std::list<OdDbObjectId> &entityList/*��Ҫɾ����ʵ��*/);

	//���ܣ�����ֱ��
	static OdDbObjectId drawLine(const OdGePoint3d& ptStart, //�߶����
								 const OdGePoint3d& ptEnd, // �߶��յ�
								 int colorIndex = 0/*�߶���ɫ, Ĭ��7Ϊ��ɫ��*/);

	//���ܣ�����ֱ��
	static OdDbObjectId drawLineAAA(const OdGePoint3d& ptStart, //�߶����
								 const OdGePoint3d& ptEnd, // �߶��յ�
									OdDbObjectId lineTypeId,
								 int colorIndex = 0/*�߶���ɫ, Ĭ��7Ϊ��ɫ��*/)
	{
		OdDbLinePtr pLine = OdDbLine::createObject();
		pLine->setStartPoint(ptStart);
		pLine->setEndPoint(ptEnd);
		pLine->setColorIndex(colorIndex);
		pLine->setLineWeight(OdDb::kLnWtByLayer);
		pLine->setLinetype(lineTypeId);
		return addToModelSpace(pLine);
	}

	//���ܣ�����ֱ��
	static OdDbObjectId drawPolyLine(const OdGePoint3d& ptStart, //�߶����
								  const OdGePoint3d& ptEnd, //�߶��յ�
								  double lineWidth = -1.0, /*�߿�*/
								  int colorIndex = 0/*�߶���ɫ*/);

	//���ܣ����ƶ����
	static OdDbObjectId drawPolyLineByPoints(const OdGePoint2dArray& ptSet, //����ߵ㼯
								          bool isClosed = false, //�Ƿ�պ� 
										  double lineWidth = -1.0, /*�߿�*/
										  int colorIndex = 0/*�߶���ɫ*/);

	//���ܣ����ƶ����
	static OdDbObjectId drawPolyLineByPoints(const std::list<OdGePoint3d> listPoint, //����ߵ㼯
											 bool isClosed = false, //�Ƿ�պ� 
											 double lineWidth = -1.0, /*�߿�*/
											 int colorIndex = 0/*�߶���ɫ*/);

	//���ܣ�����2d����
	static OdDbObjectId drawRect(const OdGePoint3d& leftTopPoint, //���Ͻǵ�
								 double height, //�߶�
								 double width,//���
								 double lineWidth = -1.0, /*�߿�Ĭ��Ϊ���߿�*/
								 int colorIndex = 0/*�߶���ɫ*/);

	//���ܣ����Ƶ����ı�
	static OdDbObjectId drawText(const OdGePoint3d& point3d, /*�ı���ʼλ�ã����Ͻǵ� */
							  const OdString& text, //�ı�����
							  OdDbObjectId& textStyleId,//����
							  double height = 10, //�ָ�
							  int colorIndex = 0); //��ɫ

	//���ܣ����ƶ����ı����Զ�����
	static OdDbObjectId drawMultiText(const OdGePoint3d& point3d, /*�ı���ʼλ�ã����Ͻǵ� */
								   const OdString& text, //�ı�����
								   const OdDbObjectId& textStyleId,//����
								   double textWidth,//�ı��Զ����п��
								   double lineSpacingFactor = 1.0,//�м��ϵ��
								   double height = 10.0, //�ָ�
								   int colorIndex = 0/*��ɫ*/);

	//���ܣ�����Բ
	static OdDbObjectId drawCircle(const OdGePoint3d& ptCenter, //Բ��λ��
								   double radius, //�뾶
								   bool isHiddenLine,/*�Ƿ�Ϊ����*/
								   int colorIndex = 2/*��ɫ*/);

	//����Բ�Ļ��ƶ���Բ
	static std::list<OdDbObjectId> drawCircles(const std::list<OdGePoint3d> &listCenter,/*Բ�ĵ㼯*/
											   double dRadius, /*�뾶*/
											   bool isHiddenLine,/*�Ƿ�Ϊ����*/
											   int color/*��ɫ*/);

	//���ܣ����ƻ�
	static OdDbObjectId drawArc(const OdGePoint3d& ptCenter, //����λ��
							 double radius, //�뾶
							 double startAngle, //��ʼ����
							 double endAngel, //��ֹ����
							 int colorIndex = 2/*����ɫ*/);

	//��תʵ���߶�
	static std::list<OdDbObjectId> rotateCurve(OdGePoint3d pointOrigin, /*��ת���ĵ�*/
											   double dAngle, /*��ת�Ƕȣ��ǻ���*/
											   const std::list<OdDbObjectId> &listWholeID/*��Ҫ��ת��ʵ���߶�id*/);

	//��ת ��list
	static void rotatePoint(std::list<OdGePoint3d> &pointList,/*��list*/
							OdGePoint3d pointOrigin, /*��ת���ĵ�*/
							double dAngle /*��ת�Ƕȣ��ǻ���*/);

	//ƽ�� ��list
	static void translatePoint(std::list<OdGePoint3d> &pointList,/*��list*/
							   OdGePoint3d direction/*ƽ�Ʒ��򣬵�����*/);

	//���ܣ����ƶ����ע  // TODO: ��ע��ʽ��������Ϊ��ͷ��ʽ�����޸�
	static OdDbObjectId dimAligned(const OdGePoint3d& dimLinePt, //��ע��Ϣλ�� 
								   const OdGePoint3d& ptStart, //��ע���
								   const OdGePoint3d& ptEnd //��ע�յ� 
								   /*OdDbObjectId dimStyleId*//*��ע��ʽ*/);
	//���ܣ��������Ա�ע
	static OdDbObjectId dimAligned(OdDbLinePtr pLine, /*��Ҫ��ע��ֱ��*/
								   OdDbObjectId dimStyleID,/*��ע��ʽ*/
								   double dimScale/*��ע�ߴ�����*/);

	//���ܣ����ƽǶȱ�ע  
	static OdDbObjectId dimAngle(const OdGePoint3d& rotataCenter, /*���ĵ�*/
								 const OdGePoint3d& linePoint,/* ���ϵĵ�*/
								 double angle, /*�Ƕȣ��ǻ���*/
								 OdDbObjectId dimStyleID,/*��ע��ʽ*/
								 int colorIndex /*��ɫ*/);

	//���ܣ����ƽǶȱ�ע  
	static OdDbObjectId dimAngle(OdDbLine *pLineStart,/*��ʼ��*/
								 OdDbLine *pLineEnd,/*��ֹ��*/
								 OdDbObjectId g_idDimStyle,/*��ע���*/
								 int colorIndex /*��ɫ*/);

	//���ܣ���ע���뾶  
	static void dimArcRadial(std::list<OdDbArcPtr> pArcCircle, /*Բ��list*/
							 OdDbObjectId g_idDimStyle, /*��ע��ʽ*/
							 double dimScale/*��ע�ߴ�����*/);

	//���ܣ�����ͼֽ��
	static void saveDWG(const OdString& pathName/*����·��*/);

	//���ܣ���Ӳ�
	static OdDbObjectId addLayer(const OdString& layerName, /*������*/
								 int colorIndex, /*��ɫ*/
								 OdDb::LineWeight lineWeight/*�߿�*/);
	
	//���ܣ����������ʽ
	static OdDbObjectId addTextStyle(const OdString& styleName/*������ʽ����*/,
									 const OdString& fileName/*С��������*/,
									 const OdString& bigFileName/*����������*/);

	//���ܣ�������
	static void addHatch();

	//���ܣ��������  // TODO: ֻд��Hidden����
	static OdDbObjectId getHiddenLineType();

	//���ܣ�
	static OdDbObjectId addObliqueDimStyle();

	//���ܣ���ӱ�ע��ʽ
	static OdDbObjectId addDimStyle(const OdString& dimStyleName/*��ע��ʽ����*/);

	//���ܣ�����ͼ��
	static OdDbObjectId insertBlk(const OdString& blockname/*ͼ������*/,
								  const OdString& strDwgFileName/*����dwg·��*/,
								  const OdGePoint3d& ptInsert /*��ʼλ��*/,
								  double dScale = 1.0 /*���ű���*/,
								  double dRotationAngle = 0.0 /*��ת�Ƕȣ�����*/);

	void drawMirrorShape(OdGePoint2dArray aryPt, const OdGePoint2d ptMirrorS,
						 const OdGePoint2d ptMirrorE,
						 OdRxObjectPtrArray& idBlockLayer, double dcolor = 0);



	//���ܣ���ȡ������е�
	static OdGePoint3d midPoint(const OdGePoint3d& point1, /*��1*/
								const OdGePoint3d& point2/*��2*/);

	//���ܣ����ö�Ӧ��עid����ʾ���
	static void setDimStyleRec(OdDbObjectId idDimStyle, /*��ע���id*/
							   double fontScale/*�������ű���*/);

	static void zoom_window(OdGePoint3d& pt1, OdGePoint3d& pt2);

	static void zoom_window(OdGePoint3d& pt1, OdGePoint3d& pt2, OdGsView* pGsView);

};