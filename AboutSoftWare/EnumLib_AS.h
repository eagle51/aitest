#pragma once
#include <vector>
#include "QString"

//#define _NotUse3rdEncrypt //�ú궨������������õ��������ܣ�����˼����ʥ��ŵ��
#define  _UseVirBoxEncrypt //�ú궨���������ʹ����˼���ܣ������������ʹ��ʥ��ŵ���ܣ�������Ŀ�����������

namespace AboutSoftWare
{

	//�������
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
		static QString typeToFullStr(Type curType);//����תȫ��
		static QString typeToENStr(Type curType);//����תӢ������
		static std::vector<SoftWare::Type> getAllTypeVec();//��ȡ����ö������
	};

	//ģ������
	class Module
	{
	public:
		enum Type
		{
			/*ͨ��*/
			General = 999,//999 ÿһ���������һ����Ӧ��ͨ�õ�ģ�飬������Ǽ��ܡ��������Ȩ��顢Ҳ���������һЩͨ���ԵĹ���
			All = 0,//000 ���������ȫ��ģ�鹦��

		};

		static QString typeToStr(Type curType);
		static Type strToType(const QString& curTypeStr);
		static std::vector<Module::Type> getAllTypeVec();//��ȡ����ö������
	};

	//���
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

	//�汾����
	class SoftwareType_AS
	{
	public:
		enum Type
		{
			General = 0,//ͨ�ð�
			FoShanYuan,//��ɽԺ
		};
		static QString typeToStr(Type curType);
		static Type strToType(const QString& curTypeStr);
	};
}