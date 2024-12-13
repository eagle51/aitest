// �ļ����� OsgCoordSystem.h
// ���ߣ����ڣ����ڣ�2015/7/30
// ����������ϵ��
//////////////////////////////////////////////////////////////////////////
// ��ʷ��¼
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

// ��׼������������(OSGϵͳ�ı�׼�����������ΪVec3f����)
#define XAxis osg::Vec3d(1.0, 0.0, 0.0)
#define YAxis osg::Vec3d(0.0, 1.0, 0.0)
#define ZAxis osg::Vec3d(0.0, 0.0, 1.0)

class ZFGK_DLLIMPEXP COsgCoordSystem : public osg::Matrixd
{
	// ���캯��
public:
	COsgCoordSystem();
	COsgCoordSystem(const osg::Matrixd &mat);
	COsgCoordSystem(const osg::Vec3d &origin, const osg::Vec3d  &xAxis, const osg::Vec3d  &yAxis);
	COsgCoordSystem(const osg::Vec3d &origin, const osg::Vec3d & normal);
	COsgCoordSystem(const osg::Vec2d &origin, const osg::Vec2d &xAxis);
	COsgCoordSystem(const osg::Vec2d &origin);
	COsgCoordSystem(const osg::Vec3d &origin);

	// ��������
public:
	// ����:		����	
	// ���ߣ�		���� 2015/4/2 9:47:41
	void Set(const osg::Matrix &mat);
	void Set(const osg::Vec3d &origin);
	void Set(const osg::Vec2d &origin);
	void Set(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &yAxis);
	void Set(const osg::Vec3d &origin, const osg::Vec3d &normal);
	void Set(const osg::Vec2d &origin, const osg::Vec2d &xAxis);

	// ͨ�����㹹��
	void SetBy3P(const osg::Vec3d &origin, const osg::Vec3d &xP, const osg::Vec3d &yP);

	// ͨ��y��z�ᴴ������ϵ
	void SetByYAndZAxis(const osg::Vec3d &origin, const osg::Vec3d &yAxis, const osg::Vec3d &zAxis);
	// ͨ��x��z�ᴴ������ϵ
	void SetByXAndZAxis(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &zAxis);

	// �ƶ�ԭ��
	void MoveOrigin(const osg::Vec3d &originOffset);

	// ����
private:
	// Description:�����������ϵ����������ϵ��ת������(���Բ������������, ������Matrixd�̳еģ�ֱ���þͿ�����)
	// Returns:   osg::Matrixd
	// Qualifier: const
	// Author:	TianTeng 2016/1/20 17:59:21
	osg::Matrixd GetTranslateMatrix() const;

public:
	// Description:�����������ϵ����������ϵ��ת������
	// Returns:   osg::Matrixd
	// Qualifier: const
	// Author:	TianTeng 2016/1/20 18:01:48
	osg::Matrixd GetInverse() const;

	// ����:		����ϵ����	
	// ���ߣ�		���� 2015/4/2 9:48:31
	osg::Vec3d kOrigin() const;
	osg::Vec3d kXAxis() const;
	osg::Vec3d kYAxis() const;
	osg::Vec3d kZAxis() const;

	// ����
public:
	//************************************
	// Method:    ͶӰ�㵽���������ϵ��XOYƽ��,�õ��ĵ���������ϵ����������ϵ��
	// FullName:  COsgCoordSystem::ProjectionPoint
	// Access:    public static 
	// Returns:   osg::Vec2d
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	//************************************
	osg::Vec2d ProjectionPoint(const osg::Vec3d &point) const;
	//************************************
	// Method:    ͶӰ�㵽���������ϵ��XOYƽ��,�õ��ĵ���������ϵ����������ϵ��
	// FullName:  COsgCoordSystem::ProjectionPoint
	// Access:    public static 
	// Returns:   std::vector<osg::Vec2d>
	// Qualifier: const
	// Parameter: const osg::ref_ptr<osg::Vec3dArray> & verts
	//************************************
	std::vector<osg::Vec2d> ProjectionPoint(const osg::ref_ptr<osg::Vec3dArray> &verts) const;

	//************************************
	// Method:    �㵽XOYƽ��ľ���
	// FullName:  COsgCoordSystem::DistanceTo
	// Access:    public 
	// Returns:   double
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	//************************************
	double DistanceTo(const osg::Vec3d &point) const;

	//************************************
	// Method:    �㵽XOY�������(��������ϵ�ĵ㣩
	// FullName:  COsgCoordSystem::GetClosestPt
	// Access:    public 
	// Returns:   osg::Vec3d
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	//************************************
	osg::Vec3d GetClosestPt(const osg::Vec3d &point) const;

	//************************************
	// Method:    �ж�һ�����Ƿ���XOYƽ����
	// FullName:  COsgCoordSystem::IsInXOY
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	// Parameter: double tol
	//************************************
	virtual bool IsInXOY(const osg::Vec3d &point, double tol = 1.E-7) const;

	//************************************
	// Method:    λ�ñ任
	// FullName:  COsgCoordSystem::TransformBy
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: const osg::Matrixd & mat
	//************************************
	void TransformBy(const osg::Matrixd &mat);

	// Description:ֱ�ߺ�ƽ��Ľ���
	// Returns:   bool
	// Qualifier:
	// Parameter: const osg::Vec3d & startPoint
	// Parameter: const osg::Vec3d & endPoint
	// Parameter: osg::Vec3d & intPoint
	// Author:	TianTeng 2016/2/24 10:32:01
	bool LineIntersectPoint(const osg::Vec3d &startPoint, const osg::Vec3d &endPoint, osg::Vec3d &intPoint) const;

	//************************************
	// Method:    ͶӰ����**ע��:Z��ͶӰ֮����0,0,0**
	// FullName:  COsgCoordSystem::ProjectionVector
	// Access:    public 
	// Returns:   osg::Vec3d
	// Qualifier: const
	// Parameter: const osg::Vec3d & point
	//************************************
	osg::Vec2d ProjectionVector(const osg::Vec3d &v) const;

	// ��̬����
public:
	// ����:		�������㷨,����Z��������������
	// �������:	������
	// �������:	������������������ֵ	
	// ���ߣ�		���� 2015/3/26 9:57:59
	static void ArbitraryAxisAlgorithm( const osg::Vec3d &normal, osg::Vec3d &xAxis, osg::Vec3d &yAxis );
	static void ArbitraryAxisAlgorithm( const osg::Vec3d &normal, osg::Vec3d &xAxis);

	// ����:		��һ����ͶӰ��һ������ϵ�ϣ������������������ϵ������	
	// ����:		�˺�����ʱЧ�ʱȽϵͣ����Գ��԰ѱ任���󱣴�����
	// mat:			�������ϵ���������� ������������
	// ���ߣ�		���� 2015/3/27 11:35:09
	//static osg::Vec3d ProjectionPoint(const osg::Vec3d &origin, const osg::Vec3d &xAxis, const osg::Vec3d &yAxis, const osg::Vec3d &point);
	static osg::Vec2d ProjectionPoint(const osg::Matrixd &mat, const osg::Vec3d & point);
};
