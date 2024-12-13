//#include "StdAfx.h"
#include "ErrMsgExporter.h"
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include "SmartException.h"


namespace SmartPoleCore
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

	void ConsoleErrExporter::SendErrMessage(const char* ErrInfo, SmartException* pErrObject)
	{
		if(iErrorCount<80)
		{
			std::cout<<ErrInfo<<std::endl;
			++iErrorCount;
		}		
	}

	void ConsoleErrExporter::ClearAll()
	{
		iErrorCount=0;
		iWorningCount=0;
	}

	void ConsoleErrExporter::SendPromptMessage( const char* Msg )
	{
		std::cout<<Msg<<std::endl;
	}

	void ConsoleErrExporter::SendWorningMessage(const char* ErrInfo, SmartException* pWorningObject)
	{
		if(iWorningCount<60)
		{
			std::cout<<ErrInfo<<std::endl;
			++iWorningCount;
		}
	}

	void ConsoleErrExporter::SendPermanentWorningMessage( const char* Msg )
	{
		std::cout<<Msg<<std::endl;
		//警告信息必须看一下
		_getch();
	}

}