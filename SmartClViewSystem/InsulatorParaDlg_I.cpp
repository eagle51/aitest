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

	this->setWindowTitle(QStringLiteral("��Ե�Ӵ�������ģ��"));
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

	QPushButton *seeBtn = new QPushButton(QStringLiteral("ѡ���Ե�Ӵ�"));
	QPushButton *okBtn = new QPushButton(QStringLiteral("ȷ��"));
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
	QGroupBox *parameterGbx = new QGroupBox(QString::fromLocal8Bit("��Ե�Ӵ�����"));
	_parameterTable = new StTableWidget();
	_parameterTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_parameterTable->horizontalHeader()->hide();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ��Ե�Ӵ��ͺ� ")
		<< QStringLiteral(" ��Ե�Ӵ������� (N) ")
		<< QStringLiteral(" ��ֱ�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" ˳�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" ������Ե�Ӵ�Ƭ�� (Ƭ) ")
		<< QStringLiteral(" ��Ƭ��Ե���ܷ���� (m^2) ")
		<< QStringLiteral(" �ش����� (N) ")
		<< QStringLiteral(" L0 ��Ե�Ӵ����� (m) ");
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

	_parameterTable->item(1, 0)->setToolTip(QString::fromLocal8Bit("�˴������Ե��������+�������"));
	_parameterTable->item(2, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/I������ʾ��ͼ.png' />").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(3, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/I������ʾ��ͼ.png' />").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(4, 0)->setToolTip(QString::fromLocal8Bit("�˴�����ʵ�ʵľ�Ե�Ӵ�Ƭ�� \n�ڼ���ʱ�Զ���������Ե�Ӵ�Ƭ����2���ڵ�Ч��ߵ��ܷ����"));
	_parameterTable->item(5, 0)->setToolTip(QString::fromLocal8Bit("�˴��ɸ����û��������������д"));
	_parameterTable->item(6, 0)->setToolTip(QString::fromLocal8Bit("�˴������ش�������������ش���������0"));
	_parameterTable->item(7, 0)->setToolTip(QString::fromLocal8Bit("L0ָ���Ǿ�Ե�Ӵ��ܳ��ȣ��뿴�Ҳ�ʾ��ͼ \n��ֵ������û��ھ�Ե�Ӵ������߼��β���������ĳߴ�����Զ����� \n�û�����Ҫ����"));

	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_parameterTable);
	parameterGbx->setLayout(gridLyt);
	parameterGbx->setMinimumHeight(273);
	return parameterGbx;
}

QGroupBox* InsulatorParaDlg_I::initGeoParameterGbx()
{
	QGroupBox *geoParameterGbx = new QGroupBox(QString::fromLocal8Bit("��Ե�Ӵ������߼��β���"));


	/*��Ե�Ӵ��������*/
	_geoParameterTable = new StTableWidget();
	_geoParameterTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_geoParameterTable->horizontalHeader()->hide();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ������ ")
		<< QStringLiteral(" ���Ѽ�� (m) ")
		<< QStringLiteral(" L1 �����ҵ�-��ߵ�һת���� (m) ")
		<< QStringLiteral(" L2 �����ҵ�-���ӵ����߼йҵ� (m) ")
		<< QStringLiteral(" L3 ���ӵ����߼йҵ�-��ѹ�� (m) ")
		<< QStringLiteral(" L4 ��ѹ����� (m) ")
		<< QStringLiteral(" L5 �ӵ����߼г��� (m) ")
		<< QStringLiteral(" L6 С���� (m) ")
		<< QStringLiteral(" �� �߼м�С����ƫ��(��) ");
	_geoParameterTable->setRowCount(vHeaders.count());
	_geoParameterTable->setColumnCount(1);
	_geoParameterTable->setVerticalHeaderLabels(vHeaders);


	/*��Ե�Ӵ������߼��β���*/
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

	_geoParameterTable->item(1, 0)->setToolTip(QStringLiteral("�˴�������Ѽ�࣬������Ϊ������ʱ�����Ѽ��Ӧ����Ϊ0��"));
	_geoParameterTable->item(2, 0)->setToolTip(QStringLiteral("�˴�����L1��������Ҳ�ʾ��ͼ������д"));
	_geoParameterTable->item(3, 0)->setToolTip(QStringLiteral("�˴�����L2��������Ҳ�ʾ��ͼ������д"));
	_geoParameterTable->item(4, 0)->setToolTip(QStringLiteral("�˴�����L3��������Ҳ�ʾ��ͼ������д \n���޾�ѹ��ʱ��L3��ֵΪ���ӵ����߼йҵ�����Ե�Ӹ�ѹ��ľ���"));
	_geoParameterTable->item(5, 0)->setToolTip(QStringLiteral("�˴�����L4��������Ҳ�ʾ��ͼ������д \n���޾�ѹ��ʱL4��0 \n����û���Ҫ���Ǹ�ѹ�˵ľ�Ե��ɡȹ��Ե���˴�Ҳ���������Ե��ֱ��"));
	_geoParameterTable->item(6, 0)->setToolTip(QStringLiteral("�˴�����L5��������Ҳ�ʾ��ͼ������д"));
	_geoParameterTable->item(7, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/С����.png' />").arg(QCoreApplication::applicationDirPath()));
	_geoParameterTable->item(8, 0)->setToolTip(QStringLiteral("�˴�����ȣ�������Ҳ�ʾ��ͼ������д��\n��ֵ��Ҫ���ǵ��߷�ƫ�����Ե�Ӵ���ƫ��֮��Ĳ�ֵӰ�죬������ȷҪ�󣬿�Ĭ��Ϊ0"));

	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentIndexChanged()));
	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateInsultorLen()));
	connect(_geoParameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotUpdateInsultorLen()));

	/*����*/
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ���������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ�������Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ������߼��β�����Ϣ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ������߼��β�����Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: ˢ�½�������   -- by mzq  2020/1/19 13:49
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

//@function: ���¾�Ե�Ӵ�������ʾ   -- by mzq  2019/5/28 10:14
void InsulatorParaDlg_I::slotUpdateInsultorLen()
{
	if (_isReadData)//��ʼ���ض�ȡ����Ҳ�ᴥ�����ݸı��źţ�����Щ���˵�
		return;

	double insultorLen = calcInsultorLen_I();
	_parameterTable->item(7, 0)->setText(QString::number(insultorLen, 10, 4));
}

//@function: ���ݾ�Ե�Ӵ����β����Զ������Ե�Ӵ��ܳ���   -- by mzq 2019/5/28 10:07
double InsulatorParaDlg_I::calcInsultorLen_I()
{
	writeInsulaGeoParaData_I();

	/*�����Ե�Ӵ������������׼��*/
	SplitNum::Type splitType = _insulaGeoPara_I->getSplitNum();
	double splitSpace = _insulaGeoPara_I->getSpaceSplit();
	double splitHeight = _insulaGeoPara_I->getHeightSplit();
	double splitAboveLen = _insulaGeoPara_I->getL2();
	double wireClipLen = _insulaGeoPara_I->getL5();

	double insultorLen = SmartCl::Auxiliary::calcInsultorLen(splitType, splitSpace, splitHeight, splitAboveLen, wireClipLen);
	return insultorLen;
}

// @ function: ��Ե�Ӵ������б�鿴  --- by ljn 2020/1/13 15:14
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