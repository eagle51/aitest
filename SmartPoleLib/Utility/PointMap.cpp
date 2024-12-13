#include "PointMap.h"
#include "PointErrException.h"
#include "ExceptionMessage.h"

namespace SmartPoleCore
{
	PointMap::PointMap()
	{
	}


	PointMap::~PointMap()
	{
	}

	void PointMap::Insert(const HandlePoint &AddNode)
	{
		if (AddNode.isNull())
			return;
		pair<std::map<int, HandlePoint>::iterator, bool> Insertflag =  \
			_pNodeMap.insert(pair<int, HandlePoint>(AddNode->TagNum(), AddNode));
		if (!Insertflag.second)
		{
			HandlePoint pOldObject = Insertflag.first->second;
			PointErrException::ErrType tmpType = PointErrException::IsymErr;
			if (pOldObject.GetPointee() != AddNode.GetPointee())
				tmpType = PointErrException::DuplicateNum;
			HandleTException ERRObj = new PointErrException(AddNode, tmpType);
			ExceptionMessageRegister::Instance()->SendErrObject(ERRObj);
		}
	}

}