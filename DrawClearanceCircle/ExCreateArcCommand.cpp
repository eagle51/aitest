#pragma once
#include "DrawClearanceCircle\DrawStraight_I.h"
#include "DrawClearanceCircle\DrawStraight_VL.h"
#include "DrawClearanceCircle\DrawMaxAngle_I.h"
#include "DrawClearanceCircle\DrawMaxAngle_V.h"
#include "DrawClearanceCircle\CreateEnt.h"

#include "ExStringIO.h"
#include "OdqInterfaces.h"
#include "..\SmartClConsole\CommandContextImpl.h"
#include "ExCommandsUtils.h"

/*绘图主入口*/
void _CreateArc_func(OdEdCommandContext* pCmdCtx)
{
	//获取名称
	OdString sClName;
	OdSmartPtr<ExStringIO> pStartUpSiteIO = pCmdCtx->arbitraryData(OD_T("CreateArc/CLNAME"));
	if (pStartUpSiteIO.get())
	{
		sClName = pStartUpSiteIO->getString(OD_T(""));
	}
	if (sClName.isEmpty())
	{
		return;
	}
	QString clName = getIApp()->toQString(sClName);

	//获取类型 //间隙圆类型 class ClCate
	OdString sClType;
	OdSmartPtr<ExStringIO> pIO_ClType = pCmdCtx->arbitraryData(OD_T("CreateArc/CLTYPE"));
	if (pIO_ClType.get())
	{
		sClType = pIO_ClType->getString(OD_T(""));
	}
	if (sClType.isEmpty())
	{
		return;
	}
	QString clType = getIApp()->toQString(sClType);

	SmartCl::ClCate::Type clCateType = SmartCl::ClCate::strToType(clType.toLocal8Bit().toStdString());

	CreateEnt::_pCmdCtx = pCmdCtx;
	CreateEnt::clearModelSpace();//清空图纸
	CreateEnt::zoom_window(OdGePoint3d(0, 0, 0), OdGePoint3d(0.01, 0.01, 0));

	switch (clCateType)
	{
	case SmartCl::ClCate::StraightIStr:
	{
										  DrawStraight_I draw;
										  draw.drawStraight_I(clName);
										  break;
	}
	case SmartCl::ClCate::StraightVStr:
	case SmartCl::ClCate::StraightLStr:
	{
										  DrawStraight_VL draw;
										  draw.drawStraight_VL(clName);
										  break;
	}
	case SmartCl::ClCate::MaxAlgle_I:
	{
										  DrawMaxAngle_I draw;
										  draw.drawMaxAngle_I(clName);
										  break;
	}
	case SmartCl::ClCate::MaxAlgle_V:
	{
										DrawMaxAngle_V draw;
										draw.drawMaxAngle_V(clName);
										break;
	}
	default:
		break;
	}
}