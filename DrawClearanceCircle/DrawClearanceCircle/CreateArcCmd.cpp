#include "StdAfx.h"
#include "DbLine.h"
#include <list>
#include "DbCircle.h"
#include <map>
#include "DbArc.h"
#include "CreateArcCmd.h"
#include "CommandContextImpl.h"

//#include <QtCore>
//#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
//#include <QtGui>
//#else
//#include <QtWidgets>
//#endif
//#include <QDir> 
//#include <QSysInfo>
//#include <QFile>
//#include <QMessageBox>


//#include <QtCore>
//
//#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
//#include <QtGui>
//#else
//#include <QtWidgets>
//#endif
//#include <QDir> 
//#include <QSysInfo>
//#include <QFile>
#include <QMessageBox>

using namespace std;


void CreateArcCmd::breakAtIntersection(std::list<OdDbObjectId> listCircle,
	std::list<OdDbArcPtr> & listArc)
{
	std::list<OdDbObjectId>::iterator iterCircleID = listCircle.begin();
	OdGePoint3dArray intersectPoints;
	std::list<OdDbObjectId>::iterator iterCircleIDNext = listCircle.begin();

	for (; iterCircleID != listCircle.end(); iterCircleID++)
	{
		OdDbCirclePtr pCircleEnt = iterCircleID->safeOpenObject(OdDb::kForRead);
		OdGePoint3d centerPoint = pCircleEnt->center();
		double dRadius = pCircleEnt->radius();
		//找和所有圆的交点
		for (; iterCircleIDNext != listCircle.end(); iterCircleIDNext++)
		{
			if (*iterCircleIDNext == *iterCircleID)
			{
				continue;
			}

			OdDbCirclePtr pCircleEntNext = iterCircleIDNext->safeOpenObject(OdDb::kForRead);
			OdGePoint3dArray points;
			pCircleEnt->intersectWith(pCircleEntNext, OdDb::kOnBothOperands, points);
			int nPointSzie = points.size();
			if (nPointSzie != 0)
			{
				intersectPoints.insert(intersectPoints.end(), points.begin(), points.end());
			}
		}
		//圆上的交点排序
		int nPointSize = intersectPoints.size();
		for (int iPoint = 0; iPoint < nPointSize; iPoint++)
		{
			double iRadians = 0.0;
			pCircleEnt->getParamAtPoint(intersectPoints[iPoint], iRadians);

			for (int jPoint = iPoint + 1; jPoint < nPointSize; jPoint++)
			{
				double jRadians = 0.0;
				pCircleEnt->getParamAtPoint(intersectPoints[jPoint], jRadians);
				if (iRadians > jRadians)
				{
					intersectPoints.swap(iPoint, jPoint);
				}
			}
		}

		if (intersectPoints.size() == 0)
		{
			return;
		}

		//创建圆弧
		OdGePoint3dArray::iterator iterPoint = intersectPoints.begin();
		OdGePoint3dArray::iterator iterPointNext = iterPoint;
		iterPointNext++;
		for (; (iterPoint != intersectPoints.end(),
			iterPointNext != intersectPoints.end()); iterPoint++, iterPointNext++)
		{
			OdDbArcPtr pArc = OdDbArc::createObject();
			pArc->setCenter(centerPoint);
			pArc->setRadius(dRadius);
			double iRadians = 0.0;
			double jRadians = 0.0;
			pCircleEnt->getParamAtPoint(*iterPoint, iRadians);
			pCircleEnt->getParamAtPoint(*iterPointNext, jRadians);
			pArc->setStartAngle(iRadians);
			pArc->setEndAngle(jRadians);
			listArc.insert(listArc.end(), pArc);
			//pSpace->appendOdDbEntity(pArc);

		}

	}



}

void CreateArcCmd::CalArcExtentPair(std::list<OdDbArcPtr> listArc,
	list<std::pair<OdDbObjectId, OdGeExtents3d> > & mapAllArc)
{
	//std::list<OdDbObjectId>::iterator iterCircleID = listCircle.begin();

	//for (; iterCircleID != listCircle.end(); iterCircleID++)
	//{
	//	OdDbCirclePtr pCircleEnt = iterCircleID->safeOpenObject(OdDb::kForWrite);
	//	OdGeExtents3d extendsCircle;

	//	pCircleEnt->getGeomExtents(extendsCircle);
	//	listAllCirclePair.insert(listAllCirclePair.end(),
	//		std::pair<OdDbObjectId, OdGeExtents3d>(*iterCircleID, extendsCircle));
	//}

}

void CreateArcCmd::delContainArc(list<std::pair<OdDbObjectId, OdGeExtents3d> > listAllCirclePair,
	list<std::pair<OdDbObjectId, OdGeExtents3d> > & mapAllArc)
{

}

void CreateArcCmd::dimArcRadius(list<std::pair<OdDbObjectId, OdGeExtents3d> > mapAllArc)
{

}
void CreateArcCmd::_CreateArc_func(OdEdCommandContext* pCmdCtx)
{
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabasePtr pDb = pDbCmdCtx->database();
	OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

	OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
	OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

	//获取所有圆
	std::list<OdDbObjectId> listCircle;//所有的圆
	std::list<OdDbArcPtr> listArc;//所有的弧

	OdDbObjectIteratorPtr iterEntity = pSpace->newIterator();
	iterEntity->start();
	while (!iterEntity->done())
	{
		OdDbEntity *pDbEntity = iterEntity->entity(OdDb::kForRead);
		if (pDbEntity->isA()->isEqualTo(OdDbCircle::desc()))
		{
			listCircle.push_back(iterEntity->objectId());
		}

		iterEntity->step();
	}

	list<std::pair<OdDbObjectId, OdGeExtents3d> > listAllCirclePair;//所有的圆及其包围框
	list<::pair<OdDbObjectId, OdGeExtents3d> > listAllArcPair;//所有的圆弧及其包围框
	std::list<OdDbObjectId>::iterator iterCircleID = listCircle.begin();

	for (; iterCircleID != listCircle.end(); iterCircleID++)
	{
		OdDbCirclePtr pCircleEnt = iterCircleID->safeOpenObject(OdDb::kForWrite);
		OdGeExtents3d extendsCircle;
		pCircleEnt->getGeomExtents(extendsCircle);
		listAllCirclePair.insert(listAllCirclePair.end(),
			std::pair<OdDbObjectId, OdGeExtents3d>(*iterCircleID, extendsCircle));
	}

	breakAtIntersection(listCircle, listArc);
	CalArcExtentPair(listArc, listAllArcPair);
	delContainArc(listAllCirclePair, listAllArcPair);

	//创建圆弧
	std::list<OdDbArcPtr>::iterator iterArc = listArc.begin();//所有的弧
	for (iterArc; iterArc != listArc.end(); iterArc++)
	{
		pSpace->appendOdDbEntity(*iterArc);
	}
	dimArcRadius(listAllArcPair);



	//////////////////////////////////////////////////////////////////////////

	//QMessageBox::information(NULL,
	//	"Title",
	//	"Test",
	//	QMessageBox::Yes | QMessageBox::No,
	//	QMessageBox::Yes);




	//OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));

	//OdString strNext_U(L"Specify next point or [Undo]:");
	//OdString strKeywords_U(L"Undo");

	//OdString strNext_CU(L"Specify next point or [Undo/Close]:");
	//OdString strKeywords_CU(L"Undo Close");
	//OdGePoint3d ptFirst, ptStart, ptEnd;
	//int nLines = 0;

	//for (bool bFinished = false; !bFinished;)
	//{
	//	ptStart = ptFirst = pIO->getPoint(L"Specify first point:");

	//	for (;;)
	//	{
	//		try
	//		{
	//			if (nLines >= 2)
	//			{
	//				ptEnd = pIO->getPoint(strNext_CU, OdEd::kGptRubberBand | OdEd::kInpThrowEmpty, NULL, strKeywords_CU);
	//			}
	//			else
	//			{
	//				ptEnd = pIO->getPoint(strNext_U, OdEd::kGptRubberBand | OdEd::kInpThrowEmpty, NULL, strKeywords_U);
	//			}
	//		}
	//		catch (const OdEdKeyword &kwd)
	//		{
	//			if (kwd.keywordIndex() == 1)
	//			{ //Close
	//				ptEnd = ptFirst;
	//				bFinished = true;
	//			}
	//			else
	//			{ //Undo
	//				if (nLines == 0)
	//				{ //Undo first point
	//					break;
	//				}
	//				OdDbObjectId idLast = oddbEntLast(pDb);
	//				OdDbLinePtr pLine = idLast.openObject(OdDb::kForWrite);
	//				if (!pLine.isNull())
	//				{
	//					ptStart = pLine->startPoint();
	//					pLine->erase();
	//					pIO->setLASTPOINT(ptStart);
	//					--nLines;
	//				}
	//				else
	//				{
	//					ODA_FAIL();
	//				}
	//				continue;
	//			}
	//		}
	//		catch (const OdEdCancel&)
	//		{
	//			return;
	//		}

	//		OdDbLinePtr pNewLine = OdDbLine::createObject();
	//		pNewLine->setDatabaseDefaults(pDb);
	//		pNewLine->setNormal(vNormal);
	//		pNewLine->setStartPoint(ptStart);
	//		pNewLine->setEndPoint(ptEnd);
	//		pSpace->appendOdDbEntity(pNewLine);
	//		ptStart = ptEnd;
	//		++nLines;

	//		if (bFinished)
	//			return;
	//	}
	//}

}