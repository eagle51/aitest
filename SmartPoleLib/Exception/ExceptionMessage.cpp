//#include "StdAfx.h"
#include "ExceptionMessage.h"

namespace SmartPoleCore
{

	ExceptionMessageRegister::ExceptionMessageRegister( void )
	{
		_ErrEnable=true;
		_WarningEnable=true;
		_ErrExporter=new ConsoleErrExporter();
	}

	ExceptionMessageRegister::~ExceptionMessageRegister(void)
	{		
	};

	void ExceptionMessageRegister::updateDisp() const
	{
		_ErrExporter->ClearAll();
		if(_ErrEnable)
		{
			for (vector<HandleTException>::const_iterator Iter=_ErrCollection.begin();Iter<_ErrCollection.end();++Iter)
				_ErrExporter->SendErrMessage((*Iter)->what(),Iter->GetPointee());
		}
		if(_WarningEnable)
		{
			for (vector<HandleTException>::const_iterator Iter=_WarningCollection.begin();Iter<_WarningCollection.end();++Iter)
				_ErrExporter->SendWorningMessage((*Iter)->what(),Iter->GetPointee());
		}	
	}

	const HandleTException& ExceptionMessageRegister::getAt( int iPos ) const
	{
		size_t Index=size_t(iPos);
		if(_ErrEnable && _WarningEnable)
		{
			if(Index<_ErrCollection.size())
				return _ErrCollection.at(Index);
			else
				return _WarningCollection.at(Index-_ErrCollection.size());
		}
		else if(_ErrEnable)
			return _ErrCollection.at(Index);
		else if(_WarningEnable)
			return _WarningCollection.at(Index);
		return _ErrCollection.at(Index);
	}
	ExceptionMessageRegister* ExceptionMessageRegister::_instance=new ExceptionMessageRegister();
	
}