#include "MemberArray.h"
#include "Constants.h"
#include "AssociatedMemberArray.h"
#include "PointErrException.h"
#include "ExceptionMessage.h"
#include "MemberException.h"
#include <algorithm>

namespace SmartPoleCore
{
	MemberArray::MemberArray()
	{
	}


	MemberArray::~MemberArray()
	{
	}

	int MemberArray::remove(const HandleMember& pMemb)
	{
		int iPos = this->refContains(pMemb);
		if (iPos < 0)
			return -1;
		_MembVec.erase(_MembVec.begin() + iPos);
		return iPos;
	}

	int MemberArray::refContains(const HandleMember& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandleMember>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter - _MembVec.begin());
		}
		return -1;
	}

	int MemberArray::contains(int J1, int J2) const
	{
		int minNum = J1;
		int maxNum = J2;
		if (minNum > maxNum)
			std::swap(minNum,maxNum);
		if (minNum<0 || minNum>Constants::SysMaxTagNum || maxNum<0 || maxNum>Constants::SysMaxTagNum)
			return -1;

		for (vector<HandleMember>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			if ((*cIter)->MinTagNum() == minNum && (*cIter)->MaxTagNum() == maxNum)
				return int(cIter - _MembVec.begin());
		}
		return -1;
	}

	void MemberArray::initAndCheck(PointMap* pointMap) const
	{
		for (std::vector<HandleMember>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			(*cIter)->Check(pointMap);
			checkMemVec(pointMap, *cIter);
		}
	}
	
	void MemberArray::checkMemVec(PointMap* pointMap, HandleMember memb)const
	{
		HandlePoint sTagNum = memb->CheckDia(pointMap, true);
		HandlePoint eTagNum = memb->CheckDia(pointMap, false);
		double sDia, eDia;
		if (!sTagNum.isNull())
			sDia = findAssMemsDia(sTagNum, memb);
		if (!eTagNum.isNull())
			eDia = findAssMemsDia(eTagNum, memb);
		memb->calcDia(sDia, sTagNum, eDia, eTagNum);
	}

	double MemberArray::findAssMemsDia(HandlePoint TagNum, HandleMember memb)const
	{
		vector<double>diaVec;
		HdAssociatedMembers AssMem = new AssociatedMemberArray(TagNum.GetPointee());
		for (auto mem : _MembVec)
			AssMem->addElm(mem);
		if (AssMem->getCnnMembCount() == 0)
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new PointErrException(TagNum, PointErrException::IsLinkMem));
			return 0;
		}

		vector<HandleMember>AssMembVec;
		for (int i = 0; i < AssMem->getCnnMembCount(); ++i)
		{
			if (AssMem->getEndPointAt(i).isNull())
				continue;
			if (AssMem->getEndPointAt(i)->isInLine(*memb->getStartPt().GetPointee(), *memb->getEndPt().GetPointee()))//选取同一方向上的杆件
				AssMembVec.push_back(AssMem->getCnnMembAt(i));
		}
		if (AssMembVec.empty())
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(memb, MemberException::TypeErr));
			return 0;
		}

		for (auto Memb : AssMembVec)
		{
			if (TagNum->TagNum() == Memb->getStartNum())
			{
				if (Memb->getStartPt().isNull() || Memb->getStartProfile().isNull())
				{
					ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(memb, MemberException::TypeErr));
					return 0;
				}
			}
			if (TagNum->TagNum() == Memb->getEndNum())
			{
				if (Memb->getEndPt().isNull() || Memb->getEndProfile().isNull())
				{
					ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(memb, MemberException::TypeErr));
					return 0;
				}
			}
		}

		for (auto Memb : AssMembVec)
		{
			if (TagNum->TagNumEqual(Memb->getStartPt().GetPointee()))
				diaVec.push_back(Memb->getStartProfile()->getWide());
			if (TagNum->TagNumEqual(Memb->getEndPt().GetPointee()))
				diaVec.push_back(Memb->getEndProfile()->getWide());
		}

		for (int i = 0; i < diaVec.size() - 1; ++i)
		{
			if (diaVec[i] == diaVec[i + 1])
				continue;
			else
			{
				ExceptionMessageRegister::Instance()->SendErrObject(new MemberException(memb, MemberException::TypeErr));
				return 0;
			}
		}
		return diaVec[0];
	}



	SmartPoleCore::CBox3D MemberArray::GetBox() const
	{
		CBox3D result;
		for (vector<HandleMember>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			HandlePoint pStartNode = (*cIter)->getStartPt();
			if (!pStartNode.isNull())
				result.extend(pStartNode.GetPointee());

			HandlePoint pEndNode = (*cIter)->getEndPt();
			if (!pEndNode.isNull())
				result.extend(pEndNode.GetPointee());
		}
		return result;
	}

	MemberArray* MemberArray::getRangeMemberArray(int startTag, int endTag)
	{
		MemberArray* resultArray = new MemberArray();
		for (vector<HandleMember>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			if ((*cIter)->MaxTagNum() >= startTag && (*cIter)->MaxTagNum() <= endTag)
				resultArray->add(*cIter);
		}
		return resultArray;
	}

	void MemberArray::sortByEndZCoor()
	{
		std::sort(_MembVec.begin(), _MembVec.end(), endZCoorCompare);
	}

	void MemberArray::clearProfileForce() const
	{
		for (vector<HandleMember>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			(*cIter)->clearProfileForce();
		}
	}
	// @ function: 求该段总高  --- by lsq 2020/4/17 15:08
	double MemberArray::getMaxZ(bool isDrawZ)
	{
		double totalH = -1000.0;
		for (vector<HandleMember>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			HandlePoint pMaxZPoint = (*cIter)->getMaxZPoint(isDrawZ);
			if (pMaxZPoint.isNull())
				return totalH;
			
			double m_z;
			if (isDrawZ)
				m_z = pMaxZPoint->getDrawZ();
			else
				m_z = pMaxZPoint->z();

			if (m_z>totalH)
			{
				totalH = m_z;
			}
		}
		return totalH;
	}
	// @ function:   --- by lsq 2020/8/14 14:52
	double MemberArray::getMembsLen()
	{
		double totalH = 0;
		for (vector<HandleMember>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
			totalH += abs((*cIter)->getStartPt()->z() - (*cIter)->getEndPt()->z());
		return totalH;
	}

}