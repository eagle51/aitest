// 文件名： OsgCoordSystem.h
// 作者：田腾；日期：2015/7/30
// 描述：坐标系类
//////////////////////////////////////////////////////////////////////////
// 历史记录
/////////////////////////////////////////////////////////////////////////
#pragma once
#include <osg\Matrixd>
#include <osg\Vec3d>
#include <osg\Matrix>
#include <osg\Array>

#if defined (_ZFGKCOMMONLIB_)
#define ZFGK_DLLIMPEXP __declspec(dllexport)
#else
#define ZFGK_DLLIMPEXP 
#endif

// 标准的坐标轴向量(OSG系统的标准向量输出内容为Vec3f类型)
#define XAxis osg::Vec3d(1.0, 0.0, 0.0)
#define YAxis osg::Vec3d(0.0, 1.0, 0.0)
#define ZAxis osg::Vec3d(0.0, 0.0, 1.0)

class ZFGK_DLLIMPEXP COsgCoordSystem : public osg::Matrixd
{
	// 构造函数
public:
	COsgCoordSystem();
	COsgCoordSystem(const osg::Matrixd &mat);
	COsgCoordSystem(const osg::Vec3d &origin, const osg::Vec3d  &xAxis, const osg::Vec3d  &yAxis);
	COsgCoordSystem(const osg::Vec3d &origin, const osg::Vec3d & normal);
	COsgCoordSystem(const osg::Vec2d &origin, const osg::Vec2d &xAxis);
	COsgCoordSystem(const osg::Vec2d &origin);
	COsgCoordSystem(const osg::Vec3d &origin);

	// 构建函数
public:
	// 描述:		设置	
	// 作者：		田腾 2015/4/2 9:47:41
	void Set(const osg::Matrix &mat);
	void Set(const osg::Vec3d &origin);
	void Set(const osg::Vec2d &origin);
	void Set(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &yAxis);
	void Set(const osg::Vec3d &origin, const osg::Vec3d &normal);
	void Set(const osg::Vec2d &origin, const osg::Vec2d &xAxis);

	// 通过三点构建
	void SetBy3P(const osg::Vec3d &origin, const osg::Vec3d &xP, const osg::Vec3d &yP);

	// 通过y和z轴创建坐标系
	void SetByYAndZAxis(const osg::Vec3d &origin, const osg::Vec3d &yAxis, const osg::Vec3d &zAxis);
	// 通过x和z轴创建坐标系
	void SetByXAndZAxis(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &zAxis);

	// 移动原点
	void MoveOrigin(const osg::Vec3d &originOffset);

	// 属性
private:
	// Description:获得自身坐标系到世界坐标系的转换矩阵(可以不用这个函数了, 这个类从Matrixd继承的，直接用就可以了)
	// Returns:   osg::Matrixd
	// Qualifier: const
	// Author:	TianTeng 2016/1/20 17:59:21
	osg::Matrixd GetTranslateMatrix() const;

public:
	// Description:获得世界坐标系到自身坐标系的转换矩阵
	// Returns:   osg::Matrixd
	// Qualifier: const
	// Author:	TianTeng 2016/1/20 18:01:48
	osg::Matrixd GetInverse() const;

	// 描述:		坐标系参数	
	// 作者：		田腾 2015/4/2 9:48:31
	osg::Vec3d kOrigin() const;
	osg::Vec3d kXAxis() const;
	osg::Vec3d kYAxis() const;
	osg::Vec3d kZAxis() const;

	// 函数
public:
	//************************************
	// Method:    投影点到到这个坐标系的XOY平面,得到的点是在坐标系的自身坐标系中
	// FullName:  COsgCoordSystem::ProjectionPoint
	// Access:    public static 
	// Returns:   osg::Vec2d
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	//************************************
	osg::Vec2d ProjectionPoint(const osg::Vec3d &point) const;
	//************************************
	// Method:    投影点到到这个坐标系的XOY平面,得到的点是在坐标系的自身坐标系中
	// FullName:  COsgCoordSystem::ProjectionPoint
	// Access:    public static 
	// Returns:   std::vector<osg::Vec2d>
	// Qualifier: const
	// Parameter: const osg::ref_ptr<osg::Vec3dArray> & verts
	//************************************
	std::vector<osg::Vec2d> ProjectionPoint(const osg::ref_ptr<osg::Vec3dArray> &verts) const;

	//************************************
	// Method:    点到XOY平面的距离
	// FullName:  COsgCoordSystem::DistanceTo
	// Access:    public 
	// Returns:   double
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	//************************************
	double DistanceTo(const osg::Vec3d &point) const;

	//************************************
	// Method:    点到XOY的最近点(世界坐标系的点）
	// FullName:  COsgCoordSystem::GetClosestPt
	// Access:    public 
	// Returns:   osg::Vec3d
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	//************************************
	osg::Vec3d GetClosestPt(const osg::Vec3d &point) const;

	//************************************
	// Method:    判断一个点是否在XOY平面内
	// FullName:  COsgCoordSystem::IsInXOY
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	// Parameter: double tol
	//************************************
	virtual bool IsInXOY(const osg::Vec3d &point, double tol = 1.E-7) const;

	//************************************
	// Method:    位置变换
	// FullName:  COsgCoordSystem::TransformBy
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: const osg::Matrixd & mat
	//************************************
	void TransformBy(const osg::Matrixd &mat);

	// Description:直线和平面的交点
	// Returns:   bool
	// Qualifier:
	// Parameter: const osg::Vec3d & startPoint
	// Parameter: const osg::Vec3d & endPoint
	// Parameter: osg::Vec3d & intPoint
	// Author:	TianTeng 2016/2/24 10:32:01
	bool LineIntersectPoint(const osg::Vec3d &startPoint, const osg::Vec3d &endPoint, osg::Vec3d &intPoint) const;

	//************************************
	// Method:    投影向量**注意:Z轴投影之后变成0,0,0**
	// FullName:  COsgCoordSystem::ProjectionVector
	// Access:    public 
	// Returns:   osg::Vec3d
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	//************************************
	osg::Vec2d ProjectionVector(const osg::Vec3d &v) const;

	// 静态函数
public:
	// 描述:		任意轴算法,根据Z轴获得另外两个轴
	// 输入参数:	法向量
	// 输出参数:	计算出的另外两个轴的值	
	// 作者：		田腾 2015/3/26 9:57:59
	static void ArbitraryAxisAlgorithm( const osg::Vec3d &normal, osg::Vec3d &xAxis, osg::Vec3d &yAxis );
	static void ArbitraryAxisAlgorithm( const osg::Vec3d &normal, osg::Vec3d &xAxis);

	// 描述:		讲一个点投影到一个坐标系上，获得其相对于这个坐标系的坐标	
	// 其他:		此函数暂时效率比较低，可以尝试把变换矩阵保存起来
	// mat:			这个坐标系矩阵的逆矩阵 ！！！！！！
	// 作者：		田腾 2015/3/27 11:35:09
	//static osg::Vec3d ProjectionPoint(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &yAxis, const osg::Vec3d &point);
	static osg::Vec2d ProjectionPoint(const osg::Matrixd &mat, const osg::Vec3d & point);
};
