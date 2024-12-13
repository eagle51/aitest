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

WireAvgHLib_Dlg::WireAvgHLib_Dlg(bool isCanEdit/*表格是否可以编辑*/)
	:_isCanEdit(isCanEdit), 
	_selectedVoltType("")
{
	try
	{
		_wireAvgHLib = ClWidget::getCoreInstance()->getWireAvgHLib()->clone();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("绝缘子串库加载"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}
	initDialog();
	readDataToForm();
}

WireAvgHLib_Dlg::~WireAvgHLib_Dlg()
{

}

// @ function: 初始化对话框  --- by mzq 2020/1/15 10:57
void WireAvgHLib_Dlg::initDialog()
{
	this->setWindowTitle(QStringLiteral("导线平均高设置"));

	_tableWidget = new StTableWidget();  //新建列表控件
	QStringList hHeaders;    //字符串列表类  对象（水平表头字符串列表）     
	hHeaders << QStringLiteral(" 电压等级 ") << QStringLiteral(" 导线平均高度 ");//设置水平表头内容
	_tableWidget->setColumnCount(2);   //给列表控件设置列数
	_tableWidget->setHorizontalHeaderLabels(hHeaders);//给列表控件设置 水平标题标签
	_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QLabel *note = new QLabel(QStringLiteral("备注：此处的导线平均高用于导线张力计算！"));

	QPushButton *_okBtn = new QPushButton(QStringLiteral(" 确定 "));
	QPushButton *_saveBtn = new QPushButton(QStringLiteral(" 保存 "));
	QPushButton *_cancelBtn = new QPushButton(QStringLiteral(" 退出 "));

	if (_isCanEdit)
	{
		_okBtn->setVisible(false);
	}
	else
	{
		_saveBtn->setVisible(false);
		_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		_tableWidget->setToolTip(QString::fromLocal8Bit("选择对应的电压等级，\n如果需要更改电压等级对应的导线平居高，\n请在 参数->导线平均高设置 中进行修改"));
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
// @ function: 将数据读入界面中  --- by ljn 2020/1/19 15:34
void WireAvgHLib_Dlg::readDataToForm()
{
	try
	{
		/*初始化表格的行数并存放数据*/
		int rowCount = _wireAvgHLib->getSize();
		_tableWidget->clearContents();          //清空列表
		_tableWidget->setRowCount(rowCount);    //给列表设置行数
		for (unsigned int rowIndex = 0; rowIndex < rowCount; ++rowIndex)
		{
			HdlWireAvgH wireAvgH = _wireAvgHLib->at(rowIndex);
			_tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(QString::fromLocal8Bit(VoltType::typeToStr(wireAvgH->getVoltType()))));
			_tableWidget->setItem(rowIndex, 1, new QTableWidgetItem(QString::number(wireAvgH->getWireAvgH())));
		}
		_tableWidget->setFontPosition(_tableWidget->rowCount(), _tableWidget->columnCount());
		/*设置表格高度*/
		_tableWidget->setFixedHeight(_tableWidget->rowHeight(0) * (_tableWidget->rowCount() +1)+ 2);
		if (_isCanEdit)
			_tableWidget->setCanEditFlag(0, rowCount, 0, 1, false);
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("数据加载"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}
// @ function: 从界面把数据写入数据库  --- by ljn 2020/1/19 15:34
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
		QMessageBox::warning(this, QString::fromLocal8Bit("信息写入"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: 确认所选的行对应的电压等级  --- by ljn 2020/1/19 19:53
void WireAvgHLib_Dlg::slotOk()
{
	if (_tableWidget->selectedItems().size() == 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请选择您需要的行"), QMessageBox::Yes);
	}
	else
	{
		int rowIndex = _tableWidget->currentRow();//获取列表当前行指数
		_selectedVoltType = _tableWidget->item(rowIndex, 0)->text();
		accept();
	}
}

//@function: 设置表格当前选中行   -- by mzq  2020/1/20 14:17
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