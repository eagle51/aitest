#include "InsulatorLibWgt_L.h"
#include "InsulatorLib_L.h"
#include "EnumLib.h"
#include "DataBase.h"
#include "ClWidget.h"
#include "Mdl_InsultorPara_L.h"

#include "qlabel.h"
#include "qgroupbox.h"
#include "qboxlayout.h"
#include <QHeaderView>
#include "qmenu.h"
#include "qmessagebox.h"
#include "qobject.h"
#include "qwidget.h"
#include "qcursor.h"
#include "qpoint.h"
#include "qcoreapplication.h"
#include <QScrollArea>
#include "qtablewidget.h"
#include "qabstractscrollarea.h"
#include "qheaderview.h"
#include "qlistview.h"
#include "Cl_Stright_L.h"
InsulatorLibWgt_L::InsulatorLibWgt_L(QWidget *parent)
	: QWidget(parent),
	_voltTypeCBx(nullptr),
	_tableWidget(nullptr),
	_splitNumCmbDeleg(nullptr),
	_tableMenu(nullptr),
	_picLbl(nullptr),
	_insulatorLib_L(nullptr),
	_insulatorLib_SingleVolt_L(nullptr)
{
	try
	{
		_insulatorLib_L = ClWidget::getCoreInstance()->getInsulatorLib_L()->clone();
		_insulatorLib_SingleVolt_L = _insulatorLib_L->getInsulatorLib_SingleVolt_L(VoltType::AC110);//Ĭ����ʾ110kv
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}

	initialWgt();
	initTableMenu();
}

InsulatorLibWgt_L::~InsulatorLibWgt_L()
{

}

// @ function:   --- by ljn 2019/12/25 18:49
void InsulatorLibWgt_L::initialWgt()
{
	/*��ѹ�ȼ�*/
	_voltTypeCBx = new QComboBox();
	_voltTypeCBx->setToolTip(QString::fromLocal8Bit("����ѡ���ѹ�ȼ�"));
	for (unsigned int loop = 0; loop < VoltType::getVoltTypeLib().size(); ++loop)
	{
		_voltTypeCBx->addItem(QString::fromLocal8Bit(VoltType::typeToStr(VoltType::getVoltTypeLib().at(loop))));
	}
	_voltTypeCBx->setView(new QListView());
	QVBoxLayout *voltTypeLayout = new QVBoxLayout;
	voltTypeLayout->addWidget(_voltTypeCBx);
	QGroupBox *voltTypeGbx = new QGroupBox(QStringLiteral("��ѹ�ȼ�"));
	voltTypeGbx->setLayout(voltTypeLayout);
	connect(_voltTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangeVolt(int)));

	_tableWidget = new QTableWidget();
	//��߱�ͷ����
	QStringList vHeaders;
	vHeaders << QStringLiteral(" ��Ե�Ӵ��ͺ� ")
		<< QStringLiteral(" L����������b֫���� (N) ")
		<< QStringLiteral(" L����������a֫���� (N) ")
		<< QStringLiteral(" a֫��ֱ�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" b֫��ֱ�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" a֫������Ե�Ӵ�Ƭ�� (Ƭ) ")
		<< QStringLiteral(" b֫������Ե�Ӵ�Ƭ�� (Ƭ) ")
		<< QStringLiteral(" ��Ƭ��Ե���ܷ���� (m^2) ")
		<< QStringLiteral(" �ش����� (N) ")
		<< QStringLiteral(" ������ ")
		<< QStringLiteral(" ���Ѽ�� (m) ")
		<< QStringLiteral(" L1_a a֫��֫���� (m) ")
		<< QStringLiteral(" L1_b b֫��֫���� (m) ")
		<< QStringLiteral(" L2_a a֫��ѹ���߶� (m) ")
		<< QStringLiteral(" L2_b b֫��ѹ���߶� (m) ")
		<< QStringLiteral(" L3_a a֫��ѹ����� (m) ")
		<< QStringLiteral(" L3_b b֫��ѹ����� (m) ")
		<< QStringLiteral(" L4 ������ (m) ")
		<< QStringLiteral(" L5 �߼йҿ�������ҿ״��� (m) ")
		<< QStringLiteral(" L6 �ӵ����߼г��� (m) ")
		<< QStringLiteral(" L7 С���� (m) ")
		<< QStringLiteral(" �� �߼м�С����ƫ�� (��) ");
	_tableWidget->setRowCount(vHeaders.count());
	_tableWidget->setRowHeight(0, 60);
	_tableWidget->setVerticalHeaderLabels(vHeaders);

	_splitNumCmbDeleg = new ComboDelegate();
	_tableWidget->setItemDelegateForRow(9, _splitNumCmbDeleg);

	connect(_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotChangePic()));
	connect(_splitNumCmbDeleg, SIGNAL(valueChanged()), this, SLOT(slotChangePic()));

	/*��Ե�Ӵ������߼��β���*/
	QStringList SplitNumTypeList;
	for (SmartCl::SplitNum::Type type = SmartCl::SplitNum::One; type <= SmartCl::SplitNum::Ten; type = (SmartCl::SplitNum::Type)(type + 1))
		SplitNumTypeList << QString::fromLocal8Bit(SmartCl::SplitNum::typeToStr(type));
	_splitNumCmbDeleg->setSList(SplitNumTypeList);

	QGroupBox *parameterGroupBox = new QGroupBox(QStringLiteral("��Ե�Ӵ�����"));
	QVBoxLayout *boxLayout = new QVBoxLayout;
	boxLayout->addWidget(voltTypeGbx);
	boxLayout->addWidget(_tableWidget);
	parameterGroupBox->setLayout(boxLayout);

	_picLbl = new QLabel();
	_picLbl->setFixedSize(500, 357);
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(parameterGroupBox);
	mainLayout->addWidget(_picLbl);
	setLayout(mainLayout);

	readDataToForm();
	slotChangePic();
}

// @ function: �ӽ��������д�����ݿ�  --- by ljn 2019/12/26 19:35
void InsulatorLibWgt_L::writeDataFromForm()
{
	try
	{
		for (unsigned int columnIndex = 0; columnIndex < _tableWidget->columnCount(); ++columnIndex)
		{
			HdlInsulator_L insulator_L = _insulatorLib_SingleVolt_L->at(columnIndex);
			writeOneInsulator_L(insulator_L, true, columnIndex);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: д��һ������   -- by mzq 2023/7/29 15:26
void InsulatorLibWgt_L::writeOneInsulator_L(HdlInsulator_L insulator_L, bool writeSpecial/*�Ƿ�д���߼м�С������Ϣ*/, int index)
{
	if (insulator_L.isNull())
		return;

	HdlMdl_InsultorPara_L insultorPara_L = insulator_L->getInsultorPara_L();
	insultorPara_L->setInsultorName(_tableWidget->item(0, index)->text().toLocal8Bit().toStdString());
	insultorPara_L->setInsultorWeight_a(_tableWidget->item(1, index)->text().toDouble());
	insultorPara_L->setInsultorWeight_b(_tableWidget->item(2, index)->text().toDouble());
	insultorPara_L->setNumSubs_a_V(_tableWidget->item(3, index)->text().toInt());
	insultorPara_L->setNumSubs_b_V(_tableWidget->item(4, index)->text().toInt());
	insultorPara_L->setNumPlate_a(_tableWidget->item(5, index)->text().toInt());
	insultorPara_L->setNumPlate_b(_tableWidget->item(6, index)->text().toInt());
	insultorPara_L->setShieldArea(_tableWidget->item(7, index)->text().toDouble());
	insultorPara_L->setHammerWeight(_tableWidget->item(8, index)->text().toDouble());

	HdlMdl_InsulaGeoPara_L insulaGeoPara_L = insulator_L->getInsulaGeoPara_L();
	insulaGeoPara_L->setSplitNum(SplitNum::Type(SplitNum::strToType(_tableWidget->item(9, index)->text().toLocal8Bit().toStdString())));
	insulaGeoPara_L->setSplitShow(_tableWidget->item(10, index)->text().toLocal8Bit().toStdString());
	insulaGeoPara_L->setL1_a(_tableWidget->item(11, index)->text().toDouble());
	insulaGeoPara_L->setL1_b(_tableWidget->item(12, index)->text().toDouble());
	insulaGeoPara_L->setL2_a(_tableWidget->item(13, index)->text().toDouble());
	insulaGeoPara_L->setL2_b(_tableWidget->item(14, index)->text().toDouble());
	insulaGeoPara_L->setL3_a(_tableWidget->item(15, index)->text().toDouble());
	insulaGeoPara_L->setL3_b(_tableWidget->item(16, index)->text().toDouble());
	insulaGeoPara_L->setL4(_tableWidget->item(17, index)->text().toDouble());
	insulaGeoPara_L->setL5(_tableWidget->item(18, index)->text().toDouble());
	insulaGeoPara_L->setL6(_tableWidget->item(19, index)->text().toDouble());

	if (writeSpecial)
	{
		insulaGeoPara_L->setL7(_tableWidget->item(20, index)->text().toDouble());
		insulaGeoPara_L->setTheta(_tableWidget->item(21, index)->text().toDouble());
	}
}

// @ function: �����ݶ��������  --- by ljn 2019/12/26 19:37
void InsulatorLibWgt_L::readDataToForm()
{
	try
	{
		/*��ʼ�������������������*/
		int columnCount = _insulatorLib_SingleVolt_L->getSize();
		_tableWidget->clearContents();
		_tableWidget->setColumnCount(columnCount);

		for (unsigned int columnIndex = 0; columnIndex < columnCount; ++columnIndex)
		{
			/*��Ե�Ӵ�ˢ�½�������*/
			HdlInsulator_L insulator_L = _insulatorLib_SingleVolt_L->at(columnIndex);

			HdlMdl_InsultorPara_L insultorPara_L = insulator_L->getInsultorPara_L();
			_tableWidget->setItem(0, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insultorPara_L->getInsultorName().c_str())));
			_tableWidget->setItem(1, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getInsultorWeight_a())));
			_tableWidget->setItem(2, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getInsultorWeight_b())));
			_tableWidget->setItem(3, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getNumSubs_a_V())));
			_tableWidget->setItem(4, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getNumSubs_b_V())));
			_tableWidget->setItem(5, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getNumPlate_a())));
			_tableWidget->setItem(6, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getNumPlate_b())));
			_tableWidget->setItem(7, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getShieldArea())));
			_tableWidget->setItem(8, columnIndex, new QTableWidgetItem(QString::number(insultorPara_L->getHammerWeight())));

			HdlMdl_InsulaGeoPara_L insulaGeoPara_L = insulator_L->getInsulaGeoPara_L();
			_tableWidget->setItem(9, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(SplitNum::typeToStr(insulaGeoPara_L->getSplitNum()))));
			_tableWidget->setItem(10, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insulaGeoPara_L->getSplitShow().c_str())));
			_tableWidget->setItem(11, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL1_a())));
			_tableWidget->setItem(12, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL1_b())));
			_tableWidget->setItem(13, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL2_a())));
			_tableWidget->setItem(14, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL2_b())));
			_tableWidget->setItem(15, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL3_a())));
			_tableWidget->setItem(16, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL3_b())));
			_tableWidget->setItem(17, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL4())));
			_tableWidget->setItem(18, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL5())));
			_tableWidget->setItem(19, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL6())));
			_tableWidget->setItem(20, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getL7())));
			_tableWidget->setItem(21, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_L->getTheta())));
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ�L�����ݼ���"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ��ʼ������һ��˵�  --- by ljn 2019/12/25 18:55
void InsulatorLibWgt_L::initTableMenu()
{
	_tableMenu = new QMenu(_tableWidget);
	QAction *addAction = new QAction(_tableMenu);
	QAction *deleteAction = new QAction(_tableMenu);
	QAction *applyAction = new QAction(_tableMenu);

	addAction->setText(QStringLiteral("������"));
	deleteAction->setText(QStringLiteral("ɾ����"));
	applyAction->setText(QStringLiteral("����ֱ����I����϶Բ������"));

	_tableMenu->addAction(addAction);
	_tableMenu->addAction(deleteAction);
	_tableMenu->addAction(applyAction);
	_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(_tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotTableMenu(QPoint)));
	connect(addAction, SIGNAL(triggered()), this, SLOT(slotAddColumn()));
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(slotDeletColumn()));
	connect(applyAction, SIGNAL(triggered()), this, SLOT(slotApplyToProject_L()));
}

// @ function: ���Ҽ��˵�  --- by ljn 2019/12/25 19:55
void InsulatorLibWgt_L::slotTableMenu(QPoint pos)
{
	_tableMenu->exec(QCursor::pos());
}


// @ function: ������  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_L::slotAddColumn()
{
	try
	{
		writeDataFromForm();
		int columnIndex = _tableWidget->currentColumn();
		HdlInsulator_L curInsulator = new Insulator_L();
		while (_insulatorLib_SingleVolt_L->has(curInsulator->getInsultorPara_L()->getInsultorName()))
		{
			std::string newName = curInsulator->getInsultorPara_L()->getInsultorName() + "_����";
			curInsulator->getInsultorPara_L()->setInsultorName(newName);
		}
		_insulatorLib_SingleVolt_L->insert(curInsulator, columnIndex + 1);
		readDataToForm();
		_tableWidget->setCurrentCell(0, columnIndex + 1);    //���õ�ǰѡ��Ϊ��ǰ�е���һ��
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ɾ����  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_L::slotDeletColumn()
{
	int select = QMessageBox::warning(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�Ƿ�ɾ����"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		try
		{
			writeDataFromForm();
			int columnIndex = _tableWidget->currentColumn();
			_insulatorLib_SingleVolt_L->del(columnIndex);
			readDataToForm();
			_tableWidget->setCurrentCell(0, 0);//���õ�ǰѡ��Ϊ��һ��
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("ɾ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		}
	}
}

//@function: ����ֱ����L����϶Բ������   -- by mzq 2023/7/29 15:26
void InsulatorLibWgt_L::slotApplyToProject_L()
{
	int select = QMessageBox::warning(this,
									  QString::fromLocal8Bit("��ʾ"),
									  QString::fromLocal8Bit("�Ƿ񽫵�ǰ��Ե�Ӵ����������������̵����С�ֱ����L������϶Բ�е�ͬ����Ե�Ӵ��У�"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		writeDataFromForm();     //�ӽ��������д�����ݿ�

		bool isUpdateTheta = true;
		int select = QMessageBox::warning(this,
										  QString::fromLocal8Bit("��ʾ"),
										  QString::fromLocal8Bit("�Ƿ�ͬ�����¡�С�������Լ����߼м�С����ƫ�ǡ���"),
										  QMessageBox::Yes | QMessageBox::No);
		if (select == QMessageBox::Yes)
			isUpdateTheta = true;
		else
			isUpdateTheta = false;

		QString showText = "";
		for (unsigned int loop = 0; loop < ClWidget::getCoreInstance()->getClCount(); ++loop)
		{
			HdlCl cl = ClWidget::getCoreInstance()->getClAt(loop);
			if (cl->getClType() == ClCate::StraightIStr)
			{
				HdlCl_Stright_L stright_L = dynamic_cast<Cl_Stright_L*>(cl.GetPointee());
				HdlMdl_InsultorPara_L insultorPara_L = stright_L->getDataUI_L()->getInsultorPara_L();
				HdlMdl_InsulaGeoPara_L insulaGeoPara_L = stright_L->getDataUI_L()->getInsulaGeoPara_L();

				if (insultorPara_L->getInsulatorType() == InsulatorType::Type_L
					&& insultorPara_L->getInsultorName() == _tableWidget->item(0, _tableWidget->currentColumn())->text().toLocal8Bit().toStdString())
				{
					HdlInsulator_L insulator_L = new Insulator_L();
					insulator_L->setInsultorPara_L(insultorPara_L);
					insulator_L->setInsulaGeoPara_L(insulaGeoPara_L);
					writeOneInsulator_L(insulator_L, isUpdateTheta, _tableWidget->currentColumn());

					showText += QString::fromLocal8Bit(cl->getClName().c_str());
					showText += "\n";
				}
			}
		}
		QMessageBox::warning(this,
							 QString::fromLocal8Bit("��ʾ"),
							 QString::fromLocal8Bit("���Ƹ���������ֱ����L����϶Բ�еľ�Ե�Ӵ�����\n\n") + showText,
							 QMessageBox::Yes);

	}
}

// @ function: ���Ľ���ʾ��ͼ  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_L::slotChangePic()
{
	if (_tableWidget->columnCount() != 0)
	{
		int columnIndex = _tableWidget->currentColumn();
		-1 == columnIndex ? columnIndex = 0 : columnIndex;//��������޵�ǰѡ���У���Ĭ����ʾ��һ�ж�Ӧ��������ͼƬ
		SplitNum::Type splitNumType = SplitNum::Type(SplitNum::strToType(_tableWidget->item(9, columnIndex)->text().toLocal8Bit().toStdString()));

		QPixmap pixmap;
		switch (splitNumType)
		{
		case SmartCl::SplitNum::One:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/noneSplit.png"));
			break;
		case SmartCl::SplitNum::TwoV:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/twoSplitVer.png"));
			break;
		case SmartCl::SplitNum::TwoH:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/twoSplitHor.png"));
			break;
		case SmartCl::SplitNum::Three:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/threeSplit.png"));
			break;
		case SmartCl::SplitNum::Four:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/fourSplit.png"));
			break;
		case SmartCl::SplitNum::Six:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/sixSplit.png"));
			break;
		case SmartCl::SplitNum::Eight:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/eightSplit.png"));
			break;
		case SmartCl::SplitNum::Ten:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitLNum/tenSplit.png"));
			break;
		default:
			break;
		}
		pixmap.scaled(_picLbl->size(), Qt::KeepAspectRatio);
		_picLbl->setScaledContents(true);
		_picLbl->setPixmap(pixmap);
	}
}
//@function: �ı��ѹ�ȼ�   -- by mzq  2020/1/10 20:58
void InsulatorLibWgt_L::slotChangeVolt(int index)
{
	try
	{
		writeDataFromForm();
		_insulatorLib_SingleVolt_L = _insulatorLib_L->getInsulatorLib_SingleVolt_L(VoltType::Type(index));
		readDataToForm();
		slotChangePic();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("�л���ѹ�ȼ�"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ���浽json�ļ�  --- by ljn 2020/1/3 16:51
void InsulatorLibWgt_L::writeToJson()
{
	try
	{
		writeDataFromForm();
		if ("" == _insulatorLib_SingleVolt_L->hasRepeat())
		{
			ClWidget::getCoreInstance()->setInsulatorLib_L(_insulatorLib_L);
			_insulatorLib_L->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());
		}
		else
		{
			std::string name = _insulatorLib_SingleVolt_L->hasRepeat();
			QMessageBox::warning(this, QString::fromLocal8Bit("����ʧ��"),
				QString::fromLocal8Bit("�����ظ��ͺţ�")
				+ QString::fromLocal8Bit(name.c_str())
				+ QString::fromLocal8Bit("\n��˶��޸ĺ��ٽ��б��棡"),
				QMessageBox::Yes);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ���ñ��༭״̬  --- by ljn 2020/1/16 11:22
void InsulatorLibWgt_L::setTableStatus()
{
	_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_tableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);//����ѡ��ģʽΪѡ����
	_tableWidget->setContextMenuPolicy(Qt::NoContextMenu);//���ñ��˵����ɼ�
}

//@function: ��ȡ��ǰ��Ե�Ӵ�   -- by mzq  2020/1/16 21:38
SmartCl::HdlInsulator_L InsulatorLibWgt_L::getCurrentInsulator()
{
	int columnIndex = _tableWidget->currentColumn();
	if (-1 != columnIndex)
		return _insulatorLib_SingleVolt_L->at(columnIndex);
	else
		return nullptr;
}

//@function: ���õ�ѹ�ȼ�   -- by mzq  2020/1/16 21:58
void InsulatorLibWgt_L::setVoltage(VoltType::Type voltType)
{
	_voltTypeCBx->setCurrentIndex(voltType);
}