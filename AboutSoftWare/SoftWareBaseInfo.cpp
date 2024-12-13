#include "SoftWareBaseInfo.h"

namespace AboutSoftWare
{
	/**********************************�ָ���****************************************/
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

	// @ function: ��ȡJson��ʽ����    -- by mzq 2020/02/06 18:10
	void SoftWareBaseInfo::readFromJson(const QJsonObject& root)
	{
		_softWareName = root[QStringLiteral("00�������")].toString();
		_version = root[QStringLiteral("01�汾��Ϣ")].toString();
		_versionTime = root[QStringLiteral("02�汾ʱ��")].toString();
		_developer = root[QStringLiteral("03������Ա")].toString();
		_telephone = root[QStringLiteral("04��ϵ�绰")].toString();
		_QQ = root[QStringLiteral("05QQ")].toString();
		_EMail = root[QStringLiteral("06����")].toString();
		_website = root[QStringLiteral("07��˾��ַ")].toString();
		_companyName_CHS = root[QStringLiteral("08��˾����_����")].toString();
		_companyName_EN = root[QStringLiteral("09��˾����_Ӣ��")].toString();
		_url = root[QStringLiteral("10Url")].toString();
		_versionType = SoftwareType_AS::strToType(root[QStringLiteral("20�汾����")].toString());
	}

	//@function: ��ȡ�汾ʱ�䣬��   -- by mzq  2020/5/6 15:27
	int SoftWareBaseInfo::getVersionTime_year()
	{
		return _versionTime.mid(0, 4).toInt();
	}

	//@function: ��ȡ�汾ʱ�䣬��   -- by mzq  2020/5/6 15:28
	int SoftWareBaseInfo::getVersionTime_month()
	{
		return _versionTime.mid(5, 2).toInt();
	}

	//@function: ��ȡ�汾ʱ�䣬��   -- by mzq  2020/5/6 15:28
	int SoftWareBaseInfo::getVersionTime_day()
	{
		return _versionTime.mid(8, 2).toInt();
	}
}