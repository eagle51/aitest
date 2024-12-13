#include "OsgDrawEnt.h"
#include "ElemArray.h"
#include "TowerModel.h"
#include "Box3D.h"
#include "Node.h"
#include <osg\Geode>
#include <osg\Geometry>
#include <osg\Point>
#include <osg\LineWidth>
#include <osg\ShapeDrawable>
#include <osg\MatrixTransform>
#include<windows.h>


OsgDrawEnt::OsgDrawEnt()
{
}


OsgDrawEnt::~OsgDrawEnt()
{

}


// @ funtion:    --- by lt 2019/5/22 18:23
osg::ref_ptr<osg::Node> OsgDrawEnt::createLine(const osg::Vec3 &start, const osg::Vec3 &end, double width /*= 1.0*/, const osg::Vec4 color /*= osg::Vec4(1.0, 1.0, 1.0, 1)*/)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

	//geode->setInitialBound(osg::BoundingSphere(osg::Vec3(), 1.0));

	//���ö�������
	osg::ref_ptr<osg::Vec3Array> vex = new osg::Vec3Array;
	vex->push_back(start);
	vex->push_back(end);
	geometry->setVertexArray(vex);

	//������ɫ����
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

	//colors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0));  // ��ɫ
	colors->push_back(color);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::PrimitiveSet> primitiveSet = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2);
	geometry->addPrimitiveSet(primitiveSet);

	//�����߿�
	osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(width);
	geometry->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);
	geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // �ص�����

	geode->addDrawable(geometry);
	return geode;
}

// @ funtion:    --- by lt(man) 2019/6/3 10:57
osg::ref_ptr<osg::Node> OsgDrawEnt::createPoint(const osg::Vec3 &pos, double width /*= 1.0*/, const osg::Vec4 color /*= osg::Vec4(1.0, 1.0, 1.0, 1)*/)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;


	geode->setInitialBound(osg::BoundingSphere(osg::Vec3(), 1.0));
	//���ö�������
	osg::ref_ptr<osg::Vec3Array> vex = new osg::Vec3Array;
	vex->push_back(pos);
	geometry->setVertexArray(vex);


	//������ɫ����
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

	//colors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0));  // ��ɫ
	colors->push_back(color);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::PrimitiveSet> primitiveSet = new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, 1);
	geometry->addPrimitiveSet(primitiveSet);

	//���ô�С
	osg::ref_ptr<osg::Point> pointSize = new osg::Point;
	pointSize->setSize(width);
	geometry->getOrCreateStateSet()->setAttribute(pointSize, osg::StateAttribute::ON);
	geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // �ص�����

	geode->addDrawable(geometry);
	return geode;
}
// @ funtion:    --- by lt(man) 2019/6/13 17:50
osg::ref_ptr<osg::Node> OsgDrawEnt::createCircle(const osg::Vec3 &center, double radius, double width /*= 1.0*/, const osg::Vec4 color /*= osg::Vec4(1.0, 1.0, 1.0, 1)*/)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

	//geode->setInitialBound(osg::BoundingSphere(osg::Vec3(), 1.0));

	//���ö�������
	osg::ref_ptr<osg::Vec3Array> vex = new osg::Vec3Array;
	for (double i = 0.0; i < 6.28; i += 0.02) {
		vex->push_back(osg::Vec3(radius * sin(i) + center.x(), -0.0 + center.y(), radius * cos(i) + center.z()));
	}
	geometry->setVertexArray(vex);

	//������ɫ����
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

	//colors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0));  // ��ɫ
	colors->push_back(color);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::PrimitiveSet> primitiveSet = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vex->size());
	geometry->addPrimitiveSet(primitiveSet);

	//�����߿�
	osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(width);
	geometry->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);
	geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // �ص�����

	geode->addDrawable(geometry);
	return geode;
}

// @ funtion:    --- by lt 2019/5/22 15:47
osg::ref_ptr<osg::Node> OsgDrawEnt::createCylinder(osg::Vec3 center,
	double Radius,
	double Height,
	osg::Vec4 xcolor /*= osg::Vec4(1.0, 1.0, 1.0, 1.0)*/,
	osg::Vec3f Axial /*= osg::Z_AXIS*/,
	osg::Vec3 vec /*= osg::Vec3(1.0, 1.0, 1.0)*/)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;

	osg::ref_ptr<osg::Cylinder> cy = new osg::Cylinder;
	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cy);
	cy->setCenter(center);
	cy->setHeight(Height);
	cy->setRadius(Radius);

	shape->setColor(xcolor);

	osg::Quat quat;
	quat.makeRotate(Axial, vec);
	cy->setRotation(quat);

	geode->addDrawable(shape.get());
	return geode;
}
// @ funtion:    --- by lt(man) 2019/6/17 16:40
osg::ref_ptr<osg::MatrixTransform> OsgDrawEnt::createCylinder(const osg::Vec3 &from, 
	const osg::Vec3 &to, 
	float radius)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	osg::ref_ptr<osg::ShapeDrawable> cylinder = new osg::ShapeDrawable();
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();

	hints->setDetailRatio(0.8f);
	osg::Vec3 cylCenter = (to + from) / 2;		//�õ�����
	float height = (to - from).length();		//�õ�����
	cylinder = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0, 0.0, 0.0), radius, height), hints.get());
	cylinder->setColor(osg::Vec4(255.0 / 255, 228.0 / 255, 181.0 / 255, 1.0f));			//¹ƤЬɫ
	geode->addDrawable(cylinder.get());
	osg::Matrix mRotate, mTrans;
	mRotate.makeRotate(osg::Vec3f(0.0f, 0.0f, 1.0f), to - from);
	mTrans.makeTranslate(cylCenter);
	mt->setMatrix(mRotate*mTrans);
	mt->addChild(geode.get());
	return mt.get();
}

// @ funtion:    --- by lt 2019/5/22 15:47
osg::ref_ptr<osg::Node> OsgDrawEnt::createCone(osg::Vec3 center,
	double Radius,
	double Height,
	osg::Vec4 xcolor /*= osg::Vec4(1.0, 1.0, 1.0, 1.0)*/,
	osg::Vec3f Axial /*= osg::Z_AXIS*/,
	osg::Vec3 vec /*= osg::Vec3(1.0, 1.0, 1.0)*/)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;

	osg::ref_ptr<osg::Cone> cone = new osg::Cone;
	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cone);
	cone->setCenter(center);
	cone->setHeight(Height);
	cone->setRadius(Radius);

	shape->setColor(xcolor);

	osg::Quat quat;
	quat.makeRotate(Axial, vec);
	cone->setRotation(quat);

	geode->addDrawable(shape);
	return geode;
}

osg::ref_ptr<osg::Node> OsgDrawEnt::createSpline(const std::vector<osg::Vec3> &posSet, double width /*= 1.0*/, const osg::Vec4 color /*= osg::Vec4(1.0, 1.0, 1.0, 1)*/)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

	geode->setInitialBound(osg::BoundingSphere(osg::Vec3(), 1.0));
	//���ö�������
	osg::ref_ptr<osg::Vec3Array> vex = new osg::Vec3Array;
	for (int i = 0; i < posSet.size(); i++)
	{
		vex->push_back(posSet[i]);
	}
	geometry->setVertexArray(vex);

	//������ɫ����
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

	//colors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0));  // ��ɫ
	colors->push_back(color);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::PrimitiveSet> primitiveSet = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, posSet.size());
	geometry->addPrimitiveSet(primitiveSet);

	//���ô�С
	// 	osg::ref_ptr<osg::Point> pointSize = new osg::Point;
	// 	pointSize->setSize(width);
	//	geometry->getOrCreateStateSet()->setAttribute(pointSize, osg::StateAttribute::ON);
	geometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // �ص�����
	geode->addDrawable(geometry);
	return geode;
}

void unicodeToUTF8(const std::wstring& src, std::string& result)
{
	char*  pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	result = pElementText;
	delete[] pElementText;
}

void gb2312ToUnicode(const std::string& src, std::wstring& result)
{
	int n = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	result.resize(n);
	::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
}

void gb2312ToUtf8(const std::string& src, std::string& result)
{
	std::wstring strWideChar;
	gb2312ToUnicode(src, strWideChar);
	unicodeToUTF8(strWideChar, result);
}

//@function: ��������
osg::ref_ptr<osg::Node> OsgDrawEnt::createText(const std::string& text,
	const osg::Vec3& possition,
	double size /*= 1.0*/,
	const osg::Vec4& color /*= osg::Vec4(0.0, 0.0, 1.0, 1.0)*/)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;

	osg::ref_ptr<osgText::Text> textItem = new osgText::Text;

	textItem->setFont(osgText::readFontFile(".//simsun.ttc")); // ����
	std::string strWideText; // ���ַ�
	gb2312ToUtf8(text, strWideText);
	textItem->setText(strWideText, osgText::String::ENCODING_UTF8);
	textItem->setPosition(possition);

	textItem->setAxisAlignment(osgText::Text::SCREEN); // ���ַ��򣺳����ӿڣ�ȱʡΪ���� Z ��ˮƽ������ XY ƽ����
	textItem->setAlignment(osgText::Text::CENTER_CENTER); // ����ê��������ֵ�λ��,ȱʡ�������ֵ���߽�ˮƽ����

	textItem->setCharacterSize(size); // ���ִ�С

	textItem->setColor(color); // ��ɫ

	geode->addDrawable(textItem);

	return geode;
}
// @ funtion:    --- by lt 2019/7/11 14:17
osg::ref_ptr<osg::Camera> OsgDrawEnt::createHUD(const osg::Vec3& pos, const std::string& tStr, const osg::Vec4& color/*, double fontSize*/)
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setName("static_text");
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1280, 0, 1024));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setFont("simsun.ttc");
	text->setPosition(pos);
	std::string strWideText; // ���ַ�
	gb2312ToUtf8(tStr, strWideText);
	text->setText(strWideText, osgText::String::ENCODING_UTF8);
	text->setColor(color);

	//text->setCharacterSize(fontSize);
	geode->addDrawable(text);
	camera->addChild(geode);
	return camera;
}

// @ funtion:    --- by lt(man) 2019/6/17 16:57
// osg::ref_ptr<osg::MatrixTransform> OsgDrawEnt::createRoundTable(const osg::Vec3 &center, float radiusUp, float radiusDown)
// {
// 	
// }
