#include "ClWidgetFactory.h"
#include "DataBase.h"
#include "StraightWgt.h"
#include "StraightWgt_I.h"
#include "StraightWgt_V.h"
#include "StraightWgt_L.h"
#include "MaxAngleWgt_I.h"
#include "MaxAngleWgt_V.h"

//根据间隙圆类型创建间隙圆界面
HdlClWidget ClWidgetFactory::createClWidget(int  clType, std::string clName)
{
	HdlClWidget pClWidget = nullptr;
	auto curClType = SmartCl::ClCate::Type(clType);
	switch (curClType)
	{
	case SmartCl::ClCate::StraightIStr:
		pClWidget = new StraightWgt_I(clType, clName);
		break;
	case SmartCl::ClCate::StraightVStr:
		pClWidget = new StraightWgt_V(clType, clName);
		break;
	case SmartCl::ClCate::StraightLStr:
		pClWidget = new StraightWgt_L(clType, clName);
		break;
	case SmartCl::ClCate::MaxAlgle_I:
		pClWidget = new MaxAngleWgt_I(clType, clName);
		break;
	case SmartCl::ClCate::MaxAlgle_V:
		pClWidget = new MaxAngleWgt_V(clType, clName);
		break;
	  default:
		 break;
	}
	return pClWidget;
}
