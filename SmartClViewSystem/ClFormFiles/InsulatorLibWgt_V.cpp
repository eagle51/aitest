#include "InsulatorLibWgt_V.h"
#include "InsulatorLib_V.h"
#include "EnumLib.h"
#include "DataBase.h"
#include "ClWidget.h"
#include "Mdl_InsultorPara_V.h"

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
#include "Cl_Stright_V.h"

InsulatorLibWgt_V::InsulatorLibWgt_V(QWidget *parent)
	: QWidget(parent),
	_voltTypeCBx(nullptr),
	_tableWidget(nullptr),
	_splitNumCmbDeleg(nullptr),
	_tableMenu(nullptr),
	_picLbl(nullptr),
	_insulatorLib_V(nullptr),
	_insulatorLib_SingleVolt_V(nullptr)
{
	try
	{
		_insulatorLib_V = ClWidget::getCoreInstance()->getInsulatorLib_V()->clone();
		_insulatorLib_SingleVolt_V = _insulatorLib_V->getInsulatorLib_SingleVolt_V(VoltType::AC110);//Ĭ����ʾ110kv
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}

	initialWgt();
	initTableMenu();
}

InsulatorLibWgt_V::~InsulatorLibWgt_V()
{

}

void InsulatorLibWgt_V::initialWgt()
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
		<< QStringLiteral(" V��������������֫���� (N) ")
		<< QStringLiteral(" ��֫��ֱ�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" ������Ե�Ӵ�Ƭ�� (Ƭ) ")
		<< QStringLiteral(" ��Ƭ��Ե���ܷ���� (m^2) ")
		<< QStringLiteral(" �ش����� (N) ")
		<< QStringLiteral(" ������ ")
		<< QStringLiteral(" ���Ѽ�� (m) ")
		<< QStringLiteral(" L1 V����֫���� (m) ")
		<< QStringLiteral(" L2 ��ѹ���߶� (m) ")
		<< QStringLiteral(" L3 ��ѹ����� (m) ")
		<< QStringLiteral(" L4 ������ (m) ")
		<< QStringLiteral(" L5 �߼йҿ�������ҿ״��� (m) ")
		<< QStringLiteral(" L6 �ӵ����߼г��� (m) ")
		<< QStringLiteral(" L7 С���� (m) ")
		<< QStringLiteral(" �� �߼м�С����ƫ�� (��) ");	
	_tableWidget->setRowCount(vHeaders.count());
	_tableWidget->setRowHeight(0, 60);
	_tableWidget->setVerticalHeaderLabels(vHeaders);

	_splitNumCmbDeleg = new ComboDelegate();
	_tableWidget->setItemDelegateForRow(6, _splitNumCmbDeleg);

	connect(_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotChangePic()));
	connect(_splitNumCmbDeleg, SIGNAL(valueChanged()), this, SLOT(slotChangePic()));

	/*��Ե�Ӵ������߼��β���*/
	QStringList SplitNumTypeList;
	for (SmartCl::SplitNum::Type type = SmartCl::SplitNum::One; type <= SmartCl::SplitNum::Ten; type = (SmartCl::SplitNum::Type)(type + 1))
		SplitNumTypeList << QString::fromLocal8Bit(SmartCl::SplitNum::typeToStr(type));
	_splitNumCmbDeleg->setSList(SplitNumTypeList);


	QGroupBox *parameterGroupBox = new QGroupBox(QStringLiteral("��Ե�Ӵ�����"));   //����һ�� �����
	QVBoxLayout *boxLayout = new QVBoxLayout;
	boxLayout->addWidget(voltTypeGbx);
	boxLayout->addWidget(_tableWidget);
	parameterGroupBox->setLayout(boxLayout);

	_picLbl = new QLabel();
	_picLbl->setFixedSize(500, 442);
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(parameterGroupBox);
	mainLayout->addWidget(_picLbl);
	setLayout(mainLayout);

	readDataToForm();
	slotChangePic();
}

// @ function: �ӽ��������д�����ݿ�  --- by ljn 2019/12/26 19:35
void InsulatorLibWgt_V::writeDataFromForm()
{
	try
	{
		for (unsigned int columnIndex = 0; columnIndex < _tableWidget->columnCount(); ++columnIndex)
		{
			HdlInsulator_V insulator_V = _insulatorLib_SingleVolt_V->at(columnIndex);
			writeOneInsulator_V(insulator_V, true, columnIndex);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: д��һ������   -- by mzq 2023/7/29 15:20
void InsulatorLibWgt_V::writeOneInsulator_V(HdlInsulator_V insulator_V, bool writeSpecial/*�Ƿ�д���߼м�С������Ϣ*/, int index)
{
	if (insulator_V.isNull())
		return;

	HdlMdl_InsultorPara_V insultorPara_V = insulator_V->getInsultorPara_V();
	insultorPara_V->setInsultorName(_tableWidget->item(0, index)->text().toLocal8Bit().toStdString());
	insultorPara_V->setInsultorWeight(_tableWidget->item(1, index)->text().toDouble());
	insultorPara_V->setSglNumSubs_V(_tableWidget->item(2, index)->text().toInt());
	insultorPara_V->setNumPlate(_tableWidget->item(3, index)->text().toInt());
	insultorPara_V->setShieldArea(_tableWidget->item(4, index)->text().toDouble());
	insultorPara_V->setHammerWeight(_tableWidget->item(5, index)->text().toDouble());

	HdlMdl_InsulaGeoPara_V insulaGeoPara_V = insulator_V->getInsulaGeoPara_V();
	insulaGeoPara_V->setSplitNum(SplitNum::Type(SplitNum::strToType(_tableWidget->item(6, index)->text().toLocal8Bit().toStdString())));
	insulaGeoPara_V->setSplitShow(_tableWidget->item(7, index)->text().toLocal8Bit().toStdString());
	insulaGeoPara_V->setL1(_tableWidget->item(8, index)->text().toDouble());
	insulaGeoPara_V->setL2(_tableWidget->item(9, index)->text().toDouble());
	insulaGeoPara_V->setL3(_tableWidget->item(10, index)->text().toDouble());
	insulaGeoPara_V->setL4(_tableWidget->item(11, index)->text().toDouble());
	insulaGeoPara_V->setL5(_tableWidget->item(12, index)->text().toDouble());
	insulaGeoPara_V->setL6(_tableWidget->item(13, index)->text().toDouble());
	if (writeSpecial)
	{
		insulaGeoPara_V->setL7(_tableWidget->item(14, index)->text().toDouble());
		insulaGeoPara_V->setTheta(_tableWidget->item(15, index)->text().toDouble());
	}
}

// @ function: �����ݶ��������  --- by ljn 2019/12/26 19:37
void InsulatorLibWgt_V::readDataToForm()
{
	try
	{
		/*��ʼ�������������������*/
		int columnCount = _insulatorLib_SingleVolt_V->getSize();
		_tableWidget->clearContents();
		_tableWidget->setColumnCount(columnCount);

		for (unsigned int columnIndex = 0; columnIndex < columnCount; ++columnIndex)
		{
			/*��Ե�Ӵ�ˢ�½�������*/
			HdlInsulator_V insulator_V = _insulatorLib_SingleVolt_V->at(columnIndex);

			HdlMdl_InsultorPara_V insultorPara_V = insulator_V->getInsultorPara_V();
			_tableWidget->setItem(0, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insultorPara_V->getInsultorName().c_str())));
			_tableWidget->setItem(1, columnIndex, new QTableWidgetItem(QString::number(insultorPara_V->getInsultorWeight())));
			_tableWidget->setItem(2, columnIndex, new QTableWidgetItem(QString::number(insultorPara_V->getSglNumSubs_V())));
			_tableWidget->setItem(3, columnIndex, new QTableWidgetItem(QString::number(insultorPara_V->getNumPlate())));
			_tableWidget->setItem(4, columnIndex, new QTableWidgetItem(QString::number(insultorPara_V->getShieldArea())));
			_tableWidget->setItem(5, columnIndex, new QTableWidgetItem(QString::number(insultorPara_V->getHammerWeight())));

			HdlMdl_InsulaGeoPara_V insulaGeoPara_V = insulator_V->getInsulaGeoPara_V();
			_tableWidget->setItem(6, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(SplitNum::typeToStr(insulaGeoPara_V->getSplitNum()))));
			_tableWidget->setItem(7, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insulaGeoPara_V->getSplitShow().c_str())));
			_tableWidget->setItem(8, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_V->getL1())));
			_tableWidget->setItem(9, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_V->getL2())));
			_tableWidget->setItem(10, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_V->getL3())));
			_tableWidget->setItem(11, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_V->getL4())));
			_tableWidget->setItem(12, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_V->getL5())));
			_tableWidget->setItem(13, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_V->getL6())));
			_tableWidget->setItem(14, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_V->getL7())));
			_tableWidget->setItem(15, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_V->getTheta())));
		}
		
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ�V�����ݼ���"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ��ʼ������һ��˵�  --- by ljn 2019/12/25 18:55
void InsulatorLibWgt_V::initTableMenu()
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
	connect(applyAction, SIGNAL(triggered()), this, SLOT(slotApplyToProject_V()));
}

// @ function: ���Ҽ��˵�  --- by ljn 2019/12/25 19:55
void InsulatorLibWgt_V::slotTableMenu(QPoint pos)
{
	_tableMenu->exec(QCursor::pos());
}

// @ function: ������  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_V::slotAddColumn()
{
	try
	{
		writeDataFromForm();
		int columnIndex = _tableWidget->currentColumn();
		HdlInsulator_V curInsulator = new Insulator_V();
		while (_insulatorLib_SingleVolt_V->has(curInsulator->getInsultorPara_V()->getInsultorName()))
		{
			std::string newName = curInsulator->getInsultorPara_V()->getInsultorName() + "_����";
			curInsulator->getInsultorPara_V()->setInsultorName(newName);
		}
		_insulatorLib_SingleVolt_V->insert(curInsulator, columnIndex + 1);
		readDataToForm();
		_tableWidget->setCurrentCell(0, columnIndex + 1);    //���õ�ǰѡ��Ϊ��ǰ�е���һ��
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ɾ����  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_V::slotDeletColumn()
{
	int select = QMessageBox::warning(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�Ƿ�ɾ����"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		try
		{
			writeDataFromForm();
			int columnIndex = _tableWidget->currentColumn();
			_insulatorLib_SingleVolt_V->del(columnIndex);
			readDataToForm();
			_tableWidget->setCurrentCell(0, 0);//���õ�ǰѡ��Ϊ��һ��
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("ɾ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		}
	}
}

//@function: ����ֱ����V����϶Բ������   -- by mzq 2023/7/29 15:20
void InsulatorLibWgt_V::slotApplyToProject_V()
{
	int select = QMessageBox::warning(this,
									  QString::fromLocal8Bit("��ʾ"),
									  QString::fromLocal8Bit("�Ƿ񽫵�ǰ��Ե�Ӵ����������������̵����С�ֱ����V������϶Բ�е�ͬ����Ե�Ӵ��У�"), QMessageBox::Yes | QMessageBox::No);
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
				HdlCl_Stright_V stright_V = dynamic_cast<Cl_Stright_V*>(cl.GetPointee());
				HdlMdl_InsultorPara_V insultorPara_V = stright_V->getDataUI_V()->getInsultorPara_V();
				HdlMdl_InsulaGeoPara_V insulaGeoPara_V = stright_V->getDataUI_V()->getInsulaGeoPara_V();

				if (insultorPara_V->getInsulatorType() == InsulatorType::Type_V
					&& insultorPara_V->getInsultorName() == _tableWidget->item(0, _tableWidget->currentColumn())->text().toLocal8Bit().toStdString())
				{
					HdlInsulator_V insulator_V = new Insulator_V();
					insulator_V->setInsultorPara_V(insultorPara_V);
					insulator_V->setInsulaGeoPara_V(insulaGeoPara_V);
					writeOneInsulator_V(insulator_V, isUpdateTheta, _tableWidget->currentColumn());

					showText += QString::fromLocal8Bit(cl->getClName().c_str());
					showText += "\n";
				}
			}
		}
		QMessageBox::warning(this,
							 QString::fromLocal8Bit("��ʾ"),
							 QString::fromLocal8Bit("���Ƹ���������ֱ����V����϶Բ�еľ�Ե�Ӵ�����\n\n") + showText,
							 QMessageBox::Yes);

	}
}

// @ function: ���Ľ���ʾ��ͼ  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_V::slotChangePic()
{
	if (_tableWidget->columnCount() != 0)
	{
		int columnIndex = _tableWidget->currentColumn();
		-1 == columnIndex ? columnIndex = 0 : columnIndex;//��������޵�ǰѡ���У���Ĭ����ʾ��һ�ж�Ӧ��������ͼƬ
		SplitNum::Type splitNumType = SplitNum::Type(SplitNum::strToType(_tableWidget->item(6, columnIndex)->text().toLocal8Bit().toStdString()));

		QPixmap pixmap;
		switch (splitNumType)
		{
		case SmartCl::SplitNum::One:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/noneSplit.png"));
			break;
		case SmartCl::SplitNum::TwoV:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/twoSplitVer.png"));
			break;
		case SmartCl::SplitNum::TwoH:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/twoSplitHor.png"));
			break;
		case SmartCl::SplitNum::Three:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/threeSplit.png"));
			break;
		case SmartCl::SplitNum::Four:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/fourSplit.png"));
			break;
		case SmartCl::SplitNum::Six:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/sixSplit.png"));
			break;
		case SmartCl::SplitNum::Eight:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/eightSplit.png"));
			break;
		case SmartCl::SplitNum::Ten:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitVNum/tenSplit.png"));
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
void InsulatorLibWgt_V::slotChangeVolt(int index)
{
	try
	{
		writeDataFromForm();
		_insulatorLib_SingleVolt_V = _insulatorLib_V->getInsulatorLib_SingleVolt_V(VoltType::Type(index));
		readDataToForm();
		slotChangePic();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("�л���ѹ�ȼ�"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ���浽json�ļ�  --- by ljn 2020/1/3 16:51
void InsulatorLibWgt_V::writeToJson()
{
	try
	{
		writeDataFromForm();
		if ("" == _insulatorLib_SingleVolt_V->hasRepeat())
		{
			ClWidget::getCoreInstance()->setInsulatorLib_V(_insulatorLib_V);
			_insulatorLib_V->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());
		}
		else
		{
			std::string name = _insulatorLib_SingleVolt_V->hasRepeat();
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
void InsulatorLibWgt_V::setTableStatus()
{
	_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_tableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);//����ѡ��ģʽΪѡ����
	_tableWidget->setContextMenuPolicy(Qt::NoContextMenu);//���ñ��˵����ɼ�
}

//@function: ��ȡ��ǰ��Ե�Ӵ�   -- by mzq  2020/1/16 21:38
SmartCl::HdlInsulator_V InsulatorLibWgt_V::getCurrentInsulator()
{
	int columnIndex = _tableWidget->currentColumn();
	if (-1 != columnIndex)
		return _insulatorLib_SingleVolt_V->at(columnIndex);
	else
		return nullptr;
}

//@function: ���õ�ѹ�ȼ�   -- by mzq  2020/1/16 21:58
void InsulatorLibWgt_V::setVoltage(VoltType::Type voltType)
{
	_voltTypeCBx->setCurrentIndex(voltType);
}
