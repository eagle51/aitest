#include "OdaCommon.h" 
#include "CreateEnt.h"
#include "DbLine.h"
#include "DbBlockTableRecord.h"
#include "OdqInterfaces.h"
#include "Ed\EdCommandContext.h"
#include "..\SmartFdConsole\CommandContextImpl.h"
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


 OdDbObjectId CreateEnt::addToModelSpace(OdDbEntity *entity)
{
	// 添加到模型空间
	IqConsole* iConsole = getIConsole();
	OdEdCommandContext* pCmdCtx = iConsole->getCommandContext();
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabase* pDatabase = pDbCmdCtx->database();
	OdDbBlockTableRecordPtr pSpace = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
	return pSpace->appendOdDbEntity(entity);
}

 OdDbObjectId CreateEnt::dLine(const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, int colorIndex/* = 1*/)
{
	
	OdDbLinePtr pLine = OdDbLine::createObject();
	pLine->setStartPoint(ptStart);
	pLine->setEndPoint(ptEnd);
	pLine->setColorIndex(colorIndex);
	return addToModelSpace(pLine);
}

 OdDbObjectId CreateEnt::d2dPolyLine(const OdGePoint2dArray& ptSet, bool isClosed/* = false*/, int colorIndex/* = 1*/)
{
	OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
	pPolyline->setClosed(isClosed);
	pPolyline->setColorIndex(colorIndex);
	for (int i = 0; i < int(ptSet.size()); ++i)
	{
		pPolyline->addVertexAt(i, ptSet.at(i));
	} 
	return addToModelSpace(pPolyline);
}

 OdDbObjectId CreateEnt::dCircle(const OdGePoint3d& ptCenter, double radius, int colorIndex/* = 2*/)
{
	OdDbCirclePtr pCircle = OdDbCircle::createObject();
	pCircle->setColorIndex(colorIndex);
	pCircle->setCenter(ptCenter);
	pCircle->setRadius(radius);
	return addToModelSpace(pCircle);
}

 OdDbObjectId CreateEnt::dText(const OdGePoint3d& point3d, const OdString& text, double height/* = 10*/, int colorIndex/* = 2*/)
{
	OdDbTextPtr pText = OdDbText::createObject();
	pText->setPosition(point3d);
	pText->setTextString(text);
	pText->setColorIndex(colorIndex);
	pText->setHeight(height);
	return addToModelSpace(pText);
}

OdDbObjectId CreateEnt::d2dSqure(const OdGePoint3d& ptCenter, double lenght, int colorIndex /*= 1*/)
{
	OdGePoint2dArray ptSet;
	ptSet.append(OdGePoint2d(ptCenter.x - lenght / 2, ptCenter.y - lenght / 2));
	ptSet.append(OdGePoint2d(ptCenter.x - lenght / 2, ptCenter.y + lenght / 2));
	ptSet.append(OdGePoint2d(ptCenter.x + lenght / 2, ptCenter.y + lenght / 2));
	ptSet.append(OdGePoint2d(ptCenter.x + lenght / 2, ptCenter.y - lenght / 2));
	return d2dPolyLine(ptSet, true);
}

OdDbObjectId CreateEnt::dArc(const OdGePoint3d& ptCenter, double radius, double startAngle, double endAngel, int colorIndex /*= 2*/)
{
	OdDbArcPtr pArc = OdDbArc::createObject();
	pArc->setColorIndex(colorIndex);
	pArc->setCenter(ptCenter);
	pArc->setRadius(radius);
	pArc->setStartAngle(startAngle);
	pArc->setEndAngle(endAngel);
	return addToModelSpace(pArc);
}

OdDbObjectId CreateEnt::dAlignedDimension(const OdGePoint3d& dimLinePt, const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, OdDbObjectId dimStyleId)
{
	OdDbAlignedDimensionPtr pDimension = OdDbAlignedDimension::createObject();
	pDimension->setXLine1Point(ptStart);
	pDimension->setXLine2Point(ptEnd);

	pDimension->setDimLinePoint(dimLinePt);
	pDimension->useDefaultTextPosition();
	pDimension->setDimscale(5.0);
	pDimension->setExtLineFixLenEnable(true);
	pDimension->setExtLineFixLen(0.0);
	
	pDimension->setDimensionText(OD_T("12900"));
	pDimension->setDimsah(true);

//	OdDbDatabase* pDatabase = getDataBase();
	pDimension->setDimblk1(dimStyleId);
	pDimension->setDimblk2(dimStyleId);
	//pDimension->setDimblk2(OdDmUtil::getArrowId(OD_T("_OBLIQUE"), pDatabase));

//  	OdDbDimStyleTableRecordPtr pDimStyle = OdDbDimStyleTableRecord::createObject();
// 	pDimStyle->setDimsah(true);
	
//   	OdDbDimStyleTablePtr pTable = pDatabase->getDimStyleTableId().safeOpenObject(OdDb::kForWrite);
// 	OdDbSymbolTableIteratorPtr itrTable = pTable->newIterator();
// 	for (itrTable->start(); !itrTable->done(); itrTable->step())
// 	{
// 		OdDbDimStyleTableRecordPtr pDimStyle = (OdDbDimStyleTableRecordPtr)itrTable->getRecord();
// 		qDebug() << "\"" << pDimStyle->getName() << "\", " << "\n";
// 		
// 	}
//     OdDbObjectId dimStyleId = pTable->add(pDimStyle);
//     pDimStyle->setDimblk1(OD_T("_OBLIQUE"));
	
	//pDimension->setDimensionStyle(dimStyleId);OdDmUtil::getArrowId(L"_CLOSEDBLANK", pDb);
	
	//pDimension->setDimstyleData(dimStyleId);
	return addToModelSpace(pDimension);
}

OdDbObjectId CreateEnt::addHiddenLineType()
{
	IqConsole* iConsole = getIConsole();
	OdEdCommandContext* pCmdCtx = iConsole->getCommandContext();
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabase* pDatabase = pDbCmdCtx->database();
	OdDbLinetypeTablePtr pLinetypes = pDatabase->getLinetypeTableId().safeOpenObject(OdDb::kForWrite);

	OdDbLinetypeTableRecordPtr pLinetype = OdDbLinetypeTableRecord::createObject();
	
	pLinetype->setName(OD_T("Hidden"));
	pLinetype->setComments(OD_T("- - - - - - - "));
	
	pLinetype->setNumDashes(2);
	pLinetype->setPatternLength(0.1875);
	pLinetype->setDashLengthAt(0, 0.125);
	pLinetype->setDashLengthAt(1, -0.0625);
	return pLinetypes->add(pLinetype);
}

OdDbDatabase* CreateEnt::getDataBase()
{
	IqConsole* iConsole = getIConsole();
	OdEdCommandContext* pCmdCtx = iConsole->getCommandContext();
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	return pDbCmdCtx->database();
}

