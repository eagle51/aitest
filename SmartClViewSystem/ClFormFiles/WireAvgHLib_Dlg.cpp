#include "WireAvgHLib_Dlg.h"

#include "qboxlayout.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include "qmessagebox.h"
#include "EnumLib.h"
#include "qwidget.h"
#include "qobject.h"
#include "qstringlist.h"
#include "ClWidget.h"
#include "qstring.h"
#include "QLabel"

WireAvgHLib_Dlg::WireAvgHLib_Dlg(bool isCanEdit/*����Ƿ���Ա༭*/)
	:_isCanEdit(isCanEdit), 
	_selectedVoltType("")
{
	try
	{
		_wireAvgHLib = ClWidget::getCoreInstance()->getWireAvgHLib()->clone();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}
	initDialog();
	readDataToForm();
}

WireAvgHLib_Dlg::~WireAvgHLib_Dlg()
{

}

// @ function: ��ʼ���Ի���  --- by mzq 2020/1/15 10:57
void WireAvgHLib_Dlg::initDialog()
{
	this->setWindowTitle(QStringLiteral("����ƽ��������"));

	_tableWidget = new StTableWidget();  //�½��б�ؼ�
	QStringList hHeaders;    //�ַ����б���  ����ˮƽ��ͷ�ַ����б�     
	hHeaders << QStringLiteral(" ��ѹ�ȼ� ") << QStringLiteral(" ����ƽ���߶� ");//����ˮƽ��ͷ����
	_tableWidget->setColumnCount(2);   //���б�ؼ���������
	_tableWidget->setHorizontalHeaderLabels(hHeaders);//���б�ؼ����� ˮƽ�����ǩ
	_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QLabel *note = new QLabel(QStringLiteral("��ע���˴��ĵ���ƽ�������ڵ����������㣡"));

	QPushButton *_okBtn = new QPushButton(QStringLiteral(" ȷ�� "));
	QPushButton *_saveBtn = new QPushButton(QStringLiteral(" ���� "));
	QPushButton *_cancelBtn = new QPushButton(QStringLiteral(" �˳� "));

	if (_isCanEdit)
	{
		_okBtn->setVisible(false);
	}
	else
	{
		_saveBtn->setVisible(false);
		_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		_tableWidget->setToolTip(QString::fromLocal8Bit("ѡ���Ӧ�ĵ�ѹ�ȼ���\n�����Ҫ���ĵ�ѹ�ȼ���Ӧ�ĵ���ƽ�Ӹߣ�\n���� ����->����ƽ�������� �н����޸�"));
	}

	connect(_okBtn, SIGNAL(clicked()), this, SLOT(slotOk()));
	connect(_saveBtn, SIGNAL(clicked()), this, SLOT(writeDataFromForm()));
	connect(_cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

	QHBoxLayout *btnHLyt = new QHBoxLayout();
	btnHLyt->addStretch();
	btnHLyt->addWidget(_okBtn);
	btnHLyt->addWidget(_saveBtn);
	btnHLyt->addWidget(_cancelBtn);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(_tableWidget);
	mainLayout->addWidget(note);
	mainLayout->addLayout(btnHLyt);
	setLayout(mainLayout);

	//this->setFixedSize(255, 496);
}
// @ function: �����ݶ��������  --- by ljn 2020/1/19 15:34
void WireAvgHLib_Dlg::readDataToForm()
{
	try
	{
		/*��ʼ�������������������*/
		int rowCount = _wireAvgHLib->getSize();
		_tableWidget->clearContents();          //����б�
		_tableWidget->setRowCount(rowCount);    //���б���������
		for (unsigned int rowIndex = 0; rowIndex < rowCount; ++rowIndex)
		{
			HdlWireAvgH wireAvgH = _wireAvgHLib->at(rowIndex);
			_tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(QString::fromLocal8Bit(VoltType::typeToStr(wireAvgH->getVoltType()))));
			_tableWidget->setItem(rowIndex, 1, new QTableWidgetItem(QString::number(wireAvgH->getWireAvgH())));
		}
		_tableWidget->setFontPosition(_tableWidget->rowCount(), _tableWidget->columnCount());
		/*���ñ��߶�*/
		_tableWidget->setFixedHeight(_tableWidget->rowHeight(0) * (_tableWidget->rowCount() +1)+ 2);
		if (_isCanEdit)
			_tableWidget->setCanEditFlag(0, rowCount, 0, 1, false);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("���ݼ���"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}
// @ function: �ӽ��������д�����ݿ�  --- by ljn 2020/1/19 15:34
void WireAvgHLib_Dlg::writeDataFromForm()
{
	try
	{
		for (unsigned int rowIndex = 0; rowIndex < _tableWidget->rowCount(); ++rowIndex)
		{
			HdlWireAvgH wireAvgH = _wireAvgHLib->at(rowIndex);
			wireAvgH->setVoltType(VoltType::strToType(_tableWidget->item(rowIndex, 0)->text().toLocal8Bit().toStdString().c_str()));
			wireAvgH->setWireAvgH(_tableWidget->item(rowIndex, 1)->text().toDouble());
		}
		ClWidget::getCoreInstance()->setWireAvgHLib(_wireAvgHLib);
		_wireAvgHLib->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ȷ����ѡ���ж�Ӧ�ĵ�ѹ�ȼ�  --- by ljn 2020/1/19 19:53
void WireAvgHLib_Dlg::slotOk()
{
	if (_tableWidget->selectedItems().size() == 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ѡ������Ҫ����"), QMessageBox::Yes);
	}
	else
	{
		int rowIndex = _tableWidget->currentRow();//��ȡ�б�ǰ��ָ��
		_selectedVoltType = _tableWidget->item(rowIndex, 0)->text();
		accept();
	}
}

//@function: ���ñ��ǰѡ����   -- by mzq  2020/1/20 14:17
void WireAvgHLib_Dlg::setCurrentTableRow(const QString& voltTypeQstr)
{
	for (unsigned int rowIndex = 0; rowIndex < _tableWidget->rowCount(); ++rowIndex)
	{
		if (voltTypeQstr == _tableWidget->item(rowIndex, 0)->text())
		{
			_tableWidget->setCurrentCell(rowIndex, 0);
			return;
		}
	}
}