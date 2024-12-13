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

	// ������
	osg::ref_ptr<osg::Node> createLine(const osg::Vec3 &start,/*���*/
									   const osg::Vec3 &end,/*�յ�*/
									   double width = 1.0,/*�߿�*/
									   const osg::Vec4 color = osg::Vec4(1.0, 1.0, 1.0, 1));

	osg::ref_ptr<osg::Node> createCircle(const osg::Vec3 &center,
		double radius,
		double width = 1.0,
		const osg::Vec4 color = osg::Vec4(1.0, 1.0, 1.0, 1)); // ����Բ

	osg::ref_ptr<osg::Node> createPoint(const osg::Vec3 &pos,
		double width = 1.0,
		const osg::Vec4 color = osg::Vec4(1.0, 1.0, 1.0, 1)); // ���Ƶ�


	osg::ref_ptr<osg::Node> createCylinder(osg::Vec3 center,
		double Radius,
		double Height,
		osg::Vec4 xcolor = osg::Vec4(1.0, 1.0, 1.0, 1.0),
		osg::Vec3f Axial = osg::Z_AXIS,
		osg::Vec3 vec = osg::Vec3(0.0, 0.0, 1.0));  // ����Բ��

	osg::ref_ptr<osg::Node> createCone(osg::Vec3 center,
		double Radius,
		double Height,
		osg::Vec4 xcolor = osg::Vec4(1.0, 1.0, 1.0, 1.0),
		osg::Vec3f Axial = osg::Z_AXIS,
		osg::Vec3 vec = osg::Vec3(0.0, 0.0, 1.0));  // ����Բ׶

	osg::ref_ptr<osg::Node> createSpline(const std::vector<osg::Vec3> &posSet,
		double width = 1.0,
		const osg::Vec4 color = osg::Vec4(1.0, 1.0, 1.0, 1)); // ��������

	// ���������������Բ��
	osg::ref_ptr<osg::MatrixTransform> createCylinder(const osg::Vec3 &from,
		const osg::Vec3 &to,
		float radius);

	// ����
	osg::ref_ptr<osg::Node> createText(const std::string& text,
		const osg::Vec3& possition,
		double size = 1.0,
		const osg::Vec4& color = osg::Vec4(0.0, 1.0, 0.0, 1.0));

	// ����HUD
	osg::ref_ptr<osg::Camera> createHUD(const osg::Vec3& pos, const std::string& tStr
		, const osg::Vec4& color = osg::Vec4(1.0, 1.0, 1.0, 1.0));

	// ����Բ̨
// 	osg::ref_ptr<osg::MatrixTransform> createRoundTable(const osg::Vec3 &center,
// 		float radiusUp, 
// 		float radiusDown);
};

