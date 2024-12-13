#include "OsgView.h"
#include "ElemArray.h"
#include "TowerModel.h"
#include "Box3D.h"
#include <osg\LineWidth>
#include <osg\Point>
#include <osg\ComputeBoundsVisitor>
#include "OsgUIManager.h"
#include "qthread.h"

using namespace SmartTower;
QtOsgView::QtOsgView(QSPtr_OsgUIManager osgUIManager
					 ,QWidget * parent
					 , const char * name
					 , const QGLWidget * shareWidget
					 , Qt::WindowFlags f)
					 :_osgUIManager(osgUIManager),
					 AdapterWidget(parent, name, shareWidget, f)
{
	getCamera()->setViewport(new osg::Viewport(0, 0, width(), height()));
	
	int iww = width();
	int ihh = height();
	double width, height;

	float m_curFactor = 0.5f;

	width = iww / m_curFactor;
	height = ihh / m_curFactor;

	getCamera()->setProjectionMatrixAsOrtho(-width * 0.1, width * 0.1, -height * 0.1, height * 0.1, 1.0f, 1000.0f);
	setProjectionMatrixAsOrtho();
	getCamera()->setGraphicsContext(getGraphicsWindow());
	setThreadingModel(osgViewer::Viewer::SingleThreaded);
}

void QtOsgView::setProjectionMatrixAsOrtho()
{
	osg::Node* model = getSceneData();
	if (model!=NULL){
		const osg::BoundingSphere& bs = model->getBound();
		//清除颜色和深度缓存，这意味着这个相机渲染子场景时将会覆盖之前任何相机的渲染数据
		getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//根据模型的包围球大小，设置相机的投影矩阵
		double viewDistance = 2.0 * bs.radius();
		double znear = viewDistance - bs.radius();
		double zfar = viewDistance + bs.radius();
		float top = bs.radius();
		float right = bs.radius();

		getCamera()->setProjectionMatrixAsOrtho(-right, right, -top, top, znear, zfar); // 设置正交投影
		//设置为正看,就是对应眼睛看的方向，人的头的向上的方向。
		osg::Vec3d upDirection(0.0, 1.0, 0.0);
		//设置眼睛所在位置，看物体的方向
		osg::Vec3d viewDirection(0.0, 0.0, 1.0);

		//设置物体的位置
		osg::Vec3d center = bs.center();

		//设置人眼所在的位置
		osg::Vec3d eyePoint = center + viewDirection * viewDistance;

		//视点变换函数
		getCamera()->setViewMatrixAsLookAt(eyePoint, center, upDirection);
	}
}

void QtOsgView::paintGL()
{	
	//QThread::msleep(10);//设置10ms的延迟，降低cpu占用
	frame();
	update();//递归调用
}

// @ funtion:    --- by lt 2019/5/22 17:06
void QtOsgView::resizeGL(int width, int height)
{
	AdapterWidget::resizeGL(width, height);
}

// @ funtion:    --- by lt 2019/5/22 17:14
void QtOsgView::setRotateCenterFromBox()
{
	osg::ref_ptr<COsgMainpulator> traceBall = static_cast<COsgMainpulator*>(getCameraManipulator());
	traceBall->SetRotateCenter(GetCenter()/*所有绘图实体边界范围的中心*/);
}

// @ funtion:  刷新界面显示  --- by mzq 2019/5/22 17:55
void QtOsgView::updateView()
{
	_root = new osg::Group();
	std::vector<osg::ref_ptr<osg::Node>> modelspace = _osgUIManager->getmodelspace();
	for (size_t i = 0; i < modelspace.size(); i++)
		_root->addChild(modelspace[i].get());
	_coorSystem = new CoordSystem();
	_coorSystem->draw(_root);
	setSceneData(_root.get());
	realize(); 
	setRotateCenterFromBox();
}

// @ funtion:    --- by lt 2019/5/23 16:32
double QtOsgView::GetDiameter()
{
	double dist = 2.0f * getBoundingSphere().radius();
	return dist;
}

// @ funtion: 获取边界范围 --- by lt 2019/5/23 17:26
osg::BoundingSphere QtOsgView::getBoundingSphere()
{
	// 所有边界盒 计算的时候不能考虑坐标轴
	osg::BoundingSphere boundingSphere;

	osg::BoundingBox bbAll;
	unsigned int uNum = _root->getNumChildren();
	unsigned int i = 0;
	for (i = 0; i < uNum - 1; i++)
	{
		osg::Node *pNode1 = _root->getChild(i);
		if (pNode1)
		{
			const std::string& name = pNode1->getName();
			if (("Axis") != name
				&& ("SelectRect") != name
				&& ("Background") != name
				&& ("static_text") != name)
			{
				osg::ComputeBoundsVisitor cbVisitor;
				pNode1->accept(cbVisitor);
				osg::BoundingBox &bb = cbVisitor.getBoundingBox();
				bbAll.expandBy(bb);
			}
		}
	}

	if (bbAll.valid()) 
		boundingSphere.expandBy(bbAll);
	else 
		boundingSphere = _root->getBound();

	return boundingSphere;	// set dist to default
}

// @ funtion:    --- by lt 2019/5/23 17:29
osg::Vec3f QtOsgView::GetCenter()
{
	return getBoundingSphere().center();
}

// @ funtion:    --- by lt(man) 2019/6/24 11:05
void QtOsgView::setRotateCenter(osg::Vec3 pt)
{
	osg::ref_ptr<COsgMainpulator> traceBall = static_cast<COsgMainpulator*>(getCameraManipulator());
	traceBall->SetRotateCenter(pt);
}