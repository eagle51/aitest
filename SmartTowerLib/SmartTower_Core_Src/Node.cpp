//#include "StdAfx.h"
#include "Node.h"
#include "config.hpp"
#include "ExceptionMessage.h"

//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
///////////////////////////////////// 

namespace SmartTower
{
	const int dofset::max_known_dof = 6;
	const int dofset::Xdisp = 1 << 0;
	const int dofset::Ydisp = 1 << 1;
	const int dofset::Zdisp = 1 << 2;
	const int dofset::XYZdisp =(1 << 0)+(1 << 1) + (1 << 2);
	const int dofset::Xrot = 1 << 3;
	const int dofset::Yrot  = 1 << 4;
	const int dofset::Zrot  = 1 << 5;
	const int dofset::XYZrot= (1 << 3) + (1 << 4) + (1 << 5);
	const int dofset::FixedAll=(1 << 0)+(1 << 1) + (1 << 2) + (1 << 3)+(1 << 4) + (1 << 5);

	dofset::~dofset(void)
	{
	}

	dofset::dofset( int flag/*=0*/ ):_dofflag(flag)
	{
	}

	GBCoordinateSystem::~GBCoordinateSystem()
	{
	}


	GBCoordinateSystem::GBCoordinateSystem(CVec3D *pOrigin,CVec3D *pPointX,CVec3D *pPointY):_OriginalCoor(*pOrigin)
	{
		//通过三个点计算坐标系
		_AxisX=*pPointX-*pOrigin;
		_AxisX.normalize();
		_AxisY=*pPointY-*pOrigin;
		_AxisY.normalize();
		_AxisZ=_AxisX.cross(_AxisY);
		_AxisZ.normalize();
		_AxisY=_AxisZ.cross(_AxisX);
	}
	const Matrix3d* GBCoordinateSystem::TranferMat() const
	{			
		_TransferMat->coeffRef(0,0)=_AxisX.x();
		_TransferMat->coeffRef(0,1)=_AxisY.x();
		_TransferMat->coeffRef(0,2)=_AxisZ.x();

		_TransferMat->coeffRef(1,0)=_AxisX.y();
		_TransferMat->coeffRef(1,1)=_AxisY.y();
		_TransferMat->coeffRef(1,2)=_AxisZ.y();

		_TransferMat->coeffRef(2,0)=_AxisX.z();
		_TransferMat->coeffRef(2,1)=_AxisY.z();
		_TransferMat->coeffRef(2,2)=_AxisZ.z();		
		return _TransferMat;
	}

	void GBCoordinateSystem::Rot( const CVec3D& axisV,double degree )
	{
		Eigen::AngleAxisd tansAA(degree,Eigen::Vector3d(axisV.x(),axisV.y(),axisV.z()));
		Matrix3d transMat1=tansAA.toRotationMatrix();
		_AxisX.TransferCoor(&transMat1);
		_AxisY.TransferCoor(&transMat1);
		_AxisZ.TransferCoor(&transMat1);
	}


	Matrix3d* GBCoordinateSystem::_TransferMat=new Matrix3d();

	

	CNode::~CNode(void)
	{

	}

	int CNode::ISymTagNum(Symetry::Type isym)
	{		
		if(isym==Symetry::None)
			return this->TagNum();

		int itemp=(_TagNum%10);
		if(isym==Symetry::Xsym && abs(this->x())>Constants::DISTANCE_EPSILON)
		{
			if(itemp==1 || itemp==3)
				return this->TagNum()-1;
			else
				return this->TagNum()+1;
		}
		else if(isym==Symetry::Ysym && abs(this->y())>Constants::DISTANCE_EPSILON)
		{
			if(itemp==2 || itemp==3)
				return this->TagNum()-2;
			else
				return this->TagNum()+2;
		}//有四个对称点
		else if(isym==Symetry::Osym && (abs(this->x())>Constants::DISTANCE_EPSILON && abs(this->y())>Constants::DISTANCE_EPSILON ))
		{
			switch(itemp)
			{
			case 0:
				return this->TagNum()+3;
			case 1:
				return this->TagNum()+1;
			case 2:
				return this->TagNum()-1;
			case 3:
				return this->TagNum()-3;
			}
		}//只有连个对称点
		else if (isym==Symetry::Osym && (abs(this->x())>Constants::DISTANCE_EPSILON && abs(this->y())<Constants::DISTANCE_EPSILON ))
		{
			switch(itemp)
			{
			case 0:
			case 2:
				return this->TagNum()+1;
			case 1:
			case 3:
				return this->TagNum()-1;
			}
		}
		else if (isym==Symetry::Osym && (abs(this->x())<Constants::DISTANCE_EPSILON && abs(this->y())>Constants::DISTANCE_EPSILON ))
		{
			switch(itemp)
			{
			case 0:
			case 1:
				return this->TagNum()+2;
			case 2:
			case 3:
				return this->TagNum()-2;
			}
		}
		return this->TagNum();
	}	
	CNode* CNode::getISNode(Symetry::Type isym)
	{
		if(isym==Symetry::None)
			return new CNode(*this);
		int itemp=(_TagNum%10);
		if(isym==Symetry::Xsym && abs(this->x())>Constants::DISTANCE_EPSILON)
		{
			if(itemp==1||itemp==3)
				return new CNode(this->TagNum()-1,-this->x(),this->y(),this->z(),this->_OriginNum);
			else
				return new CNode(this->TagNum()+1,-this->x(),this->y(),this->z(),this->_OriginNum);
		}
		else if(isym==Symetry::Ysym && abs(this->y())>Constants::DISTANCE_EPSILON)
		{
			if(itemp==2||itemp==3)
				return new CNode(this->TagNum()-2,this->x(),-this->y(),this->z(),this->_OriginNum);
			else
				return new CNode(this->TagNum()+2,this->x(),-this->y(),this->z(),this->_OriginNum);
		}
		else if(isym==Symetry::Osym && (abs(this->x())>Constants::DISTANCE_EPSILON && abs(this->y())>Constants::DISTANCE_EPSILON ))
		{
			switch(itemp)
			{
			case 0:
				return new CNode(this->TagNum()+3,-this->x(),-this->y(),this->z(),this->_OriginNum);
				break;
			case 1:
				return new CNode(this->TagNum()+1,-this->x(),-this->y(),this->z(),this->_OriginNum);
				break;
			case 2:
				return new CNode(this->TagNum()-1,-this->x(),-this->y(),this->z(),this->_OriginNum);
				break;
			case 3:
				return new CNode(this->TagNum()-3,-this->x(),-this->y(),this->z(),this->_OriginNum);
				break;
			}
		}
		//只有连个对称点
		else if (isym==Symetry::Osym && (abs(this->x())>Constants::DISTANCE_EPSILON && abs(this->y())<Constants::DISTANCE_EPSILON ))
		{
			switch(itemp)
			{
			case 0:
			case 2:
				return new CNode(this->TagNum()+1,-this->x(),this->y(),this->z(),this->_OriginNum);
				break;
			case 1:
			case 3:
				return new CNode(this->TagNum()-1,-this->x(),this->y(),this->z(),this->_OriginNum);
				break;
			}
		}
		else if (isym==Symetry::Osym && (abs(this->x())<Constants::DISTANCE_EPSILON && abs(this->y())>Constants::DISTANCE_EPSILON ))
		{
			switch(itemp)
			{
			case 0:
			case 1:
				return new CNode(this->TagNum()+2,this->x(),-this->y(),this->z(),this->_OriginNum);
				break;
			case 2:
			case 3:
				return new CNode(this->TagNum()-2,this->x(),-this->y(),this->z(),this->_OriginNum);
				break;
			}
		}
		else if(isym==Symetry::Xsym && abs(this->x())<Constants::DISTANCE_EPSILON)
		{
			return new CNode(*this);
		}
		else if(isym==Symetry::Ysym && abs(this->y())<Constants::DISTANCE_EPSILON)
		{
			return new CNode(*this);
		}
		else if (isym==Symetry::Osym && (abs(this->x())<Constants::DISTANCE_EPSILON && abs(this->y())<Constants::DISTANCE_EPSILON ))
		{
			return new CNode(*this);
		}		
		CExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(_OriginNum,PointErrException::NumErr));
		return NULL;
	}

	CNode::CNode( int iNum/*=0*/,double xcoor/*=0*/,double ycoor/*=0*/,double zcoor/*=0*/,HandleTowerPoint iOriginNum/*=0*/ )
		:CVec3D(xcoor,ycoor,zcoor),_TagNum(iNum),_OriginNum(iOriginNum),_DoFStartIndex(0),_DOFCount(3),_SingularType(Singular)
	{	  

	};

	int CNode::LogicalAffiliateSymetryFlag() const
	{
		if(this->x()>=0 && this->y()>=0)
			return 0;
		else if(this->x()<0 && this->y()>=0)
			return 1;
		else if(this->x()>=0 && this->y()<0)
			return 2;
		else
			return 3;
	}

	bool CNode::HasRealNode( Symetry::Type isym )
	{
		
		if(isym==Symetry::None)
			return true;
		else if(isym==Symetry::Xsym)
		{
			if(abs(this->x())<Constants::DISTANCE_EPSILON)
				return true;
			else if(_OriginNum.isNull())
				return false;
			else if(_OriginNum->Isym()==Symetry::Xsym || _OriginNum->Isym()==Symetry::XYsym)
				return true;
			else
				return false;
		}
		else if(isym==Symetry::Ysym)
		{
			if(abs(this->y())<Constants::DISTANCE_EPSILON)
				return true;
			else if(_OriginNum.isNull())
				return false;
			else if(_OriginNum->Isym()==Symetry::Ysym || _OriginNum->Isym()==Symetry::XYsym)
				return true;
			else
				return false;
		}
		else if(isym==Symetry::Osym)
		{
			if(abs(this->y())<Constants::DISTANCE_EPSILON || abs(this->x())<Constants::DISTANCE_EPSILON)
				return true;
			else if(_OriginNum.isNull())
				return false;
			else if(_OriginNum->Isym()==Symetry::Osym || _OriginNum->Isym()==Symetry::XYsym)
				return true;
			else
				return false;
		}
		return false;
	}

	bool CNode::IsFirstQuadrant() const
	{
		if(this->x()>=-Constants::SCALAR_EPSILON && this->y()>=-Constants::SCALAR_EPSILON)
			return true;
		return false;
	}
}