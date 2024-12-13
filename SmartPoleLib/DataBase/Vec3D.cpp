#include "Vec3D.h"

namespace SmartPoleCore
{

	CVec3D::~CVec3D(void)
	{
	}

// 	CVec3D operator *(Matrix3d &Mat,CVec3D &Vect)
// 	{
// 		double x=Mat(0,0)*Vect.x()+Mat(0,1)*Vect.y()+Mat(0,2)*Vect.z();
// 		double y=Mat(1,0)*Vect.x()+Mat(1,1)*Vect.y()+Mat(1,2)*Vect.z();
// 		double z=Mat(2,0)*Vect.x()+Mat(2,1)*Vect.y()+Mat(2,2)*Vect.z();
// 		return CVec3D(x,y,z);
// 	};
// 	CVec3D CVec3D::Multiply(const Matrix3d *Mat) const
// 	{
// 		double x=Mat->coeff(0,0)*this->x()+Mat->coeff(0,1)*this->y()+Mat->coeff(0,2)*this->z();
// 		double y=Mat->coeff(1,0)*this->x()+Mat->coeff(1,1)*this->y()+Mat->coeff(1,2)*this->z();
// 		double z=Mat->coeff(2,0)*this->x()+Mat->coeff(2,1)*this->y()+Mat->coeff(2,2)*this->z();
// 		return CVec3D(x,y,z);
// 	}

// 	void CVec3D::TransferCoor( const Matrix3d *Mat )
// 	{
// 		double x=Mat->coeff(0,0)*this->x()+Mat->coeff(0,1)*this->y()+Mat->coeff(0,2)*this->z();
// 		double y=Mat->coeff(1,0)*this->x()+Mat->coeff(1,1)*this->y()+Mat->coeff(1,2)*this->z();
// 		double z=Mat->coeff(2,0)*this->x()+Mat->coeff(2,1)*this->y()+Mat->coeff(2,2)*this->z();
// 		_x=x;
// 		_y=y;
// 		_z=z;
// 	}
}