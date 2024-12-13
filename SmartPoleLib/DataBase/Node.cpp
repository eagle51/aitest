#include "Node.h"
//#include "config.hpp"
namespace SmartPoleCore
{

	CNode::CNode(int iNum /*= 0*/, double xcoor /*= 0*/, double ycoor /*= 0*/, double zcoor /*= 0*/, bool isFixed/*=false*/)
		: CVec3D(xcoor, ycoor, zcoor, iNum), _isFixed(isFixed), _fatherVec(0)
	{

	}

	CNode::CNode(Point* vec3d, int newTagNum)
		: CVec3D(vec3d->x(), vec3d->y(), vec3d->z(), newTagNum), _isFixed(vec3d->isFixed()), _fatherVec(vec3d)
	{

	}

}