#include "SettingsDlg.h"
#include "EnumLib.h"
#include "DataBase.h"
#include "Settings.h"

#include "qlabel.h"
#include "qpushbutton.h"
#include "qboxlayout.h"
#include "qmessagebox.h"
#include "qevent.h"
#include <QLabel>

using namespace SmartCl;
SettingsDlg::SettingsDlg(QWidget *parent) : QDialog(parent)
{
	initDialog();
	readData();
}

SettingsDlg::~SettingsDlg()
{

}

//@function: CurrentCalcInfo     -- by wjw  2022/2/25 11:20
void SettingsDlg::initDialog()
{
	this->setWindowTitle(QStringLiteral("绘图设置"));

	/*间隙圆图纸比例尺*/
	QLabel *plottingScaleLbl = new QLabel("1:");
	_plottingScaleEdt = new QLineEdit("100");
	_plottingScaleEdt->setToolTip(QString::fromLocal8Bit("输入生成的间隙圆图纸尺寸比例尺"));
	QHBoxLayout *plottingScaleLayout = new QHBoxLayout;
	plottingScaleLayout->addWidget(plottingScaleLbl);
	plottingScaleLayout->addWidget(_plottingScaleEdt);
	QGroupBox *plottingScaleGbx = new QGroupBox(QStringLiteral("间隙圆图纸比例尺"));
	plottingScaleGbx->setLayout(plottingScaleLayout);

	/*标注设置*/
	_dimStyleTypeCbx = new QComboBox();
	_dimStyleTypeCbx->setToolTip(QString::fromLocal8Bit("用于设置间隙圆图纸的标注形式，通过下拉框选择需要的标注形式。"));
	for (unsigned int loop = 0; loop < DimStyle::getTypeLib().size(); ++loop)
		_dimStyleTypeCbx->addItem(QString::fromLocal8Bit(DimStyle::typeToStr(DimStyle::getTypeLib().at(loop))));
	QVBoxLayout *dimStyleTypeLayout = new QVBoxLayout;
	dimStyleTypeLayout->addWidget(_dimStyleTypeCbx);
	QGroupBox *dimStyleTyleGbx = new QGroupBox(QStringLiteral("标注设置"));
	dimStyleTyleGbx->setLayout(dimStyleTypeLayout);

	/*V串、L串小弧垂绘制样式*/
	_smallSagStyleCbx = new QComboBox();
	_smallSagStyleCbx->setToolTip(QString::fromLocal8Bit("用于设置V串、L串小弧垂绘制样式。"));
	for (unsigned int loop = 0; loop < SmallSagStyle::getTypeLib().size(); ++loop)
		_smallSagStyleCbx->addItem(QString::fromLocal8Bit(SmallSagStyle::typeToStr(SmallSagStyle::getTypeLib().at(loop))));
	QVBoxLayout *smallSagStyleLayout = new QVBoxLayout;
	smallSagStyleLayout->addWidget(_smallSagStyleCbx);
	QGroupBox *smallSagStyleGbx = new QGroupBox(QStringLiteral("V串、L串小弧垂绘制样式"));
	smallSagStyleGbx->setLayout(smallSagStyleLayout);

	/*考虑均压环环径的影响*/
	QLabel *ringRLbl = new QLabel(QStringLiteral("均压环半径(m)："));
	_ringREdt = new QLineEdit("0");
	_ringREdt->setToolTip(QString::fromLocal8Bit("输入均压环环径，如无特殊的精细化需求，此处可以不考虑。\n如考虑均压环环径的影响，则在绝缘子串的均压环尺寸输入时，应以均压环圆心处为准进行输入！"));
	QHBoxLayout *ringRLayout = new QHBoxLayout;
	ringRLayout->addWidget(ringRLbl);
	ringRLayout->addWidget(_ringREdt);
	_isConsiderRingRGBox = new QGroupBox(QStringLiteral("考虑均压环环径的影响"));
	_isConsiderRingRGBox->setLayout(ringRLayout);
	_isConsiderRingRGBox->setCheckable(true);

	QPushButton *okBtn = new QPushButton(QStringLiteral(" 确定 "));
	QPushButton *cancelBtn = new QPushButton(QStringLiteral(" 取消 "));
	QHBoxLayout *btnHLyt = new QHBoxLayout();
	btnHLyt->addStretch();
	btnHLyt->addWidget(okBtn);
	btnHLyt->addWidget(cancelBtn);

	/*TODOMZQ 功能待实现*/
	plottingScaleGbx->setVisible(false);
	dimStyleTyleGbx->setVisible(false);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(plottingScaleGbx);
	mainLayout->addWidget(dimStyleTyleGbx);
	mainLayout->addWidget(smallSagStyleGbx);
	mainLayout->addWidget(_isConsiderRingRGBox);
	mainLayout->addLayout(btnHLyt);
	this->setLayout(mainLayout);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(slotSaveData()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}


//@function: 读取数据   -- by mzq 2022/12/3 10:20
void SettingsDlg::readData()
{
	try
	{
		HdlSettings settings = DataBase::getInstance()->getSettings();
		_plottingScaleEdt->setText(QString::number(settings->getPlottingScale()));
		_dimStyleTypeCbx->setCurrentIndex(settings->getDimStyleTyle());
		_smallSagStyleCbx->setCurrentIndex(settings->getSmallSagStyle());
		_isConsiderRingRGBox->setChecked(settings->getIsConsiderRingR());
		_ringREdt->setText(QString::number(settings->getRingR()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("数据读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: 保存数据   -- by wjw  2022/2/25 14:51
void SettingsDlg::slotSaveData()
{
	try
	{
		HdlSettings settings = DataBase::getInstance()->getSettings();
		settings->setPlottingScale(_plottingScaleEdt->text().toDouble());
		settings->setDimStyleTyle(DimStyle::Type(_dimStyleTypeCbx->currentIndex()));
		settings->setSmallSagStyle(SmallSagStyle::Type(_smallSagStyleCbx->currentIndex()));
		settings->setIsConsiderRingR(_isConsiderRingRGBox->isChecked());
		settings->setRingR(_ringREdt->text().toDouble());

		reject();
	}
	catch (std::exception& e)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
		return;
	}
 }

//@function:    -- by wjw  2022/6/7 15:51
void SettingsDlg::closeEvent(QCloseEvent * event)
{
	try
	{
		QMessageBox::StandardButton rb = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否保存界面输入数据？"),
															  QMessageBox::Yes | QMessageBox::No);
		switch (rb)
		{
		case QMessageBox::Yes:
			slotSaveData();;
			return;
		case QMessageBox::No:
			return ;
		default:
			return;
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
		event->ignore();
	}
}