//#include "StdAfx.h"
#include "PlaneFunction.h"
#include "config.hpp"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	CPlaneFunction::CPlaneFunction(const CVec3D *pt1, const  CVec3D *pt2, const CVec3D *pt3)//三个点确定一个面
	{
		CalcPlanePara(pt1,pt2,pt3);
	}
	CPlaneFunction::CPlaneFunction(const CNode *pt1, const  CNode *pt2, const  CNode *pt3)//三个点确定一个面
	{
		CalcPlanePara(pt1,pt2,pt3);
	}
	CPlaneFunction::CPlaneFunction(double dA,double dB,double dC,double dD):
	_PlaneVec(dA,dB,dC),m_dD(dD) 
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

	CPlaneFunction::CPlaneFunction(const NodeArray* pNodes)
	{
		for (int iloop = 2; iloop < pNodes->size(); ++iloop)
		{
			if (CalcPlanePara(pNodes->at(0).GetPointee(), pNodes->at(1).GetPointee(), pNodes->at(iloop).GetPointee()))
				return;
		}
	}

	CPlaneFunction::~CPlaneFunction(void)
	{

	}
	bool CPlaneFunction::CalcPlanePara(const CVec3D *pPoint1, const  CVec3D *pPoint2, const  CVec3D *pPoint3)
	{//空间向量的值,Point1为基准点
		CVec3D _AxisX = pPoint2->Subtraction(pPoint1);
		CVec3D _AxisY = pPoint3->Subtraction(pPoint1);
		_PlaneVec = _AxisX.cross(_AxisY);
		if (_PlaneVec.norm() < Constants::SCALAR_EPSILON)
			return _IsAPlane = false;
		_PlaneVec.normalize();
		m_dD = -_PlaneVec.dot(*pPoint1);
		return _IsAPlane = true;
	}
	HdElemArray CPlaneFunction::FindElmInPlane(const ElemArray* pElmArray)  const//查找点集，在所在平面内的
	{
		HdElemArray elmCollect=new ElemArray();
		if (pElmArray == NULL)
			return elmCollect;
		
		for (int iloop = 0; iloop < pElmArray->size(); ++iloop)
		{
			HandleElement pElm = pElmArray->at(iloop);
			if (CheckCoplanarity(pElm->StartNode().GetPointee()))
				if (CheckCoplanarity(pElm->EndNode().GetPointee()))
					elmCollect->push_back(pElm);//两个点都在面上，表示该杆件在面上
		}
		return elmCollect;
	}

	HdElemArray CPlaneFunction::FindElmInPlaneEpsion(const ElemArray* pElmArray, double dEPSILON) const
	{
		HdElemArray elmRes = new ElemArray();
		if (pElmArray == NULL)
			return elmRes;

		for (int iloop = 0; iloop < pElmArray->size(); ++iloop)
		{
			HandleElement pElm = pElmArray->at(iloop);
			if (CheckCoplanarity(pElm->StartNode().GetPointee(), dEPSILON))
				if (CheckCoplanarity(pElm->EndNode().GetPointee(), dEPSILON))
					elmRes->push_back(pElm);//两个点都在面上，表示该杆件在面上
		}
		return elmRes;
	}
	HdNodeArray CPlaneFunction::FindNodeInPlane(const NodeArray* pNodeArray)  const
	{
		HdNodeArray nodeRes=new NodeArray();
		if (pNodeArray == NULL)
			return nodeRes;
		
		for (int iloop = 0; iloop < pNodeArray->size(); ++iloop)
		{
			HandleNode pNode = pNodeArray->at(iloop);
			if (CheckCoplanarity(pNode.GetPointee()))
				nodeRes->push_back(pNode);
		};
		return nodeRes;
	}
	CVec3D* CPlaneFunction::CrossPoint(const CVec3D *pStart, const CVec3D *pEnd)  const
	{
		CVec3D lineVec = pEnd->Subtraction(pStart);
		double dtemp=_PlaneVec.dot(lineVec);
		//表示直线与平面平行
		if(dtemp<Constants::SCALAR_EPSILON && dtemp>-Constants::SCALAR_EPSILON)
			return NULL;
		//下面计算
		double t=-(_PlaneVec.dot(*pStart)+m_dD)/dtemp;
		return new CVec3D(pStart->x()+t*lineVec.x(),pStart->y()+t*lineVec.y(),pStart->z()+t*lineVec.z());
	}

	bool CPlaneFunction::CheckCoplanarity(const  CPlaneFunction *rhs) const
	{
		if(!_IsAPlane || !rhs->_IsAPlane)
			return false;		
		//直接相同
		if(rhs->_PlaneVec==_PlaneVec && abs(m_dD-rhs->m_dD)<Constants::DISTANCE_EPSILON0_001)
			return true;
		//可能向量相反，取反操作
		CVec3D tmpPlaneVec=rhs->_PlaneVec;
		tmpPlaneVec.reverse_vector();
		double tmpD=-rhs->m_dD;
		if(tmpPlaneVec==_PlaneVec && abs(m_dD-tmpD)<Constants::DISTANCE_EPSILON0_001)
			return true;
		return false;
	}

	bool CPlaneFunction::CheckCoplanarity(const CPlaneFunction *rhs, double dEPSILON) const
	{
		if(!_IsAPlane || !rhs->_IsAPlane)
			return false;		
		CVec3D tmp1Vec=rhs->_PlaneVec.cross(_PlaneVec);
		double normal2=tmp1Vec.norm();
		if(tmp1Vec.norm()<dEPSILON && abs(abs(m_dD)-abs(rhs->m_dD))<dEPSILON)
			return true;
		return false;
	}
	const Matrix3d* CPlaneFunction::GetProjectionTransferMat() const
	{
		static Eigen::Matrix3d _TransMat;
		static Eigen::Matrix3d _TransMat2;
		CVec3D axial_A;
		CVec3D vecZ_A;
		CVec3D planeVec=_PlaneVec;		
		double absX=abs(planeVec.x());
		double absY=abs(planeVec.y());
		double absZ=abs(planeVec.z());
		_TransMat2.setZero();
		if(absY>=absX && absY>=absZ)
		{//确定主轴是Y
			vecZ_A.setY(1);
			axial_A=planeVec.cross(vecZ_A);
			axial_A.normalize();
			_TransMat2(0,0)=1;
			_TransMat2(1,2)=-1;
			_TransMat2(2,1)=-1;

		}
		else if(absX>=absY && absX>=absZ)
		{//确定主轴是X	
			vecZ_A.setX(1);
			axial_A=planeVec.cross(vecZ_A);
			axial_A.normalize();						
			_TransMat2(0,1)=1;
			_TransMat2(1,2)=1;
			_TransMat2(2,0)=1;
		}
		else
		{//确定主轴是Z
			vecZ_A.setZ(1);
			axial_A=planeVec.cross(vecZ_A);
			axial_A.normalize();	
			_TransMat2(0,0)=1;
			_TransMat2(1,1)=1;
			_TransMat2(2,2)=1;
		}
		planeVec.normalize();
		if(axial_A.norm()>Constants::SCALAR_EPSILON)
		{
			double angle1=acos(vecZ_A.dot(planeVec));		
			Eigen::AngleAxisd TansAA(angle1,Eigen::Vector3d(axial_A.x(),axial_A.y(),axial_A.z()));
			_TransMat=_TransMat2*TansAA.toRotationMatrix();
		}
		else
		{
			_TransMat=_TransMat2;
		}		
		return &_TransMat;
	}

	HdNodeArray CPlaneFunction::Outofplane(const NodeArray* pNodeArray)const
	{
		HdNodeArray nodeRes=new NodeArray();
		if (pNodeArray == NULL)
			return nodeRes;
		
		for (int iloop = 0; iloop < pNodeArray->size(); ++iloop)
		{
			HandleNode pNode = pNodeArray->at(iloop);
			if (!this->CheckCoplanarity(pNode.GetPointee()))
				nodeRes->push_back(pNode);
		}
		return nodeRes;
	}

	HdElemArray CPlaneFunction::Outofplane(const ElemArray* pElmArray)const
	{
		HdElemArray elmRes=new ElemArray();
		if (pElmArray == NULL)
			return elmRes;
		
		for (int iloop = 0; iloop < pElmArray->size(); ++iloop)
		{
			HandleElement pElm = pElmArray->at(iloop);
			HandleNode startNode=pElm->StartNode();
			HandleNode endNode=pElm->EndNode();
			if(!this->CheckCoplanarity(startNode.GetPointee()) || !this->CheckCoplanarity(endNode.GetPointee()))
				elmRes->push_back(pElm);
		}
		return elmRes;
	}

	const Matrix3d* CPlaneFunction::GetProjXOYTransferMat() const
	{
		static Eigen::Matrix3d _TransMat1;
		static Eigen::Matrix3d _TransMat2;
		CVec3D planeVec=this->GetVector();	//空间上的第三点
		_TransMat1.setZero();	
		double dDtemp=sqrt(1-planeVec.z()*planeVec.z());
		if (dDtemp<Constants::SCALAR_EPSILON)
		{
			if(planeVec.z()>Constants::SCALAR_EPSILON)
			{
				_TransMat1(0,2)=1;_TransMat1(1,1)=1;_TransMat1(2,0)=-1;}
			else
			{
				_TransMat1(0,2)=-1;_TransMat1(1,1)=1;_TransMat1(2,0)=1;
			}
		}
		else
		{
			_TransMat1(0,0)=planeVec.x();
			_TransMat1(0,1)=planeVec.y();
			_TransMat1(0,2)=planeVec.z();
			_TransMat1(1,0)=-planeVec.y()/dDtemp;
			_TransMat1(1,1)=planeVec.x()/dDtemp;
			_TransMat1(1,2)=0;
			_TransMat1(2,0)=-planeVec.x()*planeVec.z()/dDtemp;
			_TransMat1(2,1)=-planeVec.y()*planeVec.z()/dDtemp;
			_TransMat1(2,2)=dDtemp;
		}
		_TransMat2.setZero();	
		double absX=abs(planeVec.x());
		double absY=abs(planeVec.y());
		double absZ=abs(planeVec.z());
		if(absY>=absX && absY>=absZ)
		{
			_TransMat2(0,1)=1;
			_TransMat2(1,2)=1;
			_TransMat2(2,0)=1;
		}
		else
		{
			_TransMat2(0,2)=1;
			_TransMat2(1,1)=1;
			_TransMat2(2,0)=1;
		}
		_TransMat1.applyOnTheLeft(_TransMat2);
		return &_TransMat1;
	}

	const double CPlaneFunction::Coplanarity_EPSILON=0.009;

	CVec3D* LineSegmentHelper::CrossPoint(CVec3D *pStartPoint1,CVec3D *pEndPoint1,CVec3D *pStartPoint2,CVec3D *pEndPoint2)
	{
 		//是否是一直线
 		CVec3D vectorLine1=pEndPoint1->Subtraction(pStartPoint1);
 		double line1Length=vectorLine1.norm();
 		if(Constants::is_zero(line1Length))
 			return NULL;
 
 		CVec3D vectorLine2=pEndPoint2->Subtraction(pStartPoint2);
 		double line2Length=vectorLine2.norm();
 		if(Constants::is_zero(line2Length))
 			return NULL;
 
 		CVec3D vecU=pStartPoint1->Subtraction(pStartPoint2);
 		double lengthVecU=vecU.norm();
 		if(Constants::is_zero(lengthVecU))
 			return NULL;
 
 		//根据计算机图形学几何工具算法详解p298计算方法
 		double a=line1Length*line1Length;
 		double b=vectorLine1.dot(vectorLine2);
 		double c=line2Length*line2Length;
 		double d=vectorLine1.dot(vecU);
 		double e=vectorLine2.dot(vecU);
 		double f=lengthVecU*lengthVecU;
 		double det=a*c-b*b;
 
 		//两条直线平行
 		if(det<Constants::SCALAR_EPSILON && det>-Constants::SCALAR_EPSILON)
 			return NULL;		
 		//计算两条直线的距离
 		double s=(b*e-c*d)/det;
 
 		double t=(b*d-a*e)/det;
 
 		//计算两条直线的最短距离
 		double dMinLen=s*(a*s+b*t+2*d)+t*(b*s+c*t+2*e)+f;
 
 		//一个毫米
 		if(abs(dMinLen)<Constants::COINCIDENT2_EPSILON)
 			return new CVec3D((*pStartPoint1)+s*vectorLine1);
 		return NULL;

	}



	HandleNode LineSegmentHelper::SegmentSegmentCrossPoint(const HandleNode& pStartPoint1, const HandleNode& pEndPoint1, const HandleNode& pStartPoint2, const HandleNode& pEndPoint2)
	{
 		//排除法
 		CVec3D vectorLine1=pEndPoint1->Subtraction(pStartPoint1.GetPointee());
 		double line1Length=vectorLine1.norm();
 		if(Constants::is_zero(line1Length))
 			return NULL;
 
 		CVec3D vectorLine2=pEndPoint2->Subtraction(pStartPoint2.GetPointee());
 		double line2Length=vectorLine2.norm();
 		if(Constants::is_zero(line2Length))
 			return NULL;
 
 		CVec3D vecU=pStartPoint1->Subtraction(pStartPoint2.GetPointee());
 		double lengthVecU=vecU.norm();
 		if(Constants::is_zero(lengthVecU))
 			return NULL;
 		
 		//根据计算机图形学几何工具算法详解p298计算方法
 		double a=line1Length*line1Length;
 		double b=vectorLine1.dot(vectorLine2);
 		double c=line2Length*line2Length;
 		double d=vectorLine1.dot(vecU);
 		double e=vectorLine2.dot(vecU);
 		double det=a*c-b*b;
 
 		//两条直线平行
 		if(det<Constants::SCALAR_EPSILON && det>-Constants::SCALAR_EPSILON)
 			return NULL;
 
 		double f=lengthVecU*lengthVecU;
 
 		//计算两条直线的距离
 		double s=(b*e-c*d)/det;
 		double t=(b*d-a*e)/det;
 
 		//线段没有交点
 		if(s<-0.0002 || s>1.0002 || t>0.0002 || t<-1.0002)
 			return NULL;
 
 		//计算两条直线的最短距离
 		double MinLen=s*(a*s+b*t+2*d)+t*(b*s+c*t+2*e)+f;
 		if(abs(MinLen)>Constants::COINCIDENT2_EPSILON)
 			return NULL;
 
 		//有焦点焦点在端点
 		if(Constants::is_zero(s))
 			return pStartPoint1;
 
 		if(Constants::is_zero(s-1))
 			return pEndPoint1;
 
 		if(Constants::is_zero(t))
 			return pStartPoint2;
 
 		if(Constants::is_zero(t+1))
 			return pEndPoint2;		
 
 		CVec3D result=(*pStartPoint1)+s*vectorLine1;
 
 		if(result.distanceTo(pStartPoint1.GetPointee())<Constants::COINCIDENT_EPSILON)
 			return pStartPoint1;
 
 		if(result.distanceTo(pEndPoint1.GetPointee())<Constants::COINCIDENT_EPSILON)
 			return pEndPoint1;
 
 		if(result.distanceTo(pStartPoint2.GetPointee())<Constants::COINCIDENT_EPSILON)
 			return pStartPoint2;
 
 		if(result.distanceTo(pEndPoint2.GetPointee())<Constants::COINCIDENT_EPSILON)
 			return pEndPoint2;
 
 		return new CNode(0,result.x(),result.y(),result.z());
	}


	double LineSegmentHelper::LineAngle(const CNode *pStartPoint1,const CNode *pEndPoint1,const CNode *pStartPoint2,const CNode *pEndPoint2)
	{
		CVec3D elm1Vec=pEndPoint1->Subtraction(pStartPoint1);
		elm1Vec.normalize();
		CVec3D elm2Vec=pEndPoint2->Subtraction(pStartPoint2);
		elm2Vec.normalize();
		double angleLineLB=abs(elm1Vec.dot(elm2Vec));
		return acos(angleLineLB)*180/Constants::PI;
	}
}