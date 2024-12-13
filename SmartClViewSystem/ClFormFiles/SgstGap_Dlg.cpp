#include "SgstGap_Dlg.h"
#include "StTableWidget.h"

#include "qgroupbox.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include <QPushButton>
#include "qmessagebox.h"
#include <QLabel>
#include "ClWidget.h"

SgstGap_Dlg::SgstGap_Dlg()
{
	this->setWindowTitle(QStringLiteral("间隙推荐值库"));
	initWgt();
}


SgstGap_Dlg::~SgstGap_Dlg()
{

}


void SgstGap_Dlg::initWgt()
{
	/*电压等级*/
	_voltTypeCBx = new QComboBox();
	_voltTypeCBx->setStyleSheet("QComboBox QAbstractItemView::item{height:25px; }");
	for (unsigned int loop = 0; loop < VoltType::getVoltTypeLib().size(); ++loop)
	{
		_voltTypeCBx->addItem(QString::fromLocal8Bit(VoltType::typeToStr(VoltType::getVoltTypeLib().at(loop))));
	}
	QVBoxLayout *voltTypeLayout = new QVBoxLayout;
	voltTypeLayout->addWidget(_voltTypeCBx);
	QGroupBox *voltTypeGbx = new QGroupBox(QStringLiteral("电压等级"));
	voltTypeGbx->setLayout(voltTypeLayout);
	connect(_voltTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*回路类型*/
	_loopTypeCBx = new QComboBox();
	_loopTypeCBx->setStyleSheet("QComboBox QAbstractItemView::item{height:25px; }");
	for (unsigned int loop = 0; loop < LoopType::getLoopTypeLib().size(); ++loop)
	{
		_loopTypeCBx->addItem(QString::fromLocal8Bit(LoopType::typeToStr(LoopType::getLoopTypeLib().at(loop))));
	}
	QVBoxLayout *loopTypeLayout = new QVBoxLayout;
	loopTypeLayout->addWidget(_loopTypeCBx);
	QGroupBox *loopTypeGbx = new QGroupBox(QStringLiteral("回路类型"));
	loopTypeGbx->setLayout(loopTypeLayout);
	connect(_loopTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*绝缘子串类型*/
	_insulatorTypeCBx = new QComboBox();
	_insulatorTypeCBx->setStyleSheet("QComboBox QAbstractItemView::item{height:25px; }");
	for (unsigned int loop = 0; loop < InsulatorType::getInsulatorTypeLib().size(); ++loop)
	{
		_insulatorTypeCBx->addItem(QString::fromLocal8Bit(InsulatorType::typeToStr(InsulatorType::getInsulatorTypeLib().at(loop))));
	}
	QVBoxLayout *insulatorTypeLayout = new QVBoxLayout;
	insulatorTypeLayout->addWidget(_insulatorTypeCBx);
	QGroupBox *insulatorTypeGbx = new QGroupBox(QStringLiteral("绝缘子串类型"));
	insulatorTypeGbx->setLayout(insulatorTypeLayout);
	connect(_insulatorTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*海拔高度范围*/
	_altitudeTypeCBx = new QComboBox();
	_altitudeTypeCBx->setStyleSheet("QComboBox QAbstractItemView::item{height:25px; }");
	for (unsigned int loop = 0; loop < AltitudeType::getAltitudeTypeLib().size(); ++loop)
	{
		_altitudeTypeCBx->addItem(QString::fromLocal8Bit(AltitudeType::typeToStr(AltitudeType::getAltitudeTypeLib().at(loop))));
	}
	QVBoxLayout *altitudeTypeLayout = new QVBoxLayout;
	altitudeTypeLayout->addWidget(_altitudeTypeCBx);
	QGroupBox *altitudeTypeGbx = new QGroupBox(QStringLiteral("海拔高度范围"));
	altitudeTypeGbx->setLayout(altitudeTypeLayout);
	connect(_altitudeTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*间隙值表格*/
	_gapValueTbl = new StTableWidget();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 工频 (m) ")
		<< QStringLiteral(" 操作 (m) ")
		<< QStringLiteral(" 雷电 (m) ")
		<< QStringLiteral(" 带电 (m) ");
	_gapValueTbl->setRowCount(vHeaders.size());
	_gapValueTbl->setVerticalHeaderLabels(vHeaders);

	QStringList hHeaders;
	hHeaders << QStringLiteral(" 间隙推荐值 ");
	_gapValueTbl->setColumnCount(hHeaders.size());
	_gapValueTbl->setHorizontalHeaderLabels(hHeaders);
	_gapValueTbl->setFixedHeight(153);

	_gapValueTbl->setItem(0, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setFontPosition(_gapValueTbl->rowCount(), _gapValueTbl->columnCount());

	/*提示label*/
	QLabel *note = new QLabel(QStringLiteral("备注：如果间隙推荐值中出现 \"A/B\" 则A代表塔身\n间隙推荐值，B代表塔顶间隙推荐值。"));

	/*按钮*/
	_saveBtn = new QPushButton(QStringLiteral(" 保存 "));
	_cancelBtn = new QPushButton(QStringLiteral(" 退出 "));
	connect(_saveBtn, SIGNAL(clicked()), this, SLOT(slotSave()));
	connect(_cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	QHBoxLayout *btnHLyt = new QHBoxLayout();
	btnHLyt->addStretch();
	btnHLyt->addWidget(_saveBtn);
	btnHLyt->addWidget(_cancelBtn);

	QGridLayout *gridLyt = new QGridLayout();//栅格为5*2
	gridLyt->addWidget(voltTypeGbx, 0, 0, 1, 1);
	gridLyt->addWidget(loopTypeGbx, 0, 1, 1, 1);
	gridLyt->addWidget(insulatorTypeGbx, 1, 0, 1, 1);
	gridLyt->addWidget(altitudeTypeGbx, 1, 1, 1, 1);
	gridLyt->addWidget(_gapValueTbl, 2, 0, 1, 2);
	gridLyt->addWidget(note, 3, 0, 1, 2);
	gridLyt->addLayout(btnHLyt, 4, 0, 1, 2);
	this->setLayout(gridLyt);

	slotSgstGapChanged();
}

//@function: 保存   -- by mzq  2019/8/8 16:04
void SgstGap_Dlg::slotSave()
{
	int select = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否保存对应条件下的间隙推荐值？"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		VoltType::Type voltType = VoltType::Type(_voltTypeCBx->currentIndex());
		LoopType::Type loopType = LoopType::Type(_loopTypeCBx->currentIndex());
		InsulatorType::Type insulatorType = InsulatorType::Type(_insulatorTypeCBx->currentIndex());
		AltitudeType::Type altitudeType = AltitudeType::Type(_altitudeTypeCBx->currentIndex());

		try
		{
			HdlSgstGapLib sgstGapLib = ClWidget::getCoreInstance()->getSgstGapLib();
			HdlSingleDataSgstGap windGap = sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::Wind);
			windGap->setGap(_gapValueTbl->item(0, 0)->text().toLocal8Bit().toStdString());
			HdlSingleDataSgstGap inOverVoltGap = sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::InOverVolt);
			inOverVoltGap->setGap(_gapValueTbl->item(1, 0)->text().toLocal8Bit().toStdString());
			HdlSingleDataSgstGap outOverVoltGap = sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::OutOverVolt);
			outOverVoltGap->setGap(_gapValueTbl->item(2, 0)->text().toLocal8Bit().toStdString());
			HdlSingleDataSgstGap hotLineWorkGap = sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::HotLineWork);
			hotLineWorkGap->setGap(_gapValueTbl->item(3, 0)->text().toLocal8Bit().toStdString());

			ClWidget::getCoreInstance()->getSgstGapLib()->save(ClWidget::getCoreInstance()->getPathFolder_File());
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("间隙推荐值保存"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		}
	}
}

//@function: 界面和间隙推荐相关参数发生了改变   -- by mzq  2019/8/8 11:31
void SgstGap_Dlg::slotSgstGapChanged()
{
	VoltType::Type voltType = VoltType::Type(_voltTypeCBx->currentIndex());
	LoopType::Type loopType = LoopType::Type(_loopTypeCBx->currentIndex());
	InsulatorType::Type insulatorType = InsulatorType::Type(_insulatorTypeCBx->currentIndex());
	AltitudeType::Type altitudeType = AltitudeType::Type(_altitudeTypeCBx->currentIndex());

	try
	{
		HdlSgstGapLib sgstGapLib = ClWidget::getCoreInstance()->getSgstGapLib();
		std::vector<std::string> fourSugestGap;
		fourSugestGap.push_back(sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::Wind)->getGap());
		fourSugestGap.push_back(sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::InOverVolt)->getGap());
		fourSugestGap.push_back(sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::OutOverVolt)->getGap());
		fourSugestGap.push_back(sgstGapLib->find(voltType, loopType, insulatorType, altitudeType, MeteCate::HotLineWork)->getGap());

		_gapValueTbl->item(0, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(0).c_str()));
		_gapValueTbl->item(1, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(1).c_str()));
		_gapValueTbl->item(2, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(2).c_str()));
		_gapValueTbl->item(3, 0)->setText(QString::fromLocal8Bit(fourSugestGap.at(3).c_str()));
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("间隙推荐值读入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}