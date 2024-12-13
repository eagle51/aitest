//#include "StdAfx.h"
#include <iomanip>
#include "InnerUtility.h"
#include "ExceptionMessage.h"
#include "TowerException.h"
#include "TowerModelIO.h"


namespace SmartTower
{


	SmartTower::HandleNode InnerUtility::FindCoorEqualNode( std::map<int,HandleNode>* tmpMapNodes,const HandleNode& pNewNode )
	{
		for (std::map<int,HandleNode>::iterator Iter=tmpMapNodes->begin();Iter!=tmpMapNodes->end();++Iter)
		{
			HandleNode pNode=Iter->second;
			if(pNode->CoordEqual(pNewNode.GetPointee()))
				return pNode;
		}
		return NULL;
	}

	SmartTower::FloatVector InnerUtility::EigenVecXToFloatVec( const VectorXd& vec )
	{
		FloatVector result;
		for(int iloop=0;iloop<vec.rows();++iloop)
			result.push_back(vec(iloop));
		return result;
	}

	int InnerUtility::GCD( int m,int n )
	{
		if(m<n)
			std::swap(m,n);
		if(n==0)
			return 0;
		int d;
		while (m>0)
		{
			d=m;
			m=n%d;
			n=d;
		}
		return n;
	}

	double InnerUtility::getVectorAngle( CVec3D* vec2DX0Y )
	{
		vec2DX0Y->normalize();
		double angleupLine=acos(abs(vec2DX0Y->x()))*180/Constants::PI;//角度还要判断顺时
		if(vec2DX0Y->x()>=-Constants::SCALAR_EPSILON && vec2DX0Y->y()>=-Constants::SCALAR_EPSILON)
			return angleupLine;
		else if(vec2DX0Y->x()<0 && vec2DX0Y->y()>=-Constants::SCALAR_EPSILON)
			angleupLine=180-angleupLine;
		else if(vec2DX0Y->x()<0 && vec2DX0Y->y()<0) 
			angleupLine+=180;
		else if(vec2DX0Y->x()>=-Constants::SCALAR_EPSILON && vec2DX0Y->y()<0)
			angleupLine=360-angleupLine;
		return angleupLine;
	}

}