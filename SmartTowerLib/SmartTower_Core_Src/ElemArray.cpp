#include "ElemArray.h"
#include "NodeArray.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日初步改造------------------
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	struct __ElmKeyCompare
	{
		bool operator()(const HandleElement &plink1, long long iNum) const
		{
			return plink1->getKey() < iNum;
		}
		bool operator()(long long iNum, const HandleElement &plink1) const
		{
			return iNum < plink1->getKey();
		}
		bool operator()(const HandleElement &plink1, const HandleElement &plink2) const
		{
			return  plink1->getKey() < plink2->getKey();
		}
	};

	struct __ElmEndNumLessCompare
	{
		bool operator()(const HandleElement& plink1, int iNum) const
		{ 
			return plink1->EndNum() < iNum;
		}
		bool operator()(int iNum, const  HandleElement& plink1)const
		{
			return iNum < plink1->EndNum();
		}
		bool operator()(const HandleElement& plink1, const HandleElement& plink2)const
		{
			return  plink1->EndNum() < plink2->EndNum();
		}
	};

	struct __ElmCoorSort
	{
	public:
		static bool ElmCoorSortY(const HandleElement& pElm1, const HandleElement& pElm2);
		static bool ElmCoorSortOther(const HandleElement& pElm1, const HandleElement& pElm2);
		static bool ElmCoorSortLeg(const HandleElement& pElm1, const HandleElement& pElm2);
	};

	ElemArray::ElemArray()
	{
	}


	ElemArray::ElemArray(const ElemArray* pArray)
		:_ElemVec(pArray->_ElemVec)
	{

	}

	ElemArray::~ElemArray()
	{
	}

	void ElemArray::remove(const HandleMemberInf& pMemb1)
	{
		size_t index3 = 0;
		while (index3 < _ElemVec.size())
		{
			HandleElement pElm3 = _ElemVec.at(index3);
			HandleMemberInf pMemb3 = pElm3->GetMenber();
			if (pMemb1.GetPointee() == pMemb3.GetPointee())
				_ElemVec.erase(_ElemVec.begin() + index3);
			else
				++index3;
		}
	}

	int ElemArray::Remove(const HandleElement& pPoint)
	{
		int iPos = this->RefContains(pPoint);
		if (iPos < 0)
			return -1;
		_ElemVec.erase(_ElemVec.begin() + iPos);
		return iPos;
	}

	int ElemArray::RefContains(const HandleElement& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter - _ElemVec.begin());
		}
		return -1;
	}

	int ElemArray::Contains(int minj1, int maxj2) const
	{
		if (minj1 > maxj2)
			std::swap(minj1, maxj2);

		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			if ((*cIter)->StartNum() == minj1 && (*cIter)->EndNum() == maxj2)
				return int(cIter - _ElemVec.begin());
		}
		return -1;
	}

	SmartTower::ElemArray* ElemArray::filterRange(int largerQualNum, int lessMaxNum) const
	{
		ElemArray* newArray = new ElemArray();
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pTmpElm = _ElemVec.at(iloop);
			if (pTmpElm->EndNum() >= largerQualNum && pTmpElm->EndNum() < lessMaxNum)
				newArray->_ElemVec.push_back(pTmpElm);
		}
		return newArray;
	}

 	HandleElement ElemArray::BinarySearch(int j1, int j2) const
	{
 		long long _Elmkey = CBaseElement::CalcElmKey(j1, j2);
 		std::vector<HandleElement>::const_iterator curElmIter = std::lower_bound(_ElemVec.begin(), _ElemVec.end(), _Elmkey, __ElmKeyCompare());
 		if (curElmIter == _ElemVec.end() || (*curElmIter)->getKey() != _Elmkey)
 			return 0;
 		return curElmIter->GetPointee();
 	}

 	HandleElement ElemArray::BinarySearch(const HandleElement& pElm, Symetry::Type iYsm) const
 	{
		long long _Elmkey = CBaseElement::CalcElmKey(pElm->StartNode()->ISymTagNum(iYsm), pElm->EndNode()->ISymTagNum(iYsm));
		std::vector<HandleElement>::const_iterator curElmIter = std::lower_bound(_ElemVec.begin(), _ElemVec.end(), _Elmkey, __ElmKeyCompare());
		if (curElmIter == _ElemVec.end() || (*curElmIter)->getKey() != _Elmkey)
			return 0;
		return curElmIter->GetPointee();
 	}

	SmartTower::ElemArray* ElemArray::BinaryRange(int minNum, int maxNum) const
 	{
  		std::vector<HandleElement>::const_iterator minIter = lower_bound(_ElemVec.begin(), _ElemVec.end(), minNum, __ElmEndNumLessCompare());
		std::vector<HandleElement>::const_iterator maxIter = lower_bound(_ElemVec.begin(), _ElemVec.end(), maxNum, __ElmEndNumLessCompare());
  		ElemArray* newArray = new ElemArray();
  		for (std::vector<HandleElement>::const_iterator nIter = minIter; nIter < maxIter; ++nIter)
  			newArray->_ElemVec.push_back(*nIter);
  		return newArray;
 	}

	void ElemArray::addUniArray(const ElemArray* pArray)
	{
		for (int iloop = 0; iloop < pArray->size(); ++iloop)
		{
			HandleElement pInNode = pArray->at(iloop);
			int iPos = this->Contains(pInNode->StartNum(), pInNode->EndNum());
			if (iPos < 0)
				_ElemVec.push_back(pInNode);
		}
	}

	int ElemArray::getMinConnectTagNum() const
	{
		int iResult = Constants::MinAuxTagNum;
		for (std::vector<HandleElement>::const_iterator eIter = _ElemVec.begin(); eIter < _ElemVec.end(); ++eIter)
		{
			if ((*eIter)->StartNum() < iResult)
				iResult = (*eIter)->StartNum();
		}
		return iResult;
	}

	SmartTower::HandleNode ElemArray::findBracingUpNode(bool bFornt, int ifdtNum /*= -1*/)
	{
		if (ifdtNum < 0)
		{
			for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
			{
				HandleElement pElm = _ElemVec.at(iloop);
				//斜材
				if (pElm->EndNum()<Constants::MinAuxTagNum && pElm->GetMenber()->Type()>1)
				{
					HandleNode sNode = pElm->StartNode();
					if (bFornt && (sNode->y()*0.7)>sNode->x() && sNode->y() > 0.30 && sNode->x() > -Constants::DISTANCE_EPSILON)
						return pElm->StartNode();
					else if (!bFornt && sNode->y()<(sNode->x()*0.7) && sNode->x()>0.30 && sNode->y() > -Constants::DISTANCE_EPSILON)
						return pElm->StartNode();
				}
			}
		}
		else
		{
			for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
			{
				HandleElement pElm = _ElemVec.at(iloop);
				//斜材
				if (pElm->EndNum()<Constants::MinAuxTagNum && pElm->EndNum() == ifdtNum && pElm->GetMenber()->Type()>1)
				{
					HandleNode sNode = pElm->StartNode();
					if (bFornt && (sNode->y()*0.7)>sNode->x() && sNode->y() > 0.30 && sNode->x() > -Constants::DISTANCE_EPSILON)
						return pElm->StartNode();
					else if (!bFornt && sNode->y()<(sNode->x()*0.7) && sNode->x()>0.30 && sNode->y() > -Constants::DISTANCE_EPSILON)
						return pElm->StartNode();
				}
			}
		}
		return NULL;
	}



	SmartTower::HandleNode ElemArray::findFdtLegNode(int fdtTagNum) const
	{
		HandleElement pLEgElm = this->getFdtFrontBracingElm(fdtTagNum);
		if (pLEgElm.isNull())
			return NULL;
		return pLEgElm->EndNode();
	}


	SmartTower::HandleNode ElemArray::findMidAxisNode(const HandleNode& pLegUpNode, bool isXZero, double epsilonZero /*= 0.0*/)
	{
		if (pLegUpNode.isNull() || _ElemVec.empty())
			return NULL;

		CVec3D minCoor(pLegUpNode->x(), pLegUpNode->y(), pLegUpNode->z());
		if (isXZero)
			minCoor.setX(0.0);
		else
			minCoor.setY(0.0);

		double minDis = 1000.0;
		HandleNode resNode;
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode startNode = pElm->StartNode();
			HandleNode endNode = pElm->EndNode();
			double dDisS = minCoor.distanceTo(startNode.GetPointee());
			if (dDisS < minDis && abs(startNode->z() - minCoor.z()) < Constants::DISTANCE_EPSILON0_001)
			{
				if (isXZero && abs(minCoor.y() - startNode->y()) < 0.5)
				{
					minDis = dDisS;
					resNode = startNode;
				}
				else if (!isXZero && abs(minCoor.x() - startNode->x()) < 0.5)
				{
					minDis = dDisS;
					resNode = startNode;
				}

			}
			double dDisS2 = minCoor.distanceTo(endNode.GetPointee());
			if (dDisS2 < minDis && abs(endNode->z() - minCoor.z()) < Constants::DISTANCE_EPSILON0_001)
			{
				if (isXZero && abs(minCoor.y() - endNode->y()) < 0.5)
				{
					minDis = dDisS2;
					resNode = endNode;
				}
				else if (!isXZero && abs(minCoor.x() - endNode->x()) < 0.5)
				{
					minDis = dDisS2;
					resNode = endNode;
				}
			}
		}
		if (resNode.isNull())
			return NULL;

		if (isXZero)
		{
			if (epsilonZero < Constants::DISTANCE_EPSILON0_001)
				epsilonZero = abs(pLegUpNode->y()*0.4);
			if (resNode->x() < epsilonZero)
				return resNode;
			return NULL;
		}
		else
		{
			if (epsilonZero < Constants::DISTANCE_EPSILON0_001)
				epsilonZero = abs(pLegUpNode->x()*0.4);
			if (resNode->y() < epsilonZero)
				return resNode;
			return NULL;
		}
		return NULL;
	}

	SmartTower::HandleNode ElemArray::findMayLegUpNode(int fdtNum) const
	{
		if (_ElemVec.empty())
			return NULL;

		HandleNode resNode;
		HandleMemberInf frontMember;
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			if (resNode.isNull())
			{
				resNode = pElm->StartNode();
				frontMember = pElm->GetMenber();
			}

			HandleMemberInf pMemb = pElm->GetMenber();
			HandleNode pStartNode = pElm->StartNode();
			HandleNode pEndNode = pElm->EndNode();

			if (pMemb->Type() <= 1 && pStartNode->x() > 0.2 && pStartNode->y() > 0.2 && pEndNode->x() > 0.2 && pEndNode->y() > 0.2)
			{//如果是单基塔可能存在找不到的情况
				if (abs(pStartNode->z() - pEndNode->z())<Constants::DISTANCE_EPSILON)
					continue;

				if (frontMember->Type()>1)
				{
					frontMember = pMemb;
					if (pStartNode->z() < pEndNode->z())
						resNode = pStartNode;
					else
						resNode = pEndNode;
				}

				if (resNode->z() < pStartNode->z() || (resNode->z() == pStartNode->z() && resNode->x() <= pStartNode->x() && resNode->y() <= pStartNode->y()))
				{
					if (pStartNode->TagNum() < fdtNum)
					{
						frontMember = pMemb;
						resNode = pStartNode;
					}
				}

				if (resNode->z() < pEndNode->z() || (resNode->z() == pEndNode->z() && resNode->x() <= pEndNode->x() && resNode->y() <= pEndNode->y()))
				{
					if (pEndNode->TagNum() < fdtNum)
					{
						frontMember = pMemb;
						resNode = pEndNode;
					}
				}
			}
		}

		if (!frontMember.isNull() && frontMember->Type()>1 && abs(resNode->x() - resNode->y()) > Constants::EPSILON0_1)
		{//说明找到的主材端点可能不对，需要重新找
			for (size_t iloop2 = 0; iloop2 < _ElemVec.size(); ++iloop2)
			{
				HandleElement pElm2 = _ElemVec.at(iloop2);
				HandleNode pStartNode = pElm2->StartNode();
				if (abs(pStartNode->x() - pStartNode->y()) < Constants::EPSILON0_1)
				{
					if (resNode->z() < pStartNode->z())
						resNode = pStartNode;
				}
				HandleNode pEndNode = pElm2->EndNode();
				if (abs(pEndNode->x() - pEndNode->y()) < Constants::EPSILON0_1)
				{
					if (resNode->z() < pEndNode->z())
						resNode = pEndNode;
				}
			}
		}
		return resNode;
	}



	

	CBox3D ElemArray::GetBox() const
	{
		CBox3D result;
		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			HandleNode pStartNode = (*cIter)->StartNode();
			HandleNode pEndNode = (*cIter)->EndNode();
			result.extend(pStartNode.GetPointee());
			result.extend(pEndNode.GetPointee());
		}
		return result;
	}

	ElemArray* ElemArray::findColinear(const CVec3D* pStartVec, const CVec3D* pEndVec) const
	{

		CBox3D box;
		box.extend(pStartVec);
		box.extend(pEndVec);
		box.extend(0.001);
		ElemArray* pResult = new ElemArray();
		CVec3D dirVec = pEndVec->Subtraction(pStartVec);
		if (dirVec.norm() < Constants::DISTANCE_EPSILON0_001)
			return pResult;
		dirVec.normalize();
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode startNode = pElm->StartNode();
			HandleNode endNode = pElm->EndNode();
			if (box.contains(startNode.GetPointee()) && box.contains(endNode.GetPointee()))
			{
				CVec3D lineVec= pElm->getLineVec();
				CVec3D crossVec = lineVec.cross(dirVec);
				if (crossVec.norm() < Constants::DISTANCE_EPSILON)
					pResult->push_back(pElm);
			}
		}
		return pResult;
	}

	SmartTower::ElemArray* ElemArray::findDiaphramElms(const HandleNode& pKeyNode) const
	{
		ElemArray* pResult=new ElemArray();
		if (_ElemVec.empty())
			return pResult;
		HandleNode pXYNode = pKeyNode->getISNode(Symetry::Osym);
		CBox3D box;
		box.extend(pKeyNode.GetPointee());
		box.extend(pXYNode.GetPointee());
		box.extend(Constants::DISTANCE_EPSILON);
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode startNode = pElm->StartNode();
			HandleNode endNode = pElm->EndNode();
			if (box.contains(startNode.GetPointee()) && box.contains(endNode.GetPointee()))
				pResult->push_back(pElm);
		}
		return pResult;
	}

	ElemArray* ElemArray::findInBox3D(const CBox3D& rangeBox) const
	{
		ElemArray* pResElms = new ElemArray();
		if (_ElemVec.empty())
			return pResElms;

		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			HandleNode pStartNode = (*cIter)->StartNode();
			HandleNode pEndNode = (*cIter)->EndNode();
			if (rangeBox.contains(pStartNode.GetPointee()) && rangeBox.contains(pEndNode.GetPointee()))
				pResElms->push_back(*cIter);
		}
		return pResElms;
	}

	SmartTower::ElemArray* ElemArray::findZCoorLess(const HandleNode& pNode) const
	{
		ElemArray* resElms = new ElemArray();
		if (_ElemVec.empty() || pNode.isNull())
			return resElms;

		double dXCoor = abs(pNode->x());
		double dYCoor = abs(pNode->y());
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode startNode = pElm->StartNode();
			HandleNode endNode = pElm->EndNode();
			if (startNode->z() <= pNode->z() && endNode->z() <= pNode->z())
			{
				if ((startNode->z() - pNode->z())<-Constants::DISTANCE_EPSILON || (endNode->z() - pNode->z())<-Constants::DISTANCE_EPSILON)
					resElms->push_back(pElm);
				else if (abs(startNode->x())>dXCoor && abs(startNode->x())>dYCoor)
					resElms->push_back(pElm);
				else if (abs(endNode->x()) > dXCoor && abs(endNode->x()) > dYCoor)
					resElms->push_back(pElm);
			}
		}
		return resElms;
	}

	NodeArray* ElemArray::getConnectNodeArray() const
	{
		NodeArray* pResult = new NodeArray();

		if (_ElemVec.empty())
			return pResult;

		std::map<int, HandleNode> tmpNodeMap;
		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			tmpNodeMap.insert(pair<int, HandleNode>((*cIter)->StartNum(), (*cIter)->StartNode()));
			tmpNodeMap.insert(pair<int, HandleNode>((*cIter)->EndNum(), (*cIter)->EndNode()));
		}
		for (std::map<int, HandleNode>::iterator CpCIter = tmpNodeMap.begin(); CpCIter != tmpNodeMap.end(); ++CpCIter)
			pResult->push_back(CpCIter->second);
		return pResult;
	}

	// @ function:    --- by lsq 2021/6/3 16:32
	ElemArray* ElemArray::findCnnElms(int refTagNum)
	{
		ElemArray* resultArray = new ElemArray();
		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			if ((*cIter)->StartNum() == refTagNum || (*cIter)->EndNum() == refTagNum)
				resultArray->push_back(*cIter);
		}
		return resultArray;
	}

	MemberArray* ElemArray::getMemberArray() const
	{
		MemberArray* pMemberArray = new MemberArray();
		std::map<long long, HandleMemberInf> MembMap;
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleMemberInf pMemb = _ElemVec.at(iloop)->GetMenber();
			long long key = CBaseElement::CalcElmKey(pMemb->J1TagNum(), pMemb->J2TagNum());
			MembMap.insert(pair<long long, HandleMemberInf>(key, pMemb));
		}
		for (std::map<long long, HandleMemberInf>::iterator Iter = MembMap.begin(); Iter != MembMap.end(); ++Iter)
		{
			HandleMemberInf pMemb = Iter->second;
			pMemberArray->push_back(pMemb);
		}
		return pMemberArray;
	}

	bool ElemArray::hasMainElm() const
	{
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			if (pElm->EndNum()<Constants::MinAuxTagNum)
				return true;
		}
		return false;
	}

	double ElemArray::getMinZ() const
	{
		double resMinZ = 10000.0;
		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			HandleNode pStartNode = (*cIter)->StartNode();
			HandleNode pEndNode = (*cIter)->EndNode();
			if (pStartNode->z() < resMinZ)
				resMinZ = pStartNode->z();
			if (pEndNode->z() < resMinZ)
				resMinZ = pEndNode->z();
		}
		return resMinZ;
	}

	//function ---by raoyi  2022/1/18 20:45
	SmartTower::HandleElement ElemArray::find(int j1, int j2) const
	{
		if (j1>j2)
			std::swap(j1, j2);

		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			if ((*cIter)->StartNum() == j1 && (*cIter)->EndNum() == j2)
				return *cIter;
		}
		return NULL;
	}

	SmartTower::HandleElement ElemArray::findMembStartNumEqual(int j1, int j2) const
	{
		if (j1>j2)
			std::swap(j1, j2);

		for (vector<HandleElement>::const_iterator cIter = _ElemVec.begin(); cIter < _ElemVec.end(); ++cIter)
		{
			if ((*cIter)->StartNum() == j1 && (*cIter)->EndNum() == j2)
				return *cIter;
			HandleMemberInf pMemb = (*cIter)->GetMenber();
			if (pMemb->MinTagNum() == j1 && pMemb->MaxTagNum() == j2 && j1 == (*cIter)->StartNum())
				return *cIter;//内部相同，且起点认为是相同也认可的
		}
		return NULL;
	}


	SmartTower::HandleElement ElemArray::getFdtLegElm(int fdtTagNum) const
	{
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pTmpElm = _ElemVec.at(iloop);
			HandleMemberInf pMemb = pTmpElm->GetMenber();
			if (pTmpElm->EndNum()<Constants::MinAuxTagNum && pMemb->Type() <= 1 && abs(pTmpElm->CosZZ())>0.78 && (fdtTagNum == -1 || pTmpElm->EndNum()== fdtTagNum))
				return pTmpElm;
		}
		return NULL;
	}

	SmartTower::HandleMemberInf ElemArray::getFdtLegMemb(int fdtTagNum) const
	{
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pTmpElm = _ElemVec.at(iloop);
			HandleMemberInf pMemb = pTmpElm->GetMenber();
			if (pTmpElm->EndNum()<Constants::MinAuxTagNum && pMemb->Type() <= 1 && (fdtTagNum == -1 || abs(pMemb->MaxTagNum() - fdtTagNum) < 4))//如果没有输入就是第一个
				return pMemb;
		}
		return NULL;
	}

	SmartTower::HandleElement ElemArray::getFdtFrontBracingElm(int fdtTagNum) const
	{
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pTmpElm = _ElemVec.at(iloop);
			if (pTmpElm->EndNum()>Constants::MinAuxTagNum)
				continue;
			HandleMemberInf pMemb = pTmpElm->GetMenber();
			HandleNode pStartNode = pTmpElm->StartNode();
			if (pTmpElm->EndNum()<Constants::MinAuxTagNum && pMemb->Type()>1 && pMemb->Type()<4 && (fdtTagNum == -1 || abs(pMemb->MaxTagNum() - fdtTagNum)<4)//如果没有输入就是第一个 
				&& pStartNode->y()>pStartNode->x() && pStartNode->y()>0.4 && pStartNode->x() > -Constants::DISTANCE_EPSILON)
				return pTmpElm;
		}
		return NULL;
	}

	SmartTower::HandleMemberInf ElemArray::getfdtFrontTp2Memb(int fdtTagNum) const
	{
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pTmpElm = _ElemVec.at(iloop);
			if (pTmpElm->EndNum()>Constants::MinAuxTagNum)
				continue;
			HandleMemberInf pMemb = pTmpElm->GetMenber();
			HandleNode pStartNode = pTmpElm->StartNode();
			if (pTmpElm->EndNum()<Constants::MinAuxTagNum && pMemb->Type()>1 && pMemb->Type()<4 && (fdtTagNum == -1 || abs(pMemb->MaxTagNum() - fdtTagNum)<4)//如果没有输入就是第一个
				&& pStartNode->y()>pStartNode->x() && pStartNode->y()>0.4 && pStartNode->x() > -Constants::DISTANCE_EPSILON)
				return pMemb;
		}
		return NULL;
	}

	SmartTower::HandleElement ElemArray::getFdtLatrelBracingElm(int fdtTagNum) const
	{
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pTmpElm = _ElemVec.at(iloop);
			if (pTmpElm->EndNum()>Constants::MinAuxTagNum)
				continue;
			HandleMemberInf pMemb = pTmpElm->GetMenber();
			HandleNode pStartNode = pTmpElm->StartNode();
			if (pTmpElm->EndNum()<Constants::MinAuxTagNum && pMemb->Type()>1 && pMemb->Type()<4 && (fdtTagNum == -1 || abs(pMemb->MaxTagNum() - fdtTagNum)<4)//如果没有输入就是第一个
				&& pStartNode->x()>pStartNode->y() && pStartNode->x()>0.4 && pStartNode->y() > -Constants::DISTANCE_EPSILON)
				return pTmpElm;
		}
		return NULL;
	}

	SmartTower::HandleMemberInf ElemArray::getfdtLatrelTp2Memb(int fdtTagNum) const
	{
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pTmpElm = _ElemVec.at(iloop);
			if (pTmpElm->EndNum()>Constants::MinAuxTagNum)
				continue;
			HandleMemberInf pMemb = pTmpElm->GetMenber();
			HandleNode pStartNode = pTmpElm->StartNode();
			if (pTmpElm->EndNum()<Constants::MinAuxTagNum && pMemb->Type()>1 && pMemb->Type()<4 && (fdtTagNum == -1 || abs(pMemb->MaxTagNum() - fdtTagNum)<4)//如果没有输入就是第一个
				&& pStartNode->x()>pStartNode->y() && pStartNode->x()>0.4 && pStartNode->y() > -Constants::DISTANCE_EPSILON)
				return pMemb;
		}
		return NULL;
	}

	
	//function ---by raoyi  2022/1/23 20:08
	void ElemArray::sortByEndNumLessCmp() 
	{
	//	std::sort(_ElemVec.begin(), _ElemVec.end(), __ElmEndNumLessCompare());
	}

	//function ---by raoyi  2022/1/24 17:27
	void ElemArray::sortByElmCoorY() 
	{
		std::sort(_ElemVec.begin(), _ElemVec.end(), __ElmCoorSort::ElmCoorSortY);
	}


	//function ---by raoyi  2022/1/24 17:27
	void ElemArray::sortByElmCoorOther() 
	{
		std::sort(_ElemVec.begin(), _ElemVec.end(), __ElmCoorSort::ElmCoorSortOther);
	}

	//function ---by raoyi  2022/1/24 17:27
	void ElemArray::sortByElmCoorLeg() 
	{
		std::sort(_ElemVec.begin(), _ElemVec.end(), __ElmCoorSort::ElmCoorSortLeg);
	}

	SmartTower::ElemArray* ElemArray::findFdtFirstMainElms(int fdtNum) const
	{
		ElemArray* resultElms = new ElemArray();
		if (_ElemVec.empty())
				return resultElms;

		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode pEndNode = pElm->EndNode();
			HandleMemberInf pMemb = pElm->GetMenber();
			if (abs(pMemb->MaxTagNum() - fdtNum)<5 && pEndNode->x()>Constants::DISTANCE_EPSILON && pEndNode->y()>Constants::DISTANCE_EPSILON)
			{
				resultElms->push_back(pElm);
			}
		}
		return resultElms;
	}

	ElemArray* ElemArray::firstQuadrantElms() const
	{
		ElemArray* resElmArray = new ElemArray();
		if (_ElemVec.empty())
			return resElmArray;

		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode pStartNode = pElm->StartNode();
			HandleNode pEndNode = pElm->EndNode();
			if (pStartNode->x()>-Constants::SCALAR_EPSILON && pStartNode->y() > -Constants::SCALAR_EPSILON &&  \
				pEndNode->x() > -Constants::SCALAR_EPSILON && pEndNode->y() > -Constants::SCALAR_EPSILON)
				resElmArray->push_back(pElm);
		}
		return resElmArray;
	}

	ElemArray* ElemArray::secondQuadrantElms() const
	{
		ElemArray* resElmArray = new ElemArray();
		if (_ElemVec.empty())
			return resElmArray;

		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode pStartNode = pElm->StartNode();
			HandleNode pEndNode = pElm->EndNode();
			if (pStartNode->x()<Constants::SCALAR_EPSILON && pStartNode->y()>-Constants::SCALAR_EPSILON &&  \
				pEndNode->x()<Constants::SCALAR_EPSILON && pEndNode->y()>-Constants::SCALAR_EPSILON)
				resElmArray->push_back(pElm);
		}
		return resElmArray;
	}

	ElemArray* ElemArray::thirdQuadrantElms() const
	{
		ElemArray* resElmArray = new ElemArray();
		if (_ElemVec.empty())
			return resElmArray;

		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode pStartNode = pElm->StartNode();
			HandleNode pEndNode = pElm->EndNode();
			if (pStartNode->x()>-Constants::DISTANCE_EPSILON0_001 && pStartNode->y()<Constants::DISTANCE_EPSILON0_001 &&  \
				pEndNode->x()>-Constants::DISTANCE_EPSILON0_001 && pEndNode->y() < Constants::DISTANCE_EPSILON0_001)
				resElmArray->push_back(pElm);
		}
		return resElmArray;
	}

	ElemArray* ElemArray::fourthQuadrantElms() const
	{
		ElemArray* resElmArray = new ElemArray();
		if (_ElemVec.empty())
			return resElmArray;

		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleNode pStartNode = pElm->StartNode();
			HandleNode pEndNode = pElm->EndNode();
			if (pStartNode->x() < Constants::DISTANCE_EPSILON0_001 && pStartNode->y() < Constants::DISTANCE_EPSILON0_001 &&  \
				pEndNode->x() < Constants::DISTANCE_EPSILON0_001 && pEndNode->y() < Constants::DISTANCE_EPSILON0_001)
				resElmArray->push_back(pElm);
		}
		return resElmArray;
	}

	SmartTower::ElemArray* ElemArray::findElmByType(int iType) const
	{
		ElemArray* resElms=new ElemArray();
		for (size_t iloop = 0; iloop < _ElemVec.size(); ++iloop)
		{
			HandleElement pElm = _ElemVec.at(iloop);
			HandleMemberInf pMemb = pElm->GetMenber();
			if (pMemb->Type() == iType)
				resElms->push_back(pElm);
		}
		return resElms;
	}

	//function ---by raoyi  2022/1/24 17:26
	bool __ElmCoorSort::ElmCoorSortY(const HandleElement& pElm1, const HandleElement& pElm2)
	{
		HandleNode pStartNode1 = pElm1->StartNode();
		HandleNode pEndNode1 = pElm1->EndNode();
		CVec3D centre1(abs((pStartNode1->x() + pEndNode1->x()) / 2.0), abs((pStartNode1->y() + pEndNode1->y()) / 2.0), (pStartNode1->z() + pEndNode1->z()) / 2.0);
		HandleNode pStartNode2 = pElm2->StartNode();
		HandleNode pEndNode2 = pElm2->EndNode();
		CVec3D centre2((abs(pStartNode2->x() + pEndNode2->x()) / 2.0), abs((pStartNode2->y() + pEndNode2->y()) / 2.0), (pStartNode2->z() + pEndNode2->z()) / 2.0);
		if (centre1.z()<centre2.z())
			return true;
		else if (centre1.z()>centre2.z())
			return false;
		else if (centre1.y() > centre2.y())
			return true;
		else if (centre1.y() < centre2.y())
			return false;
		else
			return centre1.x() < centre2.x();
	}

	//function ---by raoyi  2022/1/24 17:26
	bool __ElmCoorSort::ElmCoorSortOther(const HandleElement& pElm1, const HandleElement& pElm2)
	{
		HandleNode pStartNode1 = pElm1->StartNode();
		HandleNode pEndNode1 = pElm1->EndNode();
		CVec3D centre1(abs((pStartNode1->x() + pEndNode1->x()) / 2.0), abs((pStartNode1->y() + pEndNode1->y()) / 2.0), (pStartNode1->z() + pEndNode1->z()) / 2.0);
		HandleNode pStartNode2 = pElm2->StartNode();
		HandleNode pEndNode2 = pElm2->EndNode();
		CVec3D centre2((abs(pStartNode2->x() + pEndNode2->x()) / 2.0), abs((pStartNode2->y() + pEndNode2->y()) / 2.0), (pStartNode2->z() + pEndNode2->z()) / 2.0);
		if (centre1.z()<centre2.z())
			return true;
		else if (centre1.z()>centre2.z())
			return false;
		else if (centre1.x() > centre2.x())
			return true;
		else if (centre1.x() < centre2.x())
			return false;
		else
			return centre1.y() < centre2.y();
	}

	//function ---by raoyi  2022/1/24 17:26
	bool __ElmCoorSort::ElmCoorSortLeg(const HandleElement& pElm1, const HandleElement& pElm2)
	{
		HandleNode pStartNode1 = pElm1->StartNode();
		HandleNode pEndNode1 = pElm1->EndNode();
		CVec3D centre1(abs((pStartNode1->x() + pEndNode1->x()) / 2.0), abs((pStartNode1->y() + pEndNode1->y()) / 2.0), (pStartNode1->z() + pEndNode1->z()) / 2.0);
		HandleNode pStartNode2 = pElm2->StartNode();
		HandleNode pEndNode2 = pElm2->EndNode();
		CVec3D centre2((abs(pStartNode2->x() + pEndNode2->x()) / 2.0), abs((pStartNode2->y() + pEndNode2->y()) / 2.0), (pStartNode2->z() + pEndNode2->z()) / 2.0);
		if (centre1.z() > centre2.z())
			return true;
		else if (centre1.z()<centre2.z())
			return false;
		else if (centre1.x()>centre2.x())
			return true;
		else if (centre1.x() < centre2.x())
			return false;
		else
			return centre1.y() < centre2.y();
	}

}
