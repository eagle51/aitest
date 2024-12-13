#include "NonPipeProfile.h"
#include "Constants.h"

namespace SmartPoleCore
{
	NonPipeProfile::NonPipeProfile(Type itype, double High, double Wide, double UDThickness, double sidePanelThickness)
		:Profile(itype, Wide, sidePanelThickness), _High(High), _UDThickness(UDThickness)
	{
		calcParameter();
	}


	NonPipeProfile::~NonPipeProfile()
	{
	}
	// @ function:   --- by lsq 2020/3/23 15:30
	void NonPipeProfile::calcParameter()
	{
		double h = 0;
		double b = 0;
		switch (_eType)
		{
		case SmartPoleCore::Profile::CSone:
		{
			b = _iWide - _iThickness*2.0;
			h = _High - _UDThickness*2.0;
			break;
		}
		case SmartPoleCore::Profile::CStwo:
		{
			b = _iWide - 2.0*_iThickness;
			h = _High - _UDThickness;

			break;
		}
		default:
			break;
		}

		_fArea = 1.0*_High*_iWide - h*b;

		calcIx();
		calcIy();
		calcJxy();
	}
	// @ function:   --- by lsq 2020/3/23 15:30
	Profile* NonPipeProfile::ratioProfile(Profile* pEndProfofile, double totalLen, double distanceToMe)
	{
		if (pEndProfofile->getType() != _eType)
			throw exception("Errors in the Profile Type, check please!");
	
		int pEndProfofileH = static_cast<NonPipeProfile*>(pEndProfofile)->getHigh();
		int pEndProfofileUDThk = static_cast<NonPipeProfile*>(pEndProfofile)->getUDThickness();

		double iwide = _iWide - (_iWide - pEndProfofile->getWide()) / totalLen*distanceToMe;
		double iThickness = _iThickness - (_iThickness - pEndProfofile->getThk()) / totalLen*distanceToMe;
		double High = _High - (_High - pEndProfofileH) / totalLen*distanceToMe;
		double iUDThickness = _UDThickness - (_UDThickness - pEndProfofileUDThk) / totalLen*distanceToMe;
		Profile* ProfileTemp = new NonPipeProfile(_eType, High, iwide, iUDThickness, iThickness);
		ProfileTemp->calcParameter();

		return ProfileTemp;
	}
	// @ function:   --- by lsq 2020/3/23 15:39
	void NonPipeProfile::calcIx()
	{
		switch (_eType)
		{
		case SmartPoleCore::Profile::CSone:
		{
			double b = _iWide - _iThickness*2.0;
			double h = _High - _UDThickness*2.0;

			_fIxx = (1.0*_High*_High*_High*_iWide - h*h*h*b) / 12.0;
			break;
		}
		case SmartPoleCore::Profile::CStwo:
		{
			double h = _High - _UDThickness*2.0;
			double temp1 = 1.0*_High*_High*_iWide - 1.0*(_iWide - 2.0*_iThickness)*(_High - _UDThickness)*(_High - _UDThickness);
			double temp2 = _High*_iWide - (_iWide - 2.0*_iThickness)*(_High - _UDThickness);
			double x2 = temp1 / temp2*0.5;
			double x1 = _High - x2;
			_fIxx = 1.0*_iThickness*_High*_High*_High / 6.0 + 2.0*(_High*0.5 - x1)*(_High*0.5 - x1)*_iThickness*_High
				+ 1.0*(_iWide - 2.0*_iThickness)*_UDThickness*_UDThickness*_UDThickness / 12.0
				+ (x1 - _UDThickness*0.5)*(x1 - _UDThickness*0.5)*(_iWide - 2.0*_iThickness)*_UDThickness;
			break;
		}
		default:
			_fIxx = Constants::MAX_EXCEPTION_VALUE;
			break;
		}

	}
	// @ function:   --- by lsq 2020/3/23 15:50
	void NonPipeProfile::calcIy()
	{
		switch (_eType)
		{
		case SmartPoleCore::Profile::CSone:
		{	
			double b1 = _iWide - _iThickness*2.0;
			double h1 = _High - _UDThickness*2.0;
			_fIyy = (1.0*_iWide*_iWide*_iWide*_High - b1*b1*b1*h1) / 12.0;
			break;
		}
		case SmartPoleCore::Profile::CStwo:
		{
			_fIyy = (1.0*_High*_iWide*_iWide*_iWide - 1.0*(_High - _UDThickness)*(_iWide - _iThickness*2.0)
				   *(_iWide - _iThickness*2.0)*(_iWide - _iThickness*2.0)) / 12.0;
			break;
		}
		default:
			_fIyy = Constants::MAX_EXCEPTION_VALUE;
			break;
		}
	}
	// @ function:   --- by lsq 2020/3/23 16:36
	void NonPipeProfile::calcJxy()
	{
		switch (_eType)
		{
		case SmartPoleCore::Profile::CSone:
		{
			double temp1 = 2.0*_UDThickness*_iThickness*((_High - _UDThickness)*(_High - _UDThickness))*((_iWide - _iThickness)*(_iWide - _iThickness));
			double temp2 = 1.0*_High*_UDThickness + 1.0*_iWide*_iThickness - 1.0*_UDThickness*_UDThickness - 1.0*_iThickness*_iThickness;
			_dJxy= temp1 / temp2;
			break;
		}
		case SmartPoleCore::Profile::CStwo:
		{
			double a = 1.0*_UDThickness*_UDThickness*_UDThickness*_iWide;
			double b = 2.0 * _iThickness*_iThickness*_iThickness*(_High - _UDThickness);
			double sum = a + b;
			_dJxy = sum / 3.0;
			break;
		}
		default:
			_dJxy = Constants::MAX_EXCEPTION_VALUE;
			break;
		}
	}
	// @ function:   --- by lsq 2020/5/7 9:07
	Profile* NonPipeProfile::DeepCopy()
	{
		Profile* res = new NonPipeProfile(_eType, _High, _iWide, _UDThickness, _iThickness);
		res->calcParameter();
		return res;
	}

}