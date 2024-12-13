#include "AddClByType_Dlg.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include "DataBase.h"
#include "ClFactory.h"

AddClByType_Dlg::AddClByType_Dlg(QWidget *parent)
	:QDialog(parent)
{
	this->setWindowTitle(QString::fromLocal8Bit("������϶Բ"));
	this->initDlg();
}

AddClByType_Dlg::~AddClByType_Dlg()
{

}
//@ ��ʼ������:
void AddClByType_Dlg::initDlg()
{
	QLabel *clTypeLbl = new QLabel(QString::fromLocal8Bit("��϶Բ����"));
	_clTypeCbx = new QComboBox();
	_clTypeCbx->setFixedWidth(220);
	
	QStringList name;
	SmartCl::ClCate::Type type;
	for (type = SmartCl::ClCate::StraightIStr; type < SmartCl::ClCate::ErrType; type = (SmartCl::ClCate::Type)(type + 1))
		name << QString::fromLocal8Bit(SmartCl::ClCate::typeToStr(type));
	_clTypeCbx->addItems(name);

	QLabel *clNameLbl = new QLabel(QString::fromLocal8Bit("��϶Բ����"));
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
		m_strClName = _clNameEdt->text();
		if (m_strClName == "")
			throw std::exception("��϶Բ����Ϊ��");
		//����Ƿ��Ѵ��ڸü�϶Բ
		if (SmartCl::DataBase::getInstance()->existCl(m_strClName.toLocal8Bit().toStdString()))
			throw std::exception("��ǰ��϶Բ�����Ѵ��ڣ��޷���������������д");

		m_strClType = _clTypeCbx->currentText();
		SmartCl::ClCate::Type clType = SmartCl::ClCate::strToType(m_strClType.toLocal8Bit().toStdString());
		auto tempCl = SmartCl::ClFactory::getInstance()->getClearence(clType, m_strClName.toLocal8Bit());
		SmartCl::DataBase::getInstance()->addCl(tempCl);
		SmartCl::DataBase::getInstance()->setCurrentClName(m_strClName.toLocal8Bit().toStdString());
		emit createTreeByClName(m_strClName);
		accept();
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

QString AddClByType_Dlg::getClType()
{
	return m_strClType;
	
}
QString AddClByType_Dlg::getClName()
{
	return m_strClName;
}
