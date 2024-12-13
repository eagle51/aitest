#include "insulatorstring_l.h"
#include "qlabel.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include <QHeaderView>
InsulatorString_L::InsulatorString_L(QFileInfo fileInfo, QWidget *parent)
	 : _fileInfo(fileInfo), QWidget(parent)
{
	QLabel *_leftAreaLbl = new QLabel(QString::fromLocal8Bit(" 需要加入的图片，后面继续完善"));

	_tableWidget = new QTableWidget(10, 10);
	_tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	_tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	_tableWidget->setStyleSheet("QTableView::Item{background-color:#FF3EFF} ");
	_tableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
	_tableWidget->setShowGrid(true);
	//左边表头命名
	QStringList vHeaders;
	vHeaders << QStringLiteral(" 绝缘子串型号 ")
		<< QStringLiteral(" 绝缘子串总重力 (N) ")
		<< QStringLiteral(" 垂直风向绝缘子串联数 (联) ")
		<< QStringLiteral(" 顺风向绝缘子串联数 (联) ")
		<< QStringLiteral(" 单联绝缘子串片数 (片) ")
		<< QStringLiteral(" 单片绝缘子受风面积 (m^2) ")
		<< QStringLiteral(" 重锤重力 (N) ")
		<< QStringLiteral(" L0 绝缘子串长度 (m) ")
		<< QStringLiteral(" 分裂数 ")
		<< QStringLiteral(" 分裂间距 (m) ")
		<< QStringLiteral(" L1 铁塔挂点-金具第一转动点 (m) ")
		<< QStringLiteral(" L2 铁塔挂点-上子导线线夹挂点 (m) ")
		<< QStringLiteral(" L3 上子导线线夹挂点-均压环 (m) ")
		<< QStringLiteral(" L4 均压环宽度 (m) ")
		<< QStringLiteral(" L5 子导线线夹长度 (m) ")
		<< QStringLiteral(" L6 小弧垂 (m) ");
	_tableWidget->setRowCount(vHeaders.count());
	_tableWidget->setVerticalHeaderLabels(vHeaders);

	QGroupBox *parameterGroupBox = new QGroupBox(QStringLiteral("绝缘子串参数"));   //创建一个 分组框

	QVBoxLayout *boxLayout = new QVBoxLayout;
	boxLayout->addWidget(_tableWidget);
	parameterGroupBox->setLayout(boxLayout);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(_leftAreaLbl);
	mainLayout->addWidget(parameterGroupBox);
	setLayout(mainLayout);
}

InsulatorString_L::~InsulatorString_L()
{

}
