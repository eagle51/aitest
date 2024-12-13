#include "Question_Dlg.h"
#include "qcoreevent.h"
#include "qmessagebox.h"
#include "qboxlayout.h"


Question_Dlg::Question_Dlg()
{
	this->initDlg();
	this->changeLanguage();
}


Question_Dlg::~Question_Dlg()
{
}


void Question_Dlg::initDlg()
{
	_okBtn = new QPushButton(tr("Left"));
	_cancelBtn = new QPushButton(tr("Right"));
	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addStretch();
	btnLyt->addWidget(_okBtn);
	btnLyt->addWidget(_cancelBtn);

	QVBoxLayout *mainLyt = new QVBoxLayout();
	mainLyt->addLayout(btnLyt);
	this->setLayout(mainLyt);

	connect(_okBtn, SIGNAL(clicked()), this, SLOT(slotClickOk()));

	connect(_cancelBtn, SIGNAL(clicked()), this, SLOT(slotClickCancel()));
}

//@ function:
void Question_Dlg::slotClickOk()
{
	try
	{
		accept();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("µ¼Èë"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}
//@ function:
void Question_Dlg::slotClickCancel()
{
	reject();
}
// @ function:      -- by lt 2019/4/30 16:20
void Question_Dlg::changeEvent(QEvent *e)
{
	switch (e->type())
	{
	case QEvent::LanguageChange:
		changeLanguage();
		break;
	default:
		break;
	}
}
// @ function:      -- by lt 2019/4/30 16:20
void Question_Dlg::changeLanguage()
{
	this->setWindowTitle(tr("select inner corner side"));
	_okBtn->setText(tr("Left"));
	_cancelBtn->setText(tr("Right"));
}
