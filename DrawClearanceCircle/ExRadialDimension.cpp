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
#include "DbRadialDimension.h"

void _RadialDim_func(OdEdCommandContext* pCmdCtx) {
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabase* pDb           = pDbCmdCtx->database();
	OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
	OdSmartPtr<OdDbUserIO> pIO  = pDbCmdCtx->userIO();

	OdGePoint3d centerPnt = pIO->getPoint("Set center point");
	OdGePoint3d chordPnt  = pIO->getPoint("Set chord point");

	OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
	OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

	OdDbRadialDimensionPtr pDimension = OdDbRadialDimension::createObject();
	pDimension->setDatabaseDefaults(pDb);

	pSpace->appendOdDbEntity(pDimension);

	pDimension->setCenter(centerPnt);
	pDimension->setChordPoint(chordPnt);
	pDimension->setLeaderLength(0.125);
	pDimension->useDefaultTextPosition();
}
