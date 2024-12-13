#include "InsulatorLibWgt_L.h"
#include "InsulatorLib_L.h"
#include "EnumLib.h"
#include "DataBase.h"
#include "ClWidget.h"
#include "Mdl_InsultorPara_L.h"

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
#include "Cl_Stright_L.h"
InsulatorLibWgt_L::InsulatorLibWgt_L(QWidget *parent)
	: QWidget(parent),
	_voltTypeCBx(nullptr),
	_tableWidget(nullptr),
	_splitNumCmbDeleg(nullptr),
	_tableMenu(nullptr),
	_picLbl(nullptr),
	_insulatorLib_L(nullptr),
	_insulatorLib_SingleVolt_L(nullptr)
{
	try
	{
		_insulatorLib_L = ClWidget::getCoreInstance()->getInsulatorLib_L()->clone();
		_insulatorLib_SingleVolt_L = _insulatorLib_L->getInsulatorLib_SingleVolt_L(VoltType::AC110);//默认显示110kv
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串库加载"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}

	initialWgt();
	initTableMenu();
}

InsulatorLibWgt_L::~InsulatorLibWgt_L()
{

}

// @ function:   --- by ljn 2019/12/25 18:49
void InsulatorLibWgt_L::initialWgt()
{
	/*电压等级*/
	_voltTypeCBx = new QComboBox();
	_voltTypeCBx->setToolTip(QString::fromLocal8Bit("单击选择电压等级"));
	for (unsigned int loop = 0; loop < VoltType::getVoltTypeLib().size(); ++loop)
	{
		_voltTypeCBx->addItem(QString::fromLocal8Bit(VoltType::typeToStr(VoltType::getVoltTypeLib().at(loop))));
	}
	_voltTypeCBx->setView(new QListView());
	QVBoxLayout *voltTypeLayout = new QVBoxLayout;
	voltTypeLayout->addWidget(_voltTypeCBx);
	QGroupBox *voltTypeGbx = new QGroupBox(QStringLiteral("电压等级"));
	voltTypeGbx->setLayout(voltTypeLayout);
	connect(_voltTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangeVolt(int)));

	_tableWidget = new QTableWidget();
	//左边表头命名
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 绝缘子串型号 ")
		<< QStringLiteral(" L串总重力减b肢重力 (N) ")
		<< QStringLiteral(" L串总重力减a肢重力 (N) ")
		<< QStringLiteral(" a肢垂直风向绝缘子串联数 (联) ")
		<< QStringLiteral(" b肢垂直风向绝缘子串联数 (联) ")
		<< QStringLiteral(" a肢单联绝缘子串片数 (片) ")
		<< QStringLiteral(" b肢单联绝缘子串片数 (片) ")
		<< QStringLiteral(" 单片绝缘子受风面积 (m^2) ")
		<< QStringLiteral(" 重锤重力 (N) ")
		<< QStringLiteral(" 分裂数 ")
		<< QStringLiteral(" 分裂间距 (m) ")
		<< QStringLiteral(" L1_a a肢单肢长度 (m) ")
		<< QStringLiteral(" L1_b b肢单肢长度 (m) ")
		<< QStringLiteral(" L2_a a肢均压环高度 (m) ")
		<< QStringLiteral(" L2_b b肢均压环高度 (m) ")
		<< QStringLiteral(" L3_a a肢均压环宽度 (m) ")
		<< QStringLiteral(" L3_b b肢均压环宽度 (m) ")
		<< QStringLiteral(" L4 联板宽度 (m) ")
		<< QStringLiteral(" L5 线夹挂孔与联板挂孔垂距 (m) ")
		<< QStringLiteral(" L6 子导线线夹长度 (m) ")
		<< QStringLiteral(" L7 小弧垂 (m) ")
		<< QStringLiteral(" θ 线夹及小弧垂偏角 (°) ");
	_tableWidget->setRowCount(vHeaders.count());
	_tableWidget->setRowHeight(0, 60);
	_tableWidget->setVerticalHeaderLabels(vHeaders);

	_splitNumCmbDeleg = new ComboDelegate();
	_tableWidget->setItemDelegateForRow(9, _splitNumCmbDeleg);

	connect(_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotChangePic()));
	connect(_splitNumCmbDeleg, SIGNAL(valueChanged()), this, SLOT(slotChangePic()));

	/*绝缘子串及导线几何参数*/
	QStringList SplitNumTypeList;
	for (SmartCl::SplitNum::Type type = SmartCl::SplitNum::One; type <= SmartCl::SplitNum::Ten; type = (SmartCl::SplitNum::Type)(type + 1))
		SplitNumTypeList << QString::fromLocal8Bit(SmartCl::SplitNum::typeToStr(type));
	_splitNumCmbDeleg->setSList(SplitNumTypeList);

	QGroupBox *parameterGroupBox = new QGroupBox(QStringLiteral("绝缘子串参数"));
	QVBoxLayout *boxLayout = new QVBoxLayout;
	boxLayout->addWidget(voltTypeGbx);
	boxLayout->addWidget(_tableWidget);
	parameterGroupBox->setLayout(boxLayout);

	_picLbl = new QLabel();
	_picLbl->setFixedSize(500, 357);
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(parameterGroupBox);
	mainLayout->addWidget(_picLbl);
	setLayout(mainLayout);

	readDataToForm();
	slotChangePic();
}

// @ function: 从界面把数据写入数据库  --- by ljn 2019/12/26 19:35
void InsulatorLibWgt_L::writeDataFromForm()
{
	try
	{
		for (unsigned int columnIndex = 0; columnIndex < _tableWidget->columnCount(); ++columnIndex)
		{
			HdlInsulator_L insulator_L = _insulatorLib_SingleVolt_L->at(columnIndex);
			writeOneInsulator_L(insulator_L, true, columnIndex);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 写入一列数据   -- by mzq 2023/7/29 15:26
void InsulatorLibWgt_L::writeOneInsulator_L(HdlInsulator_L insulator_L, bool writeSpecial/*是否写入线夹及小弧垂信息*/, int index)
{
	if (insulator_L.isNull())
		return;

	HdlMdl_InsultorPara_L insultorPara_L = insulator_L->getInsultorPara_L();
	insultorPara_L->setInsultorName(_tableWidget->item(0, index)->text().toLocal8Bit().toStdString());
	insultorPara_L->setInsultorWeight_a(_tableWidget->item(1, index)->text().toDouble());
	insultorPara_L->setInsultorWeight_b(_tableWidget->item(2, index)->text().toDouble());
	insultorPara_L->setNumSubs_a_V(_tableWidget->item(3, index)->text().toInt());
	insultorPara_L->setNumSubs_b_V(_tableWidget->item(4, index)->text().toInt());
	insultorPara_L->setNumPlate_a(_tableWidget->item(5, index)->text().toInt());
	insultorPara_L->setNumPlate_b(_tableWidget->item(6, index)->text().toInt());
	insultorPara_L->setShieldArea(_tableWidget->item(7, index)->text().toDouble());
	insultorPara_L->setHammerWeight(_tableWidget->item(8, index)->text().toDouble());

	HdlMdl_InsulaGeoPara_L insulaGeoPara_L = insulator_L->getInsulaGeoPara_L();
	insulaGeoPara_L->setSplitNum(SplitNum::Type(SplitNum::strToType(_tableWidget->item(9, index)->text().toLocal8Bit().toStdString())));
	insulaGeoPara_L->setSplitShow(_tableWidget->item(10, index)->text().toLocal8Bit().toStdString());
	insulaGeoPara_L->setL1_a(_tableWidget->item(11, index)->text().toDouble());
	insulaGeoPara_L->setL1_b(_tableWidget->item(12, index)->text().toDouble());
	insulaGeoPara_L->setL2_a(_tableWidget->item(13, index)->text().toDouble());
	insulaGeoPara_L->setL2_b(_tableWidget->item(14, index)->text().toDouble());
	insulaGeoPara_L->setL3_a(_tableWidget->item(15, index)->text().toDouble());
	insulaGeoPara_L->setL3_b(_tableWidget->item(16, index)->text().toDouble());
	insulaGeoPara_L->setL4(_tableWidget->item(17, index)->text().toDouble());
	insulaGeoPara_L->setL5(_tableWidget->item(18, index)->text().toDouble());
	insulaGeoPara_L->setL6(_tableWidget->item(19, index)->text().toDouble());

	if (writeSpecial)
	{
		insulaGeoPara_L->setL7(_tableWidget->item(20, index)->text().toDouble());
		insulaGeoPara_L->setTheta(_tableWidget->item(21, index)->text().toDouble());
	}
}

// @ function: 将数据读入界面中  --- by ljn 2019/12/26 19:37
void InsulatorLibWgt_L::readDataToForm()
{
	try
	{
		/*初始化表格的列数并存放数据*/
		int columnCount = _insulatorLib_SingleVolt_L->getSize();
		_tableWidget->clearContents();
		_tableWidget->setColumnCount(columnCount);

		for (unsigned int columnIndex = 0; columnIndex < columnCount; ++columnIndex)
		{
			/*绝缘子串刷新界面数据*/
			HdlInsulator_L insulator_L = _insulatorLib_SingleVolt_L->at(columnIndex);

			HdlMdl_InsultorPara_L insultorPara_L = insulator_L->getInsultorPara_L();
			_tableWidget->setItem(0, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insultorPara_L->getInsultorName().c_str())));
			_tableWidget->setItem(1, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getInsultorWeight_a())));
			_tableWidget->setItem(2, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getInsultorWeight_b())));
			_tableWidget->setItem(3, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getNumSubs_a_V())));
			_tableWidget->setItem(4, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getNumSubs_b_V())));
			_tableWidget->setItem(5, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getNumPlate_a())));
			_tableWidget->setItem(6, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getNumPlate_b())));
			_tableWidget->setItem(7, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getShieldArea())));
			_tableWidget->setItem(8, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getHammerWeight())));

			HdlMdl_InsulaGeoPara_L insulaGeoPara_L = insulator_L->getInsulaGeoPara_L();
			_tableWidget->setItem(9, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(SplitNum::typeToStr(insulaGeoPara_L->getSplitNum()))));
			_tableWidget->setItem(10, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insulaGeoPara_L->getSplitShow().c_str())));
			_tableWidget->setItem(11, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL1_a())));
			_tableWidget->setItem(12, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL1_b())));
			_tableWidget->setItem(13, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL2_a())));
			_tableWidget->setItem(14, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL2_b())));
			_tableWidget->setItem(15, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL3_a())));
			_tableWidget->setItem(16, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL3_b())));
			_tableWidget->setItem(17, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL4())));
			_tableWidget->setItem(18, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL5())));
			_tableWidget->setItem(19, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL6())));
			_tableWidget->setItem(20, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL7())));
			_tableWidget->setItem(21, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getTheta())));
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串L串数据加载"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: 初始化表格右击菜单  --- by ljn 2019/12/25 18:55
void InsulatorLibWgt_L::initTableMenu()
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
	_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(_tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotTableMenu(QPoint)));
	connect(addAction, SIGNAL(triggered()), this, SLOT(slotAddColumn()));
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(slotDeletColumn()));
	connect(applyAction, SIGNAL(triggered()), this, SLOT(slotApplyToProject_L()));
}

// @ function: 打开右键菜单  --- by ljn 2019/12/25 19:55
void InsulatorLibWgt_L::slotTableMenu(QPoint pos)
{
	_tableMenu->exec(QCursor::pos());
}


// @ function: 增加列  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_L::slotAddColumn()
{
	try
	{
		writeDataFromForm();
		int columnIndex = _tableWidget->currentColumn();
		HdlInsulator_L curInsulator = new Insulator_L();
		while (_insulatorLib_SingleVolt_L->has(curInsulator->getInsultorPara_L()->getInsultorName()))
		{
			std::string newName = curInsulator->getInsultorPara_L()->getInsultorName() + "_副本";
			curInsulator->getInsultorPara_L()->setInsultorName(newName);
		}
		_insulatorLib_SingleVolt_L->insert(curInsulator, columnIndex + 1);
		readDataToForm();
		_tableWidget->setCurrentCell(0, columnIndex + 1);    //设置当前选中为当前列的下一列
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("增加列"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: 删除列  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_L::slotDeletColumn()
{
	int select = QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否删除？"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		try
		{
			writeDataFromForm();
			int columnIndex = _tableWidget->currentColumn();
			_insulatorLib_SingleVolt_L->del(columnIndex);
			readDataToForm();
			_tableWidget->setCurrentCell(0, 0);//设置当前选中为第一列
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("删除列"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		}
	}
}

//@function: 更新直线塔L串间隙圆串参数   -- by mzq 2023/7/29 15:26
void InsulatorLibWgt_L::slotApplyToProject_L()
{
	int select = QMessageBox::warning(this,
									  QString::fromLocal8Bit("提示"),
									  QString::fromLocal8Bit("是否将当前绝缘子串参数更新至本工程的所有“直线塔L串”间隙圆中的同名绝缘子串中？"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		writeDataFromForm();     //从界面把数据写入数据库

		bool isUpdateTheta = true;
		int select = QMessageBox::warning(this,
										  QString::fromLocal8Bit("提示"),
										  QString::fromLocal8Bit("是否同步更新“小弧垂”以及“线夹及小弧垂偏角”？"),
										  QMessageBox::Yes | QMessageBox::No);
		if (select == QMessageBox::Yes)
			isUpdateTheta = true;
		else
			isUpdateTheta = false;

		QString showText = "";
		for (unsigned int loop = 0; loop < ClWidget::getCoreInstance()->getClCount(); ++loop)
		{
			HdlCl cl = ClWidget::getCoreInstance()->getClAt(loop);
			if (cl->getClType() == ClCate::StraightIStr)
			{
				HdlCl_Stright_L stright_L = dynamic_cast<Cl_Stright_L*>(cl.GetPointee());
				HdlMdl_InsultorPara_L insultorPara_L = stright_L->getDataUI_L()->getInsultorPara_L();
				HdlMdl_InsulaGeoPara_L insulaGeoPara_L = stright_L->getDataUI_L()->getInsulaGeoPara_L();

				if (insultorPara_L->getInsulatorType() == InsulatorType::Type_L
					&& insultorPara_L->getInsultorName() == _tableWidget->item(0, _tableWidget->currentColumn())->text().toLocal8Bit().toStdString())
				{
					HdlInsulator_L insulator_L = new Insulator_L();
					insulator_L->setInsultorPara_L(insultorPara_L);
					insulator_L->setInsulaGeoPara_L(insulaGeoPara_L);
					writeOneInsulator_L(insulator_L, isUpdateTheta, _tableWidget->currentColumn());

					showText += QString::fromLocal8Bit(cl->getClName().c_str());
					showText += "\n";
				}
			}
		}
		QMessageBox::warning(this,
							 QString::fromLocal8Bit("提示"),
							 QString::fromLocal8Bit("共计更新了以下直线塔L串间隙圆中的绝缘子串参数\n\n") + showText,
							 QMessageBox::Yes);

	}
}

// @ function: 更改界面示意图  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_L::slotChangePic()
{
	if (_tableWidget->columnCount() != 0)
	{
		int columnIndex = _tableWidget->currentColumn();
		-1 == columnIndex ? columnIndex = 0 : columnIndex;//如果界面无当前选中列，则默认显示第一列对应分裂数的图片
		SplitNum::Type splitNumType = SplitNum::Type(SplitNum::strToType(_tableWidget->item(9, columnIndex)->text().toLocal8Bit().toStdString()));

		QPixmap pixmap;
		switch (splitNumType)
		{
		case SmartCl::SplitNum::One:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/noneSplit.png"));
			break;
		case SmartCl::SplitNum::TwoV:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/twoSplitVer.png"));
			break;
		case SmartCl::SplitNum::TwoH:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/twoSplitHor.png"));
			break;
		case SmartCl::SplitNum::Three:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/threeSplit.png"));
			break;
		case SmartCl::SplitNum::Four:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/fourSplit.png"));
			break;
		case SmartCl::SplitNum::Six:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/sixSplit.png"));
			break;
		case SmartCl::SplitNum::Eight:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/eightSplit.png"));
			break;
		case SmartCl::SplitNum::Ten:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/tenSplit.png"));
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
void InsulatorLibWgt_L::slotChangeVolt(int index)
{
	try
	{
		writeDataFromForm();
		_insulatorLib_SingleVolt_L = _insulatorLib_L->getInsulatorLib_SingleVolt_L(VoltType::Type(index));
		readDataToForm();
		slotChangePic();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("切换电压等级"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: 保存到json文件  --- by ljn 2020/1/3 16:51
void InsulatorLibWgt_L::writeToJson()
{
	try
	{
		writeDataFromForm();
		if ("" == _insulatorLib_SingleVolt_L->hasRepeat())
		{
			ClWidget::getCoreInstance()->setInsulatorLib_L(_insulatorLib_L);
			_insulatorLib_L->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());
		}
		else
		{
			std::string name = _insulatorLib_SingleVolt_L->hasRepeat();
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
void InsulatorLibWgt_L::setTableStatus()
{
	_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_tableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);//设置选中模式为选中行
	_tableWidget->setContextMenuPolicy(Qt::NoContextMenu);//设置表格菜单不可见
}

//@function: 获取当前绝缘子串   -- by mzq  2020/1/16 21:38
SmartCl::HdlInsulator_L InsulatorLibWgt_L::getCurrentInsulator()
{
	int columnIndex = _tableWidget->currentColumn();
	if (-1 != columnIndex)
		return _insulatorLib_SingleVolt_L->at(columnIndex);
	else
		return nullptr;
}

//@function: 设置电压等级   -- by mzq  2020/1/16 21:58
void InsulatorLibWgt_L::setVoltage(VoltType::Type voltType)
{
	_voltTypeCBx->setCurrentIndex(voltType);
}