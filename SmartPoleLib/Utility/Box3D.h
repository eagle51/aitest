#pragma once
#include "Vec3D.h"
//#include "config.hpp"
namespace SmartPoleCore
{
	class CBox3D
	{
	private:
		double _MinX;
		double _MaxX;
		double _MinY;
		double _MaxY;
		double _MinZ;
		double _MaxZ;
		int _NumExtend;
	public:
		CBox3D(double ix0=0,double iy0=0,double iz0=0,double ix1=0,double iy1=0, double iz1=0);
		double MinX() const
		{
			return _MinX;
		}
		double MaxX() const
		{
			return _MaxX;
		}
		double MinY() const
		{
			return _MinY;
		}
		double MaxY() const
		{
			return _MaxY;

		}
		double MinZ() const
		{
			return _MinZ;
		}
		double MaxZ() const
		{
			return _MaxZ;
		}
		double WidthX() const// Length of X direction
		{
			return _MaxX - _MinX;
		}

		double HeightY() const// Length of Y direction
		{
			return _MaxY - _MinY;
		}
		double LengthZ() const // Length of Z direction
		{
			return _MaxZ - _MinZ;
		}
		void magnify(double fac);
		void extend(double Espion);
		void extend(const CVec3D *Rhs);
		void extend(const CBox3D *rhs);
		void setEmpty();	
		bool IsEmpty()  const
		{
			return (abs(_MinX-_MaxX)+abs(_MinY-_MaxY)+abs(_MinZ-_MaxZ)<Constants::DISTANCE_EPSILON);
		}
		bool contains(CVec3D *rhs)  const
		{
			return (rhs->x()>=_MinX && rhs->x()<=_MaxX && rhs->y()>=_MinY  \
				&& rhs->y()<=_MaxY && rhs->z()>=_MinZ && rhs->z()<=_MaxZ);
		}
		CVec3D getCenter() const
		{
			return CVec3D((_MaxX+_MinX)/2.0,(_MaxY+_MinY)/2.0,(_MaxZ+_MinZ)/2.0);
		}
		~CBox3D(void);
	};
}