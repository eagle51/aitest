#include "MemberArray.h"
#include <set>
#include "Constants.h"
#include "CodeRegister.h"
#include "ExceptionMessage.h"

namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月21日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	struct __MemberSetCompare
	{
		static bool GroupNumCompare(const HandleMemberInf &pSect1, const HandleMemberInf &pSect2)
		{
			return pSect1->OrderIndex() < pSect2->OrderIndex();
		}
		static bool OrderCompare(const HandleMemberInf &pSect1, const HandleMemberInf &pSect2)
		{
			return pSect1->OrderIndex() < pSect2->OrderIndex();
		} 
	};
	MemberArray::MemberArray()
	{
	}

	MemberArray::MemberArray(const MemberArray* rhs)
	{
		_MembVec = rhs->_MembVec;
	}

	MemberArray::~MemberArray()
	{
	}

	
	SmartTower::HandleMemberInf MemberArray::find(int j1Num, int j2Num) const
	{
		if (j1Num > j2Num)
			std::swap(j1Num, j2Num);
		for (vector<HandleMemberInf>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			if ((*cIter)->MinTagNum() == j1Num && (*cIter)->MaxTagNum() == j2Num)
				return *cIter;
		}
		return NULL;
	}

	int MemberArray::remove(const HandleMemberInf& pMemb)
	{
		int iPos = this->refContains(pMemb);
		if (iPos < 0)
			return -1;
		_MembVec.erase(_MembVec.begin() + iPos);
		return iPos;
	}

	int MemberArray::refContains(const HandleMemberInf& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandleMemberInf>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter - _MembVec.begin());
		}
		return -1;
	}

	int MemberArray::contains(int j1Num, int j2Num) const
	{
		if (j1Num > j2Num)
			std::swap(j1Num, j2Num);
		
		for (size_t iloop = 0; iloop < _MembVec.size(); ++iloop)
		{
			HandleMemberInf pTmpMemb = _MembVec.at(iloop);
			if (pTmpMemb->MinTagNum() == j1Num && pTmpMemb->MaxTagNum() == j2Num)
				return int(iloop);
		}
		return -1;
	}
	void MemberArray::Renumbering(RenumberingList* Renumblist)
	{
		for (std::vector<HandleMemberInf>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
		{
			(*cIter)->Renumbering(Renumblist);
		}
	}
	SmartTower::MemberArray* MemberArray::DeepCopy() const
	{
		MemberArray* pNewArray = new MemberArray();
		pNewArray->_MembVec.reserve(_MembVec.size());
		for (size_t iloop = 0; iloop < _MembVec.size(); ++iloop)
		{
			HandleMemberInf pTmpMemb = _MembVec.at(iloop);
			pTmpMemb.MakeCopy();
			pNewArray->_MembVec.push_back(pTmpMemb);
		}
		return pNewArray;
	}

	MemberArray* MemberArray::getMainArray() const
	{
		MemberArray* pNewArray = new MemberArray();
		for (std::vector<HandleMemberInf>::const_iterator Iter = _MembVec.begin(); Iter < _MembVec.end(); ++Iter)
		{
			if ((*Iter)->MaxTagNum() < Constants::MinAuxTagNum)
				pNewArray->_MembVec.push_back(*Iter);
		}
		return pNewArray;
	}

	SmartTower::MemberArray* MemberArray::getLegTypeArray() const
	{
		MemberArray* pNewArray = new MemberArray();
		for (std::vector<HandleMemberInf>::const_iterator Iter = _MembVec.begin(); Iter < _MembVec.end(); ++Iter)
		{
			if ((*Iter)->Type() < 2)
				pNewArray->_MembVec.push_back(*Iter);
		}
		return pNewArray;
	}

	SmartTower::MemberArray* MemberArray::getOtherArray() const
	{
		MemberArray* pNewArray = new MemberArray();
		for (std::vector<HandleMemberInf>::const_iterator Iter = _MembVec.begin(); Iter < _MembVec.end(); ++Iter)
		{
			if ((*Iter)->Type() >1)
				pNewArray->_MembVec.push_back(*Iter);
		}
		return pNewArray;
	}

	//function ---by raoyi  2022/1/21 15:43
	void MemberArray::sortByOrderIndex()
	{
		std::sort(_MembVec.begin(), _MembVec.end(), __MemberSetCompare::OrderCompare);
	}


    //function ---by raoyi  2022/6/9 11:30
    void MemberArray::check()
    {
        for (std::vector<HandleMemberInf>::const_iterator cIter = _MembVec.begin(); cIter < _MembVec.end(); ++cIter)
        {
            (*cIter)->check();
        }
    }

}
