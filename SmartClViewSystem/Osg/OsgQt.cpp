#include "OsgQt.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <osg/LineWidth>
#include <osg/Group>
#include <osgFX/Scribe>
#include <osgText/Text>
#include "OsgView.h"
#include "PickHandler.h"
#include <osg/ref_ptr>
#include <osg/ShapeDrawable>
#include <osg/ComputeBoundsVisitor>
#include "CoordSystem.h"
#include "OsgMainpulator.h"
#include "OsgUIManager.h"
#include "qmessagebox.h"
#include <QShortcut>

OsgQt::OsgQt(QSPtr_OsgUIManager osgUIManager, QWidget *parent) : _osgUIManager(osgUIManager), QWidget(parent)
{
	this->resize(600, 400);
	
	_osgViewer = new QtOsgView(_osgUIManager);
 	osg::ref_ptr<osgGA::TrackballManipulator> trackBall = new osgGA::TrackballManipulator;
	osg::ref_ptr<COsgMainpulator> ball = new COsgMainpulator(_osgViewer->getCamera());
	_osgViewer->m_trackball = ball;
	_osgViewer->setCameraManipulator(ball.release());
	_osgViewer->addEventHandler(new osgViewer::WindowSizeHandler);
	_osgViewer->addEventHandler(new osgViewer::StatsHandler);
	_osgViewer->addEventHandler(new PickHandler(_osgUIManager));//为场景加入事件处理功能，并将场景本身传入到事件内部
	_osgViewer->realize();
	_osgViewer->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 0)); // 黑色底
	_osgViewer->getCamera()->setAllowEventFocus(false);

	this->createHToolBar();
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(_hToolBar);
	layout->addWidget(_osgViewer);
	layout->setMargin(0);
	this->setLayout(layout);

	new QShortcut(Qt::Key_Escape, this, SLOT(slotEscCommand()));//设置键盘esc快捷键的操作
	connect(_osgUIManager.data(), SIGNAL(connectPoint(int, osg::Vec3)), this, SLOT(slotSelectFinished(int, osg::Vec3)));
	connect(_osgUIManager.data(), SIGNAL(measureFinished()), this, SLOT(slotMeasureFinished()));
	connect(_osgUIManager.data(), SIGNAL(updateRotateCenter(osg::Vec3)), this, SLOT(slotUpdateRotateCenter(osg::Vec3)));
	connect(_osgUIManager.data(), SIGNAL(setRotateFromCenter()), this, SLOT(slotsetRotateFromCenter()));
	connect(_osgUIManager.data(), SIGNAL(addNode(osg::ref_ptr<osg::Node>)), this, SLOT(slotAddNode(osg::ref_ptr<osg::Node>)));
	connect(_osgUIManager.data(), SIGNAL(removeNode(osg::ref_ptr<osg::Node>)), this, SLOT(slotRemoveNode(osg::ref_ptr<osg::Node>)));
}

OsgQt::~OsgQt()
{

}
// @ function: 水平工具栏   -- by lt 2019/4/27 11:29
void OsgQt::createHToolBar()
{
	_hToolBar = new QToolBar(tr("tool"));
	_hToolBar->addSeparator();

	_aLLsAct = new QAction(tr("All"));
	_aLLsAct->setToolTip(tr("All"));
	_aLLsAct->setIcon(QIcon(":images/osg/ALLs.png"));

	_frontAct = new QAction(tr("Front"));
	_frontAct->setToolTip(tr("Front"));
	_frontAct->setIcon(QIcon(":images/osg/front.png"));

	_backAct = new QAction(tr("Back"));
	_backAct->setToolTip(tr("Back"));
	_backAct->setIcon(QIcon(":images/osg/back.png"));

	_leftAct = new QAction(tr("Left"));
	_leftAct->setToolTip(tr("Left"));
	_leftAct->setIcon(QIcon(":images/osg/left.png"));

	_rightAct = new QAction(tr("Right"));
	_rightAct->setToolTip(tr("Right"));
	_rightAct->setIcon(QIcon(":images/osg/right.png"));

	_topAct = new QAction(tr("Top"));
	_topAct->setToolTip(tr("Top"));
	_topAct->setIcon(QIcon(":images/osg/top.png"));

	_bottomAct = new QAction(tr("Bottom"));
	_bottomAct->setToolTip(tr("Bottom"));
	_bottomAct->setIcon(QIcon(":images/osg/bottom.png"));

	_reversalAct = new QAction(QString::fromLocal8Bit("翻转模型")); // 切换转向
	_reversalAct->setIcon(QIcon(":images/osg/Reversal.png"));

	_selectPoint = new QAction(QString::fromLocal8Bit("捕捉绝缘子串挂点")); // 捕捉挂点
	_selectPoint->setIcon(QIcon(":images/osg/select.png"));

	//_measuringAct = new QAction(tr(""));
	//_measuringAct->setIcon(QIcon(":images/osg/measuring.png"));


	_hToolBar->addAction(_aLLsAct);
	_hToolBar->addAction(_frontAct);
	_hToolBar->addAction(_backAct);
	_hToolBar->addAction(_leftAct);
	_hToolBar->addAction(_rightAct);
	_hToolBar->addAction(_topAct);
	_hToolBar->addAction(_bottomAct);
	_hToolBar->addSeparator();
	_hToolBar->addAction(_reversalAct);
	_hToolBar->addAction(_selectPoint);
	//_hToolBar->addAction(_measuringAct);

	connect(_aLLsAct, SIGNAL(triggered()), this, SLOT(slotAllsAct()));
	connect(_frontAct, SIGNAL(triggered()), this, SLOT(slotFrontAct()));
	connect(_backAct, SIGNAL(triggered()), this, SLOT(slotBackAct()));
	connect(_leftAct, SIGNAL(triggered()), this, SLOT(slotLeftAct()));
	connect(_rightAct, SIGNAL(triggered()), this, SLOT(slotRightAct()));
	connect(_topAct, SIGNAL(triggered()), this, SLOT(slotTopAct()));
	connect(_bottomAct, SIGNAL(triggered()), this, SLOT(slotBottomAct()));
	connect(_reversalAct, SIGNAL(triggered()), this, SLOT(slotReversal()));
	connect(_selectPoint, SIGNAL(triggered()), this, SLOT(slotSelectPoint()));
	//connect(_measuringAct, SIGNAL(triggered()), this, SLOT(slotMeasuringAct()));
}

void OsgQt::updateView()
{
	_osgViewer->updateView();
}

// @ function:      -- by lt 2019/4/29 19:23
void OsgQt::slotAllsAct()
{
	setViewDiriction(Front); // home默认视图
}
// @ function:      -- by lt 2019/4/29 19:26
void OsgQt::slotFrontAct()
{
	setViewDiriction(Front);
}
// @ function:      -- by lt 2019/4/29 19:28
void OsgQt::slotBackAct()
{
	setViewDiriction(Back);
}
// @ function:      -- by lt 2019/4/29 19:35
void OsgQt::slotLeftAct()
{
	setViewDiriction(Left);
}
// @ function:      -- by lt 2019/4/29 19:35
void OsgQt::slotRightAct()
{
	setViewDiriction(Right);
}
// @ function:       -- by lt 2019/4/29 19:35
void OsgQt::slotTopAct()
{
	setViewDiriction(Top);
}
// @ function:      -- by lt 2019/4/29 19:35
void OsgQt::slotBottomAct()
{
	setViewDiriction(Bottom);
}

//@function: set view direction   -- by fqw 2019/4/30 10:16
void OsgQt::setViewDiriction(Direction direction)
{
	osg::ref_ptr<COsgMainpulator> traceBall = static_cast<COsgMainpulator*>(_osgViewer->getCameraManipulator());
	_osgViewer->home();
	osg::Vec3d eye, center, up;
	traceBall->SetRotateCenter(_osgViewer->GetCenter());
	center = traceBall->GetRotateCenter();
	switch (direction)
	{
	case OsgQt::Front:// home默认为这个视图
		eye = center - osg::Y_AXIS * _osgViewer->GetDiameter();
		break;
 	case OsgQt::Back:
		eye = center + osg::Y_AXIS * _osgViewer->GetDiameter();
 		break;
 	case OsgQt::Left:
		eye = center - osg::X_AXIS * _osgViewer->GetDiameter();
 		break;
 	case OsgQt::Right:
		eye = center + osg::X_AXIS * _osgViewer->GetDiameter();
 		break;
 	case OsgQt::Top:
		eye = center + osg::Z_AXIS * _osgViewer->GetDiameter();
 		break;
 	case OsgQt::Bottom:
		eye = center - osg::Z_AXIS * _osgViewer->GetDiameter();
 		break;
	default:
		break;
	}
	up = osg::Z_AXIS;
	traceBall->setTransformation(eye, center, up);
}

// @ funtion: 翻转模型   --- by lt 2019/6/12 11:56
void OsgQt::slotReversal()
{
	_osgUIManager->reverseTowerModel();
	_osgUIManager->loadTowerModel();
	updateView();
	setViewDiriction(Front);

	emit reverseHangPoint();//翻转挂点
}

// @ funtion:    --- by lt 2019/6/21 15:12
void OsgQt::slotSelectPoint()
{
	this->setCursor(Qt::CrossCursor);
	_osgUIManager->setMode(Mode::SELECTPOINT);
}
// @ funtion:    --- by lt 2019/6/21 16:05
void OsgQt::slotSelectFinished(int, osg::Vec3 pt)
{
	this->setCursor(Qt::ArrowCursor);
	_osgUIManager->setMode(Mode::NORMAL);
}
// @ funtion:    --- by lt 2019/6/24 11:32
void OsgQt::slotUpdateRotateCenter(osg::Vec3 pt)
{
	_osgViewer->setRotateCenter(pt);
}
// @ funtion: 设置旋转中心   --- by lt 2019/6/24 14:32
void OsgQt::slotsetRotateFromCenter()
{
	_osgViewer->setRotateCenterFromBox();
	setViewDiriction(Front);
}
// @ funtion:  取消操作  --- by lt 2019/6/24 15:06
void OsgQt::slotEscCommand()
{
	_osgViewer->setRotateCenterFromBox();
	_osgUIManager->clearNoSelectPoint(osg::Vec3(0, 0, 0), false);
}
// @ funtion:    --- by lt 2019/6/24 15:17
void OsgQt::slotAddNode(osg::ref_ptr<osg::Node> node)
{
	_osgViewer->addNode(node);
}
// @ funtion:    --- by lt 2019/7/5 11:51
void OsgQt::slotRemoveNode(osg::ref_ptr<osg::Node> node)
{
	_osgViewer->removeNode(node);
}
// @ funtion:    --- by lt 2019/8/21 10:48
void OsgQt::slotMeasuringAct()
{
	this->setCursor(Qt::CrossCursor);
	_osgUIManager->setMode(Mode::MEASURING);
}
// @ funtion:    --- by lt 2019/9/6 11:42
void OsgQt::slotMeasureFinished()
{
	this->setCursor(Qt::ArrowCursor);
	_osgUIManager->setMode(Mode::NORMAL);
}