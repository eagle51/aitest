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
	this->setWindowTitle(QStringLiteral("��϶�Ƽ�ֵ��"));
	initWgt();
}


SgstGap_Dlg::~SgstGap_Dlg()
{

}


void SgstGap_Dlg::initWgt()
{
	/*��ѹ�ȼ�*/
	_voltTypeCBx = new QComboBox();
	_voltTypeCBx->setStyleSheet("QComboBox QAbstractItemView::item{height:25px; }");
	for (unsigned int loop = 0; loop < VoltType::getVoltTypeLib().size(); ++loop)
	{
		_voltTypeCBx->addItem(QString::fromLocal8Bit(VoltType::typeToStr(VoltType::getVoltTypeLib().at(loop))));
	}
	QVBoxLayout *voltTypeLayout = new QVBoxLayout;
	voltTypeLayout->addWidget(_voltTypeCBx);
	QGroupBox *voltTypeGbx = new QGroupBox(QStringLiteral("��ѹ�ȼ�"));
	voltTypeGbx->setLayout(voltTypeLayout);
	connect(_voltTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*��·����*/
	_loopTypeCBx = new QComboBox();
	_loopTypeCBx->setStyleSheet("QComboBox QAbstractItemView::item{height:25px; }");
	for (unsigned int loop = 0; loop < LoopType::getLoopTypeLib().size(); ++loop)
	{
		_loopTypeCBx->addItem(QString::fromLocal8Bit(LoopType::typeToStr(LoopType::getLoopTypeLib().at(loop))));
	}
	QVBoxLayout *loopTypeLayout = new QVBoxLayout;
	loopTypeLayout->addWidget(_loopTypeCBx);
	QGroupBox *loopTypeGbx = new QGroupBox(QStringLiteral("��·����"));
	loopTypeGbx->setLayout(loopTypeLayout);
	connect(_loopTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*��Ե�Ӵ�����*/
	_insulatorTypeCBx = new QComboBox();
	_insulatorTypeCBx->setStyleSheet("QComboBox QAbstractItemView::item{height:25px; }");
	for (unsigned int loop = 0; loop < InsulatorType::getInsulatorTypeLib().size(); ++loop)
	{
		_insulatorTypeCBx->addItem(QString::fromLocal8Bit(InsulatorType::typeToStr(InsulatorType::getInsulatorTypeLib().at(loop))));
	}
	QVBoxLayout *insulatorTypeLayout = new QVBoxLayout;
	insulatorTypeLayout->addWidget(_insulatorTypeCBx);
	QGroupBox *insulatorTypeGbx = new QGroupBox(QStringLiteral("��Ե�Ӵ�����"));
	insulatorTypeGbx->setLayout(insulatorTypeLayout);
	connect(_insulatorTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*���θ߶ȷ�Χ*/
	_altitudeTypeCBx = new QComboBox();
	_altitudeTypeCBx->setStyleSheet("QComboBox QAbstractItemView::item{height:25px; }");
	for (unsigned int loop = 0; loop < AltitudeType::getAltitudeTypeLib().size(); ++loop)
	{
		_altitudeTypeCBx->addItem(QString::fromLocal8Bit(AltitudeType::typeToStr(AltitudeType::getAltitudeTypeLib().at(loop))));
	}
	QVBoxLayout *altitudeTypeLayout = new QVBoxLayout;
	altitudeTypeLayout->addWidget(_altitudeTypeCBx);
	QGroupBox *altitudeTypeGbx = new QGroupBox(QStringLiteral("���θ߶ȷ�Χ"));
	altitudeTypeGbx->setLayout(altitudeTypeLayout);
	connect(_altitudeTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSgstGapChanged()));

	/*��϶ֵ���*/
	_gapValueTbl = new StTableWidget();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ��Ƶ (m) ")
		<< QStringLiteral(" ���� (m) ")
		<< QStringLiteral(" �׵� (m) ")
		<< QStringLiteral(" ���� (m) ");
	_gapValueTbl->setRowCount(vHeaders.size());
	_gapValueTbl->setVerticalHeaderLabels(vHeaders);

	QStringList hHeaders;
	hHeaders << QStringLiteral(" ��϶�Ƽ�ֵ ");
	_gapValueTbl->setColumnCount(hHeaders.size());
	_gapValueTbl->setHorizontalHeaderLabels(hHeaders);
	_gapValueTbl->setFixedHeight(153);

	_gapValueTbl->setItem(0, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(1, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(2, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setItem(3, 0, new QTableWidgetItem("0"));
	_gapValueTbl->setFontPosition(_gapValueTbl->rowCount(), _gapValueTbl->columnCount());

	/*��ʾlabel*/
	QLabel *note = new QLabel(QStringLiteral("��ע�������϶�Ƽ�ֵ�г��� \"A/B\" ��A��������\n��϶�Ƽ�ֵ��B����������϶�Ƽ�ֵ��"));

	/*��ť*/
	_saveBtn = new QPushButton(QStringLiteral(" ���� "));
	_cancelBtn = new QPushButton(QStringLiteral(" �˳� "));
	connect(_saveBtn, SIGNAL(clicked()), this, SLOT(slotSave()));
	connect(_cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	QHBoxLayout *btnHLyt = new QHBoxLayout();
	btnHLyt->addStretch();
	btnHLyt->addWidget(_saveBtn);
	btnHLyt->addWidget(_cancelBtn);

	QGridLayout *gridLyt = new QGridLayout();//դ��Ϊ5*2
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

//@function: ����   -- by mzq  2019/8/8 16:04
void SgstGap_Dlg::slotSave()
{
	int select = QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�Ƿ񱣴��Ӧ�����µļ�϶�Ƽ�ֵ��"), QMessageBox::Yes | QMessageBox::No);
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
			QMessageBox::warning(this, QString::fromLocal8Bit("��϶�Ƽ�ֵ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		}
	}
}

//@function: ����ͼ�϶�Ƽ���ز��������˸ı�   -- by mzq  2019/8/8 11:31
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��϶�Ƽ�ֵ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}