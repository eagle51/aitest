//#include "DrawClearanceCircle.h"
//
//DrawClearanceCircle::DrawClearanceCircle()
//{
//}

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

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif

/************************************************************************/
/* CommandsDLL.cpp: Defines the entry points for the DLL           */
/************************************************************************/
#include "OdaCommon.h"
#include "RxSysRegistry.h"
#include <Ed/EdCommandContext.h>
#include "RxDynamicModule.h"
#include "DrawClearanceCircleModule.h"

#define CMD_DEF_CLASS(CmdName, GroupName) \
  extern void _##CmdName##_func(OdEdCommandContext* pCmdCtx);\
  const OdString _##CmdName##_cmd::globalName() const { return OD_T(#CmdName); }\
  const OdString _##CmdName##_cmd::groupName() const { return GroupName; }

#define CMD_DEF(CmdName, GroupName) \
  extern void _##CmdName##_func(OdEdCommandContext* pCmdCtx);\
  const OdString _##CmdName##_cmd::globalName() const { return OD_T(#CmdName); }\
  const OdString _##CmdName##_cmd::groupName() const { return GroupName; }\
  void _##CmdName##_cmd::execute(OdEdCommandContext* pCmdCtx) { _##CmdName##_func(pCmdCtx); }

#include "DbCmdDef.h"

#undef CMD_DEF_CLASS
#undef CMD_DEF


ODRX_DEFINE_DYNAMIC_MODULE(DrawClearanceCircleModule);


void DrawClearanceCircleModule::initApp()
{
	::odrxDynamicLinker()->loadApp(OdRecomputeDimBlockModuleName, false);
	OdEdCommandStackPtr pCommands = odedRegCmds();

#define CMD_DEF(CmdName, GroupName) \
  pCommands->addCommand(&m_##CmdName##_cmd);
#include "DbCmdDef.h"
#undef CMD_DEF

	pCommands->addCommand(&m_ClipPdfUnderlayCommand);
	pCommands->addCommand(&m_ClipDwfUnderlayCommand);
	pCommands->addCommand(&m_ClipDgnUnderlayCommand);
}

void DrawClearanceCircleModule::uninitApp()
{
	OdEdCommandStackPtr pCommands = odedRegCmds();

#define CMD_DEF(CmdName, GroupName) \
  pCommands->removeCmd(GroupName, OD_T(#CmdName));
#include "DbCmdDef.h"
#undef CMD_DEF

	pCommands->removeCmd(&m_ClipPdfUnderlayCommand);
	pCommands->removeCmd(&m_ClipDwfUnderlayCommand);
	pCommands->removeCmd(&m_ClipDgnUnderlayCommand);
}
