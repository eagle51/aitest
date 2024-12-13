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
	this->setWindowTitle(QString::fromLocal8Bit("新增间隙圆"));
	this->initDlg();
}

AddClByType_Dlg::~AddClByType_Dlg()
{

}
//@ 初始化界面:
void AddClByType_Dlg::initDlg()
{
	QLabel *clTypeLbl = new QLabel(QString::fromLocal8Bit("间隙圆类型"));
	_clTypeCbx = new QComboBox();
	_clTypeCbx->setFixedWidth(220);
	
	QStringList name;
	SmartCl::ClCate::Type type;
	for (type = SmartCl::ClCate::StraightIStr; type < SmartCl::ClCate::ErrType; type = (SmartCl::ClCate::Type)(type + 1))
		name << QString::fromLocal8Bit(SmartCl::ClCate::typeToStr(type));
	_clTypeCbx->addItems(name);

	QLabel *clNameLbl = new QLabel(QString::fromLocal8Bit("间隙圆名称"));
	_clNameEdt = new QLineEdit();
	QGridLayout *gridLyt = new QGridLayout();
	gridLyt->addWidget(clTypeLbl, 0, 0);
	gridLyt->addWidget(_clTypeCbx, 0, 1);
	gridLyt->addWidget(clNameLbl, 1, 0);
	gridLyt->addWidget(_clNameEdt, 1, 1);

	QPushButton *okBtn = new QPushButton(QString::fromLocal8Bit("确定"));
	QPushButton *cancelBtn = new QPushButton(QString::fromLocal8Bit("取消"));
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
		//检查间隙圆名称是否有输入
		m_strClName = _clNameEdt->text();
		if (m_strClName == "")
			throw std::exception("间隙圆名称为空");
		//检查是否已存在该间隙圆
		if (SmartCl::DataBase::getInstance()->existCl(m_strClName.toLocal8Bit().toStdString()))
			throw std::exception("当前间隙圆名称已存在，无法创建，需重新填写");

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
		QMessageBox::warning(this, QString::fromLocal8Bit("新增间隙圆"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
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
