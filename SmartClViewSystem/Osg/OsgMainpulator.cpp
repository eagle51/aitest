// 文件名：COsgMainpulator.cpp
// 作者：明军；日期：2015-03-21
// 描述：自定义操作器实现文件
//////////////////////////////////////////////////////////////////////////
// 历史记录
// 支持序列化 by mj 150609
/////////////////////////////////////////////////////////////////////////
//

#include "OsgMainpulator.h"
#include "OsgCoordSystem.h"

const double g_Dis = 200000.0f;
#define VERSION_OSGMAINPULATOR 1

COsgMainpulator::COsgMainpulator(osg::Camera *pCamera)
{
	m_curFactor = 0.5f;
	m_mainType = MainType_Normal;
	SetCamera(pCamera);
	setAllowThrow(false);	
	setVerticalAxisFixed(false);
	// 设置其放大缩小方式与cad一样
// 	double dOld =getWheelZoomFactor();
// 	setWheelZoomFactor(-1.0 * dOld);
	osg::Vec3d eyePoint(0.0f, 200000.0f, 0.0f);
	osg::Vec3d center(0.0f, 200000.0f - g_Dis, 0.0f);
	osg::Vec3d up = -osg::Z_AXIS;
	setTransformation(eyePoint, center, up);
}

COsgMainpulator::COsgMainpulator()
{
	m_curFactor = 0.5f;
	m_mainType = MainType_Pan;
	SetCamera(NULL);
	setAllowThrow(false);	
	setVerticalAxisFixed(false);
	osg::Vec3d eyePoint(0.0f, 200000.0f, 0.0f);
	osg::Vec3d center(0.0f, 200000.0f - g_Dis, 0.0f);
	osg::Vec3d up = -osg::Z_AXIS;
	setTransformation(eyePoint, center, up);
}

COsgMainpulator::~COsgMainpulator(void)
{
}

void COsgMainpulator::rotateTrackball( const double px0, const double py0, const double px1, const double py1, const double scale )
{
	osg::Vec3d axis;
	double angle;

	trackball( axis, angle, px1, py1, px0, py0 );

	osg::Quat new_rotate;
	new_rotate.makeRotate( angle*2, axis ); // 2倍速

	m_rotation = m_rotation * new_rotate;

	osg::Vec3d vecTemp = m_eye - m_ptRoateBy;
	osg::Vec3d vecTemp2 = new_rotate * vecTemp;
	m_eye = m_eye + (vecTemp2 - vecTemp);
}

void COsgMainpulator::SetRotateCenter( const osg::Vec3d &ptCenter )
{
	m_ptRoateBy = ptCenter;
}

osg::Vec3d COsgMainpulator::GetRotateCenter() const
{
	return m_ptRoateBy;
}

void COsgMainpulator::setByMatrix( const osg::Matrixd& matrix )
{
	m_eye = matrix.getTrans();
	m_rotation = matrix.getRotate();
}

void COsgMainpulator::setByInverseMatrix( const osg::Matrixd& matrix )
{
	setByMatrix(osg::Matrixd::inverse(matrix));
}

osg::Matrixd COsgMainpulator::getMatrix() const
{
	return osg::Matrixd::rotate(m_rotation) * osg::Matrixd::translate(m_eye);
}

osg::Matrixd COsgMainpulator::getInverseMatrix() const
{
	return osg::Matrixd::translate(-m_eye) * osg::Matrixd::rotate(m_rotation.inverse());
}

void COsgMainpulator::setTransformation( const osg::Vec3d& eye, const osg::Quat& rotation )
{
	m_eye = eye;
	m_rotation = rotation;

	// fix current rotation
	if( getVerticalAxisFixed() )
		fixVerticalAxis( m_ptRoateBy, m_rotation, true );
}

void COsgMainpulator::setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up )
{
	osg::Vec3d lv( center - eye );

	osg::Vec3d f( lv );
	f.normalize();
	osg::Vec3d s( f^up );
	s.normalize();
	osg::Vec3d u( s^f );
	u.normalize();

	osg::Matrixd rotation_matrix( s[0], u[0], -f[0], 0.0f,
		s[1], u[1], -f[1], 0.0f,
		s[2], u[2], -f[2], 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f );

	m_eye = eye;
	m_rotation = rotation_matrix.getRotate().inverse();

	// fix current rotation
	if( getVerticalAxisFixed() )
		fixVerticalAxis( m_ptRoateBy, m_rotation, true );
}

void COsgMainpulator::getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const
{
	eye = m_eye;
	rotation = m_rotation;
}

void COsgMainpulator::getTransformation( osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up ) const
{
	center = m_eye + m_rotation * osg::Vec3d( 0., 0., -g_Dis );
	eye = m_eye;
	up = m_rotation * osg::Vec3d( 0., 1., 0. );
}

bool COsgMainpulator::performMovementMiddleMouseButton( const double eventTimeDelta,
													   const double dx, const double dy )
{
	// MainType_Orbit操作器当用户按下Ctrl键的时候，中键也执行Orbit功能

	// get deltaX and deltaY
	double x1 = _ga_t0->getX(), y1 = _ga_t0->getY(), x2 = _ga_t1->getX(), y2 = _ga_t1->getY();
	osg::Vec3d vecWindow1(x1, y1, 0.0), vecWindow2(x2, y2, 0.0);
	osg::Vec3d vecWord1, vecWord2;

	// 获取当前运算矩阵
	osg::Matrix VPW = m_pCamera->getViewMatrix() *
		m_pCamera->getProjectionMatrix() *
		(m_pCamera->getViewport())->computeWindowMatrix();
	osg::Matrix inverseVPW;
	inverseVPW.invert(VPW);
	// 矩阵运算获得世界坐标
	vecWord1 = vecWindow1 * inverseVPW;
	vecWord2 = vecWindow2 * inverseVPW;

	osg::Vec3d vecMove = vecWord2 - vecWord1;
	panModel(vecMove.x(), vecMove.y(), vecMove.z());		

	return true;
}

void COsgMainpulator::panModel( const double dx, const double dy, const double dz /*= 0.f */ )
{
	osg::Matrix rotation_matrix;
	rotation_matrix.makeRotate( m_rotation );

	osg::Vec3d dv( dx, dy, dz );

	m_eye += dv;// * rotation_matrix;
}

bool COsgMainpulator::performMovementLeftMouseButton( const double eventTimeDelta,
													 const double dx, const double dy )
{
	switch(m_mainType)
	{
	case MainType_Normal:
		{
			return false;
		}
	case MainType_Orbit:
		{
			// 旋转
			rotateTrackball( _ga_t0->getXnormalized(), _ga_t0->getYnormalized(),
				_ga_t1->getXnormalized(), _ga_t1->getYnormalized(),
				getThrowScale( eventTimeDelta ) );
			return true;
		}
	case MainType_Pan:
		{
			// 平移
			double x1 = _ga_t0->getX(), y1 = _ga_t0->getY(), x2 = _ga_t1->getX(), y2 = _ga_t1->getY();
			osg::Vec3d vecWindow1(x1, y1, 0.0), vecWindow2(x2, y2, 0.0);
			osg::Vec3d vecWord1, vecWord2;

			// 获取当前运算矩阵
			osg::Matrix VPW = m_pCamera->getViewMatrix() *
				m_pCamera->getProjectionMatrix() *
				(m_pCamera->getViewport())->computeWindowMatrix();
			osg::Matrix inverseVPW;
			inverseVPW.invert(VPW);
			// 矩阵运算获得世界坐标
			vecWord1 = vecWindow1 * inverseVPW;
			vecWord2 = vecWindow2 * inverseVPW;

			osg::Vec3d vecMove = vecWord2 - vecWord1;
			panModel(vecMove.x(), vecMove.y(), vecMove.z());
			return true;
			//return performMovementMiddleMouseButton(eventTimeDelta, dx, dy);
			//CenterMoveTo();
		}
	case MainType_Zoom:
		{
			return true;
		}
	default:
		return false;
	}
	return false;
}

// @ funtion:    --- by lt 2019/5/23 16:55
bool COsgMainpulator::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
	// 旋转
	rotateTrackball(_ga_t0->getXnormalized(), _ga_t0->getYnormalized(),
		_ga_t1->getXnormalized(), _ga_t1->getYnormalized(),
		getThrowScale(eventTimeDelta));
	return true;
}


void COsgMainpulator::SetMainpulatorType( MainpulatorType tyep )
{
	m_mainType = tyep;
}

COsgMainpulator::MainpulatorType COsgMainpulator::GetMainpulatorType() const
{
	return m_mainType;
}

/**
 * Simulate a track-ball.  Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 *
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */
void COsgMainpulator::trackball( osg::Vec3d& axis, double& angle, double p1x, double p1y, double p2x, double p2y )
{
 /*
        * First, figure out z-coordinates for projection of P1 and P2 to
        * deformed sphere
        */

    osg::Matrixd rotation_matrix(m_rotation);

	osg::Vec3d uv = osg::Vec3d(0.0f,1.0f,0.0f)*rotation_matrix;
    osg::Vec3d sv = osg::Vec3d(1.0f,0.0f,0.0f)*rotation_matrix;
    osg::Vec3d lv = osg::Vec3d(0.0f,0.0f,-1.0f)*rotation_matrix;

	double trackballSize = 0.8;
    osg::Vec3d p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(trackballSize, p1x, p1y);
    osg::Vec3d p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(trackballSize, p2x, p2y);

    /*
        *  Now, we want the cross product of P1 and P2
        */
    axis = p2^p1;
    axis.normalize();

    /*
        *  Figure out how much to rotate around that axis.
        */
    double t = (p2 - p1).length() / (2.0 * trackballSize);

    /*
        * Avoid problems with out-of-control values...
        */
    if (t > 1.0) t = 1.0;
    if (t < -1.0) t = -1.0;
	angle = osg::inRadians(asin(t));
}

/**
 * Helper trackball method that projects an x,y pair onto a sphere of radius r OR
 * a hyperbolic sheet if we are away from the center of the sphere.
 */
double COsgMainpulator::tb_project_to_sphere( double r, double x, double y )
{
	double d, t, z;

	d = sqrt(x*x + y*y);
	/* Inside sphere */
	if (d < r * 0.70710678118654752440)
	{
		z = sqrt(r*r - d*d);
	}                            /* On hyperbola */
	else
	{
		t = r / 1.41421356237309504880;
		z = t*t / d;
	}
	return z;
}

bool COsgMainpulator::handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

	switch( sm )
	{
		// mouse scroll up event
	case osgGA::GUIEventAdapter::SCROLL_UP:
		{
			// perform zoom
			zoomModel( ea, false);
			us.requestRedraw();
			us.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// mouse scroll down event
	case osgGA::GUIEventAdapter::SCROLL_DOWN:
		{
			// perform zoom
			zoomModel( ea, true);
			us.requestRedraw();
			us.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// unhandled mouse scrolling motion
	default:
		return false;
	}
}

void COsgMainpulator::zoomModel(const osgGA::GUIEventAdapter& ea, bool bUp )
{
	osg::Vec3 ptWorldBefore, ptWorldAfter;	// 修改投影矩阵前后鼠标位置对应的世界坐标点
	float x1 = ea.getX(), y1 = ea.getY();
	osg::Vec3 vecWindow1(x1, y1, 0.0);
	{
		// 计算修改投影矩阵前

		// 获取当前运算矩阵
		osg::Matrix VPW = m_pCamera->getViewMatrix() *
			m_pCamera->getProjectionMatrix() *
			(m_pCamera->getViewport())->computeWindowMatrix();
		osg::Matrix inverseVPW;
		inverseVPW.invert(VPW);
		// 矩阵运算获得世界坐标
		ptWorldBefore = vecWindow1 * inverseVPW;
	}

	if (bUp)
	{
		m_curFactor /= 1.2;
	} 
	else
	{
		m_curFactor *= 1.2;
	}

	int iww = m_pCamera->getGraphicsContext()->getTraits()->width;
	int ihh = m_pCamera->getGraphicsContext()->getTraits()->height;
	double width ,height ;

	width = iww / m_curFactor;
	height = ihh / m_curFactor;

	m_pCamera->setProjectionMatrixAsOrtho(-width * 0.1, width * 0.1, 
		-height * 0.1, height * 0.1, 1.0, 1000.0);

	{
		// 计算修改投影矩阵后
		// 获取当前运算矩阵
		osg::Matrix VPW = m_pCamera->getViewMatrix() *
			m_pCamera->getProjectionMatrix() *
			(m_pCamera->getViewport())->computeWindowMatrix();
		osg::Matrix inverseVPW;
		inverseVPW.invert(VPW);
		// 矩阵运算获得世界坐标
		ptWorldAfter = vecWindow1 * inverseVPW;
	}

	// 保证视点平移后还在原来的平面
	// 构造原来视点所在的平面
	osg::Vec3d vecX, vecY;
	vecX = m_rotation * osg::X_AXIS;
	vecY = m_rotation * osg::Y_AXIS;
	COsgCoordSystem coord(m_eye, vecX, vecY);

	osg::Vec3 vecMove = ptWorldBefore - ptWorldAfter;
	//panModel(vecMove.x(), vecMove.y(), vecMove.z());
	m_eye = coord.GetClosestPt(m_eye);
}

bool COsgMainpulator::handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	return false;
}

void COsgMainpulator::SetFactor( double dFactor )
{
	m_curFactor = dFactor;
}

double COsgMainpulator::GetFactor() const
{
	return m_curFactor;
}

void COsgMainpulator::SetCamera( osg::Camera *pCamera )
{
	m_pCamera = pCamera;
}

void COsgMainpulator::CenterMoveTo( const osg::Vec3d &centerNew )
{
	osg::Vec3d eye,center,  up;
	getTransformation(eye, center, up);
	osg::Vec3d vecMove = centerNew - center;
	eye = eye + vecMove;
	setTransformation(eye, centerNew, up);
}
