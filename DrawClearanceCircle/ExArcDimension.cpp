/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DbArcDimension.h"

void _DimArc_func(OdEdCommandContext* pCmdCtx) {
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabase* pDb = pDbCmdCtx->database();
	OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
	OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

	OdGePoint3d center   = pIO->getPoint("Set arc center point");
	OdGePoint3d start    = pIO->getPoint("Set start point");
	OdGePoint3d end      = pIO->getPoint("Set end point");
	OdGePoint3d arcPoint = pIO->getPoint("Set dimension point");

	OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
	OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

	OdDbArcDimensionPtr pDimension = OdDbArcDimension::createObject();
	pSpace->appendOdDbEntity(pDimension);

	pDimension->setCenterPoint(center);
	pDimension->setArcPoint(arcPoint);
	pDimension->setXLine1Point(start);
	pDimension->setXLine2Point(end);

	pDimension->setArcSymbolType(1);
}
