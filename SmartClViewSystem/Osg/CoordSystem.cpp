#include "CoordSystem.h"
#include <osg\Geode>
#include <osg\ShapeDrawable>

CoordSystem::CoordSystem():
_AxialR(0.05),
_AxialLen(0.9),
_AxialLenR(0.2)
{
	XAxialColor = osg::Vec4(1.0, 0, 0, 1.0);
	YAxialColor = osg::Vec4(0, 1.0, 0, 1.0);
	ZAxialColor = osg::Vec4(0, 0, 1.0, 1.0);
	initEntity();
}

CoordSystem::~CoordSystem()
{
}

// @ funtion:    --- by lt 2019/5/22 15:54
void CoordSystem::draw(osg::ref_ptr<osg::Group> root)
{
 	root->addChild(quadraticX.get());
	root->addChild(quadraticXr.get());
	root->addChild(quadraticY.get());
	root->addChild(quadraticYr.get());
	root->addChild(quadraticZ.get());
	root->addChild(quadraticZr.get());
}

// @ funtion:    --- by lt 2019/5/22 16:03
void CoordSystem::initEntity()
{
	quadraticX = createCylinder(osg::Vec3(_AxialLen / 2, 0.0, 0.0),
								_AxialR,
								_AxialLen,
								XAxialColor,
								osg::Z_AXIS,
								osg::Vec3(1.0, 0.0, 0.0));

	quadraticXr = createCone(osg::Vec3(_AxialLen + _AxialLenR / 4, 0.0, 0.0),
							 _AxialR * 3 / 2,
							 _AxialLenR,
							 XAxialColor,
							 osg::Z_AXIS,
							 osg::Vec3(1.0, 0.0, 0.0));

	quadraticY = createCylinder(osg::Vec3(0.0, _AxialLen / 2, 0.0),
								_AxialR,
								_AxialLen,
								YAxialColor,
								osg::Z_AXIS,
								osg::Vec3(0.0, 1.0, 0.0));

	quadraticYr = createCone(osg::Vec3(0.0, _AxialLen + _AxialLenR / 4, 0.0),
							 _AxialR * 3 / 2,
							 _AxialLenR,
							 YAxialColor,
							 osg::Z_AXIS,
							 osg::Vec3(0.0, 1.0, 0.0));

	quadraticZ = createCylinder(osg::Vec3(0.0, 0.0, _AxialLen / 2),
								_AxialR,
								_AxialLen,
								ZAxialColor,
								osg::Z_AXIS,
								osg::Vec3(0.0, 0.0, 1.0));

	quadraticZr = createCone(osg::Vec3(0.0, 0.0, _AxialLen + _AxialLenR / 4),
							 _AxialR * 3 / 2,
							 _AxialLenR,
							 ZAxialColor,
							 osg::Z_AXIS,
							 osg::Vec3(0.0, 0.0, 1.0));
}

// @ funtion: 设置坐标系箭头尺寸   --- by lt 2019/5/22 17:18
void CoordSystem::updateEntity(double radio)
{
	_AxialR = radio / 100;
	_AxialLen = 18 * radio;
	_AxialLenR = 4 * radio;
}