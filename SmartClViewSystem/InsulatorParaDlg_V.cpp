#include "InsulatorParaDlg_V.h"
#include "EnumLib.h"
#include "DataUI_Stright_V.h"
#include "Cl_Stright_V.h"
#include "Auxiliary.h"

#include "qboxlayout.h"
#include "qlabel.h"
#include "qgroupbox.h"
#include "qpushbutton.h"
#include "qcombobox.h"
#include "qlistview.h"
#include "qscrollarea.h"
#include "qlineedit.h"
#include "qmessagebox.h"
#include "qcoreapplication.h"
#include "InsulatorLib_Dlg.h"

InsulatorParaDlg_V::InsulatorParaDlg_V(HdlCl_Stright_V cl_Stright_V)
{
	_cl_Stright_V = cl_Stright_V;
	_insultorPara_V = _cl_Stright_V->getDataUI_V()->getInsultorPara_V();
	_insulaGeoPara_V = _cl_Stright_V->getDataUI_V()->getInsulaGeoPara_V();
	_voltType = _cl_Stright_V->getDataUI_V()->getBasicInfo()->getVoltType();
	_isReadData = false;

	this->setWindowTitle(QStringLiteral("绝缘子串参数及模型"));
	initDialog();
	updateUIData();
}

// @function: 
InsulatorParaDlg_V::~InsulatorParaDlg_V()
{

}

void InsulatorParaDlg_V::initDialog()
{
	_parameterGbx = this->initParameterGbx();
	_angleValueGbx = this->initAngleValueGbx();
	_geoParameterGbx = this->initGeoParameterGbx();

	QVBoxLayout *paraLyt = new QVBoxLayout();
	paraLyt->addWidget(_parameterGbx);
	paraLyt->addWidget(_angleValueGbx);
	paraLyt->addWidget(_geoParameterGbx);
	QGroupBox *paraGbx = new QGroupBox();
	paraGbx->setLayout(paraLyt);

	QScrollArea *paraSCrollArea = new QScrollArea();
	paraSCrollArea->setWidget(paraGbx);
	paraSCrollArea->setWidgetResizable(true);  //设置QScrollArea可根据Qwidget窗口可调整大小
	paraSCrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);  //设置滚动条需要时出现
	paraSCrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	paraSCrollArea->setBackgroundRole(QPalette::Light);

	QPushButton *seeBtn = new QPushButton(QStringLiteral("选择绝缘子串"));
	QPushButton *okBtn = new QPushButton(QStringLiteral("确认"));
	QHBoxLayout *btnHLyt = new QHBoxLayout();
	btnHLyt->addStretch();
	btnHLyt->addWidget(seeBtn);
	btnHLyt->addWidget(okBtn);
	connect(seeBtn, SIGNAL(clicked()), this, SLOT(slotOpenInsulatorLibDlg()));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(slotOk()));

	_leftAreaWidget = new QWidget();
	_leftAreaWidget->setMinimumWidth(350);

	QVBoxLayout *leftDialogLyt = new QVBoxLayout();
	leftDialogLyt->addWidget(paraSCrollArea);
	leftDialogLyt->addLayout(btnHLyt);
	_leftAreaWidget->setLayout(leftDialogLyt);
	
	_rightAreaLbl = new QLabel();

	QHBoxLayout *dialogLyt = new QHBoxLayout();
	dialogLyt->addWidget(_leftAreaWidget);
	dialogLyt->addWidget(_rightAreaLbl);
	this->setLayout(dialogLyt);

	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentIndexChanged()));
}

QGroupBox* InsulatorParaDlg_V::initParameterGbx()
{
	_parameterGbx = new QGroupBox(QString::fromLocal8Bit("绝缘子串参数"));
	_parameterGbx->setMinimumHeight(215);
	_parameterTable = new StTableWidget();
	_parameterTable->setColumnCount(1);
	_parameterTable->horizontalHeader()->hide();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 绝缘子串型号 ") 
		<< QStringLiteral(" v串总重力减背风肢重力(N) ") 
		<< QStringLiteral(" 单肢垂直风向绝缘子串联数 (联) ") 
		<< QStringLiteral(" 单肢单联绝缘子串片数 (片) ")
		<< QStringLiteral(" 单片绝缘子受风面积 (m^2) ")
		<< QStringLiteral(" 重锤重力 (N) ");
	_parameterTable->setRowCount(vHeaders.count());
	_parameterTable->setVerticalHeaderLabels(vHeaders);

	_parameterTable->setItem(0, 0, new QTableWidgetItem("---"));
	_parameterTable->setItem(1, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(2, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(3, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(4, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(5, 0, new QTableWidgetItem("0"));
	_parameterTable->setFontPosition(_parameterTable->rowCount(), _parameterTable->columnCount());

	_parameterTable->item(1, 0)->setToolTip(QString::fromLocal8Bit("<img src = '%1/Resources/ToolTipImg/V串重力示意图.png' / >").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(2, 0)->setToolTip(QString::fromLocal8Bit("<img src = '%1/Resources/ToolTipImg/V串联数示意图.png' / >").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(3, 0)->setToolTip(QString::fromLocal8Bit("此处输入实际的单肢单联绝缘子串片数 \n在计算时自动将单联绝缘子串片数加2用于等效金具的受风面积"));
	_parameterTable->item(4, 0)->setToolTip(QString::fromLocal8Bit("此处可根据用户工程情况进行填写"));
	_parameterTable->item(5, 0)->setToolTip(QString::fromLocal8Bit("此处输入重锤重力，如果无重锤，则输入0"));

	connect(_parameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotEditAngleTable()));

	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_parameterTable);
	_parameterGbx->setLayout(gridLyt);

	return _parameterGbx;
}

QGroupBox* InsulatorParaDlg_V::initAngleValueGbx()
{
	_angleValueGbx = new QGroupBox(QString::fromLocal8Bit("V串夹角取值"));
	_angleValueGbx->setMinimumHeight(125);
	_angleValueTable = new StTableWidget();
	_angleValueTable->setColumnCount(1);
	
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 受压角 (度) ") 
		<< QStringLiteral(" α 推荐值 (度) ")
		<< QStringLiteral(" α 实际取值 (度) ");
	_angleValueTable->setRowCount(vHeaders.count());
	_angleValueTable->setVerticalHeaderLabels(vHeaders);
	_angleValueTable->horizontalHeader()->hide();

	_alphaCorrectEdit = new QLineEdit("0");
	_alphaCorrectEdit->setStyleSheet("border-style:outset; border:0px solid white; font-family:Microsoft YaHei; font-size:12px;");
	_alphaCorrectEdit->setAlignment(Qt::AlignHCenter);

	_angleValueTable->setCellWidget(0, 0, _alphaCorrectEdit);
	_angleValueTable->setItem(1, 0, new QTableWidgetItem("0"));
	_angleValueTable->setItem(2, 0, new QTableWidgetItem("0"));
	_angleValueTable->item(2, 0)->setBackgroundColor(QColor(0x87CEEB));
	_angleValueTable->item(2, 0)->setTextColor(QColor(0x1C1C1C));
	_angleValueTable->setCanEditFlag(1, 2, 0, 1, false);
	_angleValueTable->setFontPosition(_angleValueTable->rowCount(), _angleValueTable->columnCount());

	_alphaCorrectEdit->setToolTip(QString::fromLocal8Bit("此处输入V串受压角，根据用户实际工程情况进行填写"));
	_angleValueTable->item(1, 0)->setToolTip(QString::fromLocal8Bit("该值为软件自动计算出的值 \nV串夹角推荐值计算时，将V串的单肢参数等效成I串进行摇摆角计算 \nV串夹角 α推荐值 = 2 × (等效I串的大风工况摇摆角 - 受压角)"));
	_angleValueTable->item(2, 0)->setToolTip(QString::fromLocal8Bit("α实际取值为最终绘图时的V串夹角，用户可以采用软件计算得到的α推荐值，也可在推荐值的基础上根据实际工程情况进行调整"));

	connect(_alphaCorrectEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotEditAngleTable()));

	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_angleValueTable);
	_angleValueGbx->setLayout(gridLyt);

	return _angleValueGbx;
}

QGroupBox* InsulatorParaDlg_V::initGeoParameterGbx()
{
	_geoParameterGbx = new QGroupBox(QString::fromLocal8Bit("绝缘子串及导线几何参数"));
	_geoParameterGbx->setMinimumHeight(335);


	/*绝缘子串参数表格*/
	_geoParameterTable = new StTableWidget();
	_geoParameterTable->setColumnCount(1);
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 分裂数 ")
		<< QStringLiteral(" 分裂间距 (m) ") 
		<< QStringLiteral(" L1 V串单肢长度 (m) ")
		<< QStringLiteral(" L2 均压环高度 (m) ") 
		<< QStringLiteral(" L3 均压环宽度 (m) ") 
		<< QStringLiteral(" L4 联板宽度 (m) ") 
		<< QStringLiteral(" L5 线夹挂孔与连板挂孔垂距 (m) ")
		<< QStringLiteral(" L6 子导线线夹长度 (m) ") 
		<< QStringLiteral(" L7 小弧垂 (m) ") 
		<< QStringLiteral(" θ 线夹及小弧垂偏角(度) ");
	_geoParameterTable->setRowCount(vHeaders.count());
	_geoParameterTable->setVerticalHeaderLabels(vHeaders);
	_geoParameterTable->horizontalHeader()->hide();


	/*绝缘子串及导线几何参数*/
	QStringList SplitNumTypeList;
	for (SmartCl::SplitNum::Type type = SmartCl::SplitNum::One; type <= SmartCl::SplitNum::Ten; type = (SmartCl::SplitNum::Type)(type + 1))
		SplitNumTypeList << QString::fromLocal8Bit(SmartCl::SplitNum::typeToStr(type));
	_splitNumCmb = new QComboBox();
	_splitNumCmb->addItems(SplitNumTypeList);
	_splitNumCmb->setView(new QListView());
	_splitNumCmb->setEnabled(true);
	
	_geoParameterTable->setCellWidget(0, 0, _splitNumCmb);
	_geoParameterTable->setItem(1, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(2, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(3, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(4, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(5, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(6, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(7, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(8, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(9, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setFontPosition(_geoParameterTable->rowCount(), _geoParameterTable->columnCount());

	_geoParameterTable->item(1, 0)->setToolTip(QStringLiteral("此处输入分裂间距，当导线为单分裂时，分裂间距应设置为0。"));
	_geoParameterTable->item(2, 0)->setToolTip(QStringLiteral("此处输入L1，请参照右侧示意图进行填写"));
	_geoParameterTable->item(3, 0)->setToolTip(QStringLiteral("此处输入L2，请参照右侧示意图进行填写 \n当无均压环时，L2数值为绝缘子高压点的距离"));
	_geoParameterTable->item(4, 0)->setToolTip(QStringLiteral("此处输入L3，请参照右侧示意图进行填写 \n当无均压环时L3填0 \n如果用户需要考虑高压端的绝缘子伞裙边缘，此处也可以输入绝缘子直径"));
	_geoParameterTable->item(5, 0)->setToolTip(QStringLiteral("此处输入L4，请参照右侧示意图进行填写"));
	_geoParameterTable->item(6, 0)->setToolTip(QStringLiteral("此处输入L5，请参照右侧示意图进行填写"));
	_geoParameterTable->item(7, 0)->setToolTip(QStringLiteral("此处输入L6，请参照右侧示意图进行填写"));
	_geoParameterTable->item(8, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/小弧垂.png' />").arg(QCoreApplication::applicationDirPath()));
	_geoParameterTable->item(9, 0)->setToolTip(QStringLiteral("此处输入θ，请参照右侧示意图进行填写"));

	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotEditAngleTable()));
	connect(_geoParameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotEditAngleTable()));


	/*布局*/
	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_geoParameterTable);
	_geoParameterGbx->setLayout(gridLyt);
	return _geoParameterGbx;
}

void InsulatorParaDlg_V::slotCurrentIndexChanged()
{
	switch (SplitNum::Type(_splitNumCmb->currentIndex()))
	{
	case SmartCl::SplitNum::One:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/noneSplit.png"));
		break;
	case SmartCl::SplitNum::TwoV:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/twoSplitVer.png"));
		break;
	case SmartCl::SplitNum::TwoH:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/twoSplitHor.png"));
		break;
	case SmartCl::SplitNum::Three:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/threeSplit.png"));
		break;
	case SmartCl::SplitNum::Four:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/fourSplit.png"));
		break;
	case SmartCl::SplitNum::Six:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/sixSplit.png"));
		break;
	case SmartCl::SplitNum::Eight:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/eightSplit.png"));
		break;
	case SmartCl::SplitNum::Ten:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/tenSplit.png"));
		break;
	default:
		break;
	}
}

void InsulatorParaDlg_V::readInsultorParaData_V()
{
	if (_insultorPara_V.isNull())
		return;
	try
	{
		_parameterTable->item(0, 0)->setText(QString::fromLocal8Bit(_insultorPara_V->getInsultorName().c_str()));
		_parameterTable->item(1, 0)->setText(QString::number(_insultorPara_V->getInsultorWeight()));
		_parameterTable->item(2, 0)->setText(QString::number(_insultorPara_V->getSglNumSubs_V()));
		_parameterTable->item(3, 0)->setText(QString::number(_insultorPara_V->getNumPlate()));
		_parameterTable->item(4, 0)->setText(QString::number(_insultorPara_V->getShieldArea()));
		_parameterTable->item(5, 0)->setText(QString::number(_insultorPara_V->getHammerWeight()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串参数读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

void InsulatorParaDlg_V::writeInsultorParaData_V()
{
	try
	{
		_insultorPara_V->setInsultorName(_parameterTable->item(0, 0)->text().toLocal8Bit().toStdString());
		_insultorPara_V->setInsultorWeight(_parameterTable->item(1, 0)->text().toDouble());
		_insultorPara_V->setSglNumSubs_V(_parameterTable->item(2, 0)->text().toInt());
		_insultorPara_V->setNumPlate(_parameterTable->item(3, 0)->text().toInt());
		_insultorPara_V->setShieldArea(_parameterTable->item(4, 0)->text().toDouble());
		_insultorPara_V->setHammerWeight(_parameterTable->item(5, 0)->text().toDouble());
		_insultorPara_V->setInsultorLen(calcInsultorLen_V());//V 串绝缘子串长度，为等效值（向I串等效），内核计算绝缘子串风压用
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串参数信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

void InsulatorParaDlg_V::readInsulaGeoParaData_V()
{
	if (_insulaGeoPara_V.isNull())
		return;
	try
	{
		_splitNumCmb->setCurrentIndex(_insulaGeoPara_V->getSplitNum());
		_geoParameterTable->item(1, 0)->setText(QString::fromLocal8Bit(_insulaGeoPara_V->getSplitShow().c_str()));
		_geoParameterTable->item(2, 0)->setText(QString::number(_insulaGeoPara_V->getL1()));
		_geoParameterTable->item(3, 0)->setText(QString::number(_insulaGeoPara_V->getL2()));
		_geoParameterTable->item(4, 0)->setText(QString::number(_insulaGeoPara_V->getL3()));
		_geoParameterTable->item(5, 0)->setText(QString::number(_insulaGeoPara_V->getL4()));
		_geoParameterTable->item(6, 0)->setText(QString::number(_insulaGeoPara_V->getL5()));
		_geoParameterTable->item(7, 0)->setText(QString::number(_insulaGeoPara_V->getL6()));
		_geoParameterTable->item(8, 0)->setText(QString::number(_insulaGeoPara_V->getL7()));
		_geoParameterTable->item(9, 0)->setText(QString::number(_insulaGeoPara_V->getTheta()));

		_alphaCorrectEdit->setText(QString::number(_insulaGeoPara_V->getAlphaCorrectAngle()));
		_angleValueTable->item(2, 0)->setText(QString::number(_insulaGeoPara_V->getAlpha()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串及导线几何参数信息读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

void InsulatorParaDlg_V::writeInsulaGeoParaData_V()
{
	try
	{
		_insulaGeoPara_V->setSplitNum(SplitNum::Type(_splitNumCmb->currentIndex()));
		_insulaGeoPara_V->setSplitShow(_geoParameterTable->item(1, 0)->text().toLocal8Bit().toStdString());
		_insulaGeoPara_V->setL1(_geoParameterTable->item(2, 0)->text().toDouble());
		_insulaGeoPara_V->setL2(_geoParameterTable->item(3, 0)->text().toDouble());
		_insulaGeoPara_V->setL3(_geoParameterTable->item(4, 0)->text().toDouble());
		_insulaGeoPara_V->setL4(_geoParameterTable->item(5, 0)->text().toDouble());
		_insulaGeoPara_V->setL5(_geoParameterTable->item(6, 0)->text().toDouble());
		_insulaGeoPara_V->setL6(_geoParameterTable->item(7, 0)->text().toDouble());
		_insulaGeoPara_V->setL7(_geoParameterTable->item(8, 0)->text().toDouble());
		_insulaGeoPara_V->setTheta(_geoParameterTable->item(9, 0)->text().toDouble());

		_insulaGeoPara_V->setAlphaCorrectAngle(_alphaCorrectEdit->text().toDouble());
		_insulaGeoPara_V->setAlpha(_angleValueTable->item(2, 0)->text().toDouble());

	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串及导线几何参数信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 刷新界面数据   -- by mzq  2020/1/19 13:59
void InsulatorParaDlg_V::updateUIData()
{
	_isReadData = true;
	readInsultorParaData_V();
	readInsulaGeoParaData_V();
	_isReadData = false;

	slotEditAngleTable();
	slotCurrentIndexChanged();
}

void InsulatorParaDlg_V::slotOk()
{
	writeInsultorParaData_V();
	writeInsulaGeoParaData_V();
	accept();
}

// @function: V串夹角取值表格显示  -mzq 2019/5/22 21:51
void InsulatorParaDlg_V::slotEditAngleTable()
{
	if (_isReadData)//初始加载读取数据也会触发数据改变信号，将这些过滤掉
		return;

	double alphaRecommend = getVAngleRecommend();
	_angleValueTable->item(1, 0)->setText(QString::number(alphaRecommend, 10, 2));
}

// @function: V 串夹角推荐值  -mzq 2019/5/23 11:58
double InsulatorParaDlg_V::getVAngleRecommend()
{
	writeInsultorParaData_V();
	writeInsulaGeoParaData_V();

	HdlDataUI_Stright_V  pCurClData = _cl_Stright_V->getDataUI_V();
	pCurClData->setInsultorPara_V(_insultorPara_V);
	pCurClData->setInsulaGeoPara_V(_insulaGeoPara_V);
	_cl_Stright_V->calcProcess();

	double alphaRecommend = pCurClData->getInsulaGeoPara_V()->getAlphaRecommend();
	return alphaRecommend;
}

//@function: 根据绝缘子串几何参数计算绝缘子串总长度，此长度为等效长度，为垂直面上的投影长度   -- by mzq  2019/5/28 15:25
double InsulatorParaDlg_V::calcInsultorLen_V()
{
	writeInsulaGeoParaData_V();

	/*计算绝缘子串长度所需参数准备*/
	SplitNum::Type splitType = _insulaGeoPara_V->getSplitNum();
	double splitSpace = _insulaGeoPara_V->getSpaceSplit();
	double splitHeight = _insulaGeoPara_V->getHeightSplit();
	double L1 = _insulaGeoPara_V->getL1();
	double L5 = _insulaGeoPara_V->getL5();
	double splitAboveLen = L1 * cos(SmartCl::Auxiliary::angle_to_radian(90 / 2)) + L5;//90为默认V串总夹角，绝缘子串长度对计算影响不大，该误差可忽略
	double wireClipLen = _insulaGeoPara_V->getL6();

	double insultorLen = SmartCl::Auxiliary::calcInsultorLen(splitType, splitSpace, splitHeight, splitAboveLen, wireClipLen);
	return insultorLen;
}
// @ function: 绝缘子串库查看  --- by ljn 2020/1/17 14:21
void InsulatorParaDlg_V::slotOpenInsulatorLibDlg()
{
	InsulatorLib_Dlg dlg(InsulatorType::Type_V, _voltType);
	if (dlg.exec() == QDialog::Accepted)
	{
		if (!dlg.getSelect_V().isNull())
		{
			_insultorPara_V = dlg.getSelect_V()->getInsultorPara_V()->clone();
			_insulaGeoPara_V = dlg.getSelect_V()->getInsulaGeoPara_V()->clone();
			_cl_Stright_V->getDataUI_V()->setInsultorPara_V(_insultorPara_V);
			_cl_Stright_V->getDataUI_V()->setInsulaGeoPara_V(_insulaGeoPara_V);
			updateUIData();
		}
	}
}