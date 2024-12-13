#include <QMessageBox>
#include <QHeaderView>
#include <QCompleter>
#include <QShortcut>
#include "WPA_Dlg.h"
#include "WireParamLib.h"
#include "WireParam.h"
#include "SplitStr.h"
#include "Auxiliary.h"
#include "DataBase.h"
#include "ClWidget.h"

namespace SmartCl{

	using namespace std;

	WPA_Dlg::WPA_Dlg(bool flagAffirmBtn, QWidget *parent) :
		QDialog(parent)
	{
		this->FormWireLibrary_Load(flagAffirmBtn);
		this->translateLanguage();
		this->createActions();
		if (!flagAffirmBtn)
			AffirmBtn->setVisible(false);
		else
			SaveDataBtn->setVisible(false);
 		pWireLibraryModel->UpdateWireVieFromSysLib();

		this->setMinimumWidth(970);
		this->setMinimumHeight(430);
	}

	void WPA_Dlg::FormWireLibrary_Load(bool flagAffirmBtn)
	{
		SearchLineEdit = new QLineEdit();
		SearchButton = new QPushButton(QStringLiteral("搜索"));
		SearchButton->setCursor(Qt::PointingHandCursor);
		SearchLineEdit->setFixedWidth(300);
		SearchLineEdit->setPlaceholderText(QStringLiteral("请输入搜索内容"));

		QHBoxLayout *pSearchLayout = new QHBoxLayout();		
		pSearchLayout->addWidget(SearchLineEdit);
		pSearchLayout->addWidget(SearchButton);
		pSearchLayout->addStretch();
		pSearchLayout->setContentsMargins(0, 0, 0, 0);

		WireLibraryView = new StTable_View(this);
		pWireLibraryModel = new WireLibraryModel(flagAffirmBtn, this);
		QItemSelectionModel* pWireLibrarySelectionModel = new QItemSelectionModel(pWireLibraryModel);
		WireLibraryView->setModel(pWireLibraryModel);
		WireLibraryView->setSelectionModel(pWireLibrarySelectionModel);

		QHBoxLayout *FormWireBtnLayout = new QHBoxLayout();
		SaveDataBtn = new QPushButton();
		AffirmBtn = new QPushButton();
		CancelBtn = new QPushButton();
		FormWireBtnLayout->addStretch();
		FormWireBtnLayout->addWidget(SaveDataBtn);
		FormWireBtnLayout->addWidget(AffirmBtn);
		FormWireBtnLayout->addWidget(CancelBtn);

		QVBoxLayout *MainLayout = new QVBoxLayout;
		MainLayout->addLayout(pSearchLayout);
		MainLayout->addWidget(WireLibraryView);
		MainLayout->addLayout(FormWireBtnLayout);
		this->setLayout(MainLayout);
		this->setMinimumWidth(970);
		this->setMinimumHeight(430);

		QShortcut *key = new QShortcut(QKeySequence(Qt::Key_Return), this);//创建一个快捷键"Key_Return"键  
		connect(key, SIGNAL(activated()), this, SLOT(SearchButton_Click()));//连接到指定槽函数  
		connect(SearchButton, SIGNAL(clicked(bool)), this, SLOT(SearchButton_Click()));
		connect(SaveDataBtn, SIGNAL(clicked()), this, SLOT(SaveDataBtn_Click()));
		connect(AffirmBtn, SIGNAL(clicked()), this, SLOT(accept()));
		connect(CancelBtn, SIGNAL(clicked()), this, SLOT(close()));

		if (!flagAffirmBtn)
		{
			WireLibraryView->setContextMenuPolicy(Qt::CustomContextMenu);//表格右键点击事件
			connect(WireLibraryView, SIGNAL(customContextMenuRequested(const QPoint&)),
				this, SLOT(WireLibraryDgv_CellMouseDown(const QPoint &)));
		}

		this->fuzzySearchData();
	}

	void WPA_Dlg::translateLanguage()
	{
		pWireLibraryModel->changeLanguage();
		SaveDataBtn->setText(QStringLiteral("保存数据"));
		AffirmBtn->setText(QStringLiteral("确认"));
		CancelBtn->setText(QStringLiteral("取消"));
		this->setWindowTitle(QStringLiteral("导线参数库"));
	}

	//右键
	void WPA_Dlg::createActions()//菜单
	{
		InsertNew = new QAction(this);
		DeleteWireProperty = new QAction(this);

		InsertNew->setText(QStringLiteral("插入导线参数"));
		DeleteWireProperty->setText(QStringLiteral("删除导线参数"));

		connect(InsertNew, SIGNAL(triggered()), this, SLOT(InsertNewBtn_Click()));
		connect(DeleteWireProperty, SIGNAL(triggered()), this, SLOT(DeleteWireProperty_Click()));
	}

	void WPA_Dlg::WireLibraryDgv_CellMouseDown(const QPoint & pos)//右键菜单
	{
		QMenu popMenu(this);
		popMenu.addAction(InsertNew);
		popMenu.addAction(DeleteWireProperty);
		popMenu.exec(QCursor::pos());
	}

	//搜索数据
	void WPA_Dlg::SearchButton_Click()
	{
		try
		{
			QString strText = SearchLineEdit->text();
			int currentRow = pWireLibraryModel->findRowByWireName(strText);
			if (currentRow == -1)
				throw exception("查询不到导线型号，如果为新增导线，则忽略本条提示！");				
			else
				WireLibraryView->selectRow(currentRow);
		}
		catch (exception& e)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("导线参数库"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
			return;
		}

	}

	//保存数据
	void WPA_Dlg::SaveDataBtn_Click()
	{
		try
		{
			pWireLibraryModel->saveToGrobalModel();
			ClWidget::getCoreInstance()->getWireParamLib()->save(ClWidget::getCoreInstance()->getPathFolder_File());
			this->close();
			this->fuzzySearchData();//重新设置模糊查询选项
		}
		catch (exception& e)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("导线参数库"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
			return;
		}
	}

	void WPA_Dlg::InsertNewBtn_Click()
	{
		try
		{
			pWireLibraryModel->InsertNewWPARow(WireLibraryView->currentIndex().row() + 1);
		}
		catch (exception& e)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("information"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
			return;
		}
	}

	void WPA_Dlg::DeleteWireProperty_Click()
	{
		try
		{
			int select = QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否删除？"), QMessageBox::Yes | QMessageBox::No);
			if (select == QMessageBox::Yes)
			{
				int currentIndex = WireLibraryView->currentIndex().row();
				pWireLibraryModel->removeRow(currentIndex);
				ClWidget::getCoreInstance()->getWireParamLib()->del(currentIndex);
			}
			else
			{
				return;
			}
		}
		catch (exception& e)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("information"), QString::fromLocal8Bit(e.what()), QMessageBox::Ok);
			return;
		}
	}

	QString WPA_Dlg::getCurrentSelectWireName() const
	{
		QModelIndex curIndex = WireLibraryView->currentIndex();
		if (curIndex.isValid() && curIndex.row()>=0)
		{
			HdlWireParam pWPA = ClWidget::getCoreInstance()->getWireParamLib()->find(curIndex.row());
			return QString::fromLocal8Bit(pWPA->getName().c_str());
		}
		return QString();
	}

	void WPA_Dlg::fuzzySearchData()
	{
		//构建模糊查询对象
		QStringList word_list;
		for (int iloop = 0; iloop < ClWidget::getCoreInstance()->getWireParamLib()->getSize(); iloop++)
		{
			HdlWireParam pWPA = ClWidget::getCoreInstance()->getWireParamLib()->find(iloop);
			word_list.append(pWPA->getName().c_str());
		}
		QCompleter *MyInfor = new QCompleter(word_list);
		MyInfor->setCaseSensitivity(Qt::CaseInsensitive);
		//设置Eidt的模糊查询对象
		SearchLineEdit->setCompleter(MyInfor);
	}
}