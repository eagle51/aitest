/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.12.9
//Description:	osg显示widget
/****************************************************************/
#pragma once
#include <QDockWidget>
#include <osg/Node>
#include <string>
#include "TowerModel.h"
#include <osg/Vec3>
#include <osgViewer/ViewerEventHandlers>
#include "OsgDrawEnt.h"
#include "qcheckbox.h"
#include "OsgUIManager.h"

QT_BEGIN_NAMESPACE
class QAction;
class QToolBar;
QT_END_NAMESPACE

using namespace SmartTower;
class QtOsgView;
class OsgQt : public QWidget
{
	Q_OBJECT
public:
	enum Direction{ Front, Back, Left, Right, Top, Bottom };

public:
	OsgQt(QSPtr_OsgUIManager osgUIManager, QWidget *parent = 0);
	~OsgQt();
	void updateView();

private:
	void createHToolBar();//水平工具栏
	void setViewDiriction(Direction direction);

signals:
	void reverseHangPoint();//当翻转塔模型时，挂点参数也跟随翻转

private slots:
	void slotEscCommand(); //取消操作
	void slotAllsAct();
	void slotFrontAct();
	void slotBackAct();
	void slotLeftAct();
	void slotRightAct();
	void slotTopAct();
	void slotBottomAct();
	void slotReversal(); //翻转模型
	void slotSelectPoint(); //连接点
	void slotMeasuringAct();
	void slotSelectFinished(int, osg::Vec3);
	void slotMeasureFinished();
	void slotUpdateRotateCenter(osg::Vec3 pt);//刷新旋转中心点
	void slotsetRotateFromCenter();//设置旋转中心
	void slotAddNode(osg::ref_ptr<osg::Node> node);
	void slotRemoveNode(osg::ref_ptr<osg::Node> node);

private:
	QSPtr_OsgUIManager  _osgUIManager;//Osg绘图管理类

	QtOsgView          *_osgViewer;
	QToolBar           *_hToolBar;//水平工具栏

	QAction            *_aLLsAct;
	QAction            *_backAct;
	QAction            *_bottomAct;
	QAction            *_frontAct;
	QAction            *_leftAct;
	QAction            *_rightAct;
	QAction            *_topAct;
	QAction            *_reversalAct;

	//QAction            *_measuringAct;
	QAction            *_selectPoint;
};

