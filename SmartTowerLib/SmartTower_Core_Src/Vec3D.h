#pragma once
#include "Dense"
#include <math.h>
#include "config.hpp"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月30日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
///////////////////////////////////// 
using namespace Eigen;
namespace SmartTower
{
	class CVec3D
	{
	private:
		double _x;
		double _y;
		double _z;
	public:
		CVec3D(double xcoor=0,double ycoor=0,double zcoor=0)
			:_x(xcoor),_y(ycoor),_z(zcoor)
		{	
		};
		void setX(double xcoor)
		{
			_x=xcoor;
		}
		double x() const
		{
			return _x;
		}
		void setY(double ycoor)
		{
			_y=ycoor;
		}
		double y() const 
		{
			return _y;
		}		
		void setZ(double zcoor)
		{
			_z=zcoor;
		}
		double z() const 
		{
			return _z;
		};		
		CVec3D operator - (const CVec3D &rhs) const
		{
			return CVec3D(_x-rhs._x,_y-rhs._y,_z-rhs._z);
		}
		//减
		CVec3D Subtraction(const CVec3D *rhs) const
		{
			return CVec3D(_x-rhs->_x,_y-rhs->_y,_z-rhs->_z);
		}	

		bool CoordEqual(double xCoor, double yCoor, double zCoor) const
		{
			return (abs(xCoor - _x) < Constants::DISTANCE_EPSILON &&
				abs(yCoor - _y) < Constants::DISTANCE_EPSILON &&
				abs(zCoor - _z) < Constants::DISTANCE_EPSILON);
		}

		bool CoordEqual(const CVec3D *rhs) const
		{
			return (abs(_x - rhs->_x) + abs(_y - rhs->_y) + abs(_z - rhs->_z))<Constants::COINCIDENT_EPSILON;
		}

		bool operator == (const CVec3D &rhs) const
		{
			return (abs(_x - rhs._x) + abs(_y - rhs._y) + abs(_z - rhs._z))<Constants::DISTANCE_EPSILON;
		}
		double distanceTo(const CVec3D *rhs) const
		{
			return sqrt((_x-rhs->_x)*(_x-rhs->_x)+(_y-rhs->_y)*(_y-rhs->_y)+(_z-rhs->_z)*(_z-rhs->_z));
		}
		void operator -= (const CVec3D &rhs)
		{
			_x-=rhs._x;
			_y-=rhs._y;
			_z-=rhs._z;
		}
		void operator += (const CVec3D &rhs)
		{
			_x+=rhs._x;
			_y+=rhs._y;
			_z+=rhs._z;		
		}

        //单位向量
		void normalize()
		{
			double len=norm();
			if(len>Constants::SCALAR_EPSILON)
			{
				_x/=len;
				_y/=len;
				_z/=len;
			}
		}

        //向量的模
		double norm() const
		{
			return sqrt(_x*_x+_y*_y+_z*_z);
		}

        //叉乘， 为0，两向量平行
		CVec3D cross(const CVec3D &v) const
		{
			return CVec3D(_y*v._z-v._y*_z,v._x*_z-_x*v._z,_x*v._y-v._x*_y);
		}

        //点乘 为0，两向量垂直
        double dot(const CVec3D &v)  const
        {
            return _x*v._x + _y*v._y + _z*v._z;
        }

		void reverse_vector()
		{
			_x=-_x;
			_y=-_y;
			_z=-_z;
		}

		void setZero()
		{
			_x=_y=_z=0.0;
		}
	
		CVec3D operator + (const CVec3D &rhs)
		{
			return CVec3D(_x+rhs._x,_y+rhs._y,_z+rhs._z);
		}
		CVec3D Addition(const CVec3D *rhs) const
		{
			return CVec3D(_x+rhs->_x,_y+rhs->_y,_z+rhs->_z);
		}
		friend CVec3D operator *(Matrix3d &mat, CVec3D &vect);
		friend CVec3D operator * (const double radio,const CVec3D &rhs)
		{
			return CVec3D(rhs._x*radio,rhs._y*radio,rhs._z*radio);
		};
		void ScaleRatio(double val)
		{
			_x*=val;
			_y*=val;
			_z*=val;
		}
		CVec3D Multiply(const Matrix3d *mat) const;
		void TransferCoor(const Matrix3d *mat);
		~CVec3D(void);
	};
}
