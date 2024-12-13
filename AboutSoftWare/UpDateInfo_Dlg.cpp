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
	UpDateInfo_Dlg::UpDateInfo_Dlg(Module::Type moduleType/*ģ������*/,
								   bool isNewestUpdataDlg,/*������ʾ�������������������°汾��Ϣ:true ������ʾ�����ǰ���ذ汾��Ϣ:false*/
								   bool isRunAutoCheck,/*�Ƿ����ڳ�������ʱ���Զ����*/
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
			this->setWindowTitle(QStringLiteral("��⵽��������°汾������Ϣ"));
		else
			this->setWindowTitle(QStringLiteral("�����ǰ�汾������Ϣ"));

		_tableWidget = new ASTableWgt(false, this);
		QStringList hHeaders;
		hHeaders << QStringLiteral(" �汾 ")
			<< QStringLiteral(" ����ʱ�� ")
			<< QStringLiteral(" ģ�� ")
			<< QStringLiteral(" �������� ");
		_tableWidget->setColumnCount(hHeaders.count());
		_tableWidget->setHorizontalHeaderLabels(hHeaders);
		_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

		TextEditDelegate * textEditer = new TextEditDelegate;
		_tableWidget->setItemDelegateForColumn(2, textEditer);
		_tableWidget->setItemDelegateForColumn(3, textEditer);

		QLabel *label = new QLabel(QStringLiteral("  ģ�����ͣ�"));
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

		QPushButton* downLoadBtn = new QPushButton(QString::fromLocal8Bit("�������°汾"));
		QPushButton *checkVersionUpdateBtn = new QPushButton(QString::fromLocal8Bit("������"));
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

	//@function: ��ȡ����   -- by mzq 2022/5/12 17:42
	void UpDateInfo_Dlg::readData()
	{
		try
		{
			double rowCount = _upDateInfoVec->getAllModuleUpdateCount(_moduleType);
			_tableWidget->setRowCount(rowCount);

			/*��ȡ��������*/
			unsigned int rowIndexMark = 0;//�б��
			std::vector<HdlVersionUpDateInfo> upDateInfoByModuleVec = _upDateInfoVec->getUpDateInfoByModule(_moduleType);
			for (unsigned int versionLoop = 0; versionLoop < upDateInfoByModuleVec.size(); ++versionLoop)
			{
				/*��ȡ�汾������ʱ��*/
				HdlVersionUpDateInfo versionUpDateInfo = upDateInfoByModuleVec[versionLoop];
				_tableWidget->setItem(rowIndexMark, 0, new QTableWidgetItem(versionUpDateInfo->getVersion()));
				_tableWidget->setItem(rowIndexMark, 1, new QTableWidgetItem(versionUpDateInfo->getVersionTime()));

				/*��ȡģ�鼰��������*/
				std::vector<HdlUpDateInfo> upDateInfos = versionUpDateInfo->getUpDateInfoByModule(_moduleType);
				for (unsigned int moduleLoop = 0; moduleLoop < upDateInfos.size(); ++moduleLoop)
				{
					HdlUpDateInfo upDateInfo = upDateInfos[moduleLoop];
					_tableWidget->setItem(rowIndexMark + moduleLoop, 2, new QTableWidgetItem(Module::typeToStr(upDateInfo->getModuleType())));
					_tableWidget->setItem(rowIndexMark + moduleLoop, 3, new QTableWidgetItem(upDateInfo->getUpdateInfo()));
				}

				/*���ð汾�Լ�����ʱ��ϲ�*/
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
			QMessageBox::warning(this, QString::fromLocal8Bit("�ļ���ȡ"), QString::fromLocal8Bit(e.what()), QStringLiteral("ȷ��"));
		}
	}

	//@function: ģ�����͸ı�   -- by mzq 2022/5/20 14:35
	void UpDateInfo_Dlg::slotModuleTypeChanged()
	{
		_moduleType = Module::strToType(_typeCBx->currentText());
		readData();
	}

	//@function: �������   -- by mzq 2022/9/2 19:10
	void UpDateInfo_Dlg::slotDownLoad()
	{
		//QDesktopServices::openUrl(QUrl(ASMgr->getSoftWareBaseInfo()->getWebsite()+"/index.php/download-center/")); // Note: ��վ���
		QDesktopServices::openUrl(QUrl(ASMgr->getSoftWareBaseInfo()->getWebsite()+"?page_id=2/")); // Note: ��վ���
	}

	//@function: ������   -- by mzq 2022/9/1 19:21
	void UpDateInfo_Dlg::slotCheckUpdate()
	{
		QNetworkRequest quest;
		QUrl Url = QUrl(ASMgr->getSoftWareBaseInfo()->getUrl());
		quest.setUrl(Url);
		quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
		_networkManager->get(quest);
	}

	//@function: �����������   -- by mzq 2022/9/1 19:21
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

	//@function: ����������Ϣ�������ɹ�����true��ʧ�ܷ���false   -- by mzq 2022/9/1 19:22
	bool UpDateInfo_Dlg::parse_UpdateJson(const QString& info)
	{
		try
		{
			QByteArray byteArray = info.toUtf8();
			QJsonParseError parseJsonErr;
			QJsonDocument document = QJsonDocument::fromJson(byteArray, &parseJsonErr);
			if (parseJsonErr.error != QJsonParseError::NoError)
			{
#ifdef _DEBUG //�����ڼ�
				QString errInfo = QString::fromLocal8Bit("�޷�����������������������ļ�����ȷ������ͨ���������һ����ѯ����ϵ�����Ӧ�̣�! \n ��ϸ��Ϣ:%1").arg(parseJsonErr.errorString());
				QMessageBox::information(nullptr, QString::fromLocal8Bit("��ʾ"), errInfo, QStringLiteral("ȷ��"));
#endif
				return false;
			}
			QJsonObject root = document.object();

			// ����Ķ�������
			_softWareBaseInfo_network->readFromJson(root[QStringLiteral("00������Ϣ")].toObject());
			_upDateInfoVec_network->readFromJson(root[QStringLiteral("01�汾������Ϣ")].toArray());
			return true;
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("�ļ���ȡ"), QString::fromLocal8Bit(e.what()), QStringLiteral("ȷ��"));
			return false;
		}
	}

	//@function: ѡ����ʽ   -- by mzq 2022/9/2 17:01
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
					QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�����ǰ�汾Ϊ���°汾��"), QStringLiteral("ȷ��"));
				return;
			}
			else
			{
				if (_isRunAutoCheck)
				{
					int btnStatus = QMessageBox::information(this,
															 QString::fromLocal8Bit("��ʾ"),
															 QString::fromLocal8Bit("�����ǰ�汾Ϊ%1�����°汾Ϊ%2")
															 .arg(ASMgr->getSoftWareBaseInfo()->getVersion())
															 .arg(_softWareBaseInfo_network->getVersion()),
															 QString::fromLocal8Bit("�鿴���°汾��Ϣ"),
															 QString::fromLocal8Bit("15���ڲ����Զ�������"),
															 QString::fromLocal8Bit("�ر�"));

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
															 QString::fromLocal8Bit("��ʾ"),
															 QString::fromLocal8Bit("�����ǰ�汾Ϊ%1�����°汾Ϊ%2")
															 .arg(ASMgr->getSoftWareBaseInfo()->getVersion())
															 .arg(_softWareBaseInfo_network->getVersion()),
															 QString::fromLocal8Bit("�鿴���°汾��Ϣ"),
															 QString::fromLocal8Bit("�ر�"));

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