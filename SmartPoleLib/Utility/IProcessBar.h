#pragma once
#include "SharedObject.h"
#include "SharedHandle.hpp"

namespace SmartPoleCore
{
	class IProcessBar:public SharedObject
	{
	public:
		virtual void setRange(int Val)=0;
		virtual void setValue(int Val)=0;
		virtual void addValue(int val)=0;
		virtual void reset() = 0;
		IProcessBar(void);
		virtual ~IProcessBar(void)=0;
	};
	typedef SharedHandle<IProcessBar> HdProcessBar;
}