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
// OdqMainWindow.h
//

#ifndef ODQ_MAIN_WINDOW_H_
#define ODQ_MAIN_WINDOW_H_

#include <QMainWindow>

QT_BEGIN_NAMESPACE
  class QTextBrowser;
  class QUrl;
  //class QProgressBar;
  QT_END_NAMESPACE

#include "OdaCommon.h"
#include "SharedPtr.h"
#include "QItemSelectionModel"
#include "DataBase.h"
#include "qmessagebox.h"
#include "VirBox\VirBoxChecker.h"

class ClListWidget;
class CommentsWidget;
class MainModuleWidget;
class ReportErrWidget;
class ReportErrModel;
class QTableView;
class QTextEdit;
class QMdiArea;

class OdqMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  OdqMainWindow();
  virtual ~OdqMainWindow();

  bool isValid() const;
  bool isClosed() const; // by closeEvent

  class OdRxModule* getAppModule();

  bool showUrl(const QString& qsUrl,
               const QString& qsTitle = "%1", // by app name
               bool bModal = true);
  bool isBrowserClosed() const;

  void saveWidgetPlacement(QWidget* pWidget, 
                           const QString qsKey = "") const;  // for QMainWindow
  void restoreWidgetPlacement(QWidget* pWidget, 
                              const QString qsKey = "", // for QMainWindow
                              int widthDefault = 600,
                              int heightDefault = 200) const;

  bool saveQObjectProperties(QObject* pObj,
                             const QString& qsProps = "", // an empty for all
                             const QString& qsSkipProps = "",
                             const QString& qsDelim = " ") const;
  bool restoreQObjectProperties(QObject* pObj,
                                const QString& qsProps = "", // an empty for all
                                const QString& qsSkipProps = "",
                                const QString& qsDelim = " ") const;

private:
  bool initAppModule();
  void uninitAppModule();
  //class OdqServices* getServices() const;

protected:
  //virtual void showEvent(QShowEvent* pEvent);
  //virtual void paintEvent(QPaintEvent* pEvent);
  virtual void closeEvent(QCloseEvent* pEvent);
  virtual void dragEnterEvent(QDragEnterEvent* pEvent);
  //virtual void dragMoveEvent(QDragMoveEvent* pEvent);
  ////virtual void dragLeaveEvent(QDragLeaveEvent* pEvent);
  virtual void dropEvent(QDropEvent* pEvent);

public slots:
  void revertConsoleVisibility();
  void showHelp();
  void changedBrowserSource(const QUrl& url);
  void clickedBrowserAnchor(const QUrl& url);
  void clickedBrowserCloseButton();
  void unlockCommandViaBrowser();

private:
  SmartCl::DataBase*		     _coreDbPt; //内核数据库指针存放，便于其他项目调用
  AboutSoftWare::VirBoxChecker*  _virBoxCheckerInstance; //深思加密检查指针
  bool m_bValid;
  OdRxModule* m_pAppModule;
  OdSharedPtr<QTextBrowser> m_pBrowser;
  // is used to block duplicate commands via Url 
  // (cleared via unlockCommandViaBrowser by QTimer::singleShot)
  QString m_qsBrowserLastCommand; 
  bool m_bDragging;

public:
	SmartCl::DataBase* getCoreDbPt() const { return _coreDbPt; }
	AboutSoftWare::VirBoxChecker *getVirBoxChecker(){ return _virBoxCheckerInstance; };

signals:
	void errDoubleClicked(int);
	void crateProTreew();
	void reshTreeProName();
	void clearTree();
	void clearClWidget();
	void errGoto(int errType);
	void testVW(QString strOut);
	void signalDrawAllClActClicked();//绘制所有间隙圆

public:
	void moduleEnabled();//设置模块是否可用

public slots:
	void slotSetWidgetEditable(bool flag);//设置界面是否可用

private slots:

	void slotOpen();
	void open(QString path);
	void slotSaveAs();
	void slotSave();
	QMessageBox::StandardButton slotDealCurProject();//新建或者打开原有工程的时候，处理当前工程
	void slotChangeActState(bool state);//新建打开文件等操作后更改按钮状态
	void slotAddStraight_I();
	void slotAddStraight_V();
	void slotAddStraight_L();
	void slotAddMaxAngle_I();
	void slotAddMaxAngle_V();
	void slotAddCl();//添加间隙圆
	void slotAddClByClType(SmartCl::ClCate::Type clType);//通过间隙圆类别添加间隙圆
	void slotNewProject();//新建工程
	bool slotLoadParaLibFile(bool isNewProject/*是否为新建工程*/);//检查是否正确加载参数库文件，加载正确返回true，错误返回false
	void slotOpenRecentFile(QAction*);
	void slotUpdateRecentFileActions();
	void slotWriteRecentFiles(const QString& filePath);//写入最近文件到注册表
	void slotClearPro();//清除工程
	void slotShowWPA();//显示导线参数区
	void slotShowcurMeteArea();//显示气象参数区
	void slotShowSgstGap();//显示间隙值推荐库
	void slotShowInsulatorLib();//显示绝缘子串库
	void slotShowWireAvgHLib();//显示导线平均高库
	void slotAbout();//关于
	void slotUpdateLogDlg();//更新日志
	void slotHelp();//帮助
	void slotChangestyle();//换肤
	void slotChangeWinTitle();//更改标题栏
	void slotOpenFilePath();//打开当前工程保存路径
	void slotDrawAllCl();//绘制所有间隙元以及生成计算书
	void slotShowSettingsDlg();//显示绘图设置对话框

protected:
	void initDockWidget(QMdiArea* pMdiArea);
	void creatAction();
	QMenu* initRecentMenus(QMenu *parent/*父指针*/);
	QString getCurrentPath();//
	void readRecentFile();//从注册表读入最近文件
	void loadSkinQss(); // 加载换肤Qss
	void setStyle(const QString &qssFile);//设置样式

private:
	enum { MaxRecentFiles = 10 };
	enum QssType {White = 0,Blue,Dark};
	QAction                  *_recentFileActs[MaxRecentFiles];//存放最近文件路径的action
	QAction                  *_recentFileSubMenuAct;//最近文件的二级菜单
	bool				      _isProNull;//当前是否有项目

	QAction                  *_newProAct;
	QAction                  *_openAct;
	QAction                  *_saveAct;
	QAction                  *_saveAsAct;
	QAction                  *_openFilePathAct;//打开当前工程文件目录

	QAction                  *_pAddClAct;
	QAction                  *_straight_I_Act;
	QAction                  *_straight_V_Act;
	QAction                  *_straight_L_Act;
	QAction                  *_maxAngle_I_Act;
	QAction                  *_maxAngle_V_Act;
	QAction                  *_drawAllClAct;//绘制所有间隙圆以及生成计算书
	QAction                  *_settingsDlgAct;

	QMenu                    *_operatMenu;//操作菜单
	QMenu                    *_pParameterMenu;//参数菜单

	QAction                  *_wireParameterAct;
	QAction                  *_meteAreaAct;
	QAction                  *_sgstGapAct;
	QAction                  *_insulatorLibAct;
	QAction                  *_wireAvgHLibAct;

	ClListWidget             *_pClListWidget;//左侧栏树状列表
	QStringList	             _recentFileList;//存放最近打开文件的路径
	QMenu					 *_skinMenu;//换肤
	QssType                   _qssType;

};

#endif // ODQ_MAIN_WINDOW_H_