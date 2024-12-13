//#include "StdAfx.h"
#include "MemberInf.h"
#include<cmath>
#include "Vec3D.h"
#include "ExceptionMessage.h"
#include "CodeRegister.h"

namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月25日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	MemberInf::MemberInf(int iJ1, int iJ2, int iNmat, Symetry::Type iIsym, int iType, int iM, int iLenmin, int iLenpa, int iSMC) :
		  m_iJ1(iJ1),m_iJ2(iJ2),_NMatGroupNum(iNmat),m_Isym(iIsym),m_iType(iType),m_iMKind(iM),m_iLenmin(iLenmin),m_iLenpa(iLenpa),m_iTTASMC(iSMC)		
	{			
			m_dLen=0.000001;
			_CosZ=0.0;
	};

	MemberInf::MemberInf( const MemberInf& rhs)
	{		
		m_iJ1=rhs.m_iJ1;
		m_iJ2=rhs.m_iJ2;
		_NMatGroupNum=rhs._NMatGroupNum;
		m_Isym=rhs.m_Isym;
		m_iType=rhs.m_iType;
		m_iMKind=rhs.m_iMKind;
		m_iLenmin=rhs.m_iLenmin;
		m_iLenpa=rhs.m_iLenpa;
		m_iTTASMC=rhs.m_iTTASMC;
		m_dLen=rhs.m_dLen;
		_CosZ=rhs._CosZ;
		_OrderIndex=rhs._OrderIndex;

	};

	bool MemberInf::check()
	{
		bool res=true;
		if(m_iJ1==m_iJ2)
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new MenberException(this,MenberException::ZerosLength));
			res=false;
		}
		if(m_iLenmin==0 && m_iLenpa==0) 
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new MenberException(this,MenberException::L1L2Err));
			res=false;
		}
		if (m_iLenmin <0 && m_iLenpa<0)
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new MenberException(this, MenberException::L1L2Err));
			res = false;
		}
		if (m_iType>7 || m_iType<-1 || m_iType==0)//|| m_iType==6
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new MenberException(this,MenberException::TypeErr));
			res=false;
		}
		return res;
	};


	MemberInf::~MemberInf(void)
	{	

	}

	int MemberInf::solveSectCount() const
	{
		int itemp = this->L1();
		if (itemp == 0)
			itemp = this->L2();

		int iNumSect = 0;
		if (itemp<10 && itemp>0)
			iNumSect = itemp;
		else if (itemp == 50)
			iNumSect = 2;
		else if (itemp == 34 || itemp == 33)
			iNumSect = 3;
		else if (itemp == 25)
			iNumSect = 4;
		else if (itemp == 20)
			iNumSect = 5;
		else if (itemp == 16 || itemp == 17)
			iNumSect = 6;
		else if (itemp == 14 || itemp == 15)
			iNumSect = 7;
		else if (itemp >= 10 && itemp <= 1000)
		{
			iNumSect = 100 / itemp;
			if ((100 % itemp) > 8)
				iNumSect++;
		}
		else if (itemp > 1000)
			iNumSect = itemp - 1000;

		return iNumSect;
	}

	void MemberInf::Renumbering(RenumberingList* list)
	{
		int maxNum=this->MaxTagNum();
		if(maxNum<Constants::MinAuxTagNum)
		{
			int newTagNum=list->FindNumTag(m_iJ1);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				m_iJ1=newTagNum;

			newTagNum=list->FindNumTag(m_iJ2);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				m_iJ2=newTagNum;
		}
		else
		{
			int newTagNum=list->FindNumTag(m_iJ1);
			if(newTagNum>0)
				m_iJ1=newTagNum;

			newTagNum=list->FindNumTag(m_iJ2);
			if(newTagNum>0)
				m_iJ2=newTagNum;
		}
	}

	MemberInf& MemberInf::operator=( const MemberInf& rhs )
	{
		if(this==&rhs)
			return *this;
		m_iJ1=rhs.m_iJ1;
		m_iJ2=rhs.m_iJ2;
		_NMatGroupNum=rhs._NMatGroupNum;
		m_Isym=rhs.m_Isym;
		m_iType=rhs.m_iType;
		m_iMKind=rhs.m_iMKind;
		m_iLenmin=rhs.m_iLenmin;
		m_iLenpa=rhs.m_iLenpa;
		m_iTTASMC=rhs.m_iTTASMC;
		m_dLen=rhs.m_dLen;
		_CosZ=rhs._CosZ;
		_OrderIndex=rhs._OrderIndex;
		return *this;
	}
}
