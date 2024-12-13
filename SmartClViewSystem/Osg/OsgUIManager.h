/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.12.9
//Description:	Osg��ʾ���������
/****************************************************************/
#pragma once
#include <QObject>
#include <QSharedPointer>

#include <math.h>
#include "OsgDrawEnt.h"
#include "EnumLib.h"
#include "JPoint.h"
#include "Clearence.h"

//ѡ��ģʽ
enum Mode
{
	NORMAL,
	SELECTPOINT,
	FITTING,
	MEASURING
};

/*�ڵ㣬�������Ҿ�Ե�Ӵ�*/
struct SuspensionNode
{
	SuspensionNode::SuspensionNode(int Id, osg::Vec3 Pos) 
	:id(Id),
	pos(Pos){}

	int id;
	osg::Vec3 pos;
};

class OsgUIManager :public QObject, OsgDrawEnt
{
	Q_OBJECT

public:
	OsgUIManager(SmartCl::HdlCl cl);
	~OsgUIManager();

	std::vector<osg::ref_ptr<osg::Node>> getmodelspace(){ return _modelspace; }//��ȡģ�Ϳռ�
	Mode getMode() const { return _curMode; }//��ȡѡ��ģʽ

	void setMode(Mode mode) { _curMode = mode; }//����ѡ��ģʽ

	int addToModelspace(osg::ref_ptr<osg::Node> node/*�ڵ�*/);//��ӵ�ģ�Ϳռ�
	void clearModelspace(){ _modelspace.clear(); }//���ģ�Ϳռ�

	void writeTowerModelToCurrentCl(bool isSmartPole); // ��ȡ����ģ�����ݵ���ǰ��Cl��
	void reverseTowerModel();  // ��ת����ģ��
	void loadTowerModel();  // ������������

	void clearAll();//������е�����

	void selectNode(int index/*���*/); //���ݵ��ѡ�񲢸�����
	int getNodeNumber(osg::Vec3 node/*������*/); //��ȡ���
	int getNodeNumber(int id/*ģ�Ϳռ�id*/); // ���� ģ�Ϳռ��е�id ��ȡ ���

	void clearNoSelectPoint(osg::Vec3 pt, 
							bool isHasSelect= true); // ��ճ�ѡ�е���������ĸ���

private:
	void loadTowerNode();//�������ڵ�

	void modifyNode(int index,/*ģ�Ϳռ�id*/
					double size/*��ߴ�*/,
					osg::Vec4 xcolor/*��ɫ*/);//�޸ĵ���ʾΪ����

	double nodeDistance(osg::Vec3 pt1/*��1*/, 
						osg::Vec3 pt2/*��2*/) const;//�������

	int findNode(osg::Vec3 node/*������*/);//���ݵ������ڽڵ�������Ѱ�ң����ؽڵ����ڵ�ģ�Ϳռ�id
	
signals:
	void connectPoint(int, osg::Vec3);
	void addNode(osg::ref_ptr<osg::Node>); //��ӽڵ�
	void removeNode(osg::ref_ptr<osg::Node>); //ɾ���ڵ�
	void updateVeiw(); //������ͼ
	void updateRotateCenter(osg::Vec3);//������ת����ߴ�
	void setRotateFromCenter(); //������ת���ĵ�
	void measureFinished();//��������

private:
	SmartCl::HdlCl                           _cl;//��϶Բ
	std::vector<osg::ref_ptr<osg::Node>>     _modelspace;//ģ�Ϳռ�
	vector<SuspensionNode>                   _nodeSet; //�ҵ㼯
	Mode                                     _curMode; //ѡ��ģʽ
	int                                      _curSelectNodeNum; //��ǰѡ�е��(osg��)
};
typedef QSharedPointer<OsgUIManager> QSPtr_OsgUIManager;