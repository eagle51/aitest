#pragma once
#include "Node.h"
#include "Point.h"
namespace SmartPoleCore
{
	class CPlaneFunction:public SharedObject
	{
	private:
		//Ax+By+Cz+D=0；
		CVec3D _PlaneVec;
		double m_dD;
		bool _IsAPlane;
		static const double Coplanarity_EPSILON;
	public:
		CPlaneFunction(CVec3D *Point1,CVec3D *Point2,CVec3D *Point3);//三个点确定一个面
		CPlaneFunction(double A=0,double B=0,double C=0,double D=0);
		//计算面的参数,如果是共线返回ture，否则返回false。
		bool CalcPlanePara(CVec3D *Point1,CVec3D *Point2,CVec3D *Point3);		
		//三点是否形成一个面，如果不是一个平面返回false
		bool isPlane() const
		{
			return _IsAPlane;
		};
		//获取向量
		CVec3D GetVector() const
		{
			return _PlaneVec;
		};
		//获取截距离
		double getD() const
		{
			return m_dD;
		}
		double DistanceTo(CVec3D *Point)
		{
			return abs(_PlaneVec.x()*Point->x()+_PlaneVec.y()*Point->y()+_PlaneVec.z()*Point->z()+m_dD); 
		}
		//判断是否共面
		bool CheckCoplanarity(CVec3D *Point, double dEPSILON = CPlaneFunction::Coplanarity_EPSILON) const
		{//
			return abs(_PlaneVec.x()*Point->x()+_PlaneVec.y()*Point->y()+_PlaneVec.z()*Point->z()+m_dD)<dEPSILON; 
		}
		//判断是否共面
		bool CheckCoplanarity(CPlaneFunction *rhs) const;
		//判断是否共面
		bool CheckCoplanarity(CPlaneFunction *rhs,double dEPSILON) const;
		//求线面交点
		CVec3D* CrossPoint(CVec3D *LineStart,CVec3D *LineEnd)  const;
		 ~CPlaneFunction(void);
	};
	typedef SharedHandle<CPlaneFunction> HandlePlaneFunc;
	
	class LineSegmentHelper
	{	
	private:
		static bool Equal(double f1,double f2)
		{
			return (abs(f1 - f2)<Constants::SCALAR_EPSILON);
		}
		static bool CoordLessCompare(const CNode* p1,const CNode* p2)
		{
			if( (p1->x() + Constants::SCALAR_EPSILON)< p2->x())
				return false;
			else if( p1->x() > (p2->x() + Constants::SCALAR_EPSILON))
				return true;
			return p1->y() > p2->y();
		}
		static double CrossProduct2D(const CVec3D& p1,const CVec3D& p2)
		{
			return (p1.x() * p2.y() - p1.y() * p2.x());
		}
	public:
		static CVec3D* CrossPoint(CVec3D *StartPoint1,CVec3D *EndPoint1,CVec3D *StartPoint2,CVec3D *EndPoint2);
		//两条线段内部的交点
		static CVec3D* SegmentSegmentCrossPoint(CVec3D* StartPoint1,CVec3D* EndPoint1,CVec3D* StartPoint2,CVec3D* EndPoint2);
		static double LineAngle(const CNode *StartPoint1,const CNode *EndPoint1,const CNode *StartPoint2,const CNode *EndPoint2);
		static HandleNode createNode(int nodeNum, const CVec3D* pStartPoint, const CVec3D* pEndPoint, double distanceToStart, double totalineLen);
	};

}