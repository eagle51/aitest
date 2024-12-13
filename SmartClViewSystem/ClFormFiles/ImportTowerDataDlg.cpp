#include "ImportTowerDataDlg.h"
#include "TTAFormatIO.h"
#include "DaoHengFormatIO.h"
#include "JSONIo.h"
#include "ZheJiangTSTFormatIO.h"
#include "../SmartPoleLib/IO/JsonFormatIO.h"
#include "../SmartPoleLib/DataBase/ModelDateBase.h"

#include "MaxAngleWgt.h"
#include "OsgUIManager.h"

#include "Question_Dlg.h"
#include "qmessagebox.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>
#include "qaction.h"
#include "qboxlayout.h"
#include "qgroupbox.h"
#include "XBTTAPFormatIO.h"

ImportTowerDataDlg::ImportTowerDataDlg(QSPtr_OsgUIManager osgUIManager, MaxAngleWgt *parent) :_osgUIManager(osgUIManager), _maxAngleWgt(parent)
{
	this->setWindowTitle(QStringLiteral("导入杆塔模型"));

	_typicalHeadTmplWidget = new TypicalHeadTmplWidget(this);
	_typicalHeadTmplWidget->setModal(true);
	connect(_typicalHeadTmplWidget, SIGNAL(showModel(bool)), this, SLOT(loadOsgTowerModel(bool)));
	QPushButton *templateBtn = new QPushButton(QStringLiteral("塔头模版"));
	connect(templateBtn, SIGNAL(clicked()), _typicalHeadTmplWidget, SLOT(show()));
	QHBoxLayout *templateBtnLyt = new QHBoxLayout();
	templateBtnLyt->addWidget(templateBtn);
	QGroupBox *templateBtnGBox = new QGroupBox(QStringLiteral("插入塔头模版"));
	templateBtnGBox->setLayout(templateBtnLyt);


	_DBTTABtn = new QPushButton(QStringLiteral("TTA或SmartTower格式"));
	connect(_DBTTABtn, SIGNAL(clicked()), this, SLOT(ShowDBTTA2008Dialog()));
	_XBTTABtn = new QPushButton(QStringLiteral("西北院格式文件"));
	connect(_XBTTABtn, SIGNAL(clicked()), this, SLOT(showXBTTADialog()));
	_DaoHBtn = new QPushButton(QStringLiteral("道亨MYL格式"));
	connect(_DaoHBtn, SIGNAL(clicked()), this, SLOT(showDaoHDialog()));
	_HNEPDITTABtn = new QPushButton(QStringLiteral("河南院TANS格式"));
	connect(_HNEPDITTABtn, SIGNAL(clicked()), this, SLOT(showHenanDialog()));
	_ZheJiangTTABtn = new QPushButton(QStringLiteral("浙江院TST格式"));
	connect(_ZheJiangTTABtn, SIGNAL(clicked()), this, SLOT(showZheJiangOUTCDialog()));
	_GDEPDITTABtn = new QPushButton(QStringLiteral("广东拓展2.0格式"));
	connect(_GDEPDITTABtn, SIGNAL(clicked()), this, SLOT(showGDEPDIDialog()));
	_JSONFormatBtn = new QPushButton(QStringLiteral("导入JSON格式"));
	connect(_JSONFormatBtn, SIGNAL(clicked()), this, SLOT(showJSONDialog()));
	_PoleBtn = new QPushButton(QStringLiteral("导入SmartPole钢管杆格式"));
	connect(_PoleBtn, SIGNAL(clicked()), this, SLOT(showPoleDialog()));

	QVBoxLayout *btnLyt = new QVBoxLayout();
	btnLyt->addWidget(_DBTTABtn);
	btnLyt->addWidget(_XBTTABtn);
	btnLyt->addWidget(_DaoHBtn);
	btnLyt->addWidget(_HNEPDITTABtn);
	btnLyt->addWidget(_ZheJiangTTABtn);
	btnLyt->addWidget(_GDEPDITTABtn);
	btnLyt->addWidget(_JSONFormatBtn);
	btnLyt->addWidget(_PoleBtn);
	QGroupBox *modelGBox = new QGroupBox(QStringLiteral("已有杆塔数据"));
	modelGBox->setLayout(btnLyt);
	modelGBox->setMinimumWidth(210);


	_returnBtn = new QPushButton(QStringLiteral("返回"));
	connect(_returnBtn, SIGNAL(clicked()), this, SLOT(reject()));
	QHBoxLayout *returnBtnLyt = new QHBoxLayout();
	returnBtnLyt->addStretch();
	returnBtnLyt->addWidget(_returnBtn);

	QVBoxLayout *totalLyt= new QVBoxLayout();
	totalLyt->addWidget(templateBtnGBox);
	totalLyt->addWidget(modelGBox);
	totalLyt->addLayout(returnBtnLyt);
	this->setLayout(totalLyt);
}


ImportTowerDataDlg::~ImportTowerDataDlg(void)
{

}

void ImportTowerDataDlg::ShowDBTTA2008Dialog()
{ 
	//读入东北院数据
	SmartTower::HandleModelIO ttaIO = new SmartTower::CTTAFormatIO(0, false);
	_titleStr = tr("input tta2.0/3.0 dat file");
	if (loadFile(ttaIO, 1))
		InnerCornerSideTip();
}

void ImportTowerDataDlg::showXBTTADialog()
{//读入西北院数据
	SmartTower::HandleModelIO ttaIO;
	_titleStr = tr("input tta dat file");
	ttaIO = new SmartTower::CXBTTAPFormatIO(0, true);
	if (loadFile(ttaIO, 1))
		InnerCornerSideTip();
}

void ImportTowerDataDlg::showDaoHDialog()
{
	SmartTower::HandleModelIO daoIO = new SmartTower::DaoHengFormatIO(0);
	_titleStr = tr("input dao myl file");
	if (loadFile(daoIO, 2))
		InnerCornerSideTip();
}

void ImportTowerDataDlg::showHenanDialog()
{
	SmartTower::HandleModelIO ttaIO = new SmartTower::HenanTTAFormat(0, false);
	QMessageBox::information(_maxAngleWgt, tr("HeNan Info"), tr("Please check TANS data.ini file"), QMessageBox::Ok);
	_titleStr = tr("Input Henan Tans Data File");
	if (loadFile(ttaIO, 1))
		InnerCornerSideTip();
}

void ImportTowerDataDlg::showZheJiangOUTCDialog()
{
	SmartTower::HandleModelIO ttaIO = new SmartTower::ZheJiangTSTFormatIO(0, false);
	_titleStr = tr("Input ZheJiang data file,note: Please ini renamed to data.ini prior to import");
	QMessageBox::information(_maxAngleWgt, tr("Zhejiang TST"), _titleStr, QMessageBox::Ok);
	if (loadFile(ttaIO, 3))
		InnerCornerSideTip();
}

void ImportTowerDataDlg::showGDEPDIDialog()
{
	SmartTower::HandleModelIO ttaIO = new SmartTower::GDEPDITTAFormart(0, false);
	_titleStr = tr("Input GuangDong data file!");
	QMessageBox::information(_maxAngleWgt, tr("GuangDong TST"), _titleStr, QMessageBox::Ok);
	loadFile(ttaIO, 1);
}

void ImportTowerDataDlg::showJSONDialog()
{
	QString fileName = QFileDialog::getOpenFileName(_maxAngleWgt, tr("json file"), "", "Json (*.json)");
	if (fileName.isEmpty())
		return;
	this->ReadJsonFile(fileName);
}

//@function:    -- by mzq 2023/6/28 11:57
void ImportTowerDataDlg::showPoleDialog()
{
	QString fileName = QFileDialog::getOpenFileName(_maxAngleWgt, tr("SmartPole file"), "", "Pole (*.Pole)");
	if (fileName.isEmpty())
		return;
	this->ReadJsonFile(fileName, true);
}

bool ImportTowerDataDlg::loadFile(const SmartTower::HandleModelIO& reader, int iTTTDataFormat)
{
	QString strDefaultPath = QCoreApplication::applicationDirPath() + "/../test/";
	QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), strDefaultPath);
	if (fileName.isEmpty())
		return false;
	QFileInfo fileinfo(fileName);
	QString fileName2 = fileinfo.absolutePath() + "/" + fileinfo.baseName();
	if (!QFile::exists(fileName))
	{
		QMessageBox::warning(_maxAngleWgt, tr("File Error"), fileName + tr("--Model file does not exist!"), QMessageBox::Ok);
		return false;
	}

	QString completeSuffix = fileinfo.completeSuffix();
	std::string Loacl8Bitfile = fileName2.toLocal8Bit();

	if (iTTTDataFormat == 2)
		Loacl8Bitfile = fileName.toLocal8Bit();

	SmartTower::CTowerModel::Instance()->ClearAllInfo();
	SmartTower::CTowerModel::Instance()->Filename(Loacl8Bitfile.c_str());
	reader->setTowerModel(SmartTower::CTowerModel::Instance());

	if (iTTTDataFormat != 2 && completeSuffix.isEmpty())
		reader->modelFileHasPostfix(false);

	if (!ReadDatAndDat2(reader))
		return false;

	SmartTower::CTowerModel::Instance()->CheckAndUpdata();
	loadOsgTowerModel();

	return true;
}

//@function:    -- by mzq 2023/6/28 15:40
void ImportTowerDataDlg::InnerCornerSideTip()
{

}

bool ImportTowerDataDlg::ReadDatAndDat2(const SmartTower::HandleModelIO& reader)
{
	bool Result = false;
	try
	{
		reader->Read();
		Result = true;
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(_maxAngleWgt, tr("File Error"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
	}
	return Result;
}

//@function:    -- by mzq 2023/6/28 11:58
void ImportTowerDataDlg::ReadJsonFile(const QString& fileName, bool isSmartPole /*= false*/)
{
	if (!QFile::exists(fileName))
	{
		QMessageBox::warning(_maxAngleWgt, tr("File Error"), fileName + tr("--Model file does not exist!"), QMessageBox::Ok);
		return;
	}
	QFileInfo fileinfo(fileName);
	QString fileName2 = fileinfo.absolutePath() + "/" + fileinfo.baseName();
	if (isSmartPole){ // smartPole格式
		SmartPoleCore::ModelDateBase* instance = SmartPoleCore::ModelDateBase::Instance();
		instance->clearAll();
		instance->setFilename(fileName2.toLocal8Bit());
		SmartPoleCore::JsonFormatIO modelReader;
		modelReader.read();
	}
	else{
		SmartTower::CTowerModel::Instance()->ClearAllInfo();
		SmartTower::CTowerModel::Instance()->Filename(fileName2.toLocal8Bit());
		SmartTower::JSONIo io(SmartTower::CTowerModel::Instance());
		io.read(fileName.toLocal8Bit());
		SmartTower::CTowerModel::Instance()->CheckAndUpdata();
	}

	loadOsgTowerModel(isSmartPole);
	InnerCornerSideTip();
}

//@function: 加载Osg铁塔模型   -- by mzq  2019/12/4 16:20
void ImportTowerDataDlg::loadOsgTowerModel(bool isSmartPole)
{
	_osgUIManager->writeTowerModelToCurrentCl(isSmartPole);
	_osgUIManager->clearAll();
	_maxAngleWgt->updateOsgView();
	_maxAngleWgt->initialHangPointTableData();
	_osgUIManager->loadTowerModel();
}