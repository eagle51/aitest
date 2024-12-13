#pragma once
#include "Vec3D.h"
#include "config.hpp"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
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
	CBox3D(double minX=0.0,double maxX=0.0,double minY=0.0,double maxY=0.0,double minZ=0.0,double maxZ=0.0);
	double MinX() const
	{
		return _MinX;
	}
	void setMinX(double val)
	{
		_MinX = val;
	}
	double MaxX() const
	{
		return _MaxX;
	}
	void setMaxX(double val)
	{
		_MaxX = val;
	}
	double MinY() const
	{
		return _MinY;
	}
	void setMinY(double val)
	{
		_MinY = val;
	}
	double MaxY() const
	{
		return _MaxY;
	}
	void setMaxY(double val)
	{
		_MaxY = val;
	}
	double MinZ() const
	{
		return _MinZ;
	}
	void setMinZ(double val)
	{
		_MinZ = val;
	}
	double MaxZ() const
	{
		return _MaxZ;
	}
	void setMaxZ(double val)
	{
		_MaxZ = val;
	}
	double WidthX() const// Length of X direction
	{
		return _MaxX-_MinX;
	}

	double HeightY() const// Length of Y direction
	{
		return _MaxY-_MinY;
	}
	double LengthZ() const // Length of Z direction
	{
		return _MaxZ-_MinZ;
	}
	double centerX() const
	{
		return 0.5*(_MaxX + _MinX);
	}
	double centerY() const
	{
		return 0.5*(_MinY + _MaxY);
	}
	double centerZ() const
	{
		return 0.5*(_MinZ + _MaxZ);
	}
	void move(double xCoor,double yCoor);
	void magnify(double fac);
	void extend(double Espion);
	void extend(const CVec3D *Rhs);
	void extend(const CBox3D *rhs);
	void extendY(double ycoor);
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