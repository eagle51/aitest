#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "TowerException.h"
#include "ErrMsgExporter.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
using namespace std; 
namespace SmartTower
{
	
	//全局的错误消息集合类，用于处理消息
	class CExceptionMessageRegister
	{	
	public:
		//singlton 模式
		static CExceptionMessageRegister * Instance()
		{
			return _instance;
		}
		void ChangeErrExporter(StHandle<ErrMsgExporter> pExporter)
		{
			_ErrExporter=pExporter;
		}
		StHandle<ErrMsgExporter> getErrExporter() const
		{
			return _ErrExporter;
		}		

		void SendErrObject(HandleTException pExp)
		{
			_ErrCollection.push_back(pExp);
			if(_ErrEnable)
				_ErrExporter->SendErrMessage(pExp->what(), pExp.GetPointee());
		};

		void SendErrMessage(const char* errInfo)
		{
			HandleTException pMsgException = new TowerException(errInfo);
			_ErrCollection.push_back(pMsgException);
			if(_ErrEnable)
				_ErrExporter->SendErrMessage(errInfo, pMsgException.GetPointee());
		};

		void SendWorningObject(HandleTException pExp)
		{
			_WarningCollection.push_back(pExp);
			if(_WarningEnable)
				_ErrExporter->SendWorningMessage(pExp->what(), pExp.GetPointee());
		}

		void SendWorningMessage(const char* errInfo)
		{
			HandleTException pWarningException = new TowerException(errInfo);
			_WarningCollection.push_back(pWarningException);
			if(_WarningEnable)
				_ErrExporter->SendWorningMessage(errInfo, pWarningException.GetPointee());
		}

		void SendPromptMessage(const char* errInfo)
		{
			_ErrExporter->SendPromptMessage(errInfo);
		}

		void SendPermanentWorningMessage(const char* errInfo)
		{
			_ErrExporter->SendPermanentWorningMessage(errInfo);
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
		HandleTException getErrObjAt(size_t idx) const
		{
			return _ErrCollection.at(idx);
		}
		size_t WorningCount() const
		{
			return _WarningCollection.size();
		}		
		HandleTException getWorningObjAt(size_t idx) const
		{
			return _WarningCollection.at(idx);
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
		vector<HandleTException> _ErrCollection;
		vector<HandleTException> _WarningCollection;
		bool _ErrEnable;
		bool _WarningEnable;
		StHandle<ErrMsgExporter> _ErrExporter;
		CExceptionMessageRegister(void);
		~CExceptionMessageRegister(void);
		static CExceptionMessageRegister* _instance;	
	};	

}