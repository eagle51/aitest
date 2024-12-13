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
// ExGsQtDevice.h
//

#ifndef ODEXGSQTDEVICE_INCLUDED
#define ODEXGSQTDEVICE_INCLUDED

//#include <QtCore>
//#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
//#include <QtWidgets>
//#else
//#include <QtGui>
//#endif
#include <QGLWidget>
#include <QPointer>

#include "CommonDeviceProps.h"
#include "OdPlatform.h"

//typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;
#include "Gs/GsBaseVectorizeDevice.h"

/** Description:
  This class contains methods and properties common to Win32 Vectorizer devices.
  Library: Source *code* provided.
  {group:ExRender_Windows_Classes} 
*/
class ExGsQtDevice : public OdCommonDeviceProps
{
public:
  ExGsQtDevice();
  virtual ~ExGsQtDevice();

  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExGsQtDevice);

  /** Description:
    Sets the device context for this Device object.
  */
  QGLWidget* setGLWidget(QGLWidget* pGLWidget)
  {
    if (pGLWidget == m_pGLWidget)
      return pGLWidget;
    QGLWidget* pGLWidgetPrev = m_pGLWidget;
    m_pGLWidget = pGLWidget;
    return pGLWidgetPrev;
  }
//#if (TD_MAJOR_VERSION > 3 || TD_MINOR_VERSION > 2)
  OdIntPtr setGLWidget(OdIntPtr ptrGLWidget)
  {
    QGLWidget* pGLWidget = qobject_cast<QGLWidget*>((QObject*) ptrGLWidget);
    return (OdIntPtr)(QObject*) setGLWidget(pGLWidget);
  }
//#else
//  // temp solution for mac and linux to work with source v3.2 only
//  OdInt32 setGLWidget(OdInt32 lGLWidget)
//  {
//    QGLWidget* pGLWidget = qobject_cast<QGLWidget*>((QObject*) lGLWidget);
//    return (OdInt32)(QObject*) setGLWidget(pGLWidget);
//  }
//#endif
  QGLWidget* getGLWidget() const
  {
    return m_pGLWidget;
  }

protected:
  
  QPointer<QGLWidget> m_pGLWidget;
  int m_nColorDepth;
  bool m_bColorIndexMode;

protected:
  /** Description:
    Returns true if and only if the vectorization is to be rendered to the screen.
    Note:
    The default implementation of this function always returns true.
  */
  virtual bool renderToScreen() const 
  { 
    return true; 
  }
  /** Description:
    Creates a device context for this Device object.
  */
  virtual void createContext() = 0;
  /** Description:
    Deletes the device context for this Device object.
  */
  virtual void deleteContext() = 0;
  /** Description:
    Creates a logical palette for this Device object.
    Arguments:
    logicalPalette (O) Receives a pointer to the logical palette.
  */
  virtual void createPalette(ODGSPALETTE* logicalPalette);

  /** Description:
    Returns the *color* depth for this Device object.
  */
  virtual int getDeviceColorDepth() const;
};

#endif // ODEXGSQTDEVICE_INCLUDED
