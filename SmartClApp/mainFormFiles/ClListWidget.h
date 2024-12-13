/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2019.02.17
//Description:	主界面中左侧的树状列表
/****************************************************************/
#pragma once
#include <QWidget>
#include "qdesktopwidget.h"
#include "qrect.h"
#include "qglobal.h"
#include "qapplication.h"
#include "EnumLib.h"

QT_BEGIN_NAMESPACE
class QTreeView;
class QStandardItemModel;
class QStandardItem;
class QModelIndex;
class QStatusBar;
QT_END_NAMESPACE

class ClListWidget :
	public QWidget
{
	Q_OBJECT
public:
	explicit ClListWidget(QWidget *parent = 0);
	~ClListWidget();

	QSize sizeHint() const; //初始化窗口大小

	std::vector<std::string> getclNameVec();//获取间隙圆名称列表

public slots :
	void slotGenerateTreeView();//生成数列表
	void slotCreateItemBySubClName(const QString&);//通过单个间隙圆类型建立树
	void slotCreateTreeItem();//打开文件时一次性创建工程树
	void slotReshTreeProName();//刷新工程名
	void slotClearTree();//清除工程树
	void slotClearClWidget();//清除间隙圆界面
	void slotOnClick(const QModelIndex&);//单击事件
	void slotDoubleClick(const QModelIndex&); //双击事件
	void slotRightMenu(const QPoint&);//右键菜单
	void slotDeleteCl();//删除间隙圆
	void slotRenameCl();//重命名间隙圆
	void slotCopyCl();//创建副本
	void slotItemChanged(const QModelIndex& current);//当前节点数据发生改变
	void slotDrawAllCl();//绘制所有间隙圆及生成计算书

signals:
	void rightAddCl();
	void initClWidget();
	void saveDataFromForm();
	void signalDrawAllCl();
private:
	enum itemRole
	{
		markIndexCate = 0, //索引类别
		markFirst, //一级节点，间隙圆
		markSecond //二级节点。
	};
	void initTreeView();
	void setIconToCl(QStandardItem *curItem, SmartCl::ClCate::Type clType);//为间隙圆节点加载图片
	std::vector<std::string> getAllNodeNames(const QStandardItem* parentItem);// 获取所有节点名称


private:
	QTreeView             *_pTreeView;
	QStandardItemModel    *_pItemModel;
	QStandardItem         *_proItem;
	QString                _oldClName;
	bool                   _isRename;
	bool                   _isInit;//初始化时

};