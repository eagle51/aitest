#include "Auxiliary.h"
#include "Constants.h"
#include <sstream>
#include <wtypes.h>
#include <tchar.h>
#include <direct.h>
#include <atlconv.h>
#include <iosfwd>
namespace SmartPoleCore{
	Auxiliary::Auxiliary()
	{
	}


	Auxiliary::~Auxiliary()
	{
	}

 	string Auxiliary::itos(int i)
 	{
 		stringstream ss;
 		ss << i;
 		return ss.str();
 	}

	string Auxiliary::dtos(double i)
	{
		stringstream ss;
		ss << i;
		return ss.str();
	}

	double Auxiliary::Round(double a, int precision)
	{
		return (floor(a * pow(10.0, precision) + 0.5)) / pow(10.0, precision);
	}

	int Auxiliary::Round(double a)
	{
		return (int)(a + 0.5);
	}

	double Auxiliary::Sign(double a)
	{
		if (a > 0)return 1;
		if (a < 0)return -1;
		return 0;
	}

	bool Auxiliary::isEqual(double a, double b)
	{
		return (abs(a - b) <= Constants::SCALAR_EPSILON);
	}

	double Auxiliary::minDoubleVal()
	{
		return 0.0005;
	}

 	int Auxiliary::stoi(const string& str)
 	{
 		stringstream ss;
 		ss << str;
 		int i = 0;
 		ss >> i;
 		return i;
 	}

 	double Auxiliary::stod(const string& str)
 	{
 		stringstream ss;
 		ss << str;
 		double i = 0;
 		ss >> i;
 		return i;
 	}

	string& Auxiliary::replace_all(string& str, const string& old_val, const string& new_val)
	{
		for (string::size_type pos(0); pos != string::npos; pos += new_val.length())   {
			if ((pos = str.find(old_val, pos)) != string::npos)
				str.replace(pos, old_val.length(), new_val);
			else   break;
		}
		return   str;
	}

// 	bool Auxiliary::isSyetemFile(const char* path)
// 	{
// 		char systemPath[1000];
// 		_getcwd(systemPath, sizeof(systemPath));
// 		string strSysPath = systemPath;
// 		Auxiliary::replace_all(strSysPath, "\\", "/");
// 		if (string(path).find(strSysPath) != string::npos)
// 			return true;
// 		return false;
// 	}

	double Auxiliary::arc(double angle)
	{
		return angle / 45 * atan(1.0);
	}

	double Auxiliary::getArea(double diamter)
	{
		return Constants::PI * diamter * diamter / 4;
	}

	double Auxiliary::getAnnulusArea(double diamter,double thick)
	{
		//计算圆环面积，传参--外径、壁厚
		return Constants::PI * (pow(diamter*0.5, 2) - pow(((diamter - thick*2.0)*0.5), 2));
	}

	bool Auxiliary::isLessOrEqual(double a, double b)
	{
		return ((a < b) || (abs(a - b) <= Constants::SCALAR_EPSILON));
	}

	bool Auxiliary::isMoreOrEqual(double a, double b)
	{
		return ((a > b) || (abs(a - b) <= Constants::SCALAR_EPSILON));
	}

	//@function: 多字节转utf-8
	void Auxiliary::MBSToUTF8(char * utf8, int size, const char* mbs)
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
	void Auxiliary::UTF8ToMBS(char * mbs, int size, const char* utf8)
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

	void Auxiliary::unicodeToUTF8(const std::wstring &src, std::string& result)
	{
// 		int n = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0);
// 		result.resize(n);
// 		::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, (char*)result.c_str(), result.length(), 0, 0);
		char*  pElementText;
		int    iTextLen;
		// wide char to multi char
		iTextLen = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
		pElementText = new char[iTextLen + 1];
		memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
		::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, pElementText, iTextLen, NULL, NULL);
		result = pElementText;
		delete[] pElementText;
	}

	void Auxiliary::gb2312ToUnicode(const std::string& src, std::wstring& result)
	{
		int n = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
		result.resize(n);
		::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
	}

	void Auxiliary::gb2312ToUtf8(const std::string& src, std::string& result)
	{
		std::wstring strWideChar;
		gb2312ToUnicode(src, strWideChar);
		unicodeToUTF8(strWideChar, result);
	}
	// @ function:   --- by lsq 2019/6/19 17:44
	double Auxiliary::CEILING(double number, double significance)
	{
		return significance * ceil(number / significance);
	}

	bool Auxiliary::cmp(const pair<int, double> &p1, const pair<int, double> &p2)//要用常数，不然编译错误 
	{
		return p1.second < p2.second;
	}

	bool Auxiliary::cmpMod(const pair<int, double> &p1, const pair<int, double> &p2)//要用常数，不然编译错误，输出三维Mod用 
	{
		return p1.second > p2.second;
	}

	//字符串分割
	vector<string> Auxiliary::splitString(string str, char del) //忽略“空位”（::192:168:ABC::416）->（192 168 ABC 416）
	{
		stringstream ss(str);
		string tok;
		vector<string> ret;
		while (getline(ss, tok, del))
		{
			if (tok > "")
				ret.push_back(tok);
		}
		return ret;
	};
	// @ function:   --- by lsq 2019/12/2 16:37
	double Auxiliary::calc2NodeLength(double x1, double y1, double z1, double x2, double y2, double z2)
	{
		double dx = x1 - x2;
		double dy = y1 - y2;
		double dz = z1 - z2;
		return sqrt(dx*dx + dy*dy + dz*dz);
	}

	double Auxiliary::vectordoubleSum(vector<double>::iterator first, vector<double>::size_type size)
	{
		double sum = 0.0;
		for (int ix = 0; ix != size; ++ix){
			sum += *first++;
		}
		return sum;
	}
	// @ function:   --- by lsq 2020/10/10 18:05
	std::string Auxiliary::d2s(double i, int precision /*= 5*/)
	{
		i = Round(i, precision);
		std::stringstream ss;
		ss << i;

		return ss.str();
	}

}
