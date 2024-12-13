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
	this->setWindowTitle(QStringLiteral("增加气象区"));
	this->setLayout(initDialogLyt());
}

QVBoxLayout* AddMeteAreaDialog::initDialogLyt()
{
	_addMtAreaNameLabel = new QLabel(QStringLiteral("气象区名称"));
	_addMtAreaNameLineEdit = new QLineEdit;
	_okBtn = new QPushButton(QStringLiteral("确定"));
	_cancelBtn = new QPushButton(QStringLiteral("取消"));
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

	std::string areaName = getMeteAreaName();//当前名称

	if (!areaName.empty())//除去首尾空格
	{
		areaName.erase(0, areaName.find_first_not_of(" "));
		areaName.erase(areaName.find_last_not_of(" ") + 1);
	}

	if (areaName.empty() || areaName == "")
	{
		_errLabel->setText(QStringLiteral("名称不允许为空！"));
		return;
	}

	int index = ClWidget::getCoreInstance()->getMeteAreaLib()->has(areaName);//从库中提取下标，若为-1，则不存在，可以add；否则，存在相同名称
	if (index != -1)
	{
		_errLabel->setText(QStringLiteral("名称已存在！"));
		return;
	}
	else
	{
		this->getMeteAreaName();
	}
	accept();
}



/***************************************************分割线****************************************************/
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
	_pMeteRegionModel->changeLanguage();//作用
	int curIndex=this->curMeteAreaLblIndex(curMeteAreaLblName);
	this->_meteArealist->setCurrentRow(curIndex);
	
}

QHBoxLayout* MeteAreaDlg::initDialogLyt()
{
	_meteArealist = new QListWidget();
	_meteArealist->setFixedWidth(120);
	_curMeteAreaLbl = new QLabel();
	_curMeteAreaEdt = new QLineEdit();
	_curMeteAreaEdt->setObjectName(QStringLiteral("当前气象区！"));//颜色标识
	_curMeteAreaEdt->setFixedWidth(150);
	_curMeteAreaEdt->setEnabled(false);
	_deleteBtn = new QPushButton();
	_saveBtn = new QPushButton();
	_addBtn = new QPushButton();
	_affirmBtn = new QPushButton();
	_cancelBtn = new QPushButton();
	_pMeteAreaView = new StTable_View();
	_pMeteAreaView->setToolTip(QString::fromLocal8Bit("<img src='%1/Resources/ToolTipImg/气象区库提示.png' />").arg(QCoreApplication::applicationDirPath()));
	_pMeteRegionModel = new SingleMeteRegionModel(0, false);
	_pMeteAreaView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_pMeteAreaView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_pMeteAreaView->setModel(_pMeteRegionModel);
	_pMeteAreaView->setFixedHeight(121);
	_noteLabel = new QLabel(QStringLiteral("备注：当需要修改数据时，要保存成功后，才可以应用到工程中！\n表中大风工况风速为10m高处的基本风速。"));

	QHBoxLayout *hLyt = new QHBoxLayout();
	hLyt->addWidget(_addBtn);
	hLyt->addWidget(_deleteBtn);
	hLyt->addWidget(_saveBtn);

	QVBoxLayout *vLyt = new QVBoxLayout();
	vLyt->addWidget(_meteArealist);
	vLyt->addLayout(hLyt);

	QGroupBox *meteAreagropBox = new QGroupBox(QStringLiteral("气象区库"));
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
	_curMeteAreaLbl->setText(QStringLiteral("当前气象区："));
	this->setWindowTitle(QStringLiteral("气象区"));
	_addBtn->setText(QStringLiteral("增加"));
	_deleteBtn->setText(QStringLiteral("删除"));
	_saveBtn->setText(QStringLiteral("保存"));
	_affirmBtn->setText(QStringLiteral("应用到工程"));
	_cancelBtn->setText(QStringLiteral("关闭"));
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

			string areaName = _curMeteAreaEdt->text().toLocal8Bit();//当前名称
			HdlSingleMeteArea curMtAreaTpl = _pMeteRegionModel->getCurMtAreaTpl()->deepCopy();//深复制当前气象区【Model中气象区为成员变量，气象区：名称和vec工况】
			curMtAreaTpl->setMeteAreaName(areaName.c_str());//获取当前气象区的名称；如果名字相同，则替换；如果名字不同，则增加。也可以用add()代替replaceOrPushBack();
			ClWidget::getCoreInstance()->getMeteAreaLib()->replaceOrPushBack(curMtAreaTpl);//在数据库中添加一份气象区
			ClWidget::getCoreInstance()->getMeteAreaLib()->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());//写入本地文件中；
			this->meteAreaNameList();//读取本地文件，从而在界面中添加该气象区；
			_meteArealist->setCurrentRow(_meteArealist->count()-1);//界面显示当前气象区；
		}
	}
	catch (exception& e)
	{
		QMessageBox::information(this, QStringLiteral("气象区数据"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
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
			QMessageBox::warning(this, QStringLiteral("删除错误"), QStringLiteral("无法找到气象区：") + _curMeteAreaEdt->text(), QMessageBox::Ok);
			return;
		}
		else if (iIndex >= 0 && iIndex < 1)
		{
			QMessageBox::warning(this, QStringLiteral("无法删除"), QStringLiteral("系统数据，不允许删除！"), QMessageBox::Yes);
			return;
		}
		else
		{
			QMessageBox::StandardButton rb = QMessageBox::information(this, QStringLiteral("删除"), QStringLiteral("是否要删除？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			if (QMessageBox::Yes == rb)
			{
				ClWidget::getCoreInstance()->getMeteAreaLib()->deleteAt(iIndex);//在数据库中删除
				ClWidget::getCoreInstance()->getMeteAreaLib()->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());//在本地文件中删除
				this->meteAreaNameList();//读取本地文件，从而在界面中删除
			}
		}
	}
	catch (exception& e)
	{
		QMessageBox::information(this, QStringLiteral("气象区数据"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
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
			QMessageBox::warning(this, QStringLiteral("名称错误"), QStringLiteral("不能为空，无法保存！"), QMessageBox::Yes);
			return;
		}

		int index = ClWidget::getCoreInstance()->getMeteAreaLib()->has(areaName);//在数据库中的索引,若为-1，则不存在，
		if (index == -1)
		{
			QMessageBox::warning(this, QStringLiteral("保存错误"), QStringLiteral("无法找到对应气象区！") + _curMeteAreaEdt->text(), QMessageBox::Ok);
			return;
		}
		else
		{
			HdlSingleMeteArea curNewMtAreaTpl = _pMeteRegionModel->getCurMtAreaTpl()->deepCopy();//深复制当前气象区【Model中气象区为成员变量，气象区：名称和vec工况】
			ClWidget::getCoreInstance()->getMeteAreaLib()->replaceOrPushBack(curNewMtAreaTpl);//获取当前气象区的名称；如果名字相同，则替换；如果名字不同，则增加。也可以用add()代替replaceOrPushBack();
			ClWidget::getCoreInstance()->getMeteAreaLib()->writeToJson(ClWidget::getCoreInstance()->getPathFolder_File());//保存到本地文档
			QMessageBox::information(this, QStringLiteral("保存"), QStringLiteral("保存成功1"), QMessageBox::Ok);
		}
	}
	catch (exception& e)
	{
		QMessageBox::information(this, QStringLiteral("气象区数据"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
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
	HdlSingleMeteArea pInputMtAreaTpl = ClWidget::getCoreInstance()->getMeteAreaLib()->at(Index);//TODO:没有深复制，直接操作数据库
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
			QMessageBox::warning(this, QStringLiteral("名称错误"), QStringLiteral("名称不存在！"), QMessageBox::Yes);
	}
	else
	{
		QMessageBox::warning(this, QStringLiteral("名称错误"), QStringLiteral("名称不允许为空！"), QMessageBox::Yes);
	}
}

void MeteAreaDlg::hideText() //隐藏备注信息
{
	delete _affirmBtn;
}