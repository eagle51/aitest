/**************************************************************************
Copyright:陕西恒巨软件科技有限公司
Author: fqw
Date: $time$
Description: functions  
**************************************************************************/
#pragma once
#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <QOpenGLWidget>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <QString>
#include <QKeyEvent>
#include <QMouseEvent>  
#include <QWheelEvent>
#include <iostream>

class AdapterWidget :public QOpenGLWidget
{
	Q_OBJECT;
public:
	AdapterWidget(QWidget *parent = 0, const char* name = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
	virtual ~AdapterWidget(){}

	osgViewer::GraphicsWindow* getGraphicsWindow() { return mGw.get();}

	const osgViewer::GraphicsWindow* getGraphicsWidow()const { return mGw.get();}

protected:
	virtual void resizeGL(int width, int height);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGw;
};