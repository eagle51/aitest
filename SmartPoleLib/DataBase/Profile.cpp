//#include "StdAfx.h"
#include <sstream>
#include <iostream>
#include "Profile.h"
#include "Constants.h"
namespace SmartPoleCore
{
	Profile::Profile(Type itype, int iwide, int thk, double dArea/*=0*/, double IX /*= 0*/, double IY /*= 0*/, double IXY /*= 0*/)
		:_eType(itype), _UIWide(iwide), _iThickness(thk), _fArea(dArea), _fIxx(IX), _fIyy(IY), _dJxy(IXY), _iCtrlCase(0), _dEffic(0)
	{
		if (_UIWide < Constants::SysMaxTagNum)
			_iWide = _UIWide;
		else
			_iWide = 0;
	}
	
	 int Profile::getEdgeNum()
	 {
		 switch (_eType)
		 {
		 case SmartPoleCore::Profile::zero:
			 return 0;
			 break;
		 case SmartPoleCore::Profile::four:
		 case SmartPoleCore::Profile::CSone:
		 case SmartPoleCore::Profile::CStwo:
			 return 4;
			 break;
		 case SmartPoleCore::Profile::six:
			 return 6;
			 break;
		 case SmartPoleCore::Profile::eight:
			 return 8;
			 break;
		 case SmartPoleCore::Profile::twelve:
			 return 12;
			 break;
		 case SmartPoleCore::Profile::sixteen:
			 return 16;
			 break;
		 default:
			 return 9999;
			 break;
		 }
	 }

	 Profile::Type Profile::transfer(int val)
	 {
		 switch (val)
		 {
		 case 0:
			return SmartPoleCore::Profile::zero;
		 case 4:
			 return SmartPoleCore::Profile::four;
		 case 6:
			 return SmartPoleCore::Profile::six;
		 case 8:
			 return SmartPoleCore::Profile::eight;
		 case 12:
			 return SmartPoleCore::Profile::twelve;
		 case 16:
			 return SmartPoleCore::Profile::sixteen;
		 default:
			 return SmartPoleCore::Profile::UNDEFINED;
		 }
	 }

	 const char* Profile::getTypeName() const
	 {
		 switch (_eType)
		 {
		 case SmartPoleCore::Profile::zero:
			return "Round";
			break;
		 case SmartPoleCore::Profile::four:
			return "Square";
			break;
		 case SmartPoleCore::Profile::six:
			return "Hexagon";
			break;
		 case SmartPoleCore::Profile::eight:
			return "Octagon";
			break;
		 case SmartPoleCore::Profile::twelve:
			return "Dodecagon";
			break;
		 case SmartPoleCore::Profile::sixteen:
		    return "Hexadecagon";
			break;
		 case SmartPoleCore::Profile::CSone:
			return "Box";		
			break;
		 case SmartPoleCore::Profile::CStwo:
			return "Groove";
			break;
		 default:
			return "Null";
			break;
		 }
	 }


	 // @ function:  --- by raoyi  2020/03/23 11:42
	Profile::~Profile(void)
	{
	}
}