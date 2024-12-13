/****************************************************************/
//Author:   马梓淇
//Version:  1.0
//Date:		2022.10.17
//Description:	软件授权验证管理		
/****************************************************************/
#pragma once
#include <QThread>
#include <QWidget>
#include <QDateTime>
#include "VirBox\VirBoxVerify.h"

namespace AboutSoftWare
{
	class SoftWareVerify : public QThread
	{
		Q_OBJECT
	public:
		static SoftWareVerify* getInstance();
		SoftWare::Type getSoftWareType() const { return _softWareType; }
		Year::Type getYearType() const { return _yearType; }

		//初始设置软件和年份信息
		void setSoftwareInfo0(SoftWare::Type, Year::Type);
		void setIsCheckRegedit0(bool val) { _isCheckRegedit = val; }
		void setIsCheckConfig0(bool val) { _isCheckConfig = val; }
		void setOverTimeStep0(int iYear, int iMonth, int iDay);
		bool checkVirBoxInitStep1();//检查深思加密初始化，成功返回true，失败返回false
		bool checkVirBoxLicenseIDStep2();//检查深思加密许可，成功返回true，失败返回false
		////设置窗体是否无效，并进行相关提示,狗失效就提示
		void checkVirBoxLicenseROM3();//检查深思加密许可读写区,！！！！这个必须在多线程启动之前执行！！！
		bool hasLicense_id4(Module::Type moduleType);//判断子模块是否存在
		bool checkRegedit5(QWidget *wgt/*待处理窗体*/);//检查注册表，成功返回true，失败返回false
		bool CheckConfig5(QWidget *wgt/*待处理窗体*/);//检查配置文件，成功返回true，失败返回false
		void genetateConfigFile();//生成配置文件，用于软件版本更新时调用，生成本地用于检测授权的配置文件，正式发布时不应该调用该方法
		void logCleanEnd999();//退出调用，要清楚

	signals:
		void emitVerifyResToWgt(bool enable/*验证通过为true 不通过为False*/);//发送信号到窗体
	protected:
		virtual void run();//多线程运行，动态校验

	private:
		SoftWareVerify();
		~SoftWareVerify();

		/*模块加密授权相关*/
		QString getRegeditKey_definiteTimes();//注册表写入键名称：过期标志
		QString getRegeditKey_maxTimeDate();//注册表写入键名称：最近的时间

		QDate configEncryptDate(QWidget *wgt/*待处理窗体*/, const QString& key, const QByteArray& salt);//配置文件加密日期

#ifdef _UseVirBoxEncrypt
		unsigned int genetateId(Module::Type moduleType){ return _softWareType * 100000 + moduleType * 100 + _yearType; }//生成加密码ID
#else
		unsigned int genetateId(Module::Type moduleType){ return _softWareType; }//生成加密码ID
#endif

	private:
		SoftWare::Type           _softWareType;//软件类型
		Year::Type               _yearType;//软件年份
		int                      _allModuleFlag;

		bool                     _isCheckRegedit;//是否查验注册表加密过期，默认为false
		bool                     _isCheckConfig;//是否查验配置文件加密过期，默认为false
		QDate                    _overTime;//软件过期时间，仅用于注册表以及配置文件授权检查，对深思加密无效，格式要求：XXXX-XX-XX，必须按照此格式填入，例如2022-10-09，代表2022年10月9日
	};
}
#define SoftWareVerifyInstance AboutSoftWare::SoftWareVerify::getInstance()