#include "FamilyManager.h"
#include "ExceptionMessage.h"
#include "FamilyException.h"

namespace SmartPoleCore
{
	FamilyManager::FamilyManager()
		:_BodyCount(0)
	{
	}


	FamilyManager::~FamilyManager()
	{
	}

	// @ function:  --- by raoyi  2020/03/24 15:06
	int FamilyManager::remove(const HandleComponent& rhs)
	{
		int iPos = this->refContains(rhs);
		if (iPos < 0)
			return -1;
		_ComponentVec.erase(_ComponentVec.begin() + iPos);
		return iPos;
	}

	// @ function:  --- by raoyi  2020/03/24 15:13
	int FamilyManager::refContains(const HandleComponent& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandleComponent>::const_iterator cIter = _ComponentVec.begin(); cIter < _ComponentVec.end(); ++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter - _ComponentVec.begin());
		}
		return -1;
	}

	HandleSinglePole FamilyManager::getSingleAt(int ind) const
	{
		HandleSinglePole pSignlePole = new SinglePole();
		if (empty())
			return NULL;
		HandleComponent pLegComponet = this->getLegAt(ind);
		pSignlePole->addComponent(pLegComponet.GetPointee());
		int iCNNNum = pLegComponet->getCnnIndex();
		while (iCNNNum>=0)
		{
			HandleComponent pBdyComp = _ComponentVec.at(iCNNNum);
			pSignlePole->addComponent(pBdyComp.GetPointee());
			iCNNNum = pBdyComp->getCnnIndex();
		}
		return pSignlePole;
	}

	void FamilyManager::initAndCheck()
	{
		if (this->getTotalCount() == 0)
			return;
		_BodyCount = 0;
		for (size_t iloop = 0; iloop < _ComponentVec.size(); iloop++)
		{
			HandleComponent pCurComp = _ComponentVec.at(iloop);
			pCurComp->_PartPos = iloop;//设置序号
			if (!pCurComp->isLegType())
				++_BodyCount;//接腿加+1

			if (pCurComp->getCnnIndex() >= 0 && pCurComp->getCnnIndex()<iloop)
			{//被挂接的不能是腿，必须是本体
				HandleComponent pUpComp = _ComponentVec.at(pCurComp->getCnnIndex());
				if (pUpComp->isLegType())//只能挂接在本体上
					ExceptionMessageRegister::Instance()->SendErrObject(new FamilyException(pCurComp, FamilyException::CnnIndexErr));
			}
			else if (iloop!=0)
				ExceptionMessageRegister::Instance()->SendErrObject(new FamilyException(pCurComp, FamilyException::CnnIndexErr));

			int upMaxNum = 0;
			if (iloop != 0)
			{
				HandleComponent pUpComp = _ComponentVec.at(iloop - 1);
				upMaxNum = pUpComp->getMaxNumber();
			}

			pCurComp->initAndCheck(upMaxNum);
		};
	}
	// @ function:   --- by lsq 2020/4/17 15:36
	SmartPoleCore::HandleSinglePole FamilyManager::getMaxHighPole() const
	{
		double maxH = -1000.0;
		int index = 0;
		for (int iloop = 0; iloop < _ComponentVec.size(); ++iloop)
		{
			if (_ComponentVec[iloop]->isLegType())
			{
				double h = _ComponentVec[iloop]->getMemberArray()->getMaxZ();
				if (h>maxH)
				{
					maxH = h;
					index = iloop;
				}
			}
		}
		if (!empty())
			_ComponentVec[index]->setIsDraw(true);
		return getSingleAt(index-this->getBodyCount());
	}





}