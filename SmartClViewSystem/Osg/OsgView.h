/**************************************************************************
Copyright:陕西恒巨软件科技有限公司
Author: fqw
Date: $time$
Description: functions  
**************************************************************************/
#pragma once
#include "AdapterWidget.h"
#include "CoordSystem.h"
#include "OsgMainpulator.h"
#include "OsgUIManager.h"

class QtOsgView : public osgViewer::Viewer, public AdapterWidget
{
public:
	QtOsgView(QSPtr_OsgUIManager osgUIManager, 
			  QWidget* parent = 0,
			  const char * name = 0, 
			  const QGLWidget * shareWidget = 0,
			  Qt::WindowFlags f = 0);

	~QtOsgView(){}

	virtual void paintGL();

	void addNode(osg::ref_ptr<osg::Node> node){ _root->addChild(node.get()); }
	void removeNode(osg::ref_ptr<osg::Node> node){ _root->removeChild(node.get()); }

	void updateView();//刷新界面显示

	void setProjectionMatrixAsOrtho();

	double GetDiameter();
	osg::Vec3f GetCenter();
	void setRotateCenter(osg::Vec3 pt);
	void setRotateCenterFromBox();

protected:
	virtual void resizeGL(int width, int height);

private:
	osg::BoundingSphere getBoundingSphere();//获取边界范围

public:
	osg::ref_ptr<COsgMainpulator> m_trackball;

private:
	QSPtr_OsgUIManager           _osgUIManager;//Osg绘图管理类

	CoordSystem                 *_coorSystem;//坐标系
	osg::ref_ptr<osg::Group>     _root;//所有包含的内容
};