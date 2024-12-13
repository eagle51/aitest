#pragma once
#include "SmartException.h"
#include "Component.h"
namespace SmartPoleCore
{
	class FamilyException :public SmartException
	{
	public:
		enum ErrType
		{
			ElmNotFound = 1,
			CnnIndexErr = 2,
			UpMaxNumERR = 6,
			FDTNumErr = 8,
		};
		FamilyException(const HandleComponent& pComp, ErrType ErrInfo);
		~FamilyException();
	private:
		void getErrInfo();
		ErrType _errInFo;
	};


}