#pragma once
#include "StHandle.hpp"
#include <exception>
#include "TowerPoint.h"
#include "MemberInf.h"
#include "BasePart.h"
using namespace std;
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月21日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	class TowerException:public std::exception,public SharedObject
	{
	protected:
		std::string _ErrMsg;
		HandleObject _ErrObject;
	public:
		TowerException(HandleObject ErrObject);
		TowerException(const char* ErrMsg);
		HandleObject GetErrObject()
		{
			return _ErrObject;
		}
		virtual const char* what() const
		{
			return _ErrMsg.c_str();
		}
		virtual bool isPointCNNNodeExsit() const
		{
			return false;
		}
		virtual bool isSymErr() const
		{
			return false;
		}
		virtual bool isDuplicate() const
		{
			return false;
		}
		virtual ~TowerException(void);
	};
	typedef StHandle<TowerException> HandleTException;

	class PointErrException:public TowerException
	{

	public:
		enum ErrType
		{
			DuplicateNum=0,
			NumErr=1,
			CNNPNotExsit=2,
			RatioErr=3,
			FoundationZErr=4,
			FoundationIsymErr=5,	
			CoorErr=6,
			AuxABSCoorErr=7,
			IsymErr=8,
		};
		PointErrException(HandleTowerPoint ErrPoint,ErrType ErrInfo);
		virtual bool isPointCNNNodeExsit() const
		{
			if(_errInFo==CNNPNotExsit)
				return true;
			return false;
		}
		virtual bool isSymErr() const
		{
			if(_errInFo==IsymErr)
				return true;
			return false;
		}
		virtual bool isDuplicate() const
		{
			if(_errInFo==DuplicateNum)
				return true;
			return false;
		}
		~PointErrException();
	private:
		ErrType _errInFo;
	};

	class MenberException:public TowerException
	{
	public:
		enum ErrType
		{
			NodeNotFound=0,			
			ParaErr=1,
			Duplicate=2,
			ZerosLength=3,
			ErrPartInfo=4,
			SMCErr     =5,			
			L1L2Err    =6,
			TypeErr    =7,
			ProfileWorning =8,
			ErrDeductHols=9,
			AEfficWorning =10,
			ASRWorning=11,
			BoltEfficWorning=12,
			MkindErr   =13,
			MkindBSENErr   =14,
			ISymErr=15,
			ALenWeight =16,
			OriginNodeNotFound=20,
			RedundantCheckErr=30,
			BlockCheckWorning=40,
		};

		MenberException(HandleMemberInf ErrMenb,ErrType ErrInfo,int Tagnum=0);
		virtual bool isPointCNNNodeExsit() const
		{
			if(_errInFo==OriginNodeNotFound)
				return true;
			return false;
		}
		virtual bool isSymErr() const
		{
			if(_errInFo==ISymErr)
				return true;
			return false;
		}
		virtual bool isDuplicate() const
		{
			if(_errInFo==Duplicate)
				return true;
			return false;
		}
		~MenberException();
	private:
		ErrType _errInFo;
		int _tagNum;
	};
	class PartException:public TowerException
	{ 
	public:
		enum ErrType
		{
			NodeNotFound = 0,
			ElmNotFound = 1,
			CNNErr = 2,
			ERRNumUp = 6,
			FDTNumErr=8,
			ErrFmin=9,
			ExtendNumErr = 10,
			ExtendElmErr=11,
		};
		PartException(HandlePart ErrPart,int ErrTag,ErrType Errtp=NodeNotFound);
		//virtual const char* what() const;
		~PartException();
	private:
		int _ErrTag;
		ErrType _errtp;
	};
}