//#include "StdAfx.h"
#include "NodeManager.h"
#include "ExceptionMessage.h"
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月27日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	CNodeManager::CNodeManager(void):m_iStartLegNum(1),m_iEndLegNum(1),_BodyCount(0),_isNormalTower(true)
	{

	}
	CNodeManager::~CNodeManager(void)
	{

	};
	void CNodeManager::AddPart(HandlePart pPart,int iPos)
	{		
		vector<HandlePart>::iterator pIter=_PartArray.begin();
		if(iPos>=0 && int(_PartArray.size())>iPos)
			pIter=_PartArray.begin()+iPos;
		else
			pIter=_PartArray.end();
		//如果没有提供辅助材点号的范围，则程序自动提供
		if(AddAuxPoint && pPart->m_AuxMAX<Constants::MinAuxTagNum)
			pPart->m_AuxMAX=Constants::MinAuxTagNum*(size_t(pIter-_PartArray.begin())+2)-1;		
		_PartArray.insert(pIter,pPart);
		if(pPart->GetObjType()==TowBaseObject::BASEPART)
			_BodyCount++;		
	}

	void CNodeManager::checkConnectNumErr()
	{
		for (size_t iloop = 1; iloop<_BodyCount; iloop++)
		{
			HandlePart pBasePart = _PartArray.at(iloop);
			if (pBasePart->LB().BodyPartIndex()>pBasePart->getPartIndex())
				pBasePart->LB(0);
			int iCNNNum = pBasePart->ConnectionIndex();
			if (iCNNNum < 0 || iCNNNum >= int(_BodyCount))
			{
				CExceptionMessageRegister::Instance()->SendErrObject(new PartException(pBasePart.GetPointee(), iCNNNum, PartException::CNNErr));
				continue;
			}
			HandlePart upBasePart = _PartArray.at(iCNNNum);
			HdElemArray pBodyElms = pBasePart->getBodyElmArray();
			HdElemArray pFdtElms = pBasePart->getFdtElmArray();
			if (!pBodyElms->Empty() && !pFdtElms->Empty())
			{
				int cnnMinTagNum = pBodyElms->getMinConnectTagNum();
				int legMinTagNum = pFdtElms->getMinConnectTagNum();
				if (cnnMinTagNum > legMinTagNum)
					cnnMinTagNum = legMinTagNum;
				if (cnnMinTagNum<upBasePart->FminTagNum() && cnnMinTagNum>upBasePart->UpMaxTagNum())
					CExceptionMessageRegister::Instance()->SendErrObject(new PartException(pBasePart.GetPointee(), cnnMinTagNum, PartException::ErrFmin));
				else if ((cnnMinTagNum<Constants::MinAuxTagNum && cnnMinTagNum>upBasePart->FmaxTagNum()) || cnnMinTagNum < upBasePart->UpMaxTagNum())
					CExceptionMessageRegister::Instance()->SendErrObject(new PartException(pBasePart.GetPointee(), cnnMinTagNum, PartException::ErrFmin));
			}
		};
	}

	void CNodeManager::setBodyTransitionTminTmax()
	{
		//设置连接段的索引
		if (!_PartArray.empty())
		{
			_PartArray[0]->_cnnFmin = -1;
			_PartArray[0]->_cnnFmax = -1;
			_PartArray[0]->_cnnPart = NULL;
		}

		//设置连接的cnn
		for (size_t iloop = 1; iloop<_PartArray.size(); iloop++)
		{
			HandlePart pCurPart = _PartArray.at(iloop);
			int iCNNNum = pCurPart->ConnectionIndex();
			if (iCNNNum >= int(this->getBodyCount()))
				continue;
			if (iCNNNum>-1)
			{
				_PartArray[iloop]->_cnnFmin = _PartArray[iCNNNum]->FminTagNum();
				_PartArray[iloop]->_cnnFmax = _PartArray[iCNNNum]->FmaxTagNum();
				_PartArray[iloop]->_cnnPart = _PartArray[iCNNNum].GetPointee();
			}
		}
	}

	void CNodeManager::setMutiTowerLB()
	{
		for (size_t iloop = 0; iloop < getLegCount(); iloop++)
		{
			HandleLegPart pLegPart = getLegPart_At(iloop);
			HdElemArray pBodyElms = pLegPart->getBodyElmArray();
			HdElemArray pFdtElms = pLegPart->getFdtElmArray();
			if (pBodyElms->Empty() && pFdtElms->Empty())
				continue;
			HdNodeArray pBodyNodes = pLegPart->getBodyNodeArray();
			if (!pBodyNodes->empty())
			{
				HandleNode pNodeStart = pBodyNodes->front();
				if (!pNodeStart.isNull() && pNodeStart->TagNum() != pLegPart->JminTagNum())
					pLegPart->JminTagNum(pNodeStart->TagNum());
			}
			int iCNNNum = pLegPart->ConnectionIndex();
			if (iCNNNum >= int(this->getBodyCount()))
			{
				CExceptionMessageRegister::Instance()->SendErrObject(new PartException(pLegPart.GetPointee(), iCNNNum, PartException::CNNErr));
				continue;
			}
			else if (iCNNNum>-1)
			{
				HandlePart pUpBasePart = _PartArray.at(iCNNNum);
				int cnnMinTagNum = pBodyElms->getMinConnectTagNum();
				int legMinTagNum = pFdtElms->getMinConnectTagNum();
				if (cnnMinTagNum > legMinTagNum)
					cnnMinTagNum = legMinTagNum;
				if (cnnMinTagNum<pUpBasePart->FminTagNum() && cnnMinTagNum>pUpBasePart->UpMaxTagNum())
					CExceptionMessageRegister::Instance()->SendErrObject(new PartException(pUpBasePart.GetPointee(), cnnMinTagNum, PartException::ErrFmin));
				else if ((cnnMinTagNum > pUpBasePart->FmaxTagNum() && cnnMinTagNum < Constants::MinAuxTagNum) || cnnMinTagNum <= pUpBasePart->UpMaxTagNum())
					CExceptionMessageRegister::Instance()->SendErrObject(new PartException(pLegPart.GetPointee(), cnnMinTagNum, PartException::ErrFmin));
			}
			for (int kloop = 0; kloop <pFdtElms->size(); ++kloop)
			{
				HandleElement pElm1 = pFdtElms->at(kloop);
				if (pElm1->StartNum() >= pLegPart->FminTagNum() && pElm1->StartNum() < pLegPart->FmaxTagNum())
				{
					CExceptionMessageRegister::Instance()->SendErrObject(new PartException(pLegPart.GetPointee(), pElm1->StartNum(), PartException::FDTNumErr));
				}
			}
		}
	} 

	void CNodeManager::setPartIndexAndUpMaxNum()
	{
		HandlePart pFistPart = _PartArray.front();
		pFistPart->m_upFmax = -1;
		pFistPart->_PartIndex=0;
		pFistPart->m_UpAuxTagMax = Constants::MinAuxTagNum - 1;
		pFistPart->LB(0);
		pFistPart->_cnnPart = NULL;
		pFistPart->_cnnFmin = -1;
		pFistPart->_cnnFmax = -1;

		LegPartLB::BodyGreaterThan10 = false;
		if (this->getBodyCount() > 9)
			LegPartLB::BodyGreaterThan10 = true;

		//设置上一部分的最大值
		for (size_t iloop = 1; iloop < _PartArray.size(); iloop++)
		{
			HandlePart pUpPart = _PartArray.at(iloop-1);
			HandlePart pCurPart = _PartArray.at(iloop);
			pCurPart->_PartIndex=iloop;
			pCurPart->m_upFmax=pUpPart->FmaxTagNum();
			pCurPart->m_UpAuxTagMax = pUpPart->m_AuxMAX;
			pCurPart->_cnnPart = NULL;
			pCurPart->_cnnFmin = -1;
			pCurPart->_cnnFmax = -1;
		};
	}

	void CNodeManager::initial(NodeArray *globalNodeArray, ElemArray *globalElmArray)
	{	
		if (_PartArray.empty())
			return;
		
		this->setPartIndexAndUpMaxNum();
		
		//设置初始化和Part索引
		for(size_t iloop=0;iloop<_PartArray.size();iloop++)
			_PartArray[iloop]->initial(globalNodeArray,globalElmArray);

		//检查连接上端的最小之
		this->checkConnectNumErr();

		if(this->getLegCount()<1)
			CExceptionMessageRegister::Instance()->SendErrMessage("警告：目前还没有接腿！");

		//设置过渡区的最小最大值
		this->setBodyTransitionTminTmax();
		
		
		if(this->getLegCount()>0 && _BodyCount>0 && !this->IsSglTower())
		{
			this->setMutiTowerLB();
		}

		for(size_t iloop=1;iloop<_PartArray.size();iloop++)
		{
			if(_PartArray[iloop-1]->FmaxTagNum()>=_PartArray[iloop]->JminTagNum())
				CExceptionMessageRegister::Instance()->SendErrObject(new PartException(_PartArray[iloop-1],_PartArray[iloop-1]->FmaxTagNum(),PartException::ERRNumUp));

			if(_PartArray[iloop]->FminTagNum()<_PartArray[iloop]->JminTagNum())
				CExceptionMessageRegister::Instance()->SendErrObject(new PartException(_PartArray[iloop], _PartArray[iloop]->FminTagNum(), PartException::ErrFmin));

			if (_PartArray[iloop]->extendNumErr())
				CExceptionMessageRegister::Instance()->SendErrObject(new PartException(_PartArray[iloop], _PartArray[iloop]->getExtendNum(), PartException::ExtendNumErr));

			if (_PartArray[iloop]->extendElmErr())
				CExceptionMessageRegister::Instance()->SendErrObject(new PartException(_PartArray[iloop], _PartArray[iloop]->getExtendNum(), PartException::ExtendElmErr));

			if(_PartArray[iloop]->FmaxTagNum()<_PartArray[iloop]->FminTagNum())
				CExceptionMessageRegister::Instance()->SendErrObject(new PartException(_PartArray[iloop], _PartArray[iloop]->FmaxTagNum(), PartException::ErrFmin));

			if(_PartArray[iloop-1]->AuxMaxTagNum()>_PartArray[iloop]->AuxMaxTagNum())
				CExceptionMessageRegister::Instance()->SendErrObject(new PartException(_PartArray[iloop-1],_PartArray[iloop-1]->AuxMaxTagNum(),PartException::ERRNumUp));
		}

	}


	HandleLegPart CNodeManager::getLegPart_At(size_t idx) const
	{
		if ((idx + _BodyCount) >= _PartArray.size())
			throw exception("超出接腿表索引范围");
		return static_cast<CLegPart*>(_PartArray.at(_BodyCount + idx).GetPointee());
	};
	StHandle<CBasePart> CNodeManager::InPart(int iNum)
	{
		for(size_t iloop=0;iloop<_PartArray.size();iloop++)
		{
			if(_PartArray[iloop]->bInPart(iNum))
				return _PartArray[iloop];
		};
		return 0;
	}


	bool CNodeManager::IsAFoundationNode(const HandleNode& pNode) const
	{
		for (size_t iloop=_BodyCount;iloop<_PartArray.size();iloop++)
		{
			if (_PartArray[iloop]->FminTagNum()<=pNode->TagNum() && pNode->TagNum()<=_PartArray[iloop]->FmaxTagNum())
			{
				return true;
			}			
		}
		return false;
	}

	SmartTower::CBox3D CNodeManager::getLegMaxBox() const
	{
		CBox3D legBox;
		for (size_t iloop = 0; iloop < getLegCount(); iloop++)
		{
			HandleLegPart pLegPart = getLegPart_At(iloop);
			HdElemArray pFdtElms = pLegPart->getFdtElmArray();
			if (!pFdtElms->Empty())
				legBox.extend(&pFdtElms->GetBox());
		}
		return legBox;
	}

	CNodeManager* CNodeManager::DeepCopy()
	{
		CNodeManager* res=new CNodeManager();
		res->_BodyCount=_BodyCount;
		res->m_iStartLegNum=m_iStartLegNum;
		res->m_iEndLegNum=m_iEndLegNum;
		vector<StHandle<CBasePart>>* pPartArray=&res->_PartArray;
		for (vector<StHandle<CBasePart>>::iterator Iter=_PartArray.begin();Iter<_PartArray.end();Iter++)
		{
			pPartArray->push_back((*Iter)->DeepCopy());
		}
		return res;
	}

	int CNodeManager::ReomvePart( HandlePart pPart )
	{
		for (vector<StHandle<CBasePart>>::iterator Iter=_PartArray.begin();Iter<_PartArray.end();Iter++)
		{
			if(Iter->GetPointee()==pPart.GetPointee())
			{
				int res=int(distance(_PartArray.begin(),Iter));
				if(pPart->GetObjType()==TowBaseObject::BASEPART)
					--_BodyCount;
				_PartArray.erase(Iter);
				return res;
			}
		}
		return -1;
	}	

	int CNodeManager::getTTA_NE() const
	{
		if(_PartArray.empty())
			return 999;
		else if(!_isNormalTower)
			return -(int(_BodyCount*100)+int(_PartArray.size())-int(_BodyCount));			
		else if(_BodyCount==0 && _PartArray.size()==1)
			return 0;
		else
			return (_BodyCount*100+_PartArray.size()-_BodyCount);
	};

	bool CNodeManager::hasLegExtendBody() const
	{
		for (size_t legloop = 0; legloop < this->getLegCount(); legloop++)
		{
			HandleLegPart pCurLeg = this->getLegPart_At(legloop);
			if (pCurLeg->getExtendNum()>9)
				return true;
		}
		return false;
	}

	bool CNodeManager::IsLegSingleIter() const
	{
		if(_BodyCount==0 && _PartArray.size()==1 && _isNormalTower)
		{
			HandleLegPart pPart=static_cast<CLegPart*>(_PartArray.at(0).GetPointee());
			HdNodeArray pFdtNodes = pPart->getFdtNodeArray();
			if (pFdtNodes->empty())
				return !_isNormalTower;

			HandleNode pNodeFmin = pFdtNodes->front();
			HandleNode pNodeFmax = pFdtNodes->back();
			if(!pNodeFmax.isNull() && !pNodeFmin.isNull() && abs(pNodeFmax->TagNum()-pNodeFmin->TagNum())>5)
				return false;
			return true;//只要是单基塔他肯定是单迭代的！！！！不受控制影响
		}
		//如果是正常塔就要全迭代，不是正常塔就单一迭代
		return !_isNormalTower;
	}

	bool CNodeManager::ConnectToPart(int partIndex,HandlePart pLegPart)
	{
		if (partIndex == pLegPart->getPartIndex())
			return true;
		int iCNNNum=pLegPart->ConnectionIndex();
		while(iCNNNum>=0)
		{
			if (iCNNNum == partIndex)
				return true;
			HandlePart tmpPart=_PartArray.at(iCNNNum);
			iCNNNum=tmpPart->ConnectionIndex();
		}
		return false;
	}


    //@function  -- by raoyi  2023/5/8 14:55
    double CNodeManager::getTowerSlope(NodeArray* pNodeArray) const
    {
        if(this->getLegCount() <= 0 || pNodeArray == NULL || pNodeArray->empty())
            return -1.0;

        HandleLegPart tmpLegPart = this->getLegPart_At(0);
        int pJmin = tmpLegPart->JminTagNum();
        int pFmin = tmpLegPart->FminTagNum();

        HandleNode pNode1 = pNodeArray->find(pJmin);
        HandleNode pNode2 = pNodeArray->find(pFmin);

        if(!pNode1.isNull() && !pNode2.isNull())
        {
            if(abs(abs(pNode1->z()) - abs(pNode2->z())) < Constants::DISTANCE_EPSILON)
                return -1.0;
            else
                return abs(abs(pNode1->x()) - abs(pNode2->x())) / abs(abs(pNode1->z()) - abs(pNode2->z()));
        }
        return -1.0;
    }

	SmartTower::IntVector CNodeManager::maxHighCnnPartVec()
	{
		IntVector maxHighLGNVec;
		HandleLegPart pLegPart = this->getMaxHighLegPart();
		int iCNNNum = pLegPart->ConnectionIndex();
		while (iCNNNum >= 0)
		{
			maxHighLGNVec.push_back(iCNNNum);
			HandlePart tmpPart = this->At(iCNNNum);
			iCNNNum = tmpPart->ConnectionIndex();
		}

		maxHighLGNVec.push_back(pLegPart->getPartIndex());
		return maxHighLGNVec;
	}


	SmartTower::HandleLegPart CNodeManager::getMaxHighLegPart() const
	{
		HandleLegPart pLegPart;
		double TotalMaxZ=-1000;
		for (size_t iloop=0;iloop<this->getLegCount();++iloop)
		{
			HandleLegPart tmpLegPart=this->getLegPart_At(iloop);
			double dMaxZ=tmpLegPart->GetMaxZ();
			if(dMaxZ>TotalMaxZ)
			{
				pLegPart=tmpLegPart;
				TotalMaxZ=dMaxZ;
			}
		}
		return pLegPart;
	}

	SmartTower::HandleLegPart CNodeManager::getLegPartThrZ(double zCoor) const
	{
		for (size_t iloop=0;iloop<this->getLegCount();++iloop)
		{
			HandleLegPart tmpLegPart=this->getLegPart_At(iloop);
			double dMaxZ=tmpLegPart->GetMaxZ();
			if(abs(dMaxZ-zCoor)<Constants::DISTANCE_EPSILON0_01)
				return tmpLegPart;
		}
		return NULL;
	}
	bool CNodeManager::AddAuxPoint=true;

}