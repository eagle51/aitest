#include "UpDateInfo_Dlg.h"
#include "ASManager.h"
#include "SoftWareBaseInfo.h"
#include "TextEditDelegate.h"

#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QTextEdit>
#include "QCoreApplication"
#include "QMessageBox"
#include <QSettings>
#include <QCheckBox>
#include <QPushButton>
#include "QtNetwork/QNetworkAccessManager"
#include "QtNetwork/QNetworkReply"
#include "QDesktopServices"
#include "QJsonDocument"
#include "QListView"

namespace AboutSoftWare
{
	UpDateInfo_Dlg::UpDateInfo_Dlg(Module::Type moduleType/*模块类型*/,
								   bool isNewestUpdataDlg,/*用于显示网络更新下来的软件最新版本信息:true 用于显示软件当前本地版本信息:false*/
								   bool isRunAutoCheck,/*是否用于程序运行时的自动检测*/
								   QWidget* parent)
		: QDialog(parent),
		_moduleType(moduleType),
		_isNewestUpdataDlg(isNewestUpdataDlg),
		_isRunAutoCheck(isRunAutoCheck),
		_upDateInfoVec(new VersionUpDateInfoVec()),
		_softWareBaseInfo_network(new SoftWareBaseInfo()),
		_upDateInfoVec_network(new VersionUpDateInfoVec())
	{
		initial();
		_typeCBx->setCurrentText(Module::typeToStr(_moduleType));
		_networkManager = new QNetworkAccessManager(this);
		connect(_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));

		if (!_isNewestUpdataDlg)
		{
			_upDateInfoVec = ASMgr->getVersionUpDateInfoVec();
			readData();
		}
	}

	UpDateInfo_Dlg::~UpDateInfo_Dlg()
	{

	}

	//@function:    -- by xyy  2021/3/30 10:36
	void UpDateInfo_Dlg::initial()
	{
		if (_isNewestUpdataDlg)
			this->setWindowTitle(QStringLiteral("检测到的软件最新版本更新信息"));
		else
			this->setWindowTitle(QStringLiteral("软件当前版本更新信息"));

		_tableWidget = new ASTableWgt(false, this);
		QStringList hHeaders;
		hHeaders << QStringLiteral(" 版本 ")
			<< QStringLiteral(" 更新时间 ")
			<< QStringLiteral(" 模块 ")
			<< QStringLiteral(" 更新内容 ");
		_tableWidget->setColumnCount(hHeaders.count());
		_tableWidget->setHorizontalHeaderLabels(hHeaders);
		_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

		TextEditDelegate * textEditer = new TextEditDelegate;
		_tableWidget->setItemDelegateForColumn(2, textEditer);
		_tableWidget->setItemDelegateForColumn(3, textEditer);

		QLabel *label = new QLabel(QStringLiteral("  模块类型："));
		_typeCBx = new QComboBox();
		_typeCBx->setView(new QListView());
		_typeCBx->setFixedWidth(150);
		for (unsigned int loop = 0; loop < Module::getAllTypeVec().size(); ++loop)
		{
			_typeCBx->addItem(Module::typeToStr(Module::getAllTypeVec().at(loop)));
		}
		connect(_typeCBx, SIGNAL(currentIndexChanged(int)), this, SLOT(slotModuleTypeChanged()));
		QHBoxLayout *loopTypeLayout = new QHBoxLayout();
		loopTypeLayout->addWidget(label);
		loopTypeLayout->addWidget(_typeCBx);
		loopTypeLayout->addStretch();

		QPushButton* downLoadBtn = new QPushButton(QString::fromLocal8Bit("下载最新版本"));
		QPushButton *checkVersionUpdateBtn = new QPushButton(QString::fromLocal8Bit("检测更新"));
		connect(downLoadBtn, SIGNAL(clicked()), this, SLOT(slotDownLoad()));
		connect(checkVersionUpdateBtn, SIGNAL(clicked()), this, SLOT(slotCheckUpdate()));

		QHBoxLayout* hLyt = new QHBoxLayout();
		hLyt->addStretch();
		hLyt->addWidget(downLoadBtn, 0, Qt::AlignCenter);
		hLyt->addWidget(checkVersionUpdateBtn, 0, Qt::AlignCenter);

		if (_isNewestUpdataDlg)
			checkVersionUpdateBtn->setVisible(false);
		else
			downLoadBtn->setVisible(false);

		QVBoxLayout *mainLayout = new QVBoxLayout;
		mainLayout->addLayout(loopTypeLayout);
		mainLayout->addWidget(_tableWidget);
		mainLayout->addLayout(hLyt);
		setLayout(mainLayout);

		this->setMinimumSize(1130, 600);
	}

	//@function: 读取数据   -- by mzq 2022/5/12 17:42
	void UpDateInfo_Dlg::readData()
	{
		try
		{
			double rowCount = _upDateInfoVec->getAllModuleUpdateCount(_moduleType);
			_tableWidget->setRowCount(rowCount);

			/*读取更新内容*/
			unsigned int rowIndexMark = 0;//行标记
			std::vector<HdlVersionUpDateInfo> upDateInfoByModuleVec = _upDateInfoVec->getUpDateInfoByModule(_moduleType);
			for (unsigned int versionLoop = 0; versionLoop < upDateInfoByModuleVec.size(); ++versionLoop)
			{
				/*读取版本及更新时间*/
				HdlVersionUpDateInfo versionUpDateInfo = upDateInfoByModuleVec[versionLoop];
				_tableWidget->setItem(rowIndexMark, 0, new QTableWidgetItem(versionUpDateInfo->getVersion()));
				_tableWidget->setItem(rowIndexMark, 1, new QTableWidgetItem(versionUpDateInfo->getVersionTime()));

				/*读取模块及更新内容*/
				std::vector<HdlUpDateInfo> upDateInfos = versionUpDateInfo->getUpDateInfoByModule(_moduleType);
				for (unsigned int moduleLoop = 0; moduleLoop < upDateInfos.size(); ++moduleLoop)
				{
					HdlUpDateInfo upDateInfo = upDateInfos[moduleLoop];
					_tableWidget->setItem(rowIndexMark + moduleLoop, 2, new QTableWidgetItem(Module::typeToStr(upDateInfo->getModuleType())));
					_tableWidget->setItem(rowIndexMark + moduleLoop, 3, new QTableWidgetItem(upDateInfo->getUpdateInfo()));
				}

				/*设置版本以及更新时间合并*/
				_tableWidget->setSpan(rowIndexMark, 0, upDateInfos.size(), 1);
				_tableWidget->setSpan(rowIndexMark, 1, upDateInfos.size(), 1);

				rowIndexMark += upDateInfos.size();
			}

			_tableWidget->setColumnWidth(0, 80);
			_tableWidget->setColumnWidth(1, 100);
			_tableWidget->setColumnWidth(2, 110);
			_tableWidget->setColumnWidth(3, 500);

			_tableWidget->resizeRowsToContents();
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("文件读取"), QString::fromLocal8Bit(e.what()), QStringLiteral("确认"));
		}
	}

	//@function: 模块类型改变   -- by mzq 2022/5/20 14:35
	void UpDateInfo_Dlg::slotModuleTypeChanged()
	{
		_moduleType = Module::strToType(_typeCBx->currentText());
		readData();
	}

	//@function: 下载软件   -- by mzq 2022/9/2 19:10
	void UpDateInfo_Dlg::slotDownLoad()
	{
		//QDesktopServices::openUrl(QUrl(ASMgr->getSoftWareBaseInfo()->getWebsite()+"/index.php/download-center/")); // Note: 网站变更
		QDesktopServices::openUrl(QUrl(ASMgr->getSoftWareBaseInfo()->getWebsite()+"?page_id=2/")); // Note: 网站变更
	}

	//@function: 检查更新   -- by mzq 2022/9/1 19:21
	void UpDateInfo_Dlg::slotCheckUpdate()
	{
		QNetworkRequest quest;
		QUrl Url = QUrl(ASMgr->getSoftWareBaseInfo()->getUrl());
		quest.setUrl(Url);
		quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
		_networkManager->get(quest);
	}

	//@function: 网络请求结束   -- by mzq 2022/9/1 19:21
	void UpDateInfo_Dlg::slotReplyFinished(QNetworkReply* reply)
	{
		QString info = QString::fromLocal8Bit(reply->readAll());
		if (info == "" && _isRunAutoCheck)
			return;
		bool succeed = parse_UpdateJson(info);
		reply->deleteLater();

		if (succeed)
			selectProcessingMode();
	}

	//@function: 解析更新信息，解析成功返回true，失败返回false   -- by mzq 2022/9/1 19:22
	bool UpDateInfo_Dlg::parse_UpdateJson(const QString& info)
	{
		try
		{
			QByteArray byteArray = info.toUtf8();
			QJsonParseError parseJsonErr;
			QJsonDocument document = QJsonDocument::fromJson(byteArray, &parseJsonErr);
			if (parseJsonErr.error != QJsonParseError::NoError)
			{
#ifdef _DEBUG //调试期间
				QString errInfo = QString::fromLocal8Bit("无法解析关于软件的网络配置文件，请确保网络通畅，如需进一步咨询请联系软件供应商！! \n 详细信息:%1").arg(parseJsonErr.errorString());
				QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), errInfo, QStringLiteral("确认"));
#endif
				return false;
			}
			QJsonObject root = document.object();

			// 具体的读入数据
			_softWareBaseInfo_network->readFromJson(root[QStringLiteral("00基本信息")].toObject());
			_upDateInfoVec_network->readFromJson(root[QStringLiteral("01版本更新信息")].toArray());
			return true;
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("文件读取"), QString::fromLocal8Bit(e.what()), QStringLiteral("确认"));
			return false;
		}
	}

	//@function: 选择处理方式   -- by mzq 2022/9/2 17:01
	void UpDateInfo_Dlg::selectProcessingMode()
	{
		QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
		QDate unCheckDate = settings.value("UnCheckDate", QDate::currentDate()).toDate();
		QDate currentDate = QDate::currentDate();
		if ((currentDate >= unCheckDate) || !_isRunAutoCheck)
		{
			if (ASMgr->getVersionUpDateInfoVec()->isNewerOrEqual(_upDateInfoVec_network))
			{
				if (!_isRunAutoCheck)
					QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("软件当前版本为最新版本！"), QStringLiteral("确认"));
				return;
			}
			else
			{
				if (_isRunAutoCheck)
				{
					int btnStatus = QMessageBox::information(this,
															 QString::fromLocal8Bit("提示"),
															 QString::fromLocal8Bit("软件当前版本为%1，最新版本为%2")
															 .arg(ASMgr->getSoftWareBaseInfo()->getVersion())
															 .arg(_softWareBaseInfo_network->getVersion()),
															 QString::fromLocal8Bit("查看最新版本信息"),
															 QString::fromLocal8Bit("15天内不再自动检测更新"),
															 QString::fromLocal8Bit("关闭"));

					if (btnStatus == 0)
					{
						UpDateInfo_Dlg dlg(AboutSoftWare::Module::All, true, false, this);
						dlg.setUpDateInfoVec(_upDateInfoVec_network);
						dlg.readData();
						dlg.exec();
					}
					else if (btnStatus == 1)
					{
						QDate unCheckDate = QDate::currentDate().addDays(15);
						settings.setValue("UnCheckDate", unCheckDate);
					}
					else if (btnStatus == 2)
						return;
				}
				else
				{
					int btnStatus = QMessageBox::information(this,
															 QString::fromLocal8Bit("提示"),
															 QString::fromLocal8Bit("软件当前版本为%1，最新版本为%2")
															 .arg(ASMgr->getSoftWareBaseInfo()->getVersion())
															 .arg(_softWareBaseInfo_network->getVersion()),
															 QString::fromLocal8Bit("查看最新版本信息"),
															 QString::fromLocal8Bit("关闭"));

					if (btnStatus == 0)
					{
						UpDateInfo_Dlg dlg(AboutSoftWare::Module::All, true, false, this);
						dlg.setUpDateInfoVec(_upDateInfoVec_network);
						dlg.readData();
						dlg.exec();
					}
					else if (btnStatus == 1)
						return;
				}
			}
		}
	}
}