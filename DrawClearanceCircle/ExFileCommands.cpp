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

// ExFileCommands.cpp: implementation of "File" group commands:
//  - DXFIN
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////
// DXFIN - loads a .dxf file into existing database
//////////////////////////////////////////////////////////////////////

void _DxfIn_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdString fname = pIO->getFilePath(OD_T("Enter .dxf file name:"),
                                       OdEd::kGfpForOpen,
                                       OD_T("Select .dxf file"),
                                       OD_T("dxf"),
                                       OdString::kEmpty,
                                       OD_T("DXF files (*.dxf)|*.dxf|"));
  pDb->readFile(fname);
}

