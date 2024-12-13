/****************************************************************/
//Author: 	������޸ģ����δ���
//Version: 	1.0
//Date:		2019.12.04
//Description:	��������ϵ
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
	void updateEntity(double radio);//��������ϵ��ͷ�ߴ�
	void draw(osg::ref_ptr<osg::Group> root);//��������ϵ

private:
	void initEntity();//

private:
	double _AxialR;//ֱ��
	double _AxialLen;//����
	double _AxialLenR;//��ͷ����

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