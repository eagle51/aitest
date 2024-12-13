#include "InsulatorLibWgt_I.h"
#include "InsulatorLib_I.h"
#include "EnumLib.h"
#include "DataBase.h"
#include "ClWidget.h"
#include "Mdl_InsultorPara_I.h"

#include "qlabel.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include <QHeaderView>
#include "qmenu.h"
#include "qmessagebox.h"
#include "qobject.h"
#include "qwidget.h"
#include "qcursor.h"
#include "qpoint.h"
#include "qcoreapplication.h"
#include <QScrollArea>
#include "qtablewidget.h"
#include "qabstractscrollarea.h"
#include "qheaderview.h"
#include "qlistview.h"
#include "Cl_Stright_I.h"

//I型绝缘子串Widget，构造函数 参数初始化列表
InsulatorLibWgt_I::InsulatorLibWgt_I(QWidget *parent)
	: QWidget(parent),
	_voltTypeCBx(nullptr),
	_tableWidget(nullptr),
	_splitNumCmbDeleg(nullptr),
	_tableMenu(nullptr),
	_picLbl(nullptr),
	_insulatorLib_I(nullptr),
	_insulatorLib_SingleVolt_I(nullptr)
{
	try
	{
		_insulatorLib_I = ClWidget::getCoreInstance()->getInsulatorLib_I()->clone();  //从 内核数据库 获取 I型绝缘子串数据  再复制一份
		_insulatorLib_SingleVolt_I = _insulatorLib_I->getInsulatorLib_SingleVolt_I(VoltType::AC110);//默认显示110kv   //从 I型绝缘子串数据中 获取对应电压等级下的绝缘子串库
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串库加载"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}

	initialWgt();     //初始化窗口
	initTableMenu();  //初始化表格右击菜单
}
//析构函数
InsulatorLibWgt_I::~InsulatorLibWgt_I()
{

}

// @ function:   --- by ljn 2019/12/25 18:49
void InsulatorLibWgt_I::initialWgt()
{
	/*电压等级*/
	_voltTypeCBx = new QComboBox();   //电压等级ComboBox下拉列表框组件
	_voltTypeCBx->setToolTip(QString::fromLocal8Bit("单击选择电压等级"));   //控件的toolTip用来给控件进行注释等信息
	for (unsigned int loop = 0; loop < VoltType::getVoltTypeLib().size(); ++loop)
	{
		_voltTypeCBx->addItem(QString::fromLocal8Bit(VoltType::typeToStr(VoltType::getVoltTypeLib().at(loop))));   //在电压等级ComboBox中添加一个新数据项
	}
	_voltTypeCBx->setView(new QListView());         //设置视图 列表控件
	QVBoxLayout *voltTypeLayout = new QVBoxLayout;  //创建电压等级的垂直布局
	voltTypeLayout->addWidget(_voltTypeCBx);        //在布局中加控件
	QGroupBox *voltTypeGbx = new QGroupBox(QStringLiteral("电压等级"));   //构建分组框
	voltTypeGbx->setLayout(voltTypeLayout);     //给分组框设置垂直布局
	connect(_voltTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangeVolt(int)));  //当前指数改变引起电压等级改变

	_tableWidget = new QTableWidget();  //新建列表控件
	//左边表头命名
	QStringList vHeaders;    //字符串列表类  对象（垂直表头字符串列表）     
	vHeaders << QStringLiteral(" 绝缘子串型号 ")
		<< QStringLiteral(" 绝缘子串总重力 (N) ")
		<< QStringLiteral(" 垂直风向绝缘子串联数 (联) ")
		<< QStringLiteral(" 顺风向绝缘子串联数 (联) ")
		<< QStringLiteral(" 单联绝缘子串片数 (片) ")
		<< QStringLiteral(" 单片绝缘子受风面积 (m^2) ")
		<< QStringLiteral(" 重锤重力 (N) ")
		<< QStringLiteral(" 分裂数 ")
		<< QStringLiteral(" 分裂间距 (m) ")
		<< QStringLiteral(" L1 铁塔挂点-金具第一转动点 (m) ")
		<< QStringLiteral(" L2 铁塔挂点-上子导线线夹挂点 (m) ")
		<< QStringLiteral(" L3 上子导线线夹挂点-均压环 (m) ")
		<< QStringLiteral(" L4 均压环宽度 (m) ")
		<< QStringLiteral(" L5 子导线线夹长度 (m) ")
		<< QStringLiteral(" L6 小弧垂 (m) ")
		<< QStringLiteral(" θ 线夹及小弧垂偏角(度) ");
	_tableWidget->setRowCount(vHeaders.count());   //给列表控件设置行数（由垂直表头字符串列表数量确定）
	_tableWidget->setRowHeight(0, 60);             //给列表控件设置 第0行的 高度
	_tableWidget->setVerticalHeaderLabels(vHeaders);//给列表控件设置 垂直标题标签

	_splitNumCmbDeleg = new ComboDelegate();  //创建 分裂数组合代理
	_tableWidget->setItemDelegateForRow(7, _splitNumCmbDeleg);  //为某一行 设置 项代理

	connect(_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotChangePic()));//在列表内，选择的项发生改变 引起图片改变
	connect(_splitNumCmbDeleg, SIGNAL(valueChanged()), this, SLOT(slotChangePic()));//在分裂数组合代理，值发生改变 引起图片改变

	/*绝缘子串及导线几何参数*/
	QStringList SplitNumTypeList;   //字符串列表类  对象（分裂数类型的字符串列表）
	for (SmartCl::SplitNum::Type type = SmartCl::SplitNum::One; type <= SmartCl::SplitNum::Ten; type = (SmartCl::SplitNum::Type)(type + 1))
		SplitNumTypeList << QString::fromLocal8Bit(SmartCl::SplitNum::typeToStr(type));
	_splitNumCmbDeleg->setSList(SplitNumTypeList);   //把 分裂数类型的字符串列表 放在 分裂数组合代理 里面

	QGroupBox *parameterGroupBox = new QGroupBox(QStringLiteral("绝缘子串参数"));//创建 绝缘子串参数 组合框
	QVBoxLayout *boxLayout = new QVBoxLayout;   //创建 垂直框布局
	boxLayout->addWidget(voltTypeGbx);          //在垂直框布局 放置 电压等级分组框 控件
	boxLayout->addWidget(_tableWidget);         //在垂直框布局 放置  列表控件
	parameterGroupBox->setLayout(boxLayout);    //在绝缘子串参数组合框中放置 垂直框布局

	_picLbl = new QLabel();   //创建图片标签
    _picLbl->setFixedSize(310, 600);  //给图片标签设置 宽度 高度
	QHBoxLayout *mainLayout = new QHBoxLayout; //创建水平布局
	mainLayout->addWidget(parameterGroupBox);   //水平布局里加控件（绝缘子串参数）
	mainLayout->addWidget(_picLbl);   //水平布局里加控件（图片标签）
	setLayout(mainLayout);    //给 主窗口 设置 水平主布局

	readDataToForm();  //调用 把数据读取到界面界面上 函数
	slotChangePic();   //调用图片改变函数
}

// @ function: 从界面把数据写入数据库  --- by ljn 2019/12/26 19:35
void InsulatorLibWgt_I::writeDataFromForm()
{
	try
	{
		for (unsigned int columnIndex = 0; columnIndex < _tableWidget->columnCount(); ++columnIndex)
		{
			HdlInsulator_I insulator_I = _insulatorLib_SingleVolt_I->at(columnIndex);     //当前列的电压等级对应的I型绝缘子数据库
			writeOneInsulator_I(insulator_I,true,columnIndex);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 写入一列数据   -- by mzq 2023/7/15 15:17
void InsulatorLibWgt_I::writeOneInsulator_I(HdlInsulator_I insulator_I, 
											bool writeSpecial/*是否写入线夹及小弧垂信息*/, 
											int index)
{
	if (insulator_I.isNull())
		return;

	HdlMdl_InsultorPara_I insultorPara_I = insulator_I->getInsultorPara_I();      //I型绝缘子串参数
	insultorPara_I->setInsultorName(_tableWidget->item(0, index)->text().toLocal8Bit().toStdString());//将列表里的数据写入I型绝缘子串参数的数据库中
	insultorPara_I->setInsultorWeight(_tableWidget->item(1, index)->text().toDouble());
	insultorPara_I->setNumSubs_V(_tableWidget->item(2, index)->text().toInt());
	insultorPara_I->setNumSubs_H(_tableWidget->item(3, index)->text().toInt());
	insultorPara_I->setNumPlate(_tableWidget->item(4, index)->text().toInt());
	insultorPara_I->setShieldArea(_tableWidget->item(5, index)->text().toDouble());
	insultorPara_I->setHammerWeight(_tableWidget->item(6, index)->text().toDouble());

	HdlMdl_InsulaGeoPara_I insulaGeoPara_I = insulator_I->getInsulaGeoPara_I();      //I型绝缘子串几何参数
	insulaGeoPara_I->setSplitNum(SplitNum::Type(SplitNum::strToType(_tableWidget->item(7, index)->text().toLocal8Bit().toStdString())));
	insulaGeoPara_I->setSplitShow(_tableWidget->item(8, index)->text().toLocal8Bit().toStdString());//将列表里的数据写入I型绝缘子串导线几何参数的数据库中
	insulaGeoPara_I->setL1(_tableWidget->item(9, index)->text().toDouble());
	insulaGeoPara_I->setL2(_tableWidget->item(10, index)->text().toDouble());
	insulaGeoPara_I->setL3(_tableWidget->item(11, index)->text().toDouble());
	insulaGeoPara_I->setL4(_tableWidget->item(12, index)->text().toDouble());
	insulaGeoPara_I->setL5(_tableWidget->item(13, index)->text().toDouble());
	if (writeSpecial)
	{
		insulaGeoPara_I->setL6(_tableWidget->item(14, index)->text().toDouble());
		insulaGeoPara_I->setTheta(_tableWidget->item(15, index)->text().toDouble());
	}
}

// @ function: 将数据读入界面中  --- by ljn 2019/12/26 19:37
void InsulatorLibWgt_I::readDataToForm()
{
	try
	{
		/*初始化表格的列数并存放数据*/
		int columnCount = _insulatorLib_SingleVolt_I->getSize();  //电压等级对应的I型绝缘子数据库的列数
		_tableWidget->clearContents();     //清空列表
		_tableWidget->setColumnCount(columnCount);    //给列表设置列数

		for (unsigned int columnIndex = 0; columnIndex < columnCount; ++columnIndex)
		{
			/*绝缘子串刷新界面数据*/
			HdlInsulator_I insulator_I = _insulatorLib_SingleVolt_I->at(columnIndex); //当前列的电压等级对应的I型绝缘子数据库

			HdlMdl_InsultorPara_I insultorPara_I = insulator_I->getInsultorPara_I();
			_tableWidget->setItem(0, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insultorPara_I->getInsultorName().c_str())));
			_tableWidget->setItem(1, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getInsultorWeight())));//获取到的数据添加到列表每个项中
			_tableWidget->setItem(2, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getNumSubs_V())));
			_tableWidget->setItem(3, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getNumSubs_H())));
			_tableWidget->setItem(4, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getNumPlate())));
			_tableWidget->setItem(5, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getShieldArea())));
			_tableWidget->setItem(6, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getHammerWeight())));

			HdlMdl_InsulaGeoPara_I insulaGeoPara_I = insulator_I->getInsulaGeoPara_I();
			_tableWidget->setItem(7, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(SplitNum::typeToStr(insulaGeoPara_I->getSplitNum()))));
			_tableWidget->setItem(8, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insulaGeoPara_I->getSplitShow().c_str())));
			_tableWidget->setItem(9, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL1())));
			_tableWidget->setItem(10, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL2())));
			_tableWidget->setItem(11, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL3())));
			_tableWidget->setItem(12, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL4())));
			_tableWidget->setItem(13, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL5())));
			_tableWidget->setItem(14, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL6())));
			_tableWidget->setItem(15, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getTheta())));
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串I串数据加载"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: 初始化表格右击菜单  --- by ljn 2019/12/25 18:55
void InsulatorLibWgt_I::initTableMenu()
{
	_tableMenu = new QMenu(_tableWidget);             
	QAction *addAction = new QAction(_tableMenu);     
	QAction *deleteAction = new QAction(_tableMenu);  
	QAction *applyAction = new QAction(_tableMenu);   

	addAction->setText(QStringLiteral("增加列"));     
	deleteAction->setText(QStringLiteral("删除列"));  
	applyAction->setText(QStringLiteral("更新直线塔I串间隙圆串参数"));

	_tableMenu->addAction(addAction);           
	_tableMenu->addAction(deleteAction);   
	_tableMenu->addAction(applyAction);
	_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);//设置 右键菜单内容 显示方式

	connect(_tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotTableMenu(QPoint)));    //在列表控件中，用户内容菜单需求（点击），此界面，打开右击菜单
	connect(addAction, SIGNAL(triggered()), this, SLOT(slotAddColumn()));           
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(slotDeletColumn()));  
	connect(applyAction, SIGNAL(triggered()), this, SLOT(slotApplyToProject_I()));
}

// @ function: 打开右键菜单  --- by ljn 2019/12/25 19:55
void InsulatorLibWgt_I::slotTableMenu(QPoint pos)
{
	_tableMenu->exec(QCursor::pos());//右键菜单 在鼠标位置显示
}

// @ function: 增加列  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_I::slotAddColumn()
{
	try
	{
		writeDataFromForm();     //从界面把数据写入数据库
		int columnIndex = _tableWidget->currentColumn();   //列表的当前列数
		HdlInsulator_I curInsulator = new Insulator_I();   //新建一个绝缘子串I串数据库   指针接收  
		while (_insulatorLib_SingleVolt_I->has(curInsulator->getInsultorPara_I()->getInsultorName()))//数据库中，在相应电压等级下 查询对应名称的导线
		{
			std::string newName = curInsulator->getInsultorPara_I()->getInsultorName() + "_副本";   //新名称=旧名称+副本
			curInsulator->getInsultorPara_I()->setInsultorName(newName);    //把新名字放到数据库里
		}
		_insulatorLib_SingleVolt_I->insert(curInsulator, columnIndex + 1);//在当前列下一列插入数据
		readDataToForm();//把内核数据读取到界面
		_tableWidget->setCurrentCell(0, columnIndex + 1);    //列表设置当前选中为当前列的下一列
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("增加列"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: 删除列  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_I::slotDeletColumn()
{
	int select = QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否删除？"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		try
		{
			writeDataFromForm();//从界面把数据写入数据库
			int columnIndex = _tableWidget->currentColumn(); //列表的当前列数
			_insulatorLib_SingleVolt_I->del(columnIndex);//数据库里删除当前列
			readDataToForm();//把内核数据读取到界面
			_tableWidget->setCurrentCell(0, 0);//设置当前选中为第一列
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("删除列"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		}
	}
}

//@function: 更新直线塔I串间隙圆串参数   -- by mzq 2023/7/15 14:39
void InsulatorLibWgt_I::slotApplyToProject_I()
{
	int select = QMessageBox::warning(this, 
									  QString::fromLocal8Bit("提示"), 
									  QString::fromLocal8Bit("是否将当前绝缘子串参数更新至本工程的所有“直线塔I串”间隙圆中的同名绝缘子串中？"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		writeDataFromForm();     //从界面把数据写入数据库

		bool isUpdateL6Theta = true;
		int select = QMessageBox::warning(this,
										  QString::fromLocal8Bit("提示"),
										  QString::fromLocal8Bit("是否同步更新“小弧垂”以及“线夹及小弧垂偏角”？"),
										  QMessageBox::Yes | QMessageBox::No);
		if (select == QMessageBox::Yes)
			isUpdateL6Theta = true;
		else
			isUpdateL6Theta = false;

		QString showText = "";
		for (unsigned int loop = 0; loop < ClWidget::getCoreInstance()->getClCount(); ++loop)
		{
			HdlCl cl = ClWidget::getCoreInstance()->getClAt(loop);
			if (cl->getClType() == ClCate::StraightIStr)
			{
				HdlCl_Stright_I stright_I = dynamic_cast<Cl_Stright_I*>(cl.GetPointee());
				HdlMdl_InsultorPara_I insultorPara_I = stright_I->getDataUI_I()->getInsultorPara_I();
				HdlMdl_InsulaGeoPara_I insulaGeoPara_I = stright_I->getDataUI_I()->getInsulaGeoPara_I();

				if (insultorPara_I->getInsulatorType() == InsulatorType::Type_I
					&& insultorPara_I->getInsultorName() == _tableWidget->item(0, _tableWidget->currentColumn())->text().toLocal8Bit().toStdString())
				{
					HdlInsulator_I insulator_I = new Insulator_I();
					insulator_I->setInsultorPara_I(insultorPara_I);
					insulator_I->setInsulaGeoPara_I(insulaGeoPara_I);
					writeOneInsulator_I(insulator_I, isUpdateL6Theta, _tableWidget->currentColumn());

					showText += QString::fromLocal8Bit(cl->getClName().c_str());
					showText += "\n";
				}
			}
		}
		QMessageBox::warning(this,
							 QString::fromLocal8Bit("提示"),
							 QString::fromLocal8Bit("共计更新了以下直线塔I串间隙圆中的绝缘子串参数\n\n") + showText,
							 QMessageBox::Yes );

	}
}

// @ function: 更改界面示意图  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_I::slotChangePic()
{
	if (_tableWidget->columnCount() != 0)//如果列数不为零
	{
		int columnIndex = _tableWidget->currentColumn();//获取列表当前列
		-1 == columnIndex ? columnIndex = 0 : columnIndex;//如果界面无当前选中列（-1 == columnIndex为真），则默认显示第一列（columnIndex = 0）对应分裂数的图片
		SplitNum::Type splitNumType = SplitNum::Type(SplitNum::strToType(_tableWidget->item(7, columnIndex)->text().toLocal8Bit().toStdString()));//获取分裂数类型

		QPixmap pixmap;
		switch (splitNumType)
		{
		case SmartCl::SplitNum::One:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/noneSplit.png"));
			break;
		case SmartCl::SplitNum::TwoV:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/twoSplitVer.png"));
			break;
		case SmartCl::SplitNum::TwoH:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/twoSplitHor.png"));
			break;
		case SmartCl::SplitNum::Three:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/threeSplit.png"));
			break;
		case SmartCl::SplitNum::Four:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/fourSplit.png"));
			break;
		case SmartCl::SplitNum::Six:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/sixSplit.png"));
			break;
		case SmartCl::SplitNum::Eight:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/eightSplit.png"));
			break;
		case SmartCl::SplitNum::Ten:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/tenSplit.png"));
			break;
		default:
			break;
		}
		pixmap.scaled(_picLbl->size(), Qt::KeepAspectRatio);
		_picLbl->setScaledContents(true);
		_picLbl->setPixmap(pixmap);
	}
}

//@function: 改变电压等级   -- by mzq  2020/1/10 20:58
void InsulatorLibWgt_I::slotChangeVolt(int index)
{
	try
	{
		writeDataFromForm();    //写数据
		_insulatorLib_SingleVolt_I = _insulatorLib_I->getInsulatorLib_SingleVolt_I(VoltType::Type(index));
		readDataToForm();       //读数据
		slotChangePic();        //改变图片
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("切换电压等级"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: 保存到json文件  --- by ljn 2020/1/3 16:51
void InsulatorLibWgt_I::writeToJson()
{
	try
	{
		writeDataFromForm();//写数据
		if ("" == _insulatorLib_SingleVolt_I->hasRepeat())//如果没有重复的绝缘子串名称
		{
			ClWidget::getCoreInstance()->setInsulatorLib_I(_insulatorLib_I);//内核数据库设置I型绝缘子串数据
			_insulatorLib_I->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File()); //I型绝缘子串数据写到json文件   获取工程总的文件夹目录下 “文件” 所在路径
		}
		else
		{
			std::string name = _insulatorLib_SingleVolt_I->hasRepeat();
			QMessageBox::warning(this, QString::fromLocal8Bit("保存失败"),
				QString::fromLocal8Bit("存在重复型号：")
				+ QString::fromLocal8Bit(name.c_str())
				+ QString::fromLocal8Bit("\n请核对修改后再进行保存！"),
				QMessageBox::Yes);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("保存数据"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: 设置表格编辑状态  --- by ljn 2020/1/16 11:22
void InsulatorLibWgt_I::setTableStatus()
{
	_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);      //表格设置为不可编辑
	_tableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);  //设置选中模式为选中的整列
	_tableWidget->setContextMenuPolicy(Qt::NoContextMenu);                 //设置表格右键菜单不可见
}

//@function: 获取当前绝缘子串   -- by mzq  2020/1/16 21:38
SmartCl::HdlInsulator_I InsulatorLibWgt_I::getCurrentInsulator()
{
	int columnIndex = _tableWidget->currentColumn();   //表格的当前列
	if (-1 != columnIndex)     //如果有选择的列
		return _insulatorLib_SingleVolt_I->at(columnIndex);   //电压等级对应的I型绝缘子串数据所在的列
	else
		return nullptr;
}

//@function: 设置电压等级   -- by mzq  2020/1/16 21:58
void InsulatorLibWgt_I::setVoltage(VoltType::Type voltType)
{
	_voltTypeCBx->setCurrentIndex(voltType);//给 电压等级组合框 设置 当前的电压等级指数
	slotChangeVolt(voltType);
}