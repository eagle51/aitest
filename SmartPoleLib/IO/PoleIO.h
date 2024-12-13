#pragma once
#include "ModelDateBase.h"
namespace SmartPoleCore
{
	class PoleIO
	{
	public:
		PoleIO();
		virtual ~PoleIO();
		virtual void read(const char* username = 0) = 0;
		virtual void write(const char* username = 0) = 0;
		static const char* suffix;
	};

}
