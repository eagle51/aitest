#include "DrawCl.h"
#include "..\SmartClApp\OdqMainWindow.h"
#include "OdqInterfaces.h"

//@function:    -- by mzq  2019/7/15 10:37
DrawCl::DrawCl()
{
	_insulatorBaseLengh = 10.0;//绝缘子串基准长度，用于确定fontScale用
	_plottingScale = 100;//绘图比例 1:100

	_vecHorizontalUnit = OdGeVector3d::kXAxis;
	_vecVerticalUnit = OdGeVector3d::kYAxis;
	_vecHorizontalUnit.normalize();
	_vecVerticalUnit.normalize();
}

//@function:    -- by mzq  2019/7/15 10:38
DrawCl::~DrawCl()
{

}

//@function: 内核数据库   -- by mzq  2019/12/9 9:58
SmartCl::DataBase* DrawCl::getCoreInstance()
{
	return static_cast<OdqMainWindow*>(getIApp()->getMainWindow())->getCoreDbPt();
}

//@function: 深思加密检查   -- by mzq 2022/11/4 18:18
AboutSoftWare::VirBoxChecker* DrawCl::getVirBoxCheckerInstance()
{
	return static_cast<OdqMainWindow*>(getIApp()->getMainWindow())->getVirBoxChecker();
}
