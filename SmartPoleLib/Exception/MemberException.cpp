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
		ErrMessage << "�˼�:" << Menb->MinTagNum() << "--" << Menb->MaxTagNum();
		switch (_errInFo)
		{
		case NodeNotFound:
		case OriginNodeNotFound:
			ErrMessage << "�˲��ڵ㣺" << _tagNum << "�����ڣ�";
			break;
		case ParaErr:
			ErrMessage << "�����������";
			break;
		case Duplicate:
			ErrMessage << "�˼��ظ����룡";
			break;
		case ZerosLength:
			ErrMessage << "�˼�����Ϊ0��";
			break;
		case ZerosParameter:
			ErrMessage << "�˼��������Ϊ0��";
			break;
		case PtNotInLine:
			ErrMessage << "�ڵ㣺" << _tagNum << "��˼������ߣ�";
			break;
		case TypeErr:
			ErrMessage << "�����������ݴ���";
			break;
		case ProfileWorning:
			ErrMessage << "���ʲ�����";
			break;
		case MkindErr:
			ErrMessage << "Լ����Ϣ����ˣ������ϵ�ǰ�淶Լ����Ϣ��д����˶ԣ�";
			break;
		case MkindBSENErr:
			ErrMessage << "Լ����Ϣ����ˣ���ע��ŷ��淶��������һ����˨Լ�������ע��Լ��ת����";
			break;
		case ConnectErr:
			ErrMessage << "���ӵı��岻���ڣ�����ñ����������Ӹ˼���";
		}
		_ErrMsg = ErrMessage.str();
	}


}