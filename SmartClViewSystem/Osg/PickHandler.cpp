#include "PickHandler.h"
#include <osgUtil/LineSegmentIntersector>
#include <osgFX/Scribe>
#include <osg/GraphicsContext>
#include <osg/Point>
#include "OsgUIManager.h"


//@function:  
PickHandler::PickHandler(QSPtr_OsgUIManager osgUIManager)
	:_osgUIManager(osgUIManager),
	_choseX(0), 
	_choseY(0)
{

}

//@function:  
bool PickHandler::handle(const osgGA::GUIEventAdapter& ea, 
						 osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (!view)return false;

 	switch (ea.getEventType())
 	{
 	case(osgGA::GUIEventAdapter::PUSH): //单击
		_choseX = ea.getX();
 		_choseY = ea.getY();
 		break;
 	case (osgGA::GUIEventAdapter::RELEASE) :
 		if (abs(_choseX - ea.getX()) < 0.1&&abs(_choseY - ea.getY()) < 0.1)
 			pick(view, ea.getX(), ea.getY()); // 执行对象选取
 		break;
 	default:
 		break;
 	}
 	return false;
}

//@function: pick
void PickHandler::pick(osgViewer::View* view, double x, double y)
{
	const osg::GraphicsContext::Traits* trait = view->getCamera()->getGraphicsContext()->getTraits();

	double w(4), h(4);
	osg::ref_ptr<osgUtil::PolytopeIntersector> picker;
	picker = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, x - w, y - h, x + w, y + h);

	osgUtil::IntersectionVisitor iv(picker);
	view->getCamera()->accept(iv);

	if (picker->containsIntersections()){
		
		osg::ref_ptr<osg::Node> 	node = 0;
		osg::ref_ptr<osg::Group>	parent = 0;

		bool hasPt = hasPoint(picker->getIntersections());

		for (Inters::iterator itr = picker->getIntersections().begin(); itr != picker->getIntersections().end(); ++itr){
			osg::NodePath nodePath = itr->nodePath;

			node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
			parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
			osg::ref_ptr<osg::Geometry> geometry = node->asGeode()->getDrawable(0)->asGeometry();
			if (hasPt && geometry && geometry->getPrimitiveSet(0)->getMode() != osg::PrimitiveSet::POINTS)
				continue;
			// 高亮显示选中物体
			if (parent.get() && node.get()){
				osg::ref_ptr<osgFX::Scribe> parentAsSc = dynamic_cast<osgFX::Scribe*>(parent.get());

				if (!parentAsSc){ // 选中对象，高亮显示
					
					// 添加线框
					osg::ref_ptr<osgFX::Scribe> sc = new osgFX::Scribe();
					sc->setWireframeLineWidth(3);
					sc->setWireframeColor(osg::Vec4(0.0, 1.0, 0.0, 0.2));
					sc->addChild(node.get());

					if (geometry && geometry->getPrimitiveSet(0)->getMode() == osg::PrimitiveSet::POINTS)
 					{
						osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
						colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1));
						geometry->setColorArray(colors);
						geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

						osg::ref_ptr<osg::Point> pointSize = new osg::Point;
						pointSize->setSize(6.0);
						geometry->getOrCreateStateSet()->setAttribute(pointSize, osg::StateAttribute::ON);
						osg::Vec3Array *vec3Arr = (osg::Vec3Array*)geometry->getVertexArray();
						

						if (_osgUIManager->getMode() == Mode::NORMAL){
							_osgUIManager->updateRotateCenter(vec3Arr->at(0));
						}		
						else if (_osgUIManager->getMode() == Mode::FITTING){

 						}
						else if (_osgUIManager->getMode() == Mode::MEASURING){
							
 						}	
						else if (_osgUIManager->getMode() == Mode::SELECTPOINT)
						{
							int index = _osgUIManager->getNodeNumber(vec3Arr->at(0));
							emit _osgUIManager->connectPoint(index, vec3Arr->at(0));//连接点选中
						}
						_osgUIManager->clearNoSelectPoint(vec3Arr->at(0));
					}
					
					parent->replaceChild(node.get(), sc.get());
					break;
				}
				else{ // 未选中，移除高亮显示的对象
					osg::Node::ParentList parentList = parentAsSc->getParents();
					for (osg::Node::ParentList::iterator itr = parentList.begin(); itr != parentList.end(); ++itr){
						(*itr)->replaceChild(parentAsSc.get(), node.get());
					}

					if (geometry && geometry->getPrimitiveSet(0)->getMode() == osg::PrimitiveSet::POINTS)
 					{
						osg::ref_ptr<osg::Geometry> geometry = node->asGeode()->getDrawable(0)->asGeometry();
						osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
						colors->push_back(osg::Vec4(1.0, 1.0, 1.0, 1));
						geometry->setColorArray(colors);
						geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

						osg::ref_ptr<osg::Point> pointSize = new osg::Point;
						pointSize->setSize(1.0);
						geometry->getOrCreateStateSet()->setAttribute(pointSize, osg::StateAttribute::ON);
					}
					break;
				}
			}
		}
	}
}
// @ funtion:    --- by lt(man) 2019/6/3 14:31
bool PickHandler::hasPoint(Inters inters)
{
	osg::ref_ptr<osg::Node> 	node = 0;
	osg::ref_ptr<osg::Group>	parent = 0;
	bool res = false;
	for (Inters::iterator itr = inters.begin(); itr != inters.end(); ++itr){
		osg::NodePath nodePath = itr->nodePath;
		node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
		parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
		// 高亮显示选中物体
		if (parent.get() && node.get()){
			osg::ref_ptr<osg::Geometry> geometry = node->asGeode()->getDrawable(0)->asGeometry();
			if (!geometry)
				return false;
			if (geometry->getPrimitiveSet(0)->getMode() == osg::PrimitiveSet::POINTS)
				res = true;
		}
	}
	return res;
}


