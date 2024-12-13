#include "SmartException.h"

namespace SmartPoleCore
{	

	SmartException::SmartException( const char* ErrMsg )
		:_ErrMsg(ErrMsg)
	{

	}

	SmartException::SmartException( HandleObject ErrObject )
		:_ErrObject(ErrObject)
	{

	}

	SmartException::~SmartException(void)
	{
	}

	bool SmartException::isSymErr() const
	{
		return false;
	}

	bool SmartException::isDuplicate() const
	{
		return false;
	}

	bool SmartException::isRefPointErr() const
	{
		return false;
	}

}
