//#include "StdAfx.h"
#include "ExceptionMessage.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	 
	CExceptionMessageRegister::CExceptionMessageRegister(void)
	{
		_ErrEnable=true;
		_WarningEnable=true;
		_ErrExporter=new ConsoleErrExporter();
	}

	CExceptionMessageRegister::~CExceptionMessageRegister(void)
	{		
	};

	void CExceptionMessageRegister::updateDisp() const
	{
		_ErrExporter->ClearAll();
		if(_ErrEnable)
		{
			for (vector<HandleTException>::const_iterator eIter=_ErrCollection.begin();eIter<_ErrCollection.end();++eIter)
				_ErrExporter->SendErrMessage((*eIter)->what(),eIter->GetPointee());
		}
		if(_WarningEnable)
		{
			for (vector<HandleTException>::const_iterator eIter=_WarningCollection.begin();eIter<_WarningCollection.end();++eIter)
				_ErrExporter->SendWorningMessage((*eIter)->what(),eIter->GetPointee());
		}	
	}

	const HandleTException& CExceptionMessageRegister::getAt( int iPos ) const
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
	CExceptionMessageRegister* CExceptionMessageRegister::_instance=new CExceptionMessageRegister();
	
}