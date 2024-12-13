#pragma once
#include "SharedObject.h"
///////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{	
	class TowerException;
	class ErrMsgExporter:public SharedObject 
	{
	public:
		ErrMsgExporter();
		virtual ~ErrMsgExporter()=0;
		virtual void SendErrMessage(const char* errInfo,TowerException* pErrObject=0)=0;
		virtual void SendWorningMessage(const char* errInfo, TowerException* pWorningObject = 0) = 0;
		virtual void SendPromptMessage(const char* strMsg)=0;
		virtual void SendPermanentWorningMessage(const char* strMsg) = 0;
		virtual void ClearAll()=0;
	};
	

	class ConsoleErrExporter:public ErrMsgExporter
	{
	public:
		ConsoleErrExporter();
		virtual ~ConsoleErrExporter();
		virtual void SendErrMessage(const char* errInfo, TowerException* pErrObject = 0);
		virtual void SendWorningMessage(const char* errInfo, TowerException* pWorningObject = 0);
		virtual void SendPromptMessage(const char* strMsg);
		virtual void SendPermanentWorningMessage(const char* strMsg);
		virtual void ClearAll();
	
	private:
		int iErrorCount;
		int iWorningCount;
	};
}