//#include "StdAfx.h"
#include "Vec3D.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月30日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{

	CVec3D::~CVec3D(void)
	{ 
	}

	CVec3D operator *(Matrix3d &mat,CVec3D &vect)
	{
		double x=mat(0,0)*vect.x()+mat(0,1)*vect.y()+mat(0,2)*vect.z();
		double y=mat(1,0)*vect.x()+mat(1,1)*vect.y()+mat(1,2)*vect.z();
		double z=mat(2,0)*vect.x()+mat(2,1)*vect.y()+mat(2,2)*vect.z();
		return CVec3D(x,y,z);
	};
	CVec3D CVec3D::Multiply(const Matrix3d *mat) const
	{
		double x=mat->coeff(0,0)*this->x()+mat->coeff(0,1)*this->y()+mat->coeff(0,2)*this->z();
		double y=mat->coeff(1,0)*this->x()+mat->coeff(1,1)*this->y()+mat->coeff(1,2)*this->z();
		double z=mat->coeff(2,0)*this->x()+mat->coeff(2,1)*this->y()+mat->coeff(2,2)*this->z();
		return CVec3D(x,y,z);
	}

	void CVec3D::TransferCoor( const Matrix3d *mat )
	{
		double x=mat->coeff(0,0)*this->x()+mat->coeff(0,1)*this->y()+mat->coeff(0,2)*this->z();
		double y=mat->coeff(1,0)*this->x()+mat->coeff(1,1)*this->y()+mat->coeff(1,2)*this->z();
		double z=mat->coeff(2,0)*this->x()+mat->coeff(2,1)*this->y()+mat->coeff(2,2)*this->z();
		_x=x;
		_y=y;
		_z=z;
	}
}