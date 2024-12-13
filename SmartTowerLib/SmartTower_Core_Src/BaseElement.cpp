//#include "StdAfx.h"
#include "BaseElement.h"
#include "ExceptionMessage.h"
#include "config.hpp"
//静态成员定义性说明：
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////

namespace SmartTower
{	
	double CBaseElement::_ZEpsion=1E-10;
	long long CBaseElement::_MoveValue=100000000;
	CBaseElement::CBaseElement(const HandleNode &pStartPoint,const HandleNode &pEndPoint,const HandleMemberInf &pBelongToMenb):
		  m_pBelongToMenb(pBelongToMenb)
	{
		//保持大号的在前面
		if(pStartPoint->TagNum()<pEndPoint->TagNum())
		{
			m_piNodeJ1=pStartPoint;
			m_piNodeJ2=pEndPoint;			
		}
		else
		{
			m_piNodeJ1=pEndPoint;
			m_piNodeJ2=pStartPoint;
		};		
		_ELmkey=CalcElmKey(m_piNodeJ1->TagNum(),m_piNodeJ2->TagNum());
		this->CalcLength();
	};
	CBaseElement::~CBaseElement(void)
	{
	}

	void CBaseElement::CalcLength()
	{
		//计算杆件的长度
		c[0]=m_piNodeJ2->x()-m_piNodeJ1->x();
		c[1]=m_piNodeJ2->y()-m_piNodeJ1->y();
		c[2]=m_piNodeJ2->z()-m_piNodeJ1->z();
		m_dLength=sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]);
		if(m_dLength<Constants::DISTANCE_EPSILON)
		{
			CExceptionMessageRegister::Instance()->SendErrObject(new MenberException(m_pBelongToMenb,MenberException::ZerosLength));
			m_dLength=0.001;//避免除0
		}
		//用于查找		
		c[0]/=m_dLength;
		c[1]/=m_dLength;
		c[2]/=m_dLength;
	}
	bool CBaseElement::CheckOnLine( const HandleNode& pNode )
	{
		double minZ=m_piNodeJ1->z();
		double maxZ=m_piNodeJ2->z();
		if(maxZ<minZ)
			std::swap(minZ,maxZ);
		if(pNode->z()>maxZ || pNode->z()<minZ)
			return false;

		double minX=m_piNodeJ1->x();
		double maxX=m_piNodeJ2->x();
		if(maxX<minX)
			std::swap(minX,maxX);

		if(pNode->x()>maxX || pNode->x()<minX)
			return false;

		double minY=m_piNodeJ1->y();
		double maxY=m_piNodeJ2->y();
		if(maxY<minY)
			std::swap(minY,maxY);

		if(pNode->y()>maxY || pNode->y()<minY)
			return false;

		CVec3D lineVec=m_piNodeJ1->Subtraction(m_piNodeJ2.GetPointee());
		lineVec.normalize();

		CVec3D dirVec=pNode->Subtraction(m_piNodeJ2.GetPointee());
		if(dirVec.norm()<Constants::DISTANCE_EPSILON0_001)
			return true;		

		CVec3D ddirVec2=pNode->Subtraction(m_piNodeJ1.GetPointee());
		if(ddirVec2.norm()<Constants::DISTANCE_EPSILON0_001)
			return true;

		dirVec.normalize();
		CVec3D crossVec=lineVec.cross(dirVec);
		if (crossVec.norm()<Constants::DISTANCE_EPSILON)
			return true;
		return false;
	}

	bool CBaseElement::isColinear(const CBaseElement* pLine) 
	{
		CVec3D thisDir(c[0], c[1], c[2]);
		CVec3D otherDir(pLine->c[0], pLine->c[1], pLine->c[2]);
		CVec3D crossVec = thisDir.cross(otherDir);
		if (crossVec.norm() > Constants::DISTANCE_EPSILON)
			return false;
		CVec3D dir2=pLine->m_piNodeJ1->Subtraction(m_piNodeJ1.GetPointee());
		CVec3D crossVec2 = thisDir.cross(dir2);
		return crossVec.norm() < Constants::DISTANCE_EPSILON;
	}
}