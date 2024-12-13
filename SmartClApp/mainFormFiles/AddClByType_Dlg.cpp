#include "AddClByType_Dlg.h"
#include "DataBase.h"
#include "ClFactory.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include "qlistview.h"

AddClByType_Dlg::AddClByType_Dlg()
{
	this->setWindowTitle(QString::fromLocal8Bit("���ģ��"));
	this->initDlg();
}

AddClByType_Dlg::~AddClByType_Dlg()
{

}
//@ ��ʼ������:
void AddClByType_Dlg::initDlg()
{
	QLabel *clTypeLbl = new QLabel(QString::fromLocal8Bit("����:"));
	_clTypeCbx = new QComboBox();
	_clTypeCbx->setFixedWidth(220);
	
	QStringList name;
	SmartCl::ClCate::Type type;
	for (type = SmartCl::ClCate::StraightIStr; type <= SmartCl::ClCate::MaxAlgle_V; type = (SmartCl::ClCate::Type)(type + 1))
		name << QString::fromLocal8Bit(SmartCl::ClCate::typeToStr(type));
	_clTypeCbx->addItems(name);
	_clTypeCbx->setView(new QListView());

	QLabel *clNameLbl = new QLabel(QString::fromLocal8Bit("����:"));
	_clNameEdt = new QLineEdit();
	QGridLayout *gridLyt = new QGridLayout();
	gridLyt->addWidget(clTypeLbl, 0, 0);
	gridLyt->addWidget(_clTypeCbx, 0, 1);
	gridLyt->addWidget(clNameLbl, 1, 0);
	gridLyt->addWidget(_clNameEdt, 1, 1);

	QPushButton *okBtn = new QPushButton(QString::fromLocal8Bit("ȷ��"));
	QPushButton *cancelBtn = new QPushButton(QString::fromLocal8Bit("ȡ��"));
	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addStretch();
	btnLyt->addWidget(okBtn);
	btnLyt->addWidget(cancelBtn);

	QVBoxLayout *mainVLyt = new QVBoxLayout();
	mainVLyt->addLayout(gridLyt);
	mainVLyt->addLayout(btnLyt);
	this->setLayout(mainVLyt);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(slotClickOk()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(slotClickCancel()));
}
//@ function:
void AddClByType_Dlg::slotClickOk()
{
	try
	{
		//����϶Բ�����Ƿ�������
		QString clNameQStr = _clNameEdt->text();
		if (clNameQStr == "")
			throw std::exception("��϶Բ����Ϊ��");
		//����Ƿ��Ѵ��ڸü�϶Բ
		if (SmartCl::DataBase::getInstance()->existCl(clNameQStr.toLocal8Bit().toStdString()))
			throw std::exception("��ǰ��϶Բ�����Ѵ��ڣ��޷���������������д");
		accept();
		QString clTypeQStr=_clTypeCbx->currentText();
		SmartCl::ClCate::Type clType = SmartCl::ClCate::strToType(clTypeQStr.toLocal8Bit().toStdString());
		auto tempCl = SmartCl::ClFactory::getInstance()->getClearence(clType, clNameQStr.toLocal8Bit());
		SmartCl::DataBase::getInstance()->addCl(tempCl);
		emit createTreeByClName(clNameQStr);
		
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("������϶Բ"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}
//@ function:
void AddClByType_Dlg::slotClickCancel()
{
	reject();
}
