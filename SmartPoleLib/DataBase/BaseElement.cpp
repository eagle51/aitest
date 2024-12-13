//#include "StdAfx.h"
#include "BaseElement.h"
#include "ExceptionMessage.h"
//静态成员定义性说明：
namespace SmartPoleCore
{	
	double BaseElement::_ZEpsion=1E-10;
	int BaseElement::_MoveValue = 100000;

	BaseElement::BaseElement(const HandleNode &startPoint, const HandleNode &endPoint, const HandleMember &belongToMenb, const HandleProfile& pProfile, int startPointNum, int endPointNum)
		:_pNodeJ1(startPoint), _pNodeJ2(endPoint), m_pBelongToMenb(belongToMenb), _CurProfile(pProfile), _Id(0), _J1PointNum(startPointNum), _J2PointNum(endPointNum)
	{//起点的x比终点的x小，x相同的情况下，z小，z相同的情况下y小
		linVec = endPoint->Subtraction(startPoint.GetPointee());
		m_dLength = linVec.norm();
		if (linVec.x()>Constants::DISTANCE_EPSILON)
		{
			_pNodeJ1 = startPoint;
			_pNodeJ2 = endPoint;
		}
		else if (linVec.x() < -Constants::DISTANCE_EPSILON)
		{
			_pNodeJ1 = endPoint;
			_pNodeJ2 = startPoint;
			_J1PointNum = endPointNum;
			_J2PointNum = startPointNum;
		}
		else if (linVec.z()>Constants::DISTANCE_EPSILON)
		{
			_pNodeJ1 = startPoint;
			_pNodeJ2 = endPoint;
		}
		else if (linVec.z() < -Constants::DISTANCE_EPSILON)
		{
			_pNodeJ1 = endPoint;
			_pNodeJ2 = startPoint;
			_J1PointNum = endPointNum;
			_J2PointNum = startPointNum;
		}
		else if (linVec.y() < -Constants::DISTANCE_EPSILON)
		{
			_pNodeJ1 = endPoint;
			_pNodeJ2 = startPoint;
			_J1PointNum = endPointNum;
			_J2PointNum = startPointNum;
		}
		//_CurMatriel = m_pBelongToMenb->getMatProp();

	};
	BaseElement::~BaseElement(void)
	{

	}

	bool BaseElement::CheckOnLine( const HandleNode& pNode )
	{
		double minZ=_pNodeJ1->z();
		double MaxZ=_pNodeJ2->z();
		if(MaxZ<minZ)
			std::swap(minZ,MaxZ);
		if(pNode->z()>MaxZ || pNode->z()<minZ)
			return false;

		double minX=_pNodeJ1->x();
		double MaxX=_pNodeJ2->x();
		if(MaxX<minX)
			std::swap(minX,MaxX);

		if(pNode->x()>MaxX || pNode->x()<minX)
			return false;

		double minY=_pNodeJ1->y();
		double MaxY=_pNodeJ2->y();
		if(MaxY<minY)
			std::swap(minY,MaxY);

		if(pNode->y()>MaxY || pNode->y()<minY)
			return false;

		CVec3D lineVec=_pNodeJ1->Subtraction(_pNodeJ2.GetPointee());
		lineVec.normalize();

		CVec3D DirVec=pNode->Subtraction(_pNodeJ2.GetPointee());
		if(DirVec.norm()<Constants::DISTANCE_EPSILON)
			return true;		

		CVec3D DirVec2=pNode->Subtraction(_pNodeJ1.GetPointee());
		if (DirVec2.norm()<Constants::DISTANCE_EPSILON)
			return true;

		DirVec.normalize();
		CVec3D CrossVec=lineVec.Cross(DirVec);
		if (CrossVec.norm()<Constants::DISTANCE_EPSILON)
			return true;
		return false;
	}

}