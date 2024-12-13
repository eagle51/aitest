//#include "StdAfx.h"
#include "NGPManager.h"
#include "ExceptionMessage.h"
#include "NGPException.h"

namespace SmartPoleCore
{
	NGPManager::NGPManager(int iStartCaseNum,int iEndCaseNum)
		:m_iStartCaseNum(iStartCaseNum), m_iEndCaseNum(iEndCaseNum), m_iStartLegNum(0), m_iEndLegNum(0)
	{
		if(m_iStartCaseNum<1)
			m_iStartCaseNum=1;
		if(m_iEndCaseNum<1)
			m_iEndCaseNum=1;
	}
	NGPManager::~NGPManager(void)
	{
	}


	NGPManager* NGPManager::DeepCopy()
	{
		NGPManager* res=new NGPManager();
		res->m_iStartCaseNum=m_iStartCaseNum;
		res->m_iEndCaseNum=m_iEndCaseNum;
		vector<HandleNGP>* pNGPArray=&res->_NGPArray;
		for (vector<HandleNGP>::iterator Iter=_NGPArray.begin();Iter<_NGPArray.end();Iter++)
		{
			HandleNGP pNEG=Iter->GetPointee();
			pNEG.MakeCopy();
			pNGPArray->push_back(pNEG);
		}
		return res;
	}

	int NGPManager::removeRef(const HandleNGP& pNGP )
	{
		for (vector<HandleNGP>::iterator Iter=_NGPArray.begin();Iter<_NGPArray.end();Iter++)
		{
			if(Iter->GetPointee()==pNGP.GetPointee())
			{
				size_t result=std::distance(_NGPArray.begin(),Iter);
				_NGPArray.erase(Iter);
				return result;
			}
		}
		return -1;
	}

	int NGPManager::Has( const HandleNGP& pNGP )
	{
		for (size_t iloop=0;iloop<_NGPArray.size();iloop++)
		{
			HandleNGP pcurNGP=_NGPArray.at(iloop);
			if(pcurNGP->equal(pNGP.GetPointee()))
				return int(iloop);
		}
		return -1;
	}

	void NGPManager::Check( int TotalLegCount,int TotalCaseCount)
	{
		//修正工况的范围
		if(m_iStartCaseNum<0)
			m_iStartCaseNum=1;
		if(m_iEndCaseNum<m_iStartCaseNum)
			m_iEndCaseNum=m_iStartCaseNum;
		if(m_iEndCaseNum>TotalCaseCount)
			m_iEndCaseNum=TotalCaseCount;
		//如果没有特别的NGP则不需要检查
		if(_NGPArray.empty() || TotalLegCount<1 || TotalCaseCount<1)
			return;
		//不允许只有一个荷载分组
		if(_NGPArray.size()==1)
		{
			ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(_NGPArray.front(), NGPException::onlyOneNGP));
			return;
		}
		ActiveCaseFlag.zeros(TotalCaseCount);
		ActiveLegFlag.zeros(TotalLegCount);
		//ActiveBetaZFlag.zeros(iMaxBetaZCount);
		for (size_t iloop=0;iloop<_NGPArray.size();++iloop)
		{
			HandleNGP pcurNGP=_NGPArray.at(iloop);
			pcurNGP->setActiveRangeCase(m_iStartCaseNum,m_iEndCaseNum);

			if(pcurNGP->getStartLegIndex()>pcurNGP->getEndLegIndex())
				ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(pcurNGP, NGPException::legErrorRange, pcurNGP->getEndLegIndex()));

			if(pcurNGP->getStartCaseIndex()>pcurNGP->getEndCaseIndex())
				ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(pcurNGP, NGPException::caseErrorRange, pcurNGP->getEndCaseIndex()));

			if(pcurNGP->getEndCaseIndex()>TotalCaseCount)
				ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(pcurNGP, NGPException::exceedCaseRange, pcurNGP->getEndCaseIndex()));

			for (int caseLoop=pcurNGP->getStartCaseIndex()-1;caseLoop<pcurNGP->getEndCaseIndex();++caseLoop)
				if(caseLoop<ActiveCaseFlag.size())
					++ActiveCaseFlag[caseLoop];

			for (int legLoop=pcurNGP->getStartLegIndex()-1;legLoop<pcurNGP->getEndLegIndex();++legLoop)
				if(legLoop<ActiveLegFlag.size())
					++ActiveLegFlag[legLoop];

			if(pcurNGP->getEndCaseIndex()>TotalCaseCount)
				ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(pcurNGP, NGPException::exceedCaseRange, pcurNGP->getEndCaseIndex()));

			if(pcurNGP->getEndLegIndex()>TotalLegCount)
				ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(pcurNGP, NGPException::exceedLegRange, pcurNGP->getEndLegIndex()));
		}

		for (size_t iloop=0;iloop<_NGPArray.size();++iloop)
		{
			HandleNGP pcurNGP=_NGPArray.at(iloop);
			for (size_t jloop=iloop+1;jloop<_NGPArray.size();++jloop)
			{
				HandleNGP pJNGP=_NGPArray.at(jloop);
				if(pcurNGP->equal(pJNGP.GetPointee()))
				{
					ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(pcurNGP, NGPException::legOverlap, pcurNGP->getStartLegIndex()));
					break;
				}
			}
		}

		int caseCheckFlag=true;
		for (int iNloop=0;iNloop<TotalCaseCount;++iNloop)
		{//检查是否遗漏工况
			if(ActiveCaseFlag[iNloop]==0)
			{//检查是否遗漏工况

				ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(_NGPArray.front(), NGPException::caseLoss, iNloop + 1));
				break;
			}
			else if(caseCheckFlag && ActiveCaseFlag[0]!=ActiveCaseFlag[iNloop])
			{
				caseCheckFlag=false;
				ExceptionMessageRegister::Instance()->SendWorningObject(new NGPException(_NGPArray.front(), NGPException::caseOverlap, iNloop + 1));
			}
		}

		for (int iNloop=0;iNloop<TotalLegCount;++iNloop)
		{//检查是否遗漏接腿
			if(ActiveLegFlag[iNloop]==0)
			{
				ExceptionMessageRegister::Instance()->SendErrObject(new NGPException(_NGPArray.front(), NGPException::legLoss, iNloop + 1));
				break;
			}
			else if(ActiveLegFlag[iNloop]>1)
			{//检查是否一个腿属于两个荷载分组中
				ExceptionMessageRegister::Instance()->SendWorningObject(new NGPException(_NGPArray.front(), NGPException::legOverlap, iNloop + 1));
				break;
			}
		}
	}

	bool NGPManager::HasOverLapCase() const
	{
		if(_NGPArray.size()<2)
			return false;
		return true;
	}

	HandleNGP NGPManager::getPartNGP( int partIndex ) const
	{
		for (vector<HandleNGP>::const_iterator Iter=_NGPArray.begin();Iter<_NGPArray.end();Iter++)
			if((*Iter)->isLegInNGP(partIndex))
				return *Iter;
		return NULL;
	}

	void NGPManager::setTotalProcess(const HdProcessBar& pBar)
	{
		if (_NGPArray.empty())
		{
			int activeLegCount = m_iEndLegNum - m_iStartLegNum+1;
			int activeCaseCount = m_iEndCaseNum - m_iStartCaseNum+1;
			pBar->setRange(activeLegCount*activeCaseCount);
		}
		else
		{
			int totalVal = 0;
			for (vector<HandleNGP>::const_iterator Iter = _NGPArray.begin(); Iter < _NGPArray.end(); Iter++)
			{
				int activeLegCount = (*Iter)->getActiveStartLeg() - (*Iter)->getActiveEndLeg() + 1;
				int activeCaseCount = (*Iter)->getActiveStartCase() - (*Iter)->getActiveEndCase() + 1;
				totalVal += (activeLegCount*activeCaseCount);
			}
			pBar->setRange(totalVal);
		}
	}

	IntVector NGPManager::ActiveLegFlag=IntVector();

	IntVector NGPManager::ActiveCaseFlag=IntVector();

}