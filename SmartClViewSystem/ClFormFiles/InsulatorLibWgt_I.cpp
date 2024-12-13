#include "InsulatorLibWgt_I.h"
#include "InsulatorLib_I.h"
#include "EnumLib.h"
#include "DataBase.h"
#include "ClWidget.h"
#include "Mdl_InsultorPara_I.h"

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
#include "Cl_Stright_I.h"

//I�;�Ե�Ӵ�Widget�����캯�� ������ʼ���б�
InsulatorLibWgt_I::InsulatorLibWgt_I(QWidget *parent)
	: QWidget(parent),
	_voltTypeCBx(nullptr),
	_tableWidget(nullptr),
	_splitNumCmbDeleg(nullptr),
	_tableMenu(nullptr),
	_picLbl(nullptr),
	_insulatorLib_I(nullptr),
	_insulatorLib_SingleVolt_I(nullptr)
{
	try
	{
		_insulatorLib_I = ClWidget::getCoreInstance()->getInsulatorLib_I()->clone();  //�� �ں����ݿ� ��ȡ I�;�Ե�Ӵ�����  �ٸ���һ��
		_insulatorLib_SingleVolt_I = _insulatorLib_I->getInsulatorLib_SingleVolt_I(VoltType::AC110);//Ĭ����ʾ110kv   //�� I�;�Ե�Ӵ������� ��ȡ��Ӧ��ѹ�ȼ��µľ�Ե�Ӵ���
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		return;
	}

	initialWgt();     //��ʼ������
	initTableMenu();  //��ʼ������һ��˵�
}
//��������
InsulatorLibWgt_I::~InsulatorLibWgt_I()
{

}

// @ function:   --- by ljn 2019/12/25 18:49
void InsulatorLibWgt_I::initialWgt()
{
	/*��ѹ�ȼ�*/
	_voltTypeCBx = new QComboBox();   //��ѹ�ȼ�ComboBox�����б�����
	_voltTypeCBx->setToolTip(QString::fromLocal8Bit("����ѡ���ѹ�ȼ�"));   //�ؼ���toolTip�������ؼ�����ע�͵���Ϣ
	for (unsigned int loop = 0; loop < VoltType::getVoltTypeLib().size(); ++loop)
	{
		_voltTypeCBx->addItem(QString::fromLocal8Bit(VoltType::typeToStr(VoltType::getVoltTypeLib().at(loop))));   //�ڵ�ѹ�ȼ�ComboBox�����һ����������
	}
	_voltTypeCBx->setView(new QListView());         //������ͼ �б�ؼ�
	QVBoxLayout *voltTypeLayout = new QVBoxLayout;  //������ѹ�ȼ��Ĵ�ֱ����
	voltTypeLayout->addWidget(_voltTypeCBx);        //�ڲ����мӿؼ�
	QGroupBox *voltTypeGbx = new QGroupBox(QStringLiteral("��ѹ�ȼ�"));   //���������
	voltTypeGbx->setLayout(voltTypeLayout);     //����������ô�ֱ����
	connect(_voltTypeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangeVolt(int)));  //��ǰָ���ı������ѹ�ȼ��ı�

	_tableWidget = new QTableWidget();  //�½��б�ؼ�
	//��߱�ͷ����
	QStringList vHeaders;    //�ַ����б���  ���󣨴�ֱ��ͷ�ַ����б�     
	vHeaders << QStringLiteral(" ��Ե�Ӵ��ͺ� ")
		<< QStringLiteral(" ��Ե�Ӵ������� (N) ")
		<< QStringLiteral(" ��ֱ�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" ˳�����Ե�Ӵ����� (��) ")
		<< QStringLiteral(" ������Ե�Ӵ�Ƭ�� (Ƭ) ")
		<< QStringLiteral(" ��Ƭ��Ե���ܷ���� (m^2) ")
		<< QStringLiteral(" �ش����� (N) ")
		<< QStringLiteral(" ������ ")
		<< QStringLiteral(" ���Ѽ�� (m) ")
		<< QStringLiteral(" L1 �����ҵ�-��ߵ�һת���� (m) ")
		<< QStringLiteral(" L2 �����ҵ�-���ӵ����߼йҵ� (m) ")
		<< QStringLiteral(" L3 ���ӵ����߼йҵ�-��ѹ�� (m) ")
		<< QStringLiteral(" L4 ��ѹ����� (m) ")
		<< QStringLiteral(" L5 �ӵ����߼г��� (m) ")
		<< QStringLiteral(" L6 С���� (m) ")
		<< QStringLiteral(" �� �߼м�С����ƫ��(��) ");
	_tableWidget->setRowCount(vHeaders.count());   //���б�ؼ������������ɴ�ֱ��ͷ�ַ����б�����ȷ����
	_tableWidget->setRowHeight(0, 60);             //���б�ؼ����� ��0�е� �߶�
	_tableWidget->setVerticalHeaderLabels(vHeaders);//���б�ؼ����� ��ֱ�����ǩ

	_splitNumCmbDeleg = new ComboDelegate();  //���� ��������ϴ���
	_tableWidget->setItemDelegateForRow(7, _splitNumCmbDeleg);  //Ϊĳһ�� ���� �����

	connect(_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotChangePic()));//���б��ڣ�ѡ�������ı� ����ͼƬ�ı�
	connect(_splitNumCmbDeleg, SIGNAL(valueChanged()), this, SLOT(slotChangePic()));//�ڷ�������ϴ���ֵ�����ı� ����ͼƬ�ı�

	/*��Ե�Ӵ������߼��β���*/
	QStringList SplitNumTypeList;   //�ַ����б���  ���󣨷��������͵��ַ����б�
	for (SmartCl::SplitNum::Type type = SmartCl::SplitNum::One; type <= SmartCl::SplitNum::Ten; type = (SmartCl::SplitNum::Type)(type + 1))
		SplitNumTypeList << QString::fromLocal8Bit(SmartCl::SplitNum::typeToStr(type));
	_splitNumCmbDeleg->setSList(SplitNumTypeList);   //�� ���������͵��ַ����б� ���� ��������ϴ��� ����

	QGroupBox *parameterGroupBox = new QGroupBox(QStringLiteral("��Ե�Ӵ�����"));//���� ��Ե�Ӵ����� ��Ͽ�
	QVBoxLayout *boxLayout = new QVBoxLayout;   //���� ��ֱ�򲼾�
	boxLayout->addWidget(voltTypeGbx);          //�ڴ�ֱ�򲼾� ���� ��ѹ�ȼ������ �ؼ�
	boxLayout->addWidget(_tableWidget);         //�ڴ�ֱ�򲼾� ����  �б�ؼ�
	parameterGroupBox->setLayout(boxLayout);    //�ھ�Ե�Ӵ�������Ͽ��з��� ��ֱ�򲼾�

	_picLbl = new QLabel();   //����ͼƬ��ǩ
    _picLbl->setFixedSize(310, 600);  //��ͼƬ��ǩ���� ��� �߶�
	QHBoxLayout *mainLayout = new QHBoxLayout; //����ˮƽ����
	mainLayout->addWidget(parameterGroupBox);   //ˮƽ������ӿؼ�����Ե�Ӵ�������
	mainLayout->addWidget(_picLbl);   //ˮƽ������ӿؼ���ͼƬ��ǩ��
	setLayout(mainLayout);    //�� ������ ���� ˮƽ������

	readDataToForm();  //���� �����ݶ�ȡ����������� ����
	slotChangePic();   //����ͼƬ�ı亯��
}

// @ function: �ӽ��������д�����ݿ�  --- by ljn 2019/12/26 19:35
void InsulatorLibWgt_I::writeDataFromForm()
{
	try
	{
		for (unsigned int columnIndex = 0; columnIndex < _tableWidget->columnCount(); ++columnIndex)
		{
			HdlInsulator_I insulator_I = _insulatorLib_SingleVolt_I->at(columnIndex);     //��ǰ�еĵ�ѹ�ȼ���Ӧ��I�;�Ե�����ݿ�
			writeOneInsulator_I(insulator_I,true,columnIndex);
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ϣд��"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

//@function: д��һ������   -- by mzq 2023/7/15 15:17
void InsulatorLibWgt_I::writeOneInsulator_I(HdlInsulator_I insulator_I, 
											bool writeSpecial/*�Ƿ�д���߼м�С������Ϣ*/, 
											int index)
{
	if (insulator_I.isNull())
		return;

	HdlMdl_InsultorPara_I insultorPara_I = insulator_I->getInsultorPara_I();      //I�;�Ե�Ӵ�����
	insultorPara_I->setInsultorName(_tableWidget->item(0, index)->text().toLocal8Bit().toStdString());//���б��������д��I�;�Ե�Ӵ����������ݿ���
	insultorPara_I->setInsultorWeight(_tableWidget->item(1, index)->text().toDouble());
	insultorPara_I->setNumSubs_V(_tableWidget->item(2, index)->text().toInt());
	insultorPara_I->setNumSubs_H(_tableWidget->item(3, index)->text().toInt());
	insultorPara_I->setNumPlate(_tableWidget->item(4, index)->text().toInt());
	insultorPara_I->setShieldArea(_tableWidget->item(5, index)->text().toDouble());
	insultorPara_I->setHammerWeight(_tableWidget->item(6, index)->text().toDouble());

	HdlMdl_InsulaGeoPara_I insulaGeoPara_I = insulator_I->getInsulaGeoPara_I();      //I�;�Ե�Ӵ����β���
	insulaGeoPara_I->setSplitNum(SplitNum::Type(SplitNum::strToType(_tableWidget->item(7, index)->text().toLocal8Bit().toStdString())));
	insulaGeoPara_I->setSplitShow(_tableWidget->item(8, index)->text().toLocal8Bit().toStdString());//���б��������д��I�;�Ե�Ӵ����߼��β��������ݿ���
	insulaGeoPara_I->setL1(_tableWidget->item(9, index)->text().toDouble());
	insulaGeoPara_I->setL2(_tableWidget->item(10, index)->text().toDouble());
	insulaGeoPara_I->setL3(_tableWidget->item(11, index)->text().toDouble());
	insulaGeoPara_I->setL4(_tableWidget->item(12, index)->text().toDouble());
	insulaGeoPara_I->setL5(_tableWidget->item(13, index)->text().toDouble());
	if (writeSpecial)
	{
		insulaGeoPara_I->setL6(_tableWidget->item(14, index)->text().toDouble());
		insulaGeoPara_I->setTheta(_tableWidget->item(15, index)->text().toDouble());
	}
}

// @ function: �����ݶ��������  --- by ljn 2019/12/26 19:37
void InsulatorLibWgt_I::readDataToForm()
{
	try
	{
		/*��ʼ�������������������*/
		int columnCount = _insulatorLib_SingleVolt_I->getSize();  //��ѹ�ȼ���Ӧ��I�;�Ե�����ݿ������
		_tableWidget->clearContents();     //����б�
		_tableWidget->setColumnCount(columnCount);    //���б���������

		for (unsigned int columnIndex = 0; columnIndex < columnCount; ++columnIndex)
		{
			/*��Ե�Ӵ�ˢ�½�������*/
			HdlInsulator_I insulator_I = _insulatorLib_SingleVolt_I->at(columnIndex); //��ǰ�еĵ�ѹ�ȼ���Ӧ��I�;�Ե�����ݿ�

			HdlMdl_InsultorPara_I insultorPara_I = insulator_I->getInsultorPara_I();
			_tableWidget->setItem(0, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insultorPara_I->getInsultorName().c_str())));
			_tableWidget->setItem(1, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getInsultorWeight())));//��ȡ����������ӵ��б�ÿ������
			_tableWidget->setItem(2, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getNumSubs_V())));
			_tableWidget->setItem(3, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getNumSubs_H())));
			_tableWidget->setItem(4, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getNumPlate())));
			_tableWidget->setItem(5, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getShieldArea())));
			_tableWidget->setItem(6, columnIndex, new QTableWidgetItem(QString::number(insultorPara_I->getHammerWeight())));

			HdlMdl_InsulaGeoPara_I insulaGeoPara_I = insulator_I->getInsulaGeoPara_I();
			_tableWidget->setItem(7, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(SplitNum::typeToStr(insulaGeoPara_I->getSplitNum()))));
			_tableWidget->setItem(8, columnIndex, new QTableWidgetItem(QString::fromLocal8Bit(insulaGeoPara_I->getSplitShow().c_str())));
			_tableWidget->setItem(9, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL1())));
			_tableWidget->setItem(10, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL2())));
			_tableWidget->setItem(11, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL3())));
			_tableWidget->setItem(12, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL4())));
			_tableWidget->setItem(13, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL5())));
			_tableWidget->setItem(14, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getL6())));
			_tableWidget->setItem(15, columnIndex, new QTableWidgetItem(QString::number(insulaGeoPara_I->getTheta())));
		}
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("��Ե�Ӵ�I�����ݼ���"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ��ʼ������һ��˵�  --- by ljn 2019/12/25 18:55
void InsulatorLibWgt_I::initTableMenu()
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
	_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);//���� �Ҽ��˵����� ��ʾ��ʽ

	connect(_tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotTableMenu(QPoint)));    //���б�ؼ��У��û����ݲ˵����󣨵�������˽��棬���һ��˵�
	connect(addAction, SIGNAL(triggered()), this, SLOT(slotAddColumn()));           
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(slotDeletColumn()));  
	connect(applyAction, SIGNAL(triggered()), this, SLOT(slotApplyToProject_I()));
}

// @ function: ���Ҽ��˵�  --- by ljn 2019/12/25 19:55
void InsulatorLibWgt_I::slotTableMenu(QPoint pos)
{
	_tableMenu->exec(QCursor::pos());//�Ҽ��˵� �����λ����ʾ
}

// @ function: ������  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_I::slotAddColumn()
{
	try
	{
		writeDataFromForm();     //�ӽ��������д�����ݿ�
		int columnIndex = _tableWidget->currentColumn();   //�б�ĵ�ǰ����
		HdlInsulator_I curInsulator = new Insulator_I();   //�½�һ����Ե�Ӵ�I�����ݿ�   ָ�����  
		while (_insulatorLib_SingleVolt_I->has(curInsulator->getInsultorPara_I()->getInsultorName()))//���ݿ��У�����Ӧ��ѹ�ȼ��� ��ѯ��Ӧ���Ƶĵ���
		{
			std::string newName = curInsulator->getInsultorPara_I()->getInsultorName() + "_����";   //������=������+����
			curInsulator->getInsultorPara_I()->setInsultorName(newName);    //�������ַŵ����ݿ���
		}
		_insulatorLib_SingleVolt_I->insert(curInsulator, columnIndex + 1);//�ڵ�ǰ����һ�в�������
		readDataToForm();//���ں����ݶ�ȡ������
		_tableWidget->setCurrentCell(0, columnIndex + 1);    //�б����õ�ǰѡ��Ϊ��ǰ�е���һ��
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("������"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ɾ����  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_I::slotDeletColumn()
{
	int select = QMessageBox::warning(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�Ƿ�ɾ����"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		try
		{
			writeDataFromForm();//�ӽ��������д�����ݿ�
			int columnIndex = _tableWidget->currentColumn(); //�б�ĵ�ǰ����
			_insulatorLib_SingleVolt_I->del(columnIndex);//���ݿ���ɾ����ǰ��
			readDataToForm();//���ں����ݶ�ȡ������
			_tableWidget->setCurrentCell(0, 0);//���õ�ǰѡ��Ϊ��һ��
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("ɾ����"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
		}
	}
}

//@function: ����ֱ����I����϶Բ������   -- by mzq 2023/7/15 14:39
void InsulatorLibWgt_I::slotApplyToProject_I()
{
	int select = QMessageBox::warning(this, 
									  QString::fromLocal8Bit("��ʾ"), 
									  QString::fromLocal8Bit("�Ƿ񽫵�ǰ��Ե�Ӵ����������������̵����С�ֱ����I������϶Բ�е�ͬ����Ե�Ӵ��У�"), QMessageBox::Yes | QMessageBox::No);
	if (select == QMessageBox::Yes)
	{
		writeDataFromForm();     //�ӽ��������д�����ݿ�

		bool isUpdateL6Theta = true;
		int select = QMessageBox::warning(this,
										  QString::fromLocal8Bit("��ʾ"),
										  QString::fromLocal8Bit("�Ƿ�ͬ�����¡�С�������Լ����߼м�С����ƫ�ǡ���"),
										  QMessageBox::Yes | QMessageBox::No);
		if (select == QMessageBox::Yes)
			isUpdateL6Theta = true;
		else
			isUpdateL6Theta = false;

		QString showText = "";
		for (unsigned int loop = 0; loop < ClWidget::getCoreInstance()->getClCount(); ++loop)
		{
			HdlCl cl = ClWidget::getCoreInstance()->getClAt(loop);
			if (cl->getClType() == ClCate::StraightIStr)
			{
				HdlCl_Stright_I stright_I = dynamic_cast<Cl_Stright_I*>(cl.GetPointee());
				HdlMdl_InsultorPara_I insultorPara_I = stright_I->getDataUI_I()->getInsultorPara_I();
				HdlMdl_InsulaGeoPara_I insulaGeoPara_I = stright_I->getDataUI_I()->getInsulaGeoPara_I();

				if (insultorPara_I->getInsulatorType() == InsulatorType::Type_I
					&& insultorPara_I->getInsultorName() == _tableWidget->item(0, _tableWidget->currentColumn())->text().toLocal8Bit().toStdString())
				{
					HdlInsulator_I insulator_I = new Insulator_I();
					insulator_I->setInsultorPara_I(insultorPara_I);
					insulator_I->setInsulaGeoPara_I(insulaGeoPara_I);
					writeOneInsulator_I(insulator_I, isUpdateL6Theta, _tableWidget->currentColumn());

					showText += QString::fromLocal8Bit(cl->getClName().c_str());
					showText += "\n";
				}
			}
		}
		QMessageBox::warning(this,
							 QString::fromLocal8Bit("��ʾ"),
							 QString::fromLocal8Bit("���Ƹ���������ֱ����I����϶Բ�еľ�Ե�Ӵ�����\n\n") + showText,
							 QMessageBox::Yes );

	}
}

// @ function: ���Ľ���ʾ��ͼ  --- by ljn 2019/12/25 19:47
void InsulatorLibWgt_I::slotChangePic()
{
	if (_tableWidget->columnCount() != 0)//���������Ϊ��
	{
		int columnIndex = _tableWidget->currentColumn();//��ȡ�б�ǰ��
		-1 == columnIndex ? columnIndex = 0 : columnIndex;//��������޵�ǰѡ���У�-1 == columnIndexΪ�棩����Ĭ����ʾ��һ�У�columnIndex = 0����Ӧ��������ͼƬ
		SplitNum::Type splitNumType = SplitNum::Type(SplitNum::strToType(_tableWidget->item(7, columnIndex)->text().toLocal8Bit().toStdString()));//��ȡ����������

		QPixmap pixmap;
		switch (splitNumType)
		{
		case SmartCl::SplitNum::One:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/noneSplit.png"));
			break;
		case SmartCl::SplitNum::TwoV:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/twoSplitVer.png"));
			break;
		case SmartCl::SplitNum::TwoH:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/twoSplitHor.png"));
			break;
		case SmartCl::SplitNum::Three:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/threeSplit.png"));
			break;
		case SmartCl::SplitNum::Four:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/fourSplit.png"));
			break;
		case SmartCl::SplitNum::Six:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/sixSplit.png"));
			break;
		case SmartCl::SplitNum::Eight:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/eightSplit.png"));
			break;
		case SmartCl::SplitNum::Ten:
			pixmap.load(QCoreApplication::applicationDirPath() + QString("/Resources/SplitINum/tenSplit.png"));
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
void InsulatorLibWgt_I::slotChangeVolt(int index)
{
	try
	{
		writeDataFromForm();    //д����
		_insulatorLib_SingleVolt_I = _insulatorLib_I->getInsulatorLib_SingleVolt_I(VoltType::Type(index));
		readDataToForm();       //������
		slotChangePic();        //�ı�ͼƬ
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("�л���ѹ�ȼ�"), QString::fromLocal8Bit(e.what()), QMessageBox::Yes);
	}
}

// @ function: ���浽json�ļ�  --- by ljn 2020/1/3 16:51
void InsulatorLibWgt_I::writeToJson()
{
	try
	{
		writeDataFromForm();//д����
		if ("" == _insulatorLib_SingleVolt_I->hasRepeat())//���û���ظ��ľ�Ե�Ӵ�����
		{
			ClWidget::getCoreInstance()->setInsulatorLib_I(_insulatorLib_I);//�ں����ݿ�����I�;�Ե�Ӵ�����
			_insulatorLib_I->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File()); //I�;�Ե�Ӵ�����д��json�ļ�   ��ȡ�����ܵ��ļ���Ŀ¼�� ���ļ��� ����·��
		}
		else
		{
			std::string name = _insulatorLib_SingleVolt_I->hasRepeat();
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
void InsulatorLibWgt_I::setTableStatus()
{
	_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);      //�������Ϊ���ɱ༭
	_tableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);  //����ѡ��ģʽΪѡ�е�����
	_tableWidget->setContextMenuPolicy(Qt::NoContextMenu);                 //���ñ���Ҽ��˵����ɼ�
}

//@function: ��ȡ��ǰ��Ե�Ӵ�   -- by mzq  2020/1/16 21:38
SmartCl::HdlInsulator_I InsulatorLibWgt_I::getCurrentInsulator()
{
	int columnIndex = _tableWidget->currentColumn();   //���ĵ�ǰ��
	if (-1 != columnIndex)     //�����ѡ�����
		return _insulatorLib_SingleVolt_I->at(columnIndex);   //��ѹ�ȼ���Ӧ��I�;�Ե�Ӵ��������ڵ���
	else
		return nullptr;
}

//@function: ���õ�ѹ�ȼ�   -- by mzq  2020/1/16 21:58
void InsulatorLibWgt_I::setVoltage(VoltType::Type voltType)
{
	_voltTypeCBx->setCurrentIndex(voltType);//�� ��ѹ�ȼ���Ͽ� ���� ��ǰ�ĵ�ѹ�ȼ�ָ��
	slotChangeVolt(voltType);
}