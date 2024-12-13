#pragma once
#include "Node.h"
#include "ElemArray.h"
#include "NodeArray.h"
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月29日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	class CPlaneFunction:public SharedObject
	{
	private:
		//Ax+By+Cz+D=0；
		CVec3D _PlaneVec;
		double m_dD;
		bool _IsAPlane;
		static const double Coplanarity_EPSILON;
	public:
		CPlaneFunction(const CVec3D *pPoint1, const CVec3D *pPoint2, const CVec3D *pPoint3);//三个点确定一个面
		CPlaneFunction(const CNode *pPoint1, const CNode *pPoint2, const CNode *pPoint3);
		CPlaneFunction(double dA=0,double dB=0,double dC=0,double dD=0);
		CPlaneFunction(const NodeArray* pNodes);
		//计算面的参数,如果是共线返回ture，否则返回false。
		bool CalcPlanePara(const CVec3D *pPoint1, const CVec3D *pPoint2, const CVec3D *pPoint3);
		//三点是否形成一个面，如果不是一个平面返回false
		bool isPlane() const
		{
			return _IsAPlane;
		};
		const CVec3D& GetVector() const
		{
			return _PlaneVec;
		};//获取向量
		double getD() const
		{
			return m_dD;
		}
		double DistanceTo(const CNode *pPoint)
		{
			return abs(_PlaneVec.x()*pPoint->x()+_PlaneVec.y()*pPoint->y()+_PlaneVec.z()*pPoint->z()+m_dD); 
		}
		bool CheckCoplanarity(const CVec3D *pPoint) const
		{//
			return abs(_PlaneVec.x()*pPoint->x()+_PlaneVec.y()*pPoint->y()+_PlaneVec.z()*pPoint->z()+m_dD)<CPlaneFunction::Coplanarity_EPSILON; 
		}
		bool CheckCoplanarity(const CNode *pPoint) const
		{//
			return abs(_PlaneVec.x()*pPoint->x()+_PlaneVec.y()*pPoint->y()+_PlaneVec.z()*pPoint->z()+m_dD)<CPlaneFunction::Coplanarity_EPSILON; 
		}
		bool CheckCoplanarity(const CNode *pPoint, double dEPSILON) const
		{//
			return abs(_PlaneVec.x()*pPoint->x()+_PlaneVec.y()*pPoint->y()+_PlaneVec.z()*pPoint->z()+m_dD)<dEPSILON; 
		}
		bool CheckCoplanarity(const CPlaneFunction *rhs) const;

		bool CheckCoplanarity(const CPlaneFunction *rhs, double dEPSILON) const;

		CVec3D* CrossPoint(const CVec3D *pLineStart, const CVec3D *pLineEnd)  const;
		//获取投影变换矩阵，该变换的杆件长度是不变的
		const Matrix3d* GetProjectionTransferMat() const;
		const Matrix3d* GetProjXOYTransferMat() const;
		 ~CPlaneFunction(void);
		HdElemArray FindElmInPlane(const ElemArray* pElmArray) const;//查找点集，在所在平面内的
		HdNodeArray FindNodeInPlane(const NodeArray* pNodeArray) const;
		HdNodeArray Outofplane(const NodeArray* pNodeArray)const;
		HdElemArray Outofplane(const ElemArray* pElmArray)const;
		HdElemArray FindElmInPlaneEpsion(const ElemArray* pElmArray, double dEPSILON) const;//查找点集，在所在平面内的
	};
	typedef StHandle<CPlaneFunction> HandlePlaneFunc;
	
	class LineSegmentHelper
	{	
	public:
        // StHandle<CVec3D>
		static CVec3D* CrossPoint(CVec3D *StartPoint1,CVec3D *EndPoint1,CVec3D *StartPoint2,CVec3D *EndPoint2);
		//两条线段内部的交点
		static HandleNode SegmentSegmentCrossPoint(const HandleNode& StartPoint1, const HandleNode& EndPoint1, const HandleNode& StartPoint2, const HandleNode& EndPoint2);
		static double LineAngle(const CNode *pStartPoint1,const CNode *pEndPoint1,const CNode *pStartPoint2,const CNode *pEndPoint2);
		 
	};
}