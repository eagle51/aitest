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
	this->setWindowTitle(QStringLiteral("��ͼ����"));

	/*��϶Բͼֽ������*/
	QLabel *plottingScaleLbl = new QLabel("1:");
	_plottingScaleEdt = new QLineEdit("100");
	_plottingScaleEdt->setToolTip(QString::fromLocal8Bit("�������ɵļ�϶Բͼֽ�ߴ������"));
	QHBoxLayout *plottingScaleLayout = new QHBoxLayout;
	plottingScaleLayout->addWidget(plottingScaleLbl);
	plottingScaleLayout->addWidget(_plottingScaleEdt);
	QGroupBox *plottingScaleGbx = new QGroupBox(QStringLiteral("��϶Բͼֽ������"));
	plottingScaleGbx->setLayout(plottingScaleLayout);

	/*��ע����*/
	_dimStyleTypeCbx = new QComboBox();
	_dimStyleTypeCbx->setToolTip(QString::fromLocal8Bit("�������ü�϶Բͼֽ�ı�ע��ʽ��ͨ��������ѡ����Ҫ�ı�ע��ʽ��"));
	for (unsigned int loop = 0; loop < DimStyle::getTypeLib().size(); ++loop)
		_dimStyleTypeCbx->addItem(QString::fromLocal8Bit(DimStyle::typeToStr(DimStyle::getTypeLib().at(loop))));
	QVBoxLayout *dimStyleTypeLayout = new QVBoxLayout;
	dimStyleTypeLayout->addWidget(_dimStyleTypeCbx);
	QGroupBox *dimStyleTyleGbx = new QGroupBox(QStringLiteral("��ע����"));
	dimStyleTyleGbx->setLayout(dimStyleTypeLayout);

	/*V����L��С����������ʽ*/
	_smallSagStyleCbx = new QComboBox();
	_smallSagStyleCbx->setToolTip(QString::fromLocal8Bit("��������V����L��С����������ʽ��"));
	for (unsigned int loop = 0; loop < SmallSagStyle::getTypeLib().size(); ++loop)
		_smallSagStyleCbx->addItem(QString::fromLocal8Bit(SmallSagStyle::typeToStr(SmallSagStyle::getTypeLib().at(loop))));
	QVBoxLayout *smallSagStyleLayout = new QVBoxLayout;
	smallSagStyleLayout->addWidget(_smallSagStyleCbx);
	QGroupBox *smallSagStyleGbx = new QGroupBox(QStringLiteral("V����L��С����������ʽ"));
	smallSagStyleGbx->setLayout(smallSagStyleLayout);

	/*���Ǿ�ѹ��������Ӱ��*/
	QLabel *ringRLbl = new QLabel(QStringLiteral("��ѹ���뾶(m)��"));
	_ringREdt = new QLineEdit("0");
	_ringREdt->setToolTip(QString::fromLocal8Bit("�����ѹ����������������ľ�ϸ�����󣬴˴����Բ����ǡ�\n�翼�Ǿ�ѹ��������Ӱ�죬���ھ�Ե�Ӵ��ľ�ѹ���ߴ�����ʱ��Ӧ�Ծ�ѹ��Բ�Ĵ�Ϊ׼�������룡"));
	QHBoxLayout *ringRLayout = new QHBoxLayout;
	ringRLayout->addWidget(ringRLbl);
	ringRLayout->addWidget(_ringREdt);
	_isConsiderRingRGBox = new QGroupBox(QStringLiteral("���Ǿ�ѹ��������Ӱ��"));
	_isConsiderRingRGBox->setLayout(ringRLayout);
	_isConsiderRingRGBox->setCheckable(true);

	QPushButton *okBtn = new QPushButton(QStringLiteral(" ȷ�� "));
	QPushButton *cancelBtn = new QPushButton(QStringLiteral(" ȡ�� "));
	QHBoxLayout *btnHLyt = new QHBoxLayout();
	btnHLyt->addStretch();
	btnHLyt->addWidget(okBtn);
	btnHLyt->addWidget(cancelBtn);

	/*TODOMZQ ���ܴ�ʵ��*/
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


//@function: ��ȡ����   -- by mzq 2022/12/3 10:20
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
		QMessageBox::warning(this, QString::fromLocal8Bit("���ݶ���"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: ��������   -- by wjw  2022/2/25 14:51
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
		QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
		return;
	}
 }

//@function:    -- by wjw  2022/6/7 15:51
void SettingsDlg::closeEvent(QCloseEvent * event)
{
	try
	{
		QMessageBox::StandardButton rb = QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�Ƿ񱣴�����������ݣ�"),
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
		QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
		event->ignore();
	}
}