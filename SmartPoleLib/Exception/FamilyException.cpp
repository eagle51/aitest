#include "FamilyException.h"

namespace SmartPoleCore
{
	FamilyException::FamilyException(const HandleComponent& pComp, ErrType ErrInfo)
		:SmartException(pComp.GetPointee()), _errInFo(ErrInfo)
	{
		ostringstream ErrMessage;
		ErrMessage << "������ȱ��" << pComp->getPosition()+1 << "�����ݴ���:";
		switch (_errInFo)
		{
		case ElmNotFound:
			ErrMessage << "��ǰ������/����Ϊ�գ����޸����ĸ˼�";
			break;
		case CnnIndexErr:
			ErrMessage << "�ҽ���Ŵ��󣬳�����������";
			break;
		case UpMaxNumERR:
			ErrMessage << "�϶λ��߱��ε���������������������������";
			break;
		case FDTNumErr:
			ErrMessage << "���޻����̶�Լ���㣬��ע������";
			break;
		}
		_ErrMsg = ErrMessage.str();
	}


	FamilyException::~FamilyException()
	{
	}


}