#include "PointErrException.h"
#include <sstream>

namespace SmartPoleCore
{	

	PointErrException::PointErrException(const HandlePoint& ErrPoint, ErrType ErrInfo)
		:SmartException(ErrPoint.GetPointee()),_errInFo(ErrInfo)
	{
		this->getErrInfo();
	}

	PointErrException::~PointErrException(void)
	{

	}

	void PointErrException::getErrInfo()
	{
		ostringstream ErrMessage;
		HandlePoint pPoint = static_cast<Point*>(_ErrObject.GetPointee());
		ErrMessage << "节点" << pPoint->TagNum() << "：";
		switch (_errInFo)
		{
		case DuplicateNum:
			ErrMessage << "节点号重复输入";
			break;
		case IsymErr:
			ErrMessage << "对称性错误，对称点重合";
			break;
		case NumErr:
			ErrMessage << "节点编号不符合规则";
			break;
		case RefPointErr:
			ErrMessage << "利用的点无法（生成）找到！";
			break;
		case RatioErr:
			ErrMessage << "利用的点重合，或者比例错误";
			break;
		case FoundationZErr:
			ErrMessage << "同一呼高基础点节点高度Z相同";
			break;
		case FoundationIsymErr:
			ErrMessage << "基础点的对称性不为4";
			break;
		case CoorErr:
			ErrMessage << "坐标值错误";
			break;
		case SymAxisErr:
			ErrMessage << "对称轴符号错误";
			break;
		case IsLinkMem:
			ErrMessage << "没有与杆件相连接";
			break;
		}
		_ErrMsg = ErrMessage.str();
	}

}