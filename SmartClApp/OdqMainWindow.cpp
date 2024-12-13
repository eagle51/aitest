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
// OdqMainWindow.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QDir>
#include <QSettings>
#include <QMetaProperty>

#include "OdqApplication.h" 
#include "OdqMainWindow.h"
#include "OdqCustomization.h"
#include "OdqClipData.h"
#include "Services.h"
#include "AppModule.h"
#include "ExtDbModule.h"

#include "RxVariantValue.h"
#include "TDVersion.h"

//////////////////////////////////////////////////////////////////////////

//#define OD_ANDROID_TRACEDEFS

#ifdef OD_ANDROID_TRACEDEFS
#ifdef ANDROID
#include <android/log.h>
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  "OdqMainWindow", __VA_ARGS__)
#else
#undef OD_ANDROID_TRACEDEFS
#endif
#endif
#include "mainFormFiles/ClListWidget.h"
#include "QAction"
#include "QString"
#include "QMenu"
#include "QMainWindow"
#include "QMenuBar"
#include "mainFormFiles\NewProject_Dlg.h"
#include "qfileinfo.h"

#include "UIAutilityFiles\UIAutility.h"
#include "mainFormFiles/AddClByType_Dlg.h"
#include "../SmartClViewSystem/OdqView.h"
#include "Auxiliary.h"
#include "ClFactory.h"
#include "SoftWareBaseInfo.h"
#include "SoftWareVerify.h"
#include "VirBox/VirBoxChecker.h"
#include "About_Dlg.h"
#include "SettingsDlg.h"
#include "ASManager.h"
#include "UpDateInfo_Dlg.h"


ODRX_DEFINE_STATIC_MODULE(OdqAppModule)

//////////////////////////////////////////////////////////////////////////
using namespace SmartCl;
OdqMainWindow::OdqMainWindow()
  : m_bValid(false)
  , m_pAppModule(NULL)
  , m_bDragging(false)
{
  setWindowTitle(getIApp()->getAppName());

#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
  QPixmap pixmap(QLatin1String(":odalogo.png"));
  ODA_ASSERT_ONCE(!pixmap.isNull());
  pixmap.setMask(pixmap.createHeuristicMask());
  setWindowIcon(QIcon(pixmap));
#endif

  setAcceptDrops(true);



  // create QMdiArea widget 
  // - provides an area in which MDI windows are displayed
  QMdiArea* pMdiArea = new QMdiArea(this);
  pMdiArea->setObjectName("MdiArea");
  pMdiArea->setAttribute(Qt::WA_DeleteOnClose, true);

  QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
  _qssType = QssType(settings.value("skin", 2).toInt()); // 初始默认为暗黑色

  _coreDbPt = SmartCl::DataBase::getInstance();//内核数据库指针存放，便于其他项目调用
  _coreDbPt->setVersion(ASMgr->getSoftWareBaseInfo()->getVersion().toLocal8Bit().toStdString());
  _coreDbPt->setVersionTime(ASMgr->getSoftWareBaseInfo()->getVersionTime().toLocal8Bit().toStdString());
  if (ASMgr->getSoftWareBaseInfo()->getSoftwareType_AS() == AboutSoftWare::SoftwareType_AS::General)
	  _coreDbPt->setSoftWareType(SmartCl::SoftWareType::General);
  else if (ASMgr->getSoftWareBaseInfo()->getSoftwareType_AS() == AboutSoftWare::SoftwareType_AS::FoShanYuan)
	  _coreDbPt->setSoftWareType(SmartCl::SoftWareType::FoShanYuan);

  /*第3步：验证深思加密的只读区，此处验证成功后，
  VirBoxCheckerInstance->enlargeFactor 的值由1000.0变为1.0
  VirBoxCheckerInstance->virBoxPi 的值由1.0变为3.14159265358979*/
  _virBoxCheckerInstance = VirBoxCheckerInstance;
  SoftWareVerifyInstance->checkVirBoxLicenseROM3();

  setCentralWidget(pMdiArea);
  initDockWidget(pMdiArea);
  initAppModule(); // uninitAppModule() will be executed via closeEvent
  m_bValid = true;

  /*第3步：启动多线程，实时检查狗是否存在，检测成功后，
  VirBoxCheckerInstance->canReadFlag 的值由false变为true
  VirBoxCheckerInstance->canWriteFlag 的值由false变为true
  VirBoxCheckerInstance->canSolveFlag 的值由false变为true*/
  QObject::connect(SoftWareVerifyInstance, SIGNAL(emitVerifyResToWgt(bool)), this, SLOT(slotSetWidgetEditable(bool)));
  SoftWareVerifyInstance->start();

  creatAction();
  loadSkinQss();

}

OdqMainWindow::~OdqMainWindow()
{
  ODA_ASSERT(!m_pAppModule);
}


OdRxModule* OdqMainWindow::getAppModule()
{
  ODA_ASSERT(m_pAppModule);
  return m_pAppModule;
}

bool OdqMainWindow::initAppModule()
{
  qApp->setActiveWindow(this); // app.setActiveWindow(&window);

  // do not hold console here via OdRxModulePtr as member 
  OdRxModule* pConsole = ::odrxDynamicLinker()->loadModule(OD_T("SmartClConsole")).get(); // silent loading
  if (!pConsole)
  {
    // unfortunately toPlatformFileName is inaccessible for external use now
    //OdString sConsolePath;
    //app.getServices()->toPlatformFileName(OD_T("SmartClConsole.dll"), &sConsolePath);
    //QString qsConsolePath = app.toQString(sConsolePath);
    QString qsConsolePath = "SmartClConsole.tx";
    QMessageBox::critical(NULL, QObject::tr("Fatal error"),
      QObject::tr("File \"%1\" was not found.").arg(qsConsolePath));
    return false;
  }

  // preloading all modules with "ObjectDBX Classes"
  //::odrxDynamicLinker()->loadModule(OdDynBlocksModuleName);
  //::odrxDynamicLinker()->loadModule(DbConstraintsModuleName);
  //::odrxDynamicLinker()->loadModule(Od3DSolidHistoryTxModuleName);
#if defined(_DEBUG) && defined(ODA_WINDOWS) && !defined(_WINRT)
  if (false)
    ::odrxDynamicLinker()->loadModule(OdOleSsItemHandlerModuleName);
  else
#endif
    ::odrxDynamicLinker()->loadModule(OdOlePlatformItemHandlerModuleName);

  ODA_ASSERT(!m_pAppModule);
  m_pAppModule = ODRX_STATIC_MODULE_ENTRY_POINT(OdqAppModule)(OD_T("OdqAppModule"));
  m_pAppModule->initApp();

  // (void)
  // by OdaMfcApp
  new QShortcut(Qt::SHIFT + Qt::Key_C, this, SLOT(revertConsoleVisibility()));
  
  // by ACAD
  // Ctrl+9 shows/hides Command line (2006+)
  new QShortcut(Qt::CTRL + Qt::Key_9, this, SLOT(revertConsoleVisibility()));

  //Ctrl+Q quits AutoCAD (2004+)  
  //new QShortcut(QKeySequence(tr("Ctrl+Q", "File|Exit")), this);

  new QShortcut(Qt::Key_F1, this, SLOT(showHelp()));

  return true;
}

void OdqMainWindow::uninitAppModule()
{
  if (!m_pAppModule)
    return;

  m_pAppModule->uninitApp();
  m_pAppModule->deleteModule();
  m_pAppModule = NULL;

  // it's here to save valid visibility of console palette via QSettings
  if (!::odrxDynamicLinker()->unloadModule(OD_T("SmartClConsole")))
  {
    ODA_FAIL_ONCE();
  }
}

bool OdqMainWindow::isValid() const
{
  return m_bValid;
}

bool OdqMainWindow::isClosed() const // by closeEvent
{
  return !m_pAppModule;
}

//OdqServices* OdqMainWindow::getServices() const
//{
//  static OdqServices* s_pServices = NULL;
//  if (!s_pServices)
//  {
//    OdqApplication* pApp = (OdqApplication*) ::getIApp();
//    s_pServices = pApp->getServices();
//    ODA_ASSERT_ONCE(s_pServices);
//  }
//
//  return s_pServices;
//}

void OdqMainWindow::saveWidgetPlacement(QWidget* pWidget, 
                                        const QString cqsKey) const // = "" // for QMainWindow
{
  if (!pWidget)
    return;

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  QString qsKey(cqsKey);
  if (!qsKey.isEmpty())
    qsKey += "/";
  else
    ODA_ASSERT_ONCE(pWidget == this);

  QRect rect = pWidget->geometry();
  settings.setValue(qsKey + "w", rect.width());
  settings.setValue(qsKey + "h", rect.height());
  settings.setValue(qsKey + "x", rect.x());
  settings.setValue(qsKey + "y", rect.y());

  settings.endGroup();
}

void OdqMainWindow::restoreWidgetPlacement(QWidget* pWidget, 
                                           const QString cqsKey,  // = "" // for QMainWindow
                                           int widthDefault, // = 600
                                           int heightDefault) const // = 200
{
  if (!pWidget)
    return;

  QDesktopWidget* pDesktop = NULL;
  QSize sizeParentWnd = size();
  QPoint posParentWnd = pos();

  QString qsKey(cqsKey);
  if (!cqsKey.isEmpty())
    qsKey += "/";
  else
  {
    ODA_ASSERT_ONCE(pWidget == this);

    pDesktop = QApplication::desktop();
    sizeParentWnd = pDesktop->screenGeometry().size(); // primary screen
    posParentWnd = QPoint();
  }

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  QSize size(settings.value(qsKey + "w", widthDefault).toInt(), 
             settings.value(qsKey + "h", heightDefault).toInt());

  QPoint pos((sizeParentWnd.width() - size.width()) / 2,
             (sizeParentWnd.height() - size.height()) / 2);
  pos += posParentWnd;

  pos.setX(settings.value(qsKey + "x", pos.x()).toInt());
  pos.setY(settings.value(qsKey + "y", pos.y()).toInt());

  settings.endGroup();

  if (cqsKey.isEmpty())
  {
    ODA_ASSERT_ONCE(pDesktop);
    QSize sizeParentVirtualWnd = pDesktop->geometry().size(); // full virtual size of screens

    if (   size.width() < 400 || size.height() < 300 || pos.x() < 0 || pos.y() < 0
        || (pos.x() + size.width()) > sizeParentVirtualWnd.width() 
        || (pos.y() + size.height()) > sizeParentVirtualWnd.height())
    {
      // restore default application placement
      ODA_ASSERT_ONCE(pDesktop);
      sizeParentWnd = pDesktop->screenGeometry().size(); // primary screen
      size.setWidth(widthDefault);
      size.setHeight(heightDefault);
      pos.setX((sizeParentWnd.width() - widthDefault) / 2);
      pos.setY((sizeParentWnd.height() - heightDefault) / 2);
    }
  }

  pWidget->setGeometry(pos.x(), pos.y(), size.width(), size.height());
}

bool OdqMainWindow::saveQObjectProperties(QObject* pObj,
                                          const QString& qsProps, // = "" // an empty for all
                                          const QString& qsSkipProps, // = ""
                                          const QString& qsDelim) const // = " "
{
  const QMetaObject* pMeta = pObj->metaObject();
  ODA_ASSERT_ONCE(pMeta);
  if (!pMeta)
    return false;

  QString qsObjectName = pObj->objectName(),
          qsClassName = pMeta->className();
  ODA_ASSERT_ONCE(!qsObjectName.isEmpty());
  if (qsObjectName.isEmpty())
    return false;

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);
  
  QStringList lstProps,
              lstSkipProps;
  if (!qsProps.isEmpty() && !qsDelim.isEmpty())
    lstProps = qsProps.split(qsDelim, QString::SkipEmptyParts);
  if (!qsSkipProps.isEmpty() && !qsDelim.isEmpty())
    lstSkipProps = qsSkipProps.split(qsDelim, QString::SkipEmptyParts);

  bool bRes = false;
  for (int index = 0; index < pMeta->propertyCount(); index++)
  {
    QMetaProperty prop = pMeta->property(index);
    QString qsName = prop.name();
    if (qsName.isEmpty())
      continue;
    if (!prop.isStored()) // || !prop.isWritable() || !prop.isValid())
    {
      ODA_ASSERT_ONCE(!lstProps.size() || !lstProps.contains(qsName));
      continue;
    }
    QVariant qvValue = prop.read(pObj);
    ODA_ASSERT_VAR(if (!qvValue.toBool()))
      ODA_ASSERT_ONCE(true); // brk

    if (qsName != "objectName") // is used as indication of existence
    {
      if (lstProps.size() && !lstProps.contains(qsName))
        continue;
      if (qsSkipProps.size() && qsSkipProps.contains(qsName))
        continue;
      bRes = true;
    }
    //QString qsValue = qvValue.toString();
    //if (qsValue.isEmpty())
    //  continue;

    settings.setValue(QString("objectProps/%1/%2/%3").arg(qsClassName).arg(qsObjectName).arg(qsName), qvValue);
  }
  ODA_ASSERT_ONCE(bRes);
  
  settings.endGroup();
  return bRes;
}

bool OdqMainWindow::restoreQObjectProperties(QObject* pObj,
                                             const QString& qsProps, // = "" // an empty for all
                                             const QString& qsSkipProps, // = ""
                                             const QString& qsDelim) const // = " "
{
  const QMetaObject* pMeta = pObj->metaObject();
  ODA_ASSERT_ONCE(pMeta);
  if (!pMeta)
    return false;

  QString qsObjectName = pObj->objectName(),
          qsClassName = pMeta->className();
  ODA_ASSERT_ONCE(!qsObjectName.isEmpty());
  if (qsObjectName.isEmpty())
    return false;

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  if (settings.value(QString("objectProps/%1/%2/objectName").arg(qsClassName).arg(qsObjectName), 
                     "").toString() != qsObjectName)
  {
    settings.endGroup();
    return false;
  }

  QStringList lstProps,
              lstSkipProps;
  if (!qsProps.isEmpty() && !qsDelim.isEmpty())
    lstProps = qsProps.split(qsDelim, QString::SkipEmptyParts);
  if (!qsSkipProps.isEmpty() && !qsDelim.isEmpty())
    lstSkipProps = qsSkipProps.split(qsDelim, QString::SkipEmptyParts);

  bool bRes = false;
  for (int index = 0; index < pMeta->propertyCount(); index++)
  {
    QMetaProperty prop = pMeta->property(index);
    QString qsName = prop.name();
    ODA_ASSERT_ONCE(!qsName.isEmpty());
    if (qsName.isEmpty())
      continue;
    if (!prop.isStored()) // || !prop.isWritable() || !prop.isValid())
    {
      ODA_ASSERT_ONCE(!lstProps.size() || !lstProps.contains(qsName));
      continue;
    }
    if (lstProps.size() && !lstProps.contains(qsName))
      continue;
    if (qsSkipProps.size() && qsSkipProps.contains(qsName))
      continue;
    bRes = true;

    QVariant qvValue = settings.value(QString("objectProps/%1/%2/%3").arg(qsClassName)
                                                                     .arg(qsObjectName)
                                                                     .arg(qsName), "");
    //QString qsValue = qvValue.toString();
    //ODA_ASSERT_ONCE(!qsValue.isEmpty());
    //if (qsValue.isEmpty())
    //  continue;

    prop.write(pObj, qvValue);
  }
  ODA_ASSERT_ONCE(bRes);

  settings.endGroup();
  return bRes;
}

bool OdqMainWindow::showUrl(const QString& qsUrl,
                            const QString& cqsTitle, // = "%1"
                            bool bModal) // = true
{
  QUrl url(qsUrl);
  if (!url.isValid())
    return false;
  QString qsScheme = url.scheme();

  QString qsLocal = url.toLocalFile();
  if (!qsLocal.isEmpty() && !QFileInfo(qsLocal).exists())
    return false;

  if (qsScheme.isEmpty() || qsScheme == "qrc")
  {
    QFileInfo info(QString(qsUrl).replace("qrc:", ":"));
    if (!info.exists())
      return false;
  }

  QString qsTitle;
  if (!cqsTitle.isEmpty()){
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qsTitle = QObject::tr(QString(cqsTitle).arg(getIApp()->getAppName()).toAscii().data());
#else
    qsTitle = QObject::tr(QString(cqsTitle).arg(getIApp()->getAppName()).toLatin1().data());
#endif
  }

  if (bModal)
  {
    // Workaround for : macx : QTextBrowser close button unavailable on modal window
    QDialog dlg(this);
    if (!qsTitle.isEmpty())
      dlg.setWindowTitle(qsTitle);
    restoreWidgetPlacement(&dlg, "modal", 600, 350);
   // dlg.setWindowState(Qt::WindowMaximized);

    QVBoxLayout* pLayout = new QVBoxLayout(&dlg);

    OdSharedPtr<QTextBrowser> sv_pBrowser = m_pBrowser;

    m_pBrowser = new QTextBrowser(&dlg);
    m_pBrowser->setFrameStyle(QFrame::NoFrame);
    m_pBrowser->setReadOnly(true);  
    m_pBrowser->setAttribute(Qt::WA_DeleteOnClose, false);
    connect(m_pBrowser, SIGNAL(sourceChanged(QUrl)), 
            this, SLOT(changedBrowserSource(QUrl)), Qt::QueuedConnection);
    connect(m_pBrowser, SIGNAL(anchorClicked(QUrl)), 
            this, SLOT(clickedBrowserAnchor(QUrl)));
    m_pBrowser->setSource(url);
    pLayout->addWidget(m_pBrowser);

    QPushButton* pCloseButton = new QPushButton(tr("OK"));
    pCloseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pCloseButton->setMinimumSize(100, pCloseButton->minimumSize().height());
    connect(pCloseButton, SIGNAL(clicked()),
            this, SLOT(clickedBrowserCloseButton()));
    pLayout->addWidget(pCloseButton,
                       0, //stretch
                       Qt::AlignHCenter); // alignment
  //  dlg.adjustSize();
    dlg.exec();

    saveWidgetPlacement(&dlg, "modal");
    dlg.close();

    m_pBrowser = sv_pBrowser;
    return true;
  }

  if (m_pBrowser.isNull())
  {
    m_pBrowser = new QTextBrowser(this);
    m_pBrowser->setWindowIcon(windowIcon());
    m_pBrowser->setReadOnly(true);  
    restoreWidgetPlacement(m_pBrowser, "browser", 
                           700, 400); // size at first start
  }
  m_pBrowser->setWindowModality(Qt::NonModal);

  if (!cqsTitle.isEmpty())
    m_pBrowser->setWindowTitle(qsTitle);

  m_pBrowser->setParent(NULL, Qt::Dialog);
  //m_pBrowser->setAttribute(Qt::WA_DeleteOnClose);

  connect(m_pBrowser, SIGNAL(sourceChanged(QUrl)), 
          this, SLOT(changedBrowserSource(QUrl)), Qt::QueuedConnection);
  connect(m_pBrowser, SIGNAL(anchorClicked(QUrl)), 
          this, SLOT(clickedBrowserAnchor(QUrl)));
  m_pBrowser->setSource(url);

  m_pBrowser->show();
  return true;
}

bool OdqMainWindow::isBrowserClosed() const
{
  if (m_pBrowser.isNull())
    return true;
  QDialog* pParent = qobject_cast<QDialog*>(m_pBrowser->parent());
  ODA_ASSERT_ONCE(pParent || m_pBrowser->windowModality() == Qt::NonModal);
  if (!pParent || !pParent->isVisible())
    return true;

  return false;
}

//void OdqMainWindow::showEvent(QShowEvent* pEvent)
//{
//  QMainWindow::showEvent(pEvent);
//}
//
//void OdqMainWindow::paintEvent(QPaintEvent* pEvent)
//{
//  QMainWindow::paintEvent(pEvent);
//}

void OdqMainWindow::closeEvent(QCloseEvent* pEvent)
{
		//SEA crush on exit workaround
		if (!getIConsole())
		{
			return;
		}		
		QList<OdRxObject*> lst = getIConsole()->getOpenDatabases();
		if (!lst.count())
		{
			saveWidgetPlacement(this);
			saveWidgetPlacement(m_pBrowser.get(), "browser");
			//OdqApplication* pApp = qobject_cast<OdqApplication*>(qApp);
			//pApp->getCustom()->saveSettings();  //TODOMZQ  没有新建工程并添加间隙圆时退出会报错，暂时用不到注释掉
			QMdiArea* pMdiArea = qobject_cast<QMdiArea*>(centralWidget());
			ODA_ASSERT_ONCE(pMdiArea);
			if (pMdiArea && !pMdiArea->activeSubWindow())
			{
				{
					// unload custom unreferenced modules
					// (possible crash without it (via references 
					//  to overloaded command of console like ERASE - see ExCustObjs.tx : ExCustObjsModule::uninitApp() : pPrevErase
					OdRxModulePtr pHolderConsole = ::odrxDynamicLinker()->getModule(L"SmartClConsole.tx");
					OdRxModulePtr pHolderDgnDb = ::odrxDynamicLinker()->getModule(L"TD_DgnDb.tx"); // TODO move this holder in real needed place
					OdRxModulePtr pHolderBimDb = ::odrxDynamicLinker()->getModule(L"TD_BimDb.tx");
					OdRxModulePtr pHolderTigDb = ::odrxDynamicLinker()->getModule(L"TD_TigDb.tx");
					::odrxDynamicLinker()->unloadUnreferenced();
				}
				// it's here to save valid visibility of console palette via QSettings
				uninitAppModule();						
							
				QMainWindow::closeEvent(pEvent);
				return;
			}
		}	
		if (lst.count() && !getIConsole()->isMarkedToClose(lst.first()))
		{
			IqConsole* iConsole = getIConsole();
			QString qsCmd = QString("savewidgetdata ");
			int select =QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否保存当前工程？"),
					QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			switch (select)
			{
			case QMessageBox::Yes:
				if (!iConsole)
				{
					ODA_FAIL();
					return;
				}			
				iConsole->executeCommand(qsCmd);
				_coreDbPt->save();
				getIConsole()->executeCommand("^C^C quit");//getIConsole()->postponeCommand("quit", true, NULL, 100);
				break;
			case  QMessageBox::No:
				getIConsole()->executeCommand("^C^C quit");//先关闭间隙园文档
				break;
			case  QMessageBox::Cancel:
				break;
			default:
				break;
			}								
		}			
		pEvent->ignore();
}

void OdqMainWindow::dragEnterEvent(QDragEnterEvent* pEvent) 
{
  if (m_bDragging)
  {
    QMainWindow::dragEnterEvent(pEvent);
    pEvent->acceptProposedAction();
    return;
  }

  const QMimeData *pData = pEvent->mimeData();
  if (!pData)
  {
    pEvent->ignore();
    return;
  }

  OdRxObject* pRxDatabase = getIConsole()->getCurrentDatabase();
  if (!OdqClipboardData::isAcadDataAvailable(pData, pRxDatabase))
  {
    if (!pData->hasUrls())
    {    
      pEvent->ignore();
      return;
    }
    QList<QUrl> files = pData->urls();
    bool bRes = false;
    foreach(QUrl url, files)
    {
      if (url.isEmpty())
        continue;
      QString qsFileName = url.toLocalFile();
      OdString sExt = getIApp()->toOdString(QFileInfo(qsFileName).suffix().toLower()); // .dgn .rfa .rvt .dwf ...
      if (!isAcadExtension(sExt) && !getIApp()->getExtDbModuleByExtension(sExt))
          continue;
      bRes = true;
    }
    if (bRes)
      pEvent->acceptProposedAction(); // pEvent->accept();
    else
      pEvent->ignore();
    return;
  }

  //if (getIConsole()->activeCommandState(pRxDatabase) != IqConsole::enCmdActive_No)
  //{
  //  if (m_bDragging)
  //    pEvent->acceptProposedAction();
  //  return;
  //}

  if (!OdqClipboardData::copyToClipboard(pData, pRxDatabase))
  {
    ODA_FAIL_ONCE(); // test
    return;
  }

  m_bDragging = true;
  pEvent->acceptProposedAction();
}

//void OdqMainWindow::dragMoveEvent(QDragMoveEvent* pEvent)
//{
//  pEvent->acceptProposedAction();
//
//
//  //if (!m_bDragging)
//  //{
//    QMainWindow::dragMoveEvent(pEvent);
//    return;
//  //}
//
//  //pEvent->acceptProposedAction();
//  // TODO
//}

//void OdqMainWindow::dragLeaveEvent(QDragLeaveEvent* pEvent)
//{
//  //if (!m_bDragging)
//  //{
//  //  QMainWindow::dragLeaveEvent(pEvent);
//  return;
//  //}
//
//  //m_bDragging = false;
//  //OdRxObject* pRxDatabase = getIConsole()->getCurrentDatabase();
//  //if (getIConsole()->activeCommandName(pRxDatabase).makeLower().find(OD_T("pasteclip")))
//  //  getIConsole()->markToCancel(pRxDatabase);
//}

void OdqMainWindow::dropEvent(QDropEvent* pEvent)
{
  if (!m_bDragging)
  {
    const QMimeData *pData = pEvent->mimeData();
    if (!pData || !pData->hasUrls())
    {    
      pEvent->ignore();
      return;
    }

    QList<QUrl> files = pData->urls();
    QString qsCmd;
    foreach(QUrl url, files)
    {
      QString qsFileName = url.toLocalFile();
      OdString sExt = getIApp()->toOdString(QFileInfo(qsFileName).suffix().toLower()); // .dgn .rfa .rvt .dwf ...
      if (!isAcadExtension(sExt) && !getIApp()->getExtDbModuleByExtension(sExt))
        continue;

      //if (!qsCmd.isEmpty())
      //  qsCmd += "\n";
      qsCmd.clear();
      qsCmd += QString("open \"%1\"").arg(qsFileName);
      getIConsole()->postponeCommand(qsCmd, true, (OdRxObject*) -1);
    }
    if (!qsCmd.isEmpty())
    {
      //getIConsole()->postponeCommand(qsCmd, true, (OdRxObject*) -1);
      pEvent->acceptProposedAction(); // pEvent->accept();
      return;
    }

    //pEvent->ignore();
    QMainWindow::dropEvent(pEvent);
    return;
  }
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    return;

  QPoint point(pEvent->pos().x(), pEvent->pos().y());
  point = mapToGlobal(point); // to screen coords

  IqView* iView = iVS->getViewByScreenPoint(point, // to view coords
                                            true); // do Active
  if (!iView)
    return;
  OdRxObject* pRxDatabase = iView->getRxDatabase();
    
  QString qsCmd = "dropgeom";

  //if (pRxDatabase == getIConsole()->getDatabaseDragSource())
  {
    OdGePoint3d pt;
    if (!iView->eyesTo(point, 0, pt))
      return;
    qsCmd += QString(" %1,%2").arg(pt.x).arg(pt.y);
    if (!OdZero(pt.z))
      qsCmd += QString(",%1").arg(pt.z);
  }
  getIConsole()->executeCommand(qsCmd, false, pRxDatabase);
  if (pRxDatabase == getIConsole()->getDatabaseDragSource())
    getIConsole()->updateAllGrips(pRxDatabase);

  m_bDragging = false;
}

//slot
void OdqMainWindow::revertConsoleVisibility()
{
  getIConsole()->postponeCommand(getIConsole()->isConsoleVisible() ? "commandlinehide" 
                                                                   : "commandline");
}

//slot
void OdqMainWindow::showHelp()
{
  getIConsole()->postponeCommand("help");
}

//slot
void OdqMainWindow::changedBrowserSource(const QUrl & url)
{
  //QString qsPath = url.path();
  QString qsText = m_pBrowser->toHtml();
  QString qsCopyrightDD = getIApp()->toQString(TD_COPYRIGHT_W);
  qsCopyrightDD.replace("Copyright ", "Copyright"); // remove useless space
  QString qsCompanyNameDD = qsCopyrightDD;
  qsCompanyNameDD.remove(" ('Open Design')", Qt::CaseInsensitive);
  qsText.replace("$(TD_COMPANY_NAME)", qsCompanyNameDD, Qt::CaseInsensitive);
  //qsCopyrightDD.replace("\xA9", "&copy;");
  //qsCopyrightDD.replace(" Inc. ('Open Design')", ".");
  //qsCopyrightDD.replace("2012", "2013"); // temp solution // TODO: remove after realese v3.? !!!
  qsText.replace("$(TD_COPYRIGHT)", qsCopyrightDD, Qt::CaseInsensitive);
  
  qsText.replace("$(TD_PRODUCT_NAME)",  getIApp()->toQString(TD_PRODUCT_NAME_W), Qt::CaseInsensitive);
  qsText.replace("$(TD_PRODUCT_NAME_START)",  getIApp()->toQString(TD_PRODUCT_NAME_START_W), Qt::CaseInsensitive);
  // In the About box, the TM symbol for Teigha needs to change to ®. // From: Allison Angus February 21, 2011
  //qsText.replace("$(TD_PRODUCT_NAME)", QString(TD_PRODUCT_NAME_S).replace("Teigha™", "Teigha&reg;"), Qt::CaseInsensitive);
  //qsText.replace("$(TD_PRODUCT_NAME_START)", QString(TD_PRODUCT_NAME_START_S).replace("Teigha™", "Teigha&reg;"), Qt::CaseInsensitive);
  
  qsText.replace("$(TD_APP_NAME)", getIApp()->getAppName(), Qt::CaseInsensitive);
  qsText.replace("$(TD_EXE_NAME)", QFileInfo(QCoreApplication::applicationFilePath()).baseName(), Qt::CaseInsensitive);

  //QString qsVerTD =  TD_PRODUCT_VER_STR_S; // TD_SHORT_STRING_VER_S // "3.02"
  //qsVerTD.replace(", ", "."); // for common style with Qt (same with DD short ver style)
  QString qsVerTD = QString("%1.%2.%3").arg(TD_MAJOR_VERSION).arg(TD_MINOR_VERSION).arg(TD_MAJOR_BUILD);
 #if TD_MINOR_BUILD > 0
  qsVerTD += QString(".%1").arg(TD_MINOR_BUILD)
 #endif
  qsText.replace("$(TD_VER_STR)", qsVerTD, Qt::CaseInsensitive);
  qsText.replace("$(TD_SHORT_VER_STR)", TD_SHORT_STRING_VER_S, Qt::CaseInsensitive);
  qsText.replace("$(QT_VER_STR)", QT_VERSION_STR, Qt::CaseInsensitive); // "4.5.3"
  if (qsText.indexOf("$(CAUSTIC", Qt::CaseInsensitive) >= 0)
  {
    OdGsModulePtr pCausticGs = ::odrxDynamicLinker()->loadModule(OD_T("SmartClOpenGL.txv"));
   #ifdef Q_CC_MSVC
    if (pCausticGs.isNull())
      pCausticGs = ::odrxDynamicLinker()->loadModule(OD_T("WinOpenRL.txv"));
   #endif
    if (!pCausticGs.isNull())
    {
      OdRxDictionaryPtr pProperties = pCausticGs->createDevice()->properties();
      int index = -1;
      QStringList lstMacro;
      lstMacro << "$(CAUSTIC_VER_STR)" << "$(CAUSTIC_VER_STR)"
               << "$(CAUSTIC_COMPANY_NAME)" << "$(CAUSTIC_COMPANY_NAME)"
               << "$(CAUSTIC_PRODUCT_NAME)" << "$(CAUSTIC_PRODUCT_NAME)";
                                                // more priority    -       less priority
      foreach (QString qsProp, QStringList() << "OpenRLProductVersionVI" << "OpenRLVersion" 
                                             << "OpenRLCompanyVI"        << "OpenRLVendor" 
                                             << "OpenRLProductNameVI"    << "OpenRLRenderer")
      {
        OdString sProp = getIApp()->toOdString(qsProp);
        QString qsMacro = lstMacro.at(++index);
        if (pProperties->has(sProp))
        {
          QString qsValue = getIApp()->toQString(OdRxVariantValue(pProperties->getAt(sProp))->getString());
          if (!qsValue.isEmpty())
            qsText.replace(qsMacro, qsValue, Qt::CaseInsensitive);
        }
      }
    }
    else
      ODA_FAIL_ONCE();
  }

  // set invisible background color
  // (unfortunately m_pBrowser->setTextBackgroundColor is not usable for it)
  QPalette palette = qApp->palette();
  QColor colorSysGrey = palette.color(QPalette::Normal, QPalette::Window); //Base);
  int red = colorSysGrey.red(), // 236
      green = colorSysGrey.green(), // 233
      blue = colorSysGrey.blue(); // 216
  QString qsSysGrey = QString("bgcolor=\"#%1%2%3\"").arg(red, 2, 16, QLatin1Char('0'))
                                                    .arg(green, 2, 16, QLatin1Char('0'))
                                                    .arg(blue, 2, 16, QLatin1Char('0'));
  qsSysGrey = qsSysGrey.toUpper();
  qsText.replace("bgcolor=\"#ffffff\"", qsSysGrey, Qt::CaseInsensitive);
  m_pBrowser->setHtml(qsSysGrey);

  m_pBrowser->setHtml(qsText);
}

//slot
void OdqMainWindow::clickedBrowserAnchor(const QUrl& url)
{
  QString qsScheme = url.scheme();
  QString qsLocal = url.toLocalFile();

  if (!qsLocal.isEmpty() || qsScheme.isEmpty() || qsScheme == "qrc")
  {
    QString qsUrl = qsLocal;
    if (qsUrl.isEmpty())
    {
      qsUrl = url.toString();
      qsUrl.replace("qrc:", ":");
      m_pBrowser->setSource(url);
    }
    if (QFileInfo(qsUrl).exists())
      return;
  }
  if (qsScheme == "http")
  {
    // open with standard OS browser
    if (!QDesktopServices::openUrl(url))
    {
      ODA_FAIL_ONCE(); 
    }
  }
  if (qsScheme == "cmd")
  {
    IqConsole* iConsole = getIConsole();
    if (iConsole)
    {
      // QString qsCommand = url.toString(); qsCommand.remove("cmd://");
      // solution for spaces 
      QString qsCommand = url.toEncoded(QUrl::RemoveScheme);
      qsCommand = qsCommand.mid(2);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
      qsCommand = QUrl::fromPercentEncoding(qsCommand.toAscii());
#else
      qsCommand = QUrl::fromPercentEncoding(qsCommand.toLatin1());
#endif
      qsCommand.replace("\x00a0", " "); // qsCommand.replace("&nbsp;", " ");

      // # Clicking "recover" in the help file to go into a loop
      if (qsCommand != m_qsBrowserLastCommand)
      {
        m_qsBrowserLastCommand = qsCommand;
        //m_pBrowser->blockSignals(true);
        getIConsole()->executeCommand(qsCommand);
        //m_pBrowser->blockSignals(false); 
        QTimer::singleShot(100, this, SLOT(unlockCommandViaBrowser()));
      }
      //else
      //  m_qsBrowserLastCommand,clear();
    }
    //int posV = m_pBrowser->verticalScrollBar()->value();
    //m_pBrowser->reload(); //m_pBrowser->setSource(m_pBrowser->source());
    //m_pBrowser->verticalScrollBar()->setValue(posV);
    //return;
  }
  m_pBrowser->setSource(m_pBrowser->source());
}

//slot
void OdqMainWindow::clickedBrowserCloseButton()
{
  ODA_ASSERT_ONCE(!m_pBrowser.isNull());

  // ways to close modal dialog on mac
  QDialog* pParent = qobject_cast<QDialog*>(m_pBrowser->parent());
  ODA_ASSERT_ONCE(pParent || m_pBrowser->windowModality() == Qt::NonModal);
  if (pParent)
    pParent->close();
  else
    m_pBrowser->close();
}

//slot
void OdqMainWindow::unlockCommandViaBrowser()
{
  m_qsBrowserLastCommand.clear(); 
}

void OdqMainWindow::initDockWidget(QMdiArea* pMdiArea)
{
	QDockWidget *pLeftDock = new QDockWidget(pMdiArea);
	pLeftDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	pLeftDock->setAllowedAreas(Qt::LeftDockWidgetArea);

	_pClListWidget = new ClListWidget(pLeftDock);
	pLeftDock->setWidget(_pClListWidget);
	//pLeftDock->setFixedWidth(180);

	//QDesktopWidget *deskWidget = QApplication::desktop();
	//QRect appRect = deskWidget->screenGeometry();
	//qreal dockWidgetMinW = 0.383*appRect.width();
	//pLeftDock->setMinimumWidth(dockWidgetMinW);

	/*TODO 删除dockwidget曲线救国方法*/
	QWidget* titleBar = pLeftDock->titleBarWidget();
	QWidget* newTittleBar = new QWidget(pLeftDock);
	pLeftDock->setTitleBarWidget(newTittleBar);
	delete titleBar;

	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, pLeftDock);

	connect(this, SIGNAL(crateProTreew()), _pClListWidget, SLOT(slotCreateTreeItem()));
	connect(this, SIGNAL(reshTreeProName()), _pClListWidget, SLOT(slotReshTreeProName()));
	connect(this, SIGNAL(clearTree()), _pClListWidget, SLOT(slotClearTree()));	
	connect(this, SIGNAL(clearClWidget()), _pClListWidget, SLOT(slotClearClWidget()));
	connect(_pClListWidget, SIGNAL(rightAddCl()), this, SLOT(slotAddCl()));
}

void OdqMainWindow::creatAction()
{
	/*项目菜单*/
	QMenu *pProjectMenu = new QMenu(QString::fromLocal8Bit("文件"),this->menuBar());
	this->menuBar()->addMenu(pProjectMenu);
	_newProAct = new QAction(QString::fromLocal8Bit("新建"), pProjectMenu);
	_newProAct->setIcon(QIcon(":/images/SmartCL/new.png"));
	_newProAct->setShortcut(QKeySequence::New);
	_newProAct->setStatusTip(QString::fromLocal8Bit("新建工程"));
	connect(_newProAct, SIGNAL(triggered()), this, SLOT(slotNewProject()));
	_openAct = new QAction(QString::fromLocal8Bit("打开"), pProjectMenu);
	_openAct->setIcon(QIcon(":/images/SmartCL/open.png"));
	_openAct->setShortcuts(QKeySequence::Open);
	_openAct->setStatusTip(QString::fromLocal8Bit("打开工程"));
	connect(_openAct, SIGNAL(triggered()), this, SLOT(slotOpen()));
	_saveAct = new QAction(QString::fromLocal8Bit("保存"), pProjectMenu);
	_saveAct->setEnabled(false);
	_saveAct->setIcon(QIcon(":/images/SmartCL/save.png"));
	_saveAct->setShortcuts(QKeySequence::Save);
	_saveAct->setStatusTip(QString::fromLocal8Bit("保存工程"));
	connect(_saveAct, SIGNAL(triggered()), this, SLOT(slotSave()));
	_saveAsAct = new QAction(QString::fromLocal8Bit("另存为"), pProjectMenu);
	_saveAsAct->setEnabled(false);
	_saveAsAct->setIcon(QIcon(":/images/SmartCL/saveas.png"));
	_saveAsAct->setShortcuts(QKeySequence::SaveAs);
	_saveAsAct->setStatusTip(QString::fromLocal8Bit("另存为工程"));
	connect(_saveAsAct, SIGNAL(triggered()), this, SLOT(slotSaveAs()));
	_openFilePathAct = new QAction(QString::fromLocal8Bit("打开当前工程目录"), pProjectMenu);
	_openFilePathAct->setEnabled(false);
	_openFilePathAct->setIcon(QIcon(":/images/SmartCL/openProjectFile.png"));
	_openFilePathAct->setStatusTip(QString::fromLocal8Bit("打开当前工程目录"));
	connect(_openFilePathAct, SIGNAL(triggered()), this, SLOT(slotOpenFilePath()));
	pProjectMenu->addAction(_newProAct);
	pProjectMenu->addAction(_openAct);
	pProjectMenu->addAction(_saveAct);
	pProjectMenu->addAction(_saveAsAct);
	pProjectMenu->addAction(_openFilePathAct);
	pProjectMenu->addMenu(initRecentMenus(pProjectMenu));

	/*操作菜单*/
	_operatMenu = new QMenu(QString::fromLocal8Bit("操作"), this->menuBar());
	this->menuBar()->addMenu(_operatMenu);
	_pAddClAct = new QAction(QString::fromLocal8Bit("添加计算模块"), _operatMenu);
	_pAddClAct->setIcon(QIcon(":/images/SmartCL/addCl.png"));
	_pAddClAct->setEnabled(false);
	_pAddClAct->setStatusTip(QString::fromLocal8Bit("添加计算模块"));
	connect(_pAddClAct, SIGNAL(triggered()), this, SLOT(slotAddCl()));

	_straight_I_Act = new QAction(QString::fromLocal8Bit("直线塔I串间隙圆"), _operatMenu);
	_straight_I_Act->setIcon(QIcon(":/images/SmartCL/straight_I.png"));
	_straight_I_Act->setEnabled(false);
	_straight_I_Act->setStatusTip(QString::fromLocal8Bit("直线塔I串间隙圆"));
	connect(_straight_I_Act, SIGNAL(triggered()), this, SLOT(slotAddStraight_I()));
	
	_straight_V_Act = new QAction(QString::fromLocal8Bit("直线塔V串间隙圆"), _operatMenu);
	_straight_V_Act->setIcon(QIcon(":/images/SmartCL/straight_V.png"));
	_straight_V_Act->setEnabled(false);
	_straight_V_Act->setStatusTip(QString::fromLocal8Bit("直线塔V串间隙圆"));
	connect(_straight_V_Act, SIGNAL(triggered()), this, SLOT(slotAddStraight_V()));

	_straight_L_Act = new QAction(QString::fromLocal8Bit("直线塔L串间隙圆"), _operatMenu);
	_straight_L_Act->setIcon(QIcon(":/images/SmartCL/straight_L.png"));
	_straight_L_Act->setEnabled(false);
	_straight_L_Act->setStatusTip(QString::fromLocal8Bit("直线塔L串间隙圆"));
	connect(_straight_L_Act, SIGNAL(triggered()), this, SLOT(slotAddStraight_L()));

	_maxAngle_I_Act = new QAction(QString::fromLocal8Bit("直线塔I串允许摇摆角"), _operatMenu);
	_maxAngle_I_Act->setIcon(QIcon(":/images/SmartCL/maxAngle_I.png"));
	_maxAngle_I_Act->setEnabled(false);
	_maxAngle_I_Act->setStatusTip(QString::fromLocal8Bit("直线塔I串允许摇摆角"));
	connect(_maxAngle_I_Act, SIGNAL(triggered()), this, SLOT(slotAddMaxAngle_I()));

	_maxAngle_V_Act = new QAction(QString::fromLocal8Bit("直线塔V串允许夹角"), _operatMenu);
	_maxAngle_V_Act->setIcon(QIcon(":/images/SmartCL/maxAngle_V.png"));
	_maxAngle_V_Act->setEnabled(false);
	_maxAngle_V_Act->setStatusTip(QString::fromLocal8Bit("直线塔V串允许夹角"));
	connect(_maxAngle_V_Act, SIGNAL(triggered()), this, SLOT(slotAddMaxAngle_V()));

	_drawAllClAct = new QAction(QString::fromLocal8Bit("一键生成图纸及计算书"), _operatMenu);
	_drawAllClAct->setIcon(QIcon(":/images/SmartCL/auto.png"));
	_drawAllClAct->setEnabled(false);
	_drawAllClAct->setStatusTip(QString::fromLocal8Bit("一键生成图纸及计算书"));
	connect(_drawAllClAct, SIGNAL(triggered()), this, SLOT(slotDrawAllCl()));
	connect(this, SIGNAL(signalDrawAllClActClicked()), _pClListWidget, SIGNAL(signalDrawAllCl()));

	_settingsDlgAct = new QAction(QString::fromLocal8Bit("绘图设置"), _operatMenu);
	_settingsDlgAct->setIcon(QIcon(":/images/SmartCL/settingsDlg.png"));
	_settingsDlgAct->setEnabled(false);
	_settingsDlgAct->setStatusTip(QString::fromLocal8Bit("一键生成图纸及计算书"));
	connect(_settingsDlgAct, SIGNAL(triggered()), this, SLOT(slotShowSettingsDlg()));

	_operatMenu->addAction(_pAddClAct);
	_operatMenu->addSeparator();
	_operatMenu->addAction(_straight_I_Act);
	_operatMenu->addAction(_straight_V_Act);
	_operatMenu->addAction(_straight_L_Act);
	_operatMenu->addAction(_maxAngle_I_Act);
	_operatMenu->addAction(_maxAngle_V_Act);
	_operatMenu->addSeparator();
	_operatMenu->addAction(_drawAllClAct);
	_operatMenu->addSeparator();
	_operatMenu->addAction(_settingsDlgAct);

	/*参数菜单*/
	_pParameterMenu = new QMenu(QString::fromLocal8Bit("参数"), this->menuBar());
	this->menuBar()->addMenu(_pParameterMenu);
	_wireParameterAct = new QAction(QString::fromLocal8Bit("导线参数库"), _pParameterMenu);
	_wireParameterAct->setEnabled(false);
	connect(_wireParameterAct, SIGNAL(triggered()), this, SLOT(slotShowWPA()));
	_meteAreaAct = new QAction(QString::fromLocal8Bit("气象区参数库"), _pParameterMenu);
	_meteAreaAct->setEnabled(false);
	connect(_meteAreaAct, SIGNAL(triggered()), this, SLOT(slotShowcurMeteArea()));
	_insulatorLibAct = new QAction(QString::fromLocal8Bit("绝缘子串库"), _pParameterMenu);
	_insulatorLibAct->setEnabled(false);
	connect(_insulatorLibAct, SIGNAL(triggered()), this, SLOT(slotShowInsulatorLib()));
	_sgstGapAct = new QAction(QString::fromLocal8Bit("间隙推荐值设置"), _pParameterMenu);
	_sgstGapAct->setEnabled(false);
	connect(_sgstGapAct, SIGNAL(triggered()), this, SLOT(slotShowSgstGap()));
	_wireAvgHLibAct = new QAction(QString::fromLocal8Bit("导线平均高设置"), _pParameterMenu);
	_wireAvgHLibAct->setEnabled(false);
	connect(_wireAvgHLibAct, SIGNAL(triggered()), this, SLOT(slotShowWireAvgHLib()));

	_pParameterMenu->addAction(_wireParameterAct);
	_pParameterMenu->addAction(_meteAreaAct);
	_pParameterMenu->addAction(_insulatorLibAct);
	_pParameterMenu->addSeparator();
	_pParameterMenu->addAction(_sgstGapAct);
	_pParameterMenu->addAction(_wireAvgHLibAct);

	/*帮助菜单*/
	QMenu *helpMenu = new QMenu(QString::fromLocal8Bit("帮助"), this->menuBar());
	this->menuBar()->addMenu(helpMenu);
	QAction *aboutAct = new QAction(QString::fromLocal8Bit("关于软件"), helpMenu);
	aboutAct->setIcon(QIcon(":/images/SmartCL/about.png"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(slotAbout()));
	QAction *updateLogDlgAct = new QAction(QString::fromLocal8Bit("更新日志"), helpMenu);
	updateLogDlgAct->setIcon(QIcon(":/images/SmartCL/about.png"));
	connect(updateLogDlgAct, SIGNAL(triggered()), this, SLOT(slotUpdateLogDlg()));
	QAction *helpAct = new QAction(QString::fromLocal8Bit("帮助文档"), helpMenu);
	helpAct->setIcon(QIcon(":/images/SmartCL/help.png"));
	connect(helpAct, SIGNAL(triggered()), this, SLOT(slotHelp()));
	_skinMenu = new QMenu(QString::fromLocal8Bit("换肤"), helpMenu);
	_skinMenu->setIcon(QIcon(":/images/SmartCL/skin.png"));
	QStringList name;
	name << QStringLiteral("灰白色") << QStringLiteral("浅蓝色") << QStringLiteral("暗黑色");
	foreach(QString str, name)
	{
		QAction *action = new QAction(str, this);
		_skinMenu->addAction(action);
		connect(action, SIGNAL(triggered(bool)), this, SLOT(slotChangestyle()));
	}
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(updateLogDlgAct);
	helpMenu->addAction(helpAct);
	helpMenu->addMenu(_skinMenu);
	

	/*文件工具栏*/
	QToolBar *toolBarFile = new QToolBar(QString::fromLocal8Bit("文件工具栏"));
	toolBarFile->addAction(_newProAct);
	toolBarFile->addAction(_openAct);
	toolBarFile->addAction(_saveAct);
	toolBarFile->addAction(_saveAsAct);
	toolBarFile->addAction(_openFilePathAct);
	this->addToolBar(toolBarFile);

	/*操作工具栏*/
	QToolBar *toolBarAdd = new QToolBar(QString::fromLocal8Bit("添加计算模块工具栏"));
	toolBarAdd->addAction(_pAddClAct);
	toolBarAdd->addAction(_straight_I_Act);
	toolBarAdd->addAction(_straight_V_Act);
	toolBarAdd->addAction(_straight_L_Act);
	toolBarAdd->addAction(_maxAngle_I_Act);
	toolBarAdd->addAction(_maxAngle_V_Act);
	this->addToolBar(toolBarAdd);

	/*批量处理工具栏*/
	QToolBar *toolBarAuto = new QToolBar(QString::fromLocal8Bit("批量处理工具栏"));
	toolBarAuto->addAction(_drawAllClAct);
	this->addToolBar(toolBarAuto);

	/*绘图设置工具栏*/
	QToolBar *toolBarSetting = new QToolBar(QString::fromLocal8Bit("绘图设置工具栏"));
	toolBarSetting->addAction(_settingsDlgAct);
	this->addToolBar(toolBarSetting);

	slotChangeActState(false);//将相关action设置为不可编辑
}

//@function: 加载换肤Qss   -- by yzhchc  2020/1/6 18:44
void OdqMainWindow::loadSkinQss()
{
	QString path = "";
	switch (_qssType)
	{
	case White:
		path = ":/images/White.qss";
		_skinMenu->actions().at(0)->setChecked(true);
		_skinMenu->actions().at(1)->setChecked(false);
		_skinMenu->actions().at(2)->setChecked(false);
		break;
	case Blue:
		path = ":/images/Light.qss";
		_skinMenu->actions().at(0)->setChecked(false);
		_skinMenu->actions().at(1)->setChecked(true);
		_skinMenu->actions().at(2)->setChecked(false);
		break;
	case Dark:
		path = ":/images/Dark.qss";
		_skinMenu->actions().at(0)->setChecked(false);
		_skinMenu->actions().at(1)->setChecked(false);
		_skinMenu->actions().at(2)->setChecked(true);
		break;
	}
	setStyle(path);
}

//@ function:
QMenu* OdqMainWindow::initRecentMenus(QMenu *parent/*父指针*/)
{
	QMenu *recentMenu = new QMenu(QString::fromLocal8Bit("最近使用的项目"), parent);
	for (int iLoop = 0; iLoop < MaxRecentFiles; ++iLoop) {
		_recentFileActs[iLoop] = new QAction(recentMenu);
		_recentFileActs[iLoop]->setVisible(false);
		recentMenu->addAction(_recentFileActs[iLoop]);
	}
	connect(recentMenu, SIGNAL(aboutToShow()), this, SLOT(slotUpdateRecentFileActions()));
	connect(recentMenu, SIGNAL(triggered(QAction *)), this, SLOT(slotOpenRecentFile(QAction *)));
	_recentFileSubMenuAct = recentMenu->menuAction();
	_recentFileSubMenuAct->setIcon(QIcon(":/images/SmartCL/RecentFiles.png"));
	readRecentFile();
	if (_recentFileList.size()>0)
		_recentFileSubMenuAct->setVisible(true);
	else
		_recentFileSubMenuAct->setVisible(false);
	return recentMenu;
}

//@ function:
void OdqMainWindow::readRecentFile()
{
	QSettings settings("HJSoft",
		QFileInfo(QCoreApplication::applicationFilePath()).baseName());

	_recentFileList.clear();
	QString paths = settings.value("CL/recentFiles").toString();
	if (!paths.isEmpty())
		_recentFileList = paths.split(';');
}

void OdqMainWindow::slotWriteRecentFiles(const QString& filePath)
{
	QString path = QDir::toNativeSeparators(filePath);
	_recentFileList.removeOne(path); // 去重
	_recentFileList.prepend(path); // 加到头部
	while (_recentFileList.size() > MaxRecentFiles) { // 不能超过10条
		_recentFileList.removeLast();
	}
	QSettings settings("HJSoft",
		QFileInfo(QCoreApplication::applicationFilePath()).baseName());

	settings.setValue("CL/recentFiles", _recentFileList.join(';'));
	if (_recentFileList.size() > 0)
		_recentFileSubMenuAct->setVisible(true);
	else
		_recentFileSubMenuAct->setVisible(false);
}

//@ function:
void OdqMainWindow::slotOpenRecentFile(QAction* action)
{
	/*处理当前工程*/
	if (slotDealCurProject() == QMessageBox::Cancel)
	{
		return;
	}

	QString path = action->data().toString();
	open(path);
}

//@ function:
void OdqMainWindow::slotUpdateRecentFileActions()
{
	this->readRecentFile();
	const int count = qMin(int(MaxRecentFiles), _recentFileList.size());
	for (int iLoop = 0; iLoop < MaxRecentFiles; ++iLoop) {
		if (iLoop < count) {
			const QString fileName = _recentFileList.at(iLoop);
			_recentFileActs[iLoop]->setText(fileName);
			_recentFileActs[iLoop]->setData(fileName);
			QFileInfo file(fileName);
			if (file.isFile())
				_recentFileActs[iLoop]->setVisible(true);
		}
		else {
			_recentFileActs[iLoop]->setVisible(false);
		}
	}
}

QString OdqMainWindow::getCurrentPath()
{
	QString defaultStr;
	this->readRecentFile();
	if (_recentFileList.size() > 0)
		defaultStr = _recentFileList.at(0);
	else
		defaultStr = QProcessEnvironment::systemEnvironment().value("USERPROFILE") +
		"\\Documents" + "\\Untitled.cl";
	return defaultStr;
}

//@function:    -- by mzq  2020/5/16 19:44
void OdqMainWindow::slotAddStraight_I()
{
	if (!getVirBoxChecker()->safeLoginFlag)
		return;
	slotAddClByClType(SmartCl::ClCate::StraightIStr);
}

//@function:    -- by mzq  2020/5/16 19:44
void OdqMainWindow::slotAddStraight_V()
{
	if (!getVirBoxChecker()->safeLoginFlag)
		return;
	slotAddClByClType(SmartCl::ClCate::StraightVStr);
}

//@function:    -- by mzq  2020/5/16 19:44
void OdqMainWindow::slotAddStraight_L()
{
	if (!getVirBoxChecker()->safeLoginFlag)
		return;
	slotAddClByClType(SmartCl::ClCate::StraightLStr);
}

//@function:    -- by mzq  2020/5/16 19:45
void OdqMainWindow::slotAddMaxAngle_I()
{
	if (!getVirBoxChecker()->safeLoginFlag)
		return;
	slotAddClByClType(SmartCl::ClCate::MaxAlgle_I);
}

//@function:    -- by mzq  2020/5/16 19:45
void OdqMainWindow::slotAddMaxAngle_V()
{
	if (!getVirBoxChecker()->safeLoginFlag)
		return;
	slotAddClByClType(SmartCl::ClCate::MaxAlgle_V);
}

//@ function:
void OdqMainWindow::slotAddCl()
{
	if (!getVirBoxChecker()->safeLoginFlag)
		return;
	AddClByType_Dlg *dlg = new AddClByType_Dlg();
	dlg->setAttribute(Qt::WA_DeleteOnClose, true);
	connect(dlg, SIGNAL(createTreeByClName(const QString&)), _pClListWidget, SLOT(slotCreateItemBySubClName(const QString&)));
	dlg->exec();
}

//@function: 通过间隙圆类别添加间隙圆   -- by mzq  2020/5/16 19:49
void OdqMainWindow::slotAddClByClType(SmartCl::ClCate::Type clType)
{
	try
	{

		QString clTypeQStr = QString::fromLocal8Bit(SmartCl::ClCate::typeToStr(clType));
		QString clName = clTypeQStr;
		int countNum = 1;
		while (SmartCl::DataBase::getInstance()->existCl(clName.toLocal8Bit().toStdString()))
		{
			clName = clTypeQStr + QString("-") + QString::number(countNum);
			countNum++;
		}

		SmartCl::HdlCl tempCl = SmartCl::ClFactory::getInstance()->getClearence(clType, clName.toLocal8Bit());
		SmartCl::DataBase::getInstance()->addCl(tempCl);
		_pClListWidget->slotCreateItemBySubClName(clName);

	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("新增间隙圆"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@ function:
void OdqMainWindow::slotNewProject()
{
	std::cout << "2" << std::endl;

	_coreDbPt = SmartCl::DataBase::getInstance();//内核数据库指针存放，便于其他项目调用

	/*处理当前工程*/
	if (slotDealCurProject() == QMessageBox::Cancel)
		return;

	/*新建工程*/
	NewProject_Dlg dlg;
	connect(&dlg, SIGNAL(crateProTreew()), _pClListWidget, SLOT(slotGenerateTreeView()));
	connect(&dlg, SIGNAL(writeRecentFiles(const QString&)), this, SLOT(slotWriteRecentFiles(const QString&)));
	connect(&dlg, SIGNAL(loadParaLibFile(bool)), this, SLOT(slotLoadParaLibFile(bool)));

	if (dlg.exec() == QDialog::Accepted)
	{
		slotChangeActState(true);
		_isProNull = false;
		auto path = QString(_coreDbPt->getSavePath().c_str());
		statusBar()->showMessage(QString::fromLocal8Bit("成功创建工程，路径：") + path, 5000);
		slotChangeWinTitle();
	}
}

//@function: 设置模块是否可用   -- by mzq 2022/11/15 9:46
void OdqMainWindow::moduleEnabled()
{
	if (abs(getVirBoxChecker()->enlargeFactor - 1.0) > 0.00001
		|| abs(getVirBoxChecker()->virBoxPi - 3.1415926) > 0.00001)
	{
		this->setEnabled(false);
		return;
	}
}

//@function: 设置界面是否可用   -- by mzq 2022/11/4 11:45
void OdqMainWindow::slotSetWidgetEditable(bool flag)
{
	this->setEnabled(flag);
	if (!flag)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("未找到授权，编码：003"),
							 "<html>"
							 + QString::fromLocal8Bit("未找到授权，请联系管理人员。")
							 + QString::fromLocal8Bit("<br>联系人：%1 &nbsp;&nbsp; 电话: %2 &nbsp;&nbsp; QQ: %3 ")
							 .arg(ASMgr->getSoftWareBaseInfo()->getDeveloper())
							 .arg(ASMgr->getSoftWareBaseInfo()->getTelephone())
							 .arg(ASMgr->getSoftWareBaseInfo()->getQQ())
							 + "<\html>",
							 QStringLiteral("确认"));
	}
}

void OdqMainWindow::slotOpen()
{
	/*处理当前工程，如果点击取消，则不重新打开文件*/
	if (slotDealCurProject() == QMessageBox::Cancel)
		return;

	slotChangeActState(false);//将相关action设置为不可编辑

	QString filePath = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("打开文件"),
		getCurrentPath(), QString::fromLocal8Bit("*.cl")); 
	if (!filePath.isEmpty())
	{
		this->open(filePath);
		this->slotWriteRecentFiles(filePath);
	}
}

//@ function:
void OdqMainWindow::open(QString path)
{
	if (!getVirBoxChecker()->canReadFlag)
		return;

	if (abs(getVirBoxChecker()->enlargeFactor - 1.0) > 0.00001
		|| abs(getVirBoxChecker()->virBoxPi - 3.1415926) > 0.00001)
		return;

	try
	{
		NewProject_Dlg::existPro();
		slotClearPro();

		path.replace("/", "\\");
		if (path.endsWith(".cl"))
		{
			//检查该文件夹是否存在，若不存在则新建文件夹
			QString proPath;
			QString proName;
			UIAutility::spliterPath(path, proPath, proName);
			NewProject_Dlg::checkAndCreateFolder(QString::fromLocal8Bit("间隙圆计算书"), proPath);
			NewProject_Dlg::checkAndCreateFolder(QString::fromLocal8Bit("间隙圆图纸"), proPath);
			NewProject_Dlg::checkAndCreateFolder(QString::fromLocal8Bit("文件"), proPath);
			_coreDbPt->setSavePath(path.toLocal8Bit().toStdString());

			if (!slotLoadParaLibFile(false))//判断是否正确读取了库文件
			{
				QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请您手动将缺少的库文件复制到工程路径中的“文件”菜单下，并重新打开该工程！"), QMessageBox::Yes, QMessageBox::Yes);
				return;
			}
				
			_coreDbPt->readFromJson(path.toLocal8Bit().toStdString());
			_isProNull = false;
			slotChangeActState(true);
			slotChangeWinTitle();
			emit crateProTreew();
		}
		else
		{
			QMessageBox::warning(nullptr, QString::fromLocal8Bit("文件类型错误"), QString::fromLocal8Bit("无法打开当前文件!"), QMessageBox::Ok);
			return;
		}
		statusBar()->showMessage(QString::fromLocal8Bit("打开成功，路径：") + path, 5000);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("打开文件"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
	}
}

//@function: 检查是否正确加载参数库文件，加载正确返回true，错误返回false   -- by mzq  2019/11/21 18:46
bool OdqMainWindow::slotLoadParaLibFile(bool isNewProject/*是否为新建工程*/)
{
	try
	{
		QString fileName = QCoreApplication::applicationDirPath();

		QString wirePath = QString::fromLocal8Bit(_coreDbPt->getPathFolder_File().c_str()) + QString("\\WireParameter");
		QString metePath = QString::fromLocal8Bit(_coreDbPt->getPathFolder_File().c_str()) + QString("\\MeteAreaTpl");
		QString gapPath = QString::fromLocal8Bit(_coreDbPt->getPathFolder_File().c_str()) + QString("\\GapSuggestedLib");
		QString Insulator_I_Path = QString::fromLocal8Bit(_coreDbPt->getPathFolder_File().c_str()) + QString("\\Insulator_I");
		QString Insulator_V_Path = QString::fromLocal8Bit(_coreDbPt->getPathFolder_File().c_str()) + QString("\\Insulator_V");
		QString Insulator_L_Path = QString::fromLocal8Bit(_coreDbPt->getPathFolder_File().c_str()) + QString("\\Insulator_L");
		QString WireAvgHPath = QString::fromLocal8Bit(_coreDbPt->getPathFolder_File().c_str()) + QString("\\WireAvgH");

		if (isNewProject)//如果是新建工程，则默认从软件目录下拷贝默认的库文件到工程目录下
		{
			/*QFile::copy 复制文件，如果目标路径中存在文件，则先删除*/
			if (QFile::exists(wirePath))
				QFile::remove(wirePath);
			if (QFile::exists(metePath))
				QFile::remove(metePath);
			if (QFile::exists(gapPath))
				QFile::remove(gapPath);
			if (QFile::exists(Insulator_I_Path))
				QFile::remove(Insulator_I_Path);
			if (QFile::exists(Insulator_V_Path))
				QFile::remove(Insulator_V_Path);
			if (QFile::exists(Insulator_L_Path))
				QFile::remove(Insulator_L_Path);
			if (QFile::exists(WireAvgHPath))
				QFile::remove(WireAvgHPath);

			QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\WireParameter"), wirePath);
			QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\MeteAreaTpl"), metePath);
			QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\GapSuggestedLib"), gapPath);
			QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\Insulator_I"), Insulator_I_Path);
			QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\Insulator_V"), Insulator_V_Path);
			QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\Insulator_L"), Insulator_L_Path);
			QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\WireAvgH"), WireAvgHPath);
			_coreDbPt->readParaLib();
			return true;
		}
		else//如果不是新建工程，则判断是否存在库文件，不存在，则提示用户是否拷贝
		{
			QFileInfo WireParameterFile(wirePath);
			bool isWireFile = WireParameterFile.isFile();
			QFileInfo MeteAreaTplFile(metePath);
			bool isMeteFile = MeteAreaTplFile.isFile();
			QFileInfo GapSuggestedLibFile(gapPath);
			bool isGapFile = GapSuggestedLibFile.isFile();
			QFileInfo Insulator_I_File(Insulator_I_Path);
			bool isInsulator_I_File = Insulator_I_File.isFile();
			QFileInfo Insulator_V_File(Insulator_V_Path);
			bool isInsulator_V_File = Insulator_V_File.isFile();
			QFileInfo Insulator_L_File(Insulator_L_Path);
			bool isInsulator_L_File = Insulator_L_File.isFile();
			QFileInfo WireAvgHFile(WireAvgHPath);
			bool isWireAvgHFile = WireAvgHFile.isFile();

			if (isWireFile && isMeteFile && isGapFile && isInsulator_I_File && isInsulator_V_File && isInsulator_L_File && isWireAvgHFile) //如果都存在，则直接读取
			{
				_coreDbPt->readParaLib();
				return true;
			}
			else//有一个不存在，则提示用户，并让用户决定是否考入默认库数据
			{
				QString infoQStr = QString::fromLocal8Bit("工程路径中的“文件”目录下无以下库文件：");
				if (!isWireFile)
					infoQStr += QString::fromLocal8Bit("\n导线参数库文件：WireParameter");
				if (!isMeteFile)
					infoQStr += QString::fromLocal8Bit("\n气象库文件：MeteAreaTpl");
				if (!isGapFile)
					infoQStr += QString::fromLocal8Bit("\n间隙值库文件：GapSuggestedLib");
				if (!isInsulator_I_File)
					infoQStr += QString::fromLocal8Bit("\n绝缘子串I串参数库文件：Insulator_I");
				if (!isInsulator_V_File)
					infoQStr += QString::fromLocal8Bit("\n绝缘子串V串参数库文件：Insulator_V");
				if (!isInsulator_L_File)
					infoQStr += QString::fromLocal8Bit("\n绝缘子串L串参数库文件：Insulator_L");
				if (!isWireAvgHFile)
					infoQStr += QString::fromLocal8Bit("\n导线平均高设置文件：WireAvgH");
				infoQStr += QString::fromLocal8Bit("\n是否将系统默认的库文件拷贝到“文件”目录下？");

				QMessageBox::StandardButton returnBtn = QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), infoQStr, QMessageBox::Yes | QMessageBox::No);
				if (QMessageBox::Yes == returnBtn)
				{
					if (!isWireFile)
						QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\WireParameter"), wirePath);
					if (!isMeteFile)
						QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\MeteAreaTpl"), metePath);
					if (!isGapFile)
						QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\GapSuggestedLib"), gapPath);
					if (!isInsulator_I_File)
						QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\Insulator_I"), Insulator_I_Path);
					if (!isInsulator_V_File)
						QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\Insulator_V"), Insulator_V_Path);
					if (!isInsulator_L_File)
						QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\Insulator_L"), Insulator_L_Path);
					if (!isWireAvgHFile)
						QFile::copy(QCoreApplication::applicationDirPath() + QString("\\initialFile\\WireAvgH"), WireAvgHPath);

					_coreDbPt->readParaLib();
					return true;
				}
				else if (QMessageBox::No == returnBtn)
				{
					return false;
				}
			}	
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("加载参数库文件"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
	}
	return false;
}

//保存文件:
void OdqMainWindow::slotSave()
{
	try
	{
		//保存界面数据
		{
			IqConsole* iConsole = getIConsole();
			if (!iConsole)
			{
				ODA_FAIL();
				return;
			}
			QString qsCmd = QString("savewidgetdata ");
			iConsole->executeCommand(qsCmd); // 保存界面数据
		}

		_coreDbPt->sortByClName(_pClListWidget->getclNameVec());
		_coreDbPt->save();
		_coreDbPt->writeParaLib();
		auto path = QString::fromLocal8Bit(_coreDbPt->getSavePath().c_str());
		statusBar()->showMessage(QString::fromLocal8Bit("保存成功，路径：") + path, 5000);
	}
	catch (std::exception& e){
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("Save"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
	}
}

void OdqMainWindow::slotSaveAs()
{
	QString defaultStr = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	QString filePath = QFileDialog::getSaveFileName(nullptr, QString::fromLocal8Bit("保存文件"), defaultStr, "*.cl");

	if (!filePath.isEmpty())
	{
		filePath.replace("/", "\\");
		QString proPath;
		QString proName;
		UIAutility::spliterPath(filePath, proPath, proName);

		//检查该名称的项目在本地存在
		if (NewProject_Dlg::checkProExisted(proName, proPath))
			return;

		//创建文件夹
		NewProject_Dlg::createProFileFolder(proName, proPath);
		try
		{
			//保存界面数据
			{
				IqConsole* iConsole = getIConsole();
				if (!iConsole)
				{
					ODA_FAIL();
					return;
				}
				QString qsCmd = QString("savewidgetdata ");
				iConsole->executeCommand(qsCmd); // 保存界面数据
			}
			QString pathNameQStr = proPath + "\\" + proName + "\\" + proName + ".cl";
			slotWriteRecentFiles(pathNameQStr);
			_coreDbPt->setSavePath(pathNameQStr.toLocal8Bit().toStdString());

			if (filePath.endsWith(".cl"))
			{
				_coreDbPt->sortByClName(_pClListWidget->getclNameVec());

				_coreDbPt->writeToJson(pathNameQStr.toLocal8Bit().toStdString());
				_coreDbPt->writeParaLib();
				emit reshTreeProName();
			}
			else
			{
				QMessageBox::warning(nullptr, QString::fromLocal8Bit("文件类型有误"), QString::fromLocal8Bit("该种文件类型无法保存!"), QMessageBox::Ok);
				return;
			}
			statusBar()->showMessage(QString::fromLocal8Bit("另存为成功，路径：") + filePath, 5000);
			slotChangeWinTitle();
		}
		catch (std::exception& e){
			QMessageBox::warning(nullptr, QString::fromLocal8Bit("另存为"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
		}
	}
}

// @function: 新建或者打开原有工程的时候，处理当前工程  -mzq 2019/8/14 15:10
QMessageBox::StandardButton OdqMainWindow::slotDealCurProject()
{
	/*处理当前工程*/
	if (0 != _coreDbPt->getClCount())
	{
		QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否保存当前工程"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if (rb == QMessageBox::Yes)
		{
			IqConsole* iConsole = getIConsole();
			if (!iConsole)
			{
				ODA_FAIL();
				return QMessageBox::Cancel;
			}
			QString qsCmd = QString("savewidgetdata ");
			iConsole->executeCommand(qsCmd);
			_coreDbPt->save();
			slotClearPro();

		}
		else if (rb == QMessageBox::No)
		{
			slotClearPro();

		}

		return rb;
	}

	return QMessageBox::Ignore;
}

// @ function: 清除工程（包括工程树和间隙圆界面）
void OdqMainWindow::slotClearPro()
{
	emit clearTree();
	emit clearClWidget();
	_coreDbPt->clearCl();
}

// @function: 新建打开文件等操作后更改按钮状态  -mzq 2019/5/25 19:50
void OdqMainWindow::slotChangeActState(bool state)
{
	_operatMenu->setEnabled(state);//操作菜单
	_pParameterMenu->setEnabled(state);//参数菜单

	_saveAct->setEnabled(state);
	_saveAsAct->setEnabled(state);
	_openFilePathAct->setEnabled(state);

	_wireParameterAct->setEnabled(state);
	_meteAreaAct->setEnabled(state);
	_sgstGapAct->setEnabled(state);
	_insulatorLibAct->setEnabled(state);
	_wireAvgHLibAct->setEnabled(state);

	_pAddClAct->setEnabled(state);
	_straight_I_Act->setEnabled(state);
	_straight_V_Act->setEnabled(state);
	_straight_L_Act->setEnabled(state);
	_maxAngle_I_Act->setEnabled(state);
	_maxAngle_V_Act->setEnabled(state);

	_drawAllClAct->setEnabled(state);
	_settingsDlgAct->setEnabled(state);
}

// @function: 关于  -mzq 2019/5/25 20:43
void OdqMainWindow::slotAbout()
{
	AboutSoftWare::About_Dlg dlg(this);
	dlg.exec();
}

//@function: 更新日志   -- by mzq 2024/1/16 16:43
void OdqMainWindow::slotUpdateLogDlg()
{
	AboutSoftWare::UpDateInfo_Dlg dlg(AboutSoftWare::Module::All, false, false, this);
	dlg.exec();
}

// @function: 帮助  -mzq 2019/5/25 20:44
void OdqMainWindow::slotHelp()
{
	QString appDirPathStr = QCoreApplication::applicationDirPath();
	QString filePath = appDirPathStr + "/help.pdf";
	//QString filePath = appDirPathStr + "/Jumper_help.chm";
	QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

//@function:    -- by yzhchc  2019/11/15 16:23
void OdqMainWindow::slotChangestyle()
{
	QAction *act = (QAction*)sender();
	std::string name = act->text().toLocal8Bit();
	QString qssFile = "";
	if (name == "灰白色")
	{
		_qssType = QssType(White);
		_skinMenu->actions().at(0)->setChecked(true);
		_skinMenu->actions().at(1)->setChecked(false);
		_skinMenu->actions().at(2)->setChecked(false);
		qssFile = ":/images/White.qss";
	}
	else if (name == "浅蓝色")
	{
		_qssType = QssType(Blue);
		_skinMenu->actions().at(0)->setChecked(false);
		_skinMenu->actions().at(1)->setChecked(true);
		_skinMenu->actions().at(2)->setChecked(false);
		qssFile = ":/images/Light.qss";
	}
	else if (name == "暗黑色")
	{
		_qssType = QssType(Dark);
		_skinMenu->actions().at(0)->setChecked(false);
		_skinMenu->actions().at(1)->setChecked(false);
		_skinMenu->actions().at(2)->setChecked(true);
		qssFile = ":/images/Dark.qss";
		
	}
	if (!qssFile.endsWith(".qss"))qssFile += "White.qss";
	setStyle(qssFile);
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	settings.setValue("skin", _qssType); // 肤色
}

//@function:    -- by yzhchc  2019/11/15 16:32
void OdqMainWindow::setStyle(const QString &qssFile)
{
	QFile file(qssFile);
	if (file.open(QFile::ReadOnly))
	{
		QString qss = QLatin1String(file.readAll());
		qApp->setStyleSheet(qss);
		file.close();
	}
}

//@function: 更改标题栏   -- by mzq  2019/6/13 21:13
void OdqMainWindow::slotChangeWinTitle()
{
	this->setWindowTitle(QStringLiteral("SmartClearance   ") + QString::fromLocal8Bit(_coreDbPt->getSavePath().c_str()));
}

void OdqMainWindow::slotShowWPA()
{
	if (0 == _coreDbPt->getClCount())
	{
		QMessageBox::warning(NULL,
							 QString::fromLocal8Bit("提示"),
							 QString::fromLocal8Bit("请至少创建一个间隙圆后再查看参数库"),
							 QMessageBox::Yes);
		return;
	}
	IqConsole* iConsole = getIConsole();
	if (!iConsole)
	{
		ODA_FAIL();
		return;
	}
	QString qsCmd = QString("openwireparadlg ");
	iConsole->executeCommand(qsCmd);//导线参数对话框	
}

void OdqMainWindow::slotShowcurMeteArea()
{
	if (0 == _coreDbPt->getClCount())
	{
		QMessageBox::warning(NULL,
							 QString::fromLocal8Bit("提示"),
							 QString::fromLocal8Bit("请至少创建一个间隙圆后再查看参数库"),
							 QMessageBox::Yes);
		return;
	}
	IqConsole* iConsole = getIConsole();
	if (!iConsole)
	{
		ODA_FAIL();
		return;
	}
	QString qsCmd = QString("openmeteareadlg ");
	iConsole->executeCommand(qsCmd);//气象区参数对话框
}

//@function: 显示间隙值推荐库   -- by mzq  2019/8/8 15:21
void OdqMainWindow::slotShowSgstGap()
{
	if (0 == _coreDbPt->getClCount())
	{
		QMessageBox::warning(NULL,
							 QString::fromLocal8Bit("提示"),
							 QString::fromLocal8Bit("请至少创建一个间隙圆后再查看参数库"),
							 QMessageBox::Yes);
		return;
	}
	IqConsole* iConsole = getIConsole();
	if (!iConsole)
	{
		ODA_FAIL();
		return;
	}
	QString qsCmd = QString("opensgstgapdlg ");
	iConsole->executeCommand(qsCmd);//间隙值库对话框
}

//@function: 显示绝缘子串库   -- by mzq  2019/12/30 9:19
void OdqMainWindow::slotShowInsulatorLib()
{
	if (0 == _coreDbPt->getClCount())
	{
		QMessageBox::warning(NULL,
							 QString::fromLocal8Bit("提示"),
							 QString::fromLocal8Bit("请至少创建一个间隙圆后再查看参数库"),
							 QMessageBox::Yes);
		return;
	}
	IqConsole* iConsole = getIConsole();
	if (!iConsole)
	{
		ODA_FAIL();
		return;
	}
	QString qsCmd = QString("openinsulatorlib ");
	iConsole->executeCommand(qsCmd);//间隙值库对话框
}

//@function: 显示导线平均高库   -- by mzq  2020/1/19 10:36
void OdqMainWindow::slotShowWireAvgHLib()
{
	if (0 == _coreDbPt->getClCount())
	{
		QMessageBox::warning(NULL,
							 QString::fromLocal8Bit("提示"),
							 QString::fromLocal8Bit("请至少创建一个间隙圆后再查看参数库"),
							 QMessageBox::Yes);
		return;
	}

	IqConsole* iConsole = getIConsole();
	if (!iConsole)
	{
		ODA_FAIL();
		return;
	}
	QString qsCmd = QString("openwireavghlib ");
	iConsole->executeCommand(qsCmd);//导线平均高库对话框
}

//@function: 打开当前工程保存路径   -- by mzq  2019/9/2 14:12
void OdqMainWindow::slotOpenFilePath()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromLocal8Bit(_coreDbPt->getPathFolder().c_str())));
}

//@function: 绘制所有间隙元以及生成计算书   -- by mzq  2019/9/2 17:59
void OdqMainWindow::slotDrawAllCl()
{
	if (_coreDbPt->getClCount() == 0)
	{
		QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("当前工程间隙圆数量为零！"), QMessageBox::Ok);
		return;
	}

	slotSave();//保存当前工程
	emit signalDrawAllClActClicked();//提交cllistwidget进行绘制

	QMessageBox::StandardButton rb = QMessageBox::question(nullptr, QString::fromLocal8Bit("提示"),
		QString::fromLocal8Bit("间隙圆图纸及计算书已生成，是否打开工程目录进行查看?\n\n*注意检查某些间隙圆可能因为参数错误导致无法生成图纸及计算书。"),
		QMessageBox::Yes | QMessageBox::No);
	if (rb == QMessageBox::Yes)
	{
		slotOpenFilePath();
	}
}

//@function: 显示绘图设置对话框   -- by mzq 2022/12/3 10:41
void OdqMainWindow::slotShowSettingsDlg()
{
	SettingsDlg dlg(this);
	dlg.exec();
}