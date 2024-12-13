/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.12.9
//Description:	Osg显示界面管理类
/****************************************************************/
#pragma once
#include <QObject>
#include <QSharedPointer>

#include <math.h>
#include "OsgDrawEnt.h"
#include "EnumLib.h"
#include "JPoint.h"
#include "Clearence.h"

//选择模式
enum Mode
{
	NORMAL,
	SELECTPOINT,
	FITTING,
	MEASURING
};

/*节点，用于悬挂绝缘子串*/
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

	std::vector<osg::ref_ptr<osg::Node>> getmodelspace(){ return _modelspace; }//获取模型空间
	Mode getMode() const { return _curMode; }//获取选择模式

	void setMode(Mode mode) { _curMode = mode; }//设置选择模式

	int addToModelspace(osg::ref_ptr<osg::Node> node/*节点*/);//添加到模型空间
	void clearModelspace(){ _modelspace.clear(); }//清空模型空间

	void writeTowerModelToCurrentCl(bool isSmartPole); // 读取铁塔模型数据到当前的Cl中
	void reverseTowerModel();  // 翻转铁塔模型
	void loadTowerModel();  // 加载铁塔数据

	void clearAll();//清空所有的内容

	void selectNode(int index/*点号*/); //根据点号选择并高亮点
	int getNodeNumber(osg::Vec3 node/*点坐标*/); //获取点号
	int getNodeNumber(int id/*模型空间id*/); // 根据 模型空间中的id 获取 点号

	void clearNoSelectPoint(osg::Vec3 pt, 
							bool isHasSelect= true); // 清空除选中点外其他点的高亮

private:
	void loadTowerNode();//加载塔节点

	void modifyNode(int index,/*模型空间id*/
					double size/*点尺寸*/,
					osg::Vec4 xcolor/*颜色*/);//修改点显示为高亮

	double nodeDistance(osg::Vec3 pt1/*点1*/, 
						osg::Vec3 pt2/*点2*/) const;//两点距离

	int findNode(osg::Vec3 node/*点坐标*/);//根据点坐标在节点容器中寻找，返回节点所在的模型空间id
	
signals:
	void connectPoint(int, osg::Vec3);
	void addNode(osg::ref_ptr<osg::Node>); //添加节点
	void removeNode(osg::ref_ptr<osg::Node>); //删除节点
	void updateVeiw(); //更新视图
	void updateRotateCenter(osg::Vec3);//更新旋转坐标尺寸
	void setRotateFromCenter(); //设置旋转中心点
	void measureFinished();//测量结束

private:
	SmartCl::HdlCl                           _cl;//间隙圆
	std::vector<osg::ref_ptr<osg::Node>>     _modelspace;//模型空间
	vector<SuspensionNode>                   _nodeSet; //挂点集
	Mode                                     _curMode; //选择模式
	int                                      _curSelectNodeNum; //当前选中点号(osg中)
};
typedef QSharedPointer<OsgUIManager> QSPtr_OsgUIManager;