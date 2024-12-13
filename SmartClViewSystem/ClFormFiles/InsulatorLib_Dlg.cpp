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

// @ function: ���캯��  --- by ljn 2020/1/15 10:25
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

// @ function: ��ʼ���Ի���  --- by ljn 2020/1/15 10:57
void InsulatorLib_Dlg::initDialog()
{
	this->setWindowTitle(QStringLiteral("��Ե�Ӵ���"));
	QPushButton* _saveBtn = new QPushButton(QStringLiteral(" ���� "));
	QPushButton *_applyBtn = new QPushButton(QStringLiteral(" Ӧ�� "));
	QPushButton *_cancelBtn = new QPushButton(QStringLiteral(" �˳� "));

	_tabWidget = new QTabWidget(this);
	if (true == _isShowAll)  //�����Ҫ��ʾ�������;�Ե�Ӵ�
	{
		_applyBtn->setVisible(false);   //ȷ����ť����Ϊ���ɼ��������ǿɼ���

		_insulatorLibWgt_I = new InsulatorLibWgt_I(_tabWidget);
		_insulatorLibWgt_V = new InsulatorLibWgt_V(_tabWidget);
		_insulatorLibWgt_L = new InsulatorLibWgt_L(_tabWidget);
		_tabWidget->addTab(_insulatorLibWgt_I, QString::fromLocal8Bit(" I�;�Ե�Ӵ� "));
		_tabWidget->addTab(_insulatorLibWgt_V, QString::fromLocal8Bit(" V�;�Ե�Ӵ� "));
		_tabWidget->addTab(_insulatorLibWgt_L, QString::fromLocal8Bit(" L�;�Ե�Ӵ� "));
	}
	else
	{
		//_saveBtn->setVisible(false);//���水ť����Ϊ���ɼ��������ǿɼ���

		switch (_insulatorType)
		{
		case InsulatorType::Type_I:
			_insulatorLibWgt_I = new InsulatorLibWgt_I(_tabWidget);//��ǩ���ϴ���ѡ�
			_tabWidget->addTab(_insulatorLibWgt_I, QString::fromLocal8Bit(" I�;�Ե�Ӵ� "));//��ǩ�������ѡ��Ͷ�Ӧ����
			_insulatorLibWgt_I->setVoltage(_voltType);  //ѡ������õ�ѹ�ȼ�
			//_insulatorLibWgt_I->setTableStatus();       //ѡ��� ���ñ��״̬
			break;
		case InsulatorType::Type_V:
			_insulatorLibWgt_V = new InsulatorLibWgt_V(_tabWidget);
			_tabWidget->addTab(_insulatorLibWgt_V, QString::fromLocal8Bit(" V�;�Ե�Ӵ� "));
			_insulatorLibWgt_V->setVoltage(_voltType);
			//_insulatorLibWgt_V->setTableStatus();
			break;
		case InsulatorType::Type_L:
			_insulatorLibWgt_L = new InsulatorLibWgt_L(_tabWidget);
			_tabWidget->addTab(_insulatorLibWgt_L, QString::fromLocal8Bit(" L�;�Ե�Ӵ� "));
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

//@function: ��ȡ��ǰѡ���I��   -- by mzq  2020/1/16 21:25
void InsulatorLib_Dlg::slotApply()
{
	slotWriteToJson();
	accept();
}

void InsulatorLib_Dlg::slotWriteToJson()
{
	if (true == _isShowAll)  //�����Ҫ��ʾ�������;�Ե�Ӵ�
	{
		_insulatorLibWgt_I->writeToJson();//I�;�Ե������д��json�ļ���
		_insulatorLibWgt_V->writeToJson();//V�;�Ե������д��json�ļ���
		_insulatorLibWgt_L->writeToJson();//L�;�Ե������д��json�ļ���
	}
	else
	{
		switch (_insulatorType)
		{
		case InsulatorType::Type_I:
			_insulatorLibWgt_I->writeToJson();//I�;�Ե������д��json�ļ���
			break;
		case InsulatorType::Type_V:
			_insulatorLibWgt_V->writeToJson();//V�;�Ե������д��json�ļ���
			break;
		case InsulatorType::Type_L:
			_insulatorLibWgt_L->writeToJson();//L�;�Ե������д��json�ļ���
			break;
		default:
			break;
		}
	}
}

//@function: ��ȡ��ǰѡ���I��   -- by mzq  2020/1/16 21:25
SmartCl::HdlInsulator_I InsulatorLib_Dlg::getSelect_I()
{
	HdlInsulator_I insulator_I = _insulatorLibWgt_I->getCurrentInsulator();//��ȡ��ǰѡ���I��
	return insulator_I;
}

//@function: ��ȡ��ǰѡ���V��   -- by mzq  2020/1/16 21:42
SmartCl::HdlInsulator_V InsulatorLib_Dlg::getSelect_V()
{
	HdlInsulator_V insulator_V = _insulatorLibWgt_V->getCurrentInsulator();//��ȡ��ǰѡ���V��
	return insulator_V;
}

//@function: ��ȡ��ǰѡ���L��   -- by mzq  2020/1/16 21:42
SmartCl::HdlInsulator_L InsulatorLib_Dlg::getSelect_L()
{
	HdlInsulator_L insulator_L = _insulatorLibWgt_L->getCurrentInsulator();//��ȡ��ǰѡ���L��
	return insulator_L;
}