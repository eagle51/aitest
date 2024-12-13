#include "VirBoxChecker.h"
#include <crtdbg.h>

namespace AboutSoftWare
{
	//@function:    -- by mzq 2022/11/1 9:49
	VirBoxChecker::VirBoxChecker()
		:safeLoginFlag(false),
		enlargeFactor(1000.0),
		virBoxPi(1.0),
		canReadFlag(false),
		canWriteFlag(false),
		canSolveFlag(false)
	{

	}

	//@function:    -- by mzq 2022/11/1 9:48
	VirBoxChecker* VirBoxChecker::getInstance()
	{
		static VirBoxChecker* pinstance = NULL;
		if (pinstance == NULL)
			pinstance = new VirBoxChecker();
		return pinstance;
	}

	void VirBoxChecker::setErrVal()
	{
		canReadFlag = false;
		canWriteFlag = false;
		canSolveFlag = false;
	}
}