#pragma once
namespace SmartPoleCore
{
	struct Constants
	{//�����洢��������ת��
		static const double PI;
		//�Ƕ����
		static const double ANGULAR_EPSILON;
		static const double EPSILON0_1;

		static const double DISTANCE_EPSILON;
		static const double DISTANCE_EPSILON0_001;
		static const double COINCIDENT_EPSILON;
		//�������ֵ
		static const double MAX_EXCEPTION_VALUE;
		//�߶����
		static const double SCALAR_EPSILON;
		//ϵͳ�����
		static const int SysMaxTagNum;
		//50000+��Ϊ��ĳ��������ͬ
		static const int CoorEqualNum;
		//������λN
		static const double FORCE_EPSION;
		//10��6�η�
		static const double POW10_6;
		//10��9�η�
		static const double POW10_9;

		static const double gammaG;//���ú��صķ���ϵ��
		static const double gammaQ;//�ɱ���صķ���ϵ��
		static const double SteelDensity;//�ֽ��ܶ�
		static const double g;//����

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
		//3s����ת��Ϊ10min���ٵ�ת��ϵ��1.43//����6-15�ţ���Ҫ��������׼�����Ƿ�Ӣ�ƶ���3s����
		static const double kVelocity3secTo10min;
		//1��ת��Ϊ0.45m/s
		static const double mphTOms;
		//1psf=47.8pa
		static const double psfTOpa;
		//�����¶�ת��Ϊ�����¶�
		static double ThermometerToFahrenheit(double Thermometer)
		{
			return Thermometer*1.8 + 32;
		}
		//�����¶�ת��Ϊ�����¶�
		static double FahrenheitToThermometer(double Fahrenheit)
		{
			return (Fahrenheit - 32)*0.55556;
		}
		//���ڻ�����(BataZ-1)/2 +1
		static double FdtBataZCalc(double BataZ)
		{
			return ((BataZ - 1.0) / 2.0 + 1.0);
		}

	};
}

