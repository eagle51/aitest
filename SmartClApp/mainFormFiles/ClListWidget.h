/****************************************************************/
//Author: 	�����
//Version: 	1.0
//Date:		2019.02.17
//Description:	��������������״�б�
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

	QSize sizeHint() const; //��ʼ�����ڴ�С

	std::vector<std::string> getclNameVec();//��ȡ��϶Բ�����б�

public slots :
	void slotGenerateTreeView();//�������б�
	void slotCreateItemBySubClName(const QString&);//ͨ��������϶Բ���ͽ�����
	void slotCreateTreeItem();//���ļ�ʱһ���Դ���������
	void slotReshTreeProName();//ˢ�¹�����
	void slotClearTree();//���������
	void slotClearClWidget();//�����϶Բ����
	void slotOnClick(const QModelIndex&);//�����¼�
	void slotDoubleClick(const QModelIndex&); //˫���¼�
	void slotRightMenu(const QPoint&);//�Ҽ��˵�
	void slotDeleteCl();//ɾ����϶Բ
	void slotRenameCl();//��������϶Բ
	void slotCopyCl();//��������
	void slotItemChanged(const QModelIndex& current);//��ǰ�ڵ����ݷ����ı�
	void slotDrawAllCl();//�������м�϶Բ�����ɼ�����

signals:
	void rightAddCl();
	void initClWidget();
	void saveDataFromForm();
	void signalDrawAllCl();
private:
	enum itemRole
	{
		markIndexCate = 0, //�������
		markFirst, //һ���ڵ㣬��϶Բ
		markSecond //�����ڵ㡣
	};
	void initTreeView();
	void setIconToCl(QStandardItem *curItem, SmartCl::ClCate::Type clType);//Ϊ��϶Բ�ڵ����ͼƬ
	std::vector<std::string> getAllNodeNames(const QStandardItem* parentItem);// ��ȡ���нڵ�����


private:
	QTreeView             *_pTreeView;
	QStandardItemModel    *_pItemModel;
	QStandardItem         *_proItem;
	QString                _oldClName;
	bool                   _isRename;
	bool                   _isInit;//��ʼ��ʱ

};