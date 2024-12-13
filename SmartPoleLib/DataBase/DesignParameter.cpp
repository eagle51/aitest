#include "DesignParameter.h"
//#include "BoltDisDLT5154_2012.h"

namespace SmartPoleCore
{
	DesignParameter::DesignParameter()
		:_elemSize(0.1)
		, _userBetaZ(1.0)
		, _projectName("PROJECT")
		, _designer("SXHJSoft")
		, _designeCon("CONDITION")
		, _nonlinearFlag(0)
		, _voltage(0)
		, _terrainType(CEXPO::B)
		, _deflection(5)
		, _altitude(0)
		, _isLinearPole(0)
		//, _BoltDisProj(new BoltDisDLT5154_2012())
	{
	}


	DesignParameter::~DesignParameter()
	{
	}
}