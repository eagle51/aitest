#include "ClWidget.h"
#include "OdqInterfaces.h"
#include "..\SmartClApp\OdqMainWindow.h"

#include <QMdiArea>

ClWidget::ClWidget(int clType, std::string clName)
{
	_cl = getCoreInstance()->getClByName(clName);

	_clTypeTitle = new QLabel();
	_clTypeTitle->setAlignment(Qt::AlignCenter);
	QFont font;
	font.setPointSize(12);
	_clTypeTitle->setFont(font);

	//���ɼ����鰴ť
	_pReportBtn = new QPushButton(QString::fromLocal8Bit("���ɼ�����"));
	_pReportBtn->setFixedSize(85, 60);

	//���Ƽ�϶Բ��ť
	_pDrawClearanceBtn = new QPushButton(QString::fromLocal8Bit("���Ƽ�϶Բ"));
	_pDrawClearanceBtn->setFixedSize(85, 60);

	_pErrModel = new ReportErrModel();
	_pErrTableView = new QTableView(this);
}

ClWidget::~ClWidget()
{

}

//@function: ��ȡ�ں����ݿ�ָ��
SmartCl::DataBase* ClWidget::getCoreInstance()
{
	return static_cast<OdqMainWindow*>(getIApp()->getMainWindow())->getCoreDbPt();
}

//@function: ��˼���ܼ��   -- by mzq 2022/11/4 18:12
AboutSoftWare::VirBoxChecker* ClWidget::getVirBoxCheckerInstance()
{
	return static_cast<OdqMainWindow*>(getIApp()->getMainWindow())->getVirBoxChecker();
}

// @function: ������
QWidget* ClWidget::initErrTable()
{
	_pErrTableView->setAlternatingRowColors(true);
	_pErrTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	_pErrTableView->setSelectionMode(QAbstractItemView::SingleSelection);
	_pErrTableView->setModel(_pErrModel);
	_pErrTableView->setColumnWidth(0, 35);
	_pErrTableView->setColumnWidth(1, 110);
	_pErrTableView->horizontalHeader()->setStretchLastSection(true);
	_pErrTableView->setEditTriggers(false);
	_pErrTableView->verticalHeader()->hide();

	return _pErrTableView;
}