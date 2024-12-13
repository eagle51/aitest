#include "insulatorstring_l.h"
#include "qlabel.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include <QHeaderView>
InsulatorString_L::InsulatorString_L(QFileInfo fileInfo, QWidget *parent)
	 : _fileInfo(fileInfo), QWidget(parent)
{
	QLabel *_leftAreaLbl = new QLabel(QString::fromLocal8Bit(" ��Ҫ�����ͼƬ�������������"));

	_tableWidget = new QTableWidget(10, 10);
	_tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	_tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	_tableWidget->setStyleSheet("QTableView::Item{background-color:#FF3EFF} ");
	_tableWidget->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ
	_tableWidget->setShowGrid(true);
	//��߱�ͷ����
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ��Ե�Ӵ��ͺ� ")
		<< QStringLiteral(" ��Ե�Ӵ������� (N) ")
		<< QStringLiteral(" ��ֱ�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" ˳�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" ������Ե�Ӵ�Ƭ�� (Ƭ) ")
		<< QStringLiteral(" ��Ƭ��Ե���ܷ���� (m^2) ")
		<< QStringLiteral(" �ش����� (N) ")
		<< QStringLiteral(" L0 ��Ե�Ӵ����� (m) ")
		<< QStringLiteral(" ������ ")
		<< QStringLiteral(" ���Ѽ�� (m) ")
		<< QStringLiteral(" L1 �����ҵ�-��ߵ�һת���� (m) ")
		<< QStringLiteral(" L2 �����ҵ�-���ӵ����߼йҵ� (m) ")
		<< QStringLiteral(" L3 ���ӵ����߼йҵ�-��ѹ�� (m) ")
		<< QStringLiteral(" L4 ��ѹ����� (m) ")
		<< QStringLiteral(" L5 �ӵ����߼г��� (m) ")
		<< QStringLiteral(" L6 С���� (m) ");
	_tableWidget->setRowCount(vHeaders.count());
	_tableWidget->setVerticalHeaderLabels(vHeaders);

	QGroupBox *parameterGroupBox = new QGroupBox(QStringLiteral("��Ե�Ӵ�����"));   //����һ�� �����

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
