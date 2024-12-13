#include "DrawAutility.h"
#include <QString>
#include "DbCircle.h"
#include "ExCommandsUtils.h"
#include "DbRadialDimension.h"
#include "DbBlockTableRecord.h"
#include "DbAlignedDimension.h"
#include "Db2LineAngularDimension.h"
#include "DbDimStyleTableRecord.h"
#include "DbObjectId.h"
#include "DbDatabase.h"
#include "AbstractViewPE.h"
#include "OdqInterfaces.h"
#include "DbTextStyleTableRecord.h"
#include "OdaDefs.h"
#include "DbText.h"
#include "DbPolyline.h"
#include "OdString.h"
#include "DbDimStyleTable.h"
#include "DbTextStyleTable.h"
#include "DataBase.h"
#include "..\SmartClApp\OdqMainWindow.h"
#include "CreateEnt.h"
#include "CADConsts.h"
#include "Auxiliary.h"

namespace SmartCl{
	DrawAutility::DrawAutility()
	{
	}

	DrawAutility::~DrawAutility()
	{
	}

	//@function: ���ݹ�����ȡ������ɫ   -- by mzq  2019/7/12 19:45
	int LineColor::LineColorIndex(LineColor::Type type/*���*/)
	{
		switch (type)
		{
		case LineColor::Type::Wind:
			return 4;//Cyan
		case LineColor::Type::InOverVolt:
			return 2;//Yellow
		case LineColor::Type::OutOverVolt:
			return 1;//Red
		case LineColor::Type::HotLineWork:
			return 3;//Green
		case LineColor::Type::None:
			return 7;//White
		default:
			return 7;
		}
	}

	//@function: ���ݼ�϶Բ���ƻ�ȡ��Ӧ��϶Բ   -- by mzq  2019/7/12 19:45
	HdlCl DrawAutility::getCl(QString clName/*��϶Բ����*/)
	{
		DataBase *database = static_cast<OdqMainWindow*>(getIApp()->getMainWindow())->getCoreDbPt();
		HdlCl curCl = database->getClByName(clName.toLocal8Bit().toStdString());//ͨ����϶Բ���ƻ�ȡ
		return curCl;
	}

	//@function: ��ȡԲ�Ľ���   -- by mzq  2019/11/4 11:24
	void DrawAutility::getCircleIntersection(std::list<OdDbObjectId> listCircle,/*Բlist*/
											 std::list<PointOnCircle> &listPoint/*Բid��Բ����������Բ�����ӳ�� list*/)
	{
		std::list<OdDbObjectId>::iterator iterCircleID = listCircle.begin();
		for (; iterCircleID != listCircle.end(); iterCircleID++)
		{
			OdDbCirclePtr pCircleEnt = iterCircleID->safeOpenObject(OdDb::kForRead);//��ǰԲ
			OdGePoint3d centerPoint = pCircleEnt->center();
			OdGePoint3dArray intersectPoints;//Բ�����еĽ���

			/*�Һ�����Բ�Ľ���*/
			std::list<OdDbObjectId>::iterator iterCircleIDNext = listCircle.begin();
			for (; iterCircleIDNext != listCircle.end(); iterCircleIDNext++)
			{
				if (*iterCircleIDNext == *iterCircleID)//�Լ������Լ��󽻵㣬����
					continue;

				OdDbCirclePtr pCircleEntNext = iterCircleIDNext->safeOpenObject(OdDb::kForRead);
				OdGePoint3dArray points;
				pCircleEnt->intersectWith(pCircleEntNext, OdDb::kOnBothOperands, points);//�����ཻ��
				int nPointSzie = points.size();

				if (nPointSzie != 0)//����н��㣬���¼��
					intersectPoints.insert(intersectPoints.end(), points.begin(), points.end());
			}

			if (intersectPoints.size() == 0)//�����ǰԲ������Բ���޽��㣬����
				continue;

			/*Բ�ϵĽ����������򣬰�����X��Ϊ��ʼλ�ã���ʱ�뷽������*/
			for (int iPoint = 0; iPoint < intersectPoints.size(); iPoint++)
			{
				double iRadians = 0.0;
				OdGePoint3d pointI = intersectPoints[iPoint];
				pCircleEnt->getParamAtPoint(pointI, iRadians);//�������Բ����x����ʱ����ת�ĽǶ�

				for (int jPoint = iPoint + 1; jPoint < intersectPoints.size(); jPoint++)
				{
					double jRadians = 0.0;
					OdGePoint3d pointJ = intersectPoints[jPoint];
					pCircleEnt->getParamAtPoint(pointJ, jRadians);
					if (iRadians > jRadians)
					{
						intersectPoints.swap(iPoint, jPoint);
						iRadians = jRadians;
					}
				}
			}

			listPoint.insert(listPoint.end(), std::make_pair(*iterCircleID, intersectPoints));
		}
	}

	//@function: ����Բ��ɾ������Χ���ڵ�Բ����ɾ���ظ�Բ��   -- by mzq  2019/11/4 11:24
	void DrawAutility::calcArc(const std::list<PointOnCircle>& pointOnCircle,/*Բid��Բ����������Բ�����ӳ�� list*/
							   const std::list<OdDbObjectId>& listCircle,/*Բlist����Ҫɾ���ظ�Բ*/
							   std::list<ArcOnCircle> & listArcOnCircle/*Բid��һ�������Բ�ϵ�Բ����ӳ�� list*/)
	{
		/*��һ�������ݽ��㽫Բ��ϲ�ֳ�Բ��*/
		std::list<PointOnCircle>::const_iterator iterCircle = pointOnCircle.begin();
		for (iterCircle; iterCircle != pointOnCircle.end(); iterCircle++)
		{
			OdDbCirclePtr pCircleEnt = iterCircle->first.safeOpenObject(OdDb::kForRead);//Բ
			OdGePoint3d centerPoint = pCircleEnt->center();
			OdUInt16 nColorIndex = pCircleEnt->colorIndex();
			double dRadius = pCircleEnt->radius();

			OdGePoint3dArray pointsOnCircle = iterCircle->second;//Բ�ϵĽ���

			/*����Բ�ϵĵ㽫Բ�ָ����Բ��*/
			OdGePoint3dArray::iterator iterPoint = pointsOnCircle.begin();
			OdGePoint3dArray::iterator iterPointNext = iterPoint;
			iterPointNext++;
			for (; iterPoint != pointsOnCircle.end(); iterPoint++, iterPointNext++)
			{
				if (iterPointNext == pointsOnCircle.end())
					iterPointNext = pointsOnCircle.begin();

				double iRadians = 0.0;
				double jRadians = 0.0;
				pCircleEnt->getParamAtPoint(*iterPoint, iRadians);
				pCircleEnt->getParamAtPoint(*iterPointNext, jRadians);

				OdDbArcPtr pArc = OdDbArc::createObject();
				pArc->setColorIndex(nColorIndex);
				pArc->setCenter(centerPoint);
				pArc->setRadius(dRadius);
				pArc->setStartAngle(iRadians);
				pArc->setEndAngle(jRadians);

				ArcOnCircle arcOnCircle = std::make_pair(iterCircle->first, pArc);//Բid��һ�������Բ�ϵĵ�ӳ��
				listArcOnCircle.insert(listArcOnCircle.end(), arcOnCircle);
			}
		}

		/*�ڶ�����ѭ����������Բ���������Բ���ڲ�����ɾ������Բ��*/
		std::list<ArcOnCircle>::iterator iterArc = listArcOnCircle.begin();
		for (; iterArc != listArcOnCircle.end();)
		{
			OdGePoint3d pointStartArc;//Բ�����
			OdGePoint3d pointEndArc;//Բ���յ�
			OdGePoint3d pointCenterArc;//Բ���е�

			/*��һ������ȡ��ǰԲ������㣬�յ㣬���ĵ�*/
			iterArc->second->getStartPoint(pointStartArc);
			iterArc->second->getEndPoint(pointEndArc);
			double dArcStartAngle = iterArc->second->startAngle();
			double dArcEndAngle = iterArc->second->endAngle();
			double dAngle;
			if (dArcEndAngle < dArcStartAngle)
				dAngle = (dArcStartAngle + dArcEndAngle + Oda2PI) / 2.0;
			else
				dAngle = (dArcStartAngle + dArcEndAngle) / 2.0;
			iterArc->second->getPointAtParam(dAngle, pointCenterArc);//����Բ�����ĽǶȻ�ȡԲ���ϵĵ�

			/*�ڶ�����ѭ����������Բ���жϵ�ǰԲ���Ƿ�������һ��Բ���ڲ����������ɾ����*/
			bool bContain = false;
			std::list<OdDbObjectId>::const_iterator iterCircle = listCircle.begin();
			for (; iterCircle != listCircle.end(); iterCircle++)
			{
				if (iterArc->first == *iterCircle)//�Լ������Լ��Ƚ�
					continue;

				/*�ж�һ��Բ���Ƿ�������һ��Բ���ڲ�*/
				OdDbCirclePtr pCircleEnt = iterCircle->safeOpenObject(OdDb::kForRead);
				OdGePoint3d pointCircle = pCircleEnt->center();
				double dRadiusCircle = pCircleEnt->radius();
				if ((pointCircle.distanceTo(pointCenterArc) - dRadiusCircle) <  CADConsts::dTolerance)
				{
					bContain = true;
					break;
				}
			}

			/*�������������ǰԲ��������һ��Բ�ڲ�����ɾ������Բ������������һѭ��*/
			if (bContain)
				listArcOnCircle.erase(iterArc++);
			else
				iterArc++;
		}
	}

	//@function: ɸѡ��Ҫ��ע��Բ���������б�ע   -- by mzq  2019/11/4 11:24
	void DrawAutility::dimArcs(std::list<OdDbArcPtr> listArc,/*��Ҫ��ע��Բ��*/
							   OdDbObjectId g_idDimStyle,/*��ע���*/
							   double dimScale/*��ע�ߴ�����*/)
	{
		typedef std::pair<OdUInt16, OdDbArcPtr> PairArc;
		std::list<PairArc> listBoundaryArc;
		std::list<OdDbArcPtr>::iterator citerArc = listArc.begin();
		for (citerArc; citerArc != listArc.end(); citerArc++)
		{
			OdDbArcPtr pArc = *citerArc;
			if (pArc->isA()->isEqualTo(OdDbArc::desc()))
			{
				OdGePoint3d pointArcEnd;
				pArc->getEndPoint(pointArcEnd);
				double dist = 0.0;
				pArc->getDistAtPoint(pointArcEnd, dist);
				std::list<PairArc>::iterator iterPairArc = listBoundaryArc.begin();
				for (iterPairArc; iterPairArc != listBoundaryArc.end(); iterPairArc++)
				{
					OdGePoint3d pointArcEnd;
					iterPairArc->second->getEndPoint(pointArcEnd);
					double distPair = 0.0;
					iterPairArc->second->getDistAtPoint(pointArcEnd, distPair);

					if (pArc->colorIndex() == iterPairArc->first && dist > distPair)
					{//�ҵ����滻				
						iterPairArc->second = pArc;
						break;
					}
				}
				//û�ҵ��ͼ���
				if (iterPairArc == listBoundaryArc.end())
				{
					listBoundaryArc.insert(listBoundaryArc.begin(), std::make_pair(pArc->colorIndex(), pArc));
				}
			}
		}

		std::list<OdDbArcPtr> pArcCircle;

		std::list<PairArc>::iterator iterArc = listBoundaryArc.begin();
		for (iterArc; iterArc != listBoundaryArc.end(); iterArc++)
		{
			OdDbArcPtr pArc = iterArc->second;
			pArcCircle.insert(pArcCircle.end(), pArc);
		}

		CreateEnt::dimArcRadial(pArcCircle, g_idDimStyle, dimScale);
	}

	//@function: ���ܣ��жϵ��Ƿ�����һ���Ƕȷ�Χ�ڲ�   -- by mzq  2019/11/4 11:24
	int DrawAutility::isAngleInside(OdDbLinePtr pLineStart, /*һ���Ƕȵ����ֱ��*/
									OdDbLinePtr pLineEnd, /*һ���Ƕȵ��յ�ֱ��*/
									OdGePoint3d pointVertex/*���жϵĵ�*/)
	{
		OdGePoint3dArray points;
		OdGePoint3d pointABIntersect;
		OdResult rt = pLineStart->intersectWith(pLineEnd, OdDb::kExtendBoth, points);
		if (rt != eOk && points.size() != 1)
			return -2;

		pointABIntersect = points.getAt(0);

		OdGeVector3d vectStartLine;//��ʼֱ�ߵ�����
		OdGeVector3d vectEndLine;//�յ�ֱ�ߵ�����
		OdGeVector3d vectVertex; //���жϵ������
		vectVertex = pointVertex - pointABIntersect;

		OdGePoint3d pointStart, pointEnd;
		pLineStart->getStartPoint(pointStart);
		pLineStart->getEndPoint(pointEnd);

		if (pointABIntersect.distanceTo(pointStart) > CADConsts::dTolerance)
			vectStartLine = pointStart - pointABIntersect;
		else
			vectStartLine = pointEnd - pointABIntersect;

		OdGePoint3d pointStart2, pointEnd2;
		pLineEnd->getStartPoint(pointStart2);
		pLineEnd->getEndPoint(pointEnd2);

		if (pointABIntersect.distanceTo(pointStart2) > CADConsts::dTolerance)
			vectEndLine = pointStart2 - pointABIntersect;
		else
			vectEndLine = pointEnd2 - pointABIntersect;

		double dStartAngle = vectStartLine.angleTo(OdGeVector3d::kXAxis);
		double dEndAngle = vectEndLine.angleTo(OdGeVector3d::kXAxis);
		double dAngleInside = fabs(dStartAngle - dEndAngle);//�����ü�ֱ�߼�ļн�

		if (dStartAngle > dEndAngle)
			dAngleInside = Oda2PI - dAngleInside;

		double ParameterStart = vectVertex.angleTo(vectStartLine);
		double ParameterEnd = vectVertex.angleTo(vectEndLine);

		if (ParameterStart < CADConsts::dTolerance || ParameterEnd < CADConsts::dTolerance)
			return 0;

		if (fabs(dAngleInside - (ParameterStart + ParameterEnd)) < CADConsts::dTolerance)
			return 1;
		else if (fabs(dAngleInside - (Oda2PI - ParameterStart + ParameterEnd)) < CADConsts::dTolerance 
				 || fabs(dAngleInside - (ParameterStart + Oda2PI - ParameterEnd)) < CADConsts::dTolerance)
			return 1;
		else
			return -1;
	}

	//@function: ���ܣ��ü����� I���Գ���ֱ�߲ü�   -- by mzq  2019/11/4 11:24
	void DrawAutility::clipArcOrCircle(std::list<OdDbArcPtr> &listArc,/*��Ҫ�ü���Բ��*/
									   OdGePoint3d pointBase, /*�ü�ֱ�����*/
									   double dLength, /*�ü�ֱ�߳���*/
									   bool bLeft /*�ü��ı�*/)
	{
		OdGeVector3d vecVerticalUnit = -OdGeVector3d::kYAxis;//��ֱ����
		vecVerticalUnit.normalize();

		OdDbLinePtr pLine = OdDbLine::createObject();
		pLine->setStartPoint(pointBase);
		pLine->setEndPoint(pointBase + vecVerticalUnit * dLength);


		std::list<OdDbArcPtr>::iterator citerArc = listArc.begin();
		for (citerArc; citerArc != listArc.end();)
		{
			OdDbArcPtr pCurve = *citerArc;
			if (pCurve->isA()->isEqualTo(OdDbArc::desc()))
			{
				const double dToleranceDis = CADConsts::dTolerance;

				OdGePoint3d pointCenter = pCurve->center();
				OdGePoint3dArray points;
				pCurve->intersectWith(pLine, OdDb::kOnBothOperands, points);//�ü�ֱ����Բ���ཻ�󽻵�
				int nIntersectCount = points.size();

				if (1 == nIntersectCount)//�������һ������
				{
					//OdResult result;
					OdGePoint3d startPoint;
					OdGePoint3d endPoint;
					pCurve->getStartPoint(startPoint);
					pCurve->getEndPoint(endPoint);
					if (bLeft)//���
					{
						if (startPoint.distanceTo(points.getAt(0)) < dToleranceDis)//�պòõ����
						{
							if (endPoint.x - pointBase.x < dToleranceDis)
							{
								pCurve->erase();
								listArc.erase(citerArc++);
								continue;
							}
						}
						else if (endPoint.distanceTo(points.getAt(0)) < dToleranceDis)//�պòõ��յ�
						{
							if (startPoint.x - pointBase.x < dToleranceDis)
							{
								pCurve->erase();
								listArc.erase(citerArc++);
								continue;
							}
						}
						else if (startPoint.x - pointBase.x < dToleranceDis)//�ü����м�
						{
							double dRadianIntersect = 0.0;
							pCurve->getParamAtPoint(points.at(0), dRadianIntersect);
							pCurve->setStartAngle(dRadianIntersect);
						}
						else if (endPoint.x - pointBase.x < dToleranceDis)//�ü����м�
						{
							double dRadianIntersect = 0.0;
							pCurve->getParamAtPoint(points.at(0), dRadianIntersect);
							pCurve->setEndAngle(dRadianIntersect);
						}
					}
					else
					{
						//�պòõ�����յ�
						if (startPoint.distanceTo(points.getAt(0)) < dToleranceDis)
						{
							if (endPoint.x - pointBase.x > dToleranceDis)
							{
								pCurve->erase();
								listArc.erase(citerArc++);
								continue;
							}
						}
						else if (endPoint.distanceTo(points.getAt(0)) < dToleranceDis)
						{
							if (startPoint.x - pointBase.x > dToleranceDis)
							{
								pCurve->erase();
								listArc.erase(citerArc++);
								continue;
							}
						}
						else if (startPoint.x - pointBase.x < dToleranceDis)
						{
							double dRadianIntersect = 0.0;
							pCurve->getParamAtPoint(points.at(0), dRadianIntersect);
							pCurve->setEndAngle(dRadianIntersect);
						}
						else
						{
							double dRadianIntersect = 0.0;
							pCurve->getParamAtPoint(points.at(0), dRadianIntersect);
							pCurve->setStartAngle(dRadianIntersect);
						}
					}
				}
				else if (2 == nIntersectCount) // TODOMZQ ��ʱ�������������...
				{
					pCurve->erase();
					listArc.erase(citerArc++);
					continue;
				}
				else
				{
					OdGePoint3d startPoint;
					OdGePoint3d endPoint;
					pCurve->getStartPoint(startPoint);
					pCurve->getEndPoint(endPoint);

					/*ɾ��һ���Բ��*/
					if (bLeft && startPoint.x - pointBase.x < dToleranceDis && endPoint.x - pointBase.x < dToleranceDis)
					{
						pCurve->erase();
						listArc.erase(citerArc++);
						continue;
					}
					else if (!bLeft && startPoint.x - pointBase.x > dToleranceDis && endPoint.x - pointBase.x > dToleranceDis)
					{
						pCurve->erase();
						listArc.erase(citerArc++);
						continue;
					}
				}
			}
			citerArc++;
		}
	}

	//@function: ��ȡ����ߵĵ㼯���Զ������ظ���   -- by mzq  2019/12/12 15:26
	std::list<OdGePoint3d> DrawAutility::getLinePoints(const std::list<OdDbObjectId> &listLineID/*��id list*/)
	{
		/*ɸѡ������߶εĵ�*/
		std::list<OdGePoint3d> listCenter;
		std::list<OdDbObjectId>::const_iterator iterLine = listLineID.begin();
		for (iterLine; iterLine != listLineID.end(); iterLine++)
		{
			OdDbCurvePtr curve = iterLine->safeOpenObject(OdDb::kForRead);
			OdGePoint3d pointStart, pointEnd;
			if (curve->isA()->isEqualTo(OdDbLine::desc()))
			{
				curve->getStartPoint(pointStart);
				curve->getEndPoint(pointEnd);
				listCenter.insert(listCenter.end(), pointStart);
				listCenter.insert(listCenter.end(), pointEnd);
			}
			else if (curve->isA()->isEqualTo(OdDbPolyline::desc()))
			{
				OdDbPolylinePtr pPLine = OdDbPolyline::cast(curve);
				int nVertCount = pPLine->numVerts();
				for (int nIndex = 0; nIndex < nVertCount; nIndex++)
				{
					pPLine->getPointAt(nIndex, pointStart);
					listCenter.insert(listCenter.end(), pointStart);
				}
			}
		}

		filterDupPoint(listCenter);//����ɾ�����ظ���
		return listCenter;
	}

	//@function: �����ظ���   -- by mzq  2019/12/19 16:15
	void DrawAutility::filterDupPoint(std::list<OdGePoint3d> &pointList/*�� list*/)
	{
		/*���˵��ظ���*/
		std::list<OdGePoint3d>::iterator iterPoint = pointList.begin();
		for (; iterPoint != pointList.end(); iterPoint++)
		{
			OdGePoint3d point = *iterPoint;

			std::list<OdGePoint3d>::iterator iterPointNext = iterPoint;
			iterPointNext++;
			for (iterPointNext; iterPointNext != pointList.end();)
			{
				OdGePoint3d pointNext = *iterPointNext;
				/*Բ����ͬ���뾶��ͬ����ɾ����*/
				if (pointNext.distanceTo(point) < CADConsts::dTolerance)
				{
					pointList.erase(iterPointNext++);
				}
				else
					iterPointNext++;
			}
		}
	}

	//@function: �����ظ���   -- by mzq  2019/12/20 9:20
	void DrawAutility::filterDupLine(std::list<OdDbObjectId> &lineList/*�� list*/)
	{
		std::list<OdDbObjectId>::iterator iterLine = lineList.begin();
		for (iterLine; iterLine != lineList.end(); iterLine++)//ѭ������ÿһ����
		{
			OdGePoint3d pointStart, pointEnd;
			OdDbLinePtr line = iterLine->safeOpenObject(OdDb::kForWrite);
			line->getStartPoint(pointStart);
			line->getEndPoint(pointEnd);

			std::list<OdDbObjectId>::iterator iterLineNext = iterLine;
			iterLineNext++;
			for (; iterLineNext != lineList.end();)//ѭ������һ��������֮���ÿһ���߽��бȽ�
			{
				OdGePoint3d pointStartNext, pointEndNext;
				OdDbLinePtr lineNext = iterLineNext->safeOpenObject(OdDb::kForWrite);
				lineNext->getStartPoint(pointStartNext);
				lineNext->getEndPoint(pointEndNext);

				/*�ж������ߵ������յ㣬�����ͬ����ɾ����ע�������ߵ���ʼ��ֹ��˳����ܲ�ͬ����Ҫ���бȽ�*/
				if ((pointStart.distanceTo(pointStartNext) < CADConsts::dTolerance
					&& pointEnd.distanceTo(pointEndNext) < CADConsts::dTolerance)
					|| (pointStart.distanceTo(pointEndNext) < CADConsts::dTolerance
					&& pointEnd.distanceTo(pointStartNext) < CADConsts::dTolerance))
				{
					lineNext->erase();//��ͼֽ��ɾ��
					lineList.erase(iterLineNext++);//��list��ɾ��
				}
				else
				{
					iterLineNext++;
				}
			}
		}
	}

	//@function: ���ƶ��Բ��ɵ���߽�   -- by mzq  2019/12/12 10:13
	std::list<OdDbArcPtr> DrawAutility::drawCirclesBoundary(const std::list<CirclesBoundaryInfo> &drawInfoList,/*�����Ϣ*/
															bool isHiddenLine/*�Ƿ�Ϊ����*/)
	{
		std::list<OdDbObjectId> listCircle;//���е�Բ

		std::list<CirclesBoundaryInfo>::const_iterator iter = drawInfoList.begin();
		for (; iter != drawInfoList.end(); iter++)
		{
			std::list<OdDbObjectId> tempList = CreateEnt::drawCircles(iter->_centrePoints, iter->_radius, isHiddenLine, iter->_colorIndex);
			listCircle.insert(listCircle.end(), tempList.begin(), tempList.end());
		}

		/*���Բ*/
		std::list<PointOnCircle> pointOnCircle;//Բ�Ͻ���
		DrawAutility::deleteCoincideCircle(listCircle);
		DrawAutility::getCircleIntersection(listCircle, pointOnCircle);
		std::list<ArcOnCircle> listArcOnCircle;
		DrawAutility::calcArc(pointOnCircle, listCircle, listArcOnCircle);

		std::list<OdDbArcPtr> listAllArc;//���е�Բ�������Χ��
		std::list<ArcOnCircle>::iterator iterArc = listArcOnCircle.begin();
		for (; iterArc != listArcOnCircle.end(); iterArc++)
		{
			//����Բ��
			CreateEnt::addToModelSpace(iterArc->second);
			listAllArc.insert(listAllArc.end(), iterArc->second);
		}

		/*ɸѡ����Ҫ��ɾ����Բ*/
		std::list<OdDbObjectId> listCircleDel;//��Ҫ��ɾ����Բ
		std::list<PointOnCircle>::iterator iterCircleDel = pointOnCircle.begin();
		for (; iterCircleDel != pointOnCircle.end(); iterCircleDel++)
		{
			std::list<OdDbObjectId>::iterator iterFind =
				std::find(listCircleDel.begin(), listCircleDel.end(), iterCircleDel->first);
			if (iterFind == listCircleDel.end())
			{
				listCircleDel.insert(listCircleDel.end(), iterCircleDel->first);
			}
		}
		
		/*ɾ��*/
		std::list<OdDbObjectId>::iterator iterEraseCircleID = listCircleDel.begin();
		for (; iterEraseCircleID != listCircleDel.end(); iterEraseCircleID++)
		{
			OdDbCirclePtr pCircleEnt = iterEraseCircleID->safeOpenObject(OdDb::kForWrite);
			pCircleEnt->erase();
		}

		return listAllArc;
	}

	////@function: ���ƶ��Բ��ɵ���߽�   -- by mzq  2019/12/12 10:13
	//std::list<OdDbArcPtr> DrawAutility::drawCirclesBoundary(std::list<OdDbArcPtr> listAllArc/*���е�Բ�������Χ��*/)
	//{
	//	std::list<ArcOnCircle>::iterator iterArc = listArcOnCircle.begin();
	//	for (; iterArc != listArcOnCircle.end(); iterArc++)
	//	{
	//		//����Բ��
	//		CreateEnt::addToModelSpace(iterArc->second);
	//		listAllArc.insert(listAllArc.end(), iterArc->second);
	//	}

	//	/*ɸѡ����Ҫ��ɾ����Բ*/
	//	std::list<OdDbObjectId> listCircleDel;//��Ҫ��ɾ����Բ
	//	std::list<PointOnCircle>::iterator iterCircleDel = pointOnCircle.begin();
	//	for (; iterCircleDel != pointOnCircle.end(); iterCircleDel++)
	//	{
	//		std::list<OdDbObjectId>::iterator iterFind =
	//			std::find(listCircleDel.begin(), listCircleDel.end(), iterCircleDel->first);
	//		if (iterFind == listCircleDel.end())
	//		{
	//			listCircleDel.insert(listCircleDel.end(), iterCircleDel->first);
	//		}
	//	}

	//	/*ɾ��*/
	//	std::list<OdDbObjectId>::iterator iterEraseCircleID = listCircleDel.begin();
	//	for (; iterEraseCircleID != listCircleDel.end(); iterEraseCircleID++)
	//	{
	//		OdDbCirclePtr pCircleEnt = iterEraseCircleID->safeOpenObject(OdDb::kForWrite);
	//		pCircleEnt->erase();
	//	}

	//	return listAllArc;
	//}

	//@function: ɾ���ظ���Բ   -- by mzq  2019/11/4 11:24
	void DrawAutility::deleteCoincideCircle(std::list<OdDbObjectId>& listCircle/*Բlist*/)
	{
		std::list<OdDbObjectId>::iterator iterCircle = listCircle.begin();
		for (; iterCircle != listCircle.end(); iterCircle++)
		{
			OdDbCirclePtr circle = iterCircle->safeOpenObject(OdDb::kForRead);
			OdGePoint3d pointCenter = circle->center();
			double dRadius = circle->radius();

			std::list<OdDbObjectId>::iterator iterCircleNext = iterCircle;
			iterCircleNext++;
			for (iterCircleNext; iterCircleNext != listCircle.end();)
			{
				OdDbCirclePtr circleNext = iterCircleNext->safeOpenObject(OdDb::kForWrite);
				OdGePoint3d pointCenterNext = circleNext->center();
				double dRadiusNext = circleNext->radius();

				/*Բ����ͬ���뾶��ͬ����ɾ����*/
				if (fabs(dRadius - dRadiusNext) < CADConsts::dTolerance
					&& pointCenter.distanceTo(pointCenterNext) < CADConsts::dTolerance)
				{
					circleNext->erase();
					listCircle.erase(iterCircleNext++);
				}
				else
					iterCircleNext++;
			}
		}
	}

	//@function: ���Ʒ��ѵ��ߵ�ʵ��   -- by mzq  2019/11/4 11:24
	std::list<OdDbObjectId> DrawAutility::drawSplitEntity(const std::list<OdGePoint3d> &listPoint3d /*���Ѷ˵�����*/)
	{
		std::list<OdDbObjectId> listObjectID;
		std::list<OdGePoint3d>::const_iterator iterPoint = listPoint3d.begin();
		std::list<OdGePoint3d>::const_iterator iterPointNext = iterPoint;
		iterPointNext++;
		if (iterPointNext == listPoint3d.end())
		{
			return listObjectID;
		}
		for (iterPoint; iterPoint != listPoint3d.end(); iterPoint++, iterPointNext++)
		{
			if (iterPointNext == listPoint3d.end())
			{
				iterPointNext = listPoint3d.begin();
			}
			listObjectID.insert(listObjectID.end(), CreateEnt::drawLine(*iterPoint, *iterPointNext));
		}

		return listObjectID;
	}

	//@function: ����������Ķ˵�����   -- by mzq  2019/11/4 11:24
	std::list<OdGePoint3d> DrawAutility::creatNumSplitEntity(OdGePoint3d pointHook,/*�ҵ�����*/
															 double spaceSplit,/*���Ѽ��*/
															 double heightSplit,/*���Ѹ߶ȣ�����Է�����Ϊ3�Ҳ��ǵȱ������ε�ʱ������*/
															 SplitNum::Type splitType/*������*/)
	{
		OdGeVector3d vecHorizontalUnit = OdGeVector3d::kXAxis;//ˮƽ����
		OdGeVector3d vecVerticalUnit = -OdGeVector3d::kYAxis;//��ֱ����
		vecHorizontalUnit.normalize();
		vecVerticalUnit.normalize();

		//�ҽӵ�����ֱ�ߵ����յ�
		OdGePoint3d pointStart_HookLine = pointHook - vecHorizontalUnit*fabs(spaceSplit / 2.0);
		OdGePoint3d pointEnd_HookLine = pointStart_HookLine + vecHorizontalUnit*fabs(spaceSplit);

		std::list<OdGePoint3d> listPoint;
		switch (splitType)
		{
		case SplitNum::One:
			listPoint.insert(listPoint.end(), pointHook);
			break;
		case SplitNum::TwoV:
		{
			listPoint.insert(listPoint.end(), pointHook);
			OdGePoint3d pointSec = pointHook + vecVerticalUnit * spaceSplit;
			listPoint.insert(listPoint.end(), pointSec);
			break;
		}
		case SplitNum::TwoH:
		case SplitNum::Four:
		case SplitNum::Six:
		case SplitNum::Eight:
		case SplitNum::Ten:
		{
			OdGePoint3d pointStart = pointStart_HookLine;
			OdGePoint3d pointEnd = pointEnd_HookLine;
			listPoint.insert(listPoint.end(), pointStart);
			listPoint.insert(listPoint.end(), pointEnd);
			int nSplitNum = SplitNum::typeToSplitNum(splitType);
			double dRotate = CADConsts::PI*(nSplitNum - 2) / nSplitNum;//��ת�Ƕ�Ϊ(����)��PI(N-2)/N ������ѧ������
			for (int index = 0; index < nSplitNum - 2; index++)
			{
				OdGePoint3d pointNext = pointStart;
				OdGeVector3d vecRotate(0, 0, 1);
				pointNext.rotateBy(dRotate, vecRotate, pointEnd);
				listPoint.insert(listPoint.end(), pointNext);
				pointStart = pointEnd;
				pointEnd = pointNext;
			}
			break;
		}
		case SplitNum::Three:
		{
			OdGePoint3d point1 = pointStart_HookLine;
			OdGePoint3d point2 = pointEnd_HookLine;

			OdGePoint3d point3;
			if (Auxiliary::isEqual(heightSplit, 0))//�ȱ�������
				point3 = pointHook + vecVerticalUnit * ((spaceSplit * 0.5) / tan(SmartCl::Auxiliary::angle_to_radian(30)));
			else//����������
				point3 = pointHook + vecVerticalUnit * heightSplit;

			listPoint.insert(listPoint.end(), point1);
			listPoint.insert(listPoint.end(), point2);
			listPoint.insert(listPoint.end(), point3);
		}
		}
		return  listPoint;
	}

	//@function: ���Ʒŵ��ʾ��㣬��С�뾶��Բ��ʾ   -- by mzq 2020/11/24 17:58
	void DrawAutility::drawElecPointCircle(std::list<CirclesBoundaryInfo> drawInfoList,/*�ŵ�㼯*/
										   double radius/*Բ�뾶*/)
	{
		std::list<CirclesBoundaryInfo>::iterator iter = drawInfoList.begin();
		for (; iter != drawInfoList.end(); iter++)
		{
			iter->_radius = radius;
			iter->_colorIndex = 1;//����ɫ����Ϊ��ɫ
		}

		DrawAutility::drawCirclesBoundary(drawInfoList);
	}

	//@function: ����ֱ����I���߼��Լ�С����   -- by mzq  2019/12/6 15:01
	std::list<OdDbObjectId> DrawAutility::draw_I_L5L6(double dL5,/*L5���ȣ��߼г���*/
												      double dL6,/*L6���ȣ�С����*/
												      const std::list<OdGePoint3d> &listPoint3d/*��ʼ�ο���*/)
	{
		std::list<OdDbObjectId> listObjectID;

		OdGeVector3d vecVerticalUnit = OdGeVector3d::kYAxis;//��ֱ����
		vecVerticalUnit.normalize();

		//�ҽӵ�����ֱ�ߵ����յ�
		OdGePoint3d pointStart_HookLine;
		OdGePoint3d pointEnd_HookLine;

		std::list<OdGePoint3d>::const_iterator iterPoint = listPoint3d.begin();
		for (iterPoint; iterPoint != listPoint3d.end(); iterPoint++)
		{
			pointStart_HookLine = *iterPoint;
			pointEnd_HookLine = pointStart_HookLine - vecVerticalUnit * dL5;
			OdDbObjectId LineL5ID = CreateEnt::drawLine(pointStart_HookLine, pointEnd_HookLine);

			pointStart_HookLine = pointEnd_HookLine;
			pointEnd_HookLine = pointStart_HookLine - vecVerticalUnit * dL6;
			OdDbObjectId LineL6ID = CreateEnt::drawLine(pointStart_HookLine, pointEnd_HookLine);

			listObjectID.insert(listObjectID.end(), LineL5ID);
			listObjectID.insert(listObjectID.end(), LineL6ID);
		}
		return listObjectID;
	}

	//@function: ���ܣ�����I���߼м�С�������֣����ط��������ε��߶�ID   -- by mzq 2020/11/25 9:47
	std::list<OdDbObjectId> DrawAutility::draw_I_L5L6Theta(OdGePoint3d pointHook, /*���ѽ�߹ҵ�*/ 
														   double dL5, /*�߼г���*/ 
														   double dL6, /*С��������*/ 
														   double dAngle, /*С����ƫ��*/ 
														   const std::list<OdGePoint3d> &listPoint3d, /*�������˵�*/ 
														   bool isDrawLine /*= true/*�Ƿ���ƻ����߶Σ�Ĭ��Ϊ����*/)
	{
		std::list<OdDbObjectId> listObjectID;

		OdGeVector3d vecVerticalUnit = OdGeVector3d::kYAxis;//��ֱ����
		vecVerticalUnit.normalize();

		//�ҽӵ�����ֱ�ߵ����յ�
		OdGePoint3d pointStart_HookLine;
		OdGePoint3d pointMid_HookLine;
		OdGePoint3d pointEnd_HookLine;
		OdGePoint3d pointEnd3;
		std::list<OdGePoint3d>::const_iterator iterPoint = listPoint3d.begin();
		for (iterPoint; iterPoint != listPoint3d.end(); iterPoint++)
		{
			pointStart_HookLine = *iterPoint;
			pointMid_HookLine = pointStart_HookLine - vecVerticalUnit * dL5;
			pointEnd_HookLine = pointMid_HookLine - vecVerticalUnit * dL6;

			OdDbPolylinePtr pNewLineL5 = OdDbPolyline::createObject();
			pNewLineL5->addVertexAt(0, pointStart_HookLine.convert2d());
			pNewLineL5->addVertexAt(1, pointMid_HookLine.convert2d());
			pNewLineL5->addVertexAt(2, pointEnd_HookLine.convert2d());

			double dLengthBottom = (dL5 + dL6) * tan(dAngle);
			pointEnd3 = pointEnd_HookLine;
			if (fabs(pointHook.x - pointStart_HookLine.x) <= CADConsts::dTolerance)
			{
				OdDbPolylinePtr pNewLineLeft = OdDbPolyline::createObject();
				pNewLineLeft->addVertexAt(0, pointStart_HookLine.convert2d());
				pNewLineLeft->addVertexAt(1, pointMid_HookLine.convert2d());
				pNewLineLeft->addVertexAt(2, pointEnd_HookLine.convert2d());
				pointEnd3.x -= dLengthBottom;
				pNewLineLeft->addVertexAt(3, pointEnd3.convert2d());
				pNewLineLeft->setClosed(true);
				if (isDrawLine)
					listObjectID.insert(listObjectID.end(), CreateEnt::addToModelSpace(pNewLineLeft));

				pointEnd3.x += dLengthBottom;
			}
			if (pointHook.x - pointStart_HookLine.x > CADConsts::dTolerance)
				pointEnd3.x -= dLengthBottom;
			else
				pointEnd3.x += dLengthBottom;

			pNewLineL5->addVertexAt(3, pointEnd3.convert2d());
			pNewLineL5->setClosed(true);
			if (isDrawLine)
				listObjectID.insert(listObjectID.end(), CreateEnt::addToModelSpace(pNewLineL5));
		}

		return listObjectID;
	}

	//@function: ����V���߼м�С�������֣����ط��������ε��·��˵�����   -- by mzq  2020/1/2 18:22
	std::list<OdGePoint3d> DrawAutility::draw_VL_L6L7(OdGePoint3d pointHook, /*���ѽ�߹ҵ�*/
													  double dL6, /*�߼г���*/
													  double dL7, /*С��������*/
													  double dAngle, /*С����ƫ��*/
													  const std::list<OdGePoint3d> &listPoint3d, /*�������˵�*/
													  bool isDrawLine,/*�Ƿ���ƻ����߶Σ�Ĭ��Ϊ����*/
													  bool isRightTriangle/*ֱ�������Σ�true�����������Σ�false*/)
	{
		OdGeVector3d vecVerticalUnit = OdGeVector3d::kYAxis;//��ֱ����
		vecVerticalUnit.normalize();

		//�ҽӵ�����ֱ�ߵ����յ�
		OdGePoint3d pointStart_HookLine;
		OdGePoint3d pointMid_HookLine;
		OdGePoint3d pointEnd_HookLine;
		OdGePoint3d pointEnd3;
		std::list<OdGePoint3d> listAllPoint;
		std::list<OdGePoint3d>::const_iterator iterPoint = listPoint3d.begin();
		for (iterPoint; iterPoint != listPoint3d.end(); iterPoint++)
		{
			pointStart_HookLine = *iterPoint;
			pointMid_HookLine = pointStart_HookLine - vecVerticalUnit * dL6;
			pointEnd_HookLine = pointMid_HookLine - vecVerticalUnit * dL7;

			OdDbPolylinePtr pNewLineL5 = OdDbPolyline::createObject();
			pNewLineL5->addVertexAt(0, pointStart_HookLine.convert2d());
			pNewLineL5->addVertexAt(1, pointMid_HookLine.convert2d());
			pNewLineL5->addVertexAt(2, pointEnd_HookLine.convert2d());

			double shift_X = 0.0;
			if (isRightTriangle)
				shift_X = (dL6 + dL7) * tan(dAngle);
			else
				shift_X = (dL6 + dL7) * sin(dAngle);
			double shift_Y = 0.0;
			if (isRightTriangle)
				shift_Y = 0.0;
			else
				shift_Y = (dL6 + dL7) * (1.0 - cos(dAngle));

			pointEnd3 = pointEnd_HookLine;
			if (fabs(pointHook.x - pointStart_HookLine.x) <= CADConsts::dTolerance)
			{
				OdDbPolylinePtr pNewLineLeft = OdDbPolyline::createObject();
				pNewLineLeft->addVertexAt(0, pointStart_HookLine.convert2d());
				pNewLineLeft->addVertexAt(1, pointMid_HookLine.convert2d());
				pNewLineLeft->addVertexAt(2, pointEnd_HookLine.convert2d());
				pointEnd3.x -= shift_X;
				pointEnd3.y += shift_Y;
				pNewLineLeft->addVertexAt(3, pointEnd3.convert2d());
				pNewLineLeft->setClosed(true);
				if (isDrawLine)
					CreateEnt::addToModelSpace(pNewLineLeft);
				listAllPoint.insert(listAllPoint.end(), pointEnd3);

				pointEnd3.x += shift_X;
				pointEnd3.y -= shift_Y;
			}
			if (pointHook.x - pointStart_HookLine.x > CADConsts::dTolerance)
				pointEnd3.x -= shift_X;
			else
				pointEnd3.x += shift_X;

			pointEnd3.y += shift_Y;
			pNewLineL5->addVertexAt(3, pointEnd3.convert2d());
			pNewLineL5->setClosed(true);
			if (isDrawLine)
				CreateEnt::addToModelSpace(pNewLineL5);
			listAllPoint.insert(listAllPoint.end(), pointStart_HookLine);
			listAllPoint.insert(listAllPoint.end(), pointMid_HookLine);
			listAllPoint.insert(listAllPoint.end(), pointEnd_HookLine);
			listAllPoint.insert(listAllPoint.end(), pointEnd3);
		}

		return listAllPoint;
	}

	//@function: ���ܣ�����V��Բ��   -- by mzq  2019/12/6 15:01
	std::list<OdDbArcPtr> DrawAutility::drawArc_V(const std::list<OdGePoint3d> &listArcCenter,/*����ԭ��list*/
												  double dRadius, /*�뾶*/
												  int color,/*����ɫ*/
												  OdDbLinePtr pLineL1,/*V�ǣ��߶�1*/
												  OdDbLinePtr pLineL2 /*V�ǣ��߶�2*/)
	{
		std::list<CirclesBoundaryInfo> drawInfoList;
		CirclesBoundaryInfo circlesInfo(listArcCenter, dRadius, color);//��װ���ư���Բ��ص���Ϣ
		drawInfoList.insert(drawInfoList.end(), circlesInfo);//���뵽��϶Բ��Ϣ������
		std::list<OdDbArcPtr> listAllArc = DrawAutility::drawCirclesBoundary(drawInfoList);//���ݻ��ư���Բ��Ϣ���������ư���Բ

		//����Բ�Ľǲ���
		OdResult odRes;
		std::list<OdDbArcPtr>::iterator iterCutArc = listAllArc.begin();//���е�Բ�������Χ��
		for (iterCutArc; iterCutArc != listAllArc.end();)
		{
			OdDbArcPtr pCircleEnt = *iterCutArc;
			double dArcStartAngle;
			double dArcEndAngle;
			odRes = pCircleEnt->getStartParam(dArcStartAngle);
			odRes = pCircleEnt->getEndParam(dArcEndAngle);
			OdGePoint3d pointArcStart;
			OdGePoint3d pointArcEnd;
			pCircleEnt->getStartPoint(pointArcStart);
			pCircleEnt->getEndPoint(pointArcEnd);

			int bInside_ArcStart = DrawAutility::isAngleInside(pLineL1, pLineL2, pointArcStart);
			int bInside_ArcEnd = DrawAutility::isAngleInside(pLineL1, pLineL2, pointArcEnd);
			OdGePoint3d pointBase = pCircleEnt->center();
			//����Բ��������յ�ֱ�� 
			OdDbLinePtr pLineArcStart = OdDbLine::createObject();
			pLineArcStart->setStartPoint(pointBase);
			pLineArcStart->setEndPoint(pointArcStart);
			OdDbLinePtr pLineArcEnd = OdDbLine::createObject();
			pLineArcEnd->setStartPoint(pointBase);
			pLineArcEnd->setEndPoint(pointArcEnd);

			//Բ���������ȵĽ���
			OdGePoint3dArray pointsL1a;
			OdGePoint3dArray pointsL1b;
			odRes = pCircleEnt->intersectWith(pLineL2, OdDb::kOnBothOperands, pointsL1a);
			odRes = pCircleEnt->intersectWith(pLineL1, OdDb::kOnBothOperands, pointsL1b);
			double angleIntersectL1a = 0.0;
			double angleIntersectL1b = 0.0;

			if (pointsL1a.size() == 1)
				pCircleEnt->getParamAtPoint(pointsL1a.at(0), angleIntersectL1a);
			if (pointsL1b.size() == 1)
				pCircleEnt->getParamAtPoint(pointsL1b.at(0), angleIntersectL1b);

			if (bInside_ArcStart == -1 && bInside_ArcEnd == -1 && pointsL1a.size() == 0 && pointsL1b.size() == 0)
			{
				bInside_ArcStart = DrawAutility::isAngleInside(pLineL1, pLineL2, pointArcStart);
				bInside_ArcEnd = DrawAutility::isAngleInside(pLineL1, pLineL2, pointArcEnd);

				pCircleEnt->erase();
				listAllArc.erase(iterCutArc++);
				continue;
			}

			if (bInside_ArcStart == -1)
			{
				if (pointsL1a.size() == 1)
					pCircleEnt->setStartAngle(angleIntersectL1a);
				else if (pointsL1b.size() == 1)
					pCircleEnt->setStartAngle(angleIntersectL1b);
			}
			else if (bInside_ArcEnd == -1)
			{
				if (pointsL1a.size() == 1)
					pCircleEnt->setEndAngle(angleIntersectL1a);
				else if (pointsL1b.size() == 1)
					pCircleEnt->setEndAngle(angleIntersectL1b);
			}

			iterCutArc++;
		}
		return listAllArc;
	}

	//@function: ���ܣ�����V��Բ��   -- by mzq 2021/2/26 15:31
	std::list<OdDbArcPtr> DrawAutility::drawArc_V(const std::list<CirclesBoundaryInfo>& drawInfoList,/*�����Ϣ*/
												  OdDbLinePtr pLineL1,/*V�ǣ��߶�1*/ 
												  OdDbLinePtr pLineL2 /*V�ǣ��߶�2*/)
	{
		std::list<OdDbArcPtr> listAllArc = DrawAutility::drawCirclesBoundary(drawInfoList);//���ݻ��ư���Բ��Ϣ���������ư���Բ

		//����Բ�Ľǲ���
		OdResult odRes;
		std::list<OdDbArcPtr>::iterator iterCutArc = listAllArc.begin();//���е�Բ�������Χ��
		for (iterCutArc; iterCutArc != listAllArc.end();)
		{
			OdDbArcPtr pCircleEnt = *iterCutArc;
			//double dArcStartAngle;
			//double dArcEndAngle;
			//odRes = pCircleEnt->getStartParam(dArcStartAngle);
			//odRes = pCircleEnt->getEndParam(dArcEndAngle);
			OdGePoint3d pointArcStart;
			OdGePoint3d pointArcEnd;
			pCircleEnt->getStartPoint(pointArcStart);
			pCircleEnt->getEndPoint(pointArcEnd);

			int bInside_ArcStart = DrawAutility::isAngleInside(pLineL1, pLineL2, pointArcStart);
			int bInside_ArcEnd = DrawAutility::isAngleInside(pLineL1, pLineL2, pointArcEnd);
			OdGePoint3d pointBase = pCircleEnt->center();
			//����Բ��������յ�ֱ�� 
			OdDbLinePtr pLineArcStart = OdDbLine::createObject();
			pLineArcStart->setStartPoint(pointBase);
			pLineArcStart->setEndPoint(pointArcStart);
			OdDbLinePtr pLineArcEnd = OdDbLine::createObject();
			pLineArcEnd->setStartPoint(pointBase);
			pLineArcEnd->setEndPoint(pointArcEnd);

			//Բ���������ȵĽ���
			OdGePoint3dArray pointsL1a;
			OdGePoint3dArray pointsL1b;
			odRes = pCircleEnt->intersectWith(pLineL2, OdDb::kOnBothOperands, pointsL1a);
			odRes = pCircleEnt->intersectWith(pLineL1, OdDb::kOnBothOperands, pointsL1b);
			double angleIntersectL1a = 0.0;
			double angleIntersectL1b = 0.0;

			if (pointsL1a.size() == 1)
				pCircleEnt->getParamAtPoint(pointsL1a.at(0), angleIntersectL1a);
			if (pointsL1b.size() == 1)
				pCircleEnt->getParamAtPoint(pointsL1b.at(0), angleIntersectL1b);

			if (bInside_ArcStart == -1 && bInside_ArcEnd == -1 && pointsL1a.size() == 0 && pointsL1b.size() == 0)
			{
				bInside_ArcStart = DrawAutility::isAngleInside(pLineL1, pLineL2, pointArcStart);
				bInside_ArcEnd = DrawAutility::isAngleInside(pLineL1, pLineL2, pointArcEnd);

				pCircleEnt->erase();
				listAllArc.erase(iterCutArc++);
				continue;
			}

			if (bInside_ArcStart == -1)
			{
				if (pointsL1a.size() == 1)
					pCircleEnt->setStartAngle(angleIntersectL1a);
				else if (pointsL1b.size() == 1)
					pCircleEnt->setStartAngle(angleIntersectL1b);
			}
			else if (bInside_ArcEnd == -1)
			{
				if (pointsL1a.size() == 1)
					pCircleEnt->setEndAngle(angleIntersectL1a);
				else if (pointsL1b.size() == 1)
					pCircleEnt->setEndAngle(angleIntersectL1b);
			}

			iterCutArc++;
		}
		return listAllArc;
	}

	//@function: ��������ֱ�ߵļн�   -- by mzq  2020/1/4 11:12
	double DrawAutility::getTwoLineAngle(OdDbLinePtr pLine1,/*��1*/ 
										 OdDbLinePtr pLine2/*��2*/)
	{
		OdGePoint3d line1Start, line1End, line2Start, line2End;
		pLine1->getStartPoint(line1Start);
		pLine1->getEndPoint(line1End);
		pLine2->getStartPoint(line2Start);
		pLine2->getEndPoint(line2End);

		OdGeVector3d vecLine1(line1End - line1Start);
		OdGeVector3d vecLine2(line2End - line2Start);

		return OdaToDegree(vecLine1.angleTo(vecLine2));
	}

	//@function: ��Բ�ĺͰ뾶Ϊ����������Բ���ϵĵ�Ч8����   -- by mzq 2022/12/3 14:10
	std::list<OdGePoint3d> DrawAutility::getPointOnCircle(OdGePoint3d centerPoint/*Բ�ĵ�*/, 
														  double radius/*�뾶*/)
	{
		std::list<OdGePoint3d> listPoint;

		OdGePoint3d pointStart = centerPoint;
		pointStart.y += radius;
		OdGeVector3d vecRotate(0, 0, 1);
		for (int index = 0; index < 8; index++)
		{
			double dRotate = CADConsts::PI * 2.0 * index / 8;//��ת�Ƕ�Ϊ(����)
			OdGePoint3d point = pointStart;
			point.rotateBy(dRotate, vecRotate, centerPoint);
			listPoint.insert(listPoint.end(), point);
		}
		return listPoint;
	}
}