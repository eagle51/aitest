#pragma once
#include <complex>
#include "FloatVector.h"
#include "ElemArray.h"
#include "MemberArray.h"

namespace SmartTower
{
	class InnerUtility
	{
	public:
		//求两个数的最大公约数
		static int GCD(int m, int n);
		static HandleNode FindCoorEqualNode( std::map<int,HandleNode>* tmpMapNodes,const HandleNode& pNewNode);
		static FloatVector EigenVecXToFloatVec(const VectorXd& vec); 
		static double getVectorAngle(CVec3D* vec2DX0Y);
	};
}