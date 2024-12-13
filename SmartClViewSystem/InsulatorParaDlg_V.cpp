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

	this->setWindowTitle(QStringLiteral("��Ե�Ӵ�������ģ��"));
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
	paraSCrollArea->setWidgetResizable(true);  //����QScrollArea�ɸ���Qwidget���ڿɵ�����С
	paraSCrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);  //���ù�������Ҫʱ����
	paraSCrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	paraSCrollArea->setBackgroundRole(QPalette::Light);

	QPushButton *seeBtn = new QPushButton(QStringLiteral("ѡ���Ե�Ӵ�"));
	QPushButton *okBtn = new QPushButton(QStringLiteral("ȷ��"));
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
	_parameterGbx = new QGroupBox(QString::fromLocal8Bit("��Ե�Ӵ�����"));
	_parameterGbx->setMinimumHeight(215);
	_parameterTable = new StTableWidget();
	_parameterTable->setColumnCount(1);
	_parameterTable->horizontalHeader()->hide();
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ��Ե�Ӵ��ͺ� ") 
		<< QStringLiteral(" v��������������֫����(N) ") 
		<< QStringLiteral(" ��֫��ֱ�����Ե�Ӵ����� (��) ") 
		<< QStringLiteral(" ��֫������Ե�Ӵ�Ƭ�� (Ƭ) ")
		<< QStringLiteral(" ��Ƭ��Ե���ܷ���� (m^2) ")
		<< QStringLiteral(" �ش����� (N) ");
	_parameterTable->setRowCount(vHeaders.count());
	_parameterTable->setVerticalHeaderLabels(vHeaders);

	_parameterTable->setItem(0, 0, new QTableWidgetItem("---"));
	_parameterTable->setItem(1, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(2, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(3, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(4, 0, new QTableWidgetItem("0"));
	_parameterTable->setItem(5, 0, new QTableWidgetItem("0"));
	_parameterTable->setFontPosition(_parameterTable->rowCount(), _parameterTable->columnCount());

	_parameterTable->item(1, 0)->setToolTip(QString::fromLocal8Bit("<img src = '%1/Resources/ToolTipImg/V������ʾ��ͼ.png' / >").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(2, 0)->setToolTip(QString::fromLocal8Bit("<img src = '%1/Resources/ToolTipImg/V������ʾ��ͼ.png' / >").arg(QCoreApplication::applicationDirPath()));
	_parameterTable->item(3, 0)->setToolTip(QString::fromLocal8Bit("�˴�����ʵ�ʵĵ�֫������Ե�Ӵ�Ƭ�� \n�ڼ���ʱ�Զ���������Ե�Ӵ�Ƭ����2���ڵ�Ч��ߵ��ܷ����"));
	_parameterTable->item(4, 0)->setToolTip(QString::fromLocal8Bit("�˴��ɸ����û��������������д"));
	_parameterTable->item(5, 0)->setToolTip(QString::fromLocal8Bit("�˴������ش�������������ش���������0"));

	connect(_parameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotEditAngleTable()));

	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_parameterTable);
	_parameterGbx->setLayout(gridLyt);

	return _parameterGbx;
}

QGroupBox* InsulatorParaDlg_V::initAngleValueGbx()
{
	_angleValueGbx = new QGroupBox(QString::fromLocal8Bit("V���н�ȡֵ"));
	_angleValueGbx->setMinimumHeight(125);
	_angleValueTable = new StTableWidget();
	_angleValueTable->setColumnCount(1);
	
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ��ѹ�� (��) ") 
		<< QStringLiteral(" �� �Ƽ�ֵ (��) ")
		<< QStringLiteral(" �� ʵ��ȡֵ (��) ");
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

	_alphaCorrectEdit->setToolTip(QString::fromLocal8Bit("�˴�����V����ѹ�ǣ������û�ʵ�ʹ������������д"));
	_angleValueTable->item(1, 0)->setToolTip(QString::fromLocal8Bit("��ֵΪ����Զ��������ֵ \nV���н��Ƽ�ֵ����ʱ����V���ĵ�֫������Ч��I������ҡ�ڽǼ��� \nV���н� ���Ƽ�ֵ = 2 �� (��ЧI���Ĵ�繤��ҡ�ڽ� - ��ѹ��)"));
	_angleValueTable->item(2, 0)->setToolTip(QString::fromLocal8Bit("��ʵ��ȡֵΪ���ջ�ͼʱ��V���нǣ��û����Բ����������õ��Ħ��Ƽ�ֵ��Ҳ�����Ƽ�ֵ�Ļ����ϸ���ʵ�ʹ���������е���"));

	connect(_alphaCorrectEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotEditAngleTable()));

	QVBoxLayout *gridLyt = new QVBoxLayout();
	gridLyt->addWidget(_angleValueTable);
	_angleValueGbx->setLayout(gridLyt);

	return _angleValueGbx;
}

QGroupBox* InsulatorParaDlg_V::initGeoParameterGbx()
{
	_geoParameterGbx = new QGroupBox(QString::fromLocal8Bit("��Ե�Ӵ������߼��β���"));
	_geoParameterGbx->setMinimumHeight(335);


	/*��Ե�Ӵ��������*/
	_geoParameterTable = new StTableWidget();
	_geoParameterTable->setColumnCount(1);
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ������ ")
		<< QStringLiteral(" ���Ѽ�� (m) ") 
		<< QStringLiteral(" L1 V����֫���� (m) ")
		<< QStringLiteral(" L2 ��ѹ���߶� (m) ") 
		<< QStringLiteral(" L3 ��ѹ����� (m) ") 
		<< QStringLiteral(" L4 ������ (m) ") 
		<< QStringLiteral(" L5 �߼йҿ�������ҿ״��� (m) ")
		<< QStringLiteral(" L6 �ӵ����߼г��� (m) ") 
		<< QStringLiteral(" L7 С���� (m) ") 
		<< QStringLiteral(" �� �߼м�С����ƫ��(��) ");
	_geoParameterTable->setRowCount(vHeaders.count());
	_geoParameterTable->setVerticalHeaderLabels(vHeaders);
	_geoParameterTable->horizontalHeader()->hide();


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
	_geoParameterTable->setItem(9, 0, new QTableWidgetItem("0"));
	_geoParameterTable->setFontPosition(_geoParameterTable->rowCount(), _geoParameterTable->columnCount());

	_geoParameterTable->item(1, 0)->setToolTip(QStringLiteral("�˴�������Ѽ�࣬������Ϊ������ʱ�����Ѽ��Ӧ����Ϊ0��"));
	_geoParameterTable->item(2, 0)->setToolTip(QStringLiteral("�˴�����L1��������Ҳ�ʾ��ͼ������д"));
	_geoParameterTable->item(3, 0)->setToolTip(QStringLiteral("�˴�����L2��������Ҳ�ʾ��ͼ������д \n���޾�ѹ��ʱ��L2��ֵΪ��Ե�Ӹ�ѹ��ľ���"));
	_geoParameterTable->item(4, 0)->setToolTip(QStringLiteral("�˴�����L3��������Ҳ�ʾ��ͼ������д \n���޾�ѹ��ʱL3��0 \n����û���Ҫ���Ǹ�ѹ�˵ľ�Ե��ɡȹ��Ե���˴�Ҳ���������Ե��ֱ��"));
	_geoParameterTable->item(5, 0)->setToolTip(QStringLiteral("�˴�����L4��������Ҳ�ʾ��ͼ������д"));
	_geoParameterTable->item(6, 0)->setToolTip(QStringLiteral("�˴�����L5��������Ҳ�ʾ��ͼ������д"));
	_geoParameterTable->item(7, 0)->setToolTip(QStringLiteral("�˴�����L6��������Ҳ�ʾ��ͼ������д"));
	_geoParameterTable->item(8, 0)->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/С����.png' />").arg(QCoreApplication::applicationDirPath()));
	_geoParameterTable->item(9, 0)->setToolTip(QStringLiteral("�˴�����ȣ�������Ҳ�ʾ��ͼ������д"));

	connect(_splitNumCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slotEditAngleTable()));
	connect(_geoParameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotEditAngleTable()));


	/*����*/
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ���������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
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
		_insultorPara_V->setInsultorLen(calcInsultorLen_V());//V ����Ե�Ӵ����ȣ�Ϊ��Чֵ����I����Ч�����ں˼����Ե�Ӵ���ѹ��
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ�������Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ������߼��β�����Ϣ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ������߼��β�����Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: ˢ�½�������   -- by mzq  2020/1/19 13:59
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

// @function: V���н�ȡֵ�����ʾ  -mzq 2019/5/22 21:51
void InsulatorParaDlg_V::slotEditAngleTable()
{
	if (_isReadData)//��ʼ���ض�ȡ����Ҳ�ᴥ�����ݸı��źţ�����Щ���˵�
		return;

	double alphaRecommend = getVAngleRecommend();
	_angleValueTable->item(1, 0)->setText(QString::number(alphaRecommend, 10, 2));
}

// @function: V ���н��Ƽ�ֵ  -mzq 2019/5/23 11:58
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

//@function: ���ݾ�Ե�Ӵ����β��������Ե�Ӵ��ܳ��ȣ��˳���Ϊ��Ч���ȣ�Ϊ��ֱ���ϵ�ͶӰ����   -- by mzq  2019/5/28 15:25
double InsulatorParaDlg_V::calcInsultorLen_V()
{
	writeInsulaGeoParaData_V();

	/*�����Ե�Ӵ������������׼��*/
	SplitNum::Type splitType = _insulaGeoPara_V->getSplitNum();
	double splitSpace = _insulaGeoPara_V->getSpaceSplit();
	double splitHeight = _insulaGeoPara_V->getHeightSplit();
	double L1 = _insulaGeoPara_V->getL1();
	double L5 = _insulaGeoPara_V->getL5();
	double splitAboveLen = L1 * cos(SmartCl::Auxiliary::angle_to_radian(90 / 2)) + L5;//90ΪĬ��V���ܼнǣ���Ե�Ӵ����ȶԼ���Ӱ�첻�󣬸����ɺ���
	double wireClipLen = _insulaGeoPara_V->getL6();

	double insultorLen = SmartCl::Auxiliary::calcInsultorLen(splitType, splitSpace, splitHeight, splitAboveLen, wireClipLen);
	return insultorLen;
}
// @ function: ��Ե�Ӵ���鿴  --- by ljn 2020/1/17 14:21
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