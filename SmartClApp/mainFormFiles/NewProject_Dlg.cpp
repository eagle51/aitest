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
	setWindowTitle(QString::fromLocal8Bit("�½�����"));
	this->initDlg();
}


NewProject_Dlg::~NewProject_Dlg()
{

}

void NewProject_Dlg::initDlg()
{
	QLabel *nameLbl = new QLabel(QString::fromLocal8Bit("��������:"));
	_proNameEdt = new QLineEdit();

	_proNameEdt->setText("Untitled");
	QLabel *pathLbl = new QLabel(QString::fromLocal8Bit("����·��:"));
	_pathEdt = new QLineEdit();
	_pathEdt->setFixedWidth(250);
	QString defaultStr = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	_pathEdt->setText(defaultStr);
	_browsBtn = new QPushButton(QString::fromLocal8Bit("���"));
	QLabel *proNumLbl = new QLabel(QString::fromLocal8Bit("���̴���:"));
	_proNumEdt = new QLineEdit();
	_proNumEdt->setText("Untitled");
	QLabel *volNumLbl = new QLabel(QString::fromLocal8Bit("����:"));
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

	QPushButton *okBtn = new QPushButton(QString::fromLocal8Bit("ȷ��"));
	QPushButton *cancelBtn = new QPushButton(QString::fromLocal8Bit("ȡ��"));
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
void NewProject_Dlg::slotClickBrowsBtn()//���
{
	QString file_path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("��ѡ����Ŀ����·��"), "D:\\");
	if (!file_path.isEmpty())
		_pathEdt->setText(file_path);
	
}
//@ function:
void NewProject_Dlg::slotClickOk()
{
	try
	{
		/*����ǰ����*/
		emit clearPro();
		SmartCl::DataBase::getInstance()->clearCl();
		
		//�������Ƶ���Ŀ�ڱ��ش���
		if (checkProExisted(_proNameEdt->text(), _pathEdt->text()))
			return;

		//�����ļ���
			createProFileFolder(_proNameEdt->text(), _pathEdt->text());
			
		//�����ݱ��������ݿ���
		QString pathNameQStr = _pathEdt->text() + "\\" + _proNameEdt->text() + "\\" + _proNameEdt->text();
		std::string pathNameStr = pathNameQStr.toLocal8Bit().toStdString() + ".cl";
		SmartCl::DataBase::getInstance()->setSavePath(pathNameStr);
		SmartCl::DataBase::getInstance()->setProjectNO(_proNumEdt->text().toLocal8Bit().toStdString());
		SmartCl::DataBase::getInstance()->setVolNO(_volNumEdt->text().toLocal8Bit().toStdString());

		emit loadParaLibFile(true);//���ع��̿��ļ�
		emit crateProTreew();//������ˢ�¹������б�

		QString recentPath = pathNameQStr + ".cl";
		emit writeRecentFiles(recentPath);
		SmartCl::DataBase::getInstance()->save();
		accept();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("�½�����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
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
		throw std::exception("��·����Ч");
	QDir dir;
	dir.cd(path);
	if (dir.exists(name))//�ж���Ҫ�������ļ����Ƿ����
	{
		QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, QString::fromLocal8Bit("�½�����"), QString::fromLocal8Bit("�ù����Ѵ��ڣ��Ƿ񸲸�ԭ�����ļ�"),
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

//�ļ���ɾ�� ***����***
bool NewProject_Dlg::delDir(const QString &path)
{
	if (path.isEmpty())
		return false;
	QDir dir(path);
	if (!dir.exists())
		return true;
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //���ù���  
	QFileInfoList fileList = dir.entryInfoList(); // ��ȡ���е��ļ���Ϣ  
	foreach(QFileInfo file, fileList)
	{ //�����ļ���Ϣ  
		if (file.isFile()) // ���ļ���ɾ��  
			file.dir().remove(file.fileName());
		else// �ݹ�ɾ��  
			delDir(file.absoluteFilePath());
	}
	return dir.rmpath(dir.absolutePath()); // ɾ���ļ���  
}
//���������ļ���
void NewProject_Dlg::createProFileFolder(QString& name, QString& path)
{
	QDir dir;
	dir.cd(path);
	dir.mkdir(name); //�����ļ���
	QString pathNameQStr = path + "\\" + name;
	dir.cd(pathNameQStr);
	dir.mkdir(QString::fromLocal8Bit("��϶Բ������")); //�����ļ���
	dir.mkdir(QString::fromLocal8Bit("��϶Բͼֽ")); //�����ļ���
	dir.mkdir(QString::fromLocal8Bit("�ļ�")); //�����ļ���
}
//@ function:
void NewProject_Dlg::existPro()
{
	if(0 == SmartCl::DataBase::getInstance()->getClCount())
		return;
	try
	{
		QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, QString::fromLocal8Bit("�½�����"), QString::fromLocal8Bit("�Ƿ񱣴浱ǰ����"),
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
		QMessageBox::warning(nullptr, QString::fromLocal8Bit("���沢�����ǰ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//��鲢�����ļ���
void NewProject_Dlg::checkAndCreateFolder(QString& name, QString& path)
{
	QFileInfo file(path);
	if (!file.isDir())
		throw std::exception("��·����Ч");
	QDir dir;
	dir.cd(path);
	if (!dir.exists(name))//�ж���Ҫ�������ļ����Ƿ����
		dir.mkdir(name);
}
