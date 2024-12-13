// 文件名：COsgMainpulator.h
// 作者：明军；日期：2015-03-21
// 描述：自定义操作器
// 正常的情况，平移，三维缩放，三维旋转
//////////////////////////////////////////////////////////////////////////
// 历史记录
/////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _EPRITOWERDESIGN_OSGMAINPULAOTR_H_
#define _EPRITOWERDESIGN_OSGMAINPULAOTR_H_

#include <osgGA\CameraManipulator>
#include <osgGA\StandardManipulator>

#if defined (_ZFGKCOMMONLIB_)
#define ZFGK_DLLIMPEXP __declspec(dllexport)
#else
#define ZFGK_DLLIMPEXP 
#endif

class ZFGK_DLLIMPEXP COsgMainpulator : public osgGA::StandardManipulator
{
public:
	COsgMainpulator();
	COsgMainpulator(osg::Camera *pCamera);
	virtual ~COsgMainpulator(void);

	// 操作器行为
	enum MainpulatorType
	{
		MainType_Normal = 0,	// 正常
		MainType_Pan = 1,	// 平移
		MainType_Orbit = 2,		// 三维动态观察
		MainType_Zoom = 3,		// 三维缩放
	};
	/** set the position of the matrix manipulator using a 4x4 Matrix.*/
	// 设置当前视口
	virtual void setByMatrix(const osg::Matrixd& matrix);

	///** set the position of the matrix manipulator using a 4x4 Matrix.*/
	//// 设置当前视口
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);

	///** get the position of the manipulator as 4x4 Matrix.*/
	//// 得到当前矩阵
	virtual osg::Matrixd getMatrix() const;

	///** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
	//// 得到当前矩阵
	virtual osg::Matrixd getInverseMatrix() const;

	/** Sets manipulator by eye position and eye orientation.*/
	virtual void setTransformation( const osg::Vec3d& eye, const osg::Quat& rotation );

	/** Sets manipulator by eye position, center of rotation, and up vector.*/
	virtual void setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up );

	/** Gets manipulator's eye position and eye orientation.*/
	virtual void getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const ;

	/** Gets manipulator's focal center, eye position, and up vector.*/
	virtual void getTransformation( osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up ) const;

	virtual void rotateTrackball( const double px0, const double py0,
		const double px1, const double py1, const double scale );
	void trackball( osg::Vec3d& axis, double& angle, double p1x, double p1y, double p2x, double p2y );
	double tb_project_to_sphere( double r, double x, double y );
	//void trackball2( osg::Vec3d& axis, double& angle, double p1x, double p1y, double p2x, double p2y );

	// 响应滚轮事件
	virtual bool handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
	// 左键响应
	virtual bool performMovementLeftMouseButton( const double eventTimeDelta,
		const double dx, const double dy );
	// 响应中键
	virtual bool performMovementMiddleMouseButton( const double eventTimeDelta,
		const double dx, const double dy );

	// 响应右键
	virtual bool performMovementRightMouseButton(const double eventTimeDelta,
		const double dx, const double dy);

	virtual void zoomModel(const osgGA::GUIEventAdapter& ea, bool bUp );

	// 平移模型
	virtual void panModel( const double dx, const double dy, const double dz = 0.f );

	// 响应按键消息
	virtual bool handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

public:
	// 描述：移动视图到某个中心点上,不改变其他值
	// centerNew:视点 
	// 返回值：空
	// 作者：明军；时间：2015-06-15
	void CenterMoveTo(const osg::Vec3d &centerNew);

public:
	// 设置相机
	void SetCamera(osg::Camera *pCamera);
	// 设置旋转中心点
	void SetRotateCenter(const osg::Vec3d &ptCenter);
	// 获得旋转中心点
	osg::Vec3d GetRotateCenter() const;

	// 设置缩放比例
	void SetFactor(double dFactor);
	// 获取缩放比例
	double GetFactor() const;

	// 操作器行为
	void SetMainpulatorType(MainpulatorType tyep);
	MainpulatorType GetMainpulatorType() const;

private:
	MainpulatorType m_mainType;

	osg::Quat m_rotation;	
	osg::Vec3d m_eye;

	osg::Vec3d m_ptRoateBy;	// 旋转中心点
	osg::Camera *m_pCamera;	// 视图的相机

	double m_curFactor;	// 缩放比例

};
#endif