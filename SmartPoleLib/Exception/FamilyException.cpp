#include "FamilyException.h"

namespace SmartPoleCore
{
	FamilyException::FamilyException(const HandleComponent& pComp, ErrType ErrInfo)
		:SmartException(pComp.GetPointee()), _errInFo(ErrInfo)
	{
		ostringstream ErrMessage;
		ErrMessage << "本体接腿表第" << pComp->getPosition()+1 << "行数据错误:";
		switch (_errInFo)
		{
		case ElmNotFound:
			ErrMessage << "当前本本体/接腿为空，尚无附属的杆件";
			break;
		case CnnIndexErr:
			ErrMessage << "挂接序号错误，超出本体数量";
			break;
		case UpMaxNumERR:
			ErrMessage << "上段或者本段的最大号码错误，最大号码必须依次增大";
			break;
		case FDTNumErr:
			ErrMessage << "尚无基础固定约束点，请注意设置";
			break;
		}
		_ErrMsg = ErrMessage.str();
	}


	FamilyException::~FamilyException()
	{
	}


}