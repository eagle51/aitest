#include "InsulatorLib_Dlg.h"
#include "InsulatorLibWgt_I.h"
#include "InsulatorLibWgt_V.h"
#include "InsulatorLibWgt_L.h"
#include "ClWidget.h"

#include "qgroupbox.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include <QPushButton>
#include <QLabel>

InsulatorLib_Dlg::InsulatorLib_Dlg()
	:_tabWidget(nullptr),
	_insulatorLibWgt_I(nullptr),
	_insulatorLibWgt_V(nullptr),
	_insulatorLibWgt_L(nullptr),
	_insulatorType(InsulatorType::Type_I),
	_voltType(VoltType::AC110),
	_isShowAll(true)
{
	initDialog();
}

// @ function: 构造函数  --- by ljn 2020/1/15 10:25
InsulatorLib_Dlg::InsulatorLib_Dlg(InsulatorType::Type insulatorType,
	VoltType::Type voltType)
	:_tabWidget(nullptr),
	_insulatorLibWgt_I(nullptr),
	_insulatorLibWgt_V(nullptr),
	_insulatorLibWgt_L(nullptr),
	_insulatorType(insulatorType),
	_voltType(voltType),
	_isShowAll(false)
{
	initDialog();
}

InsulatorLib_Dlg::~InsulatorLib_Dlg()
{
	
}

// @ function: 初始化对话框  --- by ljn 2020/1/15 10:57
void InsulatorLib_Dlg::initDialog()
{
	this->setWindowTitle(QStringLiteral("绝缘子串库"));
	QPushButton* _saveBtn = new QPushButton(QStringLiteral(" 保存 "));
	QPushButton *_applyBtn = new QPushButton(QStringLiteral(" 应用 "));
	QPushButton *_cancelBtn = new QPushButton(QStringLiteral(" 退出 "));

	_tabWidget = new QTabWidget(this);
	if (true == _isShowAll)  //如果需要显示所有类型绝缘子串
	{
		_applyBtn->setVisible(false);   //确定按钮设置为不可见，其他是可见的

		_insulatorLibWgt_I = new InsulatorLibWgt_I(_tabWidget);
		_insulatorLibWgt_V = new InsulatorLibWgt_V(_tabWidget);
		_insulatorLibWgt_L = new InsulatorLibWgt_L(_tabWidget);
		_tabWidget->addTab(_insulatorLibWgt_I, QString::fromLocal8Bit(" I型绝缘子串 "));
		_tabWidget->addTab(_insulatorLibWgt_V, QString::fromLocal8Bit(" V型绝缘子串 "));
		_tabWidget->addTab(_insulatorLibWgt_L, QString::fromLocal8Bit(" L型绝缘子串 "));
	}
	else
	{
		//_saveBtn->setVisible(false);//保存按钮设置为不可见，其他是可见的

		switch (_insulatorType)
		{
		case InsulatorType::Type_I:
			_insulatorLibWgt_I = new InsulatorLibWgt_I(_tabWidget);//标签栏上创建选项卡
			_tabWidget->addTab(_insulatorLibWgt_I, QString::fromLocal8Bit(" I型绝缘子串 "));//标签栏上添加选项卡和对应名称
			_insulatorLibWgt_I->setVoltage(_voltType);  //选项卡里设置电压等级
			//_insulatorLibWgt_I->setTableStatus();       //选项卡里 设置表格状态
			break;
		case InsulatorType::Type_V:
			_insulatorLibWgt_V = new InsulatorLibWgt_V(_tabWidget);
			_tabWidget->addTab(_insulatorLibWgt_V, QString::fromLocal8Bit(" V型绝缘子串 "));
			_insulatorLibWgt_V->setVoltage(_voltType);
			//_insulatorLibWgt_V->setTableStatus();
			break;
		case InsulatorType::Type_L:
			_insulatorLibWgt_L = new InsulatorLibWgt_L(_tabWidget);
			_tabWidget->addTab(_insulatorLibWgt_L, QString::fromLocal8Bit(" L型绝缘子串 "));
			_insulatorLibWgt_L->setVoltage(_voltType);
			//_insulatorLibWgt_L->setTableStatus();
			break;
		default:
			break;
		}
	}

	connect(_applyBtn, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(_saveBtn, SIGNAL(clicked()), this, SLOT(slotWriteToJson()));
	connect(_cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

	QHBoxLayout *btnHLyt = new QHBoxLayout();
	btnHLyt->addStretch();
	btnHLyt->addWidget(_applyBtn);
	btnHLyt->addWidget(_saveBtn);
	btnHLyt->addWidget(_cancelBtn);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(_tabWidget);
	mainLayout->addLayout(btnHLyt);
	setLayout(mainLayout);

	setMinimumSize(1200, 700);
}

//@function: 获取当前选择的I串   -- by mzq  2020/1/16 21:25
void InsulatorLib_Dlg::slotApply()
{
	slotWriteToJson();
	accept();
}

void InsulatorLib_Dlg::slotWriteToJson()
{
	if (true == _isShowAll)  //如果需要显示所有类型绝缘子串
	{
		_insulatorLibWgt_I->writeToJson();//I型绝缘子数据写到json文件中
		_insulatorLibWgt_V->writeToJson();//V型绝缘子数据写到json文件中
		_insulatorLibWgt_L->writeToJson();//L型绝缘子数据写到json文件中
	}
	else
	{
		switch (_insulatorType)
		{
		case InsulatorType::Type_I:
			_insulatorLibWgt_I->writeToJson();//I型绝缘子数据写到json文件中
			break;
		case InsulatorType::Type_V:
			_insulatorLibWgt_V->writeToJson();//V型绝缘子数据写到json文件中
			break;
		case InsulatorType::Type_L:
			_insulatorLibWgt_L->writeToJson();//L型绝缘子数据写到json文件中
			break;
		default:
			break;
		}
	}
}

//@function: 获取当前选择的I串   -- by mzq  2020/1/16 21:25
SmartCl::HdlInsulator_I InsulatorLib_Dlg::getSelect_I()
{
	HdlInsulator_I insulator_I = _insulatorLibWgt_I->getCurrentInsulator();//获取当前选择的I串
	return insulator_I;
}

//@function: 获取当前选择的V串   -- by mzq  2020/1/16 21:42
SmartCl::HdlInsulator_V InsulatorLib_Dlg::getSelect_V()
{
	HdlInsulator_V insulator_V = _insulatorLibWgt_V->getCurrentInsulator();//获取当前选择的V串
	return insulator_V;
}

//@function: 获取当前选择的L串   -- by mzq  2020/1/16 21:42
SmartCl::HdlInsulator_L InsulatorLib_Dlg::getSelect_L()
{
	HdlInsulator_L insulator_L = _insulatorLibWgt_L->getCurrentInsulator();//获取当前选择的L串
	return insulator_L;
}