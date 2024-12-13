/****************************************************************/
//Author:   马梓淇
//Version:  1.0
//Date:		2022.10.17
//Description:	深思加密验证		
/****************************************************************/
#pragma once
#include <map>
#include "EnumLib_AS.h"

#ifdef _UseVirBoxEncrypt
#ifdef _DEBUG
#pragma comment(lib,"..\\..\\3rd\\VirBox\\lib\\Win32\\slm_runtime_api_dev.lib")//如果为64位，则需替换64位lib
#else 
#pragma comment(lib,"..\\..\\3rd\\VirBox\\lib\\Win32\\slm_runtime_api.lib")//如果为64位，则需替换64位lib
#endif 
#else
#pragma comment(lib,"..\\..\\3rd\\SentinelHASPCpp\\lib\\libhasp_windows_101130.lib")//如果为64位，则需替换64位lib
#endif

#define _virRomSyrLen 64

namespace AboutSoftWare
{
	/*深思加密单例*/
	class VirBoxInstance
	{

	public:
		static VirBoxInstance* getInstance();

		void setGeneral_license_id(unsigned int val) { _general_license_id = val; }
		bool initStep1();//深思初始化（必须），全局调用一次即可
		bool loginGeneral_lic_idStep2();//登录通用许可ID
		void logoutFull_id();//登出通用许可ID
		void verifyROMData3();//验证读写区数据
		void cleanupStepEnd();//清空初始化申请资源（必须），全局调用一次即可
		bool hasLicense_id(unsigned int license_id/*许可ID*/);//判断许可ID是否存在
		bool dynamicCheck();//动态校验，成功返回true，失败返回false
		int rightVal(int idx);//输入int值，根据固定的原则返回int值

	private:
		VirBoxInstance();
		~VirBoxInstance();
		void zhou_decrypt(int d, int C1, int C2, int* M);//周氏解密函数
		bool writeToRAWAndEncrypt(const QString& qStr/*需要写入的字符串,注意：不支持中文，不能含有字符："#"，#用于进行字符截止判断*/);//将数据写入到通用许可读写区
		QString readFromRAWAndDecrypt();//从通用许可读写区读取数据，读取的读写区是以 # 分隔符之前的字符

	private:
		bool               _isInitial;/*是否已经进行了初始化*/
		unsigned int       _general_license_id;/*通用许可ID*/
		unsigned int       _general_slm_handle;/*通用许可ID句柄*/
		int                _timeout;//深思许可会话超时时间
		double             _ROM_number;//只读区-数字

		std::vector<int>   _randomVec;//随机数容器
		std::vector<int>   _romIntVec;//随机数容器
		std::map<unsigned int, unsigned int> _subModuleMap;//子模块检测在线情况，按照6分钟
	};
}
#define VirboxInstance AboutSoftWare::VirBoxInstance::getInstance()