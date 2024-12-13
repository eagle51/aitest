//#include "StdAfx.h"
#include "ErrMsgExporter.h"
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include "TowerException.h"

//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有 
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{	
	ErrMsgExporter::ErrMsgExporter()
	{

	}

	ErrMsgExporter::~ErrMsgExporter()
	{

	}

	ConsoleErrExporter::ConsoleErrExporter():iErrorCount(0),iWorningCount(0)
	{

	}

	ConsoleErrExporter::~ConsoleErrExporter()
	{

	}

	void ConsoleErrExporter::SendErrMessage(const char* errInfo, TowerException* pErrObject)
	{
		if(iErrorCount<80)
		{
			std::cout << errInfo << std::endl;
			++iErrorCount;
		}		
	}

	void ConsoleErrExporter::ClearAll()
	{
		iErrorCount=0;
		iWorningCount=0;
	}

	void ConsoleErrExporter::SendPromptMessage(const char* strMsg)
	{
		std::cout << strMsg << std::endl;
	}

	void ConsoleErrExporter::SendWorningMessage(const char* errInfo, TowerException* pWorningObject)
	{
		if(iWorningCount<60)
		{
			std::cout << errInfo << std::endl;
			++iWorningCount;
		}
	}

	void ConsoleErrExporter::SendPermanentWorningMessage(const char* strMsg)
	{
		std::cout << strMsg << std::endl;
		//警告信息必须看一下
		_getch();
	}

}