#pragma once
#include <osg/Node>
#include <vector>
#include <osg/ref_ptr>
#include "osg\Array"
#include <osgViewer\ViewerEventHandlers>
#include <osg\MatrixTransform>
#include <string>
using std::vector;
class OsgDrawEnt
{
public:
	OsgDrawEnt();
	~OsgDrawEnt();

	// 绘制线
	osg::ref_ptr<osg::Node> createLine(const osg::Vec3 &start,/*起点*/
									   const osg::Vec3 &end,/*终点*/
									   double width = 1.0,/*线宽*/
									   const osg::Vec4 color = osg::Vec4(1.0, 1.0, 1.0, 1));

	osg::ref_ptr<osg::Node> createCircle(const osg::Vec3 &center,
		double radius,
		double width = 1.0,
		const osg::Vec4 color = osg::Vec4(1.0, 1.0, 1.0, 1)); // 绘制圆

	osg::ref_ptr<osg::Node> createPoint(const osg::Vec3 &pos,
		double width = 1.0,
		const osg::Vec4 color = osg::Vec4(1.0, 1.0, 1.0, 1)); // 绘制点


	osg::ref_ptr<osg::Node> createCylinder(osg::Vec3 center,
		double Radius,
		double Height,
		osg::Vec4 xcolor = osg::Vec4(1.0, 1.0, 1.0, 1.0),
		osg::Vec3f Axial = osg::Z_AXIS,
		osg::Vec3 vec = osg::Vec3(0.0, 0.0, 1.0));  // 绘制圆柱

	osg::ref_ptr<osg::Node> createCone(osg::Vec3 center,
		double Radius,
		double Height,
		osg::Vec4 xcolor = osg::Vec4(1.0, 1.0, 1.0, 1.0),
		osg::Vec3f Axial = osg::Z_AXIS,
		osg::Vec3 vec = osg::Vec3(0.0, 0.0, 1.0));  // 绘制圆锥

	osg::ref_ptr<osg::Node> createSpline(const std::vector<osg::Vec3> &posSet,
		double width = 1.0,
		const osg::Vec4 color = osg::Vec4(1.0, 1.0, 1.0, 1)); // 绘制曲线

	// 根据两点坐标绘制圆柱
	osg::ref_ptr<osg::MatrixTransform> createCylinder(const osg::Vec3 &from,
		const osg::Vec3 &to,
		float radius);

	// 文字
	osg::ref_ptr<osg::Node> createText(const std::string& text,
		const osg::Vec3& possition,
		double size = 1.0,
		const osg::Vec4& color = osg::Vec4(0.0, 1.0, 0.0, 1.0));

	// 绘制HUD
	osg::ref_ptr<osg::Camera> createHUD(const osg::Vec3& pos, const std::string& tStr
		, const osg::Vec4& color = osg::Vec4(1.0, 1.0, 1.0, 1.0));

	// 绘制圆台
// 	osg::ref_ptr<osg::MatrixTransform> createRoundTable(const osg::Vec3 &center,
// 		float radiusUp, 
// 		float radiusDown);
};

