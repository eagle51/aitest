#pragma once
#include "SharedHandle.hpp"
//using namespace std;
namespace SmartPoleCore
{	
	class SmartException;
	class ErrMsgExporter:public SharedObject
	{
	public:
		ErrMsgExporter();
		virtual ~ErrMsgExporter()=0;
		virtual void SendErrMessage(const char* ErrInfo, SmartException* pErrObject = 0) = 0;
		virtual void SendWorningMessage(const char* ErrInfo, SmartException* pWorningObject = 0) = 0;
		virtual void SendPromptMessage(const char* Msg)=0;
		virtual void SendPermanentWorningMessage(const char* Msg)=0;
		virtual void ClearAll()=0;
	};
	typedef SharedHandle<ErrMsgExporter> HandleMsgExporter;
	

	class ConsoleErrExporter:public ErrMsgExporter
	{
	public:
		ConsoleErrExporter();
		virtual ~ConsoleErrExporter();
		virtual void SendErrMessage(const char* ErrInfo, SmartException* pErrObject = 0);
		virtual void SendWorningMessage(const char* ErrInfo, SmartException* pWorningObject = 0);
		virtual void SendPromptMessage(const char* Msg);
		virtual void SendPermanentWorningMessage(const char* Msg);
		virtual void ClearAll();
	
	private:
		int iErrorCount;
		int iWorningCount;
	};
}