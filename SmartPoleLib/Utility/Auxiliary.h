#pragma once
#include <string>
#include <vector>
#include<iostream>

using namespace std;
namespace SmartPoleCore{

	class Auxiliary
	{
	public:
		Auxiliary();
		~Auxiliary();
// 		static string itos(int i);
// 		static int stoi(const string& str);
// 		static string dtos(double i);
// 		static double stod(const string& str);
		static double Round(double a, int precision);
		static double calc2NodeLength(double x1, double y1, double z1, double x2, double y2, double z2);

		static int Round(double a);
		static double Sign(double a);
		static bool isEqual(double a, double b);
		static bool isLessOrEqual(double a, double b);
		static bool isMoreOrEqual(double a, double b);
		static double minDoubleVal();
		static string& replace_all(string& str,const string& old_val,const string& new_val);
//		static bool isSyetemFile(const char* path);
		static double arc(double angle);
		static double getArea(double diamter);

		static int stoi(const string& str);
		static double stod(const string& str);
		static string itos(int i);
		static string dtos(double i);
		static std::string d2s(double i,
							   int precision = 5);
		static void MBSToUTF8(char * utf8, int size, const char* mbs);
		static void UTF8ToMBS(char * mbs, int size, const char* utf8);
		static double getAnnulusArea(double diamter, double thick);
		static void unicodeToUTF8(const std::wstring &src, std::string& result);
		static void gb2312ToUnicode(const std::string& src, std::wstring& result);
		static void gb2312ToUtf8(const std::string& src, std::string& result);

		static double CEILING(double number, double significance);
		static bool cmp(const pair<int, double> &p1, const pair<int, double> &p2);//要用常数，不然编译错误;
		static bool cmpMod(const pair<int, double> &p1, const pair<int, double> &p2);//要用常数，不然编译错误，输出三维Mod用;
		static vector<string> splitString(string str, char del); //忽略“空位”（::192:168:ABC::416）->（192 168 ABC 416）;

		static double vectordoubleSum(vector<double>::iterator first, vector<double>::size_type size);
	};
}
