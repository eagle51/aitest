#include "MeteAreaDlg.h"
#include "table_view.h"
#include "ClWidget.h"

#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QGroupBox>
#include <QSplitter>
#include "qcoreapplication.h"

AddMeteAreaDialog::AddMeteAreaDialog(QWidget *parent) :QDialog(parent)
{
	this->setFixedSize(250, 80);
	this->setWindowTitle(QStringLiteral("����������"));
	this->setLayout(initDialogLyt());
}

QVBoxLayout* AddMeteAreaDialog::initDialogLyt()
{
	_addMtAreaNameLabel = new QLabel(QStringLiteral("����������"));
	_addMtAreaNameLineEdit = new QLineEdit;
	_okBtn = new QPushButton(QStringLiteral("ȷ��"));
	_cancelBtn = new QPushButton(QStringLiteral("ȡ��"));
	_errLabel = new QLabel;
	_errLabel->setObjectName(QStringLiteral(""));

	QHBoxLayout *hLyt = new QHBoxLayout();
	hLyt->addWidget(_addMtAreaNameLabel);
	hLyt->addWidget(_addMtAreaNameLineEdit);

	QHBoxLayout *hhLyt = new QHBoxLayout();
	hhLyt->addWidget(_errLabel);
	hhLyt->addStretch();
	hhLyt->addWidget(_okBtn);
	hhLyt->addWidget(_cancelBtn);

	connect(_okBtn, SIGNAL(clicked()), this, SLOT(slotokBtn()));
	connect(_cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

	QVBoxLayout *vLyt = new QVBoxLayout(this);
	vLyt->addLayout(hLyt);
	vLyt->addLayout(hhLyt);
	return vLyt;
}

std::string  AddMeteAreaDialog::getMeteAreaName() const
{
	std::string newMeteAreaName = _addMtAreaNameLineEdit->text().toLocal8Bit();
	return newMeteAreaName;
}

void AddMeteAreaDialog::slotokBtn()
{

	std::string areaName = getMeteAreaName();//��ǰ����

	if (!areaName.empty())//��ȥ��β�ո�
	{
		areaName.erase(0, areaName.find_first_not_of(" "));
		areaName.erase(areaName.find_last_not_of(" ") + 1);
	}

	if (areaName.empty() || areaName == "")
	{
		_errLabel->setText(QStringLiteral("���Ʋ�����Ϊ�գ�"));
		return;
	}

	int index = ClWidget::getCoreInstance()->getMeteAreaLib()->has(areaName);//�ӿ�����ȡ�±꣬��Ϊ-1���򲻴��ڣ�����add�����򣬴�����ͬ����
	if (index != -1)
	{
		_errLabel->setText(QStringLiteral("�����Ѵ��ڣ�"));
		return;
	}
	else
	{
		this->getMeteAreaName();
	}
	accept();
}



/***************************************************�ָ���****************************************************/
MeteAreaDlg::MeteAreaDlg(const QString &curMeteAreaLblName, QWidget *parent /*= 0*/) :QDialog(parent)
{
	this->setLayout(initDialogLyt());
	this->setFixedSize(750, 250);
	this->changeLanguage();

	connect(_addBtn, SIGNAL(clicked()), this, SLOT(slotAddBtn_Click()));
	connect(_deleteBtn, SIGNAL(clicked()), this, SLOT(slotDeleteBtn_Click()));
	connect(_saveBtn, SIGNAL(clicked()), this, SLOT(slotSaveBtn_Click()));
	connect(_affirmBtn, SIGNAL(clicked()), this, SLOT(slotOk()));
	connect(_cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
	connect(_curMeteAreaEdt, SIGNAL(editingFinished()), this, SLOT(slotEditingFinished()));
	connect(_meteArealist, SIGNAL(currentRowChanged(int)), this, SLOT(slotSetCurrentMeteArea(int)));

	this->meteAreaNameList();
	_pMeteRegionModel->changeLanguage();//����
	int curIndex=this->curMeteAreaLblIndex(curMeteAreaLblName);
	this->_meteArealist->setCurrentRow(curIndex);
	
}

QHBoxLayout* MeteAreaDlg::initDialogLyt()
{
	_meteArealist = new QListWidget();
	_meteArealist->setFixedWidth(120);
	_curMeteAreaLbl = new QLabel();
	_curMeteAreaEdt = new QLineEdit();
	_curMeteAreaEdt->setObjectName(QStringLiteral("��ǰ��������"));//��ɫ��ʶ
	_curMeteAreaEdt->setFixedWidth(150);
	_curMeteAreaEdt->setEnabled(false);
	_deleteBtn = new QPushButton();
	_saveBtn = new QPushButton();
	_addBtn = new QPushButton();
	_affirmBtn = new QPushButton();
	_cancelBtn = new QPushButton();
	_pMeteAreaView = new StTable_View();
	_pMeteAreaView->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/����������ʾ.png' />").arg(QCoreApplication::applicationDirPath()));
	_pMeteRegionModel = new SingleMeteRegionModel(0, false);
	_pMeteAreaView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_pMeteAreaView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_pMeteAreaView->setModel(_pMeteRegionModel);
	_pMeteAreaView->setFixedHeight(121);
	_noteLabel = new QLabel(QStringLiteral("��ע������Ҫ�޸�����ʱ��Ҫ����ɹ��󣬲ſ���Ӧ�õ������У�\n���д�繤������Ϊ10m�ߴ��Ļ������١�"));

	QHBoxLayout *hLyt = new QHBoxLayout();
	hLyt->addWidget(_addBtn);
	hLyt->addWidget(_deleteBtn);
	hLyt->addWidget(_saveBtn);

	QVBoxLayout *vLyt = new QVBoxLayout();
	vLyt->addWidget(_meteArealist);
	vLyt->addLayout(hLyt);

	QGroupBox *meteAreagropBox = new QGroupBox(QStringLiteral("��������"));
	meteAreagropBox->setLayout(vLyt);
	meteAreagropBox->setFixedSize(145, 220);
	meteAreagropBox->setAlignment(Qt::AlignTop);

	QHBoxLayout *hLeftLyt = new QHBoxLayout();
	hLeftLyt->addWidget(_curMeteAreaLbl);
	hLeftLyt->addWidget(_curMeteAreaEdt);
	hLeftLyt->addStretch();

	QHBoxLayout*hhLeftLyt = new QHBoxLayout();
	hhLeftLyt->addWidget(_noteLabel);
	hhLeftLyt->addStretch();
	hhLeftLyt->addWidget(_affirmBtn);
	hhLeftLyt->addWidget(_cancelBtn);

	QVBoxLayout *vRightLyt = new QVBoxLayout();
	vRightLyt->addLayout(hLeftLyt);
	vRightLyt->addWidget(_pMeteAreaView);
	vRightLyt->addLayout(hhLeftLyt);

	QHBoxLayout *mainLyt = new QHBoxLayout(this);
	mainLyt->addWidget(meteAreagropBox);
	mainLyt->addLayout(vRightLyt);
	return mainLyt;
}

int MeteAreaDlg::curMeteAreaLblIndex(const QString &curMeteAreaLblName)
{
	for (int i = 0; i < ClWidget::getCoreInstance()->getMeteAreaLib()->getCount(); i++)
	{
		if (_meteArealist->item(i)->text() == curMeteAreaLblName)
		{
			return i;
		}
	}
	return 0;
}

void MeteAreaDlg::meteAreaNameList()
{
	_meteArealist->clear();
	for (int i = 0; i < ClWidget::getCoreInstance()->getMeteAreaLib()->getCount(); i++)
	{
		HdlSingleMeteArea pMeteArea = ClWidget::getCoreInstance()->getMeteAreaLib()->at(i);
		_meteArealist->addItem(QString::fromLocal8Bit(pMeteArea->getMeteAreaName()));
	}
}

void MeteAreaDlg::changeLanguage()
{
	_curMeteAreaLbl->setText(QStringLiteral("��ǰ��������"));
	this->setWindowTitle(QStringLiteral("������"));
	_addBtn->setText(QStringLiteral("����"));
	_deleteBtn->setText(QStringLiteral("ɾ��"));
	_saveBtn->setText(QStringLiteral("����"));
	_affirmBtn->setText(QStringLiteral("Ӧ�õ�����"));
	_cancelBtn->setText(QStringLiteral("�ر�"));
}

void MeteAreaDlg::changeEvent(QEvent * e)
{
	switch (e->type())
	{
	case QEvent::LanguageChange:
		changeLanguage();
		break;
	default:
		break;
	}
}

void MeteAreaDlg::slotAddBtn_Click()
{
	try
	{
		AddMeteAreaDialog add;
		if (add.exec() == QDialog::Accepted)
		{
			string curName = add.getMeteAreaName();
			_curMeteAreaEdt->setText(QString::fromLocal8Bit(curName.c_str()));

			string areaName = _curMeteAreaEdt->text().toLocal8Bit();//��ǰ����
			HdlSingleMeteArea curMtAreaTpl = _pMeteRegionModel->getCurMtAreaTpl()->deepCopy();//��Ƶ�ǰ��������Model��������Ϊ��Ա�����������������ƺ�vec������
			curMtAreaTpl->setMeteAreaName(areaName.c_str());//��ȡ��ǰ�����������ƣ����������ͬ�����滻��������ֲ�ͬ�������ӡ�Ҳ������add()����replaceOrPushBack();
			ClWidget::getCoreInstance()->getMeteAreaLib()->replaceOrPushBack(curMtAreaTpl);//�����ݿ������һ��������
			ClWidget::getCoreInstance()->getMeteAreaLib()->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());//д�뱾���ļ��У�
			this->meteAreaNameList();//��ȡ�����ļ����Ӷ��ڽ�������Ӹ���������
			_meteArealist->setCurrentRow(_meteArealist->count()-1);//������ʾ��ǰ��������
		}
	}
	catch (exception& e)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
		return;
	}
}

void MeteAreaDlg::slotDeleteBtn_Click()
{
	try
	{
		string areaName = _curMeteAreaEdt->text().toLocal8Bit();
		int iIndex = ClWidget::getCoreInstance()->getMeteAreaLib()->has(areaName);
		if (iIndex < 0)
		{
			QMessageBox::warning(this, QStringLiteral("ɾ������"), QStringLiteral("�޷��ҵ���������") + _curMeteAreaEdt->text(), QMessageBox::Ok);
			return;
		}
		else if (iIndex >= 0 && iIndex < 1)
		{
			QMessageBox::warning(this, QStringLiteral("�޷�ɾ��"), QStringLiteral("ϵͳ���ݣ�������ɾ����"), QMessageBox::Yes);
			return;
		}
		else
		{
			QMessageBox::StandardButton rb = QMessageBox::information(this, QStringLiteral("ɾ��"), QStringLiteral("�Ƿ�Ҫɾ����"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			if (QMessageBox::Yes == rb)
			{
				ClWidget::getCoreInstance()->getMeteAreaLib()->deleteAt(iIndex);//�����ݿ���ɾ��
				ClWidget::getCoreInstance()->getMeteAreaLib()->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());//�ڱ����ļ���ɾ��
				this->meteAreaNameList();//��ȡ�����ļ����Ӷ��ڽ�����ɾ��
			}
		}
	}
	catch (exception& e)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
		return;
	}
}

void MeteAreaDlg::slotSaveBtn_Click()
{
	try
	{
 		string areaName = _curMeteAreaEdt->text().toLocal8Bit();
		if (areaName == "")
		{
			QMessageBox::warning(this, QStringLiteral("���ƴ���"), QStringLiteral("����Ϊ�գ��޷����棡"), QMessageBox::Yes);
			return;
		}

		int index = ClWidget::getCoreInstance()->getMeteAreaLib()->has(areaName);//�����ݿ��е�����,��Ϊ-1���򲻴��ڣ�
		if (index == -1)
		{
			QMessageBox::warning(this, QStringLiteral("�������"), QStringLiteral("�޷��ҵ���Ӧ��������") + _curMeteAreaEdt->text(), QMessageBox::Ok);
			return;
		}
		else
		{
			HdlSingleMeteArea curNewMtAreaTpl = _pMeteRegionModel->getCurMtAreaTpl()->deepCopy();//��Ƶ�ǰ��������Model��������Ϊ��Ա�����������������ƺ�vec������
			ClWidget::getCoreInstance()->getMeteAreaLib()->replaceOrPushBack(curNewMtAreaTpl);//��ȡ��ǰ�����������ƣ����������ͬ�����滻��������ֲ�ͬ�������ӡ�Ҳ������add()����replaceOrPushBack();
			ClWidget::getCoreInstance()->getMeteAreaLib()->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());//���浽�����ĵ�
			QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("����ɹ�1"), QMessageBox::Ok);
		}
	}
	catch (exception& e)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
		return;
	}
}

void MeteAreaDlg::slotEditingFinished()
{
	HdlSingleMeteArea pMtAreaTpl = _pMeteRegionModel->getCurMtAreaTpl();
	string curName = _curMeteAreaEdt->text().toLocal8Bit();
	pMtAreaTpl->setMeteAreaName(curName.c_str());

}

HdlSingleMeteArea MeteAreaDlg::getCurMtAreaTpl() const
{
	return _pMeteRegionModel->getCurMtAreaTpl();
}

void MeteAreaDlg::slotSetCurrentMeteArea(int Index)
{
	if (Index == -1)
		Index = 0;
	HdlSingleMeteArea pInputMtAreaTpl = ClWidget::getCoreInstance()->getMeteAreaLib()->at(Index);//TODO:û����ƣ�ֱ�Ӳ������ݿ�
	_curMeteAreaEdt->setText(QString::fromLocal8Bit(pInputMtAreaTpl->getMeteAreaName()));

	_pMeteRegionModel->updateModel(pInputMtAreaTpl);
}

// @ function:    -- gzy  2019/5/10 10:10
void MeteAreaDlg::slotOk()
{
	string curMeteArea = _curMeteAreaEdt->text().toLocal8Bit();
	if (curMeteArea != "")
	{
		int index = ClWidget::getCoreInstance()->getMeteAreaLib()->has(curMeteArea);
		if (index >=0)
		{
			HdlSingleMeteArea pTmpl = ClWidget::getCoreInstance()->getMeteAreaLib()->at(index);

			emit MeteAreaChange(pTmpl->deepCopy());
			accept();
			return;
		}
		else
			QMessageBox::warning(this, QStringLiteral("���ƴ���"), QStringLiteral("���Ʋ����ڣ�"), QMessageBox::Yes);
	}
	else
	{
		QMessageBox::warning(this, QStringLiteral("���ƴ���"), QStringLiteral("���Ʋ�����Ϊ�գ�"), QMessageBox::Yes);
	}
}

void MeteAreaDlg::hideText() //���ر�ע��Ϣ
{
	delete _affirmBtn;
}