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

#include "OdaCommon.h"
#include "SharedPtr.h"
#include "ExGsQtDevice.h"
#include "TDVersion.h"

//*******************************************************************************/
// Implementation of class ExGsQtDevice

//*******************************************************************************/
//  Constructor()
ExGsQtDevice::ExGsQtDevice()
  : m_nColorDepth(24)
  , m_bColorIndexMode(false)
{
}

ODRX_DECLARE_PROPERTY(QGLWidget)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsQtDevice);
  ODRX_INHERIT_PROPERTIES(OdCommonDeviceProps)
  ODRX_GENERATE_PROPERTY(QGLWidget)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsQtDevice);

//#if (TD_MAJOR_VERSION > 3 || TD_MINOR_VERSION > 2)
  ODRX_DEFINE_PROPERTY_METHODS(QGLWidget, ExGsQtDevice, getGLWidget, setGLWidget, getIntPtr);
//#else
//  // temp solution for mac and linux to work with source v3.2 only
//  ODRX_DEFINE_PROPERTY_METHODS(QGLWidget, ExGsQtDevice, getGLWidget, setGLWidget, getInt32);
//#endif

//*******************************************************************************/
// Destructor()
ExGsQtDevice::~ExGsQtDevice()
{
}

//*******************************************************************************/
// createPalette()
// translate logical logical DD palette to system one 
// (win CPalette via ::CreatePalette(...)
//  Qt colormap of QGLWidget() - TODO after this map is not using by our code)
//
void ExGsQtDevice::createPalette(ODGSPALETTE* pLogPalette) // input logical DD palette
{
  ODA_ASSERT_ONCE(pLogPalette);

  if (!getGLWidget() || !getGLWidget()->isValid() || !m_bColorIndexMode)
    return;

  // fill colormap of QGLWidget via DD logical palette

  QGLColormap cmap = getGLWidget()->colormap();
  int size = pLogPalette->size(),
      index = 0;

  int nLogPaletteSz = (1 << m_nColorDepth);
  if (nLogPaletteSz < size && m_nColorDepth <= 24)
    size = nLogPaletteSz;

  OdSharedPtr< QVector<QRgb> > pClrs = new QVector<QRgb>(size);
  for (; index < size; index++)
    (pClrs.get()->data())[index] = qRgba(ODGETRED(pLogPalette->getAt(index)),
                                         ODGETGREEN(pLogPalette->getAt(index)),
                                         ODGETBLUE(pLogPalette->getAt(index)),
                                         ODGETALPHA(pLogPalette->getAt(index)));
  cmap.setEntries(size, pClrs.get()->data());
  // test - fill with colors ranging from black to white
  //for (index = 0; index < size; index++)
  //  cmap.setEntry(index, qRgb(index, index, index));

  getGLWidget()->setColormap(cmap);
  ODA_ASSERT_VAR(int sizeCM = getGLWidget()->colormap().size();)
  ODA_ASSERT_ONCE(size <= sizeCM || !sizeCM);
}

//*******************************************************************************/
// getDeviceColorDepth()
//
int ExGsQtDevice::getDeviceColorDepth() const
{
  ODA_ASSERT_ONCE(getGLWidget());
  int val = getGLWidget()->depth();
  if (val < 0 || val > 24)
    val = 24;
  return val;
}
