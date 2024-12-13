#pragma once
#include "Member.h"
#include "SmartException.h"
namespace SmartPoleCore
{	
	class MemberException:public SmartException
	{
	public:
		enum ErrType
		{
			NodeNotFound=0,			
			ParaErr=1,
			Duplicate=2,
			ZerosLength=3,		
			ZerosParameter = 4,
			PtNotInLine = 5,
			TypeErr    =7,
			ProfileWorning =8,
			MkindErr   =13,
			MkindBSENErr   =14,
			ConnectErr = 16,
			OriginNodeNotFound=20,
		};
		MemberException(HandleMember ErrMenb, ErrType ErrInfo, int Tagnum = 0);
		~MemberException(void);
		virtual bool isRefPointErr() const
		{
			if (_errInFo == OriginNodeNotFound)
				return true;
			return false;
		}
		virtual bool isDuplicate() const
		{
			if (_errInFo == Duplicate)
				return true;
			return false;
		}
	private:
		void getErrInfo();
		ErrType _errInFo;
		int _tagNum;
	};

}