#include "SoftWareVerify.h"
#include "SoftWareBaseInfo.h"
#include "VirBox/VirBoxChecker.h"
#include "ASManager.h"
#include <sstream>

#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include <QString>
#include <QMessageBox>
#include <QSettings>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QJsonDocument>

namespace AboutSoftWare
{
	//@function:    -- by mzq 2022/10/20 21:21
	SoftWareVerify::SoftWareVerify() :
		_isCheckRegedit(true),
		_isCheckConfig(true),
		_overTime(2095, 01, 01),
		_allModuleFlag(0)
	{

	}

	SoftWareVerify::~SoftWareVerify()
	{

	}

	//@function:    -- by mzq 2022/11/1 9:33
	AboutSoftWare::SoftWareVerify* SoftWareVerify::getInstance()
	{
		static SoftWareVerify* pinstance = 0;
		if (pinstance == NULL)
			pinstance = new SoftWareVerify();
		return pinstance;
	}

	void SoftWareVerify::setSoftwareInfo0(SoftWare::Type softTp, Year::Type yearTp)
	{
		_softWareType = softTp;
		_yearType = yearTp;
		QCoreApplication::setOrganizationName("ShaanXiHengjuSoftWare");//设定公司名称，为非真实名称
		QCoreApplication::setApplicationName(SoftWare::typeToENStr(_softWareType));//设定软件名称，为非真实名称
	}

	void SoftWareVerify::setOverTimeStep0(int iYear, int iMonth, int iDay)
	{
		_overTime.setDate(iYear, iMonth, iDay);
	}

	//@function: 检查深思加密初始化，成功返回true，失败返回false   -- by mzq 2022/10/22 18:55
	bool SoftWareVerify::checkVirBoxInitStep1()
	{
#ifdef _NotUse3rdEncrypt
		return true;
#else
		return VirboxInstance->initStep1();
#endif

	}

	//@function: 检查深思加密，成功返回true，失败返回false   -- by mzq 2022/8/5 15:05
	bool SoftWareVerify::checkVirBoxLicenseIDStep2()
	{
#ifdef _NotUse3rdEncrypt
		VirBoxCheckerInstance->safeLoginFlag = true;
		return true;
#else
		unsigned int general_license_id = genetateId(Module::General);
		VirboxInstance->setGeneral_license_id(general_license_id);
		return VirboxInstance->loginGeneral_lic_idStep2();
#endif
	}

	//@function: 检查深思加密许可读写区   -- by mzq 2022/10/29 10:45
	void SoftWareVerify::checkVirBoxLicenseROM3()
	{
#ifdef _NotUse3rdEncrypt
		VirBoxCheckerInstance->virBoxPi = 3.14159265358979;
		VirBoxCheckerInstance->enlargeFactor = 1.0;
#else
		VirBoxCheckerInstance->virBoxPi = 1.0;
		VirBoxCheckerInstance->enlargeFactor = 100000.0;
		if (!VirBoxCheckerInstance->safeLoginFlag)
			return;
		VirboxInstance->verifyROMData3();
#endif
	}

	//多线程运行，动态校验
	void SoftWareVerify::run()
	{
#ifdef _NotUse3rdEncrypt
		VirBoxCheckerInstance->canReadFlag = true;
		VirBoxCheckerInstance->canWriteFlag = true;
		VirBoxCheckerInstance->canSolveFlag = true;
		return;
#else
		//如果失败就不运行了
		if (!VirBoxCheckerInstance->safeLoginFlag)
			return;

		//成功在继续校验
		int sleepTime = 0;
		int okCount = 0;
		int errCount = 0;
		while (true)
		{
			//进来先校验
			bool noErr = VirboxInstance->dynamicCheck();
			if (noErr)
			{//检测通过，下次时间延长500s，但不能大于深思的许可会话时长，否则会自动退出深思许可登录

				if (errCount > 2)//验证成功立马放开
					emit emitVerifyResToWgt(true);//注意能用为True 不能用False

				errCount = 0;
				++okCount;
				if (okCount > 2)
					sleepTime = 598;//15分钟
				else if (okCount > 4)
					break;//超过4次可以不用验证了
			}
			else
			{
				//没有检查到狗的时候可能网络不好
				if (errCount > 2)//代表上次没通过，提示到界面，设置为不可编辑
					emit emitVerifyResToWgt(false);//注意能用为True 不能用False
				++errCount;
				okCount = 0;
				sleepTime = 1;//两秒后再检测
				if (errCount > 6)//连续5次没有检测正确，就忽略了
					break;
			}

			this->sleep(sleepTime);
		}
#endif
	}

	//@function: 判断子模块是否存在   -- by mzq 2022/8/5 15:24
	bool SoftWareVerify::hasLicense_id4(Module::Type moduleType)
	{
#ifdef _NotUse3rdEncrypt
		return true;
#else
		if (_allModuleFlag == 0)
		{
			bool hasAll = VirboxInstance->hasLicense_id(genetateId(AboutSoftWare::Module::All));
			if (hasAll)
				_allModuleFlag = 1;
			else
				_allModuleFlag = -1;
		}

		if (_allModuleFlag > 0)//存在全模块就是直接返回true
			return true;

		//其他则要校验具体模块授权
		return VirboxInstance->hasLicense_id(genetateId(moduleType));
#endif
	}

	//@function: 检查注册表，成功返回true，失败返回false   -- by mzq 2022/8/5 15:24
	bool SoftWareVerify::checkRegedit5(QWidget *wgt/*待处理窗体*/)
	{
		/*检测注册表时间加密*/
		if (_isCheckRegedit)
		{
			QString definiteTimesQStr = getRegeditKey_definiteTimes();
			QString maxTimeDateQStr = getRegeditKey_maxTimeDate();

			/*注册表相关加密*/
			QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
			int definiteTimes = settings.value(definiteTimesQStr, 0).toInt();
			QDate maxTimeDate = settings.value(maxTimeDateQStr, QDate::currentDate()).toDate();
			QDate releaseDate(ASMgr->getSoftWareBaseInfo()->getVersionTime_year(),
							  ASMgr->getSoftWareBaseInfo()->getVersionTime_month(),
							  ASMgr->getSoftWareBaseInfo()->getVersionTime_day());//软件发布日期

			//时间增加就写入注册表
			if (maxTimeDate < QDate::currentDate())
			{
				maxTimeDate = QDate::currentDate();
				settings.setValue(maxTimeDateQStr, maxTimeDate);
			}

			if (_overTime < QDate::currentDate() || _overTime < maxTimeDate || definiteTimes > 0 || releaseDate > QDate::currentDate())
			{
				definiteTimes = 1;
				settings.setValue(definiteTimesQStr, definiteTimes);
				wgt->setEnabled(false);
				//提示过期，和联系人电话
				QMessageBox::warning(wgt, QString::fromLocal8Bit("软件使用过期，编码：004"), "<html>" +
									 QStringLiteral("软件过期可能原因如下：<br>") +
									 QStringLiteral("<br>1、该软件版本过期，为了防止用户一直使用含有bug的版本，提高设计质量，") +
									 QStringLiteral("<br>   软件内置了强制更新时间，请授权还在有效期内的用户联系管理人员替换使用新版本即可；") +
									 QStringLiteral("<br>") +
									 QStringLiteral("<br>2、未安装有效授权或授权过期，请与管理员联系。") +
									 QStringLiteral("<br>") +
									 QString::fromLocal8Bit("<br>联系人：%1 &nbsp;&nbsp; 电话: %2 &nbsp;&nbsp; QQ: %3 ")
									 .arg(ASMgr->getSoftWareBaseInfo()->getDeveloper())
									 .arg(ASMgr->getSoftWareBaseInfo()->getTelephone())
									 .arg(ASMgr->getSoftWareBaseInfo()->getQQ()) +
									 QStringLiteral("<\html>"),
									 QStringLiteral("确认"));
				return false;
			}
		}
		return true;
	}

	//@function: 检查配置文件，成功返回true，失败返回false   -- by mzq 2022/11/16 14:23
	bool SoftWareVerify::CheckConfig5(QWidget *wgt/*待处理窗体*/)
	{
		/*检测配置文件时间加密*/
		if (_isCheckConfig)
		{
			QString definiteTimesQStr = getRegeditKey_definiteTimes();
			QString maxTimeDateQStr = getRegeditKey_maxTimeDate();

			/*注册表相关加密*/
			QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
			int definiteTimes = settings.value(definiteTimesQStr, 0).toInt();
			QDate MaxTimeDate = settings.value(maxTimeDateQStr, QDate::currentDate()).toDate();
			QDate releaseDate(ASMgr->getSoftWareBaseInfo()->getVersionTime_year(),
							  ASMgr->getSoftWareBaseInfo()->getVersionTime_month(),
							  ASMgr->getSoftWareBaseInfo()->getVersionTime_day());//软件发布日期

			/*配置文件相关加密*/
			QString deadKey = definiteTimesQStr + QString("*789723#456@$*+4") + _overTime.toString();; // 过期标志
			QByteArray deadValue = QCryptographicHash::hash("1314", QCryptographicHash::Md5); // 过期标注
			QDate sysDate = configEncryptDate(wgt, "Smart", deadValue);  // 配置文件中时间，防止修改系统时间
			QByteArray deadVal = settings.value(deadKey, deadValue).toByteArray();

			if (_overTime < QDate::currentDate() ||
				deadVal != deadValue ||
				sysDate > QDate::currentDate())
			{
				//提示过期，和联系人电话
				QMessageBox::warning(wgt, QString::fromLocal8Bit("软件使用过期，编码：005"), "<html>" +
									 QStringLiteral("软件过期可能原因如下：<br>") +
									 QStringLiteral("<br>1、该软件版本过期，为了防止用户一直使用含有bug的版本，提高设计质量，") +
									 QStringLiteral("<br>   软件内置了强制更新时间，请授权还在有效期内的用户联系管理人员替换使用新版本即可；") +
									 QStringLiteral("<br>") +
									 QStringLiteral("<br>2、未安装有效授权或授权过期，请与管理员联系。") +
									 QStringLiteral("<br>") +
									 QString::fromLocal8Bit("<br>联系人：%1 &nbsp;&nbsp; 电话: %2 &nbsp;&nbsp; QQ: %3 ")
									 .arg(ASMgr->getSoftWareBaseInfo()->getDeveloper())
									 .arg(ASMgr->getSoftWareBaseInfo()->getTelephone())
									 .arg(ASMgr->getSoftWareBaseInfo()->getQQ()) +
									 QStringLiteral("<\html>"),
									 QStringLiteral("确认"));
				settings.setValue(deadKey, 0);
				wgt->setEnabled(false);
				return false;
			}
		}
		return true;
	}

	//@function: 注册表写入键名称：过期标志   -- by mzq 2022/8/4 15:15
	QString SoftWareVerify::getRegeditKey_definiteTimes()
	{
		QString res = QString("DefiniteTimes_%1_%2_%3")
			.arg(SoftWare::typeToENStr(_softWareType) + Year::typeToStr(_yearType))
			.arg(ASMgr->getSoftWareBaseInfo()->getVersionTime())
			.arg(_overTime.toString(Qt::ISODate));
		return res;
	}

	//@function: 注册表写入键名称：最近的时间   -- by mzq 2022/8/4 15:15
	QString SoftWareVerify::getRegeditKey_maxTimeDate()
	{
		QString res = QString("MaxTimeDate_%1_%2_%3")
			.arg(SoftWare::typeToENStr(_softWareType) + Year::typeToStr(_yearType))
			.arg(ASMgr->getSoftWareBaseInfo()->getVersionTime())
			.arg(_overTime.toString(Qt::ISODate));
		return res;
	}

	//@function: 配置文件加密日期   -- by mzq 2022/11/16 14:45
	QDate SoftWareVerify::configEncryptDate(QWidget *wgt/*待处理窗体*/, const QString& key, const QByteArray& salt)
	{
		QString iniFilePath = QString::fromLocal8Bit("%1//文件//主程序//Config-%2.ini")
			.arg(QCoreApplication::applicationDirPath())
			.arg(AboutSoftWare::SoftWare::typeToENStr(_softWareType));

		bool isExist = QFileInfo::exists(iniFilePath);
		if (isExist)
		{
			QSettings configIni(iniFilePath, QSettings::IniFormat);
			QVariant configValue = configIni.value("Config/" + key);
			QByteArray base64Time = QByteArray::fromBase64(configValue.toByteArray()); // 取配置文件中加密时间
			QString sysDateStr = QString::fromLocal8Bit(base64Time.left(base64Time.size() - salt.size()));
			QDate sysDate = QDate::fromJulianDay(sysDateStr.toLongLong() + key.length());
			QDate releaseData = QDate(ASMgr->getSoftWareBaseInfo()->getVersionTime_year(),
									  ASMgr->getSoftWareBaseInfo()->getVersionTime_month(),
									  ASMgr->getSoftWareBaseInfo()->getVersionTime_day());//软件发布时间

			if (sysDate.toJulianDay() >= releaseData.toJulianDay())/*当系统时间大于等于发布时间时，说明没有篡改时间，否则*/
			{
				/*将当前系统时间实时更新到配置文件中*/
				QDate maxDate = QDate::currentDate() > sysDate ? QDate::currentDate() : sysDate;// 防止修改系统时间
				configIni.setValue("Config/" + key, QStringLiteral("%1").arg(maxDate.toJulianDay() - key.length()).toLocal8Bit().append(salt).toBase64());
				return maxDate;
			}
			isExist = false;
		}

		if (!isExist)
		{ // 配置文件不存在提示|错误
			QMessageBox::warning(NULL, QString::fromLocal8Bit("配置文件错误，编码：006"),
								 "<html>"
								 + QString::fromLocal8Bit("配置文件Config.ini丢失或损坏，请联系:")
								 + QString::fromLocal8Bit("<br>联系人：%1 &nbsp;&nbsp; 电话: %2 &nbsp;&nbsp; QQ: %3 ")
								 .arg(ASMgr->getSoftWareBaseInfo()->getDeveloper())
								 .arg(ASMgr->getSoftWareBaseInfo()->getTelephone())
								 .arg(ASMgr->getSoftWareBaseInfo()->getQQ())
								 + "<\html>",
								 QStringLiteral("确认"));
			wgt->setEnabled(false);
		}
		return QDate();
	}

	//@function: 生成配置文件，用于软件版本更新时调用，生成本地用于检测授权的配置文件，正式发布时不应该调用该方法   -- by mzq 2022/11/16 14:45
	void SoftWareVerify::genetateConfigFile()
	{
		QString iniFilePath = QString::fromLocal8Bit("%1//文件//主程序//Config-%2.ini")
			.arg(QCoreApplication::applicationDirPath())
			.arg(AboutSoftWare::SoftWare::typeToENStr(_softWareType));

		bool isExist = QFileInfo::exists(iniFilePath);
		if (isExist)
		{
			QSettings configIni(iniFilePath, QSettings::IniFormat);
			QString key = "Smart";
			QByteArray salt = QCryptographicHash::hash("1314", QCryptographicHash::Md5); // 过期标注

			/*初始写入，用于更新版本信息以及发布时更新配置文件时使用，发布时需要注释掉*/
			QMessageBox::warning(nullptr, QString::fromLocal8Bit("配置文件"), QString::fromLocal8Bit("软件发布时注意注释掉有关生成配置文件的代码"), QStringLiteral("确认"));
			QByteArray base64Value = QStringLiteral("%1").arg(QDate::currentDate().toJulianDay() - key.length()).toLocal8Bit().append(salt).toBase64(); // 第一次生成配置文件时用此值
			configIni.setValue("Config/" + key, base64Value);
		}
	}

	void SoftWareVerify::logCleanEnd999()
	{
#ifdef _NotUse3rdEncrypt
		return;
#else
		VirboxInstance->logoutFull_id();
		VirboxInstance->cleanupStepEnd();
#endif
	}
}