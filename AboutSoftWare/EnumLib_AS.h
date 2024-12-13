#pragma once
#include <vector>
#include "QString"

//#define _NotUse3rdEncrypt //该宏定义了则代表不采用第三方加密，如深思或者圣天诺等
#define  _UseVirBoxEncrypt //该宏定义了则代表使用深思加密，不定义则代表使用圣天诺加密，根据项目情况按需设置

namespace AboutSoftWare
{

	//软件类型
	class SoftWare
	{
	public:
		enum Type
		{
#ifdef _UseVirBoxEncrypt

			SmartTower = 101,
			SmartLoad = 102,
			SmartFoundation = 103,
			SmartTerrain = 105,
			SmartCS = 106,
			SmartPole = 107,
			SmartCPD = 108,

			SmartCADD = 201,
			SmartETools = 202,
			SmartHeavyIcing = 203,
			SmartClearance = 204,
			SmartJumper = 205,
			SmartEM = 206,
			SmartEP = 207,

			SmartQuery = 301,
			SmartFR = 302,

			UndefineSoftWare = 998,

#else

			SmartTower = 1984,
			SmartLoad = 1987,
			SmartFoundation = 2017,
			SmartTerrain = 1986,
			SmartCS = 20189,
			SmartPole = 1993,
			SmartCPD = 9001,

			SmartCADD = 2020,
			SmartETools = 7001,
			SmartHeavyIcing = 6001,
			SmartClearance = 19941,
			SmartJumper = 1989,
			SmartEM = 1994,
			SmartEP = 4001,

			SmartQuery = 1995,
			SmartFR = 8001,

			UndefineSoftWare = 20158,

#endif
		};
		static QString typeToFullStr(Type curType);//类型转全称
		static QString typeToENStr(Type curType);//类型转英文名称
		static std::vector<SoftWare::Type> getAllTypeVec();//获取所有枚举类型
	};

	//模块类型
	class Module
	{
	public:
		enum Type
		{
			/*通用*/
			General = 999,//999 每一个软件都有一个对应的通用的模块，用于外壳加密、软件的授权检查、也代表软件的一些通用性的功能
			All = 0,//000 代表软件的全部模块功能

		};

		static QString typeToStr(Type curType);
		static Type strToType(const QString& curTypeStr);
		static std::vector<Module::Type> getAllTypeVec();//获取所有枚举类型
	};

	//年份
	class Year
	{
	public:
		enum Type
		{
			Year_2022 = 22,
			Year_2023 = 23,
			Year_2024 = 24,
			Year_2025 = 25,
			Year_2026 = 26,
			Year_2027 = 27,
			Year_2028 = 28,
			Year_2029 = 29,
			Year_2030 = 30,
		};

		static QString typeToStr(Type curType);
	};

	//版本类型
	class SoftwareType_AS
	{
	public:
		enum Type
		{
			General = 0,//通用版
			FoShanYuan,//佛山院
		};
		static QString typeToStr(Type curType);
		static Type strToType(const QString& curTypeStr);
	};
}