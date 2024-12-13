#include "MaxAngleWgt.h"
#include "ImportTowerDataDlg.h"
#include "Cl_MaxAngle_I.h"
#include <fstream>
#include "EnumLib.h"
#include "Mdl_BasicPara.h"
#include "ClWidget.h"
#include <vector>

#include "qsplitter.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "qgroupbox.h"
#include "qscrollarea.h"
#include "qmessagebox.h"
#include "qapplication.h"
#include "qgridlayout.h"
#include "qcombobox.h"
#include "qwidget.h"
#include "qobject.h"

#include "OdqInterfaces.h"
#include "Ed\EdCommandStack.h"
#include "ExStringIO.h"
#include "Ed\EdCommandContext.h"

MaxAngleWgt::MaxAngleWgt(int clType, 
						 std::string clName) 
						 :ClWidget(clType, clName)
{
	_dataUI_MaxAngle = static_cast<DataUI_MaxAngle*>(_cl->getDataUI().GetPointee());

	_isReversal = false;
	_osgUIManager = QSPtr_OsgUIManager(new OsgUIManager(_cl));
	_osgWidget = NULL;
	_cadToolBar = NULL;
}

MaxAngleWgt::~MaxAngleWgt()
{

}

//@function: 用于OdqView类调取并显示界面   -- by mzq  2019/12/4 15:10
OsgQt* MaxAngleWgt::getOsgWidget(QWidget *parent/*父窗口*/)
{
	if (NULL == _osgWidget)
		_osgWidget = new OsgQt(_osgUIManager, parent);
	return _osgWidget;
}

//@function: 用于OdqView类调取并显示   -- by mzq  2019/12/23 11:37
QToolBar* MaxAngleWgt::getCadToolBar(QWidget *parent /*= 0/*父窗口*/)
{
	if (NULL == _cadToolBar)
		_cadToolBar = new QToolBar(parent);
	return _cadToolBar;
}

//@function: 初始化窗口   -- by mzq  2019/9/9 16:49
void MaxAngleWgt::initWgt()
{
	//分割窗口
	QSplitter *centerBottomSplitter = new QSplitter(Qt::Vertical);
	centerBottomSplitter->setHandleWidth(5);
	centerBottomSplitter->addWidget(initClDesignWgt());
	centerBottomSplitter->addWidget(initErrTable());
	centerBottomSplitter->setStretchFactor(0, 20);
	centerBottomSplitter->setStretchFactor(1, 5);

	//整体布局
	QVBoxLayout *mainVLyt = new QVBoxLayout();
	mainVLyt->addWidget(centerBottomSplitter);
	mainVLyt->setMargin(0);
	this->setLayout(mainVLyt);
}

//@function: 初始化间隙圆窗口   -- by mzq  2019/9/10 10:36
QWidget* MaxAngleWgt::initClDesignWgt()
{
	/*标题及按钮栏*/
	_inportTowerBtn = new QPushButton(QStringLiteral("导入杆塔模型"));
	connect(_inportTowerBtn, SIGNAL(clicked()), this, SLOT(slotOpenImpotrTowerDlg()));
	connect(_pReportBtn, SIGNAL(clicked()), this, SLOT(slotCreatReport()));
	connect(_pDrawClearanceBtn, SIGNAL(clicked()), this, SLOT(slotDrawCommand()));
	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addWidget(_clTypeTitle);
	btnLyt->addStretch();
	btnLyt->addWidget(_inportTowerBtn);
	btnLyt->addWidget(_pReportBtn);
	btnLyt->addWidget(_pDrawClearanceBtn);
	btnLyt->setSpacing(20);
	btnLyt->setContentsMargins(8, 12, 8, 8);

	/*CAD工具栏*/
	initCadToolBar();

	/*基本信息*/
	QGroupBox *basicInfoGbx = this->initBasicInfoGbx();
	basicInfoGbx->setFixedHeight(88);

	/*间隙值信息*/
	QGroupBox *gapValueGbx = this->initGapValueGbx();
	gapValueGbx->setFixedHeight(200);

	/*绝缘子串信息*/
	QGroupBox *insulatorParaGbx = this->initInsulatorParaGbx();
	insulatorParaGbx->setFixedHeight(430);

	/*界面布局*/
	QGridLayout *gridLyt = new QGridLayout();//栅格为
	gridLyt->addWidget(basicInfoGbx, 0, 0, 1, 2);
	gridLyt->addWidget(gapValueGbx, 1, 0, 1, 2);
	gridLyt->addWidget(insulatorParaGbx, 2, 0, 1, 2);

	QVBoxLayout *SCrollAreaLyt = new QVBoxLayout();
	SCrollAreaLyt->addLayout(gridLyt);
	QWidget* widget = new QWidget();
	widget->setLayout(SCrollAreaLyt);

	QScrollArea *SCrollArea = new QScrollArea();
	SCrollArea->setWidget(widget);
	SCrollArea->setWidgetResizable(true);
	SCrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	SCrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	SCrollArea->setBackgroundRole(QPalette::Light);

	QVBoxLayout *widgetLyt = new QVBoxLayout();
	widgetLyt->addLayout(btnLyt);
	widgetLyt->addWidget(SCrollArea);
	widgetLyt->setMargin(0);
	widgetLyt->setContentsMargins(0, 8, 0, 0);

	QGroupBox *groupBox = new QGroupBox();
	groupBox->setLayout(widgetLyt);
	groupBox->setFlat(true);

	return groupBox;
}

//@function: 初始CAD绘图工具栏   -- by mzq  2019/12/23 11:30
void MaxAngleWgt::initCadToolBar()
{
	if (NULL == _cadToolBar)
		return;

	QStringList lstDirection;
	lstDirection << QStringLiteral("ext") << QStringLiteral("top") << QStringLiteral("bottom");

	/*toolbar添加action*/
	_cadToolBar->addSeparator();
	QString strIcon;
	for (int iLoop = 0; iLoop < lstDirection.length(); ++iLoop)
	{
		QAction *action = new QAction;

		if (iLoop == 0)
			strIcon = QString::fromLocal8Bit(":images/teigha/zoom_") + lstDirection.at(iLoop) + QString::fromLocal8Bit(".png");
		else
			strIcon = QString::fromLocal8Bit(":images/teigha/view_") + lstDirection.at(iLoop) + QString::fromLocal8Bit(".png");

		action->setIcon(QIcon(strIcon));
		action->setToolTip(lstDirection.at(iLoop));

		QString strCmd;
		if (iLoop == 0)
			strCmd = QString::fromLocal8Bit("zoom ") + lstDirection.at(iLoop);
		else
			strCmd = QString::fromLocal8Bit("view ") + lstDirection.at(iLoop);

		connect(action, &QAction::triggered, [=]() { getIConsole()->executeCommand(strCmd); });
		_cadToolBar->addAction(action);
	}

	_meteCateCmb = new QComboBox();
	if (ClCate::MaxAlgle_I == _cl->getClType())
	{
		_meteCateCmb->addItem(QString::fromLocal8Bit("工频"));
		_meteCateCmb->addItem(QString::fromLocal8Bit("操作"));
		_meteCateCmb->addItem(QString::fromLocal8Bit("雷电"));
		_meteCateCmb->addItem(QString::fromLocal8Bit("带电"));
		_meteCateCmb->addItem(QString::fromLocal8Bit("全部工况"));
		_meteCateCmb->setFixedWidth(80);
	}
	
	if (ClCate::MaxAlgle_V == _cl->getClType())
	{
		_meteCateCmb->addItem(QString::fromLocal8Bit("最大夹角"));
		_meteCateCmb->addItem(QString::fromLocal8Bit("最小夹角"));
		_meteCateCmb->addItem(QString::fromLocal8Bit("最大+最小夹角"));
		_meteCateCmb->setFixedWidth(120);
	}
	_meteCateCmb->setView(new QListView());
	connect(_meteCateCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));
	_cadToolBar->addWidget(_meteCateCmb);

}

//@function: 初始化基本信息组合框   -- by mzq  2019/9/10 11:27
QGroupBox* MaxAngleWgt::initBasicInfoGbx()
{
	QGroupBox *basicInfoGbx = new QGroupBox(QString::fromLocal8Bit("基本信息"));
	/*杆塔名称*/
	_towNameEdt = new QLineEdit("Untitled");
	_towNameEdt->setToolTip(QString::fromLocal8Bit("此处输入杆塔名称"));
	QVBoxLayout *towNameLayout = new QVBoxLayout;
	towNameLayout->addWidget(_towNameEdt);
	QGroupBox *towNameGbx = new QGroupBox(QStringLiteral("杆塔名称"));
	towNameGbx->setLayout(towNameLayout);

	/*电压等级*/
	_voltTypeCBx = new QComboBox();
	//_voltTypeCBx->setMinimumWidth(90);
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


	/*回路类型*/
	_loopTypeCBx = new QComboBox();
	//_loopTypeCBx->setMinimumWidth(70);
	_loopTypeCBx->setToolTip(QString::fromLocal8Bit("单击选择回路类型，回路类型与间隙推荐值有关"));
	for (unsigned int loop = 0; loop < LoopType::getLoopTypeLib().size(); ++loop)
	{
		_loopTypeCBx->addItem(QString::fromLocal8Bit(LoopType::typeToStr(LoopType::getLoopTypeLib().at(loop))));
	}
	_loopTypeCBx->setView(new QListView());
	QVBoxLayout *loopTypeLayout = new QVBoxLayout;
	loopTypeLayout->addWidget(_loopTypeCBx);
	QGroupBox *loopTypeGbx = new QGroupBox(QStringLiteral("回路类型"));
	loopTypeGbx->setLayout(loopTypeLayout);

	/*海拔高度*/
	_altitudeEdt = new QLineEdit("1000");
	_altitudeEdt->setToolTip(QString::fromLocal8Bit("输入海拔高度，海拔高度与间隙推荐值有关"));
	QVBoxLayout *altitudeLayout = new QVBoxLayout;
	altitudeLayout->addWidget(_altitudeEdt);
	QGroupBox *altitudeGbx = new QGroupBox(QStringLiteral("海拔高度(m)"));
	altitudeGbx->setLayout(altitudeLayout);

	QGridLayout *gridLyt = new QGridLayout();//栅格为1*4
	gridLyt->addWidget(towNameGbx, 0, 0, 1, 1);
	gridLyt->addWidget(voltTypeGbx, 0, 1, 1, 1);
	gridLyt->addWidget(loopTypeGbx, 0, 2, 1, 1);
	gridLyt->addWidget(altitudeGbx, 0, 3, 1, 1);
	gridLyt->setColumnStretch(0, 1);
	gridLyt->setColumnStretch(1, 1);
	gridLyt->setColumnStretch(2, 1);
	gridLyt->setColumnStretch(3, 1);

	basicInfoGbx->setLayout(gridLyt);

	connect(_voltTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));
	connect(_loopTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));
	connect(_altitudeEdt, SIGNAL(textChanged(const QString &)), this, SLOT(slotSgstGapChanged()));

	return basicInfoGbx;
}

//@function: 将数据读入界面中   -- by mzq  2019/9/9 16:49
void MaxAngleWgt::readDataToForm()
{
	try
	{
		//间隙圆类型标题
		_clTypeTitle->setText(QString::fromLocal8Bit(" 类型：") + QString::fromLocal8Bit(SmartCl::ClCate::typeToStr(_cl->getClType())));

		//基本信息模块
		this->readBasicInfoData(_dataUI_MaxAngle->getBasicInfo());

		//对应不同类型间隙圆绝缘子串弹出窗口参数
		this->readChildClDataToForm();

		_osgUIManager->clearAll();
		updateOsgView();
		_osgUIManager->loadTowerModel();

	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("界面数据写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 基本信息   -- by mzq  2019/12/13 18:00
void MaxAngleWgt::readBasicInfoData(SmartCl::HdlMdl_BasicPara pBasicPara)
{
	try
	{
		_towNameEdt->setText(QString::fromLocal8Bit(pBasicPara->getTowName()));
		_voltTypeCBx->setCurrentIndex(VoltType::Type(pBasicPara->getVoltType()));
		_loopTypeCBx->setCurrentIndex(LoopType::Type(pBasicPara->getLoopType()));
		_altitudeEdt->setText(QString::number(pBasicPara->getAltitude()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("基本信息数据读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 从界面把数据写入数据库   -- by mzq  2019/9/9 16:49
void MaxAngleWgt::writeDataFromForm()
{
	try
	{
		//基本信息模块
		this->writeBasicInfoData(_dataUI_MaxAngle->getBasicInfo());

		//对应不同类型间隙圆绝缘子串弹出窗口参数
		this->writeChildClDataFromForm();

	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("界面数据写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 基本信息   -- by mzq  2019/12/13 18:00
void MaxAngleWgt::writeBasicInfoData(SmartCl::HdlMdl_BasicPara pBasicPara)
{
	try
	{
		string name = _towNameEdt->text().toLocal8Bit();
		pBasicPara->setTowName(name.c_str());
		pBasicPara->setTowType(TowerType::Suspensive);
		pBasicPara->setVoltType(VoltType::Type(_voltTypeCBx->currentIndex()));
		pBasicPara->setLoopType(LoopType::Type(_loopTypeCBx->currentIndex()));
		pBasicPara->setAltitude(_altitudeEdt->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("基本信息数据写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 绘制间隙圆以及生成计算书，用于一键生成图纸以及计算书功能   -- by mzq  2019/9/9 16:49
void MaxAngleWgt::drawClAndCreatReport()
{
	slotCreatReport(false);//生成报告
	slotDrawCommand();//绘制间隙圆
}

//@function: 导入铁塔模型数据对话框   -- by mzq  2019/9/10 15:39
void MaxAngleWgt::slotOpenImpotrTowerDlg()
{
	emit changeTabWidgetIndex(0);
	ImportTowerDataDlg dlg(_osgUIManager, this);
	dlg.exec();
}

//@function: 生成报告   -- by mzq  2019/9/11 17:14
void MaxAngleWgt::slotCreatReport(bool isOpenReportMsg /*= true*/)
{
	
	slotCalcProcess();
	if (_hasErr)
		return;

	QString clName = QString::fromLocal8Bit(_cl->getClName().c_str());
	getIApp()->getMainWindow()->statusBar()->showMessage(QString::fromLocal8Bit("正在生成 “%1” 间隙圆计算书，请耐心等候......").arg(clName), 500000);
	QApplication::setOverrideCursor(Qt::WaitCursor);

	try
	{
		if (ClCate::MaxAlgle_I == _cl->getClType())
			_meteCateCmb->setCurrentIndex(GapCate::All);
		else if (ClCate::MaxAlgle_V == _cl->getClType())
			_meteCateCmb->setCurrentIndex(Angle_V::All);
		
		slotDrawCommand();
		writeReport();/*生成计算书*/	
	}
	catch (std::exception& e)
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}

	getIApp()->getMainWindow()->statusBar()->showMessage(QString::fromLocal8Bit(" “%1” 间隙圆计算书已生成！").arg(clName), 5000);
	QApplication::restoreOverrideCursor();

	/*是否打开计算书*/
	if (isOpenReportMsg == true)
	{
		string savePathStr = getCoreInstance()->getPathFolder() + "\\" + "间隙圆计算书" + "\\" + _cl->getClName() + ".pdf";
		QString savePath = QString::fromLocal8Bit(savePathStr.c_str());
		int select = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("计算书已生成，是否打开？"),
											  QMessageBox::Yes | QMessageBox::No);
		switch (select)
		{
		case QMessageBox::Yes:
			QDesktopServices::openUrl(QUrl::fromLocalFile(savePath));
			break;
		case  QMessageBox::No:
			break;
		default:
			break;
		}
	}
}

//@function: 绘制间隙圆   -- by mzq  2019/9/11 17:15
void MaxAngleWgt::slotDrawCommand()
{
	emit changeTabWidgetIndex(1);//切换绘图区显示至CAD绘图页面

	/*提交计算，如果有错误，则返回*/
	slotCalcProcess();
	if (_hasErr)
		return;

	QString clName = QString::fromLocal8Bit(_cl->getClName().c_str());
	const char* clType = SmartCl::ClCate::typeToStr(_cl->getClType());

	IqConsole* iConsole = getIConsole();
	if (!iConsole)
	{
		ODA_FAIL();
		return;
	}

	getIApp()->getMainWindow()->statusBar()->showMessage(QString::fromLocal8Bit("正在生成 “%1” 间隙圆图纸，请耐心等候......").arg(clName), 500000);
	QApplication::setOverrideCursor(Qt::WaitCursor);

	OdEdCommand* pCommand = ::odedRegCmds()->lookupCmd(OD_T("CreateArc")).get();
	if (pCommand)
	{
		OdEdCommandContext* pCmdCtx = iConsole->getCommandContext();
		ODA_ASSERT_ONCE(pCmdCtx);
		if (pCmdCtx)
		{
			//获取名称
			OdSmartPtr<ExStringIO> pStringIO_ClName = ExStringIO::create(
				getIApp()->toOdString(clName)); // CLNAME
			pCmdCtx->setArbitraryData(OD_T("CreateArc/CLNAME"), pStringIO_ClName);
			//间隙圆类型 class ClCate
			OdSmartPtr<ExStringIO> pStringIO_ClType = ExStringIO::create(
				getIApp()->toOdString(QString::fromLocal8Bit(clType))); // clType
			pCmdCtx->setArbitraryData(OD_T("CreateArc/CLTYPE"), pStringIO_ClType);

			try
			{
				pCommand->execute(pCmdCtx);
			}
			catch (...)
			{
				QApplication::restoreOverrideCursor();
				ODA_FAIL_ONCE();
			}
			pCmdCtx->setArbitraryData(OD_T("CreateArc/CLNAME"), NULL);
			pCmdCtx->setArbitraryData(OD_T("CreateArc/CLTYPE"), NULL);

		}
	}

	slotErrDisplay();
	getIConsole()->executeCommand("zoom ext");//调整视图
	QApplication::restoreOverrideCursor();
}

//@function: 更新Osg窗口显示   -- by mzq  2019/12/4 16:29
void MaxAngleWgt::updateOsgView()
{
	_osgWidget->updateView();
}

//@function: 界面和间隙推荐相关参数发生了改变   -- by mzq  2019/8/8 11:31
void MaxAngleWgt::slotSgstGapChanged()
{
	try
	{
		VoltType::Type voltType = VoltType::Type(_voltTypeCBx->currentIndex());
		LoopType::Type loopType = LoopType::Type(_loopTypeCBx->currentIndex());

		InsulatorType::Type insulatorType;
		switch (_cl->getClType())
		{
		case ClCate::MaxAlgle_I:
			insulatorType = InsulatorType::Type_I;
			break;
		case ClCate::MaxAlgle_V:
			insulatorType = InsulatorType::Type_V;
			break;
		default:
			break;
		}

		AltitudeType::Type altitudeType = AltitudeType::altitudeValueToType(_altitudeEdt->text().toDouble());

		HdlSgstGapLib sgstGapLib = ClWidget::getCoreInstance()->getSgstGapLib();
		std::vector<std::string> fourSugestGap;
		fourSugestGap.push_back(sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::Wind)->getGap());
		fourSugestGap.push_back(sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::InOverVolt)->getGap());
		fourSugestGap.push_back(sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::OutOverVolt)->getGap());
		fourSugestGap.push_back(sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::HotLineWork)->getGap());

		upDateSgstGap(fourSugestGap);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙值"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 显示错误   -- by mzq  2019/12/13 17:57
void MaxAngleWgt::slotErrDisplay()
{
	try
	{
		HdlCalcExcepts pException = _cl->getCalcExcepts();
		_pErrModel->updateModel(pException);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("错误信息展示"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 计算   -- by mzq  2019/12/13 17:58
void MaxAngleWgt::slotCalcProcess()
{
	try
	{
		writeDataFromForm();

		if (!_cl->hasUIDataError())
		{
			slotErrDisplay();
			_cl->calcProcess();
			_hasErr = false;
		}
		else
		{
			slotErrDisplay();
			_hasErr = true;
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙圆计算"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}
}