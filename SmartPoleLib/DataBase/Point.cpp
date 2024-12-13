#include <complex>
#include "Point.h"
#include "Constants.h"
#include "Node.h"
#include "PointMap.h"
#include "PointErrException.h"
#include "ExceptionMessage.h"
#include "Auxiliary.h"


namespace SmartPoleCore
{
	Point::Point(int iPointNum /*= 0*/, double xcoor /*= 0.0*/, double ycoor /*= 0.0*/, double zcoor /*= 0.0*/, bool bfixed /*= false*/)
		: CVec3D(xcoor, ycoor, zcoor, iPointNum), _refX(xcoor), _refY(ycoor), _refZ(zcoor), _isFixed(bfixed), _DrawZ(0)
	{

	}

	Point::Point(const CVec3D* coorPt, int newTagNum, bool bfixed /*= false*/)
		: CVec3D(coorPt->x(), coorPt->y(), coorPt->z(), newTagNum), _refX(coorPt->x()), _refY(coorPt->y()), _refZ(coorPt->z()), _isFixed(bfixed)
		, _DrawZ(coorPt->z())
	{

	}

	Point::~Point(void)
	{
	}
	bool Point::Check()
	{
		if (_refX>Constants::SysMaxTagNum && _refY > Constants::SysMaxTagNum && _refZ > Constants::SysMaxTagNum &&
			_refX < Constants::CoorEqualNum && _refY < Constants::CoorEqualNum && _refZ < Constants::CoorEqualNum)
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this, PointErrException::NumErr));
			return false;
		}
		else if (_refX < -100.0 || _refY < -100.0 || _refZ < -100.0)
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this, PointErrException::CoorErr));
			return false;
		}
		return true;
	}
	bool Point::CalcCoor(const PointMap* aPointSet)
	{
		if (isAbsoluteCoordinates()) ///第一种情况
		{
			this->setX(_refX);
			this->setY(_refY);
			this->setZ(_refZ);
			return true;
		}
		double realCoor = Constants::CoorEqualNum;
		if (_refX>Constants::CoorEqualNum || _refY > Constants::CoorEqualNum || _refZ > Constants::CoorEqualNum)
		{
			double xcoor = _refX;
			double ycoor = _refY;
			double zcoor = _refZ;
			if (_refX > Constants::CoorEqualNum)
			{
				int itempxx = int(_refX - Constants::CoorEqualNum);   //正确点1
				HandlePoint pNodeX = aPointSet->Find(itempxx);
				if (!pNodeX.isNull())
				{
					xcoor = pNodeX->x();
					realCoor = xcoor;
				}
			}
			if (_refY > Constants::CoorEqualNum)
			{
				int itempyy = int(_refY - Constants::CoorEqualNum);   //正确点1
				HandlePoint pNodeY = aPointSet->Find(itempyy);
				if (!pNodeY.isNull())
				{
					ycoor = pNodeY->y();
					realCoor = ycoor;
				}
			}
			if (_refZ > Constants::CoorEqualNum)
			{
				int itempzz = int(_refZ - Constants::CoorEqualNum);   //正确点1
				HandlePoint pNodeZ = aPointSet->Find(itempzz);
				if (!pNodeZ.isNull())
				{
					zcoor = pNodeZ->z();
					realCoor = zcoor;
				}
			}
			if (xcoor < Constants::CoorEqualNum && ycoor < Constants::CoorEqualNum && zcoor < Constants::CoorEqualNum)
			{//没有错误的时候才生成点
				this->setX(xcoor);
				this->setY(ycoor);
				this->setZ(zcoor);
				return true;
			}
		}
		if (_refX >= Constants::SysMaxTagNum && _refX < Constants::CoorEqualNum && _refY >= Constants::SysMaxTagNum && _refY < Constants::CoorEqualNum)  ////第二种情况,前两个大于100000
		{
			int itempxx = int(_refX - Constants::SysMaxTagNum);   //正确点1
			int itempyy = int(_refY - Constants::SysMaxTagNum);   //正确点2
			HandlePoint pNodeX = aPointSet->Find(itempxx);
			HandlePoint pNodeY = aPointSet->Find(itempyy);
			if (!pNodeX.isNull() && !pNodeY.isNull())
			{
				double dlenz = pNodeY->z() - pNodeX->z();
				if (dlenz<Constants::DISTANCE_EPSILON && dlenz>-Constants::DISTANCE_EPSILON)
				{
					ExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this, PointErrException::RatioErr));
					return false;
				}
				else
				{
					if (this->_refZ > Constants::SysMaxTagNum && realCoor > Constants::SysMaxTagNum)
						return false;
					else if (this->_refZ < Constants::SysMaxTagNum)
						realCoor = this->_refZ;

					double dtemp = (realCoor - pNodeX->z()) / (pNodeY->z() - pNodeX->z());
					double dtemp2 = pNodeX->x() + dtemp*(pNodeY->x() - pNodeX->x());
					double dtemp3 = pNodeX->y() + dtemp*(pNodeY->y() - pNodeX->y());
					this->setX(dtemp2);
					this->setY(dtemp3);
					this->setZ(realCoor);
					return true;
				}
			}
		}
		else if (_refX >= Constants::SysMaxTagNum && _refX < Constants::CoorEqualNum && _refZ >= Constants::SysMaxTagNum && _refZ < Constants::CoorEqualNum)  //第三种情况,头尾个大于100000
		{
			int itempxx = int(_refX - 10000);   //正确点1
			int itempyy = int(_refZ - 10000);   //正确点2
			HandlePoint pNodeX = aPointSet->Find(itempxx);
			HandlePoint pNodeY = aPointSet->Find(itempyy);
			if (!pNodeX.isNull() && !pNodeY.isNull())
			{
				double dleny = pNodeY->y() - pNodeX->y();
				if (dleny<Constants::DISTANCE_EPSILON && dleny>-Constants::DISTANCE_EPSILON)
				{
					ExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this, PointErrException::RatioErr));
					return false;
				}
				else
				{
					if (this->_refY > Constants::SysMaxTagNum && realCoor > Constants::SysMaxTagNum)
						return false;
					else if (this->_refY < Constants::SysMaxTagNum)
						realCoor = this->_refY;

					double dtemp = (realCoor - pNodeX->y()) / (pNodeY->y() - pNodeX->y());
					double dtemp2 = pNodeX->x() + dtemp*(pNodeY->x() - pNodeX->x());
					double dtemp3 = pNodeX->z() + dtemp*(pNodeY->z() - pNodeX->z());
					this->setX(dtemp2);
					this->setY(realCoor);
					this->setZ(dtemp3);
					return true;
				}
			}
		}
		else if (_refY >= Constants::SysMaxTagNum && _refY < Constants::CoorEqualNum && _refZ >= Constants::SysMaxTagNum && _refZ < Constants::CoorEqualNum)  
		{////第四种情况,最后两个大于100000
			int itempxx = int(_refY - Constants::SysMaxTagNum);   //正确点1
			int itempyy = int(_refZ - Constants::SysMaxTagNum);		//正确点2
			HandlePoint pNodeX = aPointSet->Find(itempxx);
			HandlePoint pNodeY = aPointSet->Find(itempyy);
			if (!pNodeX.isNull() && !pNodeY.isNull())   //两个都找到了
			{
				double dlenx = pNodeY->x() - pNodeX->x();
				if (dlenx<Constants::DISTANCE_EPSILON && dlenx>-Constants::DISTANCE_EPSILON)
				{
					ExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(this, PointErrException::RatioErr));
					return false;
				}
				else
				{
					if (this->_refX > Constants::SysMaxTagNum && realCoor > Constants::SysMaxTagNum)
						return false;
					else if (this->_refX < Constants::SysMaxTagNum)
						realCoor = this->_refX;

					double dtemp = (realCoor - pNodeX->x()) / (pNodeY->x() - pNodeX->x());
					double dtemp2 = pNodeX->y() + dtemp*(pNodeY->y() - pNodeX->y());
					double dtemp3 = pNodeX->z() + dtemp*(pNodeY->z() - pNodeX->z());
					this->setX(realCoor);
					this->setY(dtemp2);
					this->setZ(dtemp3);
					return true;
				}
			}
		}
		return false;
	}

	void Point::Renumbering(RenumberingList* list)
	{
		int NewTagNum = list->FindNumTag(_iTagNum);
		if (NewTagNum>-1)
			_iTagNum = NewTagNum;
		if (_refX >= Constants::SysMaxTagNum)
		{
			int xTagNum = int(_refX - Constants::SysMaxTagNum * int(_refX / Constants::SysMaxTagNum));
			NewTagNum = list->FindNumTag(xTagNum);
			if (NewTagNum > 0)
				_refX = int(_refX / Constants::SysMaxTagNum) * Constants::SysMaxTagNum + NewTagNum;
		}
		if (_refY >= Constants::SysMaxTagNum)
		{
			int yTagNum = int(_refY - Constants::SysMaxTagNum * int(_refY / Constants::SysMaxTagNum));
			NewTagNum = list->FindNumTag(yTagNum);
			if (NewTagNum > 0)
				_refY = int(_refY / Constants::SysMaxTagNum) * Constants::SysMaxTagNum + NewTagNum;
		}
		if (_refZ >= Constants::SysMaxTagNum)
		{
			int zTagNum = int(_refZ - Constants::SysMaxTagNum * int(_refZ / Constants::SysMaxTagNum));
			NewTagNum = list->FindNumTag(zTagNum);
			if (NewTagNum > 0)
				_refZ = int(_refZ / Constants::SysMaxTagNum) * Constants::SysMaxTagNum + NewTagNum;
		}
	}


	bool Point::IsReferTo(int tagNum)
	{
		if (_refX >= Constants::SysMaxTagNum)
		{
			int xTagNum = int(_refX - Constants::SysMaxTagNum);
			if (xTagNum == tagNum)
				return true;
		}
		if (_refY >= Constants::SysMaxTagNum)
		{
			int yTagNum = int(_refY - Constants::SysMaxTagNum);
			if (yTagNum == tagNum)
				return true;
		}
		if (_refZ >= Constants::SysMaxTagNum)
		{
			int yTagNum = int(_refZ - Constants::SysMaxTagNum);
			if (yTagNum == tagNum)
				return true;
		}
		return false;
	}
	


}