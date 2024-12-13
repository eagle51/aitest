/****************************************************************/
//Author:   马梓淇
//Version:  1.0
//Date:		2022.5.17
//Description:	软件基本信息		
/****************************************************************/
#pragma once
#include "SharedObject_AS.h"
#include "StHandle_AS.hpp"
#include "EnumLib_AS.h"

#include <QString>
#include <QJsonObject>

namespace AboutSoftWare
{
	//软件基本信息
	class SoftWareBaseInfo : public SharedObject_AS
	{
	public:
		SoftWareBaseInfo();
		~SoftWareBaseInfo() {};

		const QString& getSoftWareName() const { return _softWareName; }
		const QString& getVersion() const { return _version; }
		const QString& getVersionTime() const { return _versionTime; }
		const QString& getDeveloper() const { return _developer; }
		const QString& getTelephone() const { return _telephone; }
		const QString& getQQ() const { return _QQ; }
		const QString& getEMail() const { return _EMail; }
		const QString& getWebsite() const { return _website; }
		const QString& getCompanyName_CHS() const { return _companyName_CHS; }
		const QString& getCompanyName_EN() const { return _companyName_EN; }
		const QString& getUrl() const { return _url; }
		SoftwareType_AS::Type getSoftwareType_AS() const { return _versionType; }

		int getVersionTime_year();//获取版本时间，年
		int getVersionTime_month();//获取版本时间，月
		int getVersionTime_day();//获取版本时间，日

		void setSoftWareName(const QString& val) { _softWareName = val; }
		void setVersion(const QString& val) { _version = val; }
		void setVersionTime(const QString& val) { _versionTime = val; }
		void setDeveloper(const QString& val) { _developer = val; }
		void setTelephone(const QString& val) { _telephone = val; }
		void setQQ(const QString& val) { _QQ = val; }
		void setEMail(const QString& val) { _EMail = val; }
		void setWebsite(const QString& val) { _website = val; }
		void setCompanyName_CHS(const QString& val) { _companyName_CHS = val; }
		void setCompanyName_EN(const QString& val) { _companyName_EN = val; }
		void setUrl(const QString& val) { _url = val; }
		void setSoftwareType_AS(SoftwareType_AS::Type val) { _versionType = val; }

	public:
		void readFromJson(const QJsonObject& root);

	private:
		QString                     _softWareName;//软件名称
		QString                     _version;//版本信息
		QString						_versionTime;// 版本时间 格式要求：XXXX-XX-XX，必须按照此格式填入，例如2022-10-09，代表2022年10月9日
		QString					    _developer;// 开发人员
		QString						_telephone;// 联系电话
		QString						_QQ;// QQ
		QString						_EMail;// 邮箱
		QString                     _website;//网址
		QString                     _companyName_CHS;//公司名称_中文
		QString                     _companyName_EN;//公司名称_英文
		QString                     _url;//文件url地址
		SoftwareType_AS::Type       _versionType;//版本类型
	};
	typedef StHandle_AS<SoftWareBaseInfo> HdlSoftWareBaseInfo;
}