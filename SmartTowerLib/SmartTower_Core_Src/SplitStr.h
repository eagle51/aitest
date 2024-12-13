#pragma once
#include <vector>
#include <string>
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月21日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	using namespace std;
	class CSplitStr
	{
	private:
		size_t _strlinwlen;
		vector<string> _StrArray; 
	public:
		static bool commaFlag;
		void SplitOnlyNumber(const char *pChar);
		void Split(const char *pChar);
		void Split(const char *pChar, char splitChar);
		void Split(const std::string& strIn, int cLen, int cStart = 0);
		const char* operator[](size_t idx) const
		{
			if (idx >= _StrArray.size())
				return "";
			return _StrArray.at(idx).c_str();
		};
		const char* at(size_t idx) const
		{
			if (idx >= _StrArray.size())
				return "";
			return _StrArray.at(idx).c_str();
		}
		int intVal(size_t idx)
		{
			if (idx >= _StrArray.size())
				return 0;
			return std::atoi(_StrArray.at(idx).c_str());
		}
		double doubleVal(size_t idx)
		{
			if (idx >= _StrArray.size())
				return 0;
			return std::atof(_StrArray.at(idx).c_str());
		}
		size_t size() const
		{
			return _StrArray.size();
		};
		size_t getStrLineLen()
		{
			return _strlinwlen;
		}
		bool empty() const
		{
			return _StrArray.empty();
		}
		const char* removeQuotationmarkAt(size_t idx) const;
		CSplitStr(void);
		~CSplitStr(void);
	};
}
