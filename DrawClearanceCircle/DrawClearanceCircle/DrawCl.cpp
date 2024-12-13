#include "DrawCl.h"
#include "..\SmartClApp\OdqMainWindow.h"
#include "OdqInterfaces.h"

//@function:    -- by mzq  2019/7/15 10:37
DrawCl::DrawCl()
{
	_insulatorBaseLengh = 10.0;//��Ե�Ӵ���׼���ȣ�����ȷ��fontScale��
	_plottingScale = 100;//��ͼ���� 1:100

	_vecHorizontalUnit = OdGeVector3d::kXAxis;
	_vecVerticalUnit = OdGeVector3d::kYAxis;
	_vecHorizontalUnit.normalize();
	_vecVerticalUnit.normalize();
}

//@function:    -- by mzq  2019/7/15 10:38
DrawCl::~DrawCl()
{

}

//@function: �ں����ݿ�   -- by mzq  2019/12/9 9:58
SmartCl::DataBase* DrawCl::getCoreInstance()
{
	return static_cast<OdqMainWindow*>(getIApp()->getMainWindow())->getCoreDbPt();
}

//@function: ��˼���ܼ��   -- by mzq 2022/11/4 18:18
AboutSoftWare::VirBoxChecker* DrawCl::getVirBoxCheckerInstance()
{
	return static_cast<OdqMainWindow*>(getIApp()->getMainWindow())->getVirBoxChecker();
}
