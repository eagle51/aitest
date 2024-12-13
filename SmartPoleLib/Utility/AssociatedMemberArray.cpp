#include "AssociatedMemberArray.h"

namespace SmartPoleCore
{
	AssociatedMemberArray::AssociatedMemberArray(const HandleVec3D &pPoint)
		:_centrlPoint(pPoint)
	{

	}


	AssociatedMemberArray::~AssociatedMemberArray()
	{

	}

	bool AssociatedMemberArray::bCheckLineOverlap() const
	{
		bool res = false;
		if (_aCnnMembArray.size() < 2)
			return res;
		lineOverlapCheckArray.clear();
		for (std::vector<HandleMember>::const_iterator pElmIter = _aCnnMembArray.begin(); pElmIter < _aCnnMembArray.end(); ++pElmIter)
		{
			HandlePoint pStartPt = (*pElmIter)->getStartPt();
			HandlePoint pEndPt = (*pElmIter)->getEndPt();
			if (pStartPt->TagNum() == _centrlPoint->TagNum())
				LineVec3d = pStartPt->Subtraction(pEndPt.GetPointee());
			else
				LineVec3d = pEndPt->Subtraction(pStartPt.GetPointee());

			LineVec3d.normalize();
			for (vector<CVec3D>::iterator iIter = lineOverlapCheckArray.begin(); iIter<lineOverlapCheckArray.end();++iIter)
			{
				if (iIter->CoordEqual(&LineVec3d))
					return res = true;//当以一点为中心的所有线的单位向量相同的时候，表面存在重合的情况。
			}
			if (!res)
				lineOverlapCheckArray.push_back(LineVec3d);	
		}
		return res;
	}

	void AssociatedMemberArray::setCrossArmByLegDiameter()
	{
		HandleMember columnMember;
		for (size_t iloop = 0; iloop < _aCnnMembArray.size();++iloop)
		{
			HandleMember pCurMemb = _aCnnMembArray.at(iloop);
			if (pCurMemb->getTYPE() == Member::Column)
			{
				HandlePoint pStartPoint = pCurMemb->getStartPt();
				HandlePoint pEndPoint = pCurMemb->getEndPt();
				CVec3D vecLine = pEndPoint->Subtraction(pStartPoint.GetPointee());
				vecLine.normalize();
				if (abs(vecLine.z())>0.85)
				{
					columnMember = _aCnnMembArray[iloop];
					break;
				}
			}
		}
		if (columnMember.isNull())
			return;
		HandleProfile pColumnProfile = columnMember->getStartProfile();
		if (_centrlPoint->TagNum()==columnMember->getEndNum())
			pColumnProfile = columnMember->getEndProfile();
		for (size_t iloop = 0; iloop < _aCnnMembArray.size(); ++iloop)
		{
			HandleMember pCurMemb = _aCnnMembArray.at(iloop);
			if (pCurMemb->getTYPE() != Member::Column)
			{
				HandlePoint pStartPoint = pCurMemb->getStartPt();
				HandlePoint pEndPoint = pCurMemb->getEndPt();
				CVec3D vecLine = pEndPoint->Subtraction(pStartPoint.GetPointee());
				vecLine.normalize();
				if (abs(vecLine.z())<0.85)
				{
					if (pCurMemb->getStartNum() == _centrlPoint->TagNum())
						pCurMemb->_StartCnnDiameter = pColumnProfile->getWide()*0.0005;
					else
						pCurMemb->_EndCnnDiameter = pColumnProfile->getWide()*0.0005;
				}
			}
		}


	}

	SmartPoleCore::CVec3D AssociatedMemberArray::LineVec3d=CVec3D();

	vector<CVec3D> AssociatedMemberArray::lineOverlapCheckArray = vector<CVec3D>();

}