#pragma once
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
struct Constants
{
	static double loadFactor;
	static const double PI;
	//角度误差 
	static const double ANGULAR_EPSILON;
	static const double EPSILON0_1;
	//距离误差
	static const double DISTANCE_EPSILON0_001;
	static const double DISTANCE_EPSILON;
	static const double DISTANCE_EPSILON0_01;
	//共面，节点重合误差0.002
	static const double COINCIDENT_EPSILON;
	//共面，节点重合误差平方
	static const double COINCIDENT2_EPSILON;
	//尺度误差
	static const double SCALAR_EPSILON;
	//夹角限值，大于这个值得与某个坐标轴夹角最小
	static const double ANGULARCOS_EPSILON;
	//水平面误差
	static const double HorizontalPlane_EPSILON;
	//辅助材最小的点好
	static const int MinAuxTagNum;
	//
	static const int MaxAuxTagNum;
	//最大工况上限
	static int LoadCaseCountUpperLimit;
	//荷载误差单位N
	static const double FORCE_EPSION;
	static const double SCALAR9_99;

	inline static bool is_zero(double x)
	{
		return x<SCALAR_EPSILON && x>-SCALAR_EPSILON;
	}
	inline static double AngleT3oArc(double val)
	{
		return val*PI/180.0;
	}
	inline static double ArcToAngle(double val)
	{
		return val*180.0/PI;
	}
	//3s风速转化为10min风速的转化系数1.43//根据6-15号，的要求，美国标准无论是否英制都是3s分数
	static const double kVelocity3secTo10min;
	//1迈转化为0.45m/s
	static const double mphTOms;
	//1psf=47.8pa
	static const double psfTOpa;
	//摄氏温度转化为华氏温度
	inline static double ThermometerToFahrenheit(double Thermometer)
	{
		return Thermometer*1.8+32;
	}
	//摄氏温度转化为华氏温度
	inline static double FahrenheitToThermometer(double Fahrenheit)
	{
		return (Fahrenheit-32)*0.55556;
	}
	//用于基础力(BataZ-1)/2 +1
	inline static double FdtBataZCalc(double BataZ)
	{
		return ((BataZ-1.0)/2.0+1.0);
	}
};

