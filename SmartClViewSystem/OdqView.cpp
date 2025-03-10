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
// OdqView.cpp
//
#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#include <QGesture>
#include <QtOpenGL>
#include <QGLWidget>
#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#include <OpenGLES/ES2/gl.h>
#endif
#if defined(ANDROID)
#include <QTouchEvent>
#endif
#endif
#include <QTabWidget>
#include <QVector>

#include "OdaCommon.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeRay3d.h"
#include "Ed/EdCommandContext.h"
#include "DbEntity.h" // for dDbGripDataPtrArray
#include "DbGrip.h"

#include "DbBaseDatabase.h"
#include "GiContextForDbDatabase.h"
#include "DbGsManager.h"
#include "RxVariantValue.h"
#include "ColorMapping.h"
#include "Gs/GsBaseVectorizer.h"
#include "AbstractViewPE.h"
#include "DbAbstractViewportData.h"
#include "DbLayout.h"
#include "TDVersion.h"

#include "OdqViewSystem.h"
#include "OdqView.h"
#include "ExtDbModule.h"
#include "ExGripManager.h" //#include "ExGripData.h"
#include "ExDynamicModule.h" // for ODRX_SELECTION_SET

//#if defined(ANDROID_ON) && defined(_DEBUG)
//  //#define TRC(a) do { getIApp()->toStatusBar(a, 3000); } while (false)
//  //#define TRC(a) do { qDebug() << a; } while (false)
//  #define TRC(a) do { qDebug() << a; ODA_TRACE(getIApp()->toOdString(a).c_str()); getIApp()->toStatusBar(a, 3000); } while (false)
//  //#define TRC(a) do { ODA_TRACE(getIApp()->toOdString(a).c_str()); } while (false)
//  //#define TRC(a) do { ODA_TRACE(getIApp()->toOdString(a).c_str()); getIApp()->toStatusBar(a, 3000); } while (false)
//  // bad way after processEvents ////#define TRC(a) do { getIConsole()->putString(a); qApp->processEvents(); } while (false)
//#else
  #define TRC(a)
//#endif

//#define OD_ANDROID_TRACEDEFS

#ifdef OD_ANDROID_TRACEDEFS
#ifdef ANDROID
#include <android/log.h>
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  "OdqView", __VA_ARGS__)
#else
#undef OD_ANDROID_TRACEDEFS
#endif
#endif
#include "Clearence.h"
#include "DataBase.h"
#include "ClFormFiles/StraightWgt.h"
#include <qwidget.h>

#include "../SmartClApp/OdqMainWindow.h"
#include "StraightWgt.h"
#include <tchar.h>
#include "DbCommandContext.h"
#include "OsgQt.h"
#include "ClWidgetFactory.h"
#include "MaxAngleWgt.h"
#include "qnamespace.h"
#include "QAbstractScrollArea"
//////////////////////////////////////////////////////////////////////////

// static
OdqViewSystem* OdqView::getViewSystem()
{
  QPointer<OdqViewSystem> s_pVS;

  if (!s_pVS.isNull())
    return s_pVS;

  s_pVS = qobject_cast<OdqViewSystem*>(getIApp()->getSingle(IqViewSystem_iid));
  ODA_ASSERT_ONCE(!s_pVS.isNull());

  return s_pVS;
}

//static 
QGLFormat OdqRenderArea::getQGLFormat()
{
#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
  QGLFormat fmt(QGL::SampleBuffers);
  fmt.setDoubleBuffer(false);
#else
  QGLFormat fmt(QGL::DoubleBuffer); // = QGLFormat::defaultFormat();
  #if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
  int majorVersion = 1,
      minorVersion = 1;
  fmt.setVersion(majorVersion, minorVersion);
  ODA_ASSERT_ONCE(majorVersion == 1 && minorVersion == 1);
  //ODA_ASSERT_ONCE(QGLFormat::openGLVersionFlags());
  #endif
#endif
  return fmt;
}

OdqRenderArea::OdqRenderArea(QWidget* pParent, 
                             bool bMaximized, // = false
                             TEnViewBusy step) // = enBusy_UninitializedGL
#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
  : QGLWidget(pParent)
#else
  : QGLWidget(OdqRenderArea::getQGLFormat(), pParent) //: QGLWidget(QGLFormat(QGL::SampleBuffers), pParent) //: QGLWidget(pParent)
#endif
  , m_pView(qobject_cast<OdqView*>(pParent))
  , m_bUseImageFrame(false) 
  , m_idTexture(0)
  , m_enBusy(step) // enBusy_UninitializedGL
  , m_bMaximized(bMaximized)
  , m_bResized(false)
  , m_bSkipFirstResizeGL(false)
  , m_isLastResizeGL(false)
  , m_widthLastResizeGL(0)
  , m_heightLastResizeGL(0)
  , m_levelResizeGL(0)
{
  setObjectName("RenderArea");

  setAttribute(Qt::WA_DeleteOnClose, true);
  totalScaleFactor = 1;
  setCursor(Qt::CrossCursor);
  setMouseTracking(true);

// Receive touch events for iOS and Android
#if defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
  setAttribute(Qt::WA_AcceptTouchEvents);
#ifdef ANDROID
  setAutoBufferSwap(true);
#endif //ANDROID
#endif //ANDROID || IOS
}

OdqRenderArea::~OdqRenderArea()
{
}

OdqView* OdqRenderArea::getView()
{
  return m_pView;
}

QImage& OdqRenderArea::getImageFrame()
{
  return m_imageFrame; // used for Caustic device only
}

bool OdqRenderArea::isImageFrameUsed() const
{
  return m_bUseImageFrame;
}

bool OdqRenderArea::isResized() const
{
  return m_bResized;
}

QSize OdqRenderArea::minimumSizeHint() const
{
  return sizeHint(50, 50);
}

QSize OdqRenderArea::sizeHint() const
{
  return sizeHint(400, 400);
}

QSize OdqRenderArea::sizeHint(int width, int heigth) const
{
  if (!m_pView.isNull() && !m_bMaximized) // && (!m_pView->isMaximized())
  {
    //ODA_ASSERT_VAR(bool bMaximized = m_pView->isMaximized();)
    // possible with drag resizing //ODA_ASSERT_ONCE(bMaximized == m_bMaximized);
    if (OdGsDCRect* pRect = m_pView->m_pRect.get())
      return QSize(Od_abs(pRect->m_max.x - pRect->m_min.x), Od_abs(pRect->m_max.y - pRect->m_min.y));
    else if (QSize* pSize = m_pView->m_pSize.get())
      return *pSize;
  }

  return QSize(width, heigth);
}

//#if defined(_DEBUG) // && (_MSC_VER > 1600)
#define DUMP_EN_BUSY
//#endif

#if defined(DUMP_EN_BUSY)

static bool isDumped()
{
  static int iDumpBusy = -1;
  if (iDumpBusy = -1)
    iDumpBusy = getIAppProps()->toInt(L"Vars/renddump", false) ? 1 : 0;
  return iDumpBusy != 0;
}

static void dumpBusy(const OdString& csMsg, OdqView* pView, bool bDefer)
{
  static OdString s_sPrevMsgs;
  static OdString s_sPrevMsg;

  OdString sMsg = s_sPrevMsgs + csMsg;
  s_sPrevMsgs.empty();
  if (pView)
  {
    OdString sTitle = getIApp()->toOdString(pView->windowTitle());
    int pos = sTitle.find(L':');
    if (pos > 0)
      sMsg += OdString().format(L" (%ls)", sTitle.mid(pos).c_str());
  }
  if (!bDefer && pView->isResizeActive())
    bDefer = true;
  if (bDefer)
  {
    s_sPrevMsgs = sMsg + L"\n\r";
    return;
  }
  if (s_sPrevMsg == sMsg) // skip duplicates
    return;
  s_sPrevMsg = sMsg;
  getIConsole()->putString(sMsg);
}

inline void dumpBusy(const OdString& csMsg, OdqView* pView)
{
  dumpBusy(csMsg, pView, true);
}

inline void dumpBusy(const QString& qsMsg, OdqView* pView)
{
  dumpBusy(getIApp()->toOdString(qsMsg), pView);
}
#else
# define dumpBusy(arg, pView) 
#endif

class _AutoUnlocker
{
  OdqView* m_pView;
  TEnViewBusy* m_pEnBusy;
  TEnViewBusy m_valEnd;

#if defined(DUMP_EN_BUSY)
  inline void dumpBusyState(const OdString& sPref, const TEnViewBusy& val, OdqView* pView)
  {
    if (!isDumped())
      return;
    OdString sMsg = sPref;
    bool bDefer = false;
    switch (val)
    {
    case enBusy_UninitializedGL: sMsg += L"enBusy_UninitializedGL"; bDefer = true; break;
    case enBusy_InitializeGL: sMsg += L"enBusy_InitializeGL"; bDefer = true; break;
    case enBusy_UnresizedGL: sMsg += L"enBusy_UnresizedGL"; bDefer = true; break;
    case enBusy_ResizeGL: sMsg += L"enBusy_ResizeGL"; bDefer = true; break;
    case enBusy_UnpaintGL: sMsg += L"enBusy_UnpaintGL"; bDefer = true; break;
    case enBusy_PaintGL: sMsg += L"enBusy_PaintGL"; bDefer = true; break;
    case enBusy_Free: sMsg += L"enBusy_Free"; break;
    }
    dumpBusy(sMsg, pView, bDefer);
  }
#else
# define dumpBusyState(pPref, val, pView) 
#endif

  //_AutoUnlocker(const _AutoUnlocker&); // disable
  //_AutoUnlocker& operator=(const _AutoUnlocker&);
public:
  _AutoUnlocker(OdqRenderArea* pRenderArea,
                TEnViewBusy& enBusy,
                const TEnViewBusy& valBegin,
                const TEnViewBusy& valEnd)
    : m_pView(pRenderArea ? pRenderArea->getView() : NULL)
    , m_pEnBusy(pRenderArea ? &pRenderArea->m_enBusy : NULL)
    , m_valEnd(valEnd)
  {
    if (m_pEnBusy)
      *m_pEnBusy = valBegin; // lock
    dumpBusyState(L"  AutoUnlocker-const: set busy = ", valBegin, m_pView);
  }
  ~_AutoUnlocker()
  {
    if (m_pEnBusy)
      *m_pEnBusy = m_valEnd; // unlock
    dumpBusyState(L"  AutoUnlocker-dest: set busy = ", m_valEnd, m_pView);
  }
};

void OdqRenderArea::initializeGL()
{
  OdqView* pView = getView();
  if (!pView || pView->m_pGsDevice.isNull())
  {
    ODA_FAIL_ONCE();
    return;
  }
  if (m_enBusy != enBusy_UninitializedGL)
  {
    if (isDumped())
      dumpBusy(QString("    --- skip initializeGL  by m_enBusy=%1").arg(m_enBusy), pView);
    return;
  }
  {
    _AutoUnlocker locker(this, m_enBusy, enBusy_InitializeGL, enBusy_UnresizedGL);

    m_idTexture = 0;

    //// Initiate rendering.

    OdRxDictionaryPtr pProperties = pView->m_pGsDevice->properties();
    m_bUseImageFrame = pProperties->has(OD_T("QImageFrame"));
    if (m_bUseImageFrame)
    {
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
      glEnable(GL_TEXTURE_2D);
    }

    TRC(QString("    *** after initializeGL"));
  } // after problem with random noise on the screen by switching layouts on Linux
  qApp->processEvents();
}

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
// INT-7585
// remove flicking with window resizing.
// (simple setAutoBufferSwap(false) instead swapBuffer in paintgl give us
//  problem via drag other app window over TV = no criteria to call GsDevice->invalidate())
//
#define NO_SWAP_BUFFERS_WITH_RESIZE 
#endif

void OdqRenderArea::paintGL()
{
  OdqView* pView = getView();
  if (!pView || pView->m_pGsDevice.isNull())
    return;

  TEnViewBusy prevBusy = m_enBusy;
  if (   m_enBusy != enBusy_UnpaintGL 
      && m_enBusy != enBusy_Free
      //after IPF via drag-resizing of app window on Win 7 32bit
      && (m_bUseImageFrame || m_enBusy != enBusy_PaintGL)) // enable draw during draw for Open GL
  {
    if (m_enBusy == enBusy_UnresizedGL) 
    {
      // after problem with random noise on the screen by switching layouts on Linux
      
      makeCurrent();
      QColor qclrBack = QApplication::palette().color(QPalette::Normal, QPalette::Dark); // = pView->m_clrBack; // = Qt::black
      qglClearColor(qclrBack);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (isDumped())
        dumpBusy(QString("    --- draw background and skip PaintGL by m_enBusy=%1").arg(m_enBusy), pView);
      return;
    }

    if (isDumped())
      dumpBusy(QString("    --- skip PaintGL by m_enBusy=%1").arg(m_enBusy), pView);
    return;
  }

  if (!m_bMaximized) //if (!pView->isMaximized())
  {
    if (OdGsDCRect* pRect = pView->m_pRect.get())
    {
      //QPoint pt(pRect->m_min.x, pRect->m_max.y); // topleft
      //pView->move(pt);
      //// see OdqRenderArea::sizeHint() //resize(Od_abs(pRect->m_max.x - pRect->m_min.x), Od_abs(pRect->m_max.y - pRect->m_min.y));
      OdGsDCPoint low = pRect->m_min, hi = pRect->m_max;
      QRect rect = QRect(QPoint(low.x, hi.y), // topleft
                         QPoint(hi.x, low.y)); // bottomright
      pView->setGeometry(rect);
      //
      pView->m_pRect = NULL;
    }
    else if (QSize* pSize = pView->m_pSize.get())
    {
      pView->resize(*pView->m_pSize);
      pView->m_pSize = NULL;
    }
    if (minimumSize() != QSize(50, 50))
      setMinimumSize(QSize(50, 50)); // restore default
  }

  if (m_enBusy == enBusy_PaintGL)
    return; // way to fix assert problem during gl update // CORE-12888

  OdRxDictionaryPtr pProperties = pView->m_pGsDevice->properties();

#ifdef Q_CC_MSVC
  {
    if (   pProperties->has(OD_T("WindowHDC")) // is property supported
      //&& pProperties->has(OD_T("WGLContext"))
//#if (TD_MAJOR_VERSION > 3 || TD_MINOR_VERSION > 2)
        && !((OdIntPtr) (OdRxVariantValue) pProperties->getAt(OD_T("WindowHDC")).get()))
    {
      // non-portable way for Windows only (isWinOpenGlModule)
      makeCurrent();

      // TODO for crash by close in debug (via internal Qt assertion - fatal) after update to Qt 4.7.1
      QGLFormat fmt(QGL::SingleBuffer | QGL::StencilBuffer); //HasOverlay
      //fmt.setStencilBufferSize(8);
      setFormat(fmt);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
      HDC hWindowDC = getDC();
#else
      HWND hwnd = (HWND)this->winId(); 
      HDC hWindowDC = GetDC(hwnd);
#endif
      // unfortunately there is no way to get HGLRC from m_pRenderArea
      pProperties->putAt(OD_T("WindowHDC"),  OdRxVariantValue((OdIntPtr) hWindowDC));
      if (pProperties->has(OD_T("WindowHWND")))
      {
        HWND hWnd = ::WindowFromDC(hWindowDC);
        pProperties->putAt(OD_T("WindowHWND"),  OdRxVariantValue((OdIntPtr) hWnd));
      }
      if (pProperties->has(OD_T("WGLContext")))
      {
        HGLRC hGLRC = wglGetCurrentContext();
        pProperties->putAt(OD_T("WGLContext"),  OdRxVariantValue((OdIntPtr) hGLRC));
      }
      if (pProperties->has(OD_T("DoubleBufferEnabled")))
        pProperties->putAt(OD_T("DoubleBufferEnabled"), OdRxVariantValue(false));
    }
//#else
//      && !((OdInt32) (OdRxVariantValue) pProperties->getAt(OD_T("WindowHDC")).get()))
//  {
//    ODA_ASSERT_ONCE(pProperties->has(OD_T("WGLContext")));
//    HDC hWindowDC = getDC();
//    // unfortunately there is no way to get HGLRC from m_pRenderArea
//    HGLRC hGLRC = wglGetCurrentContext();
//    pProperties->putAt(OD_T("WindowHDC"),  OdRxVariantValue((OdInt32) hWindowDC));
//    pProperties->putAt(OD_T("WGLContext"),  OdRxVariantValue((OdInt32) hGLRC));
//  }
//#endif
  }
#endif

  int widthSv = m_widthLastResizeGL,
      heightSv = m_heightLastResizeGL;

  makeCurrent();

  bool bUselessUpdate = pView->m_pGsDevice->isValid();
  // INT-7585 //m_bResized = true;
  if (bUselessUpdate)
  {
#  if !defined(NO_SWAP_BUFFERS_WITH_RESIZE)
    m_bResized = true;
#  else
    bool bResized = m_bResized;
    m_bResized = true;
    if (!bResized)
      setAutoBufferSwap(false);
    else
#  endif
      swapBuffers();
  }
  else
  {
#  if defined(NO_SWAP_BUFFERS_WITH_RESIZE)
    setAutoBufferSwap(true);
#  endif

    _AutoUnlocker locker(this, m_enBusy, enBusy_PaintGL, enBusy_Free);

    TRC(QString("    <<< before Paint (GsDevice->update())"));

    try
    {
      // moved into OdqView::updateGL & OdqRenderArea::resizeGL // pView->m_pGsDevice->invalidate();
      IqRevDiff* pRevDiff = getIRevSystem() ? getIRevSystem()->getIRevDiff(pView) : NULL;
      if (pRevDiff)
        pRevDiff->syncView(pView->m_pRxDatabase, pView->m_pGsDevice);
      pView->m_pGsDevice->update();

      if (pView->m_bDoZoomExtents)
      {
        pView->m_bDoZoomExtents = false;
        getIConsole()->postponeCommand("zoom e", false, pView->m_pRxDatabase);
      }
    }
    catch (OdError& e)
    {
      ODA_FAIL_ONCE();
      throw e;
    }
    catch (...)
    {
      ODA_FAIL_ONCE();
      throw eExtendedError;
    }

    TRC(QString("    >>> after Paint (GsDevice->update())"));
    // commented after IPF via drag-resizing of app window on Win 7 32bit
    //qApp->processEvents();

    if (m_bUseImageFrame && !getImageFrame().isNull())
    {
      makeCurrent();

      // paint : 

      QImage image = getImageFrame();

      // ----- make object with texture -----

     #if 0 // defined(_DEBUG)
      if (!image.isNull())
      {
        QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/grab");
        if (QDir(qsPath).exists())
        {
          qsPath = QDir::toNativeSeparators(qsPath + "/caustic_grab.png");
          image.save(qsPath);
        }
      }
      else  {
        QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/splash.png");
        if (QFileInfo(qsPath).exists())
        {
          QImage imageSplash;
          imageSplash.load(qsPath);
          image = QGLWidget::convertToGLFormat(imageSplash);
          
          getImageFrame() = QImage(); // set to null
          ODA_ASSERT_ONCE(getImageFrame().isNull());
        }
      }
     #endif

      if (!image.isNull())
      {
        if (m_idTexture)
        {
          glDeleteTextures(1, &m_idTexture);
          m_idTexture = 0;
        }
        m_idTexture = bindTexture(image, GL_TEXTURE_2D);
      }
      else if (!m_idTexture)
      {
        TRC(QString("    !!!! image is NULL"));
        return;
      }

      //QVector<QVector3D> vertices;
      //QVector<QVector2D> texCoords;
      //texCoords.append(QVector2D(1.0, 1.0));
      //texCoords.append(QVector2D(0.0, 1.0));
      //texCoords.append(QVector2D(0.0, 0.0));
      //texCoords.append(QVector2D(1.0, 0.0));
      //vertices.append(QVector3D( 0.5, -0.5, -0.2));
      //vertices.append(QVector3D(-0.5, -0.5, -0.2));
      //vertices.append(QVector3D(-0.5,  0.5, -0.2));
      //vertices.append(QVector3D( 0.5,  0.5, -0.2));
      // after missing classes QVector3D and QVector2D on mac:
      QVector<float> vertices;
      QVector<float> texCoords;
      texCoords.append(1.0f); texCoords.append(1.0f);
      texCoords.append(0.0f); texCoords.append(1.0f);
      texCoords.append(0.0f); texCoords.append(0.0f);
      texCoords.append(1.0f); texCoords.append(0.0f);
      vertices.append( 0.5f); vertices.append( -0.5f); vertices.append(-0.2f);
      vertices.append(-0.5f); vertices.append( -0.5f); vertices.append(-0.2f);
      vertices.append(-0.5f); vertices.append(  0.5f); vertices.append(-0.2f);
      vertices.append( 0.5f); vertices.append(  0.5f); vertices.append(-0.2f);

      // -----

      qglClearColor(Qt::black); // clearColor
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
      glLoadIdentity();
      glTranslatef(0.0f, 0.0f, -10.0f);

      glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
      glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
      glBindTexture(GL_TEXTURE_2D, m_idTexture);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // glDrawArrays(GL_TRIANGLE_FAN, iLoop * 4, 4);

    }
    else if (m_bUseImageFrame)
    {
      TRC(QString("    !!! Paint (GsDevice->update()) (image frame is NULL)"));
    }
  }
  if (widthSv != m_widthLastResizeGL || heightSv != m_heightLastResizeGL)
  {
    if (isDumped())
      dumpBusy(QString("    --- resizegl via paint resizeGL(%1,%2)").arg(m_widthLastResizeGL).arg(m_heightLastResizeGL), getView());
    resizeGL(m_widthLastResizeGL, m_heightLastResizeGL);
  }

  if (prevBusy == enBusy_UnpaintGL)
    OdqView::getViewSystem()->postOperationsSlot();
}

void OdqRenderArea::resizeEvent(QResizeEvent* pEvent)
{
  m_bResized = false;
#if 0 // (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  int w = width(),
      h = height();
  ODA_ASSERT_VAR(int eWidth = pEvent->size().width();)
  ODA_ASSERT_VAR(int eHeight = pEvent->size().height();)
  ODA_ASSERT_VAR(if (w != eWidth || h != eHeight))
    ODA_ASSERT(true); // brk

  QWidget::resizeEvent(pEvent); // is an empty
  if (!isValid())
    return;

  m_isLastResizeGL = true;
  m_levelResizeGL++;
  makeCurrent();
  if (m_enBusy == enBusy_UninitializedGL) // if (!d->glcx->initialized())
    glInit();

  int w_ = width(),
      h_ = height();
  makeCurrent();

  const qreal scaleFactor = (window() && window()->windowHandle()) ?
    window()->windowHandle()->devicePixelRatio() : 1.0;
  //resizeGL(w * scaleFactor, h * scaleFactor);
  if (!m_isLastResizeGL) // the latest is usable
  {
    ODA_ASSERT_ONCE(w_ == m_widthLastResizeGL && h_ == m_heightLastResizeGL);
    return;
  }
  bool bSkipped = m_bSkipFirstResizeGL;
  resizeGL(w_ * scaleFactor, h_ * scaleFactor);
  if (!bSkipped)
    m_isLastResizeGL = false;
  m_levelResizeGL--;
#else
  QGLWidget::resizeEvent(pEvent);
#endif
}

void OdqView::changeEvent(QEvent* pEvent)
{
  QMdiSubWindow::changeEvent(pEvent);
  if (pEvent->type() == QEvent::WindowStateChange || m_pRenderArea.isNull())
    return;
  QWindowStateChangeEvent* pChangeEvent = static_cast<QWindowStateChangeEvent*>(pEvent);
  Qt::WindowStates st = pChangeEvent->oldState();
  // problem after maximaze app if subwindow maximazed too
  if (st & Qt::WindowMinimized) //0x00000001
  {
    ODA_ASSERT_VAR(if (m_pRenderArea->m_bMaximized))
      m_pRenderArea->m_bMaximized = false;
  }
  else if (st & Qt::WindowMaximized) //0x00000002
  {
    ODA_ASSERT_VAR(if (!m_pRenderArea->m_bMaximized))
      m_pRenderArea->m_bMaximized = true;
  }
}

void OdqRenderArea::resizeGL(int width, int height)
{
  OdqView* pView = getView();
  if (!pView || pView->m_pGsDevice.isNull())
    return;

  if (m_bSkipFirstResizeGL)
  {
    m_bSkipFirstResizeGL = false;
    if (isDumped())
      dumpBusy(QString("    --- skip resizeGL(%1,%2) by m_bSkipFirstResizeGL=\"true\"").arg(width).arg(height), pView);
    return;
  }

  //if (m_bMaximized != pView->isMaximized())
  //  m_bMaximized = pView->isMaximized();
  //if (!m_bMaximized) //if (!pView->isMaximized())
  //{
  //  if (OdGsDCRect* pRect = m_pView->m_pRect.get())
  //  {
  //    QPoint pt(pRect->m_min.x, pRect->m_max.y); // topleft
  //    pView->move(pt);
  //    // see OdqRenderArea::sizeHint() //resize(Od_abs(pRect->m_max.x - pRect->m_min.x), Od_abs(pRect->m_max.y - pRect->m_min.y));
  //    //pView->m_pRect = NULL;
  //  }
  //}

  if (m_enBusy != enBusy_UnresizedGL && m_enBusy != enBusy_UnpaintGL && m_enBusy != enBusy_Free)
  {
    if (m_enBusy == enBusy_ResizeGL || m_enBusy == enBusy_PaintGL)
    {
      //TRC(QString("    --- deferred resizeGL(%1,%2) by m_enBusy=%3").arg(width).arg(height).arg(m_enBusy));
      if (isDumped())
        dumpBusy(QString("    --- deferred resizeGL(%1,%2) by m_enBusy=%3").arg(width).arg(height).arg(m_enBusy), getView());
      m_widthLastResizeGL = width;
      m_heightLastResizeGL = height;
    }
    else
    {
      if (isDumped())
        dumpBusy(QString("    --- skip resizeGL(%1,%2) by m_enBusy=%3").arg(width).arg(height).arg(m_enBusy), pView);
    }
    return; 
  }

  OdGsLayoutHelper* pWrapper = OdGsLayoutHelper::cast(pView->m_pGsDevice).get();
  if (pWrapper && pWrapper->underlyingDevice().isNull())
    return; // TigDb: fix crash by close diff view

  OdRxDictionaryPtr pProperties = pView->m_pGsDevice->properties();
  ODA_ASSERT_ONCE(m_bUseImageFrame == pProperties->has(OD_T("QImageFrame")));

  //int cnt = 0;
  bool bUpdateGL = (m_enBusy != enBusy_UnresizedGL);
  {
    _AutoUnlocker locker(this, m_enBusy, enBusy_ResizeGL, enBusy_UnpaintGL);
    if (isDumped())
      dumpBusy(QString("    --- resizeGL(%1,%2)").arg(width).arg(height), getView());

    m_widthLastResizeGL = width;
    m_heightLastResizeGL = height;
    do 
    {
      //cnt++;
      width = m_widthLastResizeGL;
      height = m_heightLastResizeGL;
      if (!width || !height)
        continue;

      if (m_bUseImageFrame)
      {
        makeCurrent();
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        //int side = qMin(width, height);
        //glViewport((width - side) / 2, (height - side) / 2, side, side);
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
       #if defined(TD_OPENGL_ES) || defined(QT_OPENGL_ES_1)
        glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
       #else
        glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
       #endif
        glMatrixMode(GL_MODELVIEW);
#endif
      }

      // possible via internal QT call (from QGLWidget::glDraw()) //ODA_ASSERT_ONCE(!m_bResized);
      // Setup device coordinate space
      //ODA_ASSERT_VAR(if (m_bMaximized != pView->isMaximized()))
      // via OdqView::changeEvent now // m_bMaximized = pView->isMaximized();
#    if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
      bUpdateGL = false;
#    endif

      OdGsDCRect rect(0, width, height, 0);
      TRC(QString("    <<< before m_pGsDevice->onSize(%1,%2) of resizeGL").arg(width).arg(height));
      pView->m_pGsDevice->invalidate();
      pView->m_pGsDevice->onSize(rect);
      TRC(QString("    >>> after m_pGsDevice->onSize(...) of resizeGL"));
    }
    while (qAbs(width - m_widthLastResizeGL) > 1 || qAbs(height - m_heightLastResizeGL) > 1);
  }
#if !defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
  if (bUpdateGL) //if (cnt > 1)
#endif
  {
    TRC(QString("    *** redraw widget of resizeGL"));
    // commented after IPF via drag-resizing of app window on Win 7 32bit
    //qApp->processEvents();

    pView->m_pGsDevice->invalidate();
    emit pView->update();
    emit updateGL(); // redraw // QTimer::singleShot(50, this, SLOT(updateGL()));
  }
}

void OdqRenderArea::mouseMoveEvent(QMouseEvent* pEvent)
{
  ODA_ASSERT_ONCE(pEvent->isAccepted());
  bool bAccepted = false;
  if (!m_pView.isNull())
    emit m_pView->mouseMove(m_pView, pEvent, bAccepted);
}

void OdqRenderArea::mousePressEvent(QMouseEvent* pEvent)
{
  ODA_ASSERT_ONCE(pEvent->isAccepted());
  bool bAccepted = false;
  if (!m_pView.isNull())
  {
	  //让绘图区不可编辑，只能用中键平移
	  if (pEvent->button() == Qt::MouseButton::MidButton)
	  {
		 emit m_pView->mousePress(m_pView, pEvent, bAccepted);
	  }
  }
}

void OdqRenderArea::mouseReleaseEvent(QMouseEvent* pEvent)
{
  ODA_ASSERT_ONCE(pEvent->isAccepted());
  bool bAccepted = false;
  if (!m_pView.isNull())
    emit m_pView->mouseRelease(m_pView, pEvent, bAccepted);
}

void OdqRenderArea::mouseDoubleClickEvent(QMouseEvent* pEvent)
{
  ODA_ASSERT_ONCE(pEvent->isAccepted());
  bool bAccepted = false;
  if (!m_pView.isNull())
    emit m_pView->mouseDoubleClick(m_pView, pEvent, bAccepted);
}

void OdqRenderArea::wheelEvent(QWheelEvent* pEvent)
{
  //ODA_ASSERT_ONCE(pEvent->isAccepted());
  bool bAccepted = false;
  if (!m_pView.isNull())
    emit m_pView->mouseWheel(m_pView, pEvent, bAccepted);
}

bool OdqRenderArea::event(QEvent* pEvent)
{
  bool bRes = false;
#if defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
  QEvent::Type tp = pEvent->type();
  switch (tp)
  {
  case(QEvent::TouchBegin):
  {
     bool bAccepted = false;
     if (!m_pView.isNull())
       emit m_pView->touchPressed(m_pView, pEvent, bAccepted);
       break;
  }
  case(QEvent::TouchUpdate):
  {
     bool bAccepted = false;
     if (!m_pView.isNull())
       emit m_pView->touchMove(m_pView, pEvent, bAccepted);
       break;
  }
  case(QEvent::TouchEnd):
  {
     bool bAccepted = false;
     if (!m_pView.isNull())
       emit m_pView->touchReleased(m_pView, pEvent, bAccepted);
       break;
  }
  case QEvent::FocusIn:
    tp = tp; break;
  case QEvent::FocusOut:
    tp = tp; break;
  case QEvent::Paint:
    tp = tp; break;
  case QEvent::Resize:
    tp = tp; break;
  case QEvent::Show:
    tp = tp; break;
  case QEvent::Hide:
    tp = tp; break;
  case QEvent::Close:
    tp = tp; break;
  case QEvent::Quit:
    tp = tp; break;
  }
#endif

  bRes = QGLWidget::event(pEvent);

#if defined(NO_SWAP_BUFFERS_WITH_RESIZE)
  if (pEvent->type() == QEvent::Paint)
    setAutoBufferSwap(true); // restore
#endif

  return bRes;
}

bool OdqView::event(QEvent* pEvent)
{
//#if defined(NO_SWAP_BUFFERS_WITH_RESIZE)
//  QEvent::Type tp = pEvent->type();
//  switch (tp)
//  {
//  case QEvent::Move:
//    if (m_pGsDevice->isValid())
//    {
//      //if (m_pGsDevice->isValid())
//      //  m_pGsDevice->invalidate();
//      // for all opened views
//      IqViewSystem* iVS = getIViewSystem();
//      ODA_ASSERT_ONCE(iVS);
//      foreach(OdRxObject* pRxDb, getIConsole()->getOpenDatabases())
//        iVS->updateViews(pRxDb);
//    }
//  }
//#endif

  return QMdiSubWindow::event(pEvent);
}

// also keyPressEvent and keyReleaseEvent are used via OdqView

//////////////////////////////////////////////////////////////////////////


//static
OdqView* OdqView::create(QMdiArea* pMdiArea,
                         OdRxObject *pRxDatabase, // = NULL
                         OdDbStub* idLayoutView, // = NULL // active if NULL
                         const OdGsDCRect* pRect, // = NULL
                         bool bMaximized, // = false
                         TEnViewBusy step) // = enBusy_UninitializedGL
{
  OdqView* pView = new OdqView(pMdiArea, pRxDatabase, idLayoutView, pRect, bMaximized, step);
  //pView->setWindowFlags(Qt::FramelessWindowHint);
  //pView->setBackgroundColor(RGB(0, 0, 0));
  //pView->resetDevice(true);
  pMdiArea->addSubWindow(pView);

  //pMdiArea->resize(QSize(pMdiArea->height(), pMdiArea->width()));//重新设置单文档窗口，避免文档挤压，显示不准确
  return pView;
}

void QAbstractScrollArea::wheelEvent(QWheelEvent* e)
{

}

OdqView::OdqView(QMdiArea* pMdiArea,
                 OdRxObject* pRxDatabase,
                 OdDbStub* idLayoutView,
                 const OdGsDCRect* pRect,
                 bool bMaximized,
                 TEnViewBusy step)
  : QMdiSubWindow(pMdiArea)
  , m_pRxDatabase(pRxDatabase)
  , m_idLayoutView(idLayoutView)
  , m_bDoZoomExtents(false)
  , m_indexTabLayoutPrev(-1)
  , m_pTrackerOnTimer(NULL)
  , m_intervalTimer(0)
  , m_isByClickIfCloseEvent(true)
{
 #if !defined(Q_WS_WIN) // _MSC_VER // problem with wake14.dwg (22Mb) (via posted event QEvent::DeferredDelete on Windows only) 
  setAttribute(Qt::WA_DeleteOnClose, true);
 #endif
  ODA_ASSERT_ONCE(pMdiArea);

  if (pRect)
    m_pRect = new OdGsDCRect(*pRect);
  else
	  m_pSize = new QSize(pMdiArea->frameSize().width(), pMdiArea->frameSize().height());
 
  m_pRenderArea = new OdqRenderArea(this, bMaximized, step); // enBusy_UninitializedGL
  QObject* pReceiver = NULL;
  const char* pcszMember = NULL;
  if (   !getIConsole()->getSlotData(QEvent::MouseMove, pRxDatabase, // in
                                     pReceiver, pcszMember) // out
      || !connect(this, SIGNAL(mouseMove(IqView*, QEvent*, bool&)),
                  pReceiver, pcszMember))
  {
    ODA_FAIL_ONCE();
  }
  if (   !getIConsole()->getSlotData(QEvent::MouseButtonPress, pRxDatabase, // in
                                     pReceiver, pcszMember) // out
      || !connect(this, SIGNAL(mousePress(IqView*, QEvent*, bool&)),
                  pReceiver, pcszMember))
  {
    ODA_FAIL_ONCE();
  }
  if (   !getIConsole()->getSlotData(QEvent::MouseButtonRelease, pRxDatabase, // in
                                     pReceiver, pcszMember) // out
      || !connect(this, SIGNAL(mouseRelease(IqView*, QEvent*, bool&)),
                  pReceiver, pcszMember))
  {
    ODA_FAIL_ONCE();
  }
  if (   !getIConsole()->getSlotData(QEvent::MouseButtonDblClick, pRxDatabase, // in
                                     pReceiver, pcszMember) // out
      || !connect(this, SIGNAL(mouseDoubleClick(IqView*, QEvent*, bool&)),
                  pReceiver, pcszMember))
  {
    ODA_FAIL_ONCE();
  }
  if (   !getIConsole()->getSlotData(QEvent::Wheel, pRxDatabase, // in
                                     pReceiver, pcszMember) // out
      || !connect(this, SIGNAL(mouseWheel(IqView*, QEvent*, bool&)),
                  pReceiver, pcszMember))
  {
    ODA_FAIL_ONCE();
  }
  if (   !getIConsole()->getSlotData(QEvent::KeyPress, pRxDatabase, // in
                                     pReceiver, pcszMember) // out
      || !connect(this, SIGNAL(keyPress(IqView*, QEvent*, bool&)),
                  pReceiver, pcszMember))
  {
    ODA_FAIL_ONCE();
  }
  if (   !getIConsole()->getSlotData(QEvent::KeyRelease, pRxDatabase, // in
                                     pReceiver, pcszMember) // out
      || !connect(this, SIGNAL(keyRelease(IqView*, QEvent*, bool&)),
                  pReceiver, pcszMember))
  {
    ODA_FAIL_ONCE();
  }

  this->setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏

	/*分割窗口*/
	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	splitter->setHandleWidth(5);
	splitter->addWidget(initLeftWgt(splitter));
	
	if (ClCate::MaxAlgle_I == _clType || ClCate::MaxAlgle_V == _clType)
	{
		splitter->addWidget(showAreaWgt(splitter));
		splitter->setStretchFactor(0, 5);
		splitter->setStretchFactor(1, 5);
	}
	else
	{
		splitter->addWidget(teighaCadWgt(splitter));
		splitter->setStretchFactor(0, 10);
		splitter->setStretchFactor(1, 6);
	}
	setWidget(splitter);

	m_pTabLayout = new QTabBar(this);
	updateLayouts(); // contains resetDevice()
	//int nCountLayout = m_pTabLayout->count();
	m_pTabLayout->setHidden(true);
	m_pTabLayout->setShape(QTabBar::TriangularSouth); //m_pTabLayout->setShape(QTabBar::RoundedSouth);
	if (!connect(m_pTabLayout, SIGNAL(currentChanged(int)),
		this, SLOT(changeLayout(int))))
	{
		ODA_FAIL_ONCE();
	}


  //m_pInternalWidget = new QGroupBox();
  //m_pInternalWidget->setAttribute(Qt::WA_DeleteOnClose, true);
  //updateLayouts(); // contains resetDevice()
  
  // for cascade
  //QPoint pos(-1, -1);
  //QList<QWidget *> widgetsList = pWorkspace->findChildren<QWidget *>();
  //foreach(QWidget *widget, widgetsList)
  //{
  //  if (widget->objectName() != qsWndFrameName)
  //    continue;
  //  QPoint posWidget(widget->pos());
  //  if (posWidget.x() > pos.x())
  //    pos.setX(posWidget.x());
  //  if (posWidget.y() > pos.y())
  //    pos.setY(posWidget.y());
  //} 
  //if (pos == QPoint(-1, -1))
  //  pos = QPoint(0, 0); // is first window
  //else
  //  pos += QPoint(15, 15); // offset
  //move(pos);
  //resize(600, 400);
  if (!pMdiArea->activeSubWindow())
  {
#  if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    QSettings settings("ODA",
      QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    bool bVal = settings.value(TD_SHORT_STRING_VER_S "/view/maximized", true).toBool();
    if (bVal)
#  else
    //if (false) // if (bVal)
#  endif
    {
     //#if defined(Q_CC_MSVC) || defined(__APPLE__)
#if !defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && (QT_VERSION < QT_VERSION_CHECK(5,0,0))
      if (!m_pRenderArea.isNull())
      {
        ODA_ASSERT_ONCE(m_pRenderArea->m_enBusy == step);
        m_pRenderArea->m_bSkipFirstResizeGL = true;
      }
#endif
     //#endif
    }

#  if 0
    if ((pRect || isMaximized()) && !m_pRenderArea.isNull())
    {
      ODA_ASSERT(true); // brk m_pRenderArea->m_bSkipFirstResizeGL = true;

      //adjustSize();
      ////QRect rect = m_pRenderArea->geometry();
      //OdGsDCPoint low = pRect->m_min, hi = pRect->m_max;
      ////MoveWindow( low.x, hi.y, hi.x - low.x, low.y - hi.y );
      //QRect rect = QRect(QPoint(low.x, hi.y), // topleft
       //                  QPoint(hi.x, low.y)); // bottomright
      //m_pRenderArea->setGeometry(rect);
      //adjustSize();
      //ODA_ASSERT_ONCE(m_pRenderArea->geometry() == rect);
      QSize sz(400, 400);
      if (pRect)
        sz = QSize(Od_abs(pRect->m_max.x - pRect->m_min.x), Od_abs(pRect->m_max.y - pRect->m_min.y));
      QSize szPrev = m_pRenderArea->minimumSize();
      m_pRenderArea->setMinimumSize(sz);
      adjustSize(); m_pRenderArea->adjustSize();
      m_pRenderArea->setMinimumSize(szPrev);
    }
#  endif

#  if (QT_VERSION < QT_VERSION_CHECK(5,0,0)) // moved into OdqViewSystem::createView fro QT5
    if (bVal)
      QTimer::singleShot(0, this, SLOT(showMaximized()));
#  endif
  }

}

OdqView::~OdqView()
{

  clearDevice();
}

TEnViewBusy OdqView::getBusyState() const
{
  ODA_ASSERT_ONCE(!m_pRenderArea.isNull());
  return m_pRenderArea.isNull() ? enBusy_Closed : m_pRenderArea->m_enBusy;
}

void OdqView::saveSettings()
{
  QMdiArea* pMdiArea = mdiArea();
  if (!pMdiArea || pMdiArea->subWindowList().count() != 1)
  {
    ODA_ASSERT_ONCE(pMdiArea);
    return;
  }
  // now by last sub-window only
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  bool bMaximized = isMaximized();
  settings.setValue(TD_SHORT_STRING_VER_S "/view/maximized", bMaximized);
}

void OdqView::clearDevice()
{
  m_pGsModel2d = NULL;
  m_pGsDevice = NULL;
  OdSelectionSetPtr pSSet;
  if (m_pRxDatabase)
  {
    OdEdCommandContext* pCtx = getIConsole()->getCommandContext(m_pRxDatabase);
    if (pCtx)
      pSSet = pCtx->arbitraryData(ODRX_SELECTION_SET);
  }
  if (!pSSet.isNull() && pSSet->numEntities())
  {
    pSSet->clear();
    getIApp()->refreshQObjetcts("<event-refresh:selection_changed>");
  }
}

void OdqView::resetDevice(bool isModel)
{
  OdGsModulePtr pGs = getViewSystem()->getGsModule(true,
                                                   isModel); // caustic is usable for models only
  if (pGs.isNull())
  {
    ODA_FAIL_ONCE();
    return;
  }
  if (!m_pRxDatabase)
    return;

  ODA_ASSERT_ONCE(!m_pRenderArea.isNull());

  // delete old device
  clearDevice(); // usually already in updateLayouts()

  // create device
  // 
  // unfortunately we can not change setupActiveLayoutViews without
  // base device recreating (AFTER problem with internal GS cache clearing)


  OdRxObject* pRxDb = m_pRxDatabase;
  OdDbBaseDatabasePE* pDbPE = OdDbBaseDatabasePE::cast(pRxDb).get();
  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pExtDb && !pDbPE && (pRxDb = pExtDb->getWrappedDatabase(m_pRxDatabase)) != m_pRxDatabase)
    pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  ODA_ASSERT_ONCE(pDbPE || pExtDb);
  if (!pExtDb)
    m_pGsDevice = pGs->createDevice();
  else
    m_pGsDevice = pExtDb->createDevice(pGs, m_pRxDatabase);

  if (m_pGsDevice.isNull())
  {
    ODA_FAIL_ONCE(); // TODO for pdf ...
    return;
  }

  // create wrapper to device

  OdGiDefaultContextPtr pGiCtx = pDbPE ? pDbPE->createGiContext(pRxDb) 
                                       : pExtDb->createGiContext(m_pRxDatabase);
  ODA_ASSERT_ONCE(pRxDb == pGiCtx->database() || !pDbPE);
  // TODO add enableGsModel to OdGiDefaultContext

  // set GS cache
  bool bGsCache = false;
  if (pExtDb)
  {
    OdString sMnemo = pExtDb->baseModuleName(true); // dgn, bim, dwf, ... 
    bGsCache = getIAppProps()->toBool(OdString().format(L"Vars/%lsgscache", sMnemo.c_str()), true); // "Vars/dgngscache" "Vars/bimgscache" "Vars/dwfgscache"
    pExtDb->enableGsModel(pGiCtx, bGsCache);
  }
  else
  {
    OdGiContextForDbDatabasePtr pDbCtx = OdGiContextForDbDatabase::cast(pGiCtx); 
    ODA_ASSERT_ONCE(!pDbCtx.isNull());
    if (!pDbCtx.isNull())
    {
      bGsCache = getIAppProps()->toBool(OD_T("Vars/dwggscache"), true);
      pDbCtx->enableGsModel(bGsCache);
    }
  }

//   ODCOLORREF clrBackground = getIAppProps()->toColorref(OD_T("Options/Display/ModelBackground"),
//                                                         ODRGB(0, 0, 0));
  //ODCOLORREF clrBackground = ODRGB(255, 255, 255);
  ODCOLORREF clrBackground = ODRGB(0, 0, 0);
  // create palette
  ODGSPALETTE palette;
  // get background and palette via DgDatabase models
  if (pExtDb)
  {
    pExtDb->getModelBackground(m_pRxDatabase, clrBackground, &palette);
    ODA_ASSERT_ONCE(palette.size() == 256);
  }
  if (!palette.size())
  {
    const ODCOLORREF* pColors = odcmAcadPalette(clrBackground); // odcmAcadDarkPalette() or odcmAcadLightPalette();
    //palette.insert(palette.begin(), pFrom, pFrom + 256);
    palette.resize(256);
    for (int index = 0; index < 256; index++)
      palette.setAt(index, pColors[index]);
    int indexBack = !pExtDb ? 0 : 255;
    palette[indexBack] = clrBackground;
  }
  m_pGsDevice->setLogicalPalette(palette.asArrayPtr(), 256);

  // Prepare the device to render the active layout in this database.
  ODA_ASSERT_ONCE(!m_idLayoutView || pExtDb); // // for DGN only
  IqRevDiff* pRevDiff = getIRevSystem() ? getIRevSystem()->getIRevDiff(this) : NULL;
  if (pRevDiff)
    m_pGsDevice = pRevDiff->setupLayoutDiff(m_pRxDatabase, m_pGsDevice, pGiCtx);
  else  if (!m_idLayoutView)
    m_pGsDevice = pExtDb ? pExtDb->setupActiveLayoutViews(m_pRxDatabase, m_pGsDevice, pGiCtx
#                                   if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
                                     , (void*) m_pRenderArea->winId(), m_pRenderArea->size().width(), m_pRenderArea->size().height()
#                                   endif
                                   )
                         : pDbPE->setupActiveLayoutViews(m_pGsDevice, pGiCtx);
  else
  {
    ODA_ASSERT_ONCE(pDbPE); // TODO
    m_pGsDevice = pDbPE->setupLayoutView(m_pGsDevice, pGiCtx, m_idLayoutView);
  }

  m_pGsModel2d = NULL;
  //OdGsLayoutHelperPtr wrapper = m_pGsDevice;

  OdRxDictionaryPtr pProperties = m_pGsDevice->properties();
  if (pProperties.get())
  {
    // see also OdqRenderArea::paintGL()
    if (pProperties->has(OD_T("QGLWidget"))) // Check if property is supported
    {
      ODA_ASSERT_ONCE(!m_pRenderArea.isNull());
      // portable way for all platforms (!isWinOpenGlModule)

      QObject* pGlWidget = qobject_cast<QObject*>(m_pRenderArea);
      ODA_ASSERT_ONCE(pGlWidget);
      pProperties->putAt(OD_T("QGLWidget"), OdRxVariantValue((OdIntPtr) pGlWidget));

      if (pProperties->has(OD_T("QImageFrame")))
        pProperties->putAt(OD_T("QImageFrame"),
                           OdRxVariantValue((OdIntPtr) &m_pRenderArea->getImageFrame()));
    }

    if (pProperties->has(OD_T("DiscardBackFaces")))
    {
      OdRxVariantValue pVar = (OdRxVariantValue) pProperties->getAt(OD_T("DiscardBackFaces"));
      // such conversion gives an invalid result on Mac OS X (true instead false here) // bool isValid = (bool) pRcVar;
      bool bDiscardBackFacesDefault = pVar->getBool();
      bool bDiscardBackFaces = getIAppProps()->toBool(OD_T("Vars/discardbackfaces"), 
                                                      bDiscardBackFacesDefault);
      if (bDiscardBackFaces != bDiscardBackFacesDefault)
        pProperties->putAt(OD_T("DiscardBackFaces"), OdRxVariantValue(bDiscardBackFaces));
    }

    //if(pProperties->has(OD_T("DoubleBufferEnabled"))) // Check if property is supported
    //  pProperties->putAt(OD_T("DoubleBufferEnabled"), OdRxVariantValue(true));
    //if(pProperties->has(OD_T("EnableSoftwareHLR"))) // Check if property is supported
    //  pProperties->putAt(OD_T("EnableSoftwareHLR"), OdRxVariantValue(theApp.useSoftwareHLR()));

    ODA_ASSERT_ONCE(m_pRenderArea->m_bUseImageFrame == pProperties->has(L"QImageFrame"));

    if (pProperties->has(L"UseVisualStyles"))
    {
      bool useVS = getIAppProps()->toBool(L"Vars/usevs", false);
      pProperties->putAt(L"UseVisualStyles", OdRxVariantValue(useVS));
    }

    if (pProperties->has(OD_T("UseOverlays")))
    {
      bool useOverlays = getIAppProps()->toBool(L"Vars/useoverlays", true);
      pProperties->putAt(L"UseOverlays", OdRxVariantValue(useOverlays)); // PROD-61
    }

#  if defined(ODA_WINDOWS)
    if (pProperties->has(L"RenderSettingsDlg")) // Check if property is supported
    {
      bool bEnabled = getIAppProps()->toBool(L"Options/Display/ExGLES2CompositeMf", false);
      if (bEnabled)
      {
        void* hVal = (void*)getIApp()->getMainWindow()->winId(); // AfxGetMainWnd()->GetSafeHwnd();
        pProperties->putAt(L"RenderSettingsDlg", OdRxVariantValue((OdIntPtr) hVal));
      }
    }
#  endif
  }

  // moved upper to set it on device for Bim problem with "Bifold-2 Panel_no trim.rfa" 
  //m_pGsDevice->setLogicalPalette(palette.asArrayPtr(), 256);

  if (isModelLayout()) // if (OdGsPaperLayoutHelper::cast(m_pGsDevice).get())
  {
    pGiCtx->setPaletteBackground(clrBackground);
   // setBackgroundColor(clrBackground); // for model space
  }
  else
  {
    ODCOLORREF clrPaper = getIAppProps()->toColorref(OD_T("Options/Display/PaperColor"),
                                                     ODRGB(0, 0, 0));
    pGiCtx->setPaletteBackground(clrPaper);
    ODCOLORREF clrPaperBackground = getIAppProps()->toColorref(OD_T("Options/Display/PaperBackground"),
                                                               ODRGB(173, 173, 173)); // ACAD's color for paper background
    //setBackgroundColor(clrPaperBackground);
  }
  setViewportBorderProperties();

  m_bDoZoomExtents = false;
  OdGsView::RenderMode rm = OdGsView::kNone; // undef
  if (pExtDb && !pExtDb->argsAtSetupLayoutViews(m_pRxDatabase, m_bDoZoomExtents, rm))
  {
    ODA_ASSERT_ONCE(!m_bDoZoomExtents && rm == OdGsView::kNone);
  }
  if (!pExtDb && bGsCache && pProperties.get() && pProperties->has(L"OpenRLVersion")) // or via OpenRLContextsCount
    rm = OdGsView::kGouraudShaded;

  if (rm != OdGsView::kNone)
  {
    OdGsView* pGsView = getActiveGsView();
    pGsView->setMode(rm);
  }

  //if (m_pRenderArea->isImageFrameUsed() != pProperties->has(OD_T("QImageFrame")))
  {
    //ODA_ASSERT_ONCE(   m_pRenderArea->m_enBusy == enBusy_UninitializedGL
    //                || m_pRenderArea->m_enBusy == enBusy_Free); // test
    m_pRenderArea->m_enBusy = enBusy_UninitializedGL;
    TRC(QString("    *** recreate context in RESET (request to InitializeGL)"));

  #if 0 //defined(ANDROID_ON) //|| defined(__IPHONE_OS_VERSION_MIN_REQUIRED)

    hide();
    qApp->processEvents(); // show (?menu changes)
    //getIApp()->getMainWindow()->showMinimized();
    //getIApp()->getMainWindow()->clearFocus();

   #if 0// defined(ANDROID)// || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#if !defined(TEST_DIRECT_INIT_GLES_1)
    m_pRenderArea->setContext(new QGLContext(OdqRenderArea::getQGLFormat(), m_pRenderArea));
#else
    // test
    if (!sGLESSO)
    {
      sGLESSO = dlopen("libGLESv1_CM.so", RTLD_NOW); 
      ODA_ASSERT_ONCE(sGLESSO);

      #define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)dlsym(sGLESSO, #funcName); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0) 

      int result = 1;
      #undef FN
      #define FNPTR(name) funcPtr_##name

      //!ANDROID_NDK
      IMPORT_FUNC(eglChooseConfig);
      IMPORT_FUNC(eglCreateContext);
      IMPORT_FUNC(eglCreateWindowSurface);
      IMPORT_FUNC(eglDestroyContext);
      IMPORT_FUNC(eglDestroySurface);
      IMPORT_FUNC(eglGetConfigAttrib);
      IMPORT_FUNC(eglGetConfigs);
      IMPORT_FUNC(eglGetDisplay);
      IMPORT_FUNC(eglGetError);
      IMPORT_FUNC(eglInitialize);
      IMPORT_FUNC(eglMakeCurrent);
      IMPORT_FUNC(eglSwapBuffers);
      IMPORT_FUNC(eglTerminate);
      //ANDROID_NDK

      IMPORT_FUNC(glBlendFunc);
      IMPORT_FUNC(glClear);
      IMPORT_FUNC(glClearColorx);
      IMPORT_FUNC(glColor4x);
      IMPORT_FUNC(glColorPointer);
      IMPORT_FUNC(glDisable);
      IMPORT_FUNC(glDisableClientState);
      IMPORT_FUNC(glDrawArrays);
      IMPORT_FUNC(glEnable);
      IMPORT_FUNC(glEnableClientState);
      IMPORT_FUNC(glFrustumx);
      IMPORT_FUNC(glGetError);
      IMPORT_FUNC(glLightxv);
      IMPORT_FUNC(glLoadIdentity);
      IMPORT_FUNC(glMaterialx);
      IMPORT_FUNC(glMaterialxv);
      IMPORT_FUNC(glMatrixMode);
      IMPORT_FUNC(glMultMatrixx);
      IMPORT_FUNC(glNormalPointer);
      IMPORT_FUNC(glPopMatrix);
      IMPORT_FUNC(glPushMatrix);
      IMPORT_FUNC(glRotatex);
      IMPORT_FUNC(glScalex);
      IMPORT_FUNC(glShadeModel);
      IMPORT_FUNC(glTranslatex);
      IMPORT_FUNC(glVertexPointer);
      IMPORT_FUNC(glViewport); 
    }
    EGLDisplay dpy = 0; //dpy = QEgl::display();
    EGLContext ctx = 0;
    eglDestroyContext(dpy, ctx);
    EGLConfig config = 0;
    EGLContext share_context = 0;
    ctx = eglCreateContext(dpy, config, share_context, NULL); 
#endif
   #else
    QGLContext* pContext = const_cast<QGLContext*>(m_pRenderArea->context());
    ODA_ASSERT_ONCE(pContext);
    if (pContext)
    {
      //pContext->setFormat(OdqRenderArea::getQGLFormat());
      pContext->create(); 
    }
   #endif

    //getIApp()->getMainWindow()->showMinimized();
    //getIApp()->getMainWindow()->clearFocus();
    getIApp()->getMainWindow()->hide();
    m_pRenderArea->showMaximized();
    //getIApp()->getMainWindow()->show();
  // recreate context
  //#if QT_VERSION >= 0x040700 
  // fix problem after layout switching on Mac OS X Snow Leopard with Qt v4.7.3
  #elif QT_VERSION >= 0x040700 //&& !defined(__APPLE__) // MacOS X
    // fix crash by close in debug (via internal Qt assertion - fatal) after update to Qt 4.7.1
    //
    // http://bugreports.qt.nokia.com/browse/QTBUG-13260?page=com.atlassian.jira.plugin.system.issuetabpanels:all-tabpanel
    // We can't reproduce anything like that here, and we need compileable 
    // example that fails before we can take a closer look. That said, 
    // calling setFormat() directly on a QGLWidget is not recommended, and
    // the function is marked as obsolete which means it's slated for removal.
    // Instead, just pass the QGLFormat object into the QGLWidget constructor 
    // (iLoop.e. make it a parameter to your window class object).
    //
    QGLContext* pContext = const_cast<QGLContext*>(m_pRenderArea->context());
    ODA_ASSERT_ONCE(pContext);
    if (pContext)
      pContext->create();
  #else
    QGLFormat fmt(QGL::DoubleBuffer); // = QGLFormat::defaultFormat();
   #ifdef __APPLE__ // MacOS X
    m_pRenderArea->hide();
    m_pRenderArea->setContext(new QGLContext(fmt, m_pRenderArea)); //m_pRenderArea->setFormat(fmt); 
    m_pRenderArea->show();
   #else
    m_pRenderArea->setContext(new QGLContext(fmt, m_pRenderArea)); //m_pRenderArea->setFormat(fmt); 
   #endif
  #endif
}

  //int cnt = 0;
  //if (m_pRenderArea->isResized())
  //{
  //  //ODA_ASSERT_ONCE(m_pRenderArea->m_enBusy == enBusy_Free);
  //  _AutoUnlocker locker(this, m_pRenderArea->m_enBusy, enBusy_ResizeGL, enBusy_UnpaintGL);
  //
  //  QRect rct = m_pRenderArea->rect();
  //  ODA_ASSERT_ONCE(!rct.left() && !rct.top());
  //  int width = rct.right(), 
  //      height = rct.bottom();
  //  m_pRenderArea->m_widthLastResizeGL = width;
  //  m_pRenderArea->m_heightLastResizeGL = height;
  //  do
  //  {
  //    cnt++;
  //    width = m_pRenderArea->m_widthLastResizeGL;
  //    height = m_pRenderArea->m_heightLastResizeGL;
  //    if (!width || !height)
  //      continue;
  //    // Setup device coordinate space
  //    OdGsDCRect rect(0, width, height, 0);
  //
  //    TRC(QString("    <<< before onSize(%1,%2) of RESET").arg(width).arg(height));
  //    m_pGsDevice->onSize(rect);
  //    TRC(QString("    >>> after onSize of RESET"));
  //    // Initiate rendering.
  //    //m_pGsDevice->update();
  //  }
  //  while (   qAbs(width - m_pRenderArea->m_widthLastResizeGL) > 1 
  //         || qAbs(height - m_pRenderArea->m_heightLastResizeGL) > 1);
  //}
  //if (cnt > 0)
  //  updateGL(); // redraw //QTimer::singleShot(50, m_pRenderArea, SLOT(updateGL()));
}

bool OdqView::isModelLayout()
{
  bool isModel = true;

  OdRxObject* pRxDb = m_pRxDatabase;
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb);
  if (pDbPE.isNull() && pExtDb)
    pDbPE = OdDbBaseDatabasePE::cast(pRxDb = pExtDb->getWrappedDatabase(pRxDb));
  if (pDbPE.get())
  {
    OdDbStub* idLayoutView = m_idLayoutView;
    OdRxObjectPtr pLayout;
    OdDbBaseLayoutPEPtr pLayoutPE;
    if (m_idLayoutView)
      pLayoutPE = OdDbBaseLayoutPE::cast(pLayout = pDbPE->openObject(idLayoutView));
    if (pLayoutPE.isNull())
    {
      ODA_ASSERT_ONCE(!m_idLayoutView || pExtDb->baseModuleName(true) == L"dgn");
      
      pLayoutPE = OdDbBaseLayoutPE::cast(pLayout = pDbPE->openObject(pDbPE->currentLayoutId(pRxDb)));
    }
    ODA_ASSERT_ONCE(   (pLayout.get() && pLayoutPE.get())
                    || (pExtDb && pExtDb->baseModuleName(true) == L"prc"));
    if (pLayout.get() && pLayoutPE.get())
      isModel = pLayoutPE->isModelLayout(pLayout);
    ODA_ASSERT_ONCE(   isModel == OdGsPaperLayoutHelper::cast(m_pGsDevice).isNull()
                    || pExtDb->baseModuleName(true) == L"dwf")
  }
  return isModel;
}

QWidget* OdqView::widget()
{
  return m_pRenderArea;
}

void OdqView::updateGL() // redraw
{
  if (m_pRenderArea.isNull())
    return;

  m_pGsDevice->invalidate();
  emit m_pRenderArea->updateGL();
}

void OdqView::setActive()
{
  QMdiArea* pMdiArea = mdiArea();
  if (pMdiArea)
    pMdiArea->setActiveSubWindow(this);
}

bool OdqView::isFullScreenActive()
{
  return false;
}
bool OdqView::isFullScreenMode()
{
  return false;
}

bool OdqView::isResizeActive()
{
  return m_pRenderArea->m_levelResizeGL > 0;
}

ODCOLORREF OdqView::backgroundColor() const
{
  return ODRGB(m_clrBack.red(), m_clrBack.green(), m_clrBack.blue());
}

bool OdqView::setBackgroundColor(ODCOLORREF clrBackground)
{
  ////QPalette plt = m_pTabLayout->palette();
  ////m_pTabLayout->setPalette(plt);  ////m_pInternalWidget->setPalette(plt);

  m_clrBack = QColor(ODGETRED(clrBackground), 
                     ODGETGREEN(clrBackground),
                     ODGETBLUE(clrBackground)); // = Qt::yellow;
  m_clrBackSel = QColor(255 - m_clrBack.red(),
                        255 - m_clrBack.green(),
                        255 - m_clrBack.blue()); // = Qt::blue;
  m_clrFore = QColor((m_clrBackSel.red() + m_clrBack.red()) / 2,
                     (m_clrBackSel.green() + m_clrBack.green()) / 2,
                     (m_clrBackSel.blue() + m_clrBack.blue()) / 2); // = Qt::gray;

  //QString qsBack = "yellow",
  //        qsBackSel = "red",
  //        qsFore = "blue";
  QString qsBack = QString("rgb(%1,%2,%3)").arg(m_clrBack.red())
                                           .arg(m_clrBack.green())
                                           .arg(m_clrBack.blue()),
          qsBackSel = QString("rgb(%1,%2,%3)").arg(m_clrBackSel.red())
                                              .arg(m_clrBackSel.green())
                                              .arg(m_clrBackSel.blue()),
          qsFore = QString("rgb(%1,%2,%3)").arg(m_clrFore.red())
                                           .arg(m_clrFore.green())
                                           .arg(m_clrFore.blue());

  QString qsStyleSheet = QString("* { "
                                 "background-color: %1;"
                                 "color: %2;"
                                 " }"
                                 "*:pressed { "
                                 "background-color: %2;"
                                 "color: %1;"
                                 " }").arg(qsBack).arg(qsFore);
  //if (!m_pInternalWidget.isNull())
	//  m_pInternalWidget->setStyleSheet(qsStyleSheet);

  // TODO ???
  // http://doc.trolltech.com/4.3/stylesheet-examples.html
  // http://www.wiki.crossplatform.ru/index.php/Qt:%D0%94%D0%BE%D0%BA%D1%83%D0%BC%D0%B5%D0%BD%D1%82%D0%B0%D1%86%D0%B8%D1%8F_4.3.2/stylesheet-examples#customizing-qtabwidget-and-qtabbar
  //"* { "
  ////"alternate-background-color: white;"
  ////"padding: 7px;"
  //"color: blue;"
  //"selection-background-color: blue;" 
  //"selection-color: yellow;"
  //"lighting-background-color: black;" // ": rgb(150,147,88);"
  //"lighting-color: white;" // rgb(255,255,255);"
  //"border-bottom-color: white;"
  //"outline-color: gray;"
  //border-color
  // " }"
  //
  //"QTabBar::tab { height: 100px; width: 100px; }"); 
  //   padding: 7px;
  //   width: 100px;
  //   right-margin: 50;
  //   border: 2px solid red;
  //   border-top: 2px solid #FFFFFF; 
  //    border-bottom: 0px; border-top: 0px;
  //   margin: 0px 0px 2px 0px;
  //   min-width: 8ex;

 #ifndef Q_OS_LINUX // simplest solution to fix invisible text in bar on Linux (Qt bug)
	qsStyleSheet = QString("QTabBar::tab { "
		"  text-align: left; "
		// expand/overlap to the left and right by 3px
		"  margin-left: -3px; margin-right: -3px; "
		//"  background-color: %2;"
		//"  color: %1;"
		"} "
		"QTabBar::tab:first { "
		"  margin-left: 0; margin-right: -3px; "
		"} "
		"QTabBar::tab:!selected { "
		"  color: %1; "
		// non-selected tabs look smaller
		//"  margin-top: 2px; "
		"} "
		"QTabBar::tab:selected { "
		//"  color: %1; "
		////"  border-color: %3; "
		////"  border-bottom-color: %3; "
		"} "
		//"QTabBar QToolButton { "
		//"  border-width: 2px; "
		//"} "
		).arg(qsFore).arg(qsBack).arg(qsBackSel);
	if (!m_pTabLayout.isNull())
		m_pTabLayout->setStyleSheet(qsStyleSheet);
#endif

  if (m_pGsDevice.isNull())
    return false;
  return m_pGsDevice->setBackgroundColor(clrBackground);
}

void OdqView::setViewportBorderProperties()
{
  ODA_ASSERT_ONCE(!m_pGsDevice.isNull()); // test
  if (m_pGsDevice.isNull())
    return;

  OdGsLayoutHelperPtr pLayoutHelper = OdGsLayoutHelper::cast(m_pGsDevice);
  if (!pLayoutHelper.get())
    return; // DGN way
  OdGsViewPtr pActiveView = pLayoutHelper->activeView();

  OdGsViewPtr pOverallView;
  OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(m_pGsDevice);
  if (pPSHelper.get())
    pOverallView = pPSHelper->overallView();

  int count = m_pGsDevice->numViews();
  if (count <= 1)
    return;
  const ODCOLORREF* pColors = odcmAcadPalette(m_pGsDevice->getBackgroundColor()); // odcmAcadDarkPalette() or odcmAcadLightPalette();

  for (int index = 0; index < count; index++)
  {
    OdGsViewPtr pGsView = m_pGsDevice->viewAt(index);
    if (   (pGsView == pOverallView) 
        || (OdGsPaperLayoutHelper::cast(m_pGsDevice).get() && (pGsView != pActiveView)))
    {
      // no border
      pGsView->setViewportBorderVisibility(false);
    }
    else if (pGsView != pActiveView)
    {
      pGsView->setViewportBorderVisibility(true);
      pGsView->setViewportBorderProperties(pColors[7], 1);
    }
    else
    {
      pGsView->setViewportBorderVisibility(true);
      pGsView->setViewportBorderProperties(pColors[7], 3);
    }
  }
}

void OdqView::subscribeTimer(OdEdIntegerTracker* pTracker, 
                             int msec) // = 100
{
  if (!pTracker)
  {
    m_pTrackerOnTimer = NULL;
    m_intervalTimer = 0;
    return;
  }

  m_pTrackerOnTimer = pTracker;
  ODA_ASSERT_ONCE(msec > 0);
  m_intervalTimer = msec;

  QTimer::singleShot(msec, this, SLOT(timerUpdate()));
}

void OdqView::unsubscribeTimer(OdEdIntegerTracker*) //pTracker
{
  subscribeTimer(NULL, 0); 
}

// slot
void OdqView::timerUpdate()
{
  if (!m_pTrackerOnTimer || m_intervalTimer <= 0)
    return;
  if (m_pGsDevice.isNull())
  {
    m_intervalTimer = 0;
    return;
  }

  m_pTrackerOnTimer->setValue(m_intervalTimer);
  updateGL(); //getIViewSystem()->updateViews(); // redraw
  if (m_pTrackerOnTimer && m_intervalTimer > 0) // maybe changed by setValue call
    QTimer::singleShot(m_intervalTimer, this, SLOT(timerUpdate()));
}

bool OdqView::emitClose()
{
  m_isByClickIfCloseEvent = false;
  if (m_pRenderArea.data())
  {
    if (m_pRenderArea->m_enBusy == enBusy_Closed)
      return true;
    m_pRenderArea->m_enBusy = enBusy_Closed;
  }
  bool bRes = close();
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  qApp->processEvents(); // INT-7585 Crash by exit with non-closed drawings.
#endif
  return bRes;
}

void OdqView::closeEvent(QCloseEvent* pEvent)
{
  QPointer<OdqView> pThis = this;
  bool isSingle = false;
  if (m_isByClickIfCloseEvent && getViewSystem()->getView(m_pRxDatabase, false, &isSingle) && isSingle)
  {
    getIConsole()->postponeCommand("^C^C close"); // see also OdqMainWindow::closeEvent
    pEvent->ignore();
    return;
  }

  if (!getViewSystem()->removeView(pThis)) // no-warning (this))
    pEvent->ignore();
  //else
  //  ODA_ASSERT_ONCE(pThis.isNull() || m_pGsDevice.isNull()); // should be already cleared via clearDevice
}

static int getTabBarIndexByText(const QTabBar* pTabBar, const QString& qsText)
{
  ODA_ASSERT_ONCE(pTabBar && !qsText.isEmpty());

  for (int index = 0; index < pTabBar->count(); index++) 
  {
    if (qsText == pTabBar->tabText(index))
      return index;
  }
  return -1; // was not found
}

void OdqView::updateLayouts()
{
	OdRxObject* pRxDb = m_pRxDatabase;
	OdDbBaseDatabasePE* pDbPE = OdDbBaseDatabasePE::cast(pRxDb).get();
	OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb);
	if (pExtDb && !pDbPE && (pRxDb = pExtDb->getWrappedDatabase(m_pRxDatabase)) != m_pRxDatabase)
		pDbPE = OdDbBaseDatabasePE::cast(pRxDb);

	QString qsCurrLayoutName,
		qsName;
	QStringList lstLayouts;
	bool isModel = true;

	if (pDbPE)
	{
		int indexLastModel = 0;

		OdDbStub* id = pDbPE->currentLayoutId(pRxDb);
		OdRxObjectPtr pCurrLayout = pDbPE->openObject(id);
		if (pCurrLayout.get())
		{
			OdDbBaseLayoutPEPtr pLayoutInfo(pCurrLayout);
			if (!pLayoutInfo.isNull())
			{
				qsCurrLayoutName = getIApp()->toQString(pLayoutInfo->name(pCurrLayout));
				isModel = pLayoutInfo->isModelLayout(pCurrLayout);
			}
		}

		OdRxIteratorPtr pLayIter = pDbPE->layouts(pRxDb);

		for (; pLayIter.get() && !pLayIter->done(); pLayIter->next())
		{
			OdRxObjectPtr pLayout = pLayIter->object();
			OdDbBaseLayoutPEPtr pLayoutInfo(pLayout);

			if (pLayoutInfo.isNull())
			{
				ODA_FAIL_ONCE();
				continue;
			}

			if ((qsName = getIApp()->toQString(pLayoutInfo->name(pLayout))).isEmpty())
			{
				ODA_FAIL_ONCE();
				continue;
			}
			if (lstLayouts.contains(qsName))
				continue; // Bim Views Iterator contains duplicates
#ifdef ANDROID
			if (qsCurrLayoutName == qsName) {
				lstLayouts << qsName;
			}
#else
			lstLayouts << qsName;
#endif
			if (getTabBarIndexByText(m_pTabLayout, qsName) >= 0)
				continue;

#ifdef ANDROID
			if (qsCurrLayoutName == qsName)
#endif
				if (pLayoutInfo->isModelLayout(pLayout))
					m_pTabLayout->insertTab(indexLastModel++, qsName);
				else
					m_pTabLayout->addTab(qsName);

		} // end for

		if (!qsCurrLayoutName.isEmpty()
			&& getTabBarIndexByText(m_pTabLayout, qsCurrLayoutName) < 0)
		{
			// for Tvf bug only
			if (!lstLayouts.contains(qsCurrLayoutName))
				lstLayouts.insert(0, qsCurrLayoutName);
			m_pTabLayout->insertTab(0, qsCurrLayoutName);
			indexLastModel++;
		}
	}

	int szLayouts = (int)lstLayouts.size();
	if (!szLayouts && pExtDb)
	{
		OdStringArray namesLayoutView;
		OdDbStubPtrArray idsLayoutView;
		OdString sActiveLayoutViewName;
		if (pExtDb->getLayoutViewNamesAndIds(m_pRxDatabase, namesLayoutView, idsLayoutView, &sActiveLayoutViewName))
		{
			qsCurrLayoutName = getIApp()->toQString(sActiveLayoutViewName);
			szLayouts = namesLayoutView.size();
			for (int idx = 0; idx < szLayouts; idx++)
			{
				lstLayouts << (qsName = getIApp()->toQString(namesLayoutView[idx]));
				m_pTabLayout->addTab(qsName);
			}
		}
		else if (pExtDb->baseModuleName(true) == L"prc")
		{
			lstLayouts << (qsCurrLayoutName = "Model");
			if (m_pTabLayout->count() < ++szLayouts)
				m_pTabLayout->addTab(qsCurrLayoutName);
		}
	}

	if (szLayouts < m_pTabLayout->count())
	{
		// find and remove tabs of deleted layouts
		for (int index = 0; index < m_pTabLayout->count(); index++)
		{
			QString qsText = m_pTabLayout->tabText(index);
			if (lstLayouts.indexOf(qsText) >= 0)
				continue;

			m_pTabLayout->removeTab(index--);
		}
	}

	int indexCurrLayout = getTabBarIndexByText(m_pTabLayout, qsCurrLayoutName);
	if (indexCurrLayout < 0)
	{
		ODA_FAIL_ONCE();
		return;
	}

	clearDevice(); // prevent activity of changeLayout slot

	m_pTabLayout->setCurrentIndex(indexCurrLayout);
	if (indexCurrLayout != m_indexTabLayoutPrev && m_indexTabLayoutPrev >= 0)
		m_pTabLayout->setTabTextColor(m_indexTabLayoutPrev, m_clrFore); //Qt::blue // normal
	m_indexTabLayoutPrev = indexCurrLayout;

	if (pExtDb)
	{
		OdDbStub* idActiveLayoutView = NULL;
		OdDbStubPtrArray idsLayoutViews;
		OdGsDCRectArray rects;
		// for active model(group) only
		if (pExtDb->getVisibleLayoutViewIds(m_pRxDatabase, idActiveLayoutView, idsLayoutViews, rects))
		{
			ODA_ASSERT_ONCE(m_idLayoutView && idActiveLayoutView);
			if (!idsLayoutViews.contains(m_idLayoutView))
			{
				ODA_ASSERT_ONCE(m_idLayoutView != idActiveLayoutView);
				m_idLayoutView = idActiveLayoutView;
			}
		}
		else
		{
			ODA_ASSERT_ONCE(pExtDb->baseModuleName(true) != L"dgn");
		}
	}
	m_pTabLayout->setTabTextColor(indexCurrLayout, m_clrBackSel); //Qt::gray // selected //!!! // const QColor &color);
	resetDevice(isModel); // can change m_clrBackSel as side effect
	m_pTabLayout->setTabTextColor(indexCurrLayout, m_clrBackSel);
}

// slot
void OdqView::changeLayout(int index)
{
	if (!m_pTabLayout || m_pGsDevice.isNull())
		return;
	ODA_ASSERT_ONCE(index >= 0 && index == m_pTabLayout->currentIndex());

	if (index == m_indexTabLayoutPrev)
		return;

	if (m_indexTabLayoutPrev >= 0)
		m_pTabLayout->setTabTextColor(m_indexTabLayoutPrev, m_clrFore); //Qt::blue // normal

	m_indexTabLayoutPrev = index;
	getIConsole()->markToCancel(m_pRxDatabase); // #9152 ?Problem with switching layout during rectangular selection
	getIConsole()->postponeCommand(QString("qvlayout \"%1\"").arg(m_pTabLayout->tabText(index)),
		false, m_pRxDatabase);
}

OdRxObject* OdqView::getRxDatabase()
{
  return m_pRxDatabase;
}

OdGsDevice* OdqView::getGsDevice()
{
  return m_pGsDevice; // the device to render the active layout (is a OdGsLayoutHelper) 
}

OdGsView* OdqView::getActiveGsView()
{
  OdGsView* pGsView = NULL;
  OdGsLayoutHelperPtr pWrapper = OdGsLayoutHelper::cast(getGsDevice());
  if (!pWrapper.isNull())
    pGsView = pWrapper->activeView();
  else
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(m_pRxDatabase);
    OdGsDevice* pGsDevice = getGsDevice();
    if (pExtDb && pGsDevice)
      pGsView = pExtDb->getActiveGsView(pGsDevice);
  }
  //ODA_ASSERT_ONCE(pGsView);

  return pGsView;
}

OdGsView* OdqView::getActiveTopGsView()
{
  OdGsView* pGsView = getActiveGsView();

  OdRxObject* pRxDb = m_pRxDatabase;
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pRxDb);
  OdExtDbModule* pExtDb;
  if (pDwgDb.isNull() && (pExtDb = getIApp()->getExtDbModule(pRxDb)) != 0)
    pDwgDb = OdDbDatabase::cast(pRxDb = pExtDb->getWrappedDatabase(pRxDb));
  if (!pDwgDb.isNull())
  {
    if (!pDwgDb->getTILEMODE())
    {
      OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject();
      OdDbAbstractViewportDataPtr pAVD(pVpObj);
      if (!pAVD.isNull() && pAVD->gsView(pVpObj))
      {
        pGsView = pAVD->gsView(pVpObj);
      
        // after OdExDollyCmd::execute
        OdDbLayoutPtr pLayout = pDwgDb->currentLayoutId().openObject();
        if (   pLayout->drawViewportsFirst()
            && pGsView->device()->viewAt(pGsView->device()->numViews() - 1) == pGsView)
          pGsView = pGsView->device()->viewAt(0);
      }
    }
  }

  return pGsView;
}

OdGsModel* OdqView::getGsModel(bool b2d) // = false
{
  // test only 
  if (b2d)
  {
    if (m_pGsModel2d.isNull())
    {
      m_pGsModel2d = m_pGsDevice->createModel();
      if (!m_pGsModel2d.isNull())
        m_pGsModel2d->setRenderType(OdGsModel::kDirect); // Skip Z-buffer for 2d drawables
    }
    return m_pGsModel2d;
  }

  OdGsModel* pGsModel = NULL;
  OdGsLayoutHelperPtr pWrapper = OdGsLayoutHelper::cast(getGsDevice());
  if (!pWrapper.isNull())
    pGsModel = pWrapper->gsModel();
  else
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModule(m_pRxDatabase);
    if (pExtDb)
      pGsModel = pExtDb->getGsModel(getGsDevice());
  }
  // it is NULL if GS cache off //ODA_ASSERT_ONCE(pGsModel);

  return pGsModel;
}

QCursor OdqView::cursor()
{
  ODA_ASSERT_ONCE(!m_pRenderArea.isNull());
  return m_pRenderArea->cursor();
}

void OdqView::setCursor(const QCursor& cursor)
{
  ODA_ASSERT_ONCE(!m_pRenderArea.isNull());
  m_pRenderArea->setCursor(cursor);
}

bool OdqView::getUcsPlane(class OdGePlane& plane, 
                          const OdGeVector3d* pvHintDirToIntersect) // = NULL
{
  OdGsView* pGsView = getActiveTopGsView();
  //ODA_ASSERT_ONCE(pGsView);
  if (!pGsView)
    return false;

  OdGsClientViewInfo vi;
  pGsView->clientViewInfo(vi);
  OdDbObjectId idViewport;
  if (OdDbDatabase::cast(baseDatabaseBy(vi.viewportObjectId)).get())
    idViewport = vi.viewportObjectId;
  
  if (!idViewport.isNull())
  {
    OdDbObjectPtr pVpObj = idViewport.safeOpenObject();
    OdDbAbstractViewportDataPtr pAVD(pVpObj);
    OdGePoint3d ucsOrigin;
    OdGeVector3d ucsXAxis, ucsYAxis;
    pAVD->getUcs(pVpObj, ucsOrigin, ucsXAxis, ucsYAxis);
    double dElevation = pAVD->elevation(pVpObj);
    if (!OdZero(dElevation))
    {
      OdGeVector3d vElevation = ucsXAxis.crossProduct(ucsYAxis) * dElevation;
      ucsOrigin += vElevation;
    }
    plane.set(ucsOrigin, ucsXAxis, ucsYAxis);
    return true;
  }
  ODA_ASSERT_ONCE(OdDbDatabase::cast(m_pRxDatabase).isNull());

  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(m_pRxDatabase);
  if (   !pExtDb
      || !pExtDb->getUcsPlane(m_pRxDatabase, pGsView, 
                              plane, // out
                              pvHintDirToIntersect))
    return false;

  return true; 
}

// convert eyes point to UCS(by empty OdEd::kGptNoUCS flag in option) or to WCS in eye plane
bool OdqView::eyesTo(const QPoint& ptEyes, // in 
                     int options, // in 
                     OdGePoint3d& ptOut, // out
                     const OdGePlane* pUcsPlane) // = NULL // in
{
  OdGsView* pGsView = getActiveTopGsView();
  if (!pGsView)
    return false;

  // eyes to WCS in eye plane

  ptOut.set(ptEyes.x(), ptEyes.y(), 0.0);
  if (pGsView->isPerspective())
    ptOut.z = pGsView->projectionMatrix()(2,3);
  ptOut.transformBy((pGsView->screenMatrix() * pGsView ->projectionMatrix()).inverse());
  ptOut.z = 0.;
  // eye CS at this point.
  ptOut.transformBy(OdAbstractViewPEPtr(pGsView)->eyeToWorld(pGsView)); 

  if (GETBIT(options, OdEd::kGptNoUCS)) // Do not project result point onto UCS plane (input point in eye plane)
    return true;

  // project result point onto UCS plane

  OdGeVector3d vDirection = OdAbstractViewPEPtr(pGsView)->direction(pGsView);
  OdGePlane plane;
  if (!pUcsPlane)
  {
    if (!getUcsPlane(plane, &vDirection))
    {
      if (OdDbDatabase::cast(m_pRxDatabase).get())
        return false; 
      // DGN way only
      return true; //return false;
    }
    pUcsPlane = &plane;
  }

  if (!pGsView->isPerspective())
  {
    // For orhogonal projection we simply check intersection between viewing direction and UCS plane.
    OdGeLine3d line(ptOut, vDirection);
    return pUcsPlane->intersectWith(line, ptOut);
  }

  // For perspective projection we emit ray from viewer position through WCS point.
  const double focalLength = -1.0 / pGsView->projectionMatrix()(3, 2);
  const OdGePoint3d pos = pGsView->target() + (vDirection.normal() * focalLength);
  OdGeRay3d ray(pos, ptOut);
  return pUcsPlane->intersectWith(ray, ptOut);
}

bool OdqView::wcsToScreen(const OdGePoint3d& wcsPt,
                          OdGePoint2d& ptScr) // out
{
  ptScr = OdGePoint2d();

  OdGsView* pGsView = getActiveTopGsView();
  ODA_ASSERT_ONCE(pGsView);

  // To DCS
  OdGsClientViewInfo viewInfo;
  pGsView->clientViewInfo(viewInfo);

  //OdRxObjectPtr pObj = OdDbObjectId(viewInfo.viewportObjectId).openObject();
  //OdAbstractViewPEPtr pVp(pObj);
  OdAbstractViewPEPtr pVp = OdAbstractViewPE::cast(pGsView);
  ODA_ASSERT_ONCE(!pVp.isNull());
  if (pVp.isNull())
    return false;

  OdGeVector3d vecY = pVp->upVector(pGsView);
  OdGeVector3d vecZ = pVp->direction(pGsView);
  OdGeVector3d vecX = vecY.crossProduct(vecZ).normal();
  OdGeVector2d offset = pVp->viewOffset(pGsView);
  OdGePoint3d prTarg = pVp->target(pGsView) - vecX * offset.x - vecY * offset.y;
  ptScr.x = vecX.dotProduct(wcsPt - prTarg);
  ptScr.y = vecY.dotProduct(wcsPt - prTarg);
  return true;
}

void OdqView::updateObject(OdDbStub*) // id
{
  ODA_FAIL_ONCE(); // TODO
}

void OdqView::highlightObject(bool bValue,
                              OdDbStub* id,
                              const OdDbBaseFullSubentPath* pPath)
{
  ODA_ASSERT_ONCE(id && m_pRxDatabase == baseDatabaseBy(id));
  if (!id || m_pRxDatabase != baseDatabaseBy(id) || !m_pRxDatabase)
    return;

  if (OdDbDatabase::cast(m_pRxDatabase).get())
  {
    OdDbObjectId idObj(id);
    if (idObj.isErased())
      return;
    OdDbEntityPtr pEnt = OdDbEntity::cast(idObj.openObject());
    if (!pEnt.get())
    {
      ODA_FAIL_ONCE();
      return;
    }

    const OdDbFullSubentPath* pSubId = (const OdDbFullSubentPath *)pPath;
    pEnt->highlight(bValue, pSubId);
  }
  else if (OdExtDbModule* pExtDb = getIApp()->getExtDbModule(m_pRxDatabase))
    pExtDb->highlight(getActiveTopGsView(), bValue, id, pPath);
}

void OdqView::onModifiedGrip(OdGiDrawablePtr pGripData)
{
  ODA_ASSERT_ONCE(pGripData.get());
  if (pGripData.isNull())
    return;

  OdGsModel* pGsModel2d = getGsModel(true);
  if (pGsModel2d)
  {
    pGsModel2d->onModified(pGripData, (OdGiDrawable*)0);
    return;
  }

  OdGsLayoutHelper* pGsLayoutHelper = OdGsLayoutHelper::cast(m_pGsDevice);
  if (pGsLayoutHelper)
    pGsLayoutHelper->invalidate();
}

void OdqView::highlightGripPoint(bool bValue,
                                 OdGiDrawablePtr pGripData) //OdExGripDataPtr pGripData)
{
  ODA_ASSERT_ONCE(pGripData.get()); // && m_pRxDatabase == baseDatabaseBy(pGripData->entityId()));
  if (pGripData.isNull()) // || m_pRxDatabase != baseDatabaseBy(pGripData->entityId()))
    return;

  OdGsModel* pGsModel2d = getGsModel(true);
  if (!pGsModel2d)
    return; // grip points are unsupported for this device

  OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(m_pGsDevice);
  OdUInt32 index, 
           size = m_pGsDevice->numViews();
  if (bValue)
  {
    if (pPSHelper.get())
    {
      OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(m_pRxDatabase);
      if (pDwgDb.isNull())
      {
        ODA_FAIL_ONCE(); // TODO
        return;
      }

      //for (index = 0; index < size; index++)
      //  if (bModel==(pPSHelper->viewAt(index) != pPSHelper->overallView()))
      //    pPSHelper->viewAt(index)->add(pGripData, pGsModel2d);
      OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().openObject();
      OdDbAbstractViewportDataPtr pAVD = OdDbAbstractViewportDataPtr(pVpObj);
      if (!pAVD.isNull() && pAVD->gsView(pVpObj))
        pAVD->gsView(pVpObj)->add(pGripData, pGsModel2d);
      else
        pPSHelper->overallView()->add(pGripData, pGsModel2d);
    }
    else
    {
      for (index = 0; index < size; index++)
        m_pGsDevice->viewAt(index)->add(pGripData, pGsModel2d);
    }

    //#9203 // updateGL(); // repaint
    return;
  }

  // hide grip

  //if (pPSHelper.get())
  //{
    for (index = 0; index < size; index++)
      m_pGsDevice->viewAt(index)->erase(pGripData);
  //}
  //else
  //{
  //  for (index = 0; index < size; index++)
  //    m_pGsDevice->viewAt(index)->erase(pGripData);
  //}
  //#9203 //updateGL(); // repaint
}

void OdqView::highlightSnapPoint(bool, // bValue
                                 OdDbStub*, // id
                                 const OdDbBaseFullSubentPath*, // pPath
                                 OdDb::OsnapMode, // osm
                                 const OdGePoint3d&) // point
{
  ODA_FAIL_ONCE(); // TODO
}

void OdqView::keyPressEvent(QKeyEvent* pEvent)
{
  ODA_ASSERT_ONCE(pEvent->isAccepted());
  bool bAccepted = false;
  emit keyPress(this, pEvent, bAccepted);
}

void OdqView::keyReleaseEvent(QKeyEvent* pEvent)
{
  ODA_ASSERT_ONCE(pEvent->isAccepted());
  bool bAccepted = false;
  emit keyRelease(this, pEvent, bAccepted);
}

#pragma region 马梓淇单一文档界面代码

//@function: 左侧间隙圆界面窗口   -- by mzq  2019/9/2 18:22
QWidget* OdqView::initLeftWgt(QWidget *parent/*父窗口*/)
{
	QString clNameQStr = getIConsole()->getQTitle(m_pRxDatabase);
	std::string clNameStr = clNameQStr.toLocal8Bit().toStdString();
	_clType = ClWidget::getCoreInstance()->getClByName(clNameStr)->getClType();
	_ClWidget = ClWidgetFactory::createClWidget(_clType, clNameStr);

	if (ClCate::StraightIStr == _clType || ClCate::StraightVStr == _clType || ClCate::StraightLStr == _clType)
	{
		HdlStraightIWgt straightWgt = static_cast<StraightWgt*>(_ClWidget.data());
		_cadToolBar = straightWgt->getCadToolBar(parent);
	}

	/*有Osg的页面需要获取页面的指针*/
	if (ClCate::MaxAlgle_I == _clType || ClCate::MaxAlgle_V == _clType)
	{
		HdlMaxAngleWgt maxAngleWgt = static_cast<MaxAngleWgt*>(_ClWidget.data());
		_osgWidget = maxAngleWgt->getOsgWidget(parent);
		_cadToolBar = maxAngleWgt->getCadToolBar(parent);
		connect(maxAngleWgt, SIGNAL(changeTabWidgetIndex(int)), this, SLOT(slotChangeTabWidgetIndex(int)));
	}

	_ClWidget->initWgt();//构造界面
	_ClWidget->readDataToForm();//加载初始界面数据
	return _ClWidget;
}

//@function: 图像显示区   -- by mzq  2019/11/29 15:38
QWidget* OdqView::showAreaWgt(QWidget *parent/*父窗口*/)
{
	QWidget *showAreaWgt = new QWidget(parent);

	_showAreaTabWgt = new QTabWidget();
	_showAreaTabWgt->addTab(_osgWidget, QStringLiteral(" 铁塔三维模型 "));
	_showAreaTabWgt->addTab(teighaCadWgt(parent), QStringLiteral(" CAD图纸 "));

	QVBoxLayout *showAreaLyt = new QVBoxLayout;
	showAreaLyt->addWidget(_showAreaTabWgt);
	showAreaLyt->setMargin(0);
	showAreaWgt->setLayout(showAreaLyt);
	return showAreaWgt;
}

//@function: CAD绘图区Gbox   -- by mzq  2019/9/16 15:13
QWidget* OdqView::teighaCadWgt(QWidget *parent/*父窗口*/)
{
	QWidget *teighaCadWgt = new QWidget(parent);

	QVBoxLayout *drawAreaLyt = new QVBoxLayout(teighaCadWgt);
	drawAreaLyt->addWidget(_cadToolBar);
	drawAreaLyt->addWidget(m_pRenderArea);
	drawAreaLyt->setMargin(0);
	teighaCadWgt->setLayout(drawAreaLyt);

	//QStringList lstDirection;
	//lstDirection << QStringLiteral("ext")
	//	<< QStringLiteral("top")
	//	<< QStringLiteral("bottom");

	///*toolbar添加action*/
	//toolBar->addSeparator();
	//QString strIcon;
	//for (int iLoop = 0; iLoop < lstDirection.length(); ++iLoop)
	//{
	//	QAction *action = new QAction;

	//	if (iLoop == 0)
	//		strIcon = QString::fromLocal8Bit(":images/teigha/zoom_") + lstDirection.at(iLoop) + QString::fromLocal8Bit(".png");
	//	else
	//		strIcon = QString::fromLocal8Bit(":images/teigha/view_") + lstDirection.at(iLoop) + QString::fromLocal8Bit(".png");
	//	
	//	action->setIcon(QIcon(strIcon));
	//	action->setToolTip(lstDirection.at(iLoop));

	//	QString strCmd;
	//	if (iLoop == 0)
	//		strCmd = QString::fromLocal8Bit("zoom ") + lstDirection.at(iLoop);
	//	else
	//		strCmd = QString::fromLocal8Bit("view ") + lstDirection.at(iLoop);
	//	
	//	connect(action, &QAction::triggered, [=]() { getIConsole()->executeCommand(strCmd); });
	//	toolBar->addAction(action);
	//}

	return teighaCadWgt;
}

//@function: 保存界面数据   -- by mzq  2019/9/2 18:22
void OdqView::saveWidgetData()
{
	if (_ClWidget)
	{
		_ClWidget->writeDataFromForm();
	}
}

//@function: 绘制间隙圆及生成计算书   -- by mzq  2019/9/2 18:22
void OdqView::drawCl()
{
	if (_ClWidget)
	{
		_ClWidget->drawClAndCreatReport();

		if (ClCate::MaxAlgle_I == _clType || ClCate::MaxAlgle_V == _clType)//如果是最大允许摇摆角页面，则切换至CAD显示区
			slotChangeTabWidgetIndex(1);
	}
}

//@function: 切换当前图形显示tabwidget至相应的页面   -- by mzq  2019/12/18 15:57
void OdqView::slotChangeTabWidgetIndex(int index/*图形tab页面下标*/)
{
	if (_showAreaTabWgt)
		_showAreaTabWgt->setCurrentIndex(index);
}

#pragma endregion