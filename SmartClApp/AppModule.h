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
//
// AppModule.h
//

#ifndef ODA_QT_APP_MODULE_H_
#define ODA_QT_APP_MODULE_H_

#include "OdaCommon.h"
#include "ExDynamicModule.h" // #include "RxDynamicModule.h" 
#include "StaticRxObject.h"
#include "Ed/EdCommandStack.h"

//////////////////////////////////////////////////////////////////////////
//#pragma comment(linker, "/include:?getInstance@DataBase@SmartCl@@SAPAV12@XZ")
//#pragma comment(linker, "/include:?_Instance@DataBase@SmartCl@@0PAV12@A")

#define ODRX_MODULE_NAME_W L"OdqAppModule"

#define ODRX_CMD_ENTRY(cmdName, name, impl) \
    ODRX_DECLARE_COMMAND(cmdName, OdqCmd##name, OdEdCommand, impl);

#include "AppCommands.h"

//////////////////////////////////////////////////////////////////////////

class OdqAppModule : public OdRxModule
{
protected:
  // at the start to register objects and commands
  virtual void initApp();
  // at the end to unregister objects and commands
  virtual void uninitApp();
  
  OdqAppModule()
  {
    #define ODRX_CMD_ENTRY(cmdName, name, impl) m_cmd##name.m_pModule = this;
    #include "AppCommands.h"
  }
  virtual ~OdqAppModule()
  {
  }

  /////// Commands ///////

  //OdStaticRxObject<OdqCmdOpen> m_cmdOpen;
  #define ODRX_CMD_ENTRY(cmdName, name, impl) OdStaticRxObject<OdqCmd##name> m_cmd##name;
  #include "AppCommands.h"

  friend class OdqCmd_CuiLoad;
  friend class OdqCmd_Print;
  //friend class OdqCmd_PageSetup;
};
//typedef OdSmartPtr<OdqAppModule> OdqAppModulePtr;

#endif  // ODA_QT_APP_MODULE_H_
