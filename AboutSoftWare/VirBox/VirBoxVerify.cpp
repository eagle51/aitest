#include "VirBoxVerify.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include "VirBoxChecker.h"
#include "SoftWareVerify.h"

#ifdef _UseVirBoxEncrypt
#include "ss_define.h"
#include "ss_lm_runtime.h"
#include "common.h"
#else
#include "vendor_code.h"
#include "hasp_api_cpp.h"
#endif

#ifdef _UseVirBoxEncrypt

/** 高安全强度认证数据区大小 */
#define SS_AUTH_MAX_R 4 

/** APP跟SS认证 */
typedef struct _ST_SS_AUTH
{
	int size;/** 结构体大小 size */
	int r[SS_AUTH_MAX_R];/** 随机数数组 */
	int g;/** 公钥 */
}ST_SS_AUTH;

#endif

namespace AboutSoftWare
{
	//@function:    -- by mzq 2022/10/20 21:36
	VirBoxInstance::VirBoxInstance()
		:_isInitial(false),
		_general_license_id(999999),
		_general_slm_handle(0),
		_timeout(600),//默认600s
		_ROM_number(3.14159)
	{
		qsrand(QTime::currentTime().msec());
		for (int iloop = 0; iloop < 10; ++iloop)
		{
			int randomVal = qrand();
			_randomVec.push_back(randomVal);
		}
	}

	VirBoxInstance::~VirBoxInstance()
	{

	}

	//@function:    -- by mzq 2022/11/1 9:24
	VirBoxInstance* VirBoxInstance::getInstance()
	{
		static VirBoxInstance* pinstance = NULL;
		if (pinstance == NULL)
			pinstance = new VirBoxInstance();
		return pinstance;
	}

	//@function: 周氏解密函数   -- by mzq 2022/10/25 20:38
	void VirBoxInstance::zhou_decrypt(int d, int C1, int C2, int* M)
	{
		*M = C2 - d*C1;
	}

	int VirBoxInstance::rightVal(int idx)
	{
		int moveI = 1 << idx;
		return 2 + idx + idx*idx + idx *idx *idx + moveI;
	}

#ifdef _UseVirBoxEncrypt

	//@function: 深思初始化（必须），全局调用一次即可   -- by mzq 2022/10/20 18:03
	bool VirBoxInstance::initStep1()
	{
		// 开发商密码(slm_init参数使用)，置为全局，供客户修改。
		// 建议：考虑到软件安全性，API密码请使用byte数组类型定义。
		unsigned char api_password[16] = { 0xD1, 0x56, 0xC4, 0x6E, 0xAC, 0x0A, 0xEF, 0x45, 0x2D, 0x83, 0xDD, 0xAA, 0xE9, 0xF6, 0xF0, 0xEF };

		// 1.初始化（必须），全局调用一次即可，可重复初始化，但以第一次初始化为准。
		// 服务消息回调函数版本号。注意：固定参数，请勿随意变更修改。（必须）
		ST_INIT_PARAM init_param = { 0 };
		init_param.version = SLM_CALLBACK_VERSION02;

		// 服务消息接收（回调）函数，此处不设置，即不接收服务消息。
		init_param.pfn = NULL;

		// 设置应用程序与加密锁服务通讯超时（单位：毫秒）。如果填0，则使用默认超时时间7秒，建议使用默认超时。
		// 通讯超时指API执行超时，当调用API未在超时时间内执行完成，返回超时。
		init_param.timeout = 0;

		// 设置API密码
		// 访问正式开发商用户锁必须修改此密码，开发商密码获取方法见文档说明。
		// 如果API密码与引用库不匹配，返回错误。
		memcpy(init_param.password, api_password, sizeof(api_password));
		SS_UINT32 ret = slm_init(&(init_param));
		if (SS_OK == ret)
		{
			_isInitial = true;
			return true;
		}
		else
		{
			_isInitial = false;
			return false;
		}
	}

	//@function: 清空初始化申请资源（必须），全局调用一次即可   -- by mzq 2022/10/20 18:10
	void VirBoxInstance::cleanupStepEnd()
	{
		slm_cleanup();
		_isInitial = false;
	}

	//@function: 登录通用许可ID   -- by mzq 2022/10/25 20:38
	bool VirBoxInstance::loginGeneral_lic_idStep2()
	{
		if (!_isInitial)
			initStep1();

		if (!_isInitial)
			return false;

		// 2.登录许可
		// 初始化结构体大小（必须）
		ST_LOGIN_PARAM login_param = { 0 };

		login_param.size = sizeof(ST_LOGIN_PARAM);

		// 登录访问许可ID，需要修改为您加密锁中需要访问的对应的许可ID
		login_param.license_id = _general_license_id;

		// 设置自动类型。可根据当前计算机是否插锁、检测到网络锁或登录云账号等情况逐一尝试登录本地锁、
		// 网络锁、云锁、软锁许可，只要其中有一种符合要求，即可登录成功，否则返回许可登录失败错误码。
		// 指定登录类型后只能访问特定，登录类型可使用 | （或）进行组合。类型详情见文档说明。
		login_param.login_mode = SLM_LOGIN_MODE_AUTO;

		// 设置许可会话超时时间（单位：秒），设定时间内如果未进行任何许可相关操作，超时后访问会返回许可过期错误，最大不可超过12小时
		// 必须需要重新登录才能继续使用许可。
		// 许可会话，许可登录到许可登出期间表示一次会话。
		// 许可会话超时，许可登录后没有进行任何与许可相关的操作，没有操作时间超过设置超时时间被视为程序当前会话失效，需要重新登录许可。
		login_param.timeout = _timeout;

		ST_SS_AUTH auth = { 0 };
		auth.size = (int)sizeof(ST_SS_AUTH);
		auth.g = 0x10001;
		for (int i = 0; i < SS_AUTH_MAX_R; i++)
			auth.r[i] = rand();

		int a0 = auth.r[0];
		int a1 = auth.r[1];
		SS_UINT32 encrypt_ret = slm_login(&login_param, STRUCT, &(_general_slm_handle), &(auth));
		int M = a1;
		zhou_decrypt(a0 % auth.g, auth.r[0], auth.r[1], &(M));

		if (encrypt_ret != (M^a1))
		{
			VirBoxCheckerInstance->safeLoginFlag = false;
			return false;
		}

		//返回值解扰
		SS_UINT32 ret = encrypt_ret + a0;
		if (SS_OK == ret)
		{
			VirBoxCheckerInstance->safeLoginFlag = true;
			return true;
		}
		else
		{
			VirBoxCheckerInstance->safeLoginFlag = false;
			return false;
		}
	}

	//@function: 登出通用许可ID   -- by mzq 2022/10/25 20:38
	void VirBoxInstance::logoutFull_id()
	{
		if (!_isInitial)
			return;

		// 4.许可使用完毕，登出许可，登出后不可执行与许可相关的操作。
		// 如仍需使用许可功能，请重新登录许可。
		if (0 != _general_slm_handle)//登出许可
		{
			SS_UINT32 ret = slm_logout(_general_slm_handle);
			if (SS_OK == ret)
				_general_slm_handle = 0;
		}
		for (std::map<unsigned int, unsigned int>::iterator mIter = _subModuleMap.begin(); mIter != _subModuleMap.end(); ++mIter)
			slm_logout(mIter->second);
		_subModuleMap.clear();
	}

	//@function: 验证读写区数据   -- by mzq 2022/10/28 18:16
	void VirBoxInstance::verifyROMData3()
	{
		SS_UINT32 ulROWLen = 0;
		SS_UINT32 ret = slm_user_data_getsize(_general_slm_handle, ROM, &ulROWLen);
		QString resStr;
		if (ret == SS_OK && ulROWLen > 0)
		{
			SS_BYTE decrypted_data[_virRomSyrLen] = { 0 };
			SS_BYTE *pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulROWLen);
			ret = slm_user_data_read(_general_slm_handle, ROM, pData, 0, ulROWLen);
			if (SS_OK == ret)
				ret = slm_decrypt(_general_slm_handle, pData, decrypted_data, _virRomSyrLen);
			free(pData);
			pData = NULL;
			if (ret != SS_OK)//解密失败也返回
				return;
			resStr = QString(QLatin1String((char *)decrypted_data));
		}
		if (ret != SS_OK)
			return;

		QStringList strVec = resStr.split("_");
		if (strVec.size() < 11)
			return;

		VirBoxCheckerInstance->virBoxPi = strVec[1].toDouble();
		//数组存储后用于后续继续校验
		_romIntVec.clear();
		for (int iloop = 1; iloop < 11; ++iloop)
			_romIntVec.push_back(strVec[iloop].toInt());

		if (abs(VirBoxCheckerInstance->virBoxPi - _ROM_number) <= 0.0001)
			VirBoxCheckerInstance->enlargeFactor = 1.0;

		_romIntVec[0] = int(VirBoxCheckerInstance->virBoxPi);
	}

	//@function: 判断许可ID是否存在   -- by mzq 2022/10/21 17:25
	bool VirBoxInstance::hasLicense_id(unsigned int license_id/*许可ID*/)
	{
		if (!_isInitial)
			initStep1();
		if (!_isInitial)
			return false;

		SS_UINT32 ret = SS_ERROR_INVALID_PARAM;
		std::map<unsigned int, unsigned int>::iterator  licIter = _subModuleMap.find(license_id);
		if (licIter != _subModuleMap.end())
		{//如果已经存在，则继续保存在线
			ret = slm_keep_alive(licIter->second);
		}
		//如果没有登录则
		if (ret != SS_OK)
		{//则登录
			ST_LOGIN_PARAM login_param = { 0 };
			login_param.size = sizeof(ST_LOGIN_PARAM);
			login_param.license_id = license_id;
			login_param.login_mode = SLM_LOGIN_MODE_AUTO;
			login_param.timeout = _timeout;
			SLM_HANDLE_INDEX slm_handle = 0;
			ret = slm_login(&login_param, STRUCT, &(slm_handle), NULL);
			_subModuleMap.insert(std::pair<unsigned int, unsigned int>(license_id, slm_handle));
		}
		return SS_OK == ret;
	}

	//@function: 动态校验，成功返回true，失败返回false   -- by mzq 2022/10/21 17:25
	bool VirBoxInstance::dynamicCheck()
	{
		VirBoxCheckerInstance->setErrVal();
		if (_romIntVec.empty())
			return false;
		// 维持许可会话心跳。定时发送心跳包请求，维持当前会话可用，不会超时。
		// 如果会话超时（slm_login 时设置超时）范围内没有调用任何与许可相关的接口（包括slm_keep_alive），会造成会话超时，
		// 超时后需要重新登录才能继续使用许可。
		// 注意：许可使用过程中，建议使用独立线程自定义间隔调用slm_keep_alive（建议：600秒），如果在维持心跳过程中发现错误，需要及时处理。
		int curIndex = qrand() % 10;
		SS_UINT32 ret = slm_keep_alive(_general_slm_handle);
		if (SS_OK != ret)
		{//如果失败则继续登录
			_isInitial = false;
			if (!loginGeneral_lic_idStep2())
				return false;
		}
		/*此处不设置系数相关，防止因多线性正运行到这里，然后刚好此时程序点击计算时调用系数相关的参数，引起意外的错误计算*/
		QString qStrToWrite = QString::number(_randomVec[curIndex] + _romIntVec[curIndex]);
		//写入可读取
		if (!writeToRAWAndEncrypt(qStrToWrite))
			return false;
		//可读取
		int readVal = readFromRAWAndDecrypt().toInt();
		if ((readVal - _randomVec[curIndex]) == rightVal(curIndex))
			VirBoxCheckerInstance->canSolveFlag = true;
		/*改变随机数*/
		for (int iloop = 0; iloop < 10; ++iloop)
			_randomVec[iloop] += 20;

		return VirBoxCheckerInstance->canSolveFlag;
	}

	//@function: 从通用许可读写区读取数据，读取的读写区是以 # 分隔符之前的字符   -- by mzq 2022/10/25 9:45
	QString VirBoxInstance::readFromRAWAndDecrypt()
	{
		SS_UINT32 ulROWLen = 0;
		SS_UINT32 ret = slm_user_data_getsize(_general_slm_handle, RAW, &ulROWLen);
		if (ret == SS_OK && ulROWLen > 0)
		{
			SS_BYTE decrypted_data[_virRomSyrLen] = { 0 };
			SS_BYTE *pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulROWLen);
			ret = slm_user_data_read(_general_slm_handle, RAW, pData, 0, ulROWLen);
			if (SS_OK == ret)
				ret = slm_decrypt(_general_slm_handle, pData, decrypted_data, _virRomSyrLen);
			free(pData);
			pData = NULL;
			if (ret == SS_OK)//解密失败也返回
			{
				VirBoxCheckerInstance->canWriteFlag = true;
				return QLatin1String((char *)decrypted_data);
			}
		}
		return "";
	}

	//@function: 将数据写入到深思读写区   -- by mzq 2022/10/25 9:45
	bool VirBoxInstance::writeToRAWAndEncrypt(const QString& qStr/*需要写入的字符串,注意：不支持中文，不能含有字符："#"，#用于进行字符截止判断*/)
	{
		QByteArray qByteArray = qStr.toLatin1();
		SS_BYTE *original_data = (unsigned char *)qByteArray.data();
		SS_BYTE encrypted_data[_virRomSyrLen] = { 0 };
		SS_UINT32 ret = slm_encrypt(_general_slm_handle, original_data, encrypted_data, _virRomSyrLen);
		if (SS_OK != ret)
			return false;
		ret = slm_user_data_write(_general_slm_handle, encrypted_data, 0, _virRomSyrLen);
		if (SS_OK == ret)
			VirBoxCheckerInstance->canReadFlag = true;
		return SS_OK == ret;
	}



	//TODOMZQ 验证一下一台电脑多次login一个ID，会不会记录成多个节点
#else
	//
	bool VirBoxInstance::initStep1()
	{
		ChaspFeature feature20158 = ChaspFeature::fromFeature(20158);
		Chasp SXHJFlag(feature20158);
		haspStatus status = SXHJFlag.login(vendorCode);
		if (HASP_SUCCEEDED(status))
			_isInitial = true;
		SXHJFlag.logout();
		return _isInitial;
	}

	//@function: 清空初始化申请资源（必须），全局调用一次即可   -- by mzq 2022/10/20 18:10
	void VirBoxInstance::cleanupStepEnd()
	{
		_isInitial = false;
	}

	//@function: 登录通用许可ID   -- by mzq 2022/10/25 20:38
	bool VirBoxInstance::loginGeneral_lic_idStep2()
	{
		VirBoxCheckerInstance->safeLoginFlag = false;
		if (!_isInitial)
			return false;

		ChaspFeature softF = ChaspFeature::fromFeature(_general_license_id);
		Chasp softT(softF);
		haspStatus status = softT.login(vendorCode);
		if (HASP_SUCCEEDED(status))
			VirBoxCheckerInstance->safeLoginFlag = true;
		softT.logout();
		return HASP_SUCCEEDED(status);
	}



	//@function: 登出通用许可ID   -- by mzq 2022/10/25 20:38
	void VirBoxInstance::logoutFull_id()
	{
		return;
	}

	//@function: 验证读写区数据   -- by mzq 2022/10/28 18:16
	void VirBoxInstance::verifyROMData3()
	{
		VirBoxCheckerInstance->virBoxPi = 3.14159265358979;
		VirBoxCheckerInstance->enlargeFactor = 1.0;
	}

	//@function: 判断许可ID是否存在   -- by mzq 2022/10/21 17:25
	bool VirBoxInstance::hasLicense_id(unsigned int license_id/*许可ID*/)
	{
		return true;
	}

	//@function: 动态校验，成功返回true，失败返回false   -- by mzq 2022/10/21 17:25
	bool VirBoxInstance::dynamicCheck()
	{
		VirBoxCheckerInstance->setErrVal();
		Chasp sxhjFlag;
		QDate curDate = QDate::currentDate();
		if (curDate.month() % 2 == 1)
		{
			ChaspFeature feature = ChaspFeature::fromFeature(20158);
			sxhjFlag = Chasp(feature);
		}
		else
		{
			ChaspFeature feature = ChaspFeature::fromFeature(_general_license_id);//
			sxhjFlag = Chasp(feature);
		}

		haspStatus status = sxhjFlag.login(vendorCode);
		if (!HASP_SUCCEEDED(status))
			return false;

		_general_slm_handle = (unsigned int)(&sxhjFlag);
		int curIndex = qrand() % 10;

		QString qStrToWrite = QString::number(_randomVec[curIndex]);

		this->writeToRAWAndEncrypt(qStrToWrite);

		//可读取
		int readVal = readFromRAWAndDecrypt().toInt();

		if (readVal == _randomVec[curIndex])
		{
			VirBoxCheckerInstance->canSolveFlag = true;
		}
		sxhjFlag.logout();
		/*改变随机数*/
		for (int iloop = 0; iloop < 10; ++iloop)
			_randomVec[iloop] += 20;
		return true;
	}

	//@function: 从通用许可读写区读取数据，读取的读写区是以 # 分隔符之前的字符   -- by mzq 2022/10/25 9:45
	QString VirBoxInstance::readFromRAWAndDecrypt()
	{
		std::string keyMemoryStr;
		Chasp* pSXHJFlag = (Chasp*)(_general_slm_handle);
		ChaspFile mainFile = pSXHJFlag->getFile(ChaspFile::fileReadWrite);
		haspStatus status = mainFile.read(keyMemoryStr);
		if (HASP_SUCCEEDED(status) && !keyMemoryStr.empty())
		{
			haspStatus statusWrite = pSXHJFlag->decrypt(keyMemoryStr);
			if (HASP_SUCCEEDED(statusWrite))
			{
				VirBoxCheckerInstance->canWriteFlag = true;
				return QString::fromLocal8Bit(keyMemoryStr.c_str());
			}

		}
		return "";
	}

	//@function: 将数据写入到深思读写区   -- by mzq 2022/10/25 9:45
	bool VirBoxInstance::writeToRAWAndEncrypt(const QString& qStr/*需要写入的字符串,注意：不支持中文，不能含有字符："#"，#用于进行字符截止判断*/)
	{
		hasp_size_t size = 0;
		Chasp* pSXHJFlag = (Chasp*)(_general_slm_handle);
		ChaspFile mainFile = pSXHJFlag->getFile(ChaspFile::fileReadWrite);
		haspStatus status = mainFile.getFileSize(size);
		if (size)
		{
			std::string tmpStr = qStr.toLocal8Bit();
			haspStatus statusEnCry1 = pSXHJFlag->encrypt(tmpStr);
			haspStatus statusWrite = mainFile.write(tmpStr);
			if (HASP_SUCCEEDED(statusEnCry1) && HASP_SUCCEEDED(statusWrite))
			{
				VirBoxCheckerInstance->canReadFlag = true;
				return true;
			}
		}
		return false;
	}
#endif
}