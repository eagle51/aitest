#include "SoftWareBaseInfo.h"

namespace AboutSoftWare
{
	/**********************************分割线****************************************/
	// @ function:    -- by mzq 2020/02/06 18:10
	SoftWareBaseInfo::SoftWareBaseInfo()
		:_softWareName(""),
		_version("V1.0.0"),
		_versionTime("2020-01-01"),
		_developer(""),
		_telephone(""),
		_QQ(""),
		_EMail(""),
		_website(""),
		_companyName_CHS(""),
		_companyName_EN(""),
		_versionType(SoftwareType_AS::Type::General)
	{

	}

	// @ function: 读取Json格式数据    -- by mzq 2020/02/06 18:10
	void SoftWareBaseInfo::readFromJson(const QJsonObject& root)
	{
		_softWareName = root[QStringLiteral("00软件名称")].toString();
		_version = root[QStringLiteral("01版本信息")].toString();
		_versionTime = root[QStringLiteral("02版本时间")].toString();
		_developer = root[QStringLiteral("03开发人员")].toString();
		_telephone = root[QStringLiteral("04联系电话")].toString();
		_QQ = root[QStringLiteral("05QQ")].toString();
		_EMail = root[QStringLiteral("06邮箱")].toString();
		_website = root[QStringLiteral("07公司网址")].toString();
		_companyName_CHS = root[QStringLiteral("08公司名称_中文")].toString();
		_companyName_EN = root[QStringLiteral("09公司名称_英文")].toString();
		_url = root[QStringLiteral("10Url")].toString();
		_versionType = SoftwareType_AS::strToType(root[QStringLiteral("20版本类型")].toString());
	}

	//@function: 获取版本时间，年   -- by mzq  2020/5/6 15:27
	int SoftWareBaseInfo::getVersionTime_year()
	{
		return _versionTime.mid(0, 4).toInt();
	}

	//@function: 获取版本时间，月   -- by mzq  2020/5/6 15:28
	int SoftWareBaseInfo::getVersionTime_month()
	{
		return _versionTime.mid(5, 2).toInt();
	}

	//@function: 获取版本时间，日   -- by mzq  2020/5/6 15:28
	int SoftWareBaseInfo::getVersionTime_day()
	{
		return _versionTime.mid(8, 2).toInt();
	}
}