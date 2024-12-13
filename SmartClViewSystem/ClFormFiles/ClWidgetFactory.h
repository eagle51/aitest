/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.4.4
//Description:	用于生产不同间隙圆界面的工厂类
/****************************************************************/
#pragma once
#include "ClWidget.h"

class ClWidgetFactory
{
   public:
	   static HdlClWidget createClWidget(int clType, std::string clName);

};
