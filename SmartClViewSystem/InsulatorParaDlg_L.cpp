#include "InsulatorParaDlg_L.h"
#include "Cl_Stright_L.h"

#include "qboxlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qgroupbox.h"
#include "qcombobox.h"
#include "qlistview.h"
#include "qmessagebox.h"
#include "qscrollarea.h"
#include "qlineedit.h"
#include "Auxiliary.h"
#include "qcoreapplication.h"
#include "InsulatorLib_Dlg.h"

InsulatorParaDlg_L::InsulatorParaDlg_L(HdlCl_Stright_L cl_Stright_L)
{
	_cl_Stright_L = cl_Stright_L;
	_insultorPara_L = _cl_Stright_L->getDataUI_L()->getInsultorPara_L();
	_insulaGeoPara_L = _cl_Stright_L->getDataUI_L()->getInsulaGeoPara_L();
	_voltType = _cl_Stright_L->getDataUI_L()->getBasicInfo()->getVoltType();
	_isReadData = false;

	this->setWindowTitle(QStringLiteral("绝缘子串参数及模型"));
	initDialog_L();
	updateUIData();
}

InsulatorParaDlg_L::~InsulatorParaDlg_L()
{

}

void InsulatorParaDlg_L::initDialog_L()
{
	_parameterGbx = this->initParameterGbx();
	_geoParameterGbx = this->initGeoParameterGbx();
	_angleValueGbx = this->initAngleValueGbx();

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

QGroupBox* InsulatorParaDlg_L::initParameterGbx()
{
	_parameterGbx = new QGroupBox(QString::fromLocal8Bit("绝缘子串参数"));
	_parameterGbx->setMinimumHeight(305);
	_parameterTable = new StTableWidget();
	_parameterTable->setColumnCount(1);
	_parameterTable->horizontalHeader()->hide();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 绝缘子串型号 ") 
		<< QStringLiteral(" L串总重力减b肢重力(N) ")
		<< QStringLiteral(" L串总重力减a肢重力 (N) ")
		<< QStringLiteral(" a 肢垂直风向绝缘子串联数 (联) ") 
		<< QStringLiteral(" b 肢垂直风向绝缘子串联数 (联) ")
		<< QStringLiteral(" a 肢单联绝缘子串片数 (片) ") 
		<< QStringLiteral(" b 肢单联绝缘子串片数 (片) ") 
		<< QStringLiteral(" 单片绝缘子受风面积 (m^2) ")
		<< QStringLiteral(" 重锤重力(N) ");
	_parameterTable->setRowCount(vHeaders.count());
	_parameterTable->setVerticalHeaderLabels(vHeaders);

	_parameterTable->setItem(0, 0, new QTableWidgetItem("---"));
	_parameterTable->setItem(1, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(2, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(3, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(4, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(5, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(6, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(7, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(8, 0, new QTableWidgetItem("0"));
	_parameterTable->setFontPosition(_parameterTable->rowCount(), _parameterTable->columnCount());

	_parameterTable->item(1, 0)->setToolTip(QString::fromLocal8Bit("<img src = '%1/Resources/ToolTipImg/L串重力示意图.png' / >").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(2, 0)->setToolTip(QString::fromLocal8Bit("<img src = '%1/Resources/ToolTipImg/L串重力示意图.png' / >").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(3, 0)->setToolTip(QString::fromLocal8Bit("<img src = '%1/Resources/ToolTipImg/L串联数示意图.png' / >").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(4, 0)->setToolTip(QString::fromLocal8Bit("<img src = '%1/Resources/ToolTipImg/L串联数示意图.png' / >").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(5, 0)->setToolTip(QString::fromLocal8Bit("此处输入实际的单肢单联绝缘子串片数 \n在计算时自动将单联绝缘子串片数加2用于等效金具的受风面积"));
	_parameterTable->item(6, 0)->setToolTip(QString::fromLocal8Bit("此处输入实际的单肢单联绝缘子串片数 \n在计算时自动将单联绝缘子串片数加2用于等效金具的受风面积"));
	_parameterTable->item(7, 0)->setToolTip(QString::fromLocal8Bit("此处可根据用户工程情况进行填写"));
	_parameterTable->item(8, 0)->setToolTip(QString::fromLocal8Bit("此处输入重锤重力，如果无重锤，则输入0"));

	connect(_parameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotEditAngleTable()));

	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_parameterTable);
	_parameterGbx->setLayout(gridLyt);

	return _parameterGbx;
}

QGroupBox* InsulatorParaDlg_L::initAngleValueGbx()
{
	_angleValueGbx = new QGroupBox(QString::fromLocal8Bit("L串夹角取值"));
	_angleValueGbx->setMinimumHeight(215);
	_angleValueTable = new StTableWidget();
	_angleValueTable->setColumnCount(1);

	QStringList vHeaders;
	vHeaders << QStringLiteral(" α 受压角 (度) ") 
		<< QStringLiteral(" α 推荐值 (度) ") 
		<< QStringLiteral(" α 实际取值 (度) ")
		<< QStringLiteral(" β 受压角 (度) ")
		<< QStringLiteral(" β 推荐值 (度) ") 
		<< QStringLiteral(" β 实际取值 (度) ");
	_angleValueTable->setRowCount(vHeaders.count());
	_angleValueTable->setVerticalHeaderLabels(vHeaders);
	_angleValueTable->horizontalHeader()->hide();

	_alphaCorrectEdit = new QLineEdit("0");
	_alphaCorrectEdit->setAlignment(Qt::AlignHCenter);
	_alphaCorrectEdit->setStyleSheet("border-style:outset; border:0px solid white; font-family:Microsoft YaHei; font-size:12px;");
	_betaCorrectEdit = new QLineEdit("0");
	_betaCorrectEdit->setAlignment(Qt::AlignHCenter);
	_betaCorrectEdit->setStyleSheet("border-style:outset; border:0px solid white; font-family:Microsoft YaHei; font-size:12px;");

	_angleValueTable->setCellWidget(0, 0, _alphaCorrectEdit);
	_angleValueTable->setItem(1, 0, new QTableWidgetItem("0"));
	_angleValueTable->setItem(2, 0, new QTableWidgetItem("0"));
	_angleValueTable->item(2, 0)->setBackgroundColor(QColor(0x87CEEB));
	_angleValueTable->item(2, 0)->setTextColor(QColor(0x1C1C1C));
	_angleValueTable->setCellWidget(3, 0, _betaCorrectEdit);
	_angleValueTable->setItem(4, 0, new QTableWidgetItem("0"));
	_angleValueTable->setItem(5, 0, new QTableWidgetItem("0"));
	_angleValueTable->item(5, 0)->setBackgroundColor(QColor(0x87CEEB));
	_angleValueTable->item(5, 0)->setTextColor(QColor(0x1C1C1C));

	_angleValueTable->setCanEditFlag(1, 2, 0, 1, false);
	_angleValueTable->setCanEditFlag(4, 5, 0, 1, false);
	_angleValueTable->setFontPosition(_angleValueTable->rowCount(), _angleValueTable->columnCount());

	_alphaCorrectEdit->setToolTip(QString::fromLocal8Bit("此处输入 α 受压角，根据用户实际工程情况进行填写"));
	_angleValueTable->item(1, 0)->setToolTip(QString::fromLocal8Bit("该值为软件自动计算出的值 \nα 推荐值计算时，将L串的 a 肢参数等效成I串进行摇摆角计算 \nα 推荐值 = a 肢等效I串的大风工况摇摆角 - α 受压角)"));
	_angleValueTable->item(2, 0)->setToolTip(QString::fromLocal8Bit("α 实际取值为最终绘图时的 α 角，用户可以采用软件计算得到的α角，也可在 α 推荐值的基础上根据实际工程情况进行调整"));
	_betaCorrectEdit->setToolTip(QString::fromLocal8Bit("此处输入 β 受压角，根据用户实际工程情况进行填写"));
	_angleValueTable->item(4, 0)->setToolTip(QString::fromLocal8Bit("该值为软件自动计算出的值 \nβ 推荐值计算时，将L串的 β 肢参数等效成I串进行摇摆角计算 \nβ 推荐值 = β 肢等效I串的大风工况摇摆角 - β 受压角)"));
	_angleValueTable->item(5, 0)->setToolTip(QString::fromLocal8Bit("β 实际取值为最终绘图时的 β 角，用户可以采用软件计算得到的 β 角，也可在 β 推荐值的基础上根据实际工程情况进行调整"));

	connect(_alphaCorrectEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotEditAngleTable()));
	connect(_betaCorrectEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotEditAngleTable()));

	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_angleValueTable);
	_angleValueGbx->setLayout(gridLyt);

	return _angleValueGbx;
}

QGroupBox* InsulatorParaDlg_L::initGeoParameterGbx()
{
	_geoParameterGbx = new QGroupBox(QString::fromLocal8Bit("绝缘子串及导线几何参数"));
	_geoParameterGbx->setMinimumHeight(425);

	/*绝缘子串参数表格*/
	_geoParameterTable = new StTableWidget();
	_geoParameterTable->setColumnCount(1);
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 导线分裂数 ") 
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
		<< QStringLiteral(" θ 线夹及小弧垂偏角 (度) ");
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
	_geoParameterTable->setItem(10, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(11, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setItem(12, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setFontPosition(_geoParameterTable->rowCount(), _geoParameterTable->columnCount());

	_geoParameterTable->item(1, 0)->setToolTip(QStringLiteral("此处输入分裂间距，当导线为单分裂时，分裂间距应设置为0。"));
	_geoParameterTable->item(2, 0)->setToolTip(QStringLiteral("此处输入L1_a，请参照右侧示意图进行填写"));
	_geoParameterTable->item(3, 0)->setToolTip(QStringLiteral("此处输入L1_b，请参照右侧示意图进行填写"));
	_geoParameterTable->item(4, 0)->setToolTip(QStringLiteral("此处输入L2_a，请参照右侧示意图进行填写 \n当无均压环时，L2_a数值为a肢绝缘子高压点的距离"));
	_geoParameterTable->item(5, 0)->setToolTip(QStringLiteral("此处输入L2_b，请参照右侧示意图进行填写 \n当无均压环时，L2_b数值为b肢绝缘子高压点的距离"));
	_geoParameterTable->item(6, 0)->setToolTip(QStringLiteral("此处输入L3_a，请参照右侧示意图进行填写 \n当无均压环时L3_a填0 \n如果用户需要考虑高压端的绝缘子伞裙边缘，此处也可以输入绝缘子直径"));
	_geoParameterTable->item(7, 0)->setToolTip(QStringLiteral("此处输入L3_b，请参照右侧示意图进行填写 \n当无均压环时L3_b填0 \n如果用户需要考虑高压端的绝缘子伞裙边缘，此处也可以输入绝缘子直径"));
	_geoParameterTable->item(8, 0)->setToolTip(QStringLiteral("此处输入L4，请参照右侧示意图进行填写"));
	_geoParameterTable->item(9, 0)->setToolTip(QStringLiteral("此处输入L5，请参照右侧示意图进行填写"));
	_geoParameterTable->item(10, 0)->setToolTip(QStringLiteral("此处输入L6，请参照右侧示意图进行填写"));
	_geoParameterTable->item(11, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/小弧垂.png' />").arg(QCoreApplication::applicationDirPath()));
	_geoParameterTable->item(12, 0)->setToolTip(QStringLiteral("此处输入θ，请参照右侧示意图进行填写"));

	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotEditAngleTable()));
	connect(_geoParameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotEditAngleTable()));


	/*布局*/
	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_geoParameterTable);
	_geoParameterGbx->setLayout(gridLyt);
	return _geoParameterGbx;
}

void InsulatorParaDlg_L::slotCurrentIndexChanged()
{
	switch (SplitNum::Type(_splitNumCmb->currentIndex()))
	{
	case SmartCl::SplitNum::One:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/noneSplit.png"));
		break;
	case SmartCl::SplitNum::TwoV:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/twoSplitVer.png"));
		break;
	case SmartCl::SplitNum::TwoH:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/twoSplitHor.png"));
		break;
	case SmartCl::SplitNum::Three:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/threeSplit.png"));
		break;
	case SmartCl::SplitNum::Four:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/fourSplit.png"));
		break;
	case SmartCl::SplitNum::Six:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/sixSplit.png"));
		break;
	case SmartCl::SplitNum::Eight:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/eightSplit.png"));
		break;
	case SmartCl::SplitNum::Ten:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/tenSplit.png"));
		break;
	default:
		break;
	}
}

void InsulatorParaDlg_L::readInsultorParaData_L()
{
	if (_insultorPara_L.isNull())
		return;
	try
	{
		_parameterTable->item(0, 0)->setText(QString::fromLocal8Bit(_insultorPara_L->getInsultorName().c_str()));
		_parameterTable->item(1, 0)->setText(QString::number(_insultorPara_L->getInsultorWeight_a()));
		_parameterTable->item(2, 0)->setText(QString::number(_insultorPara_L->getInsultorWeight_b()));
		_parameterTable->item(3, 0)->setText(QString::number(_insultorPara_L->getNumSubs_a_V()));
		_parameterTable->item(4, 0)->setText(QString::number(_insultorPara_L->getNumSubs_b_V()));
		_parameterTable->item(5, 0)->setText(QString::number(_insultorPara_L->getNumPlate_a()));
		_parameterTable->item(6, 0)->setText(QString::number(_insultorPara_L->getNumPlate_b()));
		_parameterTable->item(7, 0)->setText(QString::number(_insultorPara_L->getShieldArea()));
		_parameterTable->item(8, 0)->setText(QString::number(_insultorPara_L->getHammerWeight()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串参数读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

void InsulatorParaDlg_L::writeInsultorParaData_L()
{
	try
	{
		_insultorPara_L->setInsultorName(_parameterTable->item(0, 0)->text().toLocal8Bit().toStdString());
		_insultorPara_L->setInsultorWeight_a(_parameterTable->item(1, 0)->text().toDouble());
		_insultorPara_L->setInsultorWeight_b(_parameterTable->item(2, 0)->text().toDouble());
		_insultorPara_L->setNumSubs_a_V(_parameterTable->item(3, 0)->text().toInt());
		_insultorPara_L->setNumSubs_b_V(_parameterTable->item(4, 0)->text().toInt());
		_insultorPara_L->setNumPlate_a(_parameterTable->item(5, 0)->text().toInt());
		_insultorPara_L->setNumPlate_b(_parameterTable->item(6, 0)->text().toInt());
		_insultorPara_L->setShieldArea(_parameterTable->item(7, 0)->text().toDouble());
		_insultorPara_L->setHammerWeight(_parameterTable->item(8, 0)->text().toDouble());

		double insultorLen = calcInsultorLen_L();
		_insultorPara_L->setLen_a(insultorLen);
		_insultorPara_L->setLen_b(insultorLen);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串参数信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

void InsulatorParaDlg_L::readInsulaGeoParaData_L()
{
	if (_insulaGeoPara_L.isNull())
		return;
	try
	{
		_splitNumCmb->setCurrentIndex(_insulaGeoPara_L->getSplitNum());
		_geoParameterTable->item(1, 0)->setText(QString::fromLocal8Bit(_insulaGeoPara_L->getSplitShow().c_str()));
		_geoParameterTable->item(2, 0)->setText(QString::number(_insulaGeoPara_L->getL1_a()));
		_geoParameterTable->item(3, 0)->setText(QString::number(_insulaGeoPara_L->getL1_b()));
		_geoParameterTable->item(4, 0)->setText(QString::number(_insulaGeoPara_L->getL2_a()));
		_geoParameterTable->item(5, 0)->setText(QString::number(_insulaGeoPara_L->getL2_b()));
		_geoParameterTable->item(6, 0)->setText(QString::number(_insulaGeoPara_L->getL3_a()));
		_geoParameterTable->item(7, 0)->setText(QString::number(_insulaGeoPara_L->getL3_b()));
		_geoParameterTable->item(8, 0)->setText(QString::number(_insulaGeoPara_L->getL4()));
		_geoParameterTable->item(9, 0)->setText(QString::number(_insulaGeoPara_L->getL5()));
		_geoParameterTable->item(10, 0)->setText(QString::number(_insulaGeoPara_L->getL6()));
		_geoParameterTable->item(11, 0)->setText(QString::number(_insulaGeoPara_L->getL7()));
		_geoParameterTable->item(12, 0)->setText(QString::number(_insulaGeoPara_L->getTheta()));

		_alphaCorrectEdit->setText(QString::number(_insulaGeoPara_L->getAlpha_correctAngle()));
		_angleValueTable->item(2, 0)->setText(QString::number(_insulaGeoPara_L->getAlpha()));
		_betaCorrectEdit->setText(QString::number(_insulaGeoPara_L->getBeta_correctAngle()));
		_angleValueTable->item(5, 0)->setText(QString::number(_insulaGeoPara_L->getBeta()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串及导线几何参数信息读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

void InsulatorParaDlg_L::writeInsulaGeoParaData_L()
{
	try
	{
		_insulaGeoPara_L->setSplitNum(SplitNum::Type(_splitNumCmb->currentIndex()));
		_insulaGeoPara_L->setSplitShow(_geoParameterTable->item(1, 0)->text().toLocal8Bit().toStdString());
		_insulaGeoPara_L->setL1_a(_geoParameterTable->item(2, 0)->text().toDouble());
		_insulaGeoPara_L->setL1_b(_geoParameterTable->item(3, 0)->text().toDouble());
		_insulaGeoPara_L->setL2_a(_geoParameterTable->item(4, 0)->text().toDouble());
		_insulaGeoPara_L->setL2_b(_geoParameterTable->item(5, 0)->text().toDouble());
		_insulaGeoPara_L->setL3_a(_geoParameterTable->item(6, 0)->text().toDouble());
		_insulaGeoPara_L->setL3_b(_geoParameterTable->item(7, 0)->text().toDouble());
		_insulaGeoPara_L->setL4(_geoParameterTable->item(8, 0)->text().toDouble());
		_insulaGeoPara_L->setL5(_geoParameterTable->item(9, 0)->text().toDouble());
		_insulaGeoPara_L->setL6(_geoParameterTable->item(10, 0)->text().toDouble());
		_insulaGeoPara_L->setL7(_geoParameterTable->item(11, 0)->text().toDouble());
		_insulaGeoPara_L->setTheta(_geoParameterTable->item(12, 0)->text().toDouble());

		_insulaGeoPara_L->setAlpha_correctAngle(_alphaCorrectEdit->text().toDouble());
		_insulaGeoPara_L->setAlpha(_angleValueTable->item(2, 0)->text().toDouble());
		_insulaGeoPara_L->setBeta_correctAngle(_betaCorrectEdit->text().toDouble());
		_insulaGeoPara_L->setBeta(_angleValueTable->item(5, 0)->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串及导线几何参数信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 刷新界面数据   -- by mzq  2020/1/19 14:02
void InsulatorParaDlg_L::updateUIData()
{
	_isReadData = true;
	readInsultorParaData_L();
	readInsulaGeoParaData_L();
	_isReadData = false;

	slotEditAngleTable();
	slotCurrentIndexChanged();
}

void InsulatorParaDlg_L::slotOk()
{
	writeInsultorParaData_L();
	writeInsulaGeoParaData_L();
	accept();
}

// @function: L串夹角取值表格显示  -mzq 2019/5/23 21:22
void InsulatorParaDlg_L::slotEditAngleTable()
{
	if (_isReadData)//初始加载读取数据也会触发数据改变信号，将这些过滤掉
		return;

	std::vector<double> angleRecommend = getLAngleRecommend();
	_angleValueTable->item(1, 0)->setText(QString::number(angleRecommend.at(0), 10, 2));
	_angleValueTable->item(4, 0)->setText(QString::number(angleRecommend.at(1), 10, 2));
}

// @function: L 串夹角推荐值  -mzq 2019/5/24 11:40
std::vector<double> InsulatorParaDlg_L::getLAngleRecommend()
{
	writeInsultorParaData_L();
	writeInsulaGeoParaData_L();

	HdlDataUI_Stright_L pCurClData = _cl_Stright_L->getDataUI_L();

	pCurClData->setInsultorPara_L(_insultorPara_L);
	pCurClData->setInsulaGeoPara_L(_insulaGeoPara_L);
	_cl_Stright_L->calcProcess();

	double alphaRecommend = pCurClData->getInsulaGeoPara_L()->getAlpha_recommend();
	double betaRecommend = pCurClData->getInsulaGeoPara_L()->getBeta_recommend();

	std::vector<double> angleRecommend;
	angleRecommend.push_back(alphaRecommend);
	angleRecommend.push_back(betaRecommend);

	return angleRecommend;
}

//@function: 根据绝缘子串几何参数计算绝缘子串总长度，此长度为等效长度，为垂直面上的投影长度，存在误差，但影响很小   -- by mzq  2019/5/28 16:03
double InsulatorParaDlg_L::calcInsultorLen_L()
{
	writeInsulaGeoParaData_L();

	/*计算绝缘子串长度所需参数准备*/
	SplitNum::Type splitType = _insulaGeoPara_L->getSplitNum();
	double splitSpace = _insulaGeoPara_L->getSpaceSplit();
	double splitHeight = _insulaGeoPara_L->getHeightSplit();
	double L1_a = _insulaGeoPara_L->getL1_a();
	double L1_b = _insulaGeoPara_L->getL1_b();
	double L5 = _insulaGeoPara_L->getL5();
	double L1_Min = (L1_a <= L1_b) ? L1_a : L1_b;//取两肢中最短的用来近似计算，相当于绝缘子串挂高了，计算结果更加保守
	double splitAboveLen = L1_Min * cos(SmartCl::Auxiliary::angle_to_radian(90 / 2)) + L5;//90为默认L串总夹角，绝缘子串长度对计算影响不大，该误差可忽略
	
	double wireClipLen = _insulaGeoPara_L->getL6();

	double insultorLen = SmartCl::Auxiliary::calcInsultorLen(splitType, splitSpace, splitHeight, splitAboveLen, wireClipLen);
	return insultorLen;
}
// @ function: 绝缘子串库查看  --- by ljn 2020/1/17 11:57
void InsulatorParaDlg_L::slotOpenInsulatorLibDlg()
{
	InsulatorLib_Dlg dlg(InsulatorType::Type_L, _voltType);
	if (dlg.exec() == QDialog::Accepted)
	{
		if (!dlg.getSelect_L().isNull())
		{
			_insultorPara_L = dlg.getSelect_L()->getInsultorPara_L()->clone();
			_insulaGeoPara_L = dlg.getSelect_L()->getInsulaGeoPara_L()->clone();
			_cl_Stright_L->getDataUI_L()->setInsultorPara_L(_insultorPara_L);
			_cl_Stright_L->getDataUI_L()->setInsulaGeoPara_L(_insulaGeoPara_L);
			updateUIData();
		}
	}
}