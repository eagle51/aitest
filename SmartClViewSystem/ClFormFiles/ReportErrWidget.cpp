#include "ReportErrWidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

ReportErrWidget::ReportErrWidget()
{
	//this->setMinimumWidth(800);
	//ReportErrModel *model = new ReportErrModel();
	//_calExcep = new Collection_FdCalcExcept();
	//_calExcep->add(0, "222222222222222");
	//_calExcep->add(1, "333333333333333333");
	//_calExcep->add(2, "dffffffffffffffff");
	//_calExcep->add(3, "vbbbbbbbbbbbbbbbbbbb");
	//_calExcep->add(4, "iuuuuuuuuuuuuuuuuuuuuuuuu");
	//model->updateModel(_calExcep);
	//_pView = new QTableView();
	//_pView->setModel(model);
	//_pView->setSelectionBehavior(QTableView::SelectRows);
	//_pView->setEditTriggers(false);
	//_pView->verticalHeader()->hide();
	//connect(_pView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(soltDoubleClicked(const QModelIndex &)));
	////ToDo增加可复制

	//QVBoxLayout *mainVLyt = new QVBoxLayout();
	//mainVLyt->addWidget(_pView);
	//this->setLayout(mainVLyt);
}


ReportErrWidget::~ReportErrWidget()
{
}

//void ReportErrWidget::soltDoubleClicked(const QModelIndex & index)
//{
//	int errType=_calExcep->at(index.row())->getErrType();
//	emit  doubleClicked(errType);
//}
