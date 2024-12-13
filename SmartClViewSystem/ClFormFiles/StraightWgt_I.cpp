 #include "StraightWgt_I.h"
#include "InsulatorParaDlg_I.h"
#include "DataUI_Stright_I.h"
#include "Report_Straight_I.h"

#include "qpushbutton.h"
#include "qgroupbox.h"
#include "qgridlayout.h"
#include "qmessagebox.h"
#include "qcheckbox.h"
#include "qlineedit.h"
#include "qlabel.h"
#include "qtextstream.h"
#include "qcombobox.h"
#include "QHeaderView"
#include "QRadioButton"
#include "QCoreApplication"

StraightWgt_I::StraightWgt_I(int clType, std::string clName) : StraightWgt(clType, clName)
{	
	_cl_Stright_I = static_cast<Cl_Stright_I*>(_cl.GetPointee());
	connect(_insulatorParaBtn, SIGNAL(clicked()), this, SLOT(slotShowInsulatorParaDlg()));	
}

StraightWgt_I::~StraightWgt_I()
{

}

void StraightWgt_I::readChildClDataToForm()
{
	readGapValueParaData();
	readSwingAngleData();
	disconnect(_leftOrRightCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));//读取数据时，不触发绘制间隙圆信号*/
	_leftOrRightCmb->setCurrentIndex(_cl_Stright_I->getDataUI_I()->getLeftOrRight());
	connect(_leftOrRightCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDrawCommand()));
}

void StraightWgt_I::writeChildClDataFromForm()
{
	writeGapValueParaData();
	writeSwingAngleData();
	_cl_Stright_I->getDataUI_I()->setLeftOrRight(LeftOrRight::Type(_leftOrRightCmb->currentIndex()));
}

//@function: 
void StraightWgt_I::slotShowInsulatorParaDlg()
{
	writeDataFromForm();
	InsulatorParaDlg_I dlg(_cl_Stright_I);
	dlg.exec();
}
//@function:    -- by mzq  2019/5/28 21:09
QGroupBox* StraightWgt_I::initGapValueGbx()
{
	QGroupBox *gapValueGbx = new QGroupBox(QString::fromLocal8Bit("间隙值"));
	_gapValueTbl = new ClTableWgt(false, gapValueGbx);
	_gapValueTbl->setItem(0, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setMinimumWidth(180);
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 工频 (m) ")
		<< QStringLiteral(" 操作 (m) ")
		<< QStringLiteral(" 雷电 (m) ") 
		<< QStringLiteral(" 带电 (m) ");
	_gapValueTbl->setRowCount(vHeaders.size());
	_gapValueTbl->setVerticalHeaderLabels(vHeaders);

	QStringList hHeaders;
	hHeaders << QStringLiteral(" 推荐值 ")
		<< QStringLiteral(" 间隙值 ");
	_gapValueTbl->setColumnCount(hHeaders.size());
	_gapValueTbl->setHorizontalHeaderLabels(hHeaders);

	_gapValueTbl->setItem(0, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(0, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 1, new QTableWidgetItem("0"));
	_gapValueTbl->setFontPosition(_gapValueTbl->rowCount(), _gapValueTbl->columnCount());
	_gapValueTbl->setCanEditFlag(0, vHeaders.size(), 0, 1, false);
	_gapValueTbl->setFixedHeight(_gapValueTbl->horizontalHeader()->height() + _gapValueTbl->rowHeight(0) * _gapValueTbl->rowCount() + 2);
	_gapValueTbl->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/间隙值.png' />").arg(QCoreApplication::applicationDirPath()));

	QLabel *hotLineWorkRangeLbl = new QLabel(QStringLiteral("人体活动范围(m):"));
	_hotLineWorkRangeEdit = new QLineEdit("0");
	_hotLineWorkRangeEdit->setMinimumWidth(45);
	QHBoxLayout* hotLineWorkRangeLayout = new QHBoxLayout;
	hotLineWorkRangeLayout->addWidget(hotLineWorkRangeLbl);
	hotLineWorkRangeLayout->addWidget(_hotLineWorkRangeEdit);

	QLabel* hotLineWork_CutAngleLbl = new QLabel(QStringLiteral("控制范围(度):"));
	_hotLineWork_CutAngleEdit = new QLineEdit("0");
	_hotLineWork_CutAngleEdit->setMinimumWidth(45);
	QHBoxLayout* hotLineWork_isMergeLayout = new QHBoxLayout;
	hotLineWork_isMergeLayout->addWidget(hotLineWork_CutAngleLbl);
	hotLineWork_isMergeLayout->addWidget(_hotLineWork_CutAngleEdit);
	_hotLineWork_isMergeGbx = new QGroupBox(QStringLiteral("设置控制范围"));
	_hotLineWork_isMergeGbx->setLayout(hotLineWork_isMergeLayout);
	_hotLineWork_isMergeGbx->setCheckable(true);

	QHBoxLayout* hotLineWorkLayout = new QHBoxLayout;
	hotLineWorkLayout->addLayout(hotLineWorkRangeLayout);
	hotLineWorkLayout->addWidget(_hotLineWork_isMergeGbx);
	_hotLineWorkGBox = new QGroupBox(QStringLiteral("考虑带电作业"));
	_hotLineWorkGBox->setLayout(hotLineWorkLayout);
	_hotLineWorkGBox->setCheckable(true);

	QLabel *marginUpLbl = new QLabel(QStringLiteral("塔顶裕度值(m):"));
	_marginUpEdit = new QLineEdit("0");
	_marginUpEdit->setMinimumWidth(45);
	QHBoxLayout *marginUpLayout = new QHBoxLayout;
	marginUpLayout->addWidget(marginUpLbl);
	marginUpLayout->addWidget(_marginUpEdit);
	_marginUpGBox = new QGroupBox(QStringLiteral("对塔顶裕度"));
	_marginUpGBox->setLayout(marginUpLayout);
	_marginUpGBox->setCheckable(true);

	QLabel *marginDownLbl = new QLabel(QStringLiteral("塔身裕度值(m):"));
	_marginDownEdit = new QLineEdit("0");
	_marginDownEdit->setMinimumWidth(45);
	QHBoxLayout *marginDownLayout = new QHBoxLayout;
	marginDownLayout->addWidget(marginDownLbl);
	marginDownLayout->addWidget(_marginDownEdit);
	_marginDownGBox = new QGroupBox(QStringLiteral("对塔身裕度"));
	_marginDownGBox->setLayout(marginDownLayout);
	_marginDownGBox->setCheckable(true);

	_hotLineWorkGBox->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/带电作业控制范围.png' />").arg(QCoreApplication::applicationDirPath()));
	_marginUpGBox->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/裕度.png' />").arg(QCoreApplication::applicationDirPath()));
	_marginDownGBox->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/裕度.png' />").arg(QCoreApplication::applicationDirPath()));

	QGridLayout *gridLyt = new QGridLayout();
	gridLyt->addWidget(_gapValueTbl, 0, 0, 2, 1);
	gridLyt->addWidget(_hotLineWorkGBox, 0, 1, 1, 2);
	gridLyt->addWidget(_marginUpGBox, 1, 1, 1, 1);
	gridLyt->addWidget(_marginDownGBox, 1, 2, 1, 1);
	gapValueGbx->setLayout(gridLyt);
	return gapValueGbx;
}

//@function: I串摇摆角信息，针对I串，VL串可隐藏   -- by mzq 2020/12/3 9:40
QGroupBox* StraightWgt_I::initSwingAngleGbx()
{
	QGroupBox *swingAngleGbx = new QGroupBox(QString::fromLocal8Bit("摇摆角"));
	swingAngleGbx->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/摇摆角提示.png' />").arg(QCoreApplication::applicationDirPath()));
	
	_angleAutoCalcRbtn = new QRadioButton(QStringLiteral("自动计算"));
	_angleInputRbtn = new QRadioButton(QStringLiteral("手动输入"));
	connect(_angleAutoCalcRbtn, SIGNAL(toggled(bool)), this, SLOT(slotOnClickAngleAutoCalc()));
	connect(_angleInputRbtn, SIGNAL(toggled(bool)), this, SLOT(slotOnClickAngleInput()));

	_swingAngleTbl = new ClTableWgt(false, swingAngleGbx);
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 工频 (度) ")
		<< QStringLiteral(" 操作 (度) ")
		<< QStringLiteral(" 雷电 (度) ")
		<< QStringLiteral(" 带电 (度) ");
	_swingAngleTbl->setRowCount(vHeaders.size());
	_swingAngleTbl->setColumnCount(1);
	_swingAngleTbl->setVerticalHeaderLabels(vHeaders);
	_swingAngleTbl->horizontalHeader()->hide();

	_swingAngleTbl->setItem(0, 0, new QTableWidgetItem("0"));
	_swingAngleTbl->setItem(0, 1, new QTableWidgetItem("0"));
	_swingAngleTbl->setItem(1, 0, new QTableWidgetItem("0"));
	_swingAngleTbl->setItem(1, 1, new QTableWidgetItem("0"));
	_swingAngleTbl->setItem(2, 0, new QTableWidgetItem("0"));
	_swingAngleTbl->setItem(2, 1, new QTableWidgetItem("0"));
	_swingAngleTbl->setItem(3, 0, new QTableWidgetItem("0"));
	_swingAngleTbl->setItem(3, 1, new QTableWidgetItem("0"));
	_swingAngleTbl->setFontPosition(_swingAngleTbl->rowCount(), _swingAngleTbl->columnCount());
	_swingAngleTbl->setFixedHeight(_gapValueTbl->rowHeight(0) * _gapValueTbl->rowCount() + 2);
	_swingAngleTbl->setMinimumWidth(125);

	QVBoxLayout *vLyt = new QVBoxLayout();
	vLyt->addWidget(_angleAutoCalcRbtn);
	vLyt->addWidget(_angleInputRbtn);
	vLyt->addWidget(_swingAngleTbl);

	swingAngleGbx->setLayout(vLyt);
	return swingAngleGbx;
}

// @function: 
void StraightWgt_I::readGapValueParaData()
{
	try
	{
		HdlMdl_GapValuePara_I gapValuePara_I = _cl_Stright_I->getDataUI_I()->getGapValuePara_I();
		_gapValueTbl->item(0, 1)->setText(QString::number(gapValuePara_I->getPowerFreq_Gap()));
		_gapValueTbl->item(1, 1)->setText(QString::number(gapValuePara_I->getInOverVolt_Gap()));
		_gapValueTbl->item(2, 1)->setText(QString::number(gapValuePara_I->getOutOverVolt_Gap()));
		_gapValueTbl->item(3, 1)->setText(QString::number(gapValuePara_I->getHotLineWork_Gap()));
		_hotLineWorkGBox->setChecked(gapValuePara_I->getIsHotLineWork());
		_hotLineWorkRangeEdit->setText(QString::number(gapValuePara_I->getHotLineWorkRange()));
		_hotLineWork_isMergeGbx->setChecked(!gapValuePara_I->getHotLineWork_isMerge());
		_hotLineWork_CutAngleEdit->setText(QString::number(gapValuePara_I->getHotLineWork_CutAngle()));
		_marginUpGBox->setChecked(gapValuePara_I->getIsMargin_up());
		_marginUpEdit->setText(QString::number(gapValuePara_I->getMarginValue_up()));
		_marginDownGBox->setChecked(gapValuePara_I->getIsMargin_down());
		_marginDownEdit->setText(QString::number(gapValuePara_I->getMarginValue_down()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("导线参数信息设置"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @function: 
void StraightWgt_I::writeGapValueParaData()
{
	try
	{
		HdlMdl_GapValuePara_I gapValuePara_I = _cl_Stright_I->getDataUI_I()->getGapValuePara_I();
		gapValuePara_I->setPowerFreq_Gap(_gapValueTbl->item(0, 1)->text().toDouble());
		gapValuePara_I->setInOverVolt_Gap(_gapValueTbl->item(1, 1)->text().toDouble());
		gapValuePara_I->setOutOverVolt_Gap(_gapValueTbl->item(2, 1)->text().toDouble());
		gapValuePara_I->setHotLineWork_Gap(_gapValueTbl->item(3, 1)->text().toDouble());
		gapValuePara_I->setIsHotLineWork(_hotLineWorkGBox->isChecked());
		gapValuePara_I->setHotLineWorkRange(_hotLineWorkRangeEdit->text().toDouble());
		gapValuePara_I->setHotLineWork_isMerge(!_hotLineWork_isMergeGbx->isChecked());
		gapValuePara_I->setHotLineWork_CutAngle(_hotLineWork_CutAngleEdit->text().toDouble());
		gapValuePara_I->setIsMargin_up(_marginUpGBox->isChecked());
		gapValuePara_I->setMarginValue_up(_marginUpEdit->text().toDouble());
		gapValuePara_I->setIsMargin_down(_marginDownGBox->isChecked());
		gapValuePara_I->setMarginValue_down(_marginDownEdit->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙值信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function:    -- by mzq 2020/12/3 11:34
void StraightWgt_I::readSwingAngleData()
{
	try
	{
		HdlAngleForFourCate angleForFourCate = _cl_Stright_I->getDataUI_I()->getAngleForFourCate();

		if (angleForFourCate->getIsAutoCalc())
			_angleAutoCalcRbtn->setChecked(true);
		else
			_angleInputRbtn->setChecked(true);

		_swingAngleTbl->item(0, 0)->setText(QString::number(angleForFourCate->getAngle_wind()));
		_swingAngleTbl->item(1, 0)->setText(QString::number(angleForFourCate->getAngle_inOverVolt()));
		_swingAngleTbl->item(2, 0)->setText(QString::number(angleForFourCate->getAngle_outOverVolt()));
		_swingAngleTbl->item(3, 0)->setText(QString::number(angleForFourCate->getAngle_hotLineWork()));

	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("摇摆角信息读取"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}

}

//@function:    -- by mzq 2020/12/3 11:34
void StraightWgt_I::writeSwingAngleData()
{
	try
	{
		HdlAngleForFourCate angleForFourCate = _cl_Stright_I->getDataUI_I()->getAngleForFourCate();

		if (_angleAutoCalcRbtn->isChecked())
			angleForFourCate->setIsAutoCalc(true);
		else
			angleForFourCate->setIsAutoCalc(false);

		angleForFourCate->setAngle_wind(_swingAngleTbl->item(0, 0)->text().toDouble());
		angleForFourCate->setAngle_inOverVolt(_swingAngleTbl->item(1, 0)->text().toDouble());
		angleForFourCate->setAngle_outOverVolt(_swingAngleTbl->item(2, 0)->text().toDouble());
		angleForFourCate->setAngle_hotLineWork(_swingAngleTbl->item(3, 0)->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("摇摆角信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 生成报告   -- by mzq  2019/8/3 22:56
void StraightWgt_I::writeReport()
{
	Report_Straight_I report_I(_cl->getClName());
	report_I.wordReport();
}

//@function: 更新间隙推荐值   -- by mzq  2019/8/8 9:35
void StraightWgt_I::upDateSgstGap(const std::vector<std::string>& fourSugestGap)
{
	try
	{
		_gapValueTbl->item(0, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(0).c_str()));
		_gapValueTbl->item(1, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(1).c_str()));
		_gapValueTbl->item(2, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(2).c_str()));
		_gapValueTbl->item(3, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(3).c_str()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙值"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 点击自动计算摇摆角   -- by mzq 2020/12/3 14:16
void StraightWgt_I::slotOnClickAngleAutoCalc()
{
	_swingAngleTbl->setVisible(false);
}

//@function: 点击手动输入摇摆角   -- by mzq 2020/12/3 14:16
void StraightWgt_I::slotOnClickAngleInput()
{
	_swingAngleTbl->setVisible(true);
}