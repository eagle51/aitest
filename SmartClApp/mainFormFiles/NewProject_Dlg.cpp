#include "NewProject_Dlg.h"
#include "DataBase.h"
#include "OdqInterfaces.h"
#include "EnumLib.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QProcessEnvironment>
#include <QMessageBox>


NewProject_Dlg::NewProject_Dlg()
{
	setWindowTitle(QString::fromLocal8Bit("新建工程"));
	this->initDlg();
}


NewProject_Dlg::~NewProject_Dlg()
{

}

void NewProject_Dlg::initDlg()
{
	QLabel *nameLbl = new QLabel(QString::fromLocal8Bit("工程名称:"));
	_proNameEdt = new QLineEdit();

	_proNameEdt->setText("Untitled");
	QLabel *pathLbl = new QLabel(QString::fromLocal8Bit("保存路径:"));
	_pathEdt = new QLineEdit();
	_pathEdt->setFixedWidth(250);
	QString defaultStr = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	_pathEdt->setText(defaultStr);
	_browsBtn = new QPushButton(QString::fromLocal8Bit("浏览"));
	QLabel *proNumLbl = new QLabel(QString::fromLocal8Bit("工程代号:"));
	_proNumEdt = new QLineEdit();
	_proNumEdt->setText("Untitled");
	QLabel *volNumLbl = new QLabel(QString::fromLocal8Bit("卷册号:"));
	_volNumEdt = new QLineEdit();
	_volNumEdt->setText("Untitled");

	QGridLayout  *gridLyt = new QGridLayout();
	gridLyt->addWidget(nameLbl, 0, 0);
	gridLyt->addWidget(_proNameEdt, 0, 1);
	gridLyt->addWidget(pathLbl, 1, 0);
	gridLyt->addWidget(_pathEdt, 1, 1);
	gridLyt->addWidget(_browsBtn, 1, 2);
	gridLyt->addWidget(proNumLbl, 2, 0);
	gridLyt->addWidget(_proNumEdt, 2, 1);
	gridLyt->addWidget(volNumLbl, 3, 0);
	gridLyt->addWidget(_volNumEdt, 3, 1);

	QPushButton *okBtn = new QPushButton(QString::fromLocal8Bit("确定"));
	QPushButton *cancelBtn = new QPushButton(QString::fromLocal8Bit("取消"));
	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addStretch();
	btnLyt->addWidget(okBtn);
	btnLyt->addWidget(cancelBtn);

	QVBoxLayout *mainLyt = new QVBoxLayout();
	mainLyt->addLayout(gridLyt);
	mainLyt->addLayout(btnLyt);
	this->setLayout(mainLyt);

	connect(_browsBtn, SIGNAL(clicked()), this, SLOT(slotClickBrowsBtn()));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(slotClickOk()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(slotClickCancel()));
}
//@ function:
void NewProject_Dlg::slotClickBrowsBtn()//浏览
{
	QString file_path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择项目保存路径"), "D:\\");
	if (!file_path.isEmpty())
		_pathEdt->setText(file_path);
	
}
//@ function:
void NewProject_Dlg::slotClickOk()
{
	try
	{
		/*处理当前工程*/
		emit clearPro();
		SmartCl::DataBase::getInstance()->clearCl();
		
		//检查该名称的项目在本地存在
		if (checkProExisted(_proNameEdt->text(), _pathEdt->text()))
			return;

		//创建文件夹
			createProFileFolder(_proNameEdt->text(), _pathEdt->text());
			
		//将数据保存至数据库中
		QString pathNameQStr = _pathEdt->text() + "\\" + _proNameEdt->text() + "\\" + _proNameEdt->text();
		std::string pathNameStr = pathNameQStr.toLocal8Bit().toStdString() + ".cl";
		SmartCl::DataBase::getInstance()->setSavePath(pathNameStr);
		SmartCl::DataBase::getInstance()->setProjectNO(_proNumEdt->text().toLocal8Bit().toStdString());
		SmartCl::DataBase::getInstance()->setVolNO(_volNumEdt->text().toLocal8Bit().toStdString());

		emit loadParaLibFile(true);//加载工程库文件
		emit crateProTreew();//创建并刷新工程树列表

		QString recentPath = pathNameQStr + ".cl";
		emit writeRecentFiles(recentPath);
		SmartCl::DataBase::getInstance()->save();
		accept();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("新建工程"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@ function:
void NewProject_Dlg::slotClickCancel()
{
	reject();
}
//@ function:
bool NewProject_Dlg::checkProExisted(QString& name, QString& path)
{
	bool hasExist=false;
	QFileInfo file(path);
	if (!file.isDir())
		throw std::exception("该路径无效");
	QDir dir;
	dir.cd(path);
	if (dir.exists(name))//判断需要创建的文件夹是否存在
	{
		QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, QString::fromLocal8Bit("新建工程"), QString::fromLocal8Bit("该工程已存在，是否覆盖原工程文件"),
															  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (rb == QMessageBox::Yes)
		{
			hasExist = false;
		}
		else if (rb == QMessageBox::No)
			hasExist = true;	
	}
	return hasExist;
}

//文件夹删除 ***慎用***
bool NewProject_Dlg::delDir(const QString &path)
{
	if (path.isEmpty())
		return false;
	QDir dir(path);
	if (!dir.exists())
		return true;
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤  
	QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息  
	foreach(QFileInfo file, fileList)
	{ //遍历文件信息  
		if (file.isFile()) // 是文件，删除  
			file.dir().remove(file.fileName());
		else// 递归删除  
			delDir(file.absoluteFilePath());
	}
	return dir.rmpath(dir.absolutePath()); // 删除文件夹  
}
//创建工程文件夹
void NewProject_Dlg::createProFileFolder(QString& name, QString& path)
{
	QDir dir;
	dir.cd(path);
	dir.mkdir(name); //创建文件夹
	QString pathNameQStr = path + "\\" + name;
	dir.cd(pathNameQStr);
	dir.mkdir(QString::fromLocal8Bit("间隙圆计算书")); //创建文件夹
	dir.mkdir(QString::fromLocal8Bit("间隙圆图纸")); //创建文件夹
	dir.mkdir(QString::fromLocal8Bit("文件")); //创建文件夹
}
//@ function:
void NewProject_Dlg::existPro()
{
	if(0 == SmartCl::DataBase::getInstance()->getClCount())
		return;
	try
	{
		QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, QString::fromLocal8Bit("新建工程"), QString::fromLocal8Bit("是否保存当前工程"),
															  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (rb == QMessageBox::Yes)
		{
			IqConsole* iConsole = getIConsole();
			if (!iConsole)
			{
				ODA_FAIL();
				return;
			}
			QString qsCmd = QString("savewidgetdata ");
			iConsole->executeCommand(qsCmd);
			SmartCl::DataBase::getInstance()->save();
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("保存并清除当前工程"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//检查并创建文件夹
void NewProject_Dlg::checkAndCreateFolder(QString& name, QString& path)
{
	QFileInfo file(path);
	if (!file.isDir())
		throw std::exception("该路径无效");
	QDir dir;
	dir.cd(path);
	if (!dir.exists(name))//判断需要创建的文件夹是否存在
		dir.mkdir(name);
}
