//#include "StdAfx.h"
#include "SecondaryPoint.h"
#include "ExceptionMessage.h"
#include "InnerUtility.h"
#include "XBSecondaryPoint.h"

namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月29日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	 
	CSecondaryPoint::CSecondaryPoint(int iPointNum,Symetry::Type isym,int startPoint,int endPoint,double fRatio)
		:CTowerPoint(iPointNum,isym),m_iJ1(startPoint),m_iJ2(endPoint),m_fRatio(fRatio)
	{

	}
	CSecondaryPoint::~CSecondaryPoint(void)
	{
	}

	//CNode *CSecondaryPoint::CalcCoor(HdNodeArray *aPointSet)
	CNode *CSecondaryPoint::CalcCoor(NodeMap *aPointSet)
	{
		HandleNode pNodeX=aPointSet->Find(this->m_iJ1);
		HandleNode pNodeY=aPointSet->Find(this->m_iJ2);
		if(!pNodeX.isNull() && !pNodeY.isNull())
		{
			if(m_fRatio<=Constants::DISTANCE_EPSILON)
				return NULL;
			double dtemp=1/m_fRatio;
			double dtemp1=pNodeX->x()+dtemp*(pNodeY->x()-pNodeX->x());
			double dtemp2=pNodeX->y()+dtemp*(pNodeY->y()-pNodeX->y());
			double dtemp3=pNodeX->z()+dtemp*(pNodeY->z()-pNodeX->z());
			return new CNode(this->m_iPointNum,dtemp1,dtemp2,dtemp3,this);
		}//找不到标记那个找不到
		return NULL;
	}
	bool CSecondaryPoint::Check()
	{
		bool res=true;
		if(m_fRatio<=Constants::DISTANCE_EPSILON)
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::RatioErr));
			m_fRatio=2;
			res=false;
		}
		if(m_iJ1==m_iJ2 || checkIS(m_iPointNum%10) || m_iPointNum<10)
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::NumErr));
			res=false;
		}
		return res;
	}

	PointTagIter* CSecondaryPoint::UsingTagIter()
	{
		return new SecTagIter(this);
	}

	CTowerPoint* CSecondaryPoint::DeepCopy()
	{
		return new CSecondaryPoint(*this);
	}

	void CSecondaryPoint::Renumbering( RenumberingList* list )
	{
		int newTagNum=list->FindNumTag(m_iPointNum);
		if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
			m_iPointNum=newTagNum;


		newTagNum=list->FindNumTag(m_iJ1);
		if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
			m_iJ1=newTagNum;

		newTagNum=list->FindNumTag(m_iJ2);
		if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
			m_iJ2=newTagNum;
	}

	bool CSecondaryPoint::Equal(CTowerPoint* tmpPoint)
	{
		if(tmpPoint->GetObjType()!=this->GetObjType())
			return false;
		CSecondaryPoint* secPoint=static_cast<CSecondaryPoint*>(tmpPoint);
		if(m_iJ1==secPoint->m_iJ1 && m_iJ2==secPoint->m_iJ2 && abs(m_fRatio-secPoint->m_fRatio)<Constants::SCALAR_EPSILON)
			return true;
		else
			return false;
	}

	CTowerPoint* CSecondaryPoint::ToXBSecondaryPoint()
	{
		if(m_fRatio<=Constants::DISTANCE_EPSILON)
			return new CXBSecondaryPoint(m_iPointNum,10000+m_iJ1,10000+m_iJ2,1,1,0,m_Isym); 
		for (int iloop=3;iloop<20;++iloop)
		{
			if(abs(m_fRatio-iloop)<Constants::SCALAR_EPSILON)
				return new CXBSecondaryPoint(m_iPointNum,10000+m_iJ1,10000+m_iJ2,1,iloop-1,0,m_Isym);
		}

		double otherRatio=1.0/(1.0-(1.0/m_fRatio));
		for (int iloop=3;iloop<20;++iloop)
		{
			if(abs(otherRatio-iloop)<Constants::SCALAR_EPSILON)
				return new CXBSecondaryPoint(m_iPointNum,10000+m_iJ2,10000+m_iJ1,1,iloop-1,0,m_Isym);
		}

		double dtmpRatio=1/m_fRatio;
		if(m_fRatio<=Constants::DISTANCE_EPSILON)
			dtmpRatio=1.0;			
		int secJ1R=int(dtmpRatio*10000);
		int secJ2R=int((1-dtmpRatio)*10000);
		int gcdN=1;
		if(secJ1R!=0 && secJ2R!=0)
			gcdN=InnerUtility::GCD(secJ1R,secJ2R);
		secJ1R/=gcdN;
		secJ2R/=gcdN;
		return new CXBSecondaryPoint(m_iPointNum,10000+m_iJ1,10000+m_iJ2,secJ1R,secJ2R,0,m_Isym);		
	}

	bool CSecondaryPoint::isRefer( int tagNum )
	{
		if(tagNum==m_iJ1)
			return true;
		if(tagNum==m_iJ2)
			return true;
		return false;
	}

	void CSecondaryPoint::TryChangeRatioToInt()
	{
		if(m_fRatio<Constants::SCALAR_EPSILON)
			return;
		for(int iloop=2;iloop<20;++iloop)
		{
			if(abs(m_fRatio-iloop)<Constants::DISTANCE_EPSILON)
			{
				m_fRatio=iloop;
				return;
			}
		}
		double otherRatio=1.0/(1.0-(1.0/m_fRatio));
		for(int iloop=2;iloop<20;++iloop)
		{
			if(abs(otherRatio-iloop)<Constants::DISTANCE_EPSILON)
			{
				std::swap(m_iJ1,m_iJ2);
				m_fRatio=iloop;
				return;
			}
		}
		return;
	}

	SecTagIter::SecTagIter(CSecondaryPoint* secPoint)
		:_SecPoint(secPoint),_Index(3)
	{

	}
	void SecTagIter::First()
	{
		_Index=0;
	}

	void SecTagIter::Next()
	{
		_Index++;
	}

	bool SecTagIter::IsEnd()
	{
		return _Index>1;
	}

	int SecTagIter::CurTagNum()
	{
		switch(_Index)
		{
		case 0:
			return _SecPoint->StartPoint();
		case 1:
			return _SecPoint->EndPoint();
		default:
			return -1;
		}
	}

	void SecTagIter::ChangeCurTagNum( int NewTag )
	{
		switch(_Index)
		{
		case 0:
			_SecPoint->StartPoint(NewTag);
			break;
		case 1:
			_SecPoint->EndPoint(NewTag);
			break;
		}
	}

	SecTagIter::~SecTagIter( void )
	{

	}

}