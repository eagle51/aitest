#include "EnumLib_AS.h"

namespace AboutSoftWare
{
	//@function:  ����תȫ��  -- by mzq 2021/12/2 14:41
	QString SoftWare::typeToFullStr(Type curType)
	{
		switch (curType)
		{
		case SmartTower:
			return QStringLiteral("SmartTower����������");
		case SmartLoad:
			return QStringLiteral("SmartLoad�������ؼ������");
		case SmartFoundation:
			return QStringLiteral("SmartFoundation����������");
		case SmartTerrain:
			return QStringLiteral("SmartTerrain�����������");
		case SmartCS:
			return QStringLiteral("SmartCS���½ṹ������");
		case SmartPole:
			return QStringLiteral("SmartPole�ֹܸ�������");
		case SmartCPD:
			return QStringLiteral("SmartCPD����ƽ����������");

		case SmartCADD:
			return QStringLiteral("SmartCADD����ȫ����������");
		case SmartETools:
			return QStringLiteral("SmartETools���������ù������");
		case SmartHeavyIcing:
			return QStringLiteral("SmartHeavyIcing���ظ����������");
		case SmartClearance:
			return QStringLiteral("SmartClearance��϶Բ�������");
		case SmartJumper:
			return QStringLiteral("SmartJumper���߼������");
		case SmartEM:
			return QStringLiteral("SmartEM����ѡ�ͼ���ŷ������");
		case SmartEP:
			return QStringLiteral("SmartEP���������������");

		case SmartQuery:
			return QStringLiteral("SmartQuery�������ݲ�ѯ���");
		case SmartFR:
			return QStringLiteral("SmartFRͼֽ�鵵���");
		default:
			return "";
		}
	}

	//@function: ����תӢ������   -- by mzq 2022/10/21 20:29
	QString SoftWare::typeToENStr(Type curType)
	{
		switch (curType)
		{
		case SmartTower:
			return "SmartTower";
		case SmartLoad:
			return "SmartLoad";
		case SmartFoundation:
			return "SmartFoundation";
		case SmartTerrain:
			return "SmartTerrain";
		case SmartCS:
			return "SmartCS";
		case SmartPole:
			return "SmartPole";
		case SmartCPD:
			return "SmartCPD";

		case SmartCADD:
			return "SmartCADD";
		case SmartETools:
			return "SmartETools";
		case SmartHeavyIcing:
			return "SmartHeavyIcing";
		case SmartClearance:
			return "SmartClearance";
		case SmartJumper:
			return "SmartJumper";
		case SmartEM:
			return "SmartEM";
		case SmartEP:
			return "SmartEP";

		case SmartQuery:
			return "SmartQuery";
		case SmartFR:
			return "SmartFR";
		default:
			return "";
		}
	}

	//@function: ��ȡ����ö������   -- by mzq 2022/5/20 11:35
	std::vector<AboutSoftWare::SoftWare::Type> SoftWare::getAllTypeVec()
	{
		std::vector<SoftWare::Type> types
		{
		SoftWare::SmartTower,
		SoftWare::SmartLoad,
		SoftWare::SmartFoundation,
		SoftWare::SmartTerrain,
		SoftWare::SmartCS,
		SoftWare::SmartPole,
		SoftWare::SmartCPD,

		SoftWare::SmartCADD,
		SoftWare::SmartETools,
		SoftWare::SmartHeavyIcing,
		SoftWare::SmartClearance,
		SoftWare::SmartJumper,
		SoftWare::SmartEM,
		SoftWare::SmartEP,

		SoftWare::SmartQuery,
		SoftWare::SmartFR
		};
		return types;
	}

	//@function:    -- by mzq 2022/11/8 14:46
	QString Module::typeToStr(Type curType)
	{
		switch (curType)
		{
		case General:
			return QStringLiteral("ͨ��");
		case All:
			return QStringLiteral("ȫ��");
		default:
			return QStringLiteral("");
		}
	}

	//@function:    -- by mzq 2022/11/8 14:46
	AboutSoftWare::Module::Type Module::strToType(const QString& curTypeStr)
	{
		if (curTypeStr == QStringLiteral("ͨ��"))
			return General;
		else if (curTypeStr == QStringLiteral("ȫ��"))
			return All;
		return General;
	}

	//@function: ��ȡ����ö������   -- by mzq 2022/11/16 10:15
	std::vector<Module::Type> Module::getAllTypeVec()
	{
		std::vector<Module::Type> types
		{
		Module::All,
		Module::General,
	};
		return types;
	}


	//@function:    -- by mzq 2022/10/17 9:50
	QString Year::typeToStr(Type curType)
	{
		switch (curType)
		{
		case Year_2022:
			return "2022";
		case Year_2023:
			return "2023";
		case Year_2024:
			return "2024";
		case Year_2025:
			return "2025";
		case Year_2026:
			return "2026";
		case Year_2027:
			return "2027";
		case Year_2028:
			return "2028";
		case Year_2029:
			return "2029";
		case Year_2030:
			return "2030";
		default:
			return "";
		}
	}

	//@function:    -- by mzq 2022/11/16 16:12
	QString SoftwareType_AS::typeToStr(Type curType)
	{
		switch (curType)
		{
		case General:
			return QStringLiteral("General");
		case FoShanYuan:
			return QStringLiteral("FoShanYuan");
		default:
			return QStringLiteral("General");
		}
	}

	//@function:    -- by mzq 2022/11/16 16:12
	SoftwareType_AS::Type SoftwareType_AS::strToType(const QString& curTypeStr)
	{
		if (curTypeStr == QStringLiteral("General"))
			return General;
		else if (curTypeStr == QStringLiteral("FoShanYuan"))
			return FoShanYuan;
		return General;
	}
}