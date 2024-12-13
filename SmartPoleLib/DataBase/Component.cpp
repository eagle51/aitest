#include "Component.h"
#include "ModelDateBase.h"
#include "ExceptionMessage.h"
#include "FamilyException.h"

namespace SmartPoleCore
{
	Component::Component(int maxNum/*=0*/, int cnnIndex/*=-1*/)
		:_MaxNumber(maxNum), _UserCnnIndex(cnnIndex), _PartPos(0), _upMaxNumber(0), _isDraw(false)
	{
	}

	Component::~Component()
	{
	}

	void Component::initAndCheck(int upMaxNum)
	{
		if (_PartPos == 0)
			_UserCnnIndex = -1;
		_upMaxNumber = upMaxNum;

		HdPointArray gPointArray = ModelDateBase::Instance()->getPointArray();
		_PointArray = gPointArray->getRangePointArray(upMaxNum + 1, _MaxNumber);

		HdMemberArray gMemberArray = ModelDateBase::Instance()->getMemberArray();
		_MemberArray = gMemberArray->getRangeMemberArray(upMaxNum + 1, _MaxNumber);

		if (_upMaxNumber>=_MaxNumber)
			ExceptionMessageRegister::Instance()->SendErrObject(new FamilyException(this, FamilyException::UpMaxNumERR));
		if (_MemberArray->empty())
			ExceptionMessageRegister::Instance()->SendErrObject(new FamilyException(this, FamilyException::ElmNotFound));
		else if (this->isLegType() && !_PointArray->hasFixedPoint())
			ExceptionMessageRegister::Instance()->SendErrObject(new FamilyException(this, FamilyException::FDTNumErr));
	}

}