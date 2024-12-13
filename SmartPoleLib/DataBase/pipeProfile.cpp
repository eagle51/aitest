#include "pipeProfile.h"
#include "Constants.h"
#include "Auxiliary.h"

namespace SmartPoleCore
{
	pipeProfile::pipeProfile(Type itype, int iwide, int thk)
		:Profile(itype, iwide, thk), _Cx(0), _Cy(0), _MaxQ_It(0), _MaxC_J(0)
	{
		calcParameter();
	}


	pipeProfile::~pipeProfile()
	{
	}

	// @ function:   --- by lsq 2020/3/23 16:19
	void pipeProfile::calcParameter()
	{
		double D = _iWide - _iThickness;
		switch (_eType)
		{
		case SmartPoleCore::Profile::zero:
			_fArea = 3.14*D*_iThickness;
			_fIxx = _fIyy = 0.393 * D*D*D*_iThickness;
			break;
		case SmartPoleCore::Profile::four:
			_fArea = 4.0*D*_iThickness;
			_fIxx = _fIyy = 0.666 * D*D*D*_iThickness;
			break;
		case SmartPoleCore::Profile::six:
			_fArea = 3.46*D*_iThickness;
			_fIxx = _fIyy = 0.481 * D*D*D*_iThickness;
			break;
		case SmartPoleCore::Profile::eight:
			_fArea = 3.32*D*_iThickness;
			_fIxx = _fIyy = 0.438 * D*D*D*_iThickness;
			break;
		case SmartPoleCore::Profile::twelve:
			_fArea = 3.22*D*_iThickness;
			_fIxx = _fIyy = 0.411 * D*D*D*_iThickness;
			break;
		case SmartPoleCore::Profile::sixteen:
			_fArea = 3.19*D*_iThickness;
			_fIxx = _fIyy = 0.403 * D*D*D*_iThickness;
			break;
		default:
			_fArea = Constants::MAX_EXCEPTION_VALUE;
			_fIxx = _fIyy = Constants::MAX_EXCEPTION_VALUE;
			break;
		}

		_dJxy = _fIxx + _fIyy;

		calcCxAndCy();
		calcMaxC_J();
		calcMaxQ_It();
		calcW();
	}
	// @ function:   --- by lsq 2020/3/23 16:19
	Profile* pipeProfile::ratioProfile(Profile* pEndProfofile, double totalLen, double distanceToMe)
	{
		if (pEndProfofile->getType() != _eType)
			throw exception("Errors in the Profile Type, check please!");

		double iwide = _iWide - (_iWide - pEndProfofile->getWide()) / totalLen*distanceToMe;
		double iThickness = _iThickness - (_iThickness - pEndProfofile->getThk()) / totalLen*distanceToMe;

		Profile* ProfileTemp = new pipeProfile(_eType, iwide, iThickness);
		ProfileTemp->calcParameter();

		return ProfileTemp;
	}

	// @ function:  获取截面计算用角度 --- by lsq 2020/3/23 16:13
	double pipeProfile::calcAng()
	{
		switch (_eType)
		{
		case SmartPoleCore::Profile::zero:
			return Constants::PI*0.5;
		case SmartPoleCore::Profile::four:
			return 45.0 / 180.0 * Constants::PI;
		case SmartPoleCore::Profile::six:
			return 30.0 / 180 * Constants::PI;;
		case SmartPoleCore::Profile::eight:
			return 22.5 / 180 * Constants::PI;;
		case SmartPoleCore::Profile::twelve:
			return 15.0 / 180.0 * Constants::PI;;
		case SmartPoleCore::Profile::sixteen:
			return 11.25 / 180.0 * Constants::PI;;
		default:
			return Constants::MAX_EXCEPTION_VALUE;
		}
	}
	// @ function:   --- by lsq 2020/3/23 16:14
	void pipeProfile::calcCxAndCy()
	{
		double CNum = 0;
		switch (_eType)
		{
		case SmartPoleCore::Profile::zero:
			CNum = 0.5;
			break;
		case SmartPoleCore::Profile::sixteen:
			CNum = 0.510;
			break;
		case SmartPoleCore::Profile::twelve:
			CNum = 0.518;
			break;
		case SmartPoleCore::Profile::four:
			CNum = 0.707;
			break;
		case SmartPoleCore::Profile::six:
			CNum = 0.577;
			break;
		case SmartPoleCore::Profile::eight:
			CNum = 0.541;
			break;
		default:
			CNum = Constants::MAX_EXCEPTION_VALUE;
			break;
		}

		//规范里使用D+t,因为D=_iD0-t,所以这里直接使用_iD0
		double ang = calcAng();
		_Cx = CNum*_iWide*cos(ang);
		_Cy = CNum*_iWide*sin(ang);
	}
	// @ function:   --- by lsq 2020/3/23 16:14
	void pipeProfile::calcMaxQ_It()
	{
		double JNum = 0;
		switch (_eType)
		{
		case SmartPoleCore::Profile::zero:
			JNum = 0.637;
			break;
		case SmartPoleCore::Profile::four:
			JNum = 0.563;
			break;
		case SmartPoleCore::Profile::six:
			JNum = 0.606;
			break;
		case SmartPoleCore::Profile::eight:
			JNum = 0.618;
			break;
		case SmartPoleCore::Profile::twelve:
			JNum = 0.631;
			break;
		case SmartPoleCore::Profile::sixteen:
			JNum = 0.634;
			break;
		default:
			JNum = Constants::MAX_EXCEPTION_VALUE;
			break;
		}

		_MaxQ_It = JNum / (double(_iWide)*_iThickness);
	}
	// @ function:   --- by lsq 2020/3/23 16:14
	void pipeProfile::calcMaxC_J()
	{
		double JNum = 0;
		switch (_eType)
		{
		case SmartPoleCore::Profile::zero:
			JNum = 0.637;
			break;
		case SmartPoleCore::Profile::four:
			JNum = 0.500;
			break;
		case SmartPoleCore::Profile::six:
			JNum = 0.577;
			break;
		case SmartPoleCore::Profile::eight:
			JNum = 0.603;
			break;
		case SmartPoleCore::Profile::twelve:
			JNum = 0.622;
			break;
		case SmartPoleCore::Profile::sixteen:
			JNum = 0.628;
			break;
		default:
			JNum = Constants::MAX_EXCEPTION_VALUE;
			break;
		}

		double D = _iWide - _iThickness;
		_MaxC_J = JNum*_iWide / (D*D*D*_iThickness);
	}
	// @ function:   --- by lsq 2020/3/24 15:17
	void pipeProfile::calcW()
	{
		if (_eType != SmartPoleCore::Profile::UNDEFINED&&_eType != SmartPoleCore::Profile::zero)
		{
			switch (_eType)
			{
			case SmartPoleCore::Profile::four:
				_W = _iWide - 6.0 * _iThickness;
				break;
			case SmartPoleCore::Profile::six:
				_W = 0.577*(_iWide - 6.0 * _iThickness);
				break;
			case SmartPoleCore::Profile::eight:
				_W = 0.414*(_iWide - 6.0 * _iThickness);
				break;
			case SmartPoleCore::Profile::twelve:
				_W = 0.268*(_iWide - 6.0 * _iThickness);
				break;
			case SmartPoleCore::Profile::sixteen:
				_W = 0.199*(_iWide - 6.0 * _iThickness);
				break;
			default:
				_W = _iWide;
				break;
			}
		}
		else
			_W = _iWide;
	}
	// @ function: 压弯局部稳定的强度设计值 --- by lsq 2020/3/24 15:33
	void pipeProfile::calcfa(double steelf)
	{
		if (_eType == SmartPoleCore::Profile::zero)
			calcCirclefa(steelf);
		else
			calcPloygonfa(steelf);
	}
	// @ function: 圆形 压弯局部稳定的强度设计值 --- by lsq 2020/3/24 15:35
	void pipeProfile::calcCirclefa(double steelf)
	{
		double tempCir = _W / _iThickness;
		if (tempCir < 38060.0 / steelf)
			_fa = steelf;
		else if (Auxiliary::isLessOrEqual(38060.0 / steelf, tempCir) && Auxiliary::isLessOrEqual(tempCir, 76130.0 / steelf))
			_fa = 0.7*steelf + 11410.0 / tempCir;
	}
	// @ function: 多边形 压弯局部稳定的强度设计值 --- by lsq 2020/3/24 15:55
	void pipeProfile::calcPloygonfa(double steelf)
	{
		double tempPloygon = _W / _iThickness;		//规范中的W/t
		double fwtPloygon = sqrt(steelf)*tempPloygon;

		switch (_eType)
		{
// 		case SmartPoleCore::Profile::zero://对于环形，此方法得出的是fc
// 		{
// 			if (Auxiliary::isLessOrEqual(tempCir, 24100.0 / steelf))
// 			  _fa = steelf;
// 			else
// 			  _fa = 0.75*steelf + 6025.0 / tempCir;
// 		}
		case SmartPoleCore::Profile::four:
		case SmartPoleCore::Profile::six:
		case SmartPoleCore::Profile::eight:
		{
			if (Auxiliary::isLessOrEqual(tempPloygon, 660.0 / sqrt(steelf)))
			 _fa = steelf;
			else
			 _fa = 1.42*steelf*(1.0 - 0.000448*fwtPloygon);
		}
		case SmartPoleCore::Profile::twelve:
		{
			if (Auxiliary::isLessOrEqual(tempPloygon, 610.0 / sqrt(steelf)))
			 _fa = steelf;
			else
			 _fa = 1.45*steelf*(1.0 - 0.000507*fwtPloygon);
		}
		case SmartPoleCore::Profile::sixteen:
		{
			if (Auxiliary::isLessOrEqual(tempPloygon, 545.0 / sqrt(steelf)))
				_fa = steelf;
			else
				_fa = 1.42*steelf*(1.0 - 0.000539*fwtPloygon);
		}
		default:
			break;
		}
	}
	// @ function: 深复制  --- by lsq 2020/5/7 9:06
	Profile* pipeProfile::DeepCopy()
	{
		Profile* res = new pipeProfile(_eType, _iWide, _iThickness);
		res->calcParameter();
		return res;
	}


}