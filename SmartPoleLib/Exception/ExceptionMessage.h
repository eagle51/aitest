#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include "SmartException.h"
#include "ErrMsgExporter.h"
using namespace std;
namespace SmartPoleCore
{
	
	//全局的错误消息集合类，用于处理消息
	class ExceptionMessageRegister
	{	
	public:
		//singlton 模式
		static ExceptionMessageRegister * Instance()
		{
			return _instance;
		}
		void ChangeErrExporter(const HandleMsgExporter& pExporter)
		{
			_ErrExporter=pExporter;
		}
		HandleMsgExporter getErrExporter() const
		{
			return _ErrExporter;
		}		

		void SendErrObject(const HandleTException& Exp)
		{
			_ErrCollection.push_back(Exp);
			if(_ErrEnable)
				_ErrExporter->SendErrMessage(Exp->what(),Exp.GetPointee());
		};

		void SendErrMessage(const char* ErrInfo)
		{
			HandleTException pMsgException = new SmartException(ErrInfo);
			_ErrCollection.push_back(pMsgException);
			if(_ErrEnable)
				_ErrExporter->SendErrMessage(ErrInfo,pMsgException.GetPointee());					
		};

		void SendWorningObject(const HandleTException& Exp)
		{
			_WarningCollection.push_back(Exp);
			if(_WarningEnable)
				_ErrExporter->SendWorningMessage(Exp->what(),Exp.GetPointee());
		}

		void SendWorningMessage(const char* ErrInfo)
		{
			HandleTException pWarningException = new SmartException(ErrInfo);
			_WarningCollection.push_back(pWarningException);
			if(_WarningEnable)
				_ErrExporter->SendWorningMessage(ErrInfo,pWarningException.GetPointee());
		}

		void SendPromptMessage(const char* ErrInfo)
		{
			_ErrExporter->SendPromptMessage(ErrInfo);
		}

		void SendPermanentWorningMessage(const char* ErrInfo)
		{
			_ErrExporter->SendPermanentWorningMessage(ErrInfo);
		}

		void ErrMsgClear()
		{
			_ErrCollection.clear();
			_WarningCollection.clear();
			_ErrExporter->ClearAll();
		};

		bool HasErr() const
		{
			return _ErrCollection.size()>0;
		}
		//获取第几个信息，先错误后警告信息
		const HandleTException& getAt(int iPos) const;
		size_t ActiveCount() const
		{
			if(_ErrEnable && _WarningEnable)
				return _ErrCollection.size()+_WarningCollection.size();
			else if(_ErrEnable)
				return _ErrCollection.size();
			else if(_WarningEnable)
				return _WarningCollection.size();
			return _WarningCollection.size();
		}
		size_t TotalRegOBjectCount() const
		{
			return _ErrCollection.size()+_WarningCollection.size();
		}
		size_t ErrCount() const
		{
			return _ErrCollection.size();
		}
		HandleTException getErrObjAt(size_t Index) const
		{
			return _ErrCollection.at(Index);
		}
		size_t WorningCount() const
		{
			return _WarningCollection.size();
		}		
		HandleTException getWorningObjAt(size_t Index) const
		{
			return _WarningCollection.at(Index);
		}
		void setErrEnable(bool flag)
		{
			_ErrEnable=flag;
		}
		bool ErrEnable() const
		{
			return _ErrEnable;
		}

		void setWarningEnable(bool flag)
		{
			_WarningEnable=flag;
		}
		bool WarningEnable() const
		{
			return _WarningEnable;
		}
		void updateDisp() const;
	private:
		std::vector<HandleTException> _ErrCollection;
		std::vector<HandleTException> _WarningCollection;
		bool _ErrEnable;
		bool _WarningEnable;
		HandleMsgExporter _ErrExporter;
		ExceptionMessageRegister(void);
		~ExceptionMessageRegister(void);
		static ExceptionMessageRegister* _instance;	
	};	

}