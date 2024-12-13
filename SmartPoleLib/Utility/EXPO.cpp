//#include "StdAfx.h"
#include "EXPO.h"
namespace SmartPoleCore
{
	////////////////////////////////////
	/////////////CEXPO//////////////////
	////////////////////////////////////
	CEXPO CEXPO::A=CEXPO('A');
	CEXPO CEXPO::B=CEXPO('B');
	CEXPO CEXPO::C=CEXPO('C');
	CEXPO CEXPO::D=CEXPO('D');
	CEXPO CEXPO::E=CEXPO('E');

	CEXPO::CEXPO(char Type)
		:_Type(Type)
	{
	}
	CEXPO::~CEXPO(void)
	{
	}
	CEXPO CEXPO::Create(char Type)
	{
		if(Type=='A' || Type=='a' || Type=='0')
		{
			if(Type=='0')
				CEXPO::A._Type='0';
			else
				CEXPO::A._Type='A';
			return CEXPO::A;
		}
		else if(Type=='B' || Type=='b' || Type=='1')
		{
			if(Type=='1')
				CEXPO::B._Type='1';
			else
				CEXPO::B._Type='B';
			return CEXPO::B;
		}
		else if(Type=='C' || Type=='c' || Type=='2')
		{
			if(Type=='0')
				CEXPO::C._Type='2';
			else
				CEXPO::C._Type='C';
			return CEXPO::C;
		}
		else if(Type=='D' || Type=='d' || Type=='3')
		{
			if(Type=='0')
				CEXPO::D._Type='3';
			else
				CEXPO::D._Type='D';
			return CEXPO::D;
		}
		else if (Type=='E' || Type=='e' || Type=='4')
		{
			if(Type=='0')
				CEXPO::E._Type='4';
			else
				CEXPO::E._Type='E';
			return CEXPO::E;
		}
		return CEXPO::B;
	};

	SmartPoleCore::CEXPO CEXPO::Transfer(int Type)
	{
		switch (Type)
		{
		case 0:
			return CEXPO::A;
		case 1:
			return CEXPO::B;
		case 2:
			return CEXPO::C;
		case 3:
			return CEXPO::D;
		case 4:
			return CEXPO::E;
		}
		return CEXPO::B;
	}

}