#pragma once
#include <vector>
#include <string>
#include "SharedObject.h"
#include "StHandle.hpp"
using namespace std;
namespace SmartTower
{
	class StringContainer:public SharedObject
	{
	public:
		void addString(const char *str,int iPos)
		{
			if(str==NULL && iPos>2)
				return;
			else if(str==NULL)
				_strings[iPos]="";
			_strings[iPos]=str;
		}
		const char *getString(int Index) const 
		{
			if(Index>=(int)_strings.size())
				return "";
			return _strings[Index].c_str();
		}
		int getNumStrings() const
		{
			return 3;
		}
		void clear(void)
		{
			_strings.clear();
		}
		StringContainer(void);
		~StringContainer(void);
	private:
		vector<string> _strings;
	};
	typedef StHandle<StringContainer> HandleStringContainer;
}