#include "OdaCommon.h" 
#include "CreateEnt.h"
#include "DbLine.h"
#include "DbBlockTableRecord.h"
#include "OdqInterfaces.h"
#include "DbArc.h"
#include "DbText.h"  
#include "Gi\GiGeometry.h"
#include "DbPolyline.h"
#include "DbCircle.h"
#include "DbAlignedDimension.h"
#include "DbLinetypeTableRecord.h"
#include "DbLinetypeTable.h"
#include "DbDimension.h"
#include "DbDimStyleTableRecord.h"
#include "DbDimStyleTable.h"
#include "DbObjectId.h"
#include "DbLayerTableRecord.h"
#include "DbLayerTable.h"
#include "DbHatch.h"
#include "DbBlockTable.h"
#include "..\SmartClConsole\CommandContextImpl.h"
#include "ExCommandsUtils.h"
#include "Db3dSolid.h"
#include "..\SmartClApp\OdqMainWindow.h"
#include "DataBase.h"
#include "DbTextStyleTableRecord.h"
#include "DbDatabase.h"
#include "DbSymbolTable.h"
#include "DbTextStyleTableRecord.h"
#include "DbTextStyleTable.h"
#include "DbBlockReference.h"
#include "DbHostAppServices.h"
#include "Db2LineAngularDimension.h"
#include "CADConsts.h"
#include "DbRadialDimension.h"
#include "AbstractViewPE.h"

OdEdCommandContext* CreateEnt::_pCmdCtx = NULL;

//@function: 获取数据库   -- by mzq  2019/11/4 10:28
OdDbDatabasePtr CreateEnt::getDataBase()
{
	OdDbCommandContextPtr pDbCmdCtx(_pCmdCtx);
	return pDbCmdCtx->database();
}

//@function: 获取所有实体   -- by mzq  2019/12/11 9:11
std::list<OdDbObjectId> CreateEnt::getAllEntity()
{
	std::list<OdDbObjectId> listAllEntity;//所有实体

	OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(getDataBase());
	OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

	OdDbObjectIteratorPtr iterEntity = pSpace->newIterator();
	iterEntity->start();
	while (!iterEntity->done())
	{
		listAllEntity.push_back(iterEntity->objectId());
		iterEntity->step();
	}
	return listAllEntity;
}

//@function: 添加到模型空间   -- by mzq  2019/11/4 10:28
 OdDbObjectId CreateEnt::addToModelSpace(OdDbEntityPtr entity)
{
	OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(getDataBase());
	OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);
	return pSpace->appendOdDbEntity(entity);
}

 //@function: 清空图纸   -- by mzq  2019/11/4 10:28
 void CreateEnt::clearModelSpace()
 {
	 OdDbObjectId idModelSpace = getDataBase()->getModelSpaceId();
	 OdDbBlockTableRecordPtr pModelSpace = idModelSpace.safeOpenObject(OdDb::kForRead);
	 OdDbObjectIteratorPtr pObject = pModelSpace->newIterator();

	 for (pObject->start(); !pObject->done(); pObject->step())
	 {
		 OdDbObjectId idEntity = pObject->objectId();
		 OdDbObjectPtr pEntity = idEntity.safeOpenObject(OdDb::kForWrite);
		 pEntity->erase();
	 }
 }

 //@function: 功能：删除实体   -- by mzq  2019/12/20 15:20
 void CreateEnt::deletEntity(const std::list<OdDbObjectId> &entityList/*需要删除的实体*/)
 {
	 std::list<OdDbObjectId>::const_iterator iterID = entityList.begin();
	 for (iterID; iterID != entityList.end(); iterID++)
	 {
		 OdDbObjectPtr pEntity = iterID->safeOpenObject(OdDb::kForWrite);
		 pEntity->erase();
	 }
 }

 //@function: //功能：绘制直线   -- by mzq  2019/11/4 11:23
 OdDbObjectId CreateEnt::drawLine(const OdGePoint3d& ptStart, /*线段起点 */
								  const OdGePoint3d& ptEnd, /* 线段终点 */
								  int colorIndex /*= 0/*线段颜色, 默认7为白色线*/)
 {
	 OdDbLinePtr pLine = OdDbLine::createObject();
	 pLine->setStartPoint(ptStart);
	 pLine->setEndPoint(ptEnd);
	 pLine->setColorIndex(colorIndex);
	 pLine->setLineWeight(OdDb::kLnWtByLayer);
	 return addToModelSpace(pLine);
 }

 //@function: 功能：绘制直线   -- by mzq  2019/11/4 11:24
 OdDbObjectId CreateEnt::drawPolyLine(const OdGePoint3d& ptStart, /*线段起点 */
								   const OdGePoint3d& ptEnd, /*线段终点 */
								   double lineWidth /*= -1.0*/, /*线宽*/
								   int colorIndex /*= 0/*线段颜色*/)
 {
	 OdDbPolylinePtr pLine = OdDbPolyline::createObject();
	 pLine->addVertexAt(0, OdGePoint2d(ptStart.x, ptStart.y), 0, lineWidth, lineWidth);
	 pLine->addVertexAt(1, OdGePoint2d(ptEnd.x, ptEnd.y), 0, lineWidth, lineWidth);
	 pLine->setColorIndex(colorIndex);
	 return addToModelSpace(pLine);
 }

 //@function: 绘制多段线   -- by mzq  2019/12/6 14:46
 OdDbObjectId CreateEnt::drawPolyLineByPoints(const OdGePoint2dArray& ptSet, /*多段线点集 */
										   bool isClosed /*= false*/, /*是否闭合 */
										   double lineWidth /*= -1.0*/, /*线宽*/
										   int colorIndex /*= 0/*线段颜色*/)
 {
	 OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
	 for (int i = 0; i < int(ptSet.size()); ++i)
	 {
		 pPolyline->addVertexAt(i, ptSet.at(i), 0, lineWidth, lineWidth);
	 }
	 pPolyline->setClosed(isClosed);
	 pPolyline->setColorIndex(colorIndex);
	 return addToModelSpace(pPolyline);
 }

 //@function: 功能：绘制多段线   -- by mzq 2022/12/3 16:05
 OdDbObjectId CreateEnt::drawPolyLineByPoints(const std::list<OdGePoint3d> listPoint, /*多段线点集 */ 
											  bool isClosed /*= false*/, /*是否闭合 */ 
											  double lineWidth /*= -1.0*/, /*线宽*/ 
											  int colorIndex /*= 0/*线段颜色*/)
 {
	 OdGePoint2dArray Point2dArray;
	 for (std::list<OdGePoint3d>::const_iterator iterPoint = listPoint.begin(); iterPoint != listPoint.end(); iterPoint++)
	 {
		 OdGePoint3d point3D = *iterPoint;
		 OdGePoint2d point2D(point3D.x, point3D.y);
		 Point2dArray.append(point2D);
	 }

	 OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
	 for (int i = 0; i < int(Point2dArray.size()); ++i)
	 {
		 pPolyline->addVertexAt(i, Point2dArray.at(i), 0, lineWidth, lineWidth);
	 }
	 pPolyline->setClosed(isClosed);
	 pPolyline->setColorIndex(colorIndex);
	 return addToModelSpace(pPolyline);
 }

 //@function: 功能：绘制2d矩形   -- by mzq  2019/10/30 15:24
 OdDbObjectId CreateEnt::drawRect(const OdGePoint3d& leftTopPoint, /*左上角点 */
								  double height, /*高度 */
								  double width,/*宽度 */
								  double lineWidth /*= -1.0*/, /*线宽，默认为无线宽*/
								  int colorIndex /*= 0/*线段颜色*/)
 {
	 //画矩形
	 OdDbPolylinePtr pRect = OdDbPolyline::createObject();
	 pRect->addVertexAt(0, OdGePoint2d(leftTopPoint.x, leftTopPoint.y), 0, lineWidth, lineWidth);
	 pRect->addVertexAt(1, OdGePoint2d(leftTopPoint.x + width, leftTopPoint.y), 0, lineWidth, lineWidth);
	 pRect->addVertexAt(2, OdGePoint2d(leftTopPoint.x + width, leftTopPoint.y - height), 0, lineWidth, lineWidth);
	 pRect->addVertexAt(3, OdGePoint2d(leftTopPoint.x, leftTopPoint.y - height), 0, lineWidth, lineWidth);
	 pRect->setClosed(true);
	 pRect->setColorIndex(colorIndex);
	 return addToModelSpace(pRect);
 }

 //@function: 功能：绘制单行文本   -- by mzq  2019/10/31 15:56
 OdDbObjectId CreateEnt::drawText(const OdGePoint3d& point3d, /*文本起始位置，左上角点 */
							   const OdString& text, /*文本内容 */
							   OdDbObjectId& textStyleId,/*字体 */
							   double height /*= 10*/, /*字高 */
							   int colorIndex /*= 0*/)
 {
	 OdDbTextPtr pText = OdDbText::createObject();
	 pText->setPosition(OdGePoint3d(point3d.x, point3d.y - height, point3d.z));//pText默认起始点为左下角点，这里修改为左上角点，方便与多行文本对应
	 pText->setTextString(text);
	 pText->setColorIndex(colorIndex);
	 pText->setHeight(height);
	 pText->setWidthFactor(0.7);
	 pText->setTextStyle(textStyleId);
	 return addToModelSpace(pText);
 }

 //@function: 功能：绘制多行文本，自动换行   -- by mzq  2019/10/30 17:54
 OdDbObjectId CreateEnt::drawMultiText(const OdGePoint3d& point3d, /*文本起始位置，左上角点 */
									const OdString& text, /*文本内容 */
									const OdDbObjectId& textStyleId,/*字体 */
									double textWidth,/*文本自动换行宽度 */
									double lineSpacingFactor,//行间距系数
									double height /*= 10*/, /*字高 */
									int colorIndex /*= 2/*颜色*/)
 {
	 OdDbMTextPtr pText = OdDbMText::createObject();
	 pText->setLocation(point3d);
	 pText->setContents(text);
	 pText->setTextStyle(textStyleId);
	 pText->setWidth(textWidth);
	 pText->setLineSpacingFactor(lineSpacingFactor);//多行之间的行距
	 pText->setTextHeight(height);
	 pText->setColorIndex(colorIndex);
	 return addToModelSpace(pText);
 }

 //@function: 功能：绘制圆   -- by mzq  2019/11/4 11:26
 OdDbObjectId CreateEnt::drawCircle(const OdGePoint3d& ptCenter, //圆心位置
									double radius, //半径
									bool isHiddenLine,/*是否为虚线*/
									int colorIndex/*颜色*/)
 {
	 OdDbCirclePtr pCircle = OdDbCircle::createObject();
	 if (isHiddenLine)
		 pCircle->setLinetype(getHiddenLineType());
	 pCircle->setColorIndex(colorIndex);
	 pCircle->setCenter(ptCenter);
	 pCircle->setRadius(radius);
	 return addToModelSpace(pCircle);
 }

 //@function: 根据圆心绘制多组圆   -- by mzq  2019/11/4 11:24
 std::list<OdDbObjectId> CreateEnt::drawCircles(const std::list<OdGePoint3d> &listCenter,/*圆心点集*/
												double dRadius, /*半径*/
												bool isHiddenLine,/*是否为虚线*/
												int color/*颜色*/)
 {
	 std::list<OdDbObjectId> listObjectID;
	 std::list<OdGePoint3d>::const_iterator iterCenter = listCenter.begin();
	 for (; iterCenter != listCenter.end(); iterCenter++)
	 {
		 OdDbObjectId circleID1 = CreateEnt::drawCircle(*iterCenter, dRadius, isHiddenLine, color);
		 listObjectID.insert(listObjectID.end(), circleID1);
	 }
	 return listObjectID;
 }

 //@function: 功能：绘制弧   -- by mzq  2019/11/4 11:26
 OdDbObjectId CreateEnt::drawArc(const OdGePoint3d& ptCenter, /*弧心位置 */
							  double radius, /*半径 */
							  double startAngle, /*起始弧度 */
							  double endAngel, /*终止弧度 */
							  int colorIndex /*= 2/*弧颜色*/)
 {
	 OdDbArcPtr pArc = OdDbArc::createObject();
	 pArc->setColorIndex(colorIndex);
	 pArc->setCenter(ptCenter);
	 pArc->setRadius(radius);
	 pArc->setStartAngle(startAngle);
	 pArc->setEndAngle(endAngel);
	 return addToModelSpace(pArc);
 }

 //@function: 旋转实体   -- by mzq  2019/12/9 17:12
 std::list<OdDbObjectId> CreateEnt::rotateCurve(OdGePoint3d pointOrigin, /*旋转中心点*/
												double dAngle, /*旋转角度，非弧度*/
												const std::list<OdDbObjectId> &listWholeID/*需要旋转的实体线段id*/)
 {
	 OdGeMatrix3d xfmRotate;
	 OdGeVector3d axis;
	 xfmRotate = xfmRotate.rotation(SmartCl::CADConsts::DegToRad * dAngle, OdGeVector3d::kZAxis, pointOrigin);

	 OdGePoint3d origin;
	 OdGeVector3d xAxis;
	 OdGeVector3d yAxis;
	 OdGeVector3d zAxis;
	 xfmRotate.getCoordSystem(origin, xAxis, yAxis, zAxis);

	 std::list<OdDbObjectId> listNewEntity;
	 std::list<OdDbObjectId>::const_iterator iterID = listWholeID.begin();
	 for (iterID; iterID != listWholeID.end(); iterID++)
	 {
		 OdDbCurvePtr curve = iterID->safeOpenObject(OdDb::kForWrite);
		 OdDbCurvePtr curveNew = curve->clone();
		 curveNew->transformBy(xfmRotate);

		 OdDbObjectId entityID = addToModelSpace(curveNew);//添加到模型空间
		 listNewEntity.insert(listNewEntity.end(), entityID);
	 }

	 return listNewEntity;
 }

 //@function: 旋转 点list   -- by mzq  2019/12/19 14:15
 void CreateEnt::rotatePoint(std::list<OdGePoint3d> &pointList,/*点list*/
							 OdGePoint3d pointOrigin, /*旋转中心点*/
							 double dAngle /*旋转角度，非弧度*/)
 {
	 OdGeMatrix3d xfmRotate;
	 OdGeVector3d axis;
	 xfmRotate = xfmRotate.rotation(SmartCl::CADConsts::DegToRad * dAngle, OdGeVector3d::kZAxis, pointOrigin);

	 OdGePoint3d origin;
	 OdGeVector3d xAxis;
	 OdGeVector3d yAxis;
	 OdGeVector3d zAxis;
	 xfmRotate.getCoordSystem(origin, xAxis, yAxis, zAxis);

	 std::list<OdGePoint3d>::iterator iterID = pointList.begin();
	 for (iterID; iterID != pointList.end(); iterID++)
	 {
		 iterID->transformBy(xfmRotate);
	 }
 }

 //@function: 平移 点list   -- by mzq  2019/12/19 17:59
 void CreateEnt::translatePoint(std::list<OdGePoint3d> &pointList,/*点list*/
								OdGePoint3d direction/*平移方向，点向量*/)
 {
	 OdGeVector3d vecUnit_X = OdGeVector3d::kXAxis;//水平向量
	 OdGeVector3d vecUnit_Y = OdGeVector3d::kYAxis;//水平向量
	 OdGeVector3d vecUnit_Z = OdGeVector3d::kZAxis;//水平向量
	 vecUnit_X.normalize();
	 vecUnit_Y.normalize();
	 vecUnit_Z.normalize();

	 std::list<OdGePoint3d>::iterator iterID = pointList.begin();
	 for (iterID; iterID != pointList.end(); iterID++)
	 {
		 *iterID = *iterID + vecUnit_X * direction.x;
		 *iterID = *iterID + vecUnit_Y * direction.y;
		 *iterID = *iterID + vecUnit_Z * direction.z;
	 }
 }

 //@function: 功能：绘制对齐标注  // TODO: 标注样式参数类型为箭头样式，需修改    -- by mzq  2019/11/4 16:14
 OdDbObjectId CreateEnt::dimAligned(const OdGePoint3d& dimLinePt, //标注信息位置 
									const OdGePoint3d& ptStart, //标注起点
									const OdGePoint3d& ptEnd //标注终点 
									/*OdDbObjectId dimStyleId*//*标注样式*/)
 {
	 OdDbAlignedDimensionPtr pDimension = OdDbAlignedDimension::createObject();
	 pDimension->setXLine1Point(ptStart);
	 pDimension->setXLine2Point(ptEnd);

	 pDimension->setDimLinePoint(dimLinePt);
	 pDimension->useDefaultTextPosition();
	 //pDimension->setDimscale(5.0);
	 pDimension->setExtLineFixLenEnable(true);
	 pDimension->setExtLineFixLen(0.0);

	 pDimension->setDimensionText(OD_T("12900"));
	 pDimension->setDimsah(true);
	 bool t = OdDmUtil::isZeroLengthArrow(OD_T("_ARCHTICK"));
	 pDimension->setDimtsz(0.3);
	 //pDimension->setDimblk(OdDmUtil::getArrowId(OD_T("_OBLIQUE"), pDatabase));
	 OdDbHardPointerId id1 = OdDmUtil::getArrowId(OD_T("_ARCHTICK"), getDataBase());
	 OdDbHardPointerId id2 = OdDmUtil::getArrowId(OD_T("_DOT"), getDataBase());
	 pDimension->setDimblk1(id1);
	 pDimension->setDimblk2(id2);
	 //pDimension->setDimblk2(OdDmUtil::getArrowId(OD_T("_OBLIQUE"), pDatabase));

	 //OdDbObjectId dimStyleId = addObliqueDimStyle();
	 //
	 //pDimension->setDimstyleData(dimStyleId);
	 return addToModelSpace(pDimension);
 }

 //@function: 功能：绘制线性标注   -- by mzq  2019/12/13 9:51
 OdDbObjectId CreateEnt::dimAligned(OdDbLinePtr pLine, /*需要标注的直线*/
									OdDbObjectId dimStyleID,/*标注样式*/
									double dimScale/*标注尺寸缩放*/)
 {
	 OdGePoint3d pointStart, pointEnd;
	 OdGePoint3d pointDimLine;
	 pLine->getStartPoint(pointStart);
	 pLine->getEndPoint(pointEnd);
	 double dLineLength = pointStart.distanceTo(pointEnd);
	 pLine->getPointAtParam(dLineLength / 2.0, pointDimLine);
	 OdGeVector3d vectLine = pointEnd - pointStart;
	 vectLine.normalize();
	 vectLine = vectLine.perpVector();
	 pointDimLine = pointDimLine + vectLine * 1000/ dimScale;

	 OdDbAlignedDimensionPtr pAlignedDim = OdDbAlignedDimension::createObject();
	 pAlignedDim->setDatabaseDefaults(getDataBase());
	 pAlignedDim->setDimensionStyle(dimStyleID);
	 pAlignedDim->setXLine1Point(pointStart);
	 pAlignedDim->setXLine2Point(pointEnd);
	 pAlignedDim->setDimLinePoint(pointDimLine);


	 /*标注比例缩放*/
	 QString qstr = QString::number(dLineLength * dimScale);
	 pAlignedDim->setDimensionText(getIApp()->toOdString(qstr));


	 return addToModelSpace(pAlignedDim);
 }

 //@function: 绘制角度标注   -- by mzq  2019/12/9 18:37
 OdDbObjectId CreateEnt::dimAngle(const OdGePoint3d& rotataCenter, /*中心点*/
								  const OdGePoint3d& linePoint,/* 线上的点*/
								  double angle, /*角度，非弧度*/
								  OdDbObjectId dimStyleID,/*标注样式*/
								  int colorIndex /*颜色*/)
 {
	 //绘制思路：以中心点为中心，以第一个线上的点为标注起点，然后顺时针旋转对应角度，即完成标注

	 OdDb2LineAngularDimensionPtr pLineAngleDim = OdDb2LineAngularDimension::createObject();
	 pLineAngleDim->setDimensionStyle(dimStyleID);
	 pLineAngleDim->useDefaultTextPosition();
	 pLineAngleDim->setXLine1Start(rotataCenter);
	 pLineAngleDim->setXLine1End(linePoint);

	 /*中间点*/
	 OdGePoint3d arcPoint = linePoint;
	 arcPoint.rotateBy(-angle * SmartCl::CADConsts::DegToRad / 2.0,/*角度转弧度，按照顺时针方向旋转*/
					   OdGeVector3d::kZAxis,
					   rotataCenter);
	 pLineAngleDim->setArcPoint(arcPoint);

	 OdGePoint3d anotherLinePoint = linePoint;
	 anotherLinePoint.rotateBy(-angle * SmartCl::CADConsts::DegToRad, /*角度转弧度，按照顺时针方向旋转*/
							   OdGeVector3d::kZAxis,
							   rotataCenter);

	 pLineAngleDim->setXLine2Start(rotataCenter);
	 pLineAngleDim->setXLine2End(anotherLinePoint);
	 pLineAngleDim->setDimensionText(getIApp()->toOdString(QString::number(fabs(angle), 'f', 2)));
	 pLineAngleDim->setColorIndex(colorIndex);
	 return addToModelSpace(pLineAngleDim);
 }
 //@function: 功能：绘制角度标注     -- by mzq  2019/12/13 10:57
 OdDbObjectId CreateEnt::dimAngle(OdDbLine *pLineStart,/*起始线*/
								  OdDbLine *pLineEnd,/*终止线*/
								  OdDbObjectId g_idDimStyle,/*标注风格*/
								  int colorIndex /*颜色*/)
 {
	 //绘制角度标注，思路：以起始线开始，以终止线结束，按照顺时针方向标注
	 OdGePoint3d ptStart1, ptStart2;
	 OdGePoint3d ptEnd1, ptEnd2;
	 pLineStart->getStartPoint(ptStart1);
	 pLineStart->getEndPoint(ptEnd1);
	 pLineEnd->getStartPoint(ptStart2);
	 pLineEnd->getEndPoint(ptEnd2);

	 OdGeVector3d vectLine1 = ptEnd1 - ptStart1;
	 OdGeVector3d vectLine2 = ptEnd2 - ptStart2;

	 double dAngle = vectLine1.angleTo(vectLine2);
	 double dAngleDeg = OdaToDegree(dAngle);

	 vectLine1.normalize();
	 double length = ptStart1.distanceTo(ptEnd1);
	 OdGePoint3d line1DimPoint = ptStart1 + vectLine1 * length * 0.3;

	 OdGePoint3dArray points;
	 OdGePoint3d intersectPoint;
	 pLineStart->intersectWith(pLineEnd, OdDb::kExtendBoth, points);
	 intersectPoint = points.getAt(0);

	 return dimAngle(intersectPoint, line1DimPoint, dAngleDeg, g_idDimStyle, colorIndex);
 }

 //@function: 标注弧半径   -- by mzq  2019/12/11 9:26
 void CreateEnt::dimArcRadial(std::list<OdDbArcPtr> pArcCircle, /*圆弧list*/
							  OdDbObjectId g_idDimStyle, /*标注样式*/
							  double dimScale/*标注尺寸缩放*/)
 {
	 OdGePoint3d pointCenter;
	 double dRadius = 0.0;
	 OdGePoint3d pointOnCurve;

	 std::list<OdDbArcPtr>::iterator iterArc = pArcCircle.begin();
	 for (iterArc; iterArc != pArcCircle.end(); iterArc++)
	 {
		 OdDbArcPtr pArc = *iterArc;
		 pointCenter = pArc->center();
		 dRadius = pArc->radius();
		 double dStartAngle = pArc->startAngle();
		 double dEndAngle = pArc->endAngle();
		 double dHalfAngle = (dEndAngle + dStartAngle) / 2.0;
		 if (dStartAngle > dEndAngle)
		 {
			 dHalfAngle += SmartCl::CADConsts::PI;
		 }
		 OdResult result = pArc->getPointAtParam(dHalfAngle, pointOnCurve);
		 if (eOk != result)
		 {
			 continue;
		 }

		 //半径标注
		 OdDbRadialDimensionPtr pDimension = OdDbRadialDimension::createObject();
		 pDimension->setDatabaseDefaults(getDataBase());
		 pDimension->setDimensionStyle(g_idDimStyle);
		 pDimension->setColorIndex(pArc->colorIndex());
		 pDimension->setCenter(pointCenter);
		 OdGeVector3d chordVector(pointOnCurve - pointCenter);
		 OdGePoint3d pointTextPos = pointOnCurve - chordVector / 2.0;
		 pDimension->useDefaultTextPosition();
		 pDimension->setChordPoint(pointOnCurve);

		 /*标注比例缩放*/
		 QString qstr = QString::number(pArc->radius() * dimScale);
		 pDimension->setDimensionText(getIApp()->toOdString(qstr));

		 addToModelSpace(pDimension);
	 }
 }

 //@function: 保存图纸   -- by mzq  2019/11/4 11:01
 void CreateEnt::saveDWG(const OdString& pathName/*保存路径*/)
 {
	 try
	 {
		 getDataBase()->writeFile(pathName, OdDb::SaveType::kDwg, OdDb::DwgVersion::vAC21);
	 }
	 catch (OdError err)
	 {
		 OdResult errResult = err.code();
		 QString errTitle = QString::fromLocal8Bit("警告！");
		 if (eFileSharingViolation == errResult)
		 {
			 errTitle = QString::fromLocal8Bit("警告:该图纸已经被打开，请先关闭！");
		 }
		 QString qsError = getIApp()->toQString(err.description()); // ? out of range
		 QMessageBox::warning(nullptr, errTitle, qsError, QMessageBox::Yes);
	 }

	 QString sqtrFileFullName = getIApp()->toQString(pathName);
	 sqtrFileFullName = "%1:" + sqtrFileFullName;
	 sqtrFileFullName = sqtrFileFullName.arg(QString::fromLocal8Bit("图纸已保存至："));
	 getIApp()->toStatusBar(sqtrFileFullName);
 }

 //@function: 添加层   -- by mzq  2019/11/4 10:28
 OdDbObjectId CreateEnt::addLayer(const OdString& layerName, /*层名称*/
								  int colorIndex, /*颜色*/
								  OdDb::LineWeight lineWeight/*线宽*/)
 {
	 OdDbLayerTablePtr pLayers = getDataBase()->getLayerTableId().safeOpenObject(OdDb::kForWrite);
	 if (pLayers->has(layerName))
		 return pLayers->getAt(layerName);
	 OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::createObject();
	 pLayer->setName(layerName);
	 pLayer->setColorIndex(colorIndex);
	 pLayer->setLineWeight(lineWeight);
	 return pLayers->add(pLayer);
 }

 //@function: 添加字体样式   -- by mzq  2019/10/29 14:33
 OdDbObjectId CreateEnt::addTextStyle(const OdString& styleName/*字体样式名称*/,
									  const OdString& fileName/*小字体名称*/,
									  const OdString& bigFileName/*大字体名称*/)
 {
	 OdDbTextStyleTablePtr table = CreateEnt::getDataBase()->getTextStyleTableId().safeOpenObject(OdDb::kForWrite);
	 OdDbObjectId id = table->getAt(styleName);
	 if (id.isNull())
	 {
		 OdDbTextStyleTableRecordPtr pRecord = OdDbTextStyleTableRecord::createObject();
		 pRecord->setName(styleName);
		 pRecord->setFileName(getIApp()->toOdString(QCoreApplication::applicationDirPath()) + OD_T("\\fonts\\") + fileName);
		 pRecord->setBigFontFileName(getIApp()->toOdString(QCoreApplication::applicationDirPath()) + OD_T("\\fonts\\") + bigFileName);
		 pRecord->setTextSize(0);
		 pRecord->setXScale(0.7);
		 pRecord->setObliquingAngle(0);
		 id = table->add(pRecord);
	 }
	 return id;
 }

//@function: 功能：添加填充    -- by mzq  2019/11/4 16:14
 void CreateEnt::addHatch()
 {
	 //Add in model space
	 OdDbBlockTableRecordPtr pModel = getDataBase()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
	 OdDbHatchPtr pHatch = OdDbHatch::createObject();
	 pModel->appendOdDbEntity(pHatch);

	 // Set the parameters
	 pHatch->setAssociative(true);
	 pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("AR-CONC"));
	 pHatch->setHatchStyle(OdDbHatch::kNormal);

	 // Define the hatch color
	 OdCmColor col;
	 col.setRGB(0, 150, 255);
	 pHatch->setColor(col);

	 OdDbCirclePtr cirArc = OdDbCircle::createObject();
	 OdGePoint3d point;
	 point.x = -50;
	 point.y = 50;
	 OdGePoint3d cenPt(point.x, point.y, 0);
	 // Set porpeties to circle
	 cirArc->setCenter(cenPt);
	 cirArc->setRadius(20);
	 // Define the array of edges and apply them to hatch
	 OdDbObjectIdArray loopIds;
	 loopIds.push_back(addToModelSpace(cirArc));
	 // Append loop to the hatch
	 pHatch->appendLoop(OdDbHatch::kDefault, loopIds);
 }

OdDbObjectId CreateEnt::getHiddenLineType()
{
	OdDbLinetypeTablePtr pLinetypes = getDataBase()->getLinetypeTableId().safeOpenObject(OdDb::kForWrite);
	if (pLinetypes->has(OD_T("Hidden")))
		return pLinetypes->getAt(OD_T("Hidden"));


	OdDbLinetypeTableRecordPtr pLinetype = OdDbLinetypeTableRecord::createObject();
	pLinetype->setName(OD_T("Hidden"));
	pLinetype->setComments(OD_T("- - -"));
	pLinetype->setNumDashes(2);
	pLinetype->setPatternLength(1);
	pLinetype->setDashLengthAt(0, 100);
	pLinetype->setDashLengthAt(1, -100);
	return pLinetypes->add(pLinetype);
}

OdDbObjectId CreateEnt::addObliqueDimStyle()
{
	IqConsole* iConsole = getIConsole();
	OdEdCommandContext* pCmdCtx = iConsole->getCommandContext();
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabase* pDatabase = pDbCmdCtx->database();
	
	//Create the DimStyle 
	OdDbDimStyleTableRecordPtr pDimStyle = OdDbDimStyleTableRecord::createObject();
	// Set the name 
	pDimStyle->setName(L"DimStyle_1");
	// Open the DimStyleTable 
	OdDbDimStyleTablePtr pTable = pDatabase->getDimStyleTableId().safeOpenObject(OdDb::kForWrite);
	// Add the DimStyle
	OdDbObjectId dimStyleId = pTable->add(pDimStyle);
	// Set some properties
	pDimStyle->setDimtxsty(pDatabase->getTextStyleStandardId());
	pDimStyle->setDimsah(true);
	pDimStyle->setDimblk1(OD_T("_OBLIQUE"));
	pDimStyle->setDimblk2(OD_T("_DOT"));
	return dimStyleId;
}

//@function: 添加标注样式   -- by mzq  2019/12/9 9:43
OdDbObjectId CreateEnt::addDimStyle(const OdString& dimStyleName/*标注样式名称*/)
{
	OdDbDimStyleTableRecordPtr pDimStyle = OdDbDimStyleTableRecord::createObject();
	pDimStyle->setName(dimStyleName);
	OdDbDimStyleTablePtr pTable = getDataBase()->getDimStyleTableId().safeOpenObject(OdDb::kForWrite);// Open the DimStyleTable

	/*添加标注样式，已有侧返回*/
	OdDbObjectId dimStyleId = pTable->getAt(dimStyleName);
	if (dimStyleId != OdDbObjectId::kNull)
	{
		return dimStyleId;
	}
	dimStyleId = pTable->add(pDimStyle);

	/*设置标注字体*/
	OdDbObjectId textStyleId = CreateEnt::addTextStyle(OD_T("textStyle"), OD_T("tssdeng.shx"), OD_T("HZTXT.SHX"));
	pDimStyle->setDimtxsty(textStyleId);
	pDimStyle->setDimsah(true);
	pDimStyle->setDimblk1(OD_T("_OBLIQUE"));
	pDimStyle->setDimblk2(OD_T("_DOT"));
	return dimStyleId;
}

//@function: 功能：插入图块   -- by mzq  2019/11/4 14:08
OdDbObjectId CreateEnt::insertBlk(const OdString& blockname/*图块名称*/,
								  const OdString& strDwgFileName/*插入dwg路径*/,
								  const OdGePoint3d& ptInsert /*初始位置*/,
								  double dScale /*= 1.0 /*缩放比例*/,
								  double dRotationAngle /*= 0.0 /*旋转角度，弧度*/)
{
	OdDbDatabasePtr pCurDb = getDataBase();
	OdDbBlockTablePtr pBlkTable = pCurDb->getBlockTableId().safeOpenObject(OdDb::kForRead);
	OdDbObjectId insertId = OdDbObjectId::kNull;
	OdDbObjectId blockId = OdDbObjectId::kNull;

	if (!pBlkTable->has(blockname))
	{
		OdDbDatabasePtr pDwg = pCurDb->appServices()->readFile(strDwgFileName);
		if (!pDwg.get())
			return blockId;
		insertId = pCurDb->insert(blockname, pDwg, true);

		if (insertId.isNull())
			return blockId;
	}
	else
	{
		insertId = pBlkTable->getAt(blockname);
	}

	//设置插入点，旋转角度，比例等等
	OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::createObject();
	pBlkRef->setPosition(ptInsert);
	pBlkRef->setBlockTableRecord(insertId);
	pBlkRef->setScaleFactors(OdGeScale3d(dScale, dScale, 1));
	pBlkRef->setRotation(dRotationAngle);
	blockId = addToModelSpace(pBlkRef);
	return blockId;

}

void CreateEnt::drawMirrorShape(OdGePoint2dArray aryPt, 
								const OdGePoint2d ptMirrorS, 
								const OdGePoint2d ptMirrorE, 
								OdRxObjectPtrArray& idBlockLayer, double dcolor /*= 0*/)
{
	//绘制轴对称图形
	OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
	OdDbPolylinePtr pPolylinetran = OdDbPolyline::createObject();
	for (unsigned int i = 0; i < aryPt.length(); i++)
	{
		pPolyline->addVertexAt(i, OdGePoint2d(aryPt.at(i).x, aryPt.at(i).y));
	}
	for (unsigned int i = 0; i < aryPt.length(); i++)
	{
		pPolylinetran->addVertexAt(i, OdGePoint2d(aryPt.at(i).x, aryPt.at(i).y));
	}

	OdGeMatrix3d mat;
	mat.setToMirroring(OdGeLine3d(OdGePoint3d(ptMirrorS.x, ptMirrorS.y, 0), OdGePoint3d(ptMirrorE.x, ptMirrorE.y, 0)));
	pPolylinetran->transformBy(mat);
	pPolyline->setColorIndex(dcolor);
	pPolylinetran->setColorIndex(dcolor);

	// 	OdRxObjectPtrArray ptrAry;
	// 	ptrAry.append(pPolyline);
	// 	ptrAry.append(pPolylinetran);
	// 
	// 	addToModelSpace(ptrAry);

	idBlockLayer.push_back(pPolyline);
	idBlockLayer.push_back(pPolylinetran);
}


//@function: 功能：获取两点的中点   -- by mzq  2019/11/4 16:14
OdGePoint3d CreateEnt::midPoint(const OdGePoint3d& point1, /*点1*/
								const OdGePoint3d& point2/*点2*/)
{
	OdGePoint3d ret;
	ret.set(point1.x + point2.x, point1.y + point2.y, point1.z + point2.z);
	ret /= 2.0;
	return ret;
}

//@function: 设置对应标注id的显示风格   -- by mzq  2019/12/9 9:39
void CreateEnt::setDimStyleRec(OdDbObjectId idDimStyle, /*标注风格id*/ 
							   double fontScale/*文字缩放比例*/)
{
	OdDbDimStyleTableRecordPtr pDimStyle = idDimStyle.safeOpenObject(OdDb::kForWrite);
	pDimStyle->setDimaltu(2);
	pDimStyle->setDimscale(100.0 * fontScale);
	pDimStyle->setDimasz(2.0);//箭头
	pDimStyle->setDimtxt(4.0);//文字
	pDimStyle->setDimblk(OD_T(""));
	pDimStyle->setDimblk1(OD_T(""));
	pDimStyle->setDimblk2(OD_T(""));
	pDimStyle->setDimcen(0);
	pDimStyle->setDimtih(false);
	pDimStyle->setDimtoh(true);
	pDimStyle->setDimsah(true);
	pDimStyle->setDimzin(8);
	pDimStyle->setDimtmove(2);
	pDimStyle->setDimatfit(0);
	pDimStyle->setDimtih(true);
	//pDimStyle->setDimjust(2);
}

/************************************************************************/
/* 功能：窗口缩放
/************************************************************************/
void CreateEnt::zoom_window(OdGePoint3d& pt1, OdGePoint3d& pt2)
{
	OdDbBaseDatabase* pRxDatabase = _pCmdCtx->baseDatabase();
	if (!pRxDatabase)
		throw OdError(eNoDatabase);

	IqViewSystem* iVS = getIViewSystem();
	if (!iVS)
		throw OdError(eInvalidView);

	IqView* iView = iVS->getView(pRxDatabase);
	if (!iView)
		throw OdError(eInvalidView);

	OdGsView* pGsView = iView->getActiveTopGsView();
	zoom_window(pt1, pt2, pGsView);

}

/************************************************************************/
/* 功能：窗口缩放
/************************************************************************/
void CreateEnt::zoom_window(OdGePoint3d& pt1, OdGePoint3d& pt2, OdGsView* pGsView)
{
	OdGeMatrix3d xWorldToEye = OdAbstractViewPEPtr(pGsView)->worldToEye(pGsView);
	pt1.transformBy(xWorldToEye);
	pt2.transformBy(xWorldToEye);
	OdGeVector3d eyeVec = pt2 - pt1;
	if (OdZero(eyeVec.x) || OdZero(eyeVec.y))
		return;

	OdGePoint3d newPos = pt1 + eyeVec / 2.;

	eyeVec.x = fabs(eyeVec.x);
	eyeVec.y = fabs(eyeVec.y);

	pGsView->dolly(newPos.asVector());

	double wf = pGsView->fieldWidth() / eyeVec.x;
	double hf = pGsView->fieldHeight() / eyeVec.y;

	pGsView->zoom(odmin(wf, hf));
}