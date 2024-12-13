//#include "StdAfx.h"
#include "SplitStr.h"

namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月21日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	CSplitStr::CSplitStr(void):_strlinwlen(0)
	{
	};
	CSplitStr::~CSplitStr(void)
	{
	}
	void CSplitStr::Split(const char *pChar)
	{
		//本行数将字符串，转化为数组。
		int icountp=0,iloop=0,iplace=0; 
		_StrArray.clear();
		_strlinwlen=0;
		while(1)
		{
			_strlinwlen++;
			if('\n'==pChar[iloop] || '\0'==pChar[iloop]) //如果遇到结尾就返回
				break;
			else if(CSplitStr::commaFlag && pChar[iloop]==',')
			{
				if(icountp>0)
				{  
					_StrArray.push_back(string(pChar,iplace,icountp));
					icountp=0;
				}  
				iplace=iloop+1;
			}
			else if(pChar[iloop]!=' ' && pChar[iloop]!='\t') //遇到空格或的tab就分搁
				icountp++;
			else
			{			
				if(icountp>0)
				{  
					_StrArray.push_back(string(pChar,iplace,icountp));
					icountp=0;
				}  
				iplace=iloop+1;
			}
			iloop++;
		}
		if(icountp>0)
		{
			_StrArray.push_back(string(pChar,iplace,icountp));
		} 
	}

	void CSplitStr::Split( const char *pChar, char splitChar )
	{
		//本行数将字符串，转化为数组。
		int icountp = 0, iloop = 0, iplace = 0;
		_StrArray.clear();
		_strlinwlen = 0;
		while (1)
		{
			_strlinwlen++;
			if ('\n' == pChar[iloop] || '\0' == pChar[iloop]) //如果遇到结尾就返回
				break;
			else if (pChar[iloop] != splitChar) //遇到空格或的tab就分搁
				icountp++;
			else
			{
				if (icountp > 0)
				{
					_StrArray.push_back(string(pChar, iplace, icountp));
					icountp = 0;
				}
				iplace = iloop + 1;
			}
			iloop++;
		}
		if (icountp > 0)
		{
			_StrArray.push_back(string(pChar, iplace, icountp));
		}
	}

	void CSplitStr::Split(const std::string& strIn, int cLen, int cStart)
	{
		_StrArray.clear();
		while (cStart < int(strIn.length()))
		{
			_StrArray.push_back(strIn.substr(cStart, cLen));
			cStart += cLen;
		}
	}

	void CSplitStr::SplitOnlyNumber( const char *pChar )
	{
		int icountp=0,iloop=0,iplace=0;
		_StrArray.clear();
		_strlinwlen=0;
		while(1)
		{
			_strlinwlen++;
			char curChar=pChar[iloop];
			if('\n'==curChar || '\0'==curChar) //如果遇到结尾就返回
				break;
			else if((curChar>='0' && curChar<='9') || curChar=='.') //遇到空格或的tab就分搁
				icountp++;
			else
			{			
				if(icountp>0)
				{  
					_StrArray.push_back(string(pChar,iplace,icountp));
					icountp=0;
				}  
				iplace=iloop+1;
			}
			iloop++;
		}
		if(icountp>0)
		{
			_StrArray.push_back(string(pChar,iplace,icountp));
		} 
	}

	const char* CSplitStr::removeQuotationmarkAt( size_t idx ) const
	{
		static string tmpStr;
		if(idx>=_StrArray.size())
			return "";
		tmpStr=_StrArray.at(idx);
		if(tmpStr.length()>2)
		{
			tmpStr=tmpStr.substr(1,tmpStr.length()-2);
			return tmpStr.c_str();
		}
		else
			return "";
	}

	bool CSplitStr::commaFlag=false;

}