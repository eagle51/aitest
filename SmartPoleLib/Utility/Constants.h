#pragma once
namespace SmartPoleCore
{
	struct Constants
	{//用来存储常数，和转化
		static const double PI;
		//角度误差
		static const double ANGULAR_EPSILON;
		static const double EPSILON0_1;

		static const double DISTANCE_EPSILON;
		static const double DISTANCE_EPSILON0_001;
		static const double COINCIDENT_EPSILON;
		//极大错误值
		static const double MAX_EXCEPTION_VALUE;
		//尺度误差
		static const double SCALAR_EPSILON;
		//系统最大点号
		static const int SysMaxTagNum;
		//50000+认为是某个坐标相同
		static const int CoorEqualNum;
		//荷载误差单位N
		static const double FORCE_EPSION;
		//10负6次方
		static const double POW10_6;
		//10负9次方
		static const double POW10_9;

		static const double gammaG;//永久荷载的分项系数
		static const double gammaQ;//可变荷载的分项系数
		static const double SteelDensity;//钢筋密度
		static const double g;//重力

		static bool is_zero(double x)
		{
			return x<SCALAR_EPSILON && x>-SCALAR_EPSILON;
		}

		static double AngleT3oArc(double val)
		{
			return val*PI / 180.0;
		}
		static double ArcToAngle(double val)
		{
			return val*180.0 / PI;
		}
		//3s风速转化为10min风速的转化系数1.43//根据6-15号，的要求，美国标准无论是否英制都是3s分数
		static const double kVelocity3secTo10min;
		//1迈转化为0.45m/s
		static const double mphTOms;
		//1psf=47.8pa
		static const double psfTOpa;
		//摄氏温度转化为华氏温度
		static double ThermometerToFahrenheit(double Thermometer)
		{
			return Thermometer*1.8 + 32;
		}
		//摄氏温度转化为华氏温度
		static double FahrenheitToThermometer(double Fahrenheit)
		{
			return (Fahrenheit - 32)*0.55556;
		}
		//用于基础力(BataZ-1)/2 +1
		static double FdtBataZCalc(double BataZ)
		{
			return ((BataZ - 1.0) / 2.0 + 1.0);
		}

	};
}

