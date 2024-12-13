/**************************************************************************
Copyright:陕西恒巨软件科技有限公司
Author: fqw
Date: $time$
Description: functions  
**************************************************************************/
#pragma once

#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <QObject>
#include <osg/Vec3>
#include "OsgUIManager.h"

typedef osgUtil::PolytopeIntersector::Intersections Inters;
class PickHandler :public QObject, public osgGA::GUIEventHandler
{
	Q_OBJECT

public:
	PickHandler(QSPtr_OsgUIManager osgUIManager);
	~PickHandler(){}

protected:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void pick(osgViewer::View* view, double x, double y);
	bool hasPoint(Inters inters);

private:
	QSPtr_OsgUIManager  _osgUIManager;//Osg绘图管理类

	double			    _choseX; // 选中的
	double			    _choseY;

};