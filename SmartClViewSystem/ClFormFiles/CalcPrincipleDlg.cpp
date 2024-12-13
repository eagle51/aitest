#include "CalcPrincipleDlg.h"
#include "QGroupBox"
#include "QComboBox"
#include "QLineEdit"
#include "QBoxLayout"
#include "EnumLib.h"

#include "QLabel"
#include "QMessageBox"
#include "QCoreApplication"
#include "QListView"

CalcPrincipleDlg::CalcPrincipleDlg(HdlMdl_CalcRelePara calcRelePara, 
								   SmartCl::SoftWareType::Type softWareType,/*����汾����*/
								   QWidget *parent) 
								   : QDialog(parent)
{
	_calcRelePara = calcRelePara;
	_softWareType = softWareType;
	initDialog();
	readData();
}

CalcPrincipleDlg::~CalcPrincipleDlg()
{

}

//@function:     -- by wjw  2022/2/25 11:20
void CalcPrincipleDlg::initDialog()
{
	this->setWindowTitle(QStringLiteral("����ԭ������"));

	/*���߼���ԭ��*/
	_calcPrincipleTypeCbx_wire = new QComboBox();
	_calcPrincipleTypeCbx_wire->setMinimumWidth(150);
	_calcPrincipleTypeCbx_wire->setView(new QListView());
	for (unsigned int loop = 0; loop < CalcPrinciple_Wire::getTypeLib(_softWareType).size(); ++loop)
		_calcPrincipleTypeCbx_wire->addItem(QString::fromLocal8Bit(CalcPrinciple_Wire::typeToStr(CalcPrinciple_Wire::getTypeLib(_softWareType).at(loop))));
	_IzRefHeightLable = new QLabel(QStringLiteral("  ���ڼ���DL/T 5582-2020��Iz�Ĳο��߶�z��"));
	_IzRefHeightEdt = new QLineEdit("10");
	_IzRefHeightEdt->setToolTip(QStringLiteral("������ȷȡֵ���ݣ���Ĭ��Ϊ10m��"));
	_IzRefHeightEdt->setFixedWidth(40);
	QHBoxLayout *calcPrincipleTypeLayout_wire = new QHBoxLayout();
	calcPrincipleTypeLayout_wire->addWidget(_calcPrincipleTypeCbx_wire);
	calcPrincipleTypeLayout_wire->addWidget(_IzRefHeightLable);
	calcPrincipleTypeLayout_wire->addWidget(_IzRefHeightEdt);
	calcPrincipleTypeLayout_wire->addStretch();
	QGroupBox *calcPrincipleTypeGbx_wire = new QGroupBox(QStringLiteral("���߷���ؼ���ԭ��"));
	calcPrincipleTypeGbx_wire->setLayout(calcPrincipleTypeLayout_wire);
	QLabel *piclable_wire = new QLabel();
	piclable_wire->setPixmap(QCoreApplication::applicationDirPath() + QStringLiteral("/Resources/ToolTipImg/����ԭ��-����.png"));
	QVBoxLayout *layout_wire = new QVBoxLayout;
	layout_wire->addWidget(calcPrincipleTypeGbx_wire);
	layout_wire->addWidget(piclable_wire);

	/*��Ե�Ӵ�����ԭ��*/
	_calcPrincipleTypeCbx_insulator = new QComboBox();
	_calcPrincipleTypeCbx_insulator->setFixedWidth(120);
	_calcPrincipleTypeCbx_insulator->setView(new QListView());
	for (unsigned int loop = 0; loop < CalcPrinciple_Insulator::getTypeLib(_softWareType).size(); ++loop)
		_calcPrincipleTypeCbx_insulator->addItem(QString::fromLocal8Bit(CalcPrinciple_Insulator::typeToStr(CalcPrinciple_Insulator::getTypeLib(_softWareType).at(loop))));
	QHBoxLayout *calcPrincipleTypeLayout_insulator = new QHBoxLayout();
	calcPrincipleTypeLayout_insulator->addWidget(_calcPrincipleTypeCbx_insulator);
	calcPrincipleTypeLayout_insulator->addStretch();
	QGroupBox *calcPrincipleTypeGbx_insulator = new QGroupBox(QStringLiteral("��Ե�Ӵ�����ؼ���ԭ��"));
	calcPrincipleTypeGbx_insulator->setLayout(calcPrincipleTypeLayout_insulator);
	QLabel *piclable_insulator = new QLabel();
	piclable_insulator->setPixmap(QCoreApplication::applicationDirPath() + QStringLiteral("/Resources/ToolTipImg/����ԭ��-��Ե�Ӵ�.png"));
	QVBoxLayout *layout_insulator = new QVBoxLayout;
	layout_insulator->addWidget(calcPrincipleTypeGbx_insulator);
	layout_insulator->addWidget(piclable_insulator);
	layout_insulator->addStretch();

	QHBoxLayout *mainlayout = new QHBoxLayout;
	mainlayout->addLayout(layout_wire);
	mainlayout->addLayout(layout_insulator);
	this->setLayout(mainlayout);

	connect(_calcPrincipleTypeCbx_wire, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentIndexChanged_wire()));
}


//@function: ��ȡ����   -- by mzq 2022/12/3 10:20
void CalcPrincipleDlg::readData()
{
	try
	{
		_calcPrincipleTypeCbx_wire->setCurrentText(QString::fromLocal8Bit(CalcPrinciple_Wire::typeToStr(_calcRelePara->getCalcPrincipleType_wire())));
		_calcPrincipleTypeCbx_insulator->setCurrentText(QString::fromLocal8Bit(CalcPrinciple_Insulator::typeToStr(_calcRelePara->getCalcPrincipleType_insulator())));
		_IzRefHeightEdt->setText(QString::number(_calcRelePara->getIzRefHeight()));

		slotCurrentIndexChanged_wire();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("���ݶ���"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: ��������   -- by wjw  2022/2/25 14:51
void CalcPrincipleDlg::slotSaveData()
{
	try
	{
		_calcRelePara->setCalcPrincipleType_wire(CalcPrinciple_Wire::strToType(_calcPrincipleTypeCbx_wire->currentText().toLocal8Bit().toStdString().c_str()));
		_calcRelePara->setCalcPrincipleType_insulator(CalcPrinciple_Insulator::strToType(_calcPrincipleTypeCbx_insulator->currentText().toLocal8Bit().toStdString().c_str()));
		_calcRelePara->setIzRefHeight(_IzRefHeightEdt->text().toDouble());
	}
	catch (std::exception& e)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
	}
 }

//@function: ����ԭ�������л�   -- by mzq 2023/2/20 16:21
void CalcPrincipleDlg::slotCurrentIndexChanged_wire()
{
	if (CalcPrinciple_Wire::strToType(_calcPrincipleTypeCbx_wire->currentText().toLocal8Bit().toStdString()) == CalcPrinciple_Wire::Type::C_Wire_1
		|| CalcPrinciple_Wire::strToType(_calcPrincipleTypeCbx_wire->currentText().toLocal8Bit().toStdString()) == CalcPrinciple_Wire::Type::JumperWire_1
		|| (_calcRelePara->getCodeType() != CodeCate::DLT5582_2020 && _calcRelePara->getCodeType() != CodeCate::DLT5551_2018))
	{
		_IzRefHeightLable->setVisible(false);
		_IzRefHeightEdt->setVisible(false);
	}
	else
	{
		_IzRefHeightLable->setVisible(true);
		_IzRefHeightEdt->setVisible(true);
	}
}

//@function:    -- by wjw  2022/6/7 15:51
void CalcPrincipleDlg::closeEvent(QCloseEvent * event)
{
	slotSaveData();
}