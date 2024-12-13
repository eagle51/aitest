//#include "StdAfx.h"
#include "TTAType.h"


namespace SmartTower
{

	CTTAType::CTTAType(void) :_Type(DBTTA), _ActiveSecType(TowBaseObject::XBSECPOINT)
	{
	}


	CTTAType::~CTTAType(void)
	{
	};
	void CTTAType::Type(TTATp val)
	{
		_Type=val;
	}

	HandleModelIO CTTAType::getTowerModelIO(CTowerModel* Model)
	{
		if(_Type==DBTTA)
			return new CTTAFormatIO(Model);
		else
			return new CXBTTAPFormatIO(Model);
	}

	CTTAType* CTTAType::_Instance=new CTTAType();

} 