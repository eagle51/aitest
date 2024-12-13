#pragma once
#include <string>
#include <sstream>
#include <iostream> 
#include "SharedHandle.hpp"
namespace SmartPoleCore
{	
	class SmartException:public SharedObject
	{
	public:
		SmartException(const char* ErrMsg);
		SmartException(HandleObject ErrObject);
		~SmartException(void);
		HandleObject GetErrObject()
		{
			return _ErrObject;
		}
		virtual const char* what() const
		{
			return _ErrMsg.c_str();
		}

		virtual bool isRefPointErr() const;

		virtual bool isSymErr() const;

		virtual bool isDuplicate() const;

	protected:
		HandleObject _ErrObject;
		std::string _ErrMsg;
	};
	typedef SharedHandle<SmartException> HandleTException;
}

