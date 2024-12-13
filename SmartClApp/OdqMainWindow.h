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
  SmartCl::DataBase*		     _coreDbPt; //�ں����ݿ�ָ���ţ�����������Ŀ����
  AboutSoftWare::VirBoxChecker*  _virBoxCheckerInstance; //��˼���ܼ��ָ��
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
	void signalDrawAllClActClicked();//�������м�϶Բ

public:
	void moduleEnabled();//����ģ���Ƿ����

public slots:
	void slotSetWidgetEditable(bool flag);//���ý����Ƿ����

private slots:

	void slotOpen();
	void open(QString path);
	void slotSaveAs();
	void slotSave();
	QMessageBox::StandardButton slotDealCurProject();//�½����ߴ�ԭ�й��̵�ʱ�򣬴���ǰ����
	void slotChangeActState(bool state);//�½����ļ��Ȳ�������İ�ť״̬
	void slotAddStraight_I();
	void slotAddStraight_V();
	void slotAddStraight_L();
	void slotAddMaxAngle_I();
	void slotAddMaxAngle_V();
	void slotAddCl();//��Ӽ�϶Բ
	void slotAddClByClType(SmartCl::ClCate::Type clType);//ͨ����϶Բ�����Ӽ�϶Բ
	void slotNewProject();//�½�����
	bool slotLoadParaLibFile(bool isNewProject/*�Ƿ�Ϊ�½�����*/);//����Ƿ���ȷ���ز������ļ���������ȷ����true�����󷵻�false
	void slotOpenRecentFile(QAction*);
	void slotUpdateRecentFileActions();
	void slotWriteRecentFiles(const QString& filePath);//д������ļ���ע���
	void slotClearPro();//�������
	void slotShowWPA();//��ʾ���߲�����
	void slotShowcurMeteArea();//��ʾ���������
	void slotShowSgstGap();//��ʾ��϶ֵ�Ƽ���
	void slotShowInsulatorLib();//��ʾ��Ե�Ӵ���
	void slotShowWireAvgHLib();//��ʾ����ƽ���߿�
	void slotAbout();//����
	void slotUpdateLogDlg();//������־
	void slotHelp();//����
	void slotChangestyle();//����
	void slotChangeWinTitle();//���ı�����
	void slotOpenFilePath();//�򿪵�ǰ���̱���·��
	void slotDrawAllCl();//�������м�϶Ԫ�Լ����ɼ�����
	void slotShowSettingsDlg();//��ʾ��ͼ���öԻ���

protected:
	void initDockWidget(QMdiArea* pMdiArea);
	void creatAction();
	QMenu* initRecentMenus(QMenu *parent/*��ָ��*/);
	QString getCurrentPath();//
	void readRecentFile();//��ע����������ļ�
	void loadSkinQss(); // ���ػ���Qss
	void setStyle(const QString &qssFile);//������ʽ

private:
	enum { MaxRecentFiles = 10 };
	enum QssType {White = 0,Blue,Dark};
	QAction                  *_recentFileActs[MaxRecentFiles];//�������ļ�·����action
	QAction                  *_recentFileSubMenuAct;//����ļ��Ķ����˵�
	bool				      _isProNull;//��ǰ�Ƿ�����Ŀ

	QAction                  *_newProAct;
	QAction                  *_openAct;
	QAction                  *_saveAct;
	QAction                  *_saveAsAct;
	QAction                  *_openFilePathAct;//�򿪵�ǰ�����ļ�Ŀ¼

	QAction                  *_pAddClAct;
	QAction                  *_straight_I_Act;
	QAction                  *_straight_V_Act;
	QAction                  *_straight_L_Act;
	QAction                  *_maxAngle_I_Act;
	QAction                  *_maxAngle_V_Act;
	QAction                  *_drawAllClAct;//�������м�϶Բ�Լ����ɼ�����
	QAction                  *_settingsDlgAct;

	QMenu                    *_operatMenu;//�����˵�
	QMenu                    *_pParameterMenu;//�����˵�

	QAction                  *_wireParameterAct;
	QAction                  *_meteAreaAct;
	QAction                  *_sgstGapAct;
	QAction                  *_insulatorLibAct;
	QAction                  *_wireAvgHLibAct;

	ClListWidget             *_pClListWidget;//�������״�б�
	QStringList	             _recentFileList;//���������ļ���·��
	QMenu					 *_skinMenu;//����
	QssType                   _qssType;

};

#endif // ODQ_MAIN_WINDOW_H_