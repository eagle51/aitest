// 文件名： OsgCoordSystem.cpp
// 作者：田腾；日期：2015/7/30
// 描述：
//////////////////////////////////////////////////////////////////////////
// 历史记录
/////////////////////////////////////////////////////////////////////////

#include "OsgCoordSystem.h"
// #include "OsgMatrixUtil.h"
// #include "OsgConvertUtil.h"
// #include "OsgMathUtil.h"

COsgCoordSystem::COsgCoordSystem(void)
{
}

COsgCoordSystem::COsgCoordSystem(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &yAxis)
{
	Set(origin, xAxis, yAxis);
}

COsgCoordSystem::COsgCoordSystem(const osg::Vec3d & origin, const osg::Vec3d & normal)
{
	Set(origin, normal);
}

COsgCoordSystem::COsgCoordSystem(const osg::Vec2d &origin, const osg::Vec2d &xAxis)
{
	Set(origin, xAxis);
}

COsgCoordSystem::COsgCoordSystem(const osg::Vec2d &origin)
{
	Set(origin);
}

COsgCoordSystem::COsgCoordSystem(const osg::Vec3d &origin)
{
	Set(origin);
}

COsgCoordSystem::COsgCoordSystem(const osg::Matrix &mat)
{
	Set(mat);
}

void COsgCoordSystem::Set(const osg::Vec2d &origin)
{
	//Set(COsgConvertUtil::ToVec3(origin));
}

void COsgCoordSystem::Set(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &yAxis)
{
	//set(COsgMatrixUtil::AlignCoordSys(origin, xAxis, yAxis));
}

void COsgCoordSystem::Set(const osg::Vec3d &origin, const osg::Vec3d &normal)
{
	osg::Vec3d xAxis;
	ArbitraryAxisAlgorithm(normal, xAxis);
	SetByXAndZAxis(origin, xAxis, normal);
}

void COsgCoordSystem::Set(const osg::Vec2d &origin, const osg::Vec2d &xAxis)
{
	//Set(COsgConvertUtil::ToVec3(origin), COsgConvertUtil::ToVec3(xAxis), ZAxis ^ COsgConvertUtil::ToVec3(xAxis));
}

void COsgCoordSystem::Set(const osg::Vec3d &origin)
{
	setTrans(origin);
}

void COsgCoordSystem::Set(const osg::Matrix &mat)
{
	set(mat);
}

void COsgCoordSystem::SetBy3P(const osg::Vec3d &origin, const osg::Vec3d &xP, const osg::Vec3d &yP)
{
	Set(origin, xP - origin, yP - origin);
}

void COsgCoordSystem::SetByYAndZAxis(const osg::Vec3d &origin, const osg::Vec3d &yAxis, const osg::Vec3d &zAxis)
{
	Set(origin, yAxis ^ zAxis, yAxis);
}

void COsgCoordSystem::SetByXAndZAxis(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &zAxis)
{
	Set(origin, xAxis, zAxis ^ xAxis);
}

osg::Matrixd COsgCoordSystem::GetTranslateMatrix() const
{
	return *this;
}

osg::Matrixd COsgCoordSystem::GetInverse() const
{
	return osg::Matrix::inverse(*this);
}

osg::Vec3d COsgCoordSystem::kOrigin() const
{
#ifdef _DEBUG
	osg::Vec3d testOrigin = getTrans();
#endif

	return getTrans();
}

osg::Vec3d COsgCoordSystem::kXAxis() const
{
	return getRotate() * XAxis;
}

osg::Vec3d COsgCoordSystem::kYAxis() const
{
	return getRotate() * YAxis;
}

osg::Vec3d COsgCoordSystem::kZAxis() const
{
	return getRotate() * ZAxis;
}

osg::Vec2d COsgCoordSystem::ProjectionPoint(const osg::Vec3d &point) const
{
	return ProjectionPoint(GetInverse(), point);
}

osg::Vec2d COsgCoordSystem::ProjectionPoint(const osg::Matrixd &mat, const osg::Vec3d & point)
{
	osg::Vec3d p = point * mat;
	osg::Vec2d p2(p.x(), p.y());
	return p2;
}

std::vector<osg::Vec2d> COsgCoordSystem::ProjectionPoint(const osg::ref_ptr<osg::Vec3dArray> &verts) const
{
	std::vector<osg::Vec2d> retVerts;
	for (int i = 0 ; i < verts->size(); i++)
	{
		retVerts.push_back(ProjectionPoint(verts->at(i)));
	}
	return retVerts;
}

double COsgCoordSystem::DistanceTo(const osg::Vec3d &point) const
{
	osg::Vec3d p = point * GetInverse();
	return fabs(p.z());
}

osg::Vec3d COsgCoordSystem::GetClosestPt(const osg::Vec3d &point) const
{
	// TODO：可能计算有误
	osg::Vec3 p(ProjectionPoint(point).x(), ProjectionPoint(point).y(), point.z());
	return p * GetTranslateMatrix();
}

bool COsgCoordSystem::IsInXOY(const osg::Vec3d &point, double tol /*= 1.E-5*/) const
{
	return fabs(DistanceTo(point)) < tol;
}

void COsgCoordSystem::TransformBy(const osg::Matrixd &mat)
{
	*this *= mat;
}

void COsgCoordSystem::ArbitraryAxisAlgorithm(const osg::Vec3d &normal, osg::Vec3d &xAxis, osg::Vec3d &yAxis)
{
#ifdef _DEBUG
	if (normal.length() < 1.0E-5)
	{
		//assert (false);
	}
#endif
	ArbitraryAxisAlgorithm(normal, xAxis);
	yAxis = normal ^ xAxis;
}

void COsgCoordSystem::ArbitraryAxisAlgorithm(const osg::Vec3d &normal, osg::Vec3d &xAxis)
{
	osg::Vec3d n = normal;
	n.normalize();
	double tol = 1.0 / 64.0;
	if (fabs(n.x()) < tol && fabs(n.y()) < tol)
	{
		xAxis = osg::Y_AXIS ^ n;
	}
	else
	{
		xAxis = osg::Z_AXIS ^ n;
	}
	xAxis.normalize();
}

bool COsgCoordSystem::LineIntersectPoint(const osg::Vec3d &startPoint, const osg::Vec3d &endPoint, osg::Vec3d &intPoint) const
{
	//首先判断直线是否与平面平行 
// 	osg::Vec3d v = endPoint - startPoint;
// 	v.normalize();
// 	if (COsgMathUtil::AngleTo(v, *this) > 1.0E-7)  
// 	{  
// 		osg::Vec3d o = kOrigin(), n = kZAxis();
// 		double vpt = v.x() * n.x() + v.y() * n.y() + v.z() * n.z();
// 		double t = ((o.x() - startPoint.x()) * n.x() + (o.y() - startPoint.y()) * n.y() + (o.z() - startPoint.z()) * n.z()) / vpt;  
// 		intPoint[0] = startPoint.x() + v.x() * t;  
// 		intPoint[1] = startPoint.y() + v.y() * t;  
// 		intPoint[2] = startPoint.z() + v.z() * t;  
// 		return true;
// 	}  
	return false;
}

osg::Vec2d COsgCoordSystem::ProjectionVector(const osg::Vec3d &v) const
{
	osg::Vec2d val =  ProjectionPoint(kOrigin() + v);
	val.normalize();
	return val;
}

void COsgCoordSystem::MoveOrigin(const osg::Vec3d &originOffset)
{
	osg::Vec3d origin = kOrigin();
	osg::Vec3d xAxis = kXAxis();
	osg::Vec3d yAxis = kYAxis();
	origin = origin + originOffset;
	Set(origin, xAxis, yAxis);
}
