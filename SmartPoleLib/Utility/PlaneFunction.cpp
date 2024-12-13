//#include "StdAfx.h"
#include "PlaneFunction.h"
//#include "config.hpp"
namespace SmartPoleCore
{
	CPlaneFunction::CPlaneFunction(CVec3D *Point1,CVec3D *Point2,CVec3D *Point3)//三个点确定一个面
	{
		CalcPlanePara(Point1,Point2,Point3);
	}
	CPlaneFunction::CPlaneFunction(double A,double B,double C,double D)
		:_PlaneVec(A,B,C),m_dD(D)
	{
		double normald=_PlaneVec.norm();
		if(normald<Constants::SCALAR_EPSILON) 
		{
			_IsAPlane=false;
			return;
		}
		else 
			_IsAPlane=true;
		m_dD/=normald;
		_PlaneVec.normalize();
	};//构造函数

	CPlaneFunction::~CPlaneFunction( void )
	{

	}
	bool CPlaneFunction::CalcPlanePara(CVec3D *Point1,CVec3D *Point2,CVec3D *Point3)
	{//空间向量的值
		CVec3D _AxisX(*Point2-*Point1);
		CVec3D _AxisY(*Point3-*Point1);
		_PlaneVec=_AxisX.Cross(_AxisY);
		if(_PlaneVec.norm()<Constants::SCALAR_EPSILON) 
			return _IsAPlane=false;
		_PlaneVec.normalize();
		m_dD=-_PlaneVec.Dot(*Point1);
		return _IsAPlane=true;
	}
	
	
	CVec3D* CPlaneFunction::CrossPoint(CVec3D *LineStart,CVec3D *LineEnd)  const
	{
		CVec3D LineVec=LineEnd->Subtraction(LineStart);
		double dtemp=_PlaneVec.Dot(LineVec);
		if(dtemp<Constants::SCALAR_EPSILON && dtemp>-Constants::SCALAR_EPSILON)
			return NULL;//表示直线与平面平行
		//下面计算
		double t=-(_PlaneVec.Dot(*LineStart)+m_dD)/dtemp;
		return new CVec3D(LineStart->x()+t*LineVec.x(),LineStart->y()+t*LineVec.y(),LineStart->z()+t*LineVec.z());
	}

	bool CPlaneFunction::CheckCoplanarity( CPlaneFunction *rhs ) const
	{
		if(!_IsAPlane || !rhs->_IsAPlane)
			return false;		
		//直接相同
		if(rhs->_PlaneVec==_PlaneVec && abs(m_dD-rhs->m_dD)<Constants::DISTANCE_EPSILON)
			return true;
		//可能向量相反，取反操作
		CVec3D tmpPlaneVec=rhs->_PlaneVec;
		tmpPlaneVec.Reverse_Vector();
		double tmpD=-rhs->m_dD;
		if(tmpPlaneVec==_PlaneVec && abs(m_dD-tmpD)<Constants::DISTANCE_EPSILON)
			return true;
		return false;
	}

	bool CPlaneFunction::CheckCoplanarity( CPlaneFunction *rhs,double dEPSILON ) const
	{
		if(!_IsAPlane || !rhs->_IsAPlane)
			return false;		
		CVec3D tmp1Vec=rhs->_PlaneVec.Cross(_PlaneVec);
		double normal2=tmp1Vec.norm();
		if(tmp1Vec.norm()<dEPSILON && abs(abs(m_dD)-abs(rhs->m_dD))<dEPSILON)
			return true;
		return false;
	}

	const double CPlaneFunction::Coplanarity_EPSILON=0.001;

	CVec3D* LineSegmentHelper::CrossPoint(CVec3D *StartPoint1,CVec3D *EndPoint1,CVec3D *StartPoint2,CVec3D *EndPoint2)
	{
		//是否是一直线
		CVec3D VectorLine1=EndPoint1->Subtraction(StartPoint1);
		double Line1Length=VectorLine1.norm();
		if(Constants::is_zero(Line1Length))
			return NULL;

		CVec3D VectorLine2=EndPoint2->Subtraction(StartPoint2);
		double Line2Length=VectorLine2.norm();
		if(Constants::is_zero(Line2Length))
			return NULL;

		CVec3D VecU=StartPoint1->Subtraction(StartPoint2);
		double LengthVecU=VecU.norm();
		if(Constants::is_zero(LengthVecU))
			return NULL;

		//根据计算机图形学几何工具算法详解p298计算方法
		double a=Line1Length*Line1Length;
		double b=VectorLine1.Dot(VectorLine2);
		double c=Line2Length*Line2Length;
		double d=VectorLine1.Dot(VecU);
		double e=VectorLine2.Dot(VecU);
		double f=LengthVecU*LengthVecU;
		double det=a*c-b*b;

		//两条直线平行
		if(det<Constants::SCALAR_EPSILON && det>-Constants::SCALAR_EPSILON)
			return NULL;		
		//计算两条直线的距离
		double s=(b*e-c*d)/det;

		double t=(b*d-a*e)/det;

		//计算两条直线的最短距离
		double MinLen=s*(a*s+b*t+2*d)+t*(b*s+c*t+2*e)+f;

		//一个毫米
		if (abs(MinLen)<Constants::COINCIDENT_EPSILON)
			return new CVec3D((*StartPoint1)+s*VectorLine1);
 		return NULL;
	}



	CVec3D* LineSegmentHelper::SegmentSegmentCrossPoint(CVec3D* StartPoint1,CVec3D* EndPoint1,CVec3D* StartPoint2,CVec3D* EndPoint2)
	{
		//排除法
		CVec3D VectorLine1=EndPoint1->Subtraction(StartPoint1);
		double Line1Length=VectorLine1.norm();
		if(Constants::is_zero(Line1Length))
			return NULL;

		CVec3D VectorLine2=EndPoint2->Subtraction(StartPoint2);
		double Line2Length=VectorLine2.norm();
		if (Constants::is_zero(Line2Length))
			return NULL;

		CVec3D VecU=StartPoint1->Subtraction(StartPoint2);
		double LengthVecU=VecU.norm();
		if (Constants::is_zero(LengthVecU))
			return NULL;
		
		//根据计算机图形学几何工具算法详解p298计算方法
		double a=Line1Length*Line1Length;
		double b=VectorLine1.Dot(VectorLine2);
		double c=Line2Length*Line2Length;
		double d=VectorLine1.Dot(VecU);
		double e=VectorLine2.Dot(VecU);
		double det=a*c-b*b;

		//两条直线平行
		if(det<Constants::SCALAR_EPSILON && det>-Constants::SCALAR_EPSILON)
			return NULL;

		double f=LengthVecU*LengthVecU;

		//计算两条直线的距离
		double s=(b*e-c*d)/det;
		double t=(b*d-a*e)/det;

		//线段没有交点
		if(s<-0.0002 || s>1.0002 || t>0.0002 || t<-1.0002)
			return NULL;

		//计算两条直线的最短距离
		double MinLen=s*(a*s+b*t+2*d)+t*(b*s+c*t+2*e)+f;
		if (abs(MinLen)>Constants::COINCIDENT_EPSILON)
			return NULL;

		//有焦点焦点在端点
		if(Constants::is_zero(s))
			return StartPoint1;

		if(Constants::is_zero(s-1))
			return EndPoint1;

		if(Constants::is_zero(t))
			return StartPoint2;

		if(Constants::is_zero(t+1))
			return EndPoint2;		

		VectorLine1.ScaleRatio(s);
		CVec3D result = StartPoint1->Addition(&VectorLine1);

		if(result.distanceTo(StartPoint1)<Constants::COINCIDENT_EPSILON)
			return StartPoint1;

		if(result.distanceTo(EndPoint1)<Constants::COINCIDENT_EPSILON)
			return EndPoint1;

		if(result.distanceTo(StartPoint2)<Constants::COINCIDENT_EPSILON)
			return StartPoint2;

		if(result.distanceTo(EndPoint2)<Constants::COINCIDENT_EPSILON)
			return EndPoint2;

		return new CVec3D(result.x(), result.y(), result.z());
	}

	double LineSegmentHelper::LineAngle(const CNode *StartPoint1,const CNode *EndPoint1,const CNode *StartPoint2,const CNode *EndPoint2)
	{
		CVec3D elm1Vec=EndPoint1->Subtraction(StartPoint1);
		elm1Vec.normalize();
		CVec3D elm2Vec=EndPoint2->Subtraction(StartPoint2);
		elm2Vec.normalize();
		double angleLineLB=abs(elm1Vec.Dot(elm2Vec));
		return acos(angleLineLB)*180/Constants::PI;
	}

	SmartPoleCore::HandleNode LineSegmentHelper::createNode(int nodeNum, const CVec3D* pStartPoint, const CVec3D* pEndPoint, double distanceToStart, double totalineLen)
	{
		if (totalineLen <= Constants::DISTANCE_EPSILON)
			return NULL;
		double dtemp = distanceToStart / totalineLen;
		double dtemp1 = pStartPoint->x() + dtemp*(pEndPoint->x() - pStartPoint->x());
		double dtemp2 = pStartPoint->y() + dtemp*(pEndPoint->y() - pStartPoint->y());
		double dtemp3 = pStartPoint->z() + dtemp*(pEndPoint->z() - pStartPoint->z());
		return new CNode(nodeNum, dtemp1, dtemp2, dtemp3);
	}

}