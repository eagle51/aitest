#include "StraightWgt.h"
#include "DataBase.h"
#include "Mdl_WireRelatPara.h"
#include "WireParamLib.h"
#include "WpaButtonDelegate.h"
#include "WireParam.h"
#include "MeteAreaDlg.h"
#include "MeteAreaLib.h"
#include "Mdl_VSpanCalSet.h"
#include "InsulatorParaDlg_I.h"
#include "Cl_Stright_I.h"
#include "WPA_Dlg.h"
#include "EnumLib.h"
#include "SgstGapLib.h"
#include <fstream>

#include <QHBoxLayout>
#include <QString>
#include <QLabel>
#include <QGroupBox>
#include "qlineedit.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "qradiobutton.h"
#include "qscrollarea.h"

#include "OdqInterfaces.h"  
#include "Ed\EdCommandStack.h"
#include "ExStringIO.h"
#include "Ed\EdCommandContext.h"
#include "WireAvgHLib_Dlg.h"
#include "VirBox\VirBoxChecker.h"
#include "Auxiliary.h"
#include "CalcPrincipleDlg.h"

StraightWgt::StraightWgt(int clType, std::string clName)
	:ClWidget(clType, clName)
{
	_dataUI_Stright = static_cast<DataUI_Stright*>(_cl->getDataUI().GetPointee());

	//绝缘子串对话框Btn
	_insulatorParaBtn = new QPushButton(QString::fromLocal8Bit("绝缘子串\n参数设置"));
	_insulatorParaBtn->setFixedSize(80, 60);
	_cadToolBar = NULL;
}

StraightWgt::~StraightWgt()
{

}

//@function: 用于OdqView类调取并显示   -- by mzq  2019/12/23 11:37
QToolBar* StraightWgt::getCadToolBar(QWidget *parent /*= 0/*父窗口*/)
{
	if (NULL == _cadToolBar)
		_cadToolBar = new QToolBar(parent);
	return _cadToolBar;
}

//@function:    -- by mzq  2019/5/28 22:06
void StraightWgt::initWgt()
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

QWidget* StraightWgt::initClDesignWgt()
{
	/*按钮层布局*/
	connect(_pReportBtn, SIGNAL(clicked()), this, SLOT(slotCreatReport()));
	connect(_pDrawClearanceBtn, SIGNAL(clicked()), this, SLOT(slotDrawCommand()));
	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addWidget(_clTypeTitle);
	btnLyt->addStretch();
	btnLyt->addWidget(_pReportBtn);
	btnLyt->addWidget(_pDrawClearanceBtn);
	btnLyt->setSpacing(20);
	btnLyt->setContentsMargins(8, 12, 8, 8);

	//CAD工具栏
	initCadToolBar();

	//基本信息
	QGroupBox *basicInfoGbx = this->initBasicInfoGbx();
	//basicInfoGbx->setFixedHeight(155);

	//计算参数信息
	QGroupBox *calcParaGbx = this->initCalcParaGbx();
	//calcParaGbx->setFixedHeight(205);

	//导线参数信息
	QGroupBox *wireRelatParaGbx = this->initWireRelatParaGbx();
	//wireRelatParaGbx->setFixedHeight(155);

	//气象区信息
	QGroupBox *meteAreaGbx = this->initMeteAreaGbx();
	//meteAreaGbx->setFixedHeight(185);

	//间隙值信息
	QGroupBox *gapValueGbx = this->initGapValueGbx();
	//gapValueGbx->setMinimumWidth(460);
	//gapValueGbx->setFixedHeight(205);

	//档距信息组合框
	QGroupBox *spanParaGbx = this->initSpanParaGbx();
	//spanParaGbx->setMinimumWidth(150);
	//spanParaGbx->setFixedHeight(125);

	//垂直档距计算设置组合框
	QGroupBox *vSpanCalSetGbx = this->initVSpanCalSetGbx();
	//vSpanCalSetGbx->setFixedHeight(125);

	//绝缘子串几何参数
	QGroupBox *insulatorParaGbx = new QGroupBox(QStringLiteral("绝缘子串参数"));
	QVBoxLayout *insulatorParaLyt = new QVBoxLayout();
	insulatorParaLyt->addWidget(_insulatorParaBtn);
	insulatorParaGbx->setLayout(insulatorParaLyt);

	//摇摆角信息，I串摇摆角信息，针对I串，VL串可隐藏
	QGroupBox *swingAngleGbx = this->initSwingAngleGbx();
	if (_cl->getClType()!=ClCate::StraightIStr)
		swingAngleGbx->hide();

	QGridLayout *gridLyt = new QGridLayout();//栅格为4*8
	gridLyt->addWidget(basicInfoGbx, 0, 0, 1, 4);
	gridLyt->addWidget(wireRelatParaGbx, 0, 4, 1, 4);
	gridLyt->addWidget(spanParaGbx, 1, 0, 1, 2);
	gridLyt->addWidget(vSpanCalSetGbx, 1, 2, 1, 6);
	gridLyt->addWidget(meteAreaGbx, 2, 0, 1, 6);
	gridLyt->addWidget(insulatorParaGbx, 2, 6, 1, 1);
	gridLyt->addWidget(swingAngleGbx, 2, 7, 1, 1);
	gridLyt->addWidget(calcParaGbx, 3, 0, 1, 3);
	gridLyt->addWidget(gapValueGbx, 3, 3, 1, 5);
	for (int columnIndex = 0; columnIndex < gridLyt->columnCount(); ++columnIndex)
	{
		gridLyt->setColumnStretch(columnIndex, 1);
	}
	gridLyt->setVerticalSpacing(20);

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
void StraightWgt::initCadToolBar()
{
	if (NULL == _cadToolBar)
		return;

	QStringList lstDirection;
	lstDirection << QStringLiteral("ext")
		<< QStringLiteral("top")
		<< QStringLiteral("bottom");

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

	if (ClCate::StraightIStr == _cl->getClType())//直线塔I串需要
	{
		_leftOrRightCmb = new QComboBox();
		_leftOrRightCmb->addItem(QString::fromLocal8Bit("左侧"));
		_leftOrRightCmb->addItem(QString::fromLocal8Bit("右侧"));
		_leftOrRightCmb->addItem(QString::fromLocal8Bit("镜像"));
		_leftOrRightCmb->setFixedWidth(60);
		_leftOrRightCmb->setView(new QListView());
		connect(_leftOrRightCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));
		_cadToolBar->addWidget(_leftOrRightCmb);
	}

	_cadToolBar->addSeparator();
	QLabel* plottingScaleLbl = new QLabel(QStringLiteral("  比例尺 1:"));
	_plottingScaleEdt = new QLineEdit();
	_plottingScaleEdt->setFixedWidth(40);
	_cadToolBar->addWidget(plottingScaleLbl);
	_cadToolBar->addWidget(_plottingScaleEdt);

	_cadToolBar->addSeparator();
	QLabel* isDimensionLbl = new QLabel(QStringLiteral("  标注设置:"));
	_isDimensionCbx = new QComboBox();
	_isDimensionCbx->setFixedWidth(120);
	for (unsigned int loop = 0; loop < DimStyle::getTypeLib().size(); ++loop)
		_isDimensionCbx->addItem(QString::fromLocal8Bit(DimStyle::typeToStr(DimStyle::getTypeLib().at(loop))));
	_cadToolBar->addWidget(isDimensionLbl);
	_cadToolBar->addWidget(_isDimensionCbx);
	connect(_isDimensionCbx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));
}

// @function: 
QGroupBox* StraightWgt::initBasicInfoGbx()
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
	_voltTypeBtn = new QPushButton();
	_voltTypeBtn->setToolTip(QString::fromLocal8Bit("单击选择电压等级"));
	QHBoxLayout *voltTypeLayout = new QHBoxLayout;
	voltTypeLayout->addWidget(_voltTypeBtn);
	QGroupBox *voltTypeGbx = new QGroupBox(QStringLiteral("电压等级"));
	voltTypeGbx->setLayout(voltTypeLayout);
	connect(_voltTypeBtn, SIGNAL(clicked()), this, SLOT(slotOpenWireAvgHLib_Dlg()));

	/*回路类型*/
	_loopTypeCBx = new QComboBox();
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
	connect(_loopTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*海拔高度*/
	_altitudeEdt = new QLineEdit("1000");
	_altitudeEdt->setToolTip(QString::fromLocal8Bit("输入海拔高度，海拔高度与间隙推荐值有关"));
	QVBoxLayout *altitudeLayout = new QVBoxLayout;
	altitudeLayout->addWidget(_altitudeEdt);
	QGroupBox *altitudeGbx = new QGroupBox(QStringLiteral("海拔高度(m)"));
	altitudeGbx->setLayout(altitudeLayout);
	connect(_altitudeEdt, SIGNAL(textChanged(const QString &)), this, SLOT(slotSgstGapChanged()));

	/*转角度数*/
	_rotatAngleEdt = new QLineEdit("0");
	_rotatAngleEdt->setToolTip(QString::fromLocal8Bit("输入转角度数，当线路带转角时，需要考虑张力在水平方向上的分量对风偏角的影响"));
	QVBoxLayout *rotatAngleLayout = new QVBoxLayout;
	rotatAngleLayout->addWidget(_rotatAngleEdt);
	QGroupBox *rotatAngleGbx = new QGroupBox(QStringLiteral("转角度数(度)"));
	rotatAngleGbx->setLayout(rotatAngleLayout);

	QGridLayout *gridLyt = new QGridLayout();//栅格为2*6
	gridLyt->addWidget(towNameGbx, 0, 0, 1, 3);
	gridLyt->addWidget(voltTypeGbx, 0, 3, 1, 3);
	gridLyt->addWidget(loopTypeGbx, 1, 0, 1, 2);
	gridLyt->addWidget(altitudeGbx, 1, 2, 1, 2);
	gridLyt->addWidget(rotatAngleGbx, 1, 4, 1, 2);

	basicInfoGbx->setLayout(gridLyt);
	return basicInfoGbx;
}

// @function: 
QGroupBox* StraightWgt::initCalcParaGbx()
{
	QGroupBox *heightParaGbx = new QGroupBox(QString::fromLocal8Bit("计算参数"));

	/*导线平均高*/
	_wireAvgHEdit = new QLineEdit("0");
	_wireAvgHEdit->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/导线平均高.png' />").arg(QCoreApplication::applicationDirPath()));
	_wireAvgHEdit->setFixedHeight(20);
	QVBoxLayout *wireAvgHLayout = new QVBoxLayout;
	wireAvgHLayout->addWidget(_wireAvgHEdit);
	QGroupBox *wireAvgHGbx = new QGroupBox(QStringLiteral("导线平均高 (m)"));
	wireAvgHGbx->setLayout(wireAvgHLayout);
	wireAvgHGbx->setFixedWidth(130);

	/*杆塔计算相呼高*/
	_towerCallHEdit = new QLineEdit("0");
	_towerCallHEdit->setToolTip(QString::fromLocal8Bit("请输入杆塔计算相呼高，该高度减一半串长用于计算绝缘子串风荷载，该高度与计算导线风荷载无关"));
	_towerCallHEdit->setFixedHeight(20);
	QVBoxLayout *towerCallHLayout = new QVBoxLayout;
	towerCallHLayout->addWidget(_towerCallHEdit);
	QGroupBox *towerCallHGbx = new QGroupBox(QStringLiteral("杆塔计算相呼高 (m)"));
	towerCallHGbx->setLayout(towerCallHLayout);
	towerCallHGbx->setFixedWidth(130);

	/*地形粗糙度*/
	_terrRoughtCmb = new QComboBox();
	_terrRoughtCmb->setToolTip(QString::fromLocal8Bit("单击选择地面粗糙度类别"));
	_terrRoughtCmb->setFixedHeight(20);
	_terrRoughtCmb->addItem(QString::fromLocal8Bit("A"));
	_terrRoughtCmb->addItem(QString::fromLocal8Bit("B"));
	_terrRoughtCmb->addItem(QString::fromLocal8Bit("C"));
	_terrRoughtCmb->addItem(QString::fromLocal8Bit("D"));
	_terrRoughtCmb->setView(new QListView());
	QVBoxLayout *terrRoughtCmbLayout = new QVBoxLayout;
	terrRoughtCmbLayout->addWidget(_terrRoughtCmb);
	QGroupBox *terrRoughtCmbGbx = new QGroupBox(QStringLiteral("地面粗糙度类别"));
	terrRoughtCmbGbx->setLayout(terrRoughtCmbLayout);
	terrRoughtCmbGbx->setMinimumWidth(110);

	/*第一列布局*/
	QVBoxLayout *v1Lyt = new QVBoxLayout();
	v1Lyt->addWidget(wireAvgHGbx);
	v1Lyt->addWidget(towerCallHGbx);
	v1Lyt->addWidget(terrRoughtCmbGbx);

	/*计算原则*/
	QPushButton *calcPrincipleBtn = new QPushButton(QStringLiteral("计算原则设置"));
	calcPrincipleBtn->setFixedWidth(120);
	connect(calcPrincipleBtn, SIGNAL(clicked()), this, SLOT(slotCalcPrincipleSet()));

	/*规范类别*/
	_codeTypeCmb = new QComboBox();
	_codeTypeCmb->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/规范说明.png' />").arg(QCoreApplication::applicationDirPath()));
	_codeTypeCmb->setView(new QListView());
	QVBoxLayout *codeTypeCmbLayout = new QVBoxLayout;
	codeTypeCmbLayout->addWidget(_codeTypeCmb);
	QGroupBox *codeTypeCmbGbx = new QGroupBox(QStringLiteral("规范类别"));
	codeTypeCmbGbx->setLayout(codeTypeCmbLayout);
	connect(_codeTypeCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCodeTypeChanged()));

	/*风压不均匀系数*/
	_alphaAutoCalcRbtn = new QRadioButton(QStringLiteral("根据风速自动计算"));
	_alphaAutoCalcRbtn->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/风压不均匀系数说明.png' />").arg(QCoreApplication::applicationDirPath()));
	_alphaInputRbtn = new QRadioButton(QStringLiteral("手动输入:"));
	_alphaInputRbtn->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/风压不均匀系数说明.png' />").arg(QCoreApplication::applicationDirPath()));
	_alphaEdit = new QLineEdit("0.75");
	QHBoxLayout *InputRbtnLyt = new QHBoxLayout();
	InputRbtnLyt->addWidget(_alphaInputRbtn);
	InputRbtnLyt->addWidget(_alphaEdit);

	QVBoxLayout *RbtnLyt = new QVBoxLayout();
	RbtnLyt->addWidget(_alphaAutoCalcRbtn);
	RbtnLyt->addLayout(InputRbtnLyt);
	_alphaGBox = new QGroupBox(QStringLiteral("风压不均匀系数"));
	_alphaGBox->setLayout(RbtnLyt);
	_alphaGBox->setMinimumWidth(145);
	connect(_alphaAutoCalcRbtn, SIGNAL(toggled(bool)), this, SLOT(slotOnClickAlphaAutoCalc()));
	connect(_alphaInputRbtn, SIGNAL(toggled(bool)), this, SLOT(slotOnClickAlphaInput()));

	/*第二列布局*/
	QVBoxLayout *v2Lyt = new QVBoxLayout();
	v2Lyt->addWidget(calcPrincipleBtn);
	v2Lyt->addWidget(codeTypeCmbGbx);
	v2Lyt->addWidget(_alphaGBox);
	v2Lyt->addStretch();

	/*整体布局*/
	QHBoxLayout *Lyt = new QHBoxLayout();
	Lyt->addLayout(v1Lyt);
	Lyt->addLayout(v2Lyt);
	heightParaGbx->setLayout(Lyt);
	return heightParaGbx;
}

// @function: 导线信息参数Gbx
QGroupBox* StraightWgt::initWireRelatParaGbx()
{
	QGroupBox *groundWaterGbx = new QGroupBox(QString::fromLocal8Bit("导线相关参数"));
	_pWireParaTbl = new ClTableWgt(false, groundWaterGbx);
	_pWireParaTbl->horizontalHeader()->hide();
	QStringList wireParaHeaders;
	wireParaHeaders << QStringLiteral(" 导线型号 ")
		<< QStringLiteral(" 安全系数 ")
		<< QStringLiteral(" 新线系数 ")
		<< QStringLiteral(" 平均运行张力系数 ");
	_pWireParaTbl->setRowCount(wireParaHeaders.size());
	_pWireParaTbl->setColumnCount(1);
	_pWireParaTbl->setVerticalHeaderLabels(wireParaHeaders);
	_pWireParaTbl->setFixedHeight(_pWireParaTbl->rowHeight(0) * _pWireParaTbl->rowCount() + 2);

	_pWireParaTbl->setItem(0, 0, new QTableWidgetItem("---"));
	_pWireParaTbl->setItemDelegateForRow(0, new WpaButtonDelegate());
	_pWireParaTbl->setItem(1, 0, new QTableWidgetItem("2.5"));
	_pWireParaTbl->setItem(2, 0, new QTableWidgetItem("0.95"));
	_pWireParaTbl->setItem(3, 0, new QTableWidgetItem("0.25"));
	_pWireParaTbl->setFontPosition(_pWireParaTbl->rowCount(), _pWireParaTbl->columnCount());

	_pWireParaTbl->item(0, 0)->setToolTip(QString::fromLocal8Bit("双击可弹出导线库，在库中可以选择对应的导线型号 \n如果需要新建导线型号，请在菜单栏中的点击参数->导线参数库->插入电线属性进行添加"));
	_pWireParaTbl->item(1, 0)->setToolTip(QString::fromLocal8Bit("双击设置导线安全系数"));
	_pWireParaTbl->item(2, 0)->setToolTip(QString::fromLocal8Bit("双击设置导线新线系数，一般为0.95"));
	_pWireParaTbl->item(3, 0)->setToolTip(QString::fromLocal8Bit("双击设置导线平均运行张力系数"));

	QGridLayout *gridLyt = new QGridLayout();
	gridLyt->addWidget(_pWireParaTbl, 0, 0);
	groundWaterGbx->setLayout(gridLyt);
	return groundWaterGbx;
}

//@function: 气象区信息组合框
QGroupBox* StraightWgt::initMeteAreaGbx()
{
	QGroupBox *meteInfoGbx = new QGroupBox(QString::fromLocal8Bit("气象区"));
	_meteInfoView = new StTable_View(this);	//气象区view
	_meteInfoView->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/气象区提示.png' />").arg(QCoreApplication::applicationDirPath()));
	_meteInfoView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_meteInfoModel = new SingleMeteRegionModel(0, false);
	_meteInfoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_meteInfoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QLabel *MeteNameLable = new QLabel(QStringLiteral("气象区名称:"));//气象区名称lable
	_meteName = new QLabel();//气象区名称
	QLabel *windRefH_Lbl = new QLabel(QStringLiteral("    大风风速基准高度 (m):"));//气象区名称lable
	_windRefH_Edt = new QLineEdit("10");
	_windRefH_Edt->setFixedWidth(60);
	_windRefH_Edt->setToolTip(QString::fromLocal8Bit("为气象区中的大风风速基准高度，一般为10m，如果用户有特殊情况，可对该值进行修改"));
	_selectMete = new QPushButton(QStringLiteral("选择气象区"));	//选择气象区按钮
	connect(_selectMete, SIGNAL(clicked()), this, SLOT(slotselectMeteDlg()));
	QHBoxLayout *selectMeteLayout = new QHBoxLayout;
	selectMeteLayout->addWidget(MeteNameLable);
	selectMeteLayout->addWidget(_meteName);
	selectMeteLayout->addWidget(windRefH_Lbl);
	selectMeteLayout->addWidget(_windRefH_Edt);
	selectMeteLayout->addStretch();
	selectMeteLayout->addWidget(_selectMete);
	QVBoxLayout *meteInfoBoxLayout = new QVBoxLayout;
	meteInfoBoxLayout->addLayout(selectMeteLayout);
	meteInfoBoxLayout->addWidget(_meteInfoView);
	meteInfoGbx->setLayout(meteInfoBoxLayout);
	return meteInfoGbx;
}

// @function: 档距信息组合框
QGroupBox* StraightWgt::initSpanParaGbx()
{
	QGroupBox *spanParaGbx = new QGroupBox(QString::fromLocal8Bit("档距"));

	_spanParaTbl = new ClTableWgt(false,spanParaGbx);
	_spanParaTbl->horizontalHeader()->hide();
	QStringList headers;
	headers << QStringLiteral(" 代表档距 (m) ")
		<< QStringLiteral(" 水平档距 (m) ")
		<< QStringLiteral(" Kv系数 ");
	_spanParaTbl->setRowCount(headers.size());
	_spanParaTbl->setColumnCount(1);
	_spanParaTbl->setVerticalHeaderLabels(headers);
	_spanParaTbl->setFixedHeight(_spanParaTbl->rowHeight(0) * _spanParaTbl->rowCount() + 2);

	_spanParaTbl->setItem(0, 0, new QTableWidgetItem("350"));
	_spanParaTbl->setItem(1, 0, new QTableWidgetItem("380"));
	_spanParaTbl->setItem(2, 0, new QTableWidgetItem("0.85"));
	_spanParaTbl->setFontPosition(_spanParaTbl->rowCount(), _spanParaTbl->columnCount());

	_spanParaTbl->item(0, 0)->setToolTip(QString::fromLocal8Bit("双击设置代表档距，用于计算导线张力"));
	_spanParaTbl->item(1, 0)->setToolTip(QString::fromLocal8Bit("双击设置代表档距，该水平档距为杆塔水平档距"));
	_spanParaTbl->item(2, 0)->setToolTip(QString::fromLocal8Bit("双击设置Kv系数，用于计算垂直档距"));

	QGridLayout *gridLyt = new QGridLayout();
	gridLyt->addWidget(_spanParaTbl, 0, 0);
	spanParaGbx->setLayout(gridLyt);
	return spanParaGbx;
}

// @function: 垂直档距计算设置方式组合框
QGroupBox* StraightWgt::initVSpanCalSetGbx()
{
	_autoSetVerSpanRBtn = new QRadioButton(QString::fromLocal8Bit("自动计算垂直档距"));
	_sameVerSpanRBtn = new QRadioButton(QString::fromLocal8Bit("各工况垂直档距相同"));
	_partSetVerSpanRBtn = new QRadioButton(QString::fromLocal8Bit("各工况分别设置加权系数"));

	_autoSetVerSpanRBtn->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/垂直档距-自动计算垂直档距.png' />").arg(QCoreApplication::applicationDirPath()));
	_sameVerSpanRBtn->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/垂直档距-各工况垂直档距相同.png' />").arg(QCoreApplication::applicationDirPath()));
	_partSetVerSpanRBtn->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/垂直档距-各工况分别设置加权系数.png' />").arg(QCoreApplication::applicationDirPath()));

	QHBoxLayout *VerSpanRBtnLayout = new QHBoxLayout;
	VerSpanRBtnLayout->addWidget(_autoSetVerSpanRBtn);
	VerSpanRBtnLayout->addWidget(_sameVerSpanRBtn);
	VerSpanRBtnLayout->addWidget(_partSetVerSpanRBtn);

	_maxVerSpanLabel = new QLabel(QString::fromLocal8Bit("最大弧垂控制工况:"));
	_maxVerSpanComboxbox = new QComboBox();
	_maxVerSpanComboxbox->setMinimumWidth(90);
	QStringList SplitNumTypeList;
	SplitNumTypeList << QString::fromLocal8Bit("最高气温") << QString::fromLocal8Bit("覆冰");
	_maxVerSpanComboxbox->addItems(SplitNumTypeList);
	_maxVerSpanComboxbox->setView(new QListView());
	_maxVerSpanComboxbox->setToolTip(QString::fromLocal8Bit("单击设置最大弧垂控制工况，即排杆定位时的最大弧垂对应的工况类型"));

	QHBoxLayout *maxVerSpanLayOut = new QHBoxLayout;
	maxVerSpanLayOut->addWidget(_maxVerSpanLabel);
	maxVerSpanLayOut->addWidget(_maxVerSpanComboxbox);
	maxVerSpanLayOut->addStretch();
	maxVerSpanLayOut->setMargin(0);
	_maxVerSpanWidget = new QWidget();
	_maxVerSpanWidget->setLayout(maxVerSpanLayOut);

	_partSetVerSpanWidget = new ClTableWgt(false, this);
	_partSetVerSpanWidget->initialSize(1, 8);
	QStringList partSetVerSpanWidgetHeaderStr;
	for (int iloop = 0; iloop < _partSetVerSpanWidget->columnCount(); ++iloop)
		partSetVerSpanWidgetHeaderStr << QString::fromLocal8Bit(MeteCate::typeToStr(MeteCate::Type(iloop)));
	_partSetVerSpanWidget->setHorizontalHeaderLabels(partSetVerSpanWidgetHeaderStr);

	QStringList strVHeaderInfos;
	strVHeaderInfos << QStringLiteral(" 加权系数 ");
	_partSetVerSpanWidget->setVerticalHeaderLabels(strVHeaderInfos);
	_partSetVerSpanWidget->setFixedHeight(_partSetVerSpanWidget->horizontalHeader()->height() + _partSetVerSpanWidget->rowHeight(0) * _partSetVerSpanWidget->rowCount() + 2);

	QVBoxLayout *MaxVerSpanLayout = new QVBoxLayout();
	MaxVerSpanLayout->addWidget(_maxVerSpanWidget);
	MaxVerSpanLayout->addWidget(_partSetVerSpanWidget);
	MaxVerSpanLayout->addStretch();
	MaxVerSpanLayout->setContentsMargins(0, 12, 0, 0);

	QWidget* spanInfoWidget = new QWidget();
	spanInfoWidget->setLayout(MaxVerSpanLayout);
	//spanInfoWidget->setFixedHeight(90);

	QVBoxLayout *VerSpanInfoLayout = new QVBoxLayout;
	VerSpanInfoLayout->addLayout(VerSpanRBtnLayout);
	VerSpanInfoLayout->addWidget(spanInfoWidget);
	VerSpanInfoLayout->addStretch();
	QGroupBox *vSpanCalSetGbx = new QGroupBox(QString::fromLocal8Bit("垂直档距计算设置"));
	vSpanCalSetGbx->setLayout(VerSpanInfoLayout);

	connect(_autoSetVerSpanRBtn, SIGNAL(toggled(bool)), this, SLOT(slotOnClickSetVerSpan()));
	connect(_sameVerSpanRBtn, SIGNAL(toggled(bool)), this, SLOT(slotOnClickSetVerSpan()));
	connect(_partSetVerSpanRBtn, SIGNAL(toggled(bool)), this, SLOT(slotOnClickSetVerSpan()));

	return vSpanCalSetGbx;
}

//@ function: 从数据库把数据写入界面
void StraightWgt::readDataToForm()
{
	try
	{
		//间隙圆类型标题
		_clTypeTitle->setText(QString::fromLocal8Bit(" 类型：") + QString::fromLocal8Bit(SmartCl::ClCate::typeToStr(_cl->getClType())));

		//基本信息模块
		this->readBasicInfoData(_dataUI_Stright->getBasicInfo());

		//电压高度相关信息模块
		this->readCalcReleParaData(_dataUI_Stright->getCalcRelePara());

		//档距信息模块
		this->readSpanParaData(_dataUI_Stright->getSpanPara());

		//垂直档距计算设置模块
		this->readVSpanCalSetData(_dataUI_Stright->getVSpanCalSet(), _dataUI_Stright->getSpanPara());

		//气象工况相关参数模块
		this->readMeteAreaData(_dataUI_Stright->getMeteAreaPara());

		//导线相关参数模块
		this->readWireRelatData(_dataUI_Stright->getWireRelatPara());

		//对应不同类型间隙圆绝缘子串弹出窗口参数
		this->readChildClDataToForm();

		//设置绘图相关
		_plottingScaleEdt->setText(QString::number(_dataUI_Stright->getPlottingScale()));
		_isDimensionCbx->setCurrentText(QString::fromLocal8Bit(DimStyle::typeToStr(_dataUI_Stright->getDimStyleTyle())));

	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("界面数据写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 基本信息
void StraightWgt::readBasicInfoData(SmartCl::HdlMdl_BasicPara pBasicPara)
{
	try
	{
		_towNameEdt->setText(QString::fromLocal8Bit(pBasicPara->getTowName()));
		_voltTypeBtn->setText(QString::fromLocal8Bit(VoltType::typeToStr(pBasicPara->getVoltType())));
		_loopTypeCBx->setCurrentIndex(LoopType::Type(pBasicPara->getLoopType()));
		_altitudeEdt->setText(QString::number(pBasicPara->getAltitude()));
		_rotatAngleEdt->setText(QString::number(pBasicPara->getRotatAngle()));

		slotSgstGapChanged();//改变推荐间隙值
		slotVoltChanged();//改变规范类别
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("基本信息数据读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::readCalcReleParaData(SmartCl::HdlMdl_CalcRelePara pCalcRelePara)
{
	try
	{
		_wireAvgHEdit->setText(QString::number(pCalcRelePara->getWireAvgH()));
		_towerCallHEdit->setText(QString::number(pCalcRelePara->getTowerCallH()));
		_terrRoughtCmb->setCurrentIndex(pCalcRelePara->getTerrRought());
		_codeTypeCmb->setCurrentText(QString::fromLocal8Bit(CodeCate::typeToStr(pCalcRelePara->getCodeType())));
		slotCodeTypeChanged();
		if (pCalcRelePara->getCodeType() == CodeCate::GB50545_2010)
		{
			if (pCalcRelePara->getIsInputAlpha())
			{
				_alphaInputRbtn->setChecked(true);
				_alphaEdit->setText(QString::number(pCalcRelePara->getAlpha()));
			}


			else
			{
				_alphaAutoCalcRbtn->setChecked(true);
				_alphaEdit->setText(QString::number(pCalcRelePara->getAlpha()));
			}
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("计算参数信息数据读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::readSpanParaData(SmartCl::HdlMdl_SpanPara pSpanPara)
{
	try
	{
		_spanParaTbl->item(0, 0)->setText(QString::number(pSpanPara->getEqvSpan()));
		_spanParaTbl->item(1, 0)->setText(QString::number(pSpanPara->getHSpan()));
		_spanParaTbl->item(2, 0)->setText(QString::number(pSpanPara->getKvCoef()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("档距信息数据读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::readVSpanCalSetData(SmartCl::HdlMdl_VSpanCalSet pVSpanCalSet, SmartCl::HdlMdl_SpanPara pSpanPara)
{
	try
	{
		if (pVSpanCalSet->getCalMannerCate() == VSpanCalMannerCate::Auto)
		{
			_autoSetVerSpanRBtn->setChecked(true);
			MeteCate::Type baseLC = pSpanPara->getCorLoadCate_MaxVSpan();
			_maxVerSpanComboxbox->setCurrentIndex(baseLC != MeteCate::HighTemp);
		}
		else if (pVSpanCalSet->getCalMannerCate() == VSpanCalMannerCate::ByWeight)
		{
			_partSetVerSpanRBtn->setChecked(true);
			for (int iloop = 0; iloop < _partSetVerSpanWidget->columnCount(); ++iloop)
			{
				if (iloop < pVSpanCalSet->getSpanWeightCount())
				{
					HdlVSpanCalWeight pVSpanCalWeight = pVSpanCalSet->getAt(iloop);
					_partSetVerSpanWidget->item(0, iloop)->setText(QString::number(pVSpanCalWeight->getVSpanCalcWeight(), 'f', 2));
				}
				else
				{
					_partSetVerSpanWidget->item(0, iloop)->setText(QString::number(1.00));
				}
			}
		}
		else
		{
			_sameVerSpanRBtn->setChecked(true);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("垂直档距信息设置"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::readMeteAreaData(SmartCl::HdlMdl_MetaAreaPara pMeteArea)
{
	try
	{
		HdlSingleMeteArea pDataMeteInfo = pMeteArea->getSingleMeteArea();
		_meteName->setText(QString::fromLocal8Bit(pDataMeteInfo->getMeteAreaName()));
		_meteInfoModel->updateModel(pDataMeteInfo);
		_meteInfoModel->changeLanguage();
		_meteInfoView->setModel(_meteInfoModel);
		_windRefH_Edt->setText(QString::number(pMeteArea->getWindRefH()));

		/*设置表格高度*/
		_meteInfoView->setFixedHeight(_meteInfoView->horizontalHeader()->height() + _meteInfoView->rowHeight(0) * _meteInfoModel->rowCount() + 2);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("气象区信息设置"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::readWireRelatData(SmartCl::HdlMdl_WireRelatPara pWireRelatPara)
{
	try
	{
		_pWireParaTbl->item(0, 0)->setText(QString::fromLocal8Bit(pWireRelatPara->getWPA()->getName().c_str()));
		_pWireParaTbl->item(1, 0)->setText(QString::number(pWireRelatPara->getSaftyCof()));
		_pWireParaTbl->item(2, 0)->setText(QString::number(pWireRelatPara->getNewWireCof()));
		_pWireParaTbl->item(3, 0)->setText(QString::number(pWireRelatPara->getAvgCof()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("导线参数信息设置"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::writeDataFromForm()
{
	try
	{
		//基本信息模块
		this->writeBasicInfoData(_dataUI_Stright->getBasicInfo());

		//电压高度参数信息模块
		this->writeCalcReleParaData(_dataUI_Stright->getCalcRelePara());

		//档距信息模块
		this->writeSpanParaData(_dataUI_Stright->getSpanPara());

		//垂直档距计算设置模块
		this->writeVSpanCalSetData(_dataUI_Stright->getVSpanCalSet(), _dataUI_Stright->getSpanPara());

		//气象区模块
		this->writeMeteAreaData(_dataUI_Stright->getMeteAreaPara());

		//导线参数模块
		this->writeWireRelatData(_dataUI_Stright->getWireRelatPara());

		//对应不同类型间隙圆绝缘子串弹出窗口参数
		this->writeChildClDataFromForm();

		//设置绘图比例尺
		_dataUI_Stright->setPlottingScale(_plottingScaleEdt->text().toDouble());
		_dataUI_Stright->setDimStyleTyle(DimStyle::strToType(_isDimensionCbx->currentText().toLocal8Bit().toStdString()));

		/*进行计算原则提示*/
		if (getCoreInstance()->getSoftWareType() == SoftWareType::FoShanYuan)
		{
			if (_dataUI_Stright->getCalcRelePara()->getCalcPrincipleType_wire() != CalcPrinciple_Wire::FuoShan
				|| _dataUI_Stright->getCalcRelePara()->getCalcPrincipleType_insulator() != CalcPrinciple_Insulator::FuoShan)
			{
				QMessageBox::warning(this, QString::fromLocal8Bit("计算原则"), 
									 QString::fromLocal8Bit("当前计算原则非佛山院计算原则，请注意核查，可在计算原则设置中进行调整！"), QMessageBox::Yes);
			}
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("界面数据写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 写入基本信息
void StraightWgt::writeBasicInfoData(SmartCl::HdlMdl_BasicPara pBasicPara)
{
	try
	{
		string name = _towNameEdt->text().toLocal8Bit();
		pBasicPara->setTowName(name.c_str());
		pBasicPara->setTowType(TowerType::Suspensive);
		pBasicPara->setVoltType(VoltType::strToType(_voltTypeBtn->text().toLocal8Bit().toStdString()));
		pBasicPara->setLoopType(LoopType::Type(_loopTypeCBx->currentIndex()));
		pBasicPara->setAltitude(_altitudeEdt->text().toDouble());
		pBasicPara->setRotatAngle(_rotatAngleEdt->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("基本信息数据写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::writeCalcReleParaData(SmartCl::HdlMdl_CalcRelePara pCalcRelePara)
{
	try
	{
		pCalcRelePara->setWireAvgH(_wireAvgHEdit->text().toDouble());
		pCalcRelePara->setTowerCallH(_towerCallHEdit->text().toDouble());

		pCalcRelePara->setTerrRought(TerrainRough::Type(_terrRoughtCmb->currentIndex()));
		pCalcRelePara->setCodeType(CodeCate::strToType(_codeTypeCmb->currentText().toLocal8Bit().toStdString()));
		if (pCalcRelePara->getCodeType() == CodeCate::GB50545_2010)
		{
			if (_alphaInputRbtn->isChecked())
			{
				pCalcRelePara->setIsInputAlpha(true);
				pCalcRelePara->setAlpha(_alphaEdit->text().toDouble());
			}
			else
			{
				pCalcRelePara->setIsInputAlpha(false);
				pCalcRelePara->setAlpha(_alphaEdit->text().toDouble());
			}
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("电压高度参数信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::writeSpanParaData(SmartCl::HdlMdl_SpanPara pSpanPara)
{
	try
	{
		pSpanPara->setEqvSpan(_spanParaTbl->item(0, 0)->text().toDouble());
		pSpanPara->setHSpan(_spanParaTbl->item(1, 0)->text().toDouble());
		pSpanPara->setKvCoef(_spanParaTbl->item(2, 0)->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("档距信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::writeVSpanCalSetData(SmartCl::HdlMdl_VSpanCalSet pVSpanCalSet, SmartCl::HdlMdl_SpanPara pSpanPara)
{
	try
	{
		if (_autoSetVerSpanRBtn->isChecked())
		{
			pVSpanCalSet->setCalMannerCate(VSpanCalMannerCate::Auto);
			if (_maxVerSpanComboxbox->currentIndex() == 0)
			{
				pSpanPara->setCorMeteCate_MaxVSpan(MeteCate::HighTemp);
			}
			else
			{
				pSpanPara->setCorMeteCate_MaxVSpan(MeteCate::Ice);
			}
		}
		else if (_sameVerSpanRBtn->isChecked())
		{
			pVSpanCalSet->setCalMannerCate(VSpanCalMannerCate::Equal);
		}
		else
		{
			pVSpanCalSet->setCalMannerCate(VSpanCalMannerCate::ByWeight);
			pVSpanCalSet->clearVSpanCalWeiLib();
			for (int iloop = 0; iloop < _partSetVerSpanWidget->columnCount(); ++iloop)
			{
				double value = _partSetVerSpanWidget->item(0, iloop)->text().toDouble();
				HdlVSpanCalWeight pVSpanCalWeight = new VSpanCalWeight(MeteCate::Type(iloop), value);
				pVSpanCalSet->addToVSpanWeiLib(pVSpanCalWeight);
			}
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("垂直档距计算写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::writeMeteAreaData(SmartCl::HdlMdl_MetaAreaPara pMeteArea)
{
	try
	{
		pMeteArea->setSingleMeteArea(_meteInfoModel->getCurMtAreaTpl());
		pMeteArea->setWindRefH(_windRefH_Edt->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("气象区信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt::writeWireRelatData(SmartCl::HdlMdl_WireRelatPara pWireRelatPara)
{
	try
	{
		HdlWireParam wpa = getCoreInstance()->getWireParamLib()->find(_pWireParaTbl->item(0, 0)->text().toLocal8Bit().toStdString());
		pWireRelatPara->setWpa(wpa);
		pWireRelatPara->setSaftyCof(_pWireParaTbl->item(1, 0)->text().toDouble());
		pWireRelatPara->setNewWireCof(_pWireParaTbl->item(2, 0)->text().toDouble());
		pWireRelatPara->setAvgCof(_pWireParaTbl->item(3, 0)->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("导线参数信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 规范类别改变   -- by mzq  2019/6/5 11:38
void StraightWgt::slotCodeTypeChanged()
{
	if (_codeTypeCmb->currentText().toLocal8Bit().toStdString() == CodeCate::typeToStr(CodeCate::GB50545_2010))
	{
		_alphaGBox->setVisible(true);
	}
	else
	{
		_alphaGBox->setVisible(false);
	}
}

//@function: 点击自动计算风压不均匀系数   -- by mzq  2019/8/5 18:08
void StraightWgt::slotOnClickAlphaAutoCalc()
{
	_alphaEdit->setEnabled(false);
}

//@function: 点击手动输入风压不均匀系数   -- by mzq  2019/8/5 18:08
void StraightWgt::slotOnClickAlphaInput()
{
	_alphaEdit->setEnabled(true);
}

//@function: 计算原则设置   -- by mzq 2023/2/20 17:17
void StraightWgt::slotCalcPrincipleSet()
{
	writeCalcReleParaData(_dataUI_Stright->getCalcRelePara());
	CalcPrincipleDlg dlg(_dataUI_Stright->getCalcRelePara(),getCoreInstance()->getSoftWareType(), this);
	dlg.exec();
}

//进入气象区选择对话框
void StraightWgt::slotselectMeteDlg()
{
	HdlSingleMeteArea pTmpl = _meteInfoModel->getCurMtAreaTpl();

	MeteAreaDlg tempMACDlg(_meteName->text());
	if (tempMACDlg.exec())
	{
		pTmpl = tempMACDlg.getCurMtAreaTpl();
	}
	_meteInfoModel->updateModel(pTmpl);
	_meteInfoModel->changeLanguage();
	string cureName = pTmpl->getMeteAreaName();
	_meteName->setText(QString::fromLocal8Bit(cureName.data()));
}

// @function: 垂直档距设置
void StraightWgt::slotOnClickSetVerSpan()
{
	if (_autoSetVerSpanRBtn->isChecked())
	{
		_maxVerSpanWidget->setVisible(true);
		_partSetVerSpanWidget->setVisible(false);
	}
	else if (_sameVerSpanRBtn->isChecked())
	{
		_maxVerSpanWidget->setVisible(false);
		_partSetVerSpanWidget->setVisible(false);
	}
	else
	{
		_maxVerSpanWidget->setVisible(false);
		_partSetVerSpanWidget->setVisible(true);
		HdlMdl_VSpanCalSet vspancalsetdata = new Mdl_VSpanCalSet();// to do 
		if (vspancalsetdata.isNull())
			vspancalsetdata = new Mdl_VSpanCalSet();// to do 
		for (int iloop = 0; iloop < _partSetVerSpanWidget->columnCount(); ++iloop)
		{
			if (iloop < vspancalsetdata->getSpanWeightCount())
			{
				HdlVSpanCalWeight pVSpanCalWeight = vspancalsetdata->getAt(iloop);
				_partSetVerSpanWidget->item(0, iloop)->setText(QString::number(pVSpanCalWeight->getVSpanCalcWeight(), 'f', 2));
			}
			else
			{
				_partSetVerSpanWidget->item(0, iloop)->setText(QString::number(1.00));
			}
		}
	}
}

// @function: 计算  -mzq 2019/5/23 13:46
void StraightWgt::slotCalcProcess()
{
	if (!Auxiliary::isEqual(getVirBoxCheckerInstance()->virBoxPi, 3.1415926535))
		return;

	if (!getVirBoxCheckerInstance()->safeLoginFlag)
		return;

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

//@function: 生成报告   -- by mzq  2019/8/4 23:00
void StraightWgt::slotCreatReport(bool isOpenReportMsg)
{
	slotCalcProcess();
	if (_hasErr)
		return;

	QString clName = QString::fromLocal8Bit(_cl->getClName().c_str());
	getIApp()->getMainWindow()->statusBar()->showMessage(QString::fromLocal8Bit("正在生成 “%1” 间隙圆计算书，请耐心等候......").arg(clName), 500000);
	QApplication::setOverrideCursor(Qt::WaitCursor);

	try
	{
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

// @function: 
void StraightWgt::slotDrawCommand()
{
	slotCalcProcess();

	HdlCl currentCl = getCoreInstance()->getClByName(_cl->getClName());
	QString clName = QString::fromLocal8Bit(_cl->getClName().c_str());
	const char* clType = SmartCl::ClCate::typeToStr(currentCl->getClType());

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

	getIConsole()->executeCommand("zoom ext");//调整视图
	QApplication::restoreOverrideCursor();
}

//@ function: 错误显示
void StraightWgt::slotErrDisplay()
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

//@function: 界面和间隙推荐相关参数发生了改变   -- by mzq  2019/8/8 11:31
void StraightWgt::slotSgstGapChanged()
{
	try
	{
		VoltType::Type voltType = VoltType::strToType(_voltTypeBtn->text().toLocal8Bit().toStdString());
		LoopType::Type loopType = LoopType::Type(_loopTypeCBx->currentIndex());

		InsulatorType::Type insulatorType;
		switch (_cl->getClType())
		{
		case ClCate::StraightIStr:
			insulatorType = InsulatorType::Type_I;
			break;
		case ClCate::StraightVStr:
		case ClCate::StraightLStr:
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

//@function: 绘制间隙圆以及生成计算书，用于一键生成图纸以及计算书功能   -- by mzq  2019/9/2 18:34
void StraightWgt::drawClAndCreatReport()
{
	slotCreatReport(false);//生成报告
	slotDrawCommand();//绘制间隙圆
}
// @ function: 查看导线平均高度库对话框  --- by ljn 2020/1/19 19:24
void StraightWgt::slotOpenWireAvgHLib_Dlg()
{
	WireAvgHLib_Dlg dlg(false);//初始化对象  导线平均高度对话框信息查看
	dlg.setCurrentTableRow(_voltTypeBtn->text());
	if (dlg.exec() == QDialog::Accepted)//如果对话框被接受
	{
		_voltTypeBtn->setText(dlg.getSelectedVoltType());   //按钮的名称为选择的电压等级名称
		slotSgstGapChanged();
		slotVoltChanged();
	}
}

//@function: 电压等级发生了改变   -- by mzq 2020/11/23 19:07
void StraightWgt::slotVoltChanged()
{
	if (VoltType::AC10 == VoltType::strToType(_voltTypeBtn->text().toLocal8Bit().toStdString())
		|| VoltType::AC35 == VoltType::strToType(_voltTypeBtn->text().toLocal8Bit().toStdString())
		|| VoltType::AC66 == VoltType::strToType(_voltTypeBtn->text().toLocal8Bit().toStdString()))
	{
		_codeTypeCmb->clear();
		_codeTypeCmb->addItem(QString::fromLocal8Bit(CodeCate::typeToStr(CodeCate::GB50061_2010)));
	}
	else
	{
		if ((CodeCate::GB50545_2010 != CodeCate::strToType(_codeTypeCmb->currentText().toLocal8Bit().toStdString())
			&& CodeCate::DLT5582_2020 != CodeCate::strToType(_codeTypeCmb->currentText().toLocal8Bit().toStdString()))
			|| _codeTypeCmb->count()==0)
		{
			_codeTypeCmb->clear();
			_codeTypeCmb->addItem(QString::fromLocal8Bit(CodeCate::typeToStr(CodeCate::GB50545_2010)));
			_codeTypeCmb->addItem(QString::fromLocal8Bit(CodeCate::typeToStr(CodeCate::DLT5582_2020)));
			_codeTypeCmb->setCurrentIndex(0);
			_alphaAutoCalcRbtn->setChecked(true);
		}
	}
}
