//#include "StdAfx.h"
#include "PrimaryPointExt.h"
#include "ExceptionMessage.h"
namespace SmartTower
{
	PrimaryPointExt::PrimaryPointExt( int iPointNum/*=0*/,Symetry::Type isym/*=Symetry::None*/,double xcoor/*=0.0*/,double ycoor/*=0.0*/,double zcoor/*=0.0*/, int indexX/*=-1*/,int indexY/*=-1*/,int indexZ/*=-1*/ )
		:_iPointNum(iPointNum),_Isym(isym),_dx(xcoor),_dy(ycoor),_dz(zcoor),_IndexX(indexX),_IndexY(indexY),_IndexZ(indexZ)
	{

	}

	PrimaryPointExt::~PrimaryPointExt(void)
	{
	}

	CPrimaryPoint* PrimaryPointExt::Creator( const FloatVector& paraVec )
	{
		CPrimaryPoint* Result=new CPrimaryPoint(_iPointNum,_Isym,_dx,_dy,_dz);
		if(_IndexX>-1 && _IndexX<paraVec.size())
			Result->x(paraVec[_IndexX]);
		if(_IndexY>-1 && _IndexY<paraVec.size())
			Result->y(paraVec[_IndexY]);
		if(_IndexZ>-1 && _IndexZ<paraVec.size())
			Result->z(paraVec[_IndexZ]);
		return Result;
	}

	bool PrimaryPointExt::Check() 
	{
		return true;
	}
}