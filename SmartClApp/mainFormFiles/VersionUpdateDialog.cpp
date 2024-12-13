#include "VersionUpdateDialog.h"
#include <QLabel>
#include <QGridLayout>
#include <QDesktopServices>
#include <QUrl>
#include<QSettings>
#include <QCoreApplication>

VersionUpdateDialog::VersionUpdateDialog(QString warningStr, QString downloadUrl, QWidget *parent/* = Q_NULLPTR*/)
	: QDialog(parent)
{
	_warningStr = warningStr;
	_downloadUrl = downloadUrl;
	this->setWindowTitle(QString::fromLocal8Bit("检查更新"));
	this->setModal(true);
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	_isNotShow = settings.value("notice", false).toBool();
	initDlg();
}

VersionUpdateDialog::~VersionUpdateDialog()
{

}

//@function:    -- by xyy  2020/8/31 15:08
void VersionUpdateDialog::initDlg()
{
	QLabel *updateLbl = new QLabel();
	updateLbl->setText(QString(_warningStr));

	_downBtn = new QPushButton();
	_downBtn->setText(QString::fromLocal8Bit("下载"));
	_notDownBtn = new QPushButton();
	_notDownBtn->setText(QString::fromLocal8Bit("不更新"));

	_notShowChk = new QCheckBox();
	_notShowChk->setText(QString::fromLocal8Bit("不再显示"));
	if (_isNotShow)
	{
		_notShowChk->setCheckState(Qt::Checked);
	}
	else
	{
		_notShowChk->setCheckState(Qt::Unchecked);
	}
	

	QGridLayout *gLyt = new QGridLayout();
	gLyt->addWidget(updateLbl, 0, 3, 1, 3);
	gLyt->addWidget(_notShowChk, 2, 3, 1, 2);
	gLyt->addWidget(_downBtn, 3, 4, 1, 1);
	gLyt->addWidget(_notDownBtn, 3, 5, 1, 1);

	this->setLayout(gLyt);
	connect(_downBtn, SIGNAL(clicked()), this, SLOT(slotUpdateOperate()));
	connect(_notDownBtn, SIGNAL(clicked()), this, SLOT(slotUpdateOperate()));
	connect(_notShowChk, SIGNAL(stateChanged(int)), this, SLOT(slotSaveState(int)));
}


//@function:    -- by xyy  2020/8/31 15:48
void VersionUpdateDialog::slotUpdateOperate()
{
	if (_downBtn == qobject_cast<QPushButton*>(QObject::sender()))    //点击更新
	{
		QDesktopServices::openUrl(QUrl(_downloadUrl));
		accept();
	}
	reject();
}

//@function:    -- by xyy  2020/8/31 16:18
void VersionUpdateDialog::slotSaveState(int state)
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	if (!state)
	{
		_isNotShow = false;
		settings.setValue("notice", _isNotShow);
		_notShowChk->setCheckState(Qt::Unchecked);
	} 
	else
	{
		_isNotShow = true;
		settings.setValue("notice", _isNotShow);
		_notShowChk->setCheckState(Qt::Checked);
	}
}
