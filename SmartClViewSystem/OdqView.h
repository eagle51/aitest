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
// OdqView.h
//

#ifndef ODA_QT_VIEW_H_
#define ODA_QT_VIEW_H_


#include <QMdiSubWindow>
#include <QScrollArea>
#include <QGLWidget>
#include <QPointer>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#include <QGesture>
#endif

QT_BEGIN_NAMESPACE
  class QGroupBox;
  class QTabBar; //class QTabWidget;
  class QDialog;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "DbEntity.h"
#include "Gs/Gs.h"
#include "Ed/EdUserIO.h"

#include "DbUserIO.h" // TODO DbBaseDatabaseIO.h like DbBaseDatabase.h //#include "Ed/EdUserIO.h"
#define OdDbBaseDatabaseIO OdDbUserIO
typedef OdSmartPtr<OdDbBaseDatabaseIO> OdDbBaseDatabaseIOPtr;

class OdDbStub; //#include "OdDbStub.h"

#include "OdqInterfaces.h"
#include "ClFormFiles/ClWidget.h"
#include "EnumLib.h"
class OsgQt;//#include "OsgQt.h"

//////////////////////////////////////////////////////////////////////////

typedef enum {
  enBusy_UninitializedGL = 0 // step 1 - init
, enBusy_InitializeGL
, enBusy_UnresizedGL     // step 2 - resize
, enBusy_ResizeGL
, enBusy_UnpaintGL       // step 3 - paint
, enBusy_PaintGL
, enBusy_Free            // ready to repaint
, enBusy_Closed
} TEnViewBusy;

class OdqRenderArea : public QGLWidget
{
  Q_OBJECT
public:
  OdqRenderArea(QWidget* pParent, bool bMaximized = false, TEnViewBusy step = enBusy_UninitializedGL);
  virtual ~OdqRenderArea();

  class OdqView* getView(); 
  QImage& getImageFrame();
  bool isImageFrameUsed() const;
  bool isResized() const;

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
protected:
  QSize sizeHint(int width, int heigth) const;

  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  static QGLFormat getQGLFormat();

  virtual void mouseMoveEvent(QMouseEvent* pEvent);
  virtual void mousePressEvent(QMouseEvent* pEvent);
  virtual void mouseReleaseEvent(QMouseEvent* pEvent);
  virtual void mouseDoubleClickEvent(QMouseEvent* pEvent);
  virtual void wheelEvent(QWheelEvent* pEvent);
  // keyPressEvent and keyReleaseEvent are used via OdqView

  virtual bool event(QEvent* pEvent);
  virtual void resizeEvent(QResizeEvent* pEvent);

  QPointer<OdqView> m_pView;
  bool m_bUseImageFrame;
  GLuint m_idTexture;
  QImage m_imageFrame; // used for Caustic device only

  TEnViewBusy m_enBusy; friend class OdqView; friend class _AutoUnlocker;
  bool m_bMaximized,
       m_bResized, // set on (after resize) by paint gl with already valid gs device 
       m_bSkipFirstResizeGL,
       m_isLastResizeGL;
  int m_widthLastResizeGL,
      m_heightLastResizeGL,
      m_levelResizeGL;
  qreal totalScaleFactor;

  // QMutex works between threads and is useless here after it
};

//////////////////////////////////////////////////////////////////////////
class OdqView : public QMdiSubWindow
              , public IqView
{
  Q_OBJECT
  Q_INTERFACES(IqView)
  OdqView(QMdiArea* pMdiArea,
          OdRxObject* pRxDatabase,
          OdDbStub* idLayoutView,
          const OdGsDCRect* pRect,
          bool bMaximized,
          TEnViewBusy step);
public:
  static OdqView* create(QMdiArea* pMdiArea, 
                         OdRxObject* pRxDatabase = NULL, 
                         OdDbStub* idLayoutView = NULL, // active if NULL
                         const OdGsDCRect* pRect = NULL,
                         bool bMaximized = false,
                         TEnViewBusy step = enBusy_UninitializedGL);
  virtual ~OdqView();

  TEnViewBusy getBusyState() const;

  virtual bool event(QEvent* pEvent);
  virtual void closeEvent(QCloseEvent *pEvent);
  virtual void changeEvent(QEvent* pEvent);

  
  // ----- IqView -----

  virtual OdRxObject* getRxDatabase();
  virtual void updateLayouts();

  virtual bool emitClose();

  virtual OdGsDevice* getGsDevice(); // the device to render the active layout (is a OdGsLayoutHelper) 
  virtual OdGsView* getActiveGsView();
  virtual OdGsView* getActiveTopGsView();
  virtual OdGsModel* getGsModel(bool b2d = false);

  virtual QCursor cursor(); 
  virtual void setCursor(const QCursor& cursor); 

  bool getUcsPlane(OdGePlane& plane, const OdGeVector3d* pvHintDirToIntersect = NULL);
    // convert eyes point to UCS(by empty OdEd::kGptNoUCS flag in option) or to WCS in eye plane
  virtual bool eyesTo(const QPoint& ptEyes, // in 
                      int options, // in 
                      OdGePoint3d& ptOut, // out
                      const OdGePlane* pUcsPlane = NULL); // in
  virtual bool wcsToScreen(const OdGePoint3d& wcsPt,
                           OdGePoint2d& ptScr); // out

  virtual ODCOLORREF backgroundColor() const;
  virtual bool setBackgroundColor(ODCOLORREF clrBackground);
  virtual void setViewportBorderProperties();

  virtual QWidget* widget(); 
  virtual void updateGL(); // repaint
  virtual void setActive();
  // for Android only
  virtual bool isFullScreenMode(); 
  virtual bool isFullScreenActive();
  bool isResizeActive();

  static class OdqViewSystem* getViewSystem();

  void clearDevice();
  void resetDevice(bool isModel);
  bool isModelLayout();

  void saveSettings();

  void subscribeTimer(OdEdIntegerTracker* pTracker, int msec = 1000);
  void unsubscribeTimer(OdEdIntegerTracker* pTracker);

  void updateObject(OdDbStub* id);
  void highlightObject(bool bValue, OdDbStub* id, const OdDbBaseFullSubentPath* pPath);
  void onModifiedGrip(OdGiDrawablePtr pGripData);
  void highlightGripPoint(bool bValue, OdGiDrawablePtr pGripData);
  void highlightSnapPoint(bool bValue, OdDbStub* id, const OdDbBaseFullSubentPath* pPath, 
                          OdDb::OsnapMode osm, const OdGePoint3d& point);
protected:
  virtual void keyPressEvent(QKeyEvent* pEvent);
  virtual void keyReleaseEvent(QKeyEvent* pEvent);

signals:
  void mouseMove(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void mousePress(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void mouseRelease(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void mouseDoubleClick(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void mouseWheel(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void keyPress(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void keyRelease(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void touchPressed(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void touchMove(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void touchReleased(IqView* iView, QEvent* pEvent, bool& bAccepted);

  void activateTree(QString strTitleName);

private:
  OdRxObject* m_pRxDatabase;
  OdDbStub* m_idLayoutView; // used for DGN only
  bool m_bDoZoomExtents;

  // the device to render the active layout (is a OdGsLayoutHelper) 
  friend class OdqRenderArea;
  OdGsDevicePtr m_pGsDevice; 
  OdGsModelPtr m_pGsModel2d;

  QPointer<OdqRenderArea> m_pRenderArea;
  //QPointer<QGroupBox> m_pInternalWidget;
  QPointer<QTabBar> m_pTabLayout;
  int m_indexTabLayoutPrev;

  QColor m_clrBack,
         m_clrBackSel,
         m_clrFore;

  OdEdIntegerTracker* m_pTrackerOnTimer;
  int m_intervalTimer;
  bool m_isByClickIfCloseEvent;

  OdSharedPtr<OdGsDCRect> m_pRect; friend class OdqCmd_SysWindows;
  OdSharedPtr<QSize> m_pSize;
  //////////////////////////////////////////////////////////////////////////
  private slots:
  void changeLayout(int index);
  void timerUpdate();

#pragma region 单文档自定义界面代码
public:
	QWidget* initLeftWgt(QWidget *parent/*父窗口*/);//左侧间隙圆界面窗口
	QWidget* showAreaWgt(QWidget *parent/*父窗口*/);//图像显示区
	QWidget* teighaCadWgt(QWidget *parent/*父窗口*/);//CAD绘图区Gbox
	const HdlClWidget getClWidget() const { return _ClWidget; }

	void saveWidgetData();//保存界面数据
	void drawCl();//绘制间隙圆及生成计算书

private slots:
	void slotChangeTabWidgetIndex(int index/*图形tab页面下标*/);//切换当前图形显示tabwidget至相应的页面

private:
	QTabWidget                    *_showAreaTabWgt;//图像显示区tab页面
	SmartCl::ClCate::Type          _clType;//间隙圆类型
	HdlClWidget                    _ClWidget;//左侧间隙圆串窗口
	
	OsgQt                         *_osgWidget;//Osg窗口
	QToolBar                      *_cadToolBar;//CAD绘图区工具栏

#pragma endregion
};
#endif // ODA_QT_VIEW_H_