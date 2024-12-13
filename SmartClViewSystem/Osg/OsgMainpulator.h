// �ļ�����COsgMainpulator.h
// ���ߣ����������ڣ�2015-03-21
// �������Զ��������
// �����������ƽ�ƣ���ά���ţ���ά��ת
//////////////////////////////////////////////////////////////////////////
// ��ʷ��¼
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

	// ��������Ϊ
	enum MainpulatorType
	{
		MainType_Normal = 0,	// ����
		MainType_Pan = 1,	// ƽ��
		MainType_Orbit = 2,		// ��ά��̬�۲�
		MainType_Zoom = 3,		// ��ά����
	};
	/** set the position of the matrix manipulator using a 4x4 Matrix.*/
	// ���õ�ǰ�ӿ�
	virtual void setByMatrix(const osg::Matrixd& matrix);

	///** set the position of the matrix manipulator using a 4x4 Matrix.*/
	//// ���õ�ǰ�ӿ�
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);

	///** get the position of the manipulator as 4x4 Matrix.*/
	//// �õ���ǰ����
	virtual osg::Matrixd getMatrix() const;

	///** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
	//// �õ���ǰ����
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

	// ��Ӧ�����¼�
	virtual bool handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
	// �����Ӧ
	virtual bool performMovementLeftMouseButton( const double eventTimeDelta,
		const double dx, const double dy );
	// ��Ӧ�м�
	virtual bool performMovementMiddleMouseButton( const double eventTimeDelta,
		const double dx, const double dy );

	// ��Ӧ�Ҽ�
	virtual bool performMovementRightMouseButton(const double eventTimeDelta,
		const double dx, const double dy);

	virtual void zoomModel(const osgGA::GUIEventAdapter& ea, bool bUp );

	// ƽ��ģ��
	virtual void panModel( const double dx, const double dy, const double dz = 0.f );

	// ��Ӧ������Ϣ
	virtual bool handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

public:
	// �������ƶ���ͼ��ĳ�����ĵ���,���ı�����ֵ
	// centerNew:�ӵ� 
	// ����ֵ����
	// ���ߣ�������ʱ�䣺2015-06-15
	void CenterMoveTo(const osg::Vec3d &centerNew);

public:
	// �������
	void SetCamera(osg::Camera *pCamera);
	// ������ת���ĵ�
	void SetRotateCenter(const osg::Vec3d &ptCenter);
	// �����ת���ĵ�
	osg::Vec3d GetRotateCenter() const;

	// �������ű���
	void SetFactor(double dFactor);
	// ��ȡ���ű���
	double GetFactor() const;

	// ��������Ϊ
	void SetMainpulatorType(MainpulatorType tyep);
	MainpulatorType GetMainpulatorType() const;

private:
	MainpulatorType m_mainType;

	osg::Quat m_rotation;	
	osg::Vec3d m_eye;

	osg::Vec3d m_ptRoateBy;	// ��ת���ĵ�
	osg::Camera *m_pCamera;	// ��ͼ�����

	double m_curFactor;	// ���ű���

};
#endif