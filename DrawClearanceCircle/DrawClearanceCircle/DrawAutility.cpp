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

	//@function: 根据工况获取线条颜色   -- by mzq  2019/7/12 19:45
	int LineColor::LineColorIndex(LineColor::Type type/*类别*/)
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

	//@function: 根据间隙圆名称获取对应间隙圆   -- by mzq  2019/7/12 19:45
	HdlCl DrawAutility::getCl(QString clName/*间隙圆名称*/)
	{
		DataBase *database = static_cast<OdqMainWindow*>(getIApp()->getMainWindow())->getCoreDbPt();
		HdlCl curCl = database->getClByName(clName.toLocal8Bit().toStdString());//通过间隙圆名称获取
		return curCl;
	}

	//@function: 获取圆的交点   -- by mzq  2019/11/4 11:24
	void DrawAutility::getCircleIntersection(std::list<OdDbObjectId> listCircle,/*圆list*/
											 std::list<PointOnCircle> &listPoint/*圆id和圆与其他所有圆交点的映射 list*/)
	{
		std::list<OdDbObjectId>::iterator iterCircleID = listCircle.begin();
		for (; iterCircleID != listCircle.end(); iterCircleID++)
		{
			OdDbCirclePtr pCircleEnt = iterCircleID->safeOpenObject(OdDb::kForRead);//当前圆
			OdGePoint3d centerPoint = pCircleEnt->center();
			OdGePoint3dArray intersectPoints;//圆上所有的交点

			/*找和所有圆的交点*/
			std::list<OdDbObjectId>::iterator iterCircleIDNext = listCircle.begin();
			for (; iterCircleIDNext != listCircle.end(); iterCircleIDNext++)
			{
				if (*iterCircleIDNext == *iterCircleID)//自己不与自己求交点，跳出
					continue;

				OdDbCirclePtr pCircleEntNext = iterCircleIDNext->safeOpenObject(OdDb::kForRead);
				OdGePoint3dArray points;
				pCircleEnt->intersectWith(pCircleEntNext, OdDb::kOnBothOperands, points);//计算相交点
				int nPointSzie = points.size();

				if (nPointSzie != 0)//如果有交点，则记录下
					intersectPoints.insert(intersectPoints.end(), points.begin(), points.end());
			}

			if (intersectPoints.size() == 0)//如果当前圆与其他圆都无交点，跳出
				continue;

			/*圆上的交点依次排序，按照以X轴为起始位置，逆时针方向排序*/
			for (int iPoint = 0; iPoint < intersectPoints.size(); iPoint++)
			{
				double iRadians = 0.0;
				OdGePoint3d pointI = intersectPoints[iPoint];
				pCircleEnt->getParamAtPoint(pointI, iRadians);//计算点在圆上以x轴逆时针旋转的角度

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

	//@function: 创建圆弧删除被包围在内的圆弧，删除重复圆弧   -- by mzq  2019/11/4 11:24
	void DrawAutility::calcArc(const std::list<PointOnCircle>& pointOnCircle,/*圆id和圆与其他所有圆交点的映射 list*/
							   const std::list<OdDbObjectId>& listCircle,/*圆list，需要删除重复圆*/
							   std::list<ArcOnCircle> & listArcOnCircle/*圆id和一个在这个圆上的圆弧的映射 list*/)
	{
		/*第一步，根据交点将圆打断拆分成圆弧*/
		std::list<PointOnCircle>::const_iterator iterCircle = pointOnCircle.begin();
		for (iterCircle; iterCircle != pointOnCircle.end(); iterCircle++)
		{
			OdDbCirclePtr pCircleEnt = iterCircle->first.safeOpenObject(OdDb::kForRead);//圆
			OdGePoint3d centerPoint = pCircleEnt->center();
			OdUInt16 nColorIndex = pCircleEnt->colorIndex();
			double dRadius = pCircleEnt->radius();

			OdGePoint3dArray pointsOnCircle = iterCircle->second;//圆上的交点

			/*根据圆上的点将圆分割，创建圆弧*/
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

				ArcOnCircle arcOnCircle = std::make_pair(iterCircle->first, pArc);//圆id和一个在这个圆上的的映射
				listArcOnCircle.insert(listArcOnCircle.end(), arcOnCircle);
			}
		}

		/*第二步，循环遍历所有圆弧，如果在圆的内部，则删除掉该圆弧*/
		std::list<ArcOnCircle>::iterator iterArc = listArcOnCircle.begin();
		for (; iterArc != listArcOnCircle.end();)
		{
			OdGePoint3d pointStartArc;//圆弧起点
			OdGePoint3d pointEndArc;//圆弧终点
			OdGePoint3d pointCenterArc;//圆弧中点

			/*第一步：获取当前圆弧的起点，终点，中心点*/
			iterArc->second->getStartPoint(pointStartArc);
			iterArc->second->getEndPoint(pointEndArc);
			double dArcStartAngle = iterArc->second->startAngle();
			double dArcEndAngle = iterArc->second->endAngle();
			double dAngle;
			if (dArcEndAngle < dArcStartAngle)
				dAngle = (dArcStartAngle + dArcEndAngle + Oda2PI) / 2.0;
			else
				dAngle = (dArcStartAngle + dArcEndAngle) / 2.0;
			iterArc->second->getPointAtParam(dAngle, pointCenterArc);//根据圆弧中心角度获取圆弧上的点

			/*第二步：循环遍历所有圆，判断当前圆弧是否在任意一个圆的内部，如果在则删除掉*/
			bool bContain = false;
			std::list<OdDbObjectId>::const_iterator iterCircle = listCircle.begin();
			for (; iterCircle != listCircle.end(); iterCircle++)
			{
				if (iterArc->first == *iterCircle)//自己不与自己比较
					continue;

				/*判断一个圆弧是否在任意一个圆的内部*/
				OdDbCirclePtr pCircleEnt = iterCircle->safeOpenObject(OdDb::kForRead);
				OdGePoint3d pointCircle = pCircleEnt->center();
				double dRadiusCircle = pCircleEnt->radius();
				if ((pointCircle.distanceTo(pointCenterArc) - dRadiusCircle) <  CADConsts::dTolerance)
				{
					bContain = true;
					break;
				}
			}

			/*第三步：如果当前圆弧在任意一个圆内部，则删除掉该圆弧，并继续下一循环*/
			if (bContain)
				listArcOnCircle.erase(iterArc++);
			else
				iterArc++;
		}
	}

	//@function: 筛选需要标注的圆弧，并进行标注   -- by mzq  2019/11/4 11:24
	void DrawAutility::dimArcs(std::list<OdDbArcPtr> listArc,/*需要标注的圆弧*/
							   OdDbObjectId g_idDimStyle,/*标注风格*/
							   double dimScale/*标注尺寸缩放*/)
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
					{//找到就替换				
						iterPairArc->second = pArc;
						break;
					}
				}
				//没找到就加入
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

	//@function: 功能：判断点是否在另一个角度范围内部   -- by mzq  2019/11/4 11:24
	int DrawAutility::isAngleInside(OdDbLinePtr pLineStart, /*一个角度的起点直线*/
									OdDbLinePtr pLineEnd, /*一个角度的终点直线*/
									OdGePoint3d pointVertex/*被判断的点*/)
	{
		OdGePoint3dArray points;
		OdGePoint3d pointABIntersect;
		OdResult rt = pLineStart->intersectWith(pLineEnd, OdDb::kExtendBoth, points);
		if (rt != eOk && points.size() != 1)
			return -2;

		pointABIntersect = points.getAt(0);

		OdGeVector3d vectStartLine;//起始直线的向量
		OdGeVector3d vectEndLine;//终点直线的向量
		OdGeVector3d vectVertex; //被判断点的向量
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
		double dAngleInside = fabs(dStartAngle - dEndAngle);//两根裁剪直线间的夹角

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

	//@function: 功能：裁剪函数 I串对称轴直线裁剪   -- by mzq  2019/11/4 11:24
	void DrawAutility::clipArcOrCircle(std::list<OdDbArcPtr> &listArc,/*需要裁剪的圆弧*/
									   OdGePoint3d pointBase, /*裁剪直线起点*/
									   double dLength, /*裁剪直线长度*/
									   bool bLeft /*裁剪哪边*/)
	{
		OdGeVector3d vecVerticalUnit = -OdGeVector3d::kYAxis;//垂直向量
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
				pCurve->intersectWith(pLine, OdDb::kOnBothOperands, points);//裁剪直线与圆弧相交求交点
				int nIntersectCount = points.size();

				if (1 == nIntersectCount)//如果仅有一个交点
				{
					//OdResult result;
					OdGePoint3d startPoint;
					OdGePoint3d endPoint;
					pCurve->getStartPoint(startPoint);
					pCurve->getEndPoint(endPoint);
					if (bLeft)//左侧
					{
						if (startPoint.distanceTo(points.getAt(0)) < dToleranceDis)//刚好裁到起点
						{
							if (endPoint.x - pointBase.x < dToleranceDis)
							{
								pCurve->erase();
								listArc.erase(citerArc++);
								continue;
							}
						}
						else if (endPoint.distanceTo(points.getAt(0)) < dToleranceDis)//刚好裁到终点
						{
							if (startPoint.x - pointBase.x < dToleranceDis)
							{
								pCurve->erase();
								listArc.erase(citerArc++);
								continue;
							}
						}
						else if (startPoint.x - pointBase.x < dToleranceDis)//裁剪到中间
						{
							double dRadianIntersect = 0.0;
							pCurve->getParamAtPoint(points.at(0), dRadianIntersect);
							pCurve->setStartAngle(dRadianIntersect);
						}
						else if (endPoint.x - pointBase.x < dToleranceDis)//裁剪到中间
						{
							double dRadianIntersect = 0.0;
							pCurve->getParamAtPoint(points.at(0), dRadianIntersect);
							pCurve->setEndAngle(dRadianIntersect);
						}
					}
					else
					{
						//刚好裁到起点终点
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
				else if (2 == nIntersectCount) // TODOMZQ 暂时遇不到此种情况...
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

					/*删掉一侧的圆弧*/
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

	//@function: 获取组成线的点集，自动过滤重复点   -- by mzq  2019/12/12 15:26
	std::list<OdGePoint3d> DrawAutility::getLinePoints(const std::list<OdDbObjectId> &listLineID/*线id list*/)
	{
		/*筛选处组成线段的点*/
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

		filterDupPoint(listCenter);//过滤删除掉重复点
		return listCenter;
	}

	//@function: 过滤重复点   -- by mzq  2019/12/19 16:15
	void DrawAutility::filterDupPoint(std::list<OdGePoint3d> &pointList/*点 list*/)
	{
		/*过滤掉重复点*/
		std::list<OdGePoint3d>::iterator iterPoint = pointList.begin();
		for (; iterPoint != pointList.end(); iterPoint++)
		{
			OdGePoint3d point = *iterPoint;

			std::list<OdGePoint3d>::iterator iterPointNext = iterPoint;
			iterPointNext++;
			for (iterPointNext; iterPointNext != pointList.end();)
			{
				OdGePoint3d pointNext = *iterPointNext;
				/*圆心相同，半径相同，则删除掉*/
				if (pointNext.distanceTo(point) < CADConsts::dTolerance)
				{
					pointList.erase(iterPointNext++);
				}
				else
					iterPointNext++;
			}
		}
	}

	//@function: 过滤重复线   -- by mzq  2019/12/20 9:20
	void DrawAutility::filterDupLine(std::list<OdDbObjectId> &lineList/*线 list*/)
	{
		std::list<OdDbObjectId>::iterator iterLine = lineList.begin();
		for (iterLine; iterLine != lineList.end(); iterLine++)//循环遍历每一条线
		{
			OdGePoint3d pointStart, pointEnd;
			OdDbLinePtr line = iterLine->safeOpenObject(OdDb::kForWrite);
			line->getStartPoint(pointStart);
			line->getEndPoint(pointEnd);

			std::list<OdDbObjectId>::iterator iterLineNext = iterLine;
			iterLineNext++;
			for (; iterLineNext != lineList.end();)//循环遍历一个线与其之后的每一个线进行比较
			{
				OdGePoint3d pointStartNext, pointEndNext;
				OdDbLinePtr lineNext = iterLineNext->safeOpenObject(OdDb::kForWrite);
				lineNext->getStartPoint(pointStartNext);
				lineNext->getEndPoint(pointEndNext);

				/*判断两个线的起点和终点，如果相同，则删除，注意两个线的起始终止点顺序可能不同，否要进行比较*/
				if ((pointStart.distanceTo(pointStartNext) < CADConsts::dTolerance
					&& pointEnd.distanceTo(pointEndNext) < CADConsts::dTolerance)
					|| (pointStart.distanceTo(pointEndNext) < CADConsts::dTolerance
					&& pointEnd.distanceTo(pointStartNext) < CADConsts::dTolerance))
				{
					lineNext->erase();//从图纸中删除
					lineList.erase(iterLineNext++);//从list中删除
				}
				else
				{
					iterLineNext++;
				}
			}
		}
	}

	//@function: 绘制多个圆组成的外边界   -- by mzq  2019/12/12 10:13
	std::list<OdDbArcPtr> DrawAutility::drawCirclesBoundary(const std::list<CirclesBoundaryInfo> &drawInfoList,/*相关信息*/
															bool isHiddenLine/*是否为虚线*/)
	{
		std::list<OdDbObjectId> listCircle;//所有的圆

		std::list<CirclesBoundaryInfo>::const_iterator iter = drawInfoList.begin();
		for (; iter != drawInfoList.end(); iter++)
		{
			std::list<OdDbObjectId> tempList = CreateEnt::drawCircles(iter->_centrePoints, iter->_radius, isHiddenLine, iter->_colorIndex);
			listCircle.insert(listCircle.end(), tempList.begin(), tempList.end());
		}

		/*打断圆*/
		std::list<PointOnCircle> pointOnCircle;//圆上交点
		DrawAutility::deleteCoincideCircle(listCircle);
		DrawAutility::getCircleIntersection(listCircle, pointOnCircle);
		std::list<ArcOnCircle> listArcOnCircle;
		DrawAutility::calcArc(pointOnCircle, listCircle, listArcOnCircle);

		std::list<OdDbArcPtr> listAllArc;//所有的圆弧及其包围框
		std::list<ArcOnCircle>::iterator iterArc = listArcOnCircle.begin();
		for (; iterArc != listArcOnCircle.end(); iterArc++)
		{
			//创建圆弧
			CreateEnt::addToModelSpace(iterArc->second);
			listAllArc.insert(listAllArc.end(), iterArc->second);
		}

		/*筛选出需要被删除的圆*/
		std::list<OdDbObjectId> listCircleDel;//需要被删除的圆
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
		
		/*删除*/
		std::list<OdDbObjectId>::iterator iterEraseCircleID = listCircleDel.begin();
		for (; iterEraseCircleID != listCircleDel.end(); iterEraseCircleID++)
		{
			OdDbCirclePtr pCircleEnt = iterEraseCircleID->safeOpenObject(OdDb::kForWrite);
			pCircleEnt->erase();
		}

		return listAllArc;
	}

	////@function: 绘制多个圆组成的外边界   -- by mzq  2019/12/12 10:13
	//std::list<OdDbArcPtr> DrawAutility::drawCirclesBoundary(std::list<OdDbArcPtr> listAllArc/*所有的圆弧及其包围框*/)
	//{
	//	std::list<ArcOnCircle>::iterator iterArc = listArcOnCircle.begin();
	//	for (; iterArc != listArcOnCircle.end(); iterArc++)
	//	{
	//		//创建圆弧
	//		CreateEnt::addToModelSpace(iterArc->second);
	//		listAllArc.insert(listAllArc.end(), iterArc->second);
	//	}

	//	/*筛选出需要被删除的圆*/
	//	std::list<OdDbObjectId> listCircleDel;//需要被删除的圆
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

	//	/*删除*/
	//	std::list<OdDbObjectId>::iterator iterEraseCircleID = listCircleDel.begin();
	//	for (; iterEraseCircleID != listCircleDel.end(); iterEraseCircleID++)
	//	{
	//		OdDbCirclePtr pCircleEnt = iterEraseCircleID->safeOpenObject(OdDb::kForWrite);
	//		pCircleEnt->erase();
	//	}

	//	return listAllArc;
	//}

	//@function: 删除重复的圆   -- by mzq  2019/11/4 11:24
	void DrawAutility::deleteCoincideCircle(std::list<OdDbObjectId>& listCircle/*圆list*/)
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

				/*圆心相同，半径相同，则删除掉*/
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

	//@function: 绘制分裂导线的实体   -- by mzq  2019/11/4 11:24
	std::list<OdDbObjectId> DrawAutility::drawSplitEntity(const std::list<OdGePoint3d> &listPoint3d /*分裂端点坐标*/)
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

	//@function: 计算分裂数的端点坐标   -- by mzq  2019/11/4 11:24
	std::list<OdGePoint3d> DrawAutility::creatNumSplitEntity(OdGePoint3d pointHook,/*挂点坐标*/
															 double spaceSplit,/*分裂间距*/
															 double heightSplit,/*分裂高度，仅针对分裂数为3且不是等边三角形的时候有用*/
															 SplitNum::Type splitType/*分裂数*/)
	{
		OdGeVector3d vecHorizontalUnit = OdGeVector3d::kXAxis;//水平向量
		OdGeVector3d vecVerticalUnit = -OdGeVector3d::kYAxis;//垂直向量
		vecHorizontalUnit.normalize();
		vecVerticalUnit.normalize();

		//挂接点所在直线的起终点
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
			double dRotate = CADConsts::PI*(nSplitNum - 2) / nSplitNum;//旋转角度为(弧度)：PI(N-2)/N 根据数学计算来
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
			if (Auxiliary::isEqual(heightSplit, 0))//等边三角形
				point3 = pointHook + vecVerticalUnit * ((spaceSplit * 0.5) / tan(SmartCl::Auxiliary::angle_to_radian(30)));
			else//等腰三角形
				point3 = pointHook + vecVerticalUnit * heightSplit;

			listPoint.insert(listPoint.end(), point1);
			listPoint.insert(listPoint.end(), point2);
			listPoint.insert(listPoint.end(), point3);
		}
		}
		return  listPoint;
	}

	//@function: 绘制放电点示意点，用小半径的圆表示   -- by mzq 2020/11/24 17:58
	void DrawAutility::drawElecPointCircle(std::list<CirclesBoundaryInfo> drawInfoList,/*放电点集*/
										   double radius/*圆半径*/)
	{
		std::list<CirclesBoundaryInfo>::iterator iter = drawInfoList.begin();
		for (; iter != drawInfoList.end(); iter++)
		{
			iter->_radius = radius;
			iter->_colorIndex = 1;//线颜色设置为红色
		}

		DrawAutility::drawCirclesBoundary(drawInfoList);
	}

	//@function: 绘制直线塔I串线夹以及小弧垂   -- by mzq  2019/12/6 15:01
	std::list<OdDbObjectId> DrawAutility::draw_I_L5L6(double dL5,/*L5长度，线夹长度*/
												      double dL6,/*L6长度，小弧垂*/
												      const std::list<OdGePoint3d> &listPoint3d/*起始参考点*/)
	{
		std::list<OdDbObjectId> listObjectID;

		OdGeVector3d vecVerticalUnit = OdGeVector3d::kYAxis;//垂直向量
		vecVerticalUnit.normalize();

		//挂接点所在直线的起终点
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

	//@function: 功能：绘制I串线夹及小弧垂部分，返回分裂三角形的线段ID   -- by mzq 2020/11/25 9:47
	std::list<OdDbObjectId> DrawAutility::draw_I_L5L6Theta(OdGePoint3d pointHook, /*分裂金具挂点*/ 
														   double dL5, /*线夹长度*/ 
														   double dL6, /*小弧垂长度*/ 
														   double dAngle, /*小弧垂偏角*/ 
														   const std::list<OdGePoint3d> &listPoint3d, /*分裂数端点*/ 
														   bool isDrawLine /*= true/*是否绘制绘制线段，默认为绘制*/)
	{
		std::list<OdDbObjectId> listObjectID;

		OdGeVector3d vecVerticalUnit = OdGeVector3d::kYAxis;//垂直向量
		vecVerticalUnit.normalize();

		//挂接点所在直线的起终点
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

	//@function: 绘制V串线夹及小弧垂部分，返回分裂三角形的下方端点坐标   -- by mzq  2020/1/2 18:22
	std::list<OdGePoint3d> DrawAutility::draw_VL_L6L7(OdGePoint3d pointHook, /*分裂金具挂点*/
													  double dL6, /*线夹长度*/
													  double dL7, /*小弧垂长度*/
													  double dAngle, /*小弧垂偏角*/
													  const std::list<OdGePoint3d> &listPoint3d, /*分裂数端点*/
													  bool isDrawLine,/*是否绘制绘制线段，默认为绘制*/
													  bool isRightTriangle/*直角三角形：true，等腰三角形：false*/)
	{
		OdGeVector3d vecVerticalUnit = OdGeVector3d::kYAxis;//垂直向量
		vecVerticalUnit.normalize();

		//挂接点所在直线的起终点
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

	//@function: 功能：绘制V串圆弧   -- by mzq  2019/12/6 15:01
	std::list<OdDbArcPtr> DrawAutility::drawArc_V(const std::list<OdGePoint3d> &listArcCenter,/*中心原点list*/
												  double dRadius, /*半径*/
												  int color,/*线颜色*/
												  OdDbLinePtr pLineL1,/*V角，线段1*/
												  OdDbLinePtr pLineL2 /*V角，线段2*/)
	{
		std::list<CirclesBoundaryInfo> drawInfoList;
		CirclesBoundaryInfo circlesInfo(listArcCenter, dRadius, color);//封装绘制包络圆相关的信息
		drawInfoList.insert(drawInfoList.end(), circlesInfo);//放入到间隙圆信息容器中
		std::list<OdDbArcPtr> listAllArc = DrawAutility::drawCirclesBoundary(drawInfoList);//根据绘制包络圆信息容器，绘制包络圆

		//根据圆心角裁切
		OdResult odRes;
		std::list<OdDbArcPtr>::iterator iterCutArc = listAllArc.begin();//所有的圆弧及其包围框
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
			//构造圆弧的起点终点直线 
			OdDbLinePtr pLineArcStart = OdDbLine::createObject();
			pLineArcStart->setStartPoint(pointBase);
			pLineArcStart->setEndPoint(pointArcStart);
			OdDbLinePtr pLineArcEnd = OdDbLine::createObject();
			pLineArcEnd->setStartPoint(pointBase);
			pLineArcEnd->setEndPoint(pointArcEnd);

			//圆弧和两条腿的交点
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

	//@function: 功能：绘制V串圆弧   -- by mzq 2021/2/26 15:31
	std::list<OdDbArcPtr> DrawAutility::drawArc_V(const std::list<CirclesBoundaryInfo>& drawInfoList,/*相关信息*/
												  OdDbLinePtr pLineL1,/*V角，线段1*/ 
												  OdDbLinePtr pLineL2 /*V角，线段2*/)
	{
		std::list<OdDbArcPtr> listAllArc = DrawAutility::drawCirclesBoundary(drawInfoList);//根据绘制包络圆信息容器，绘制包络圆

		//根据圆心角裁切
		OdResult odRes;
		std::list<OdDbArcPtr>::iterator iterCutArc = listAllArc.begin();//所有的圆弧及其包围框
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
			//构造圆弧的起点终点直线 
			OdDbLinePtr pLineArcStart = OdDbLine::createObject();
			pLineArcStart->setStartPoint(pointBase);
			pLineArcStart->setEndPoint(pointArcStart);
			OdDbLinePtr pLineArcEnd = OdDbLine::createObject();
			pLineArcEnd->setStartPoint(pointBase);
			pLineArcEnd->setEndPoint(pointArcEnd);

			//圆弧和两条腿的交点
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

	//@function: 计算两个直线的夹角   -- by mzq  2020/1/4 11:12
	double DrawAutility::getTwoLineAngle(OdDbLinePtr pLine1,/*线1*/ 
										 OdDbLinePtr pLine2/*线2*/)
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

	//@function: 以圆心和半径为参数，返回圆周上的等效8个点   -- by mzq 2022/12/3 14:10
	std::list<OdGePoint3d> DrawAutility::getPointOnCircle(OdGePoint3d centerPoint/*圆心点*/, 
														  double radius/*半径*/)
	{
		std::list<OdGePoint3d> listPoint;

		OdGePoint3d pointStart = centerPoint;
		pointStart.y += radius;
		OdGeVector3d vecRotate(0, 0, 1);
		for (int index = 0; index < 8; index++)
		{
			double dRotate = CADConsts::PI * 2.0 * index / 8;//旋转角度为(弧度)
			OdGePoint3d point = pointStart;
			point.rotateBy(dRotate, vecRotate, centerPoint);
			listPoint.insert(listPoint.end(), point);
		}
		return listPoint;
	}
}