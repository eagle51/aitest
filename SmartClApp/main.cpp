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
// main.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QDir> 
#include <QSysInfo>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>

#include "OdqApplication.h"
#include "OdqMainWindow.h"
#include "OdqAppProps.h"

#include "RxDynamicModule.h"
#include "DynamicLinker.h"

#include "ExStringIO.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdCommandStack.h" 
#include "DbHostAppServices.h"
#include <QApplication>
#include <winsock.h>
#include "SoftWareVerify.h"
#include "ASManager.h"

#if defined(_MSC_VER)
 #if _MSC_VER < 1600 // removed in VC 2010
  extern "C" _CRTIMP int __cdecl _set_sbh_threshold(size_t);
  inline void attuneHeap()
  {
    // http://forum.opendesign.com/showthread.php?t=5114&highlight=heap+performance
    // http://support.microsoft.com/?kbid=323635. 
    //
    // On Windows 2000 or Windows XP, repeated loading of large files in TD 
    // may cause performance to degrade. The cause of this degradation is that
    // the CRT small-block heap is not used by default on Windows 2000 or XP. 
    _set_sbh_threshold(1016);
  }
 #else
  #define attuneHeap()
 #endif
  inline void attuneLocale()
  {
    setlocale(LC_NUMERIC, "C"); // saving doubles to dxf
    setlocale(LC_TIME, ""); // correct strftime formatting
  }
#else
 #define attuneHeap()
 #define attuneLocale()
#endif

int main(int argc, char *argv[])
{
  attuneHeap();
  attuneLocale();
  ODA_ASSERT(sizeof(OdChar) == sizeof(wchar_t));

  //OdqApplication app(argc, argv); // contains services and initialize it
  OdSharedPtr<OdqApplication> pApp = new OdqApplication(argc, argv); // contains services and initialize it
  OdqApplication& app = *pApp.get();
  
  /*第0步：设置软件类型以及初始化的一些相关设置*/
  SoftWareVerifyInstance->setSoftwareInfo0(AboutSoftWare::SoftWare::SmartClearance, AboutSoftWare::Year::Year_2024);//设定软件类型以及年份
  ASMgr->readFromJson();//读取关于软件信息
  SoftWareVerifyInstance->setOverTimeStep0(2135, 12, 30);//软件过期时间，注意格式为：XXXX-XX-XX，会影响注册表加密以及配置文件加密，
  SoftWareVerifyInstance->setIsCheckRegedit0(true);//设置是否需要检测注册表加密
  SoftWareVerifyInstance->setIsCheckConfig0(true);//设置是否需要检测注册表加密

  /*第1步：关于软件信息以及授权检测的初始化设置，并验证深思加密初始化，验证成功后，VirBoxCheckerInstance->safeLoginFlag的值由false变为true*/
  if (!SoftWareVerifyInstance->checkVirBoxInitStep1())//检测深思加密是否可以被初始化
	  return 0;

  //第2步：检测深思加密许可ID，存在错误立即不能用，这个要提示警告，需要放到后面，防止定位
  bool successLogin = SoftWareVerifyInstance->checkVirBoxLicenseIDStep2();

  //第3步：第3步的相关验证放在主窗体代码中，分别为授权只读区验证以及多线程动态校验，详见主窗体初始化的代码
  OdSharedPtr<OdqMainWindow> pWindow = new OdqMainWindow();
  OdqMainWindow& window = *pWindow.get();
  window.showMaximized();
  if (!window.isValid())
    return 0;

  //第2步：检测错误的处理放在此处，立即不能用，直接返回，不做任何提示
  if (!successLogin)
  {
	  window.slotSetWidgetEditable(false);
	  return 0;
  }

  /*第4步：软件模块许可ID校验，设置软件子模块是否可用，根据各软件的特点按需调用*/
  window.moduleEnabled();

  /*第5步：检测注册表加密，该检测只有在SoftWareVerifyInstance->setIsCheckRegedit0(true)的时候才会起作用，
  此时会根据SoftWareVerifyInstance->setOverTimeStep0(2022,11,10)中设置的时间进行验证*/
  SoftWareVerifyInstance->checkRegedit5(&window);

  QString qsPropProfile = "qrc:/oda_qt_props.xml",
          qsMenuProfile = "qrc:/cui/odq_menu_base.cui",
          qsToolbarsProfile = "qrc:/cui/odq_toolbar_base.cui";

  if (   ::odrxDynamicLinker()->loadModule(OD_T("SmartClOpenGL")).get()
      || ::odrxDynamicLinker()->loadModule(OD_T("WinOpenGL.txv")).get()
      || ::odrxDynamicLinker()->loadModule(OD_T("BmpOpenGL.txv")).get()
     )
    qsPropProfile = "qrc:/oda_qt_props_caustic.xml";

  OdqAppProps* pAppProps = qobject_cast<OdqAppProps*>(app.getSingle(IqAppProps_iid));
  if (!pAppProps || !pAppProps->load(qsPropProfile))
  {
    ODA_FAIL();
  }


  IqConsole* iConsole = getIConsole();
  if (!iConsole)
  {
    ODA_FAIL();
    return 0;
  }

  bool bPersistentUndo = pAppProps->toBool(L"Vars/persistentundo", false); // INT-6233
  if (bPersistentUndo && ::odrxDynamicLinker()->loadApp(OdUndoHistory).isNull())
    bPersistentUndo = false;
  OdDbHostAppServices* pAppServices = OdDbHostAppServices::cast(getIApp()->getBaseHostAppServices()).get();
//  if (bPersistentUndo != pAppServices->getPersistentUndoMode())
//    pAppServices->setPersistentUndoMode(bPersistentUndo); // at start only

  OdInt16 nSnapModes = (OdInt16) pAppProps->toInt(L"Vars/osmode", 4133);
  if (nSnapModes != pAppServices->getOSMODE())
    pAppServices->setOSMODE(nSnapModes);

  // created by ACAD 2009
  // TODO // created by ACAD 2010
  //iConsole->postponeCommand("menu \"qrc:/oda_qt_base.cuix\"", false);
  //iConsole->postponeCommand(QString("menu \"%1\"").arg(qsMenuProfile), true, NULL, 100);//基础菜单
  //iConsole->postponeCommand(QString("cuiload \"%1\"").arg(qsToolbarsProfile), true, NULL, 100);//工具栏
  //if (getIRevSystem())
  //  iConsole->executeCommand("cuiload \"qrc:/cui/odq_toolbar_tigdb_tx.cui\"");

  iConsole->executeCommand("commandlinehide "); //隐藏控制台


  ::odrxDynamicLinker()->loadModule("DrawClearanceCircle.tx"); 
  //::odrxDynamicLinker()->loadModule(OdExCustObjsModuleName); //自定义模块
  //::odrxDynamicLinker()->loadModule(OdExCommandsModuleName); // "ExCommands.tx"
  ::odrxDynamicLinker()->loadModule(ExDimAssocModuleName); //"ExDimAssoc.tx"
  ::odrxDynamicLinker()->loadModule(OdGripPointsModuleName);
  ::odrxDynamicLinker()->loadModule(OdExFieldEvaluatorModuleName); //CORE-11630
  
  // APPLOAD
  QStringList lstArgs = app.arguments();
  if (!lstArgs.isEmpty())
	  lstArgs.removeFirst(); // skip arg[0] (is application path)
  // load Startup Suite
  OdEdCommand* pCommand = ::odedRegCmds()->lookupCmd(OD_T("APPLOAD")).get();
  if (pCommand)
  {
	  OdEdCommandContext* pCmdCtx = iConsole->getCommandContext();
	  ODA_ASSERT_ONCE(pCmdCtx);
	  if (pCmdCtx)
	  {
		  OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(OD_T("LOADSTARTUPSUITE")); // LOADCONTENTS
		  pCmdCtx->setArbitraryData(OD_T("APPLOAD/SUBCOMMAND"), pStringIO);

		  try
		  {
			  pCommand->execute(pCmdCtx);
			  bool isLdParam = false;
			  foreach(QString qsArg, lstArgs)
			  {
				  QString qsArgLow = qsArg.toLower();
				  if (!isLdParam && (qsArgLow == "/ld" || qsArgLow == "-ld"))
				  {
					  isLdParam = true;
					  continue;
				  }
				  if (!isLdParam)
					  continue;
				  isLdParam = false;

				  if (qsArg.size() >= 2 && qsArg.startsWith('\"') && qsArg.endsWith('\"'))
					  qsArg = qsArg.mid(1, qsArg.size() - 2);

				  foreach(QString qsPath, qsArg.split(';', QString::SkipEmptyParts))
				  {
					  if (qsPath.startsWith('\"'))
					  {
						  if (qsPath.size() >= 2 && qsPath.endsWith('\"'))
							  qsPath = qsPath.mid(1, qsPath.size() - 2);
						  else
						  {
							  ODA_FAIL_ONCE();
						  }
					  }
					  if (qsPath.isEmpty())
						  continue;

					  if (true)
					  {
						  pCmdCtx->userIO()->putString(app.toOdString(QObject::tr("Command-line switch /ld loading :")));

					  }
					  OdSmartPtr<ExStringIO> pLdIO = ExStringIO::create(getIApp()->toOdString(qsPath));
					  pCmdCtx->setArbitraryData(OD_T("APPLOAD/SUBCOMMAND"), pLdIO);
					  pCommand->execute(pCmdCtx);
					  pCmdCtx->setArbitraryData(OD_T("APPLOAD/SUBCOMMAND"), NULL);

				  } // end foreach qsPath
			  } // end foreach qsArg
		  }
		  catch (...)
		  {
			  ODA_FAIL_ONCE();
		  }
		  pCmdCtx->setArbitraryData(OD_T("APPLOAD/SUBCOMMAND"), NULL);
	  }
  }
  

  ////侧栏
  //::odrxDynamicLinker()->loadModule(OD_T("OdaQtPropSystem.dll"));

  //// 处理参数
  // 	bool isBParam = false,
  // 		isVParam = false,
  // 		isOtherParam = false;
  // 	QStringList lstOtherParams;
  // 	lstOtherParams << "/s" << "-s" << "/ld" << "-ld"; // << "/v" << "-v";
  // 	bool bArgOrSwitchFound = false;
  // 	foreach(QString qsArg, lstArgs)
  // 	{
  // 		QString qsArgLow = qsArg.toLower();
  // 		if (!isBParam && !isVParam && (qsArgLow == "/b" || qsArgLow == "-b"))
  // 		{
  // 			isBParam = true;
  // 			continue;
  // 		}
  // 		if (!isBParam && !isVParam && (qsArgLow == "/v" || qsArgLow == "-v"))
  // 		{
  // 			isVParam = true;
  // 			continue;
  // 		}
  // 		if (!isBParam && !isVParam)
  // 		{
  // 			if (isOtherParam)
  // 				isOtherParam = false;
  // 			else if (lstOtherParams.contains(qsArgLow))
  // 				isOtherParam = true;
  // 			else
  // 			{
  // 				bArgOrSwitchFound = true;
  // 				iConsole->postponeCommand(QString("open \"%1\"").arg(qsArg), true, NULL, 100);
  // 			}
  // 			continue;
  // 		}
  // 		if (isVParam)
  // 		{
  // 			isVParam = false;
  // 			if (bArgOrSwitchFound)
  // 				iConsole->postponeCommand(QString("view restore %1").arg(qsArg), true, NULL, 100);
  // 			continue;
  // 		}
  // 		isBParam = false;
  // 
  // 		if (qsArg.size() >= 2 && qsArg.startsWith('\"') && qsArg.endsWith('\"'))
  // 			qsArg = qsArg.mid(1, qsArg.size() - 2);
  // 
  // 		foreach(QString qsPath, qsArg.split(';', QString::SkipEmptyParts))
  // 		{
  // 			if (qsPath.isEmpty())
  // 				continue;
  // 
  // 			bArgOrSwitchFound = true;
  // 			iConsole->postponeCommand(QString("script %1").arg(qsPath), true, NULL, 100);
  // 
  // 		} // end foreach qsPath
  // 	} // end foreach qsArg

  // 状态栏

#ifdef QT_DEBUG
  app.toStatusBar(QObject::tr("Please wait... (cfg-folder is \"%1\")").
	  arg(QDir::toNativeSeparators(QDir::homePath() + "/ODA")), 3000);
#else
  app.toStatusBar(QObject::tr("Please wait..."), 200);
#endif
  window.show();

  int res = 0;
  res = app.exec();
  pWindow = NULL;
  pApp = NULL;

  /*第999步：退出深思授权许可登录，清空深思的初始化内容，此处不执行的话会影响网络狗多节点的占用*/
  SoftWareVerifyInstance->logCleanEnd999();	//清除登录

  return res;
  //  if (bPersistentUndo != pAppServices->getPersistentUndoMode())
  //    pAppServices->setPersistentUndoMode(bPersistentUndo); // at start only
}

