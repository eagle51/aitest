#pragma once
#include <string>
#include <vector>
#include <complex>

namespace AboutSoftWare
{
	class  Auxiliary_AS
	{
	public:
		Auxiliary_AS();
		~Auxiliary_AS();

		static std::string i2s(int i);
		static std::wstring i2ws(int i);
		static int s2i(const std::string& str);

		static std::string d2s(double i,
						  int precision = 5);

		static std::string dtos(double i);

		static double s2d(const std::string& str);
		static bool s2b(const std::string& str);
		
		static double round(double a, 
							int precision);

		static double ceiling(double a,
							int precision); // ����ȡ��

		static double floor(double a,
						   int precision); // ����ȡ��

		static double arc(double angle);
		static double angle(double arc);
		static double degMinSec2Deg(double angle);

		static void MBSToUTF8(char * utf8, int size, const char* mbs); // ���ֽ�תutf-8
		static void UTF8ToMBS(char * mbs, int size, const char* utf8); // utf-8ת���ֽ�
		
		static std::string wcs2mbs_s(const wchar_t* pws); // �����ַ������ַ���ת���ɶ��ֽڱ����ַ���(wchar* ת char*)
		static std::wstring mbs2wcs_s(const char* pc); // �����ֽڱ����ַ���ת���ɿ��ַ������ַ���(char* ת wchar*)

		static std::string w2mbs_s(std::wstring str);
		static std::string Wchar_tToString(const wchar_t *wchar);

		static std::wstring str2wstr(std::string str);

	};
}
 