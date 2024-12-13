#pragma once
#include <math.h>
//#include <QDataStream>
// #include "Dense"
// #include "config.hpp"
#include "SharedObject.h"
#include "SharedHandle.hpp"
#include "Constants.h"
//using namespace Eigen;
namespace SmartPoleCore
{
	class CVec3D :public SharedObject
	{
	protected:
		double _x;
		double _y;
		double _z;
		int _iTagNum;         //点号
	public:
		CVec3D(double xcoor=0,double ycoor=0,double zcoor=0,int tagNum=0)
			:_x(xcoor), _y(ycoor), _z(zcoor), _iTagNum(tagNum)
		{	

		};
		//点号
		int TagNum() const
		{
			return _iTagNum;
		}
		void TagNum(int iPointNum)
		{
			_iTagNum = iPointNum;
		}
		//绝对坐标X
		void setX(double xcoor)
		{
			_x=xcoor;
		}
		//绝对坐标X
		double x() const
		{
			return _x;
		}
		//绝对坐标Y
		void setY(double ycoor)
		{
			_y=ycoor;
		}
		//绝对坐标Y
		double y() const 
		{
			return _y;
		}		
		//绝对坐标Z
		void setZ(double zcoor)
		{
			_z=zcoor;
		}
		//绝对坐标Z
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
		bool CoordEqual(const CVec3D *rhs) const
		{
			return (abs(_x - rhs->_x) + abs(_y - rhs->_y) + abs(_z - rhs->_z))<Constants::DISTANCE_EPSILON;
		}
		//点号
		bool TagNumEqual(const CVec3D *rhs) const
		{
			return !(_iTagNum ^ rhs->TagNum());
		}
		// @ function: 判断两个点是否在竖直杆上
		bool isVertical() const
		{
			return (abs(_x) + abs(_y))<Constants::SCALAR_EPSILON;
		}
		// @ function: 判断两个点是否在竖直杆上
		bool isVertical(const CVec3D *rhs) const
		{
			return (abs(_x - rhs->_x) + abs(_y - rhs->_y))<Constants::SCALAR_EPSILON;
		}
		bool operator == (const CVec3D &rhs) const
		{
			return (abs(_x - rhs._x) + abs(_y - rhs._y) + abs(_z - rhs._z))<Constants::DISTANCE_EPSILON;
		}
		double ZdistanceTo(const CVec3D *rhs) const
		{
			return sqrt((_z - rhs->_z)*(_z - rhs->_z));
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
		double norm() const
		{
			return sqrt(_x*_x+_y*_y+_z*_z);
		}
		CVec3D Cross(const CVec3D &v) const
		{

			return CVec3D(_y*v._z-v._y*_z,v._x*_z-_x*v._z,_x*v._y-v._x*_y);

		}
		void Reverse_Vector()
		{
			_x=-_x;
			_y=-_y;
			_z=-_z;
		}
		void SetZero()
		{
			_x=_y=_z=0.0;
		}
		double Dot(const CVec3D &v)  const
		{
			return _x*v._x+_y*v._y+_z*v._z;
		}
		CVec3D operator + (const CVec3D &rhs)
		{
			return CVec3D(_x+rhs._x,_y+rhs._y,_z+rhs._z);
		}
		CVec3D Addition(const CVec3D *rhs) const
		{
			return CVec3D(_x+rhs->_x,_y+rhs->_y,_z+rhs->_z);
		}
	//	friend CVec3D operator *(Matrix3d &Mat,CVec3D &Vect);
		friend CVec3D operator * (double radio,const CVec3D &rhs)
		{
			return CVec3D(rhs._x*radio,rhs._y*radio,rhs._z*radio);
		};
		void ScaleRatio(double val)
		{
			_x*=val;
			_y*=val;
			_z*=val;
		}
// 		CVec3D Multiply(const Matrix3d *Mat) const;
// 		void TransferCoor(const Matrix3d *Mat);
		virtual ~CVec3D(void);

		//https://blog.csdn.net/theArcticOcean/article/details/50616240
		// 向量之差
		CVec3D sub(CVec3D p2){
			return CVec3D(_x - p2.x(), _y - p2.y(), _z - p2.z());
		}
		// 叉积
		CVec3D xmulti(CVec3D p2){
			return CVec3D(_y*p2.z() - _z*p2.y(), _z*p2.x() - _x*p2.z(), _x*p2.y() - _y*p2.x());
		}
		// 距离方
		double dis2(CVec3D p2){
			return (_x - p2.x())*(_x - p2.x()) + (_y - p2.y())*(_y - p2.y()) + (_z - p2.z())*(_z - p2.z());
		}

		// 点到线的距离,p1和p2是确定直线的两个点
		double toLineDis(CVec3D p1, CVec3D p2){
			return (sub(p1).xmulti(p1.sub(p2))).norm() / sqrt(p1.dis2(p2));
		}

		bool isInLine(CVec3D p1, CVec3D p2){
			return toLineDis(p1, p2) < Constants::DISTANCE_EPSILON;
		}

// 		virtual void outputToStream(QDataStream& dataStream) const
// 		{
// 			dataStream << _x << _y << _z << _iTagNum;
// 		}
// 		virtual void inputFromStream(QDataStream& dataStream)
// 		{
// 			dataStream >> _x >> _y >> _z >> _iTagNum;
// 		}
	};
	typedef SharedHandle<CVec3D> HandleVec3D;
}
