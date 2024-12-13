#include "ClListWidget.h"
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QModelIndex>
#include <QAction>
#include <QMenu>
#include <QStatusBar>
#include "DataBase.h"
#include "OdqInterfaces.h"

#include "ExStringIO.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdCommandStack.h" 
#include "DbHostAppServices.h"
#include "UIAutilityFiles/UIAutility.h"

ClListWidget::ClListWidget(QWidget *parent /*= 0*/) : QWidget(parent)
{
	_isInit = true;
	_proItem = nullptr;
	_isRename = false;
	this->initTreeView();
}

ClListWidget::~ClListWidget()
{
}

//@function: 初始化窗口大小   -- by mzq  2019/11/22 15:52
QSize ClListWidget::sizeHint() const
{
	QDesktopWidget *deskWidget = QApplication::desktop();
	QRect appRect = deskWidget->screenGeometry();
	qreal dockWidgetMinW = 0.1*appRect.width();

	//初始化窗口大小;   
	return QSize(dockWidgetMinW, 900);
}

//@function: 获取间隙圆名称列表   -- by mzq 2023/7/5 17:13
std::vector<std::string> ClListWidget::getclNameVec()
{
	std::vector<std::string> Names = getAllNodeNames(_proItem);

	for (int row = 0; row < _pItemModel->rowCount(); ++row) 
	{
		QStandardItem* item = _pItemModel->item(row, 0);
		std::string name = item->text().toLocal8Bit().toStdString();
		Names.push_back(name);
	}
	return Names;
}

void ClListWidget::initTreeView()
{
	_pTreeView = new QTreeView(this);
	_pItemModel = new QStandardItemModel();
	_pTreeView->setModel(_pItemModel);
	_pTreeView->setDragDropMode(QAbstractItemView::InternalMove);
	_pTreeView->setDragEnabled(true);
	_pTreeView->setAcceptDrops(true);
	_pTreeView->setDefaultDropAction(Qt::MoveAction);

	QVBoxLayout *MainLayout = new QVBoxLayout();
	MainLayout->addWidget(_pTreeView);
	setLayout(MainLayout);

	_pTreeView->header()->setDefaultAlignment(Qt::AlignCenter);

	_pTreeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	_pTreeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	_pTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	_pTreeView->header()->setStretchLastSection(false);

	_pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(_pTreeView, SIGNAL(customContextMenuRequested(const QPoint&)),
		this, SLOT(slotRightMenu(const QPoint &)));
	//单击事件
	connect(_pTreeView, SIGNAL(clicked(const QModelIndex&)),
		this, SLOT(slotOnClick(const QModelIndex&)));
	//双击事件
	connect(_pTreeView, SIGNAL(doubleClicked(const QModelIndex&)),
		this, SLOT(slotDoubleClick(const QModelIndex&)));

	connect(_pTreeView->model(), SIGNAL(dataChanged(const QModelIndex, const QModelIndex)),
		this, SLOT(slotItemChanged(const QModelIndex)));

	connect(this, SIGNAL(signalDrawAllCl()),
		this, SLOT(slotDrawAllCl()));

}

void ClListWidget::slotGenerateTreeView()
{
	try
	{
		_pItemModel->clear();
		_pItemModel->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit(SmartCl::DataBase::getInstance()->getProjectName().c_str())));
		_proItem = new QStandardItem();
		_proItem->setText(QString::fromLocal8Bit("间隙圆列表"));
		_proItem->setData(markIndexCate, Qt::UserRole + 1);
		_pItemModel->appendRow(_proItem);
		_pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("创建工程树"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//单个间隙圆名建立树节点:
void ClListWidget::slotCreateItemBySubClName(const QString& clName)
{
	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);

		QStandardItem *item = new QStandardItem(clName);

		SmartCl::ClCate::Type type = SmartCl::DataBase::getInstance()->findCl(clName.toLocal8Bit().toStdString())->getClType();
		const char* clType = SmartCl::ClCate::typeToStr(type);//临时值,此界面上需要提供此参数的值
		
		setIconToCl(item, type);
		item->setData(markFirst, Qt::UserRole + 1);
		item->setFlags(item->flags() & ~Qt::ItemIsDropEnabled);//当前节点不允许drop放入子节点
		_proItem->appendRow(item);
		_pTreeView->setCurrentIndex(item->index());
		_pTreeView->expandAll();
		_isInit = false;

		IqConsole* iConsole = getIConsole();
		if (!iConsole)
		{
			ODA_FAIL();
			return;
		}

		// load Startup Suite
		OdEdCommand* pCommand = ::odedRegCmds()->lookupCmd(OD_T("new")).get();
		if (pCommand)
		{
			OdEdCommandContext* pCmdCtx = iConsole->getCommandContext();
			ODA_ASSERT_ONCE(pCmdCtx);
			if (pCmdCtx)
			{

				OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(getIApp()->toOdString(clName)); // LOADCONTENTS
				pCmdCtx->setArbitraryData(OD_T("new/CLNAME"), pStringIO);
				OdSmartPtr<ExStringIO> pStringIO_ClType =
					ExStringIO::create(getIApp()->toOdString(QString::fromLocal8Bit(clType))); // LOADCONTENTS
				pCmdCtx->setArbitraryData(OD_T("new/CLTYPE"), pStringIO_ClType);

				try
				{
					pCommand->execute(pCmdCtx);
				}
				catch (...)
				{
					ODA_FAIL_ONCE();
				}
				pCmdCtx->setArbitraryData(OD_T("new/CLNAME"), NULL);
			}
		}
		getIApp()->getMainWindow()->statusBar()->showMessage(QString::fromLocal8Bit("间隙圆：%1 已创建！").arg(clName), 5000);
		QApplication::restoreOverrideCursor();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("创建工程树"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		QApplication::restoreOverrideCursor();
	}
}

//一次性创建工程树:
void ClListWidget::slotCreateTreeItem()
{
	try
	{
		slotGenerateTreeView();//建立工程树

		//创建工程树的节点
		int clCount = SmartCl::DataBase::getInstance()->getClCount();
		for (int iLoop = 0; iLoop < clCount; ++iLoop)
		{
			auto curCl = SmartCl::DataBase::getInstance()->getClAt(iLoop);//该索引下的间隙圆
			std::string clNameStr = curCl->getClName();//间隙圆名称
			slotCreateItemBySubClName(QString::fromLocal8Bit(clNameStr.c_str()));
		}
		_pTreeView->expandAll();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("创建工程树"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}
//@ function:
void ClListWidget::slotReshTreeProName()
{
	try
	{
		if (_proItem != nullptr)
			_pItemModel->setHorizontalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit(SmartCl::DataBase::getInstance()->getProjectName().c_str())));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("更新工程树"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}
//清除工程树
void ClListWidget::slotClearTree()
{
	_pItemModel->clear();
	
}

//@function: 为间隙圆节点加载图片   -- by mzq  2020/5/16 15:43
void ClListWidget::setIconToCl(QStandardItem *curItem, SmartCl::ClCate::Type clType)
{
	QString pngName;
	switch (clType)
	{
	case SmartCl::ClCate::StraightIStr :
		pngName = ":/images/SmartCL/straight_I.png";
		break;
	case SmartCl::ClCate::StraightVStr:
		pngName = ":/images/SmartCL/straight_V.png";
		break;
	case SmartCl::ClCate::StraightLStr:
		pngName = ":/images/SmartCL/straight_L.png";
		break;
	case SmartCl::ClCate::MaxAlgle_I:
		pngName = ":/images/SmartCL/maxAngle_I.png";
		break;
	case SmartCl::ClCate::MaxAlgle_V:
		pngName = ":/images/SmartCL/maxAngle_V.png";
		break;
	default:
		break;
	}

	curItem->setIcon(QIcon(pngName));
}


//@function:  获取所有节点名称  -- by mzq 2023/7/5 17:41
std::vector<std::string> ClListWidget::getAllNodeNames(const QStandardItem* parentItem)
{
	std::vector<std::string> nodeNames;
	for (int row = 0; row < parentItem->rowCount(); ++row) 
	{
		QStandardItem* item = parentItem->child(row);
		nodeNames.push_back(item->text().toLocal8Bit().toStdString());
		if (item->hasChildren())
		{
			std::vector<std::string> strVec = getAllNodeNames(item);
			nodeNames.insert(nodeNames.end(), strVec.begin(), strVec.end());
		}
	}
	return nodeNames;
}

//右键菜单
void ClListWidget::slotRightMenu(const QPoint& pos)
{
	QMenu rightMenu;
	QAction *addClAct = new QAction(QString::fromLocal8Bit("新增"), nullptr);
	QAction *renameClAct = new QAction(QString::fromLocal8Bit("重命名"), nullptr);
	QAction *deleClAct = new QAction(QString::fromLocal8Bit("删除"), nullptr);
	QAction *copyAct = new QAction(QString::fromLocal8Bit("创建副本"), nullptr);
	addClAct->setVisible(false);
	deleClAct->setVisible(false);
	renameClAct->setVisible(false);
	copyAct->setVisible(false);

	connect(addClAct, SIGNAL(triggered()), this, SIGNAL(rightAddCl()));
	connect(deleClAct, SIGNAL(triggered()), this, SLOT(slotDeleteCl()));
	connect(renameClAct, SIGNAL(triggered()), this, SLOT(slotRenameCl()));
	connect(copyAct, SIGNAL(triggered()), this, SLOT(slotCopyCl()));

	rightMenu.addAction(addClAct);
	rightMenu.addAction(renameClAct);
	rightMenu.addAction(deleClAct);
	rightMenu.addAction(copyAct);

	QModelIndex index = _pTreeView->indexAt(pos);
	QVariant var_project = index.data(Qt::UserRole + 1);
	if (var_project.isValid())
	{
		if (markIndexCate == var_project.toInt())
			addClAct->setVisible(true);
		else if (markFirst == var_project.toInt())
		{
			deleClAct->setVisible(true);
			renameClAct->setVisible(true);
			copyAct->setVisible(true);
		}
	}
	rightMenu.exec(QCursor::pos());  //显示菜单
}

//双击事件
void ClListWidget::slotDoubleClick(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	QVariant var_project = index.data(Qt::UserRole + 1);
	if (var_project.isValid())
	{
		if (markFirst == var_project)
		{
			slotRenameCl();
		}
	}
}

//单击事件
void ClListWidget::slotOnClick(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	QVariant var_project = index.data(Qt::UserRole + 1);
	QStandardItem *item = _pItemModel->itemFromIndex(index);
	QString clNameQstr = item->text();//基础名
	try
	{
		if (markFirst != var_project)
			return;
		emit saveDataFromForm();

		//绘图区的当前文档切换命令
		{
			IqConsole* iConsole = getIConsole();
			if (!iConsole)
			{
				ODA_FAIL();
				return;
			}
			QString qsCmd = QString("syswindow %1").arg(clNameQstr);
			iConsole->executeCommand(qsCmd); // 开启图纸
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("切换间隙圆"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

void ClListWidget::slotClearClWidget()
{ 	
	int iClCount = SmartCl::DataBase::getInstance()->getClCount();
	if (iClCount > 0)
	{
		for (int i = 0; i < iClCount; i++)
		{
			IqConsole* iConsole = getIConsole();
			if (!iConsole)
			{
				ODA_FAIL();
				return;
			}
			auto curCl = SmartCl::DataBase::getInstance()->getClAt(i);
			QString allClName = QString::fromLocal8Bit(curCl->getClName().c_str());
			QString qsCmd = QString("removeview %1").arg(allClName);
			iConsole->executeCommand(qsCmd);
		}	
	}
	return;
}
//@ function:
void ClListWidget::slotCopyCl()
{
	QModelIndex curIndex = _pTreeView->currentIndex();
	_pTreeView->edit(curIndex);
	QStandardItem *curItem = _pItemModel->itemFromIndex(curIndex);
	_oldClName = curItem->text();

	QString newClName = curItem->text() + QString::fromLocal8Bit("_副本");

	try
	{
		/*保存当前界面间隙圆数据*/
		IqConsole* iConsole = getIConsole();
		if (!iConsole)
		{
			ODA_FAIL();
			return;
		}
		QString qsCmd = QString("savecurrentcl ");
		iConsole->executeCommand(qsCmd);

		if (SmartCl::DataBase::getInstance()->existCl(newClName.toLocal8Bit().toStdString()))
		{
			throw std::exception("间隙元名称："+ newClName.toLocal8Bit() + " 已存在！");
		}
		else
		{
			SmartCl::DataBase::getInstance()->createCopy(_oldClName.toLocal8Bit().toStdString(), newClName.toLocal8Bit().toStdString());
			slotCreateItemBySubClName(newClName);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("创建副本"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@ function:
void ClListWidget::slotDeleteCl()
{
	bool bDelt;
	int iDelt = QMessageBox::question(this, QString::fromLocal8Bit("删除"),
		QString::fromLocal8Bit("是否删除此间隙圆？"),
		QMessageBox::Yes | QMessageBox::No | 
		QMessageBox::Yes);
	switch (iDelt)
	{
	case QMessageBox::Yes:
		bDelt = true;
		break;
	case QMessageBox::No:
		bDelt = false;
		break;	
	}
	if (bDelt)
	{
		QModelIndex curIndex = _pTreeView->currentIndex();
		QStandardItem *curItem = _pItemModel->itemFromIndex(curIndex);
		QString  curClName = curItem->text();
		try
		{
			if (curItem->parent() == nullptr)
			{
				QMessageBox::warning(this, QString::fromLocal8Bit("删除间隙圆"), 
									 QString::fromLocal8Bit("当前间隙圆节点不在间隙圆列表中，无法删除，请将该间隙圆节点拖拽至间隙圆列表中后再执行删除"), QMessageBox::Yes);
				return;
			}
			curItem->parent()->removeRow(curItem->index().row());

			//间隙圆界面切到原始状态或则第一个节点
			QStandardItem *item = _proItem->child(0, 0);
			QString clName;
			if (item != nullptr)
			{
				clName = item->text();
			}
			else
			{
				emit initClWidget();
			}

			//删除文档
			{
				IqConsole* iConsole = getIConsole();
				if (!iConsole)
				{
					ODA_FAIL();
					return;
				}
				QString qsCmd = QString("removeview %1").arg(curClName);
				iConsole->executeCommand(qsCmd); // 重命名文档

			}
			SmartCl::DataBase::getInstance()->removeByName(curClName.toLocal8Bit().toStdString());
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("删除间隙圆"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		}
	}
	
}
//@ function:
void ClListWidget::slotRenameCl()
{
	QModelIndex curIndex = _pTreeView->currentIndex();
	_pTreeView->edit(curIndex);
	QStandardItem *curItem = _pItemModel->itemFromIndex(curIndex);
	_oldClName = curItem->text();
	_isRename = true;
}

//@ function: 当前节点数据发生改变
void ClListWidget::slotItemChanged(const QModelIndex& current)
{
	if (_isInit)
		return;
	QVariant var_project = current.data(Qt::UserRole + 1);
	if ((markFirst != var_project) || !_isRename)
		return;
	QStandardItem *curItem = _pItemModel->itemFromIndex(current);
	QString newClName = curItem->text();
	if (_oldClName == newClName)
		return;
	try
	{
		/*名称合法性检查*/
		if (newClName == "" || UIAutility::hasBlank(newClName))
		{
			curItem->setText(_oldClName);
			throw std::exception("间隙圆名称不能为空或者存在空格等特殊字符");
		}
		if (SmartCl::DataBase::getInstance()->existCl(newClName.toLocal8Bit().toStdString()))
		{
			curItem->setText(_oldClName);
			throw std::exception("当前间隙圆名称已存在，无法创建，需重新填写");
		}

		SmartCl::DataBase::getInstance()->renameByName(_oldClName.toLocal8Bit().toStdString(), newClName.toLocal8Bit().toStdString());
		//SmartCl::DataBase::getInstance()->setCurrentClName(newClName.toLocal8Bit().toStdString());

		//绘图区的文档重命名及命令
		{
			IqConsole* iConsole = getIConsole();
			if (!iConsole)
			{
				ODA_FAIL();
				return;
			}
			QString qsCmd = QString("renameview %1=%2").arg(_oldClName).arg(newClName);
			iConsole->executeCommand(qsCmd); // 重命名文档

		}
		getIApp()->getMainWindow()->statusBar()->showMessage(QString::fromLocal8Bit("重命名间隙圆成功"), 5000);
		_isRename = false;
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("重命名间隙圆"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 绘制所有间隙圆及生成计算书   -- by mzq  2019/9/3 17:12
void ClListWidget::slotDrawAllCl()
{
	for (unsigned int rowLoop = 0; rowLoop < _proItem->rowCount(); ++rowLoop)
	{
		QStandardItem *item = _proItem->child(rowLoop, 0);

		_pTreeView->setCurrentIndex(item->index());

		slotOnClick(item->index());

		IqConsole* iConsole = getIConsole();
		if (!iConsole)
		{
			ODA_FAIL();
			return;
		}
		QString qsCmd = QString("drawcl ");

		QApplication::setOverrideCursor(Qt::WaitCursor);
		iConsole->executeCommand(qsCmd);
		QApplication::restoreOverrideCursor();
	}
}