//#include "StdAfx.h"
#include "PrimaryPoint.h"
#include "ExceptionMessage.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	CPrimaryPoint::CPrimaryPoint(int iPointNum,Symetry::Type isym,double xcoor,double ycoor,double zcoor):
		CTowerPoint(iPointNum, isym), m_dx(xcoor), m_dy(ycoor), m_dz(zcoor), _isFdtPt(false)
	{

	} 
	CPrimaryPoint::~CPrimaryPoint(void)
	{
	}
	bool CPrimaryPoint::Check()
	{
		//_isFdtPt = false;
		if(checkIS(m_iPointNum%10) || m_iPointNum<10)
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::NumErr));
			return false;
		}
		else if(m_dx>10000 && m_dy>10000 && m_dz>10000 && m_dx<50000 && m_dy<50000 && m_dz<50000)
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::NumErr));
			return false;
		}
		else if(m_dx<-1000 || m_dy<-1000 || m_dz<-1000)
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::CoorErr));
			return false;
		}
		return true;
	}
	CNode *CPrimaryPoint::CalcCoor(NodeMap *aPointSet)
	{
		 //计算第一类节点坐标值
		if((m_dx+m_dy+m_dz)<10000) ///第一种情况
		{
			return new CNode(m_iPointNum,m_dx,m_dy,m_dz,this);
		}
		double realCoor=50000;
		if(m_dx>50000 || m_dy>50000 || m_dz>50000)
		{
			double xcoor=m_dx;
			double ycoor=m_dy;
			double zcoor=m_dz;
			if(m_dx>50000)
			{
				int itempxx=int(m_dx-50000);   //正确点1
				HandleNode pNodeX=aPointSet->Find(itempxx);
				if(!pNodeX.isNull())
				{
					xcoor=pNodeX->x();
					realCoor=xcoor;
				}
			}
			if(m_dy>50000)
			{
				int itempyy=int(m_dy-50000);   //正确点1
				HandleNode pNodeY=aPointSet->Find(itempyy);
				if(!pNodeY.isNull())
				{
					ycoor=pNodeY->y();
					realCoor=ycoor;
				}
			}
			if(m_dz>50000)
			{
				int itempzz=int(m_dz-50000);   //正确点1
				HandleNode pNodeZ=aPointSet->Find(itempzz);
				if(!pNodeZ.isNull())
				{
					zcoor=pNodeZ->z();
					realCoor=zcoor;
				}
			}
			if(xcoor<1000 && ycoor<1000 && zcoor<1000)
			{//没有错误的时候才生成点
				return new CNode(m_iPointNum,xcoor,ycoor,zcoor,this);
			}
		}
		if(m_dx>=10000 && m_dx<20000 && m_dy>=10000 && m_dy<20000)  ////第二种情况,前两个大于100000
		{
			int itempxx=int(m_dx-10000);   //正确点1
			int itempyy=int(m_dy-10000);   //正确点2
			HandleNode pNodeX=aPointSet->Find(itempxx);
			HandleNode pNodeY=aPointSet->Find(itempyy);
			if(!pNodeX.isNull() && !pNodeY.isNull())
			{
				double dlenz=pNodeY->z()-pNodeX->z();
				if(dlenz<Constants::DISTANCE_EPSILON && dlenz>-Constants::DISTANCE_EPSILON)
				{
					CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::RatioErr));
				}
				else
				{
					if(this->m_dz>10000 && realCoor>10000)
						return NULL;
					else if(this->m_dz<10000)
						realCoor=this->m_dz;
					double dtemp=(realCoor-pNodeX->z())/(pNodeY->z()-pNodeX->z());
					double dtemp2=pNodeX->x()+dtemp*(pNodeY->x()-pNodeX->x());
					double dtemp3=pNodeX->y()+dtemp*(pNodeY->y()-pNodeX->y());
					return new CNode(this->m_iPointNum,dtemp2,dtemp3,realCoor,this);
				}
			}
		}
		else if(m_dx>=10000 && m_dx<20000 && m_dz>=10000 && m_dz<20000)  //第三种情况,头尾个大于100000
		{
			int itempxx=int(m_dx-10000);   //正确点1
			int itempyy=int(m_dz-10000);   //正确点2
			HandleNode pNodeX=aPointSet->Find(itempxx);
			HandleNode pNodeY=aPointSet->Find(itempyy);
			if(!pNodeX.isNull() && !pNodeY.isNull()) 
			{
				double dleny=pNodeY->y()-pNodeX->y();
				if(dleny<Constants::DISTANCE_EPSILON && dleny>-Constants::DISTANCE_EPSILON)
				{
					CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::RatioErr));
				}
				else
				{
					if(this->m_dy>10000 && realCoor>10000)
						return NULL;
					else if(this->m_dy<10000)
						realCoor=this->m_dy;
					double dtemp=(realCoor-pNodeX->y())/(pNodeY->y()-pNodeX->y());
					double dtemp2=pNodeX->x()+dtemp*(pNodeY->x()-pNodeX->x());
					double dtemp3=pNodeX->z()+dtemp*(pNodeY->z()-pNodeX->z());
					return new CNode(this->m_iPointNum,dtemp2,realCoor,dtemp3,this);
				}
			}
		}
		else if(m_dy>=10000 && m_dy<20000 && m_dz>=10000 && m_dz<20000)  ////第四种情况,最后两个大于100000
		{
			int itempxx=int(m_dy-10000);   //正确点1
			int itempyy=int(m_dz-10000);		//正确点2
			HandleNode pNodeX=aPointSet->Find(itempxx);
			HandleNode pNodeY=aPointSet->Find(itempyy);
			if(!pNodeX.isNull() && !pNodeY.isNull())   //两个都找到了
			{
				double dlenx=pNodeY->x()-pNodeX->x();
				if(dlenx<Constants::DISTANCE_EPSILON && dlenx>-Constants::DISTANCE_EPSILON)
				{
					CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::RatioErr));
				}
				else
				{
					if(this->m_dx>10000 && realCoor>10000)
						return NULL;
					else if(this->m_dx<10000)
						realCoor=this->m_dx;
					double dtemp=(realCoor-pNodeX->x())/(pNodeY->x()-pNodeX->x());
					double dtemp2=pNodeX->y()+dtemp*(pNodeY->y()-pNodeX->y());
					double dtemp3=pNodeX->z()+dtemp*(pNodeY->z()-pNodeX->z());
					return new CNode(this->m_iPointNum,realCoor,dtemp2,dtemp3,this);
				}
			}
		}
		return NULL;
	}

	PointTagIter* CPrimaryPoint::UsingTagIter()
	{
		return new PriTagIter(this);
	}

	CTowerPoint* CPrimaryPoint::DeepCopy()
	{
		return new CPrimaryPoint(*this);
	}

	void CPrimaryPoint::Renumbering( RenumberingList* list )
	{
		int newTagNum=list->FindNumTag(m_iPointNum);
		if(newTagNum>-1 && newTagNum<Constants::MinAuxTagNum)
			m_iPointNum=newTagNum;
		if (m_dx>=10000)
		{
			int xTagNum=int(m_dx-10000*int(m_dx/10000));
			newTagNum=list->FindNumTag(xTagNum);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				m_dx=int(m_dx/10000)*10000+newTagNum;
		}
		if (m_dy>=10000)
		{
			int yTagNum=int(m_dy-10000*int(m_dy/10000));
			newTagNum=list->FindNumTag(yTagNum);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				m_dy=int(m_dy/10000)*10000+newTagNum;
		}
		if (m_dz>=10000)
		{
			int zTagNum=int(m_dz-10000*int(m_dz/10000));
			newTagNum=list->FindNumTag(zTagNum);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				m_dz=int(m_dz/10000)*10000+newTagNum;
		}
	}

	bool CPrimaryPoint::Equal( CTowerPoint* tmpPoint )
	{
		if(tmpPoint->GetObjType()!=this->GetObjType())
			return false;
		CPrimaryPoint* priPoint=static_cast<CPrimaryPoint*>(tmpPoint);
		if((abs(m_dx-priPoint->m_dx)+abs(m_dy-priPoint->m_dy)+abs(m_dz-priPoint->m_dz))<Constants::DISTANCE_EPSILON)
			return true;
		else
			return false;
	}

	bool CPrimaryPoint::isRefer( int tagNum )
	{
		if (m_dx>=10000)
		{
			int xTagNum=int(m_dx-10000);
			if(xTagNum==tagNum)
				return true;
		}
		if (m_dy>=10000)
		{
			int yTagNum=int(m_dy-10000);
			if(yTagNum==tagNum)
				return true;
		}
		if (m_dz>=10000)
		{
			int yTagNum=int(m_dz-10000);
			if(yTagNum==tagNum)
				return true;
		}
		return false;
	}

	bool CPrimaryPoint::bDxfDimMark() const
	{
		return true;
	}

	bool CPrimaryPoint::isFoundationPoint() const
	{
		return _isFdtPt;
	}

	void CPrimaryPoint::setFoundationPointFlag(bool flag)
	{
		_isFdtPt = flag;
	}

	PriTagIter::PriTagIter(CPrimaryPoint* priPoint)
		:_PriPoint(priPoint),_Index(3),_CurrentTag(-1)
	{
		
	}

	void PriTagIter::First()
	{
		if (_PriPoint->IsAbsoluteCoordinates())
			_Index=3;
		else if(_PriPoint->x()>=10000)
			_Index=0;
		else
			_Index=1;
		getCurrent();
	}
	void PriTagIter::Next()
	{	
		do 
		{			
			++_Index;
			getCurrent();
		} while (_CurrentTag<-1 && _Index<3);		
	}

	bool PriTagIter::IsEnd()
	{
		return _CurrentTag<9;
	}

	bool PriTagIter::HasJ1J2()
	{
		if (_PriPoint->IsAbsoluteCoordinates())
			return false;
		return true;
	}
	void PriTagIter::getCurrent()
	{
		switch(_Index)
		{
		case 0:
			_CurrentTag= int(_PriPoint->x()-10000);
			break;
		case 1:
			_CurrentTag= int(_PriPoint->y()-10000);
			break;
		case 2:
			_CurrentTag= int(_PriPoint->z()-10000);
			break;
		default:
			_CurrentTag= -1;
			break;
		}
	}

	int PriTagIter::CurTagNum()
	{
		return _CurrentTag;
	}

	int PriTagIter::J1()
	{
		if (_PriPoint->IsAbsoluteCoordinates())
			return -1;
		if (_PriPoint->x()>=10000)
			return (int(_PriPoint->x()))%10000;
		return int(_PriPoint->y())%10000;
	}

	int PriTagIter::J2()
	{
		if (_PriPoint->IsAbsoluteCoordinates())
			return -1;
		if (_PriPoint->z()>=10000)
			return (int(_PriPoint->z()))%10000;
		return (int(_PriPoint->y()))%10000;
	}

	void PriTagIter::ChangeCurTagNum( int NewTag )
	{
		switch(_Index)
		{
		case 0:
			_PriPoint->x(NewTag+10000);
			break;
		case 1:
			_PriPoint->y(NewTag+10000);
			break;
		case 2:
			_PriPoint->z(NewTag+10000);
			break;
		}
	}

	PriTagIter::~PriTagIter( void )
	{

	}

}