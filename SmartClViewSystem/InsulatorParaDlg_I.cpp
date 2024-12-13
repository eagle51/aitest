#include "InsulatorParaDlg_I.h"
#include "DataBase.h"
#include "Auxiliary.h"
#include "EnumLib.h"
#include "InsulatorLib_Dlg.h"

#include <QSplitter>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QtWidgets>
#include <QTableWidget>

InsulatorParaDlg_I::InsulatorParaDlg_I(HdlCl_Stright_I cl_Stright_I)
{
	_dataUI_Stright_I = cl_Stright_I->getDataUI_I();

	_insultorPara_I = _dataUI_Stright_I->getInsultorPara_I();
	_insulaGeoPara_I = _dataUI_Stright_I->getInsulaGeoPara_I();
	_voltType = _dataUI_Stright_I->getBasicInfo()->getVoltType();
	_isReadData = false;

	this->setWindowTitle(QStringLiteral("绝缘子串参数及模型"));
	initDialog();
	updateUIData();
}

// @function: 
InsulatorParaDlg_I::~InsulatorParaDlg_I()
{

}

void InsulatorParaDlg_I::initDialog()
{
	QHBoxLayout *dialogLyt = new QHBoxLayout();
	_leftAreaWidget = new QWidget();
	_leftAreaWidget->setMinimumWidth(360);

	_rightAreaLbl = new QLabel();
	dialogLyt->addWidget(_leftAreaWidget);
	dialogLyt->addWidget(_rightAreaLbl);
	this->setLayout(dialogLyt);

	QGroupBox *parameterGbx = this->initParameterGbx();
	QGroupBox *geoParameterGbx = this->initGeoParameterGbx();

	QPushButton *seeBtn = new QPushButton(QStringLiteral("选择绝缘子串"));
	QPushButton *okBtn = new QPushButton(QStringLiteral("确认"));
	QHBoxLayout *btnHLyt = new QHBoxLayout();
	btnHLyt->addStretch();
	btnHLyt->addWidget(seeBtn);
	btnHLyt->addWidget(okBtn);
	connect(seeBtn, SIGNAL(clicked()), this, SLOT(slotOpenInsulatorLibDlg()));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(slotOk()));

	QVBoxLayout *leftDialogLyt = new QVBoxLayout();
	leftDialogLyt->addWidget(parameterGbx);
	leftDialogLyt->addWidget(geoParameterGbx);
	leftDialogLyt->addLayout(btnHLyt);
	_leftAreaWidget->setLayout(leftDialogLyt);

}

QGroupBox* InsulatorParaDlg_I::initParameterGbx()
{
	QGroupBox *parameterGbx = new QGroupBox(QString::fromLocal8Bit("绝缘子串参数"));
	_parameterTable = new StTableWidget();
	_parameterTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_parameterTable->horizontalHeader()->hide();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 绝缘子串型号 ")
		<< QStringLiteral(" 绝缘子串总重力 (N) ")
		<< QStringLiteral(" 垂直风向绝缘子串联数 (联) ")
		<< QStringLiteral(" 顺风向绝缘子串联数 (联) ")
		<< QStringLiteral(" 单联绝缘子串片数 (片) ")
		<< QStringLiteral(" 单片绝缘子受风面积 (m^2) ")
		<< QStringLiteral(" 重锤重力 (N) ")
		<< QStringLiteral(" L0 绝缘子串长度 (m) ");
	_parameterTable->setRowCount(vHeaders.count());
	_parameterTable->setColumnCount(1);
	_parameterTable->setVerticalHeaderLabels(vHeaders);

	_parameterTable->setItem(0, 0, new QTableWidgetItem("---"));
	_parameterTable->setItem(1, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(2, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(3, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(4, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(5, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(6, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(7, 0, new QTableWidgetItem("0"));
	_parameterTable->setFontPosition(_parameterTable->rowCount(), _parameterTable->columnCount());
	_parameterTable->setCanEditFlag(7, 8, 0, 1, false);

	_parameterTable->item(1, 0)->setToolTip(QString::fromLocal8Bit("此处输入绝缘串子重力+金具重力"));
	_parameterTable->item(2, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/I串联数示意图.png' />").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(3, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/I串联数示意图.png' />").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(4, 0)->setToolTip(QString::fromLocal8Bit("此处输入实际的绝缘子串片数 \n在计算时自动将单联绝缘子串片数加2用于等效金具的受风面积"));
	_parameterTable->item(5, 0)->setToolTip(QString::fromLocal8Bit("此处可根据用户工程情况进行填写"));
	_parameterTable->item(6, 0)->setToolTip(QString::fromLocal8Bit("此处输入重锤重力，如果无重锤，则输入0"));
	_parameterTable->item(7, 0)->setToolTip(QString::fromLocal8Bit("L0指的是绝缘子串总长度，请看右侧示意图 \n该值会根据用户在绝缘子串及导线几何参数中输入的尺寸进行自动计算 \n用户不需要输入"));

	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_parameterTable);
	parameterGbx->setLayout(gridLyt);
	parameterGbx->setMinimumHeight(273);
	return parameterGbx;
}

QGroupBox* InsulatorParaDlg_I::initGeoParameterGbx()
{
	QGroupBox *geoParameterGbx = new QGroupBox(QString::fromLocal8Bit("绝缘子串及导线几何参数"));


	/*绝缘子串参数表格*/
	_geoParameterTable = new StTableWidget();
	_geoParameterTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_geoParameterTable->horizontalHeader()->hide();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 分裂数 ")
		<< QStringLiteral(" 分裂间距 (m) ")
		<< QStringLiteral(" L1 铁塔挂点-金具第一转动点 (m) ")
		<< QStringLiteral(" L2 铁塔挂点-上子导线线夹挂点 (m) ")
		<< QStringLiteral(" L3 上子导线线夹挂点-均压环 (m) ")
		<< QStringLiteral(" L4 均压环宽度 (m) ")
		<< QStringLiteral(" L5 子导线线夹长度 (m) ")
		<< QStringLiteral(" L6 小弧垂 (m) ")
		<< QStringLiteral(" θ 线夹及小弧垂偏角(度) ");
	_geoParameterTable->setRowCount(vHeaders.count());
	_geoParameterTable->setColumnCount(1);
	_geoParameterTable->setVerticalHeaderLabels(vHeaders);


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
	_geoParameterTable->setFontPosition(_geoParameterTable->rowCount(), _geoParameterTable->columnCount());

	_geoParameterTable->item(1, 0)->setToolTip(QStringLiteral("此处输入分裂间距，当导线为单分裂时，分裂间距应设置为0。"));
	_geoParameterTable->item(2, 0)->setToolTip(QStringLiteral("此处输入L1，请参照右侧示意图进行填写"));
	_geoParameterTable->item(3, 0)->setToolTip(QStringLiteral("此处输入L2，请参照右侧示意图进行填写"));
	_geoParameterTable->item(4, 0)->setToolTip(QStringLiteral("此处输入L3，请参照右侧示意图进行填写 \n当无均压环时，L3数值为上子导线线夹挂点至绝缘子高压点的距离"));
	_geoParameterTable->item(5, 0)->setToolTip(QStringLiteral("此处输入L4，请参照右侧示意图进行填写 \n当无均压环时L4填0 \n如果用户需要考虑高压端的绝缘子伞裙边缘，此处也可以输入绝缘子直径"));
	_geoParameterTable->item(6, 0)->setToolTip(QStringLiteral("此处输入L5，请参照右侧示意图进行填写"));
	_geoParameterTable->item(7, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/小弧垂.png' />").arg(QCoreApplication::applicationDirPath()));
	_geoParameterTable->item(8, 0)->setToolTip(QStringLiteral("此处输入θ，请参照右侧示意图进行填写，\n该值主要考虑导线风偏角与绝缘子串风偏角之间的差值影响，如无明确要求，可默认为0"));

	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentIndexChanged()));
	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateInsultorLen()));
	connect(_geoParameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotUpdateInsultorLen()));

	/*布局*/
	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_geoParameterTable);
	geoParameterGbx->setLayout(gridLyt);
	geoParameterGbx->setMinimumHeight(303);
	return geoParameterGbx;
}

void InsulatorParaDlg_I::slotCurrentIndexChanged()
{
	switch (SplitNum::Type(_splitNumCmb->currentIndex()))
	{
	case SmartCl::SplitNum::One:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/noneSplit.png"));
		break;
	case SmartCl::SplitNum::TwoV:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/twoSplitVer.png"));
		break;
	case SmartCl::SplitNum::TwoH:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/twoSplitHor.png"));
		break;
	case SmartCl::SplitNum::Three:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/threeSplit.png"));
		break;
	case SmartCl::SplitNum::Four:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/fourSplit.png"));
		break;
	case SmartCl::SplitNum::Six:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/sixSplit.png"));
		break;
	case SmartCl::SplitNum::Eight:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/eightSplit.png"));
		break;
	case SmartCl::SplitNum::Ten:
		_rightAreaLbl->setPixmap(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/tenSplit.png"));
		break;
	default:
		break;
	}
}
// @function: 
void InsulatorParaDlg_I::readInsultorParaData_I()
{
	if (_insultorPara_I.isNull())
		return;
	try
	{
		_parameterTable->item(0, 0)->setText(QString::fromLocal8Bit(_insultorPara_I->getInsultorName().c_str()));
		_parameterTable->item(1, 0)->setText(QString::number(_insultorPara_I->getInsultorWeight()));
		_parameterTable->item(2, 0)->setText(QString::number(_insultorPara_I->getNumSubs_V()));
		_parameterTable->item(3, 0)->setText(QString::number(_insultorPara_I->getNumSubs_H()));
		_parameterTable->item(4, 0)->setText(QString::number(_insultorPara_I->getNumPlate()));
		_parameterTable->item(5, 0)->setText(QString::number(_insultorPara_I->getShieldArea()));
		_parameterTable->item(6, 0)->setText(QString::number(_insultorPara_I->getHammerWeight()));
		_parameterTable->item(7, 0)->setText(QString::number(_insultorPara_I->getInsultorLen()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串参数读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void InsulatorParaDlg_I::writeInsultorParaData_I()
{
	try
	{
		_insultorPara_I->setInsultorName(_parameterTable->item(0, 0)->text().toLocal8Bit().toStdString());
		_insultorPara_I->setInsultorWeight(_parameterTable->item(1, 0)->text().toDouble());
		_insultorPara_I->setNumSubs_V(_parameterTable->item(2, 0)->text().toInt());
		_insultorPara_I->setNumSubs_H(_parameterTable->item(3, 0)->text().toInt());
		_insultorPara_I->setNumPlate(_parameterTable->item(4, 0)->text().toInt());
		_insultorPara_I->setShieldArea(_parameterTable->item(5, 0)->text().toDouble());
		_insultorPara_I->setHammerWeight(_parameterTable->item(6, 0)->text().toDouble());
		_insultorPara_I->setInsultorLen(_parameterTable->item(7, 0)->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串参数信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void InsulatorParaDlg_I::readInsulaGeoParaData_I()
{
	if (_insulaGeoPara_I.isNull())
		return;
	try
	{
		_splitNumCmb->setCurrentIndex(_insulaGeoPara_I->getSplitNum());
		_geoParameterTable->item(1, 0)->setText(QString::fromLocal8Bit(_insulaGeoPara_I->getSplitShow().c_str()));
		_geoParameterTable->item(2, 0)->setText(QString::number(_insulaGeoPara_I->getL1()));
		_geoParameterTable->item(3, 0)->setText(QString::number(_insulaGeoPara_I->getL2()));
		_geoParameterTable->item(4, 0)->setText(QString::number(_insulaGeoPara_I->getL3()));
		_geoParameterTable->item(5, 0)->setText(QString::number(_insulaGeoPara_I->getL4()));
		_geoParameterTable->item(6, 0)->setText(QString::number(_insulaGeoPara_I->getL5()));
		_geoParameterTable->item(7, 0)->setText(QString::number(_insulaGeoPara_I->getL6()));
		_geoParameterTable->item(8, 0)->setText(QString::number(_insulaGeoPara_I->getTheta()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串及导线几何参数信息读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void InsulatorParaDlg_I::writeInsulaGeoParaData_I()
{
	try
	{
		_insulaGeoPara_I->setSplitNum(SplitNum::Type(_splitNumCmb->currentIndex()));
		_insulaGeoPara_I->setSplitShow(_geoParameterTable->item(1, 0)->text().toLocal8Bit().toStdString());
		_insulaGeoPara_I->setL1(_geoParameterTable->item(2, 0)->text().toDouble());
		_insulaGeoPara_I->setL2(_geoParameterTable->item(3, 0)->text().toDouble());
		_insulaGeoPara_I->setL3(_geoParameterTable->item(4, 0)->text().toDouble());
		_insulaGeoPara_I->setL4(_geoParameterTable->item(5, 0)->text().toDouble());
		_insulaGeoPara_I->setL5(_geoParameterTable->item(6, 0)->text().toDouble());
		_insulaGeoPara_I->setL6(_geoParameterTable->item(7, 0)->text().toDouble());
		_insulaGeoPara_I->setTheta(_geoParameterTable->item(8, 0)->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串及导线几何参数信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 刷新界面数据   -- by mzq  2020/1/19 13:49
void InsulatorParaDlg_I::updateUIData()
{
	_isReadData = true;
	readInsultorParaData_I();
	readInsulaGeoParaData_I();
	_isReadData = false;

	slotUpdateInsultorLen();
	slotCurrentIndexChanged();
}

// @function: 
void InsulatorParaDlg_I::slotOk()
{
	writeInsultorParaData_I();
	writeInsulaGeoParaData_I();
	accept();
}

//@function: 更新绝缘子串长度显示   -- by mzq  2019/5/28 10:14
void InsulatorParaDlg_I::slotUpdateInsultorLen()
{
	if (_isReadData)//初始加载读取数据也会触发数据改变信号，将这些过滤掉
		return;

	double insultorLen = calcInsultorLen_I();
	_parameterTable->item(7, 0)->setText(QString::number(insultorLen, 10, 4));
}

//@function: 根据绝缘子串几何参数自动计算绝缘子串总长度   -- by mzq 2019/5/28 10:07
double InsulatorParaDlg_I::calcInsultorLen_I()
{
	writeInsulaGeoParaData_I();

	/*计算绝缘子串长度所需参数准备*/
	SplitNum::Type splitType = _insulaGeoPara_I->getSplitNum();
	double splitSpace = _insulaGeoPara_I->getSpaceSplit();
	double splitHeight = _insulaGeoPara_I->getHeightSplit();
	double splitAboveLen = _insulaGeoPara_I->getL2();
	double wireClipLen = _insulaGeoPara_I->getL5();

	double insultorLen = SmartCl::Auxiliary::calcInsultorLen(splitType, splitSpace, splitHeight, splitAboveLen, wireClipLen);
	return insultorLen;
}

// @ function: 绝缘子串参数列表查看  --- by ljn 2020/1/13 15:14
void InsulatorParaDlg_I::slotOpenInsulatorLibDlg()
{
	InsulatorLib_Dlg dlg(InsulatorType::Type_I, _voltType);
	if (dlg.exec() == QDialog::Accepted)
	{
		if (!dlg.getSelect_I().isNull())
		{
			_insultorPara_I = dlg.getSelect_I()->getInsultorPara_I()->clone();
			_insulaGeoPara_I = dlg.getSelect_I()->getInsulaGeoPara_I()->clone();
			_dataUI_Stright_I->setInsultorPara_I(_insultorPara_I);
			_dataUI_Stright_I->setInsulaGeoPara_I(_insulaGeoPara_I);
			updateUIData();
		}
	}
}