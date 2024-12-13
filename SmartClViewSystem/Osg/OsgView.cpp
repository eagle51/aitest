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
		//�����ɫ����Ȼ��棬����ζ����������Ⱦ�ӳ���ʱ���Ḳ��֮ǰ�κ��������Ⱦ����
		getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//����ģ�͵İ�Χ���С�����������ͶӰ����
		double viewDistance = 2.0 * bs.radius();
		double znear = viewDistance - bs.radius();
		double zfar = viewDistance + bs.radius();
		float top = bs.radius();
		float right = bs.radius();

		getCamera()->setProjectionMatrixAsOrtho(-right, right, -top, top, znear, zfar); // ��������ͶӰ
		//����Ϊ����,���Ƕ�Ӧ�۾����ķ����˵�ͷ�����ϵķ���
		osg::Vec3d upDirection(0.0, 1.0, 0.0);
		//�����۾�����λ�ã�������ķ���
		osg::Vec3d viewDirection(0.0, 0.0, 1.0);

		//���������λ��
		osg::Vec3d center = bs.center();

		//�����������ڵ�λ��
		osg::Vec3d eyePoint = center + viewDirection * viewDistance;

		//�ӵ�任����
		getCamera()->setViewMatrixAsLookAt(eyePoint, center, upDirection);
	}
}

void QtOsgView::paintGL()
{	
	//QThread::msleep(10);//����10ms���ӳ٣�����cpuռ��
	frame();
	update();//�ݹ����
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
	traceBall->SetRotateCenter(GetCenter()/*���л�ͼʵ��߽緶Χ������*/);
}

// @ funtion:  ˢ�½�����ʾ  --- by mzq 2019/5/22 17:55
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

// @ funtion: ��ȡ�߽緶Χ --- by lt 2019/5/23 17:26
osg::BoundingSphere QtOsgView::getBoundingSphere()
{
	// ���б߽�� �����ʱ���ܿ���������
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