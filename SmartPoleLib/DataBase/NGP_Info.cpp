//#include "StdAfx.h"
#include "NGP_Info.h"
namespace SmartPoleCore
{

	NGP_Info::NGP_Info(int StartLeg,int EndLeg,int StartCase,int EndCase)
		:_StartLeg(StartLeg),_EndLeg(EndLeg),_StartCase(StartCase),_EndCase(EndCase),_BetaZIndex(1),
		_ActiveStartLeg(StartLeg-1),_ActiveEndLeg(EndLeg),
		_ActiveStartCase(StartCase-1),_ActiveEndCase(EndCase),_ActiveRefMaxZ(0)//,_isActive(true)
	{
		if(_StartLeg<1)
			_StartLeg=1;
		if(_StartCase<1)
			_StartCase=1;
		if(_ActiveStartLeg<0)
			_ActiveStartLeg=0;
		if(_ActiveStartCase<0)
			_ActiveStartCase=0;
	}

	NGP_Info::~NGP_Info(void)
	{
	}

	void NGP_Info::setActiveRangeLeg( int startleg,int endleg )
	{
		if(_StartLeg>endleg || startleg>_EndLeg)
		{
			_ActiveStartLeg=-1;
			_ActiveEndLeg=-1;
		}
		else
		{
			if(_StartLeg>startleg)
				_ActiveStartLeg=_StartLeg-1;
			else
				_ActiveStartLeg=startleg-1;
			if(_EndLeg>endleg)
				_ActiveEndLeg=endleg;
			else
				_ActiveEndLeg=_EndLeg;
		}
	}

	void NGP_Info::setActiveRangeCase( int startcase,int endcase )
	{
		if(_StartCase>endcase || startcase>_EndCase)
		{
			_ActiveStartCase=-1;
			_ActiveEndCase=-1;
		}
		else
		{
			if(_StartCase>startcase)
				_ActiveStartCase=_StartCase-1;
			else
				_ActiveStartCase=startcase-1;
			if(_EndCase>endcase)
				_ActiveEndCase=endcase;
			else
				_ActiveEndCase=_EndCase;
		}
	}
}