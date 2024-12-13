#include "OsgUIManager.h"
#include "ElemArray.h"
#include "TowerModel.h"
#include "Box3D.h"
#include "Node.h"
#include <osg\Geode>
#include <osg\Geometry>
#include <osg\Point>
#include "DataBase.h"
#include "Auxiliary.h"
#include "EnumLib.h"
#include "Consts.h"
#include "measureUtil.h"
#include "qmessagebox.h"
#include "J3DModelData.h"
#include "DataUI_MaxAngle.h"
#include "PointArray.h"
#include "ModelDateBase.h"

using namespace SmartTower;
using SmartCl::Consts; 
using SmartCl::DataBase;
using SmartCl::TowerElem;
using Measure::Util;

OsgUIManager::OsgUIManager(SmartCl::HdlCl cl)
: _cl(cl),
_curMode(Mode::NORMAL), 
_curSelectNodeNum(-1)
{
	_nodeSet.push_back(SuspensionNode(-1, osg::Vec3(0, 0, 0)));
}

OsgUIManager::~OsgUIManager()
{
}

// @ funtion: ��ӵ�ģ�Ϳռ�   --- by mzq 2019/6/13 15:13
int OsgUIManager::addToModelspace(osg::ref_ptr<osg::Node> node/*�ڵ�*/)
{
	_modelspace.push_back(node);
	emit addNode(node);
	
	return _modelspace.size() - 1;
}

//@function: ��ȡ����ģ�����ݵ���ǰ��Cl��   -- by mzq  2019/9/11 11:55
void OsgUIManager::writeTowerModelToCurrentCl(bool isSmartPole)
{
	HdlDataUI_MaxAngle dataUI_MaxAngle = static_cast<DataUI_MaxAngle*>(_cl->getDataUI().GetPointee());
	HdlJ3DModelData towerModelData = dataUI_MaxAngle->getTowerModelData();
	towerModelData->clearAll();  // ���

	if (isSmartPole)
	{
		SmartPoleCore::HdMemberArray pMemberArr = SmartPoleCore::ModelDateBase::Instance()->getMemberArray();
		SmartPoleCore::CBox3D poleBox = pMemberArr->GetBox();
		for (int iloop = 0; iloop < pMemberArr->count(); ++iloop)
		{
			SmartPoleCore::HandleMember handleElement = pMemberArr->getAt(iloop);
			SmartPoleCore::HandlePoint pStartNode = handleElement->getStartPt();
			SmartPoleCore::HandlePoint pEndNode = handleElement->getEndPt();

			SmartCl::HdlTowerElem newElem = new TowerElem;
			newElem->setStartNode(new SmartCl::JPoint(pStartNode->x(), pStartNode->y(), poleBox.MaxZ() - pStartNode->z(), pStartNode->TagNum()));
			newElem->setEndNode(new SmartCl::JPoint(pEndNode->x(), pEndNode->y(), poleBox.MaxZ() - pEndNode->z(), pEndNode->TagNum()));
			towerModelData->addTowerElem(newElem);
			towerModelData->addTowerNode(new SmartCl::JPoint(pStartNode->x(), pStartNode->y(), poleBox.MaxZ() - pStartNode->z(), pStartNode->TagNum()));
			towerModelData->addTowerNode(new SmartCl::JPoint(pEndNode->x(), pEndNode->y(), poleBox.MaxZ() - pEndNode->z(), pEndNode->TagNum()));
		}
	}
	else
	{
		SmartTower::HdElemArray pElmArray = SmartTower::CTowerModel::Instance()->getElemArray();
		SmartTower::CBox3D towerBox = SmartTower::CTowerModel::Instance()->GobalModelBox();
		for (int iloop = 0; iloop < pElmArray->size(); ++iloop)
		{
			HandleElement handleElement = pElmArray->at(iloop);
			SmartTower::HandleNode pStartNode = handleElement->StartNode();
			SmartTower::HandleNode pEndNode = handleElement->EndNode();

			SmartCl::HdlTowerElem newElem = new TowerElem;
			newElem->setStartNode(new SmartCl::JPoint(pStartNode->x(), pStartNode->y(), towerBox.MaxZ() - pStartNode->z(), pStartNode->TagNum()));
			newElem->setEndNode(new SmartCl::JPoint(pEndNode->x(), pEndNode->y(), towerBox.MaxZ() - pEndNode->z(), pEndNode->TagNum()));
			towerModelData->addTowerElem(newElem);
			towerModelData->addTowerNode(new SmartCl::JPoint(pStartNode->x(), pStartNode->y(), towerBox.MaxZ() - pStartNode->z(), pStartNode->TagNum()));
			towerModelData->addTowerNode(new SmartCl::JPoint(pEndNode->x(), pEndNode->y(), towerBox.MaxZ() - pEndNode->z(), pEndNode->TagNum()));
		}
	}
}

//@function: ��ת����ģ��   -- by mzq  2019/12/24 15:49
void OsgUIManager::reverseTowerModel()
{
	HdlDataUI_MaxAngle dataUI_MaxAngle = static_cast<DataUI_MaxAngle*>(_cl->getDataUI().GetPointee());
	HdlJ3DModelData towerModelData = dataUI_MaxAngle->getTowerModelData();
	if (towerModelData->sizeOfTowerModel() > 0)
	{
		/*��ת�˼�*/
		for (int iLoop = 0; iLoop < towerModelData->sizeOfTowerModel(); ++iLoop)
		{
			/*�����нڵ�*/
			SmartCl::HdlJPoint pStartNode = towerModelData->elemAt(iLoop)->getStartNode();
			pStartNode->setX(-pStartNode->getX());
			SmartCl::HdlJPoint pEndNode = towerModelData->elemAt(iLoop)->getEndNode();
			pEndNode->setX(-pEndNode->getX());
		}

		/*��ת�ڵ�*/
		for (int iLoop = 0; iLoop < towerModelData->sizeOfTowerNode(); ++iLoop)
		{
			SmartCl::HdlJPoint pNode = towerModelData->nodeAt(iLoop);
			pNode->setX(-pNode->getX());
		}

		/*��ת������*/
		for (int iLoop = 0; iLoop < towerModelData->sizeOfIsolatedPt(); ++iLoop)
		{
			SmartCl::HdlJPoint pIsolatedPt = towerModelData->isolatedPtAt(iLoop);
			pIsolatedPt->setX(-pIsolatedPt->getX());
		}
	}
}

// @ funtion: ������������   --- by mzq 2019/6/13 15:13
void OsgUIManager::loadTowerModel()
{	
	HdlDataUI_MaxAngle dataUI_MaxAngle = static_cast<DataUI_MaxAngle*>(_cl->getDataUI().GetPointee());
	HdlJ3DModelData towerModelData = dataUI_MaxAngle->getTowerModelData();

	if (towerModelData->sizeOfTowerModel() > 0)
	{
		clearAll();
		for (int iLoop = 0; iLoop < towerModelData->sizeOfTowerModel(); ++iLoop)
		{
			SmartCl::HdlJPoint pStartNode = towerModelData->elemAt(iLoop)->getStartNode();
			SmartCl::HdlJPoint pEndNode = towerModelData->elemAt(iLoop)->getEndNode();

			osg::ref_ptr<osg::Node> line = createLine(osg::Vec3(pStartNode->getX(), pStartNode->getY(), pStartNode->getZ()),
													  osg::Vec3(pEndNode->getX(), pEndNode->getY(), pEndNode->getZ()));
			addToModelspace(line);
		}
		
		//����
		addToModelspace(createLine(osg::Vec3(0, -8, 0), osg::Vec3(0, 8, 0), 2.0, osg::Vec4(0, 1, 0, 1)));
		addToModelspace(createText("ǰ", osg::Vec3(0, 10, 0), 2.0));
		addToModelspace(createText("��", osg::Vec3(0, -10, 0), 2.0));
		addToModelspace(createLine(osg::Vec3(-8, 0, 0), osg::Vec3(8, 0, 0), 2.0, osg::Vec4(0, 1, 0, 1)));
		addToModelspace(createText("��", osg::Vec3(-10, 0, 0), 2.0));
		addToModelspace(createText("��", osg::Vec3(10, 0, 0), 2.0));

		//std::string TowerClassText = "����: " + std::string(dataUI_MaxAngle->getBasicInfo()->getTowName());
		//addToModelspace(createHUD(osg::Vec3(850.0f, 960.0f, 0), TowerClassText));

		loadTowerNode();

		emit setRotateFromCenter();
	}
}

// @ funtion: �������ڵ�   --- by mzq 2019/6/13 15:13
void OsgUIManager::loadTowerNode()
{
	HdlDataUI_MaxAngle dataUI_MaxAngle = static_cast<DataUI_MaxAngle*>(_cl->getDataUI().GetPointee());
	HdlJ3DModelData towerModelData = dataUI_MaxAngle->getTowerModelData();

	for (int iLoop = 0; iLoop < towerModelData->sizeOfTowerNode(); iLoop++){
		auto towerNode = towerModelData->nodeAt(iLoop);
		osg::Vec3 pos = osg::Vec3(towerNode->getX(), towerNode->getY(), towerNode->getZ());
		osg::ref_ptr<osg::Node> point = createPoint(pos, 3, osg::Vec4(1.0, 0, 0, 1));
		if (findNode(pos) == -1)
			_nodeSet.push_back(SuspensionNode(addToModelspace(point), pos));
	}

	for (int iLoop = 0; iLoop < towerModelData->sizeOfIsolatedPt(); iLoop++){
		auto isolatedNode = towerModelData->isolatedPtAt(iLoop);
		osg::Vec3 pos = osg::Vec3(isolatedNode->getX(), isolatedNode->getY(), isolatedNode->getZ());
		osg::ref_ptr<osg::Node> point = createPoint(pos, 3, osg::Vec4(1.0, 0, 0, 1));
		if (findNode(pos) == -1)
			_nodeSet.push_back(SuspensionNode(addToModelspace(point), pos));
	}
}

// @ funtion: ������е�����   --- by mzq 2019/7/11 16:19
void OsgUIManager::clearAll()
{
	for (int i = 0; i < _modelspace.size(); i++)
	{
		if (_modelspace[i] != NULL)
		{
			_modelspace[i]->releaseGLObjects();
			_modelspace[i] = NULL;
		}
	}
	_modelspace.clear();
	_nodeSet.clear();
	_nodeSet.push_back(SuspensionNode(-1, osg::Vec3(0, 0, 0)));
}

// @ funtion: �޸ĵ���ʾΪ����   --- by lt(man) 2019/6/19 18:27
void OsgUIManager::modifyNode(int index,/*ģ�Ϳռ�id*/
							  double size/*��ߴ�*/,
							  osg::Vec4 xcolor/*��ɫ*/)
{
	// ֻ����node�а���һ��ͼ�ε����
	osg::ref_ptr<osg::Geometry> geometry = _modelspace[index]->asGeode()->getDrawable(0)->asGeometry();
	if (geometry && geometry->getPrimitiveSet(0)->getMode() == osg::PrimitiveSet::POINTS)
	{
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		colors->push_back(xcolor);
		geometry->setColorArray(colors);
		geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

		osg::ref_ptr<osg::Point> pointSize = new osg::Point;
		pointSize->setSize(size);
		geometry->getOrCreateStateSet()->setAttribute(pointSize, osg::StateAttribute::ON);
	}
}

// @ funtion: ���ݵ��ѡ�񲢸�����   --- by mzq 2019/12/5 18:39
void OsgUIManager::selectNode(int index/*���*/)
{
	for (int i = 1; i < _nodeSet.size(); i++){
		if (i == index)
			modifyNode(_nodeSet[i].id, 6.0, osg::Vec4(0.0, 1.0, 0.0, 1));
		else
			modifyNode(_nodeSet[i].id, 1.0, osg::Vec4(1.0, 1.0, 1.0, 1));
	}
	_curSelectNodeNum = index;
}

// @ funtion: ���ݵ������ڽڵ�������Ѱ�ң����ؽڵ����ڵ�ģ�Ϳռ�id   --- --- by mzq 2019/12/5 18:39
int OsgUIManager::findNode(osg::Vec3 pt/*������*/)
{
	for (auto iter = _nodeSet.begin()+1; iter != _nodeSet.end(); ++iter){
		if (nodeDistance(pt, iter->pos) < 0.1)
			return iter->id;
	}
	return -1;
}

// @ funtion:    --- by mzq 2019/12/5 18:39
int OsgUIManager::getNodeNumber(int id/*ģ�Ϳռ�id*/)
{
	for (int i = 1; i < _nodeSet.size(); i++){
		if (id == _nodeSet[i].id)
			return i;
	}
	return -1;
}
// @ funtion:    --- by mzq 2019/12/5 18:39
int OsgUIManager::getNodeNumber(osg::Vec3 node/*������*/)
{
	return getNodeNumber(findNode(node));
}

// @ funtion:    --- by mzq 2019/12/5 18:39
void OsgUIManager::clearNoSelectPoint(osg::Vec3 pt, 
									  bool isHasSelect)
{
	if (!isHasSelect)
		for (int i = 1; i < _nodeSet.size(); i++)
				modifyNode(_nodeSet[i].id, 1.0, osg::Vec4(1.0, 1.0, 1.0, 1));
	else
		for (int i = 1; i < _nodeSet.size(); i++)
			if (nodeDistance(pt, _nodeSet[i].pos)>0.1)
				modifyNode(_nodeSet[i].id, 1.0, osg::Vec4(1.0, 1.0, 1.0, 1));
}

//@function: �������   --- by mzq 2019/12/5 18:39
double OsgUIManager::nodeDistance(osg::Vec3 pt1/*��1*/, 
								  osg::Vec3 pt2/*��2*/) const
{
	return sqrt(pow((pt1.x() - pt2.x()), 2) + pow((pt1.y() - pt2.y()), 2) + pow((pt1.z() - pt2.z()), 2));
}