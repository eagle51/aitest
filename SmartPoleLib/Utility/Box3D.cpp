//#include "StdAfx.h"
#include "Box3D.h"

namespace SmartPoleCore
{
	CBox3D::CBox3D(double ix0,double iy0,double iz0,double ix1,double iy1, double iz1):
		_MinX(ix0),_MaxX(iy0),_MinY(iz0),_MaxY(ix1),_MinZ(iy1),_MaxZ(iz1),_NumExtend(0)
	{

	}
	void CBox3D::setEmpty()
	{
		_MinX=_MaxX=_MinY=_MaxY=_MinZ=_MaxZ=0.0;
		_NumExtend=0;
	};

	CBox3D::~CBox3D(void)
	{
	}

	void CBox3D::extend( const CBox3D *rhs )
	{
		if(_MinX>rhs->_MinX)
			_MinX=rhs->_MinX;
		if(_MinY>rhs->_MinY)
			_MinY=rhs->_MinY;
		if(_MinZ>rhs->_MinZ)
			_MinZ=rhs->_MinZ;
		if(_MaxX<rhs->_MaxX)
			_MaxX=rhs->_MaxX;
		if(_MaxY<rhs->_MaxY)
			_MaxY=rhs->_MaxY;
		if(_MaxZ<rhs->_MaxZ)
			_MaxZ=rhs->_MaxZ;
	}

	void CBox3D::extend(const CVec3D *Rhs)
	{
		if (0<_NumExtend)
		{
			if (Rhs->x() < _MinX)
			{
				_MinX = Rhs->x();
			}
			else if (Rhs->x() > _MaxX)
			{
				_MaxX = Rhs->x();
			}

			if (Rhs->y() < _MinY)
			{
				_MinY = Rhs->y();
			}
			else if (Rhs->y()> _MaxY)
			{
				_MaxY = Rhs->y();
			}
			if (Rhs->z() < _MinZ)
			{
				_MinZ = Rhs->z();
			} 
			else if (Rhs->z() > _MaxZ)
			{
				_MaxZ = Rhs->z();
			}
		}
		else
		{
			_MaxX = _MinX =Rhs->x();
			_MaxY = _MinY =Rhs->y();
			_MaxZ = _MinZ =Rhs->z();;
		}
		++_NumExtend;
	}

	void CBox3D::extend( double Espion )
	{
		_MinX-=Espion;
		_MaxX+=Espion;
		_MinY-=Espion;
		_MaxY+=Espion;
		_MinZ-=Espion;
		_MaxZ+=Espion;
	}

	void CBox3D::magnify( double fac )
	{
		double cx=(_MaxX+_MinX)/2.0;
		double cy=(_MaxY+_MinY)/2.0;
		double cz=(_MaxZ+_MinZ)/2.0;
		double Wi = WidthX()*fac;
		double He = HeightY()*fac;
		double Le = LengthZ()*fac;
		_MinX=cx-Wi/2.0;
		_MaxX=cx+Wi/2.0;
		_MinY=cy-He/2.0;
		_MaxY=cy+He/2.0;
		_MinZ=cz-Le/2.0;
		_MaxZ=cz+Le/2.0;
	}

	/*
	CBox3D CBox3D::operator + (CBox3D &rhs)
	{
		CBox3D box;
		box._MinX=(_MinX<rhs._MinX)?_MinX:rhs._MinX;
		box._MinY=(_MinY<rhs._MinY)?_MinY:rhs._MinY;
		box._MinZ=(_MinZ<rhs._MinZ)?_MinZ:rhs._MinZ;
		box._MaxX=(_MaxX>rhs._MaxX)?_MaxX:rhs._MaxX;
		box._MaxY=(_MaxY>rhs._MaxY)?_MaxY:rhs._MaxY;
		box._MaxZ=(_MaxZ>rhs._MaxZ)?_MaxZ:rhs._MaxZ;
		return box;
	}*/

}