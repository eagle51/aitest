//#include "StdAfx.h"
#include "XBSecondaryPoint.h"
#include "PlaneFunction.h"
#include "ExceptionMessage.h"
#include "TowerModel.h"
namespace SmartTower
{//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月29日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	CXBSecondaryPoint::CXBSecondaryPoint(int iPointNum,int iJ1,int iJ2,int iJ3,int iJ4,int iJ5,Symetry::Type iIIS)
		:CTowerPoint(iPointNum,iIIS),_J1(iJ1),_J2(iJ2),_J3(iJ3),_J4(iJ4),_J5(iJ5)
	{ 

	}
	CXBSecondaryPoint::~CXBSecondaryPoint(void)
	{
	}
	CNode* CXBSecondaryPoint::CalcCoor(NodeMap *aPointSet)
	{
		if(_J1<10000 && _J2<10000) //J1，J2必须大于10000
			return NULL;

		HandleNode pNodeJ1=aPointSet->Find(_J1-10000);
		if(pNodeJ1.isNull())
			return NULL;

		HandleNode pNodeJ2=aPointSet->Find(_J2-10000);
		if(pNodeJ2.isNull())
			return NULL;

		CVec3D VecJ1J2=*pNodeJ2-*pNodeJ1;
		if (Constants::is_zero(VecJ1J2.norm()))
			return NULL;//如果过被利用的两个点重合
				
		if(_J3<10000 && _J3>0 && _J4<10000 && _J4>0 && _J5==0)
		{//radio point
			double radio=_J3*1.0/(_J4+_J3);
			double dtemp1=pNodeJ1->x()+radio*VecJ1J2.x();
			double dtemp2=pNodeJ1->y()+radio*VecJ1J2.y();
			double dtemp3=pNodeJ1->z()+radio*VecJ1J2.z();
			return new CNode(this->m_iPointNum,dtemp1,dtemp2,dtemp3,this);
		}
		else if(_J3>10000 && _J4>10000 && _J5==0 && _J3<20000 &&_J4<20000)
		{//cross point
			HandleNode pNodeJ3=aPointSet->Find(_J3-10000);
			if(pNodeJ3.isNull())
				return NULL;

			HandleNode pNodeJ4=aPointSet->Find(_J4-10000);
			if(pNodeJ4.isNull())
				return NULL;
			CVec3D* tPPoint=LineSegmentHelper::CrossPoint(pNodeJ1.GetPointee(),pNodeJ2.GetPointee(),pNodeJ3.GetPointee(),pNodeJ4.GetPointee());

			if(tPPoint==NULL)
				return NULL;//无法计算出空间点
			//可以计算出坐标
			CNode* res= new CNode(this->m_iPointNum,tPPoint->x(),tPPoint->y(),tPPoint->z(),this);
			//释放内存
			delete tPPoint;
			return res;
		}
		else if(_J3>10000 && _J4>10000 && _J5>10000)
		{
			HandleNode pNodeJ3=aPointSet->Find(_J3-10000);
			if(pNodeJ3.isNull())
				return NULL;

			HandleNode pNodeJ4=aPointSet->Find(_J4-10000);
			if(pNodeJ4.isNull())
				return NULL;

			HandleNode pNodeJ5=aPointSet->Find(_J5-10000);
			if(pNodeJ5.isNull())
				return NULL;

			CPlaneFunction tPlane(pNodeJ3.GetPointee(),pNodeJ4.GetPointee(),pNodeJ5.GetPointee());
			if (!tPlane.isPlane())
				return NULL;
			CVec3D* PPoint=tPlane.CrossPoint(pNodeJ1.GetPointee(),pNodeJ2.GetPointee());
			if(PPoint==NULL)
				return NULL;//无法计算出空间点			
			CNode* res= new CNode(this->m_iPointNum,PPoint->x(),PPoint->y(),PPoint->z(),this);
			//释放内存
			delete PPoint;
			return res;
		}
		else if(_J3>20000 && _J4>20000 && _J5==0)
		{
			HandleNode pNodeJ3=aPointSet->Find(_J3-20000);
			if(pNodeJ3.isNull())
				return NULL;
			HandleNode pNodeJ4=aPointSet->Find(_J4-20000);
			if(pNodeJ4.isNull())
				return NULL;
			double LengthJ3J4=(*pNodeJ4-*pNodeJ3).norm();
			VecJ1J2.normalize();
			double tempx=pNodeJ1->x()+LengthJ3J4*VecJ1J2.x();
			double tempy=pNodeJ1->y()+LengthJ3J4*VecJ1J2.y();
			double tempz=pNodeJ1->z()+LengthJ3J4*VecJ1J2.z();
			return new CNode(this->m_iPointNum,tempx,tempy,tempz,this);
		}
		else if(_J3!=0 && _J4==0 && _J5==0)
		{			
			if(Constants::is_zero(VecJ1J2.x()))
				return 0;
			double xLength=_J3/VecJ1J2.x()/1000;
			if(pNodeJ1->x()>pNodeJ2->x())
				xLength=-xLength;
			double tempx=pNodeJ2->x()+xLength*VecJ1J2.x();
			double tempy=pNodeJ2->y()+xLength*VecJ1J2.y();
			double tempz=pNodeJ2->z()+xLength*VecJ1J2.z();
			return new CNode(this->m_iPointNum,tempx,tempy,tempz,this);
		}
		else if(_J3==0 && _J4!=0 && _J5==0)
		{			
			if(Constants::is_zero(VecJ1J2.y()))
				return 0;
			double yLength=_J4/VecJ1J2.y()/1000;
			if(pNodeJ1->y()>pNodeJ2->y())
				yLength=-yLength;
			double tempx=pNodeJ2->x()+yLength*VecJ1J2.x();
			double tempy=pNodeJ2->y()+yLength*VecJ1J2.y();
			double tempz=pNodeJ2->z()+yLength*VecJ1J2.z();
			return new CNode(this->m_iPointNum,tempx,tempy,tempz,this);
		}
		else if(_J3==0 && _J4==0 && _J5!=0)
		{			
			if(Constants::is_zero(VecJ1J2.z()))
				return 0;
			double zLength=_J5/VecJ1J2.z()/1000;
			if(pNodeJ1->z()>pNodeJ2->z())
				zLength=-zLength;
			double tempx=pNodeJ2->x()+zLength*VecJ1J2.x();
			double tempy=pNodeJ2->y()+zLength*VecJ1J2.y();
			double tempz=pNodeJ2->z()+zLength*VecJ1J2.z();
			return new CNode(this->m_iPointNum,tempx,tempy,tempz,this);
		}
		return NULL;
	}
	bool CXBSecondaryPoint::Check()
	{
		if(_J1<10000 && _J2<10000) //J1，J2必须大于10000
			goto ErrMsg;
		if(checkIS(m_iPointNum%10) ||  m_iPointNum<10)
			goto ErrMsg;
		if(_J3<10000 && _J3>0 && _J4<10000 && _J4>0 && _J5==0)	
			return true;
		else if(_J3>10000 && _J4>10000 && _J5==0 && _J3<20000 &&_J4<20000)				
			return true;
		else if(_J3>10000 && _J4>10000 && _J5>10000)
			return true;
		else if(_J3>20000 && _J4>20000 && _J5==0)
			return true;
		else if(_J3!=0 && _J4==0 && _J5==0)
			return true;
		else if(_J3==0 && _J4!=0 && _J5==0)
			return true;
		else if(_J3==0 && _J4==0 && _J5!=0)
			return true;		
ErrMsg:
		CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this,PointErrException::NumErr));
		return false;
	}

	PointTagIter* CXBSecondaryPoint::UsingTagIter()
	{
		return new XBSecTagIter(this);
	}

	CTowerPoint* CXBSecondaryPoint::DeepCopy()
	{
		return new CXBSecondaryPoint(*this);
	}

	
	void CXBSecondaryPoint::Renumbering( RenumberingList* list )
	{
		int newTagNum=list->FindNumTag(m_iPointNum);
		if(newTagNum>-1 && newTagNum<Constants::MinAuxTagNum)
			m_iPointNum=newTagNum;
		if (_J1>10000)
		{
			newTagNum=list->FindNumTag(_J1%10000);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				_J1=(_J1/10000*10000+newTagNum);
		}
		if (_J2>10000)
		{
			newTagNum=list->FindNumTag(_J2%10000);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				_J2=(_J2/10000*10000+newTagNum);
		}
		if (_J3>10000)
		{
			newTagNum=list->FindNumTag(_J3%10000);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				_J3=(_J3/10000*10000+newTagNum);
		}
		if (_J4>10000)
		{
			newTagNum=list->FindNumTag(_J4%10000);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				_J4=(_J4/10000*10000+newTagNum);
		}
		if (_J5>10000)
		{
			newTagNum=list->FindNumTag(_J5%10000);
			if(newTagNum>0 && newTagNum<Constants::MinAuxTagNum)
				_J5=(_J5/10000*10000+newTagNum);
		}
	}

	bool CXBSecondaryPoint::Equal( CTowerPoint* tmpPoint )
	{
		if(tmpPoint->GetObjType()!=this->GetObjType())
			return false;
		CXBSecondaryPoint* secPoint=static_cast<CXBSecondaryPoint*>(tmpPoint);
		if(_J1==secPoint->_J1 && _J2==secPoint->_J2 && _J3==secPoint->_J3 && _J4==secPoint->_J4 && _J5==secPoint->_J5 )
			return true;
		else
			return false;
	}

	CTowerPoint* CXBSecondaryPoint::globalToDBSecondaryPoint()
	{
		HdNodeArray pNodeArray=CTowerModel::Instance()->getNodeArray();

		HandleNode pJ1Node = pNodeArray->binarySearch(_J1 - 10000);
		HandleNode pJ2Node = pNodeArray->binarySearch(_J2 - 10000);
		HandleNode pResNode = pNodeArray->binarySearch(m_iPointNum);
		if (pJ1Node.isNull())
			return NULL;
		if (pJ2Node.isNull())
			return NULL;
		return this->ToDBSecondaryPoint(pJ1Node, pJ2Node, pResNode);
	}

	CTowerPoint* CXBSecondaryPoint::ToDBSecondaryPoint( const HandleNode& pJ1Node,const HandleNode& pJ2Node,const HandleNode& meNode )
	{
		CVec3D disJ1Pn=(meNode)->Subtraction(pJ1Node.GetPointee());
		CVec3D disJ2Pn=pJ2Node->Subtraction(pJ1Node.GetPointee());
		if(disJ1Pn.norm()<Constants::DISTANCE_EPSILON || disJ2Pn.norm()<Constants::DISTANCE_EPSILON)
			return NULL;
		double dRatio=disJ2Pn.norm()/disJ1Pn.norm();
		CSecondaryPoint* resultSec=new CSecondaryPoint(m_iPointNum,m_Isym,_J1%10000,_J2%10000,dRatio);
		resultSec->TryChangeRatioToInt();
		return resultSec;
	}

	bool CXBSecondaryPoint::isRefer( int tagNum )
	{
		if (_J1>10000 && _J1%10000==tagNum)
				return true;
		if (_J2>10000 && _J2%10000==tagNum)
			return true;
		if (_J3>10000 && _J3%10000==tagNum)
			return true;
		if (_J4>10000 && _J4%10000==tagNum)
			return true;
		if (_J5>10000 && _J5%10000==tagNum)
			return true;
		return false;
	}

	bool CXBSecondaryPoint::bDxfDimMark() const
	{
		if(_J1>10000 && _J2>10000 &&_J3>10000 && _J4>10000 && _J5>10000 && _J3<20000 &&_J4<20000)
			return true;
		return false;
	}

	XBSecTagIter::XBSecTagIter( CXBSecondaryPoint* pXBSecPoint )
		:_XBSecPoint(pXBSecPoint),_Index(6)
	{

	}

	void XBSecTagIter::First()
	{
		_Index=0;
	}

	void XBSecTagIter::Next()
	{
		_Index++;
	}

	bool XBSecTagIter::IsEnd()
	{
		if(_Index==0)
			return false;
		else if (_Index==1)
			return false;
		else if (_Index==2)
		{
			if(_XBSecPoint->J3()>10000)
				return false;
		}
		else if (_Index==3)
		{
			if(_XBSecPoint->J4()>10000)
				return false;
		}
		else if (_Index==4)
		{
			if(_XBSecPoint->J5()>10000)
				return false;
		}
		return true;
	}

	int XBSecTagIter::CurTagNum()
	{		
		if(_Index==0)
			return _XBSecPoint->J1()-10000;
		else if (_Index==1)
			return _XBSecPoint->J2()-10000;
		else if (_Index==2)
		{
			if(_XBSecPoint->J3()>10000)
				return (_XBSecPoint->J3()%10000);
		}
		else if (_Index==3)
		{
			if(_XBSecPoint->J4()>10000)
				return (_XBSecPoint->J4()%10000);
		}
		else if (_Index==4)
		{
			if(_XBSecPoint->J5()>10000)
				return (_XBSecPoint->J5()%10000);

		}
		return -1;
	}

	void XBSecTagIter::ChangeCurTagNum( int newTag )
	{
		if(_Index==0)
		{
			_XBSecPoint->J1(newTag+10000);
		}
		else if (_Index==1)
		{
			_XBSecPoint->J2(newTag+10000);
		}
		else if (_Index==2)
		{
			if(_XBSecPoint->J3()>10000 && _XBSecPoint->J4()>10000 && _XBSecPoint->J3()<20000 && _XBSecPoint->J4()<20000)	
				_XBSecPoint->J3(newTag+10000);
			else if(_XBSecPoint->J3()>20000 && _XBSecPoint->J4()>20000)
				_XBSecPoint->J3(newTag+20000);
		}
		else if (_Index==3)
		{
			if(_XBSecPoint->J3()>10000 && _XBSecPoint->J4()>10000 && _XBSecPoint->J3()<20000 && _XBSecPoint->J4()<20000)	
				_XBSecPoint->J4(newTag+10000);
			else if(_XBSecPoint->J3()>20000 && _XBSecPoint->J4()>20000)
				_XBSecPoint->J4(newTag+20000);
		}
		else if (_Index==4)
		{
			if(_XBSecPoint->J3()>10000 && _XBSecPoint->J4()>10000 && _XBSecPoint->J5()>10000)	
				_XBSecPoint->J5(newTag+10000);
		}
	}

	XBSecTagIter::~XBSecTagIter( void )
	{

	}

	void XBSecTagIter::Current()
	{
		
	}



}