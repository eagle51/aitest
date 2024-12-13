#pragma once
#include "SmartException.h"
#include "NGP_Info.h"
namespace SmartPoleCore
{
	class NGPException :public SmartException
	{
	public:
		enum ErrType
		{
			exceedCaseRange = 0,
			caseOverlap = 1,
			caseLoss = 2,

			exceedLegRange = 3,
			legLoss = 4,

			onlyOneNGP = 5,

			legOverlap = 6,

			caseErrorRange = 7,

			legErrorRange = 8,

			betaZIndexError = 9,

			betaZLoss = 10,
		};
		NGPException(HandleNGP errrNGP, ErrType Info, int caseorlegindex = -1);
		~NGPException();

	private:		
		ErrType _ErrInfo;
		int _caseorlegindex;
	};

}

