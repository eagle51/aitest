#include "About_Dlg.h"
#include "ASManager.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QTextEdit>
#include "QMessageBox"

namespace AboutSoftWare
{
	About_Dlg::About_Dlg(QWidget* parent) : QDialog(parent)
	{
		initial();
		readData();
	}

	About_Dlg::~About_Dlg()
	{

	}

	//@function:    -- by mzq 2022/5/12 17:42
	void About_Dlg::initial()
	{
		setStyleSheet("QWidget{background: white;}");
		setMinimumSize(320, 220);
		setWindowTitle(QStringLiteral("�������"));

		_infoLbl = new QLabel();
		_infoLbl->setStyleSheet("color:black;");
		connect(_infoLbl, SIGNAL(linkActivated(QString)), this, SLOT([](const QString& url) {QDesktopServices::openUrl(QUrl(url)); }));
		_infoLbl->setOpenExternalLinks(true);

		QHBoxLayout* hLyt = new QHBoxLayout();
		hLyt->addWidget(_infoLbl);
		this->setLayout(hLyt);
	}

	//@function: ��ȡ����   -- by mzq 2022/5/12 17:42
	void About_Dlg::readData()
	{
		try
		{
			QString strInfo = QString::fromLocal8Bit("<html><font style='font-size:16px; font-family:΢���ź�; background-color:none;'>");
			strInfo += QString::fromLocal8Bit("<br>������ƣ�%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getSoftWareName());
			strInfo += QString::fromLocal8Bit("<br>�汾��Ϣ��%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getVersion());
			strInfo += QString::fromLocal8Bit("<br>�汾ʱ�䣺%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getVersionTime());
			strInfo += QString::fromLocal8Bit("<br>������Ա��%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getDeveloper());
			strInfo += QString::fromLocal8Bit("<br>��ϵ�绰��%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getTelephone());
			strInfo += QString::fromLocal8Bit("<br>QQ��%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getQQ());
			strInfo += QString::fromLocal8Bit("<br>���䣺%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getEMail());
			strInfo += QString::fromLocal8Bit("<br>��˾��ַ��<A href='%1'>%1</A> <br>").arg(ASMgr->getSoftWareBaseInfo()->getWebsite());
			strInfo += QString::fromLocal8Bit("<br> <br>");
			strInfo += QString::fromLocal8Bit("<br>%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getCompanyName_CHS());
			strInfo += QString::fromLocal8Bit("<br>%1<br>").arg(ASMgr->getSoftWareBaseInfo()->getCompanyName_EN());
			strInfo += QString::fromLocal8Bit("<\font><\html>");

			_infoLbl->setText(strInfo);
		}
		catch (std::exception& e)
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("�ļ���ȡ"), QString::fromLocal8Bit(e.what()), QStringLiteral("ȷ��"));
		}
	}
}