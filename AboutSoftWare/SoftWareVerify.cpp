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
		QCoreApplication::setOrganizationName("ShaanXiHengjuSoftWare");//�趨��˾���ƣ�Ϊ����ʵ����
		QCoreApplication::setApplicationName(SoftWare::typeToENStr(_softWareType));//�趨������ƣ�Ϊ����ʵ����
	}

	void SoftWareVerify::setOverTimeStep0(int iYear, int iMonth, int iDay)
	{
		_overTime.setDate(iYear, iMonth, iDay);
	}

	//@function: �����˼���ܳ�ʼ�����ɹ�����true��ʧ�ܷ���false   -- by mzq 2022/10/22 18:55
	bool SoftWareVerify::checkVirBoxInitStep1()
	{
#ifdef _NotUse3rdEncrypt
		return true;
#else
		return VirboxInstance->initStep1();
#endif

	}

	//@function: �����˼���ܣ��ɹ�����true��ʧ�ܷ���false   -- by mzq 2022/8/5 15:05
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

	//@function: �����˼������ɶ�д��   -- by mzq 2022/10/29 10:45
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

	//���߳����У���̬У��
	void SoftWareVerify::run()
	{
#ifdef _NotUse3rdEncrypt
		VirBoxCheckerInstance->canReadFlag = true;
		VirBoxCheckerInstance->canWriteFlag = true;
		VirBoxCheckerInstance->canSolveFlag = true;
		return;
#else
		//���ʧ�ܾͲ�������
		if (!VirBoxCheckerInstance->safeLoginFlag)
			return;

		//�ɹ��ڼ���У��
		int sleepTime = 0;
		int okCount = 0;
		int errCount = 0;
		while (true)
		{
			//������У��
			bool noErr = VirboxInstance->dynamicCheck();
			if (noErr)
			{//���ͨ�����´�ʱ���ӳ�500s�������ܴ�����˼����ɻỰʱ����������Զ��˳���˼��ɵ�¼

				if (errCount > 2)//��֤�ɹ�����ſ�
					emit emitVerifyResToWgt(true);//ע������ΪTrue ������False

				errCount = 0;
				++okCount;
				if (okCount > 2)
					sleepTime = 598;//15����
				else if (okCount > 4)
					break;//����4�ο��Բ�����֤��
			}
			else
			{
				//û�м�鵽����ʱ��������粻��
				if (errCount > 2)//�����ϴ�ûͨ������ʾ�����棬����Ϊ���ɱ༭
					emit emitVerifyResToWgt(false);//ע������ΪTrue ������False
				++errCount;
				okCount = 0;
				sleepTime = 1;//������ټ��
				if (errCount > 6)//����5��û�м����ȷ���ͺ�����
					break;
			}

			this->sleep(sleepTime);
		}
#endif
	}

	//@function: �ж���ģ���Ƿ����   -- by mzq 2022/8/5 15:24
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

		if (_allModuleFlag > 0)//����ȫģ�����ֱ�ӷ���true
			return true;

		//������ҪУ�����ģ����Ȩ
		return VirboxInstance->hasLicense_id(genetateId(moduleType));
#endif
	}

	//@function: ���ע����ɹ�����true��ʧ�ܷ���false   -- by mzq 2022/8/5 15:24
	bool SoftWareVerify::checkRegedit5(QWidget *wgt/*��������*/)
	{
		/*���ע���ʱ�����*/
		if (_isCheckRegedit)
		{
			QString definiteTimesQStr = getRegeditKey_definiteTimes();
			QString maxTimeDateQStr = getRegeditKey_maxTimeDate();

			/*ע�����ؼ���*/
			QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
			int definiteTimes = settings.value(definiteTimesQStr, 0).toInt();
			QDate maxTimeDate = settings.value(maxTimeDateQStr, QDate::currentDate()).toDate();
			QDate releaseDate(ASMgr->getSoftWareBaseInfo()->getVersionTime_year(),
							  ASMgr->getSoftWareBaseInfo()->getVersionTime_month(),
							  ASMgr->getSoftWareBaseInfo()->getVersionTime_day());//�����������

			//ʱ�����Ӿ�д��ע���
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
				//��ʾ���ڣ�����ϵ�˵绰
				QMessageBox::warning(wgt, QString::fromLocal8Bit("���ʹ�ù��ڣ����룺004"), "<html>" +
									 QStringLiteral("������ڿ���ԭ�����£�<br>") +
									 QStringLiteral("<br>1��������汾���ڣ�Ϊ�˷�ֹ�û�һֱʹ�ú���bug�İ汾��������������") +
									 QStringLiteral("<br>   ���������ǿ�Ƹ���ʱ�䣬����Ȩ������Ч���ڵ��û���ϵ������Ա�滻ʹ���°汾���ɣ�") +
									 QStringLiteral("<br>") +
									 QStringLiteral("<br>2��δ��װ��Ч��Ȩ����Ȩ���ڣ��������Ա��ϵ��") +
									 QStringLiteral("<br>") +
									 QString::fromLocal8Bit("<br>��ϵ�ˣ�%1 &nbsp;&nbsp; �绰: %2 &nbsp;&nbsp; QQ: %3 ")
									 .arg(ASMgr->getSoftWareBaseInfo()->getDeveloper())
									 .arg(ASMgr->getSoftWareBaseInfo()->getTelephone())
									 .arg(ASMgr->getSoftWareBaseInfo()->getQQ()) +
									 QStringLiteral("<\html>"),
									 QStringLiteral("ȷ��"));
				return false;
			}
		}
		return true;
	}

	//@function: ��������ļ����ɹ�����true��ʧ�ܷ���false   -- by mzq 2022/11/16 14:23
	bool SoftWareVerify::CheckConfig5(QWidget *wgt/*��������*/)
	{
		/*��������ļ�ʱ�����*/
		if (_isCheckConfig)
		{
			QString definiteTimesQStr = getRegeditKey_definiteTimes();
			QString maxTimeDateQStr = getRegeditKey_maxTimeDate();

			/*ע�����ؼ���*/
			QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
			int definiteTimes = settings.value(definiteTimesQStr, 0).toInt();
			QDate MaxTimeDate = settings.value(maxTimeDateQStr, QDate::currentDate()).toDate();
			QDate releaseDate(ASMgr->getSoftWareBaseInfo()->getVersionTime_year(),
							  ASMgr->getSoftWareBaseInfo()->getVersionTime_month(),
							  ASMgr->getSoftWareBaseInfo()->getVersionTime_day());//�����������

			/*�����ļ���ؼ���*/
			QString deadKey = definiteTimesQStr + QString("*789723#456@$*+4") + _overTime.toString();; // ���ڱ�־
			QByteArray deadValue = QCryptographicHash::hash("1314", QCryptographicHash::Md5); // ���ڱ�ע
			QDate sysDate = configEncryptDate(wgt, "Smart", deadValue);  // �����ļ���ʱ�䣬��ֹ�޸�ϵͳʱ��
			QByteArray deadVal = settings.value(deadKey, deadValue).toByteArray();

			if (_overTime < QDate::currentDate() ||
				deadVal != deadValue ||
				sysDate > QDate::currentDate())
			{
				//��ʾ���ڣ�����ϵ�˵绰
				QMessageBox::warning(wgt, QString::fromLocal8Bit("���ʹ�ù��ڣ����룺005"), "<html>" +
									 QStringLiteral("������ڿ���ԭ�����£�<br>") +
									 QStringLiteral("<br>1��������汾���ڣ�Ϊ�˷�ֹ�û�һֱʹ�ú���bug�İ汾��������������") +
									 QStringLiteral("<br>   ���������ǿ�Ƹ���ʱ�䣬����Ȩ������Ч���ڵ��û���ϵ������Ա�滻ʹ���°汾���ɣ�") +
									 QStringLiteral("<br>") +
									 QStringLiteral("<br>2��δ��װ��Ч��Ȩ����Ȩ���ڣ��������Ա��ϵ��") +
									 QStringLiteral("<br>") +
									 QString::fromLocal8Bit("<br>��ϵ�ˣ�%1 &nbsp;&nbsp; �绰: %2 &nbsp;&nbsp; QQ: %3 ")
									 .arg(ASMgr->getSoftWareBaseInfo()->getDeveloper())
									 .arg(ASMgr->getSoftWareBaseInfo()->getTelephone())
									 .arg(ASMgr->getSoftWareBaseInfo()->getQQ()) +
									 QStringLiteral("<\html>"),
									 QStringLiteral("ȷ��"));
				settings.setValue(deadKey, 0);
				wgt->setEnabled(false);
				return false;
			}
		}
		return true;
	}

	//@function: ע���д������ƣ����ڱ�־   -- by mzq 2022/8/4 15:15
	QString SoftWareVerify::getRegeditKey_definiteTimes()
	{
		QString res = QString("DefiniteTimes_%1_%2_%3")
			.arg(SoftWare::typeToENStr(_softWareType) + Year::typeToStr(_yearType))
			.arg(ASMgr->getSoftWareBaseInfo()->getVersionTime())
			.arg(_overTime.toString(Qt::ISODate));
		return res;
	}

	//@function: ע���д������ƣ������ʱ��   -- by mzq 2022/8/4 15:15
	QString SoftWareVerify::getRegeditKey_maxTimeDate()
	{
		QString res = QString("MaxTimeDate_%1_%2_%3")
			.arg(SoftWare::typeToENStr(_softWareType) + Year::typeToStr(_yearType))
			.arg(ASMgr->getSoftWareBaseInfo()->getVersionTime())
			.arg(_overTime.toString(Qt::ISODate));
		return res;
	}

	//@function: �����ļ���������   -- by mzq 2022/11/16 14:45
	QDate SoftWareVerify::configEncryptDate(QWidget *wgt/*��������*/, const QString& key, const QByteArray& salt)
	{
		QString iniFilePath = QString::fromLocal8Bit("%1//�ļ�//������//Config-%2.ini")
			.arg(QCoreApplication::applicationDirPath())
			.arg(AboutSoftWare::SoftWare::typeToENStr(_softWareType));

		bool isExist = QFileInfo::exists(iniFilePath);
		if (isExist)
		{
			QSettings configIni(iniFilePath, QSettings::IniFormat);
			QVariant configValue = configIni.value("Config/" + key);
			QByteArray base64Time = QByteArray::fromBase64(configValue.toByteArray()); // ȡ�����ļ��м���ʱ��
			QString sysDateStr = QString::fromLocal8Bit(base64Time.left(base64Time.size() - salt.size()));
			QDate sysDate = QDate::fromJulianDay(sysDateStr.toLongLong() + key.length());
			QDate releaseData = QDate(ASMgr->getSoftWareBaseInfo()->getVersionTime_year(),
									  ASMgr->getSoftWareBaseInfo()->getVersionTime_month(),
									  ASMgr->getSoftWareBaseInfo()->getVersionTime_day());//�������ʱ��

			if (sysDate.toJulianDay() >= releaseData.toJulianDay())/*��ϵͳʱ����ڵ��ڷ���ʱ��ʱ��˵��û�д۸�ʱ�䣬����*/
			{
				/*����ǰϵͳʱ��ʵʱ���µ������ļ���*/
				QDate maxDate = QDate::currentDate() > sysDate ? QDate::currentDate() : sysDate;// ��ֹ�޸�ϵͳʱ��
				configIni.setValue("Config/" + key, QStringLiteral("%1").arg(maxDate.toJulianDay() - key.length()).toLocal8Bit().append(salt).toBase64());
				return maxDate;
			}
			isExist = false;
		}

		if (!isExist)
		{ // �����ļ���������ʾ|����
			QMessageBox::warning(NULL, QString::fromLocal8Bit("�����ļ����󣬱��룺006"),
								 "<html>"
								 + QString::fromLocal8Bit("�����ļ�Config.ini��ʧ���𻵣�����ϵ:")
								 + QString::fromLocal8Bit("<br>��ϵ�ˣ�%1 &nbsp;&nbsp; �绰: %2 &nbsp;&nbsp; QQ: %3 ")
								 .arg(ASMgr->getSoftWareBaseInfo()->getDeveloper())
								 .arg(ASMgr->getSoftWareBaseInfo()->getTelephone())
								 .arg(ASMgr->getSoftWareBaseInfo()->getQQ())
								 + "<\html>",
								 QStringLiteral("ȷ��"));
			wgt->setEnabled(false);
		}
		return QDate();
	}

	//@function: ���������ļ�����������汾����ʱ���ã����ɱ������ڼ����Ȩ�������ļ�����ʽ����ʱ��Ӧ�õ��ø÷���   -- by mzq 2022/11/16 14:45
	void SoftWareVerify::genetateConfigFile()
	{
		QString iniFilePath = QString::fromLocal8Bit("%1//�ļ�//������//Config-%2.ini")
			.arg(QCoreApplication::applicationDirPath())
			.arg(AboutSoftWare::SoftWare::typeToENStr(_softWareType));

		bool isExist = QFileInfo::exists(iniFilePath);
		if (isExist)
		{
			QSettings configIni(iniFilePath, QSettings::IniFormat);
			QString key = "Smart";
			QByteArray salt = QCryptographicHash::hash("1314", QCryptographicHash::Md5); // ���ڱ�ע

			/*��ʼд�룬���ڸ��°汾��Ϣ�Լ�����ʱ���������ļ�ʱʹ�ã�����ʱ��Ҫע�͵�*/
			QMessageBox::warning(nullptr, QString::fromLocal8Bit("�����ļ�"), QString::fromLocal8Bit("�������ʱע��ע�͵��й����������ļ��Ĵ���"), QStringLiteral("ȷ��"));
			QByteArray base64Value = QStringLiteral("%1").arg(QDate::currentDate().toJulianDay() - key.length()).toLocal8Bit().append(salt).toBase64(); // ��һ�����������ļ�ʱ�ô�ֵ
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