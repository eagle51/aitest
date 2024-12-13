#include "MemberException.h"

namespace SmartPoleCore
{	

	MemberException::MemberException(HandleMember ErrMenb, ErrType ErrInfo, int Tagnum/*=0*/)
		:SmartException(ErrMenb.GetPointee()),_errInFo(ErrInfo),_tagNum(Tagnum)
	{
		this->getErrInfo();
	}

	MemberException::~MemberException(void)
	{

	}

	void MemberException::getErrInfo()
	{
		ostringstream ErrMessage;
		Member *Menb = static_cast<Member*>(_ErrObject.GetPointee());
		ErrMessage << "杆件:" << Menb->MinTagNum() << "--" << Menb->MaxTagNum();
		switch (_errInFo)
		{
		case NodeNotFound:
		case OriginNodeNotFound:
			ErrMessage << "端部节点：" << _tagNum << "不存在！";
			break;
		case ParaErr:
			ErrMessage << "输入参数错误！";
			break;
		case Duplicate:
			ErrMessage << "杆件重复输入！";
			break;
		case ZerosLength:
			ErrMessage << "杆件长度为0！";
			break;
		case ZerosParameter:
			ErrMessage << "杆件截面参数为0！";
			break;
		case PtNotInLine:
			ErrMessage << "节点：" << _tagNum << "与杆件不共线！";
			break;
		case TypeErr:
			ErrMessage << "截面类型数据错误！";
			break;
		case ProfileWorning:
			ErrMessage << "材质不合理！";
			break;
		case MkindErr:
			ErrMessage << "约束信息填错了，不符合当前规范约束信息填写，请核对！";
			break;
		case MkindBSENErr:
			ErrMessage << "约束信息填错了，请注意欧标规范单面连接一颗螺栓约束填法，请注意约束转化！";
			break;
		case ConnectErr:
			ErrMessage << "连接的本体不存在！或与该本体无相连接杆件！";
		}
		_ErrMsg = ErrMessage.str();
	}


}