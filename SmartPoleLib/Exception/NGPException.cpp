#include "NGPException.h"
#include <sstream>

namespace SmartPoleCore
{

	NGPException::NGPException(HandleNGP errrNGP, ErrType Info, int caseorlegindex /*= -1*/)
		:SmartException(errrNGP.GetPointee()), _ErrInfo(Info), _caseorlegindex(caseorlegindex)
	{
		std::ostringstream ErrMessage;
		ErrMessage << +("Errors in load grouping data!");
		switch (_ErrInfo)
		{
		case exceedCaseRange:
			ErrMessage << ("The number of load cases in load grouping data exceed that of total load cases specified!");
			break;
		case caseOverlap:
			ErrMessage << ("A load case is assigned to two load groups at the same time! The No. of the load case is:") << caseorlegindex;
			break;
		case caseLoss:
			ErrMessage << ("There is at least a load case left out in load grouping, the No. is:") << caseorlegindex;
			break;
		case exceedLegRange:
			ErrMessage << ("The number of leg extensions in load grouping exceed that specified in Body/Leg table!");
			break;
		case legLoss:
			ErrMessage << ("There is a leg extension left out in load grouping! The No. is:") << caseorlegindex;
			break;
		case onlyOneNGP:
			ErrMessage << ("Only one load group defined! It is not necessary and please delete!") << caseorlegindex;
			break;
		case legOverlap:
			ErrMessage << ("There exists a leg extension assigned to two load groups at the same time, the No. is:") << caseorlegindex;
			break;
		case caseErrorRange:
			ErrMessage << ("The number of the starting load case is larger than that of the ending!");
			break;
		case legErrorRange:
			ErrMessage << ("The number of the starting leg extension is larger than that of the ending!");
			break;
		case betaZIndexError:
			ErrMessage << "荷载分组对应的BetaZ列索引错误，荷载分组BetaZ索引必须和BetaZ列一一对应";
			break;
		case betaZLoss:
			ErrMessage << "存在无用BetaZ列数据，是否荷载分组错误？";
			break;
		}
		_ErrMsg = ErrMessage.str();
	}


	NGPException::~NGPException()
	{
	}


}