/****************************************************************/
//Author: 	马梓淇修改，李涛创建
//Version: 	1.0
//Date:		2019.12.04
//Description:	绘制坐标系
/****************************************************************/

#pragma once
#include <osg/Node>
#include <vector>
#include "OsgDrawEnt.h"
using std::vector;
class CoordSystem:public OsgDrawEnt
{
public:
	CoordSystem();
	~CoordSystem();
	void updateEntity(double radio);//设置坐标系箭头尺寸
	void draw(osg::ref_ptr<osg::Group> root);//绘制坐标系

private:
	void initEntity();//

private:
	double _AxialR;//直径
	double _AxialLen;//长度
	double _AxialLenR;//箭头长度

	osg::ref_ptr<osg::Node> quadraticX;
	osg::ref_ptr<osg::Node> quadraticXr;
	osg::ref_ptr<osg::Node> quadraticY;
	osg::ref_ptr<osg::Node> quadraticYr;
	osg::ref_ptr<osg::Node> quadraticZ;
	osg::ref_ptr<osg::Node> quadraticZr;

	osg::Vec4 XAxialColor;
	osg::Vec4 YAxialColor;
	osg::Vec4 ZAxialColor;
};