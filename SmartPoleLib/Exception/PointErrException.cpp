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
		ErrMessage << "�ڵ�" << pPoint->TagNum() << "��";
		switch (_errInFo)
		{
		case DuplicateNum:
			ErrMessage << "�ڵ���ظ�����";
			break;
		case IsymErr:
			ErrMessage << "�Գ��Դ��󣬶ԳƵ��غ�";
			break;
		case NumErr:
			ErrMessage << "�ڵ��Ų����Ϲ���";
			break;
		case RefPointErr:
			ErrMessage << "���õĵ��޷������ɣ��ҵ���";
			break;
		case RatioErr:
			ErrMessage << "���õĵ��غϣ����߱�������";
			break;
		case FoundationZErr:
			ErrMessage << "ͬһ���߻�����ڵ�߶�Z��ͬ";
			break;
		case FoundationIsymErr:
			ErrMessage << "������ĶԳ��Բ�Ϊ4";
			break;
		case CoorErr:
			ErrMessage << "����ֵ����";
			break;
		case SymAxisErr:
			ErrMessage << "�Գ�����Ŵ���";
			break;
		case IsLinkMem:
			ErrMessage << "û����˼�������";
			break;
		}
		_ErrMsg = ErrMessage.str();
	}

}