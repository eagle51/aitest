#include "Auxiliary_AS.h"
#include <fstream>
#include <sstream>
#include <direct.h>
#include <atlconv.h>
#include <locale.h>
#include <exception>

namespace AboutSoftWare
{
	Auxiliary_AS::Auxiliary_AS()
	{
	}
	
	Auxiliary_AS::~Auxiliary_AS()
	{
	}
	
	double Auxiliary_AS::round(double a, int precision)
	{
		double sub1 = std::pow(10.0, precision);
		double sub2 = std::floor(a * sub1 + 0.5);

		return sub2 / sub1;
	}

	//@function: 向上取整   -- by fqw 2019/7/5 9:52
	double Auxiliary_AS::ceiling(double a, int precision)
	{
		double eps = 0.5*std::pow(10.0, -precision);

		return round(a + eps, precision);
	}

	//@function: 向下取整   -- by fqw 2019/7/5 9:52
	double Auxiliary_AS::floor(double a, int precision)
	{
		double eps = 0.5*std::pow(10.0, -precision);

		return round(a - eps, precision);
	}


	int Auxiliary_AS::s2i(const std::string& str)
	{
		std::stringstream ss;
		ss << str;

		int i = 0;
		ss >> i;

		return i;
	}

	double Auxiliary_AS::s2d(const std::string& str)
	{
		std::stringstream ss;
		ss << str;

		double i = 0;
		ss >> i;

		return i;
	}

	bool Auxiliary_AS::s2b(const std::string& str)
	{
		return (0 != s2i(str));
	}
	// 角度转弧度
	double Auxiliary_AS::arc(double angle)
	{
		return angle / 45.0 * atan(1.0);
	}
	// 弧度转角度
	double Auxiliary_AS::angle(double arc)
	{
		return 180 / acos(-1.0)*arc;
	}	

	std::string Auxiliary_AS::i2s(int i)
	{
		std::stringstream ss;
		ss << i;

		return ss.str();
	}
	std::wstring Auxiliary_AS::i2ws(int i)
	{
		std::wstringstream ss;
		ss << i;

		return ss.str();
	}

	std::string Auxiliary_AS::d2s(double i, int precision /*= 0*/)
	{
		i = round(i, precision);
		std::stringstream ss;
		ss << i;

		return ss.str();
	}

	//@function: double to string
	std::string Auxiliary_AS::dtos(double i)
	{
		return std::to_string(i);
	}


	//@function: 多字节转utf-8
	void Auxiliary_AS::MBSToUTF8(char * utf8, int size, const char* mbs)
	{
		if (!utf8 || !mbs) return; UINT nACP = GetACP();
		int dwNum = MultiByteToWideChar(nACP, 0, mbs, -1, NULL, 0); // CP_ACP  
		if (dwNum <= 0)
			return;
		wchar_t* pwText = NULL;
		pwText = new wchar_t[dwNum + 1];
		if (!pwText)
		{
			delete[] pwText;
			return;
		}
		memset(pwText, 0, dwNum * 2 + 2);
		if (MultiByteToWideChar(nACP, 0, mbs, -1, pwText, dwNum) == 0) // CP_ACP  
		{
			delete[] pwText;
			return;
		}
		int dwCount = WideCharToMultiByte(CP_UTF8, 0, pwText, -1, NULL, 0, NULL, NULL);
		if (dwCount <= 0)
		{
			delete[] pwText;
			return;
		}
		if (size < dwCount + 1)
		{
			delete[] pwText;
			return;
		}
		memset(utf8, 0, dwCount + 1);
		if (WideCharToMultiByte(CP_UTF8, 0, pwText, -1, utf8, dwCount, NULL, NULL) == 0)
		{
			delete[] pwText;
			return;
		}
		delete[] pwText;
	}

	//@function: utf-8转多字节
	void Auxiliary_AS::UTF8ToMBS(char * mbs, int size, const char* utf8)
	{
		if (!utf8 || !mbs)
			return;
		int dwNum = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		if (dwNum <= 0)
			return;
		wchar_t* pwText = NULL;
		pwText = new wchar_t[dwNum + 1];
		if (!pwText)
		{
			delete[] pwText;
			return;
		}
		memset(pwText, 0, dwNum * 2 + 2);
		if (MultiByteToWideChar(CP_UTF8, 0, utf8, -1, pwText, dwNum) == 0)
		{
			delete[] pwText;
			return;
		}
		UINT nACP = GetACP();
		int dwCount = WideCharToMultiByte(nACP, 0, pwText, -1, NULL, 0, NULL, NULL); // CP_ACP  
		if (dwCount <= 0)
		{
			delete[] pwText;
			return;
		}
		if (size < dwCount + 1)
		{
			delete[] pwText;
			return;
		}
		memset(mbs, 0, dwCount + 1);
		if (WideCharToMultiByte(nACP, 0, pwText, -1, mbs, dwCount, NULL, NULL) == 0) // CP_ACP  
		{
			delete[] pwText;
			return;
		}
		delete[] pwText;
	}

	// @ function: 度分秒转化为度   -- by WD 2019/9/3 11:19
	double Auxiliary_AS::degMinSec2Deg(double angle)
	{
		return std::floor(angle) + (angle - std::floor(angle)) * 100 / 60 ;
	}
	//@function: 将宽字符编码字符串转换成多字节编码字符串(wchar* 转 char*)   -- by fqw 2019/9/25 17:57
	std::string Auxiliary_AS::wcs2mbs_s(const wchar_t* pws)
	{
		size_t len = 100;
		size_t converted = 0;
		char* pc = new char[len];
		wcstombs_s(&converted, pc, len, pws, _TRUNCATE); // https://www.jianshu.com/p/cc97a2b34c55
		std::string res(pc);
		delete pc;

		return res;
	}

	//@function: 将多字节编码字符串转换成宽字符编码字符串(char* 转 wchar*)(堆上数据需处理delete)   -- by fqw 2019/9/25 18:00
	std::wstring Auxiliary_AS::mbs2wcs_s(const char* pc)
	{
		size_t len = 100;
		size_t converted = 0;
		wchar_t* pws = new wchar_t[len];
		mbstowcs_s(&converted, pws, len, pc, _TRUNCATE);
		std::wstring res(pws);
		delete pws;

		return res;
	}

	//@function:    -- by fqw 2019/9/25 18:10
	std::string Auxiliary_AS::w2mbs_s(std::wstring str)
	{
		size_t len = 100;
		wchar_t *strTemp = new wchar_t[len];
		memmove_s(strTemp, len, str.c_str(), len);
		std::string strDes = std::string(Auxiliary_AS::wcs2mbs_s(strTemp));
		delete strTemp;

		return strDes;
	}
	// @ function:    -- by WD 2019/9/25 18:45
	std::string Auxiliary_AS::Wchar_tToString(const wchar_t *wchar)
	{
		wchar_t * wText = const_cast<wchar_t*>(wchar);
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
		
		char *psText = new char[dwNum];// psText为char*的临时数组，作为赋值给std::string的中间变量
		WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
		std::string szDst(psText);
		delete[]psText;// psText的清除

		return szDst;
	}
	// @ function:    -- by WD 2019/10/24 20:37
	std::wstring Auxiliary_AS::str2wstr(std::string str)
	{
		int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t *wide = new wchar_t[num];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
		std::wstring w_str(wide);
		delete[] wide;
		return w_str;
	}
}
