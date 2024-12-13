#include "ASManager.h"

#include "QCoreApplication"
#include "QFile"
#include "QMessageBox"
#include "QJsonDocument"
#include "SoftWareVerify.h"

namespace AboutSoftWare
{
	//@function:    -- by mzq 2022/11/16 9:20
	ASManager* ASManager::getInstance()
	{
		static ASManager* pinstance = NULL;
		if (pinstance == NULL)
			pinstance = new ASManager();
		return pinstance;
	}

	//@function:    -- by mzq 2024/1/4 20:08
	bool ASManager::isDefineNotUse3rdEncrypt()
	{
#ifdef _NotUse3rdEncrypt
		return true;
#else
		return false;
#endif
	}

	/********************************************分割线******************************************************/
	ASManager::ASManager()
	{		
		_aboutFilePath = QString::fromLocal8Bit("%1//文件//主程序//AboutSoftWare-%2.ini")
			.arg(QCoreApplication::applicationDirPath())
			.arg(AboutSoftWare::SoftWare::typeToENStr(SoftWareVerifyInstance->getSoftWareType()));
		_softWareBaseInfo = new SoftWareBaseInfo();
		_versionUpDateInfoVec = new VersionUpDateInfoVec();
	}	

	//@function: 读取本地AboutSoftWare文件   -- by mzq 2022/11/16 9:31
	void ASManager::readFromJson()
	{
		QFile file(_aboutFilePath);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		if (!file.isOpen())
		{
#ifdef _DEBUG //调试期间才读取的
			QString errInfo = QString::fromLocal8Bit("无法打开关于软件的本地配置文件，请联系软件供应商！");
			QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), errInfo, QStringLiteral("确认"));
#endif
			return;
		}

		QByteArray byteArraySource = file.readAll();
		QByteArray byteArray = QString::fromLocal8Bit(byteArraySource).toUtf8();//从本地编码转换为utf-8编码
		file.close();
		QJsonParseError parseJsonErr;
		QJsonDocument document = QJsonDocument::fromJson(byteArray, &parseJsonErr);
		if (parseJsonErr.error != QJsonParseError::NoError)
		{
#ifdef _DEBUG //调试期间才读取的
			QString errInfo = QString::fromLocal8Bit("无法解析关于软件的本地配置文件，请联系软件供应商！! \n 详细信息:%1").arg(parseJsonErr.errorString());
			QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), errInfo, QStringLiteral("确认"));
#endif
			return;
		}
		QJsonObject root = document.object();

		// 具体的读入数据
		_softWareBaseInfo->readFromJson(root[QStringLiteral("00基本信息")].toObject());
		_versionUpDateInfoVec->readFromJson(root[QStringLiteral("01版本更新信息")].toArray());
	}

}