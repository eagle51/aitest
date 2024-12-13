#include "NodeArray.h"
#include <algorithm>
#include <map>
#include <vector>
#include "ExceptionMessage.h"
#include "TTAType.h"

namespace SmartTower
{
	NodeArray::NodeArray()
	{
	} 


	NodeArray::NodeArray(const NodeArray* pArray) :_NodeVec(pArray->_NodeVec)
	{

	}

	NodeArray::~NodeArray()
	{
	}

	void NodeArray::AddNodeMap(const NodeMap* pMap)
	{
		const std::map<int, HandleNode>& pInerNodeMap = pMap->_pNodeMap;
		for (std::map<int, HandleNode>::const_iterator pNodeMapIter = pInerNodeMap.begin(); pNodeMapIter != pInerNodeMap.end(); pNodeMapIter++)
			_NodeVec.push_back(pNodeMapIter->second);
	}

	struct _NodeFindCompare
	{
		bool operator()(const HandleNode &pPoint1, int iNum) const
		{
			return pPoint1->TagNum() < iNum;
		}
		bool operator()(int iNum, const HandleNode &pPoint1) const
		{
			return iNum < pPoint1->TagNum();
		}
		bool operator()(const HandleNode &pPoint1, const HandleNode &pPoint2) const
		{
			return  pPoint1->TagNum() < pPoint2->TagNum();
		}
	};



	int NodeArray::Remove(const HandleNode& pPoint)
	{
		int iPos = this->find(pPoint);
		if (iPos < 0)
			return -1;
		_NodeVec.erase(_NodeVec.begin() + iPos);
		return iPos;
	}

	void NodeArray::removeAllOrgiinNode(int keyTagNum)
	{
		if (_NodeVec.empty())
			return;

		size_t index = 0;
		while (index < _NodeVec.size())
		{
			HandleNode pNode = _NodeVec.at(index);
			if (pNode->TagNum() >= keyTagNum &&pNode->TagNum() < (keyTagNum + 5))
			{
				_NodeVec.erase(_NodeVec.begin() + index);
				index = 0;
			}
			else
				++index;
		}
	}

	int NodeArray::find(const HandleNode& rhs) const
	{
		if (rhs.isNull())
			return -1;
		for (vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
		{
			if (cIter->GetPointee() == rhs.GetPointee())
				return int(cIter - _NodeVec.begin());
		}
		return -1;
	}

	HandleNode NodeArray::find(int tagNum) const
	{
		int iPos = this->Contains(tagNum);
		if (iPos < 0)
			return NULL;
		return _NodeVec.at(iPos);
	}

	int NodeArray::Contains(int tagNum) const
	{
		if (tagNum<0)
			return -1;
		for (std::vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
		{
			if ((*cIter)->TagNum() == tagNum)
				return int(cIter - _NodeVec.begin());
		}
		return -1;
	}


	SmartTower::NodeArray* NodeArray::filterRange(int largerQualNum, int lessMaxNum)
	{
		NodeArray* newArray = new NodeArray();
		for (size_t iloop = 0; iloop < _NodeVec.size();++iloop)
		{
			HandleNode pTmpNode = _NodeVec.at(iloop);
			if (pTmpNode->TagNum() >= largerQualNum && pTmpNode->TagNum() < lessMaxNum)
				newArray->_NodeVec.push_back(pTmpNode);
		}
		return newArray;
	}

	HandleNode NodeArray::binarySearch(int tagNum) const
	{
		std::vector<HandleNode>::const_iterator resIter = std::lower_bound(_NodeVec.begin(), _NodeVec.end(), tagNum, _NodeFindCompare());
		if (resIter == _NodeVec.end() || (*resIter)->TagNum() != tagNum)
			return NULL;
		return *resIter;
	}

	void NodeArray::findDiaphramFaceNode(const HandleNode& upNode, NodeArray* resVec)
	{
		if (_NodeVec.empty() || upNode.isNull())
			return;

		HandleNode pXYNode = upNode->getISNode(Symetry::Osym);
		CBox3D box;
		box.extend(upNode.GetPointee());
		box.extend(pXYNode.GetPointee());
		box.extend(Constants::DISTANCE_EPSILON);
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			if (box.contains(pNode.GetPointee()))
				resVec->push_back(pNode);
		}
	}

	// [minNum,maxNum)
	SmartTower::NodeArray* NodeArray::binaryRange(int minNum, int maxNum) const
	{
		NodeArray* newArray = new NodeArray();
		std::vector<HandleNode>::const_iterator minIter = std::lower_bound(_NodeVec.begin(), _NodeVec.end(), minNum, _NodeFindCompare());
		std::vector<HandleNode>::const_iterator maxIter = std::lower_bound(_NodeVec.begin(), _NodeVec.end(), maxNum, _NodeFindCompare());
		for (std::vector<HandleNode>::const_iterator nIter = minIter; nIter < maxIter; ++nIter)
			newArray->_NodeVec.push_back(*nIter);
		return newArray;
	}

	SmartTower::NodeArray* NodeArray::binarySearch(const NodeArray* pArray, Symetry::Type tp) const
	{
		NodeArray* newArray = new NodeArray();
		for (int iloop = 0; iloop < pArray->size();++iloop)
		{
			HandleNode pTmpNode = pArray->at(iloop);
			HandleNode pSymNode = this->binarySearch(pTmpNode->ISymTagNum(tp));
			if (!pSymNode.isNull())
				newArray->_NodeVec.push_back(pSymNode);
		};
		return newArray;
	}

	int NodeArray::getMinTagNum() const
	{
		int minNumX= 10000000;//基础最小点
		for (std::vector<HandleNode>::const_iterator nIter = _NodeVec.begin(); nIter < _NodeVec.end(); ++nIter)
		{
			if (minNumX>(*nIter)->TagNum())
				minNumX = (*nIter)->TagNum();
		}
		return minNumX;
	}

	int NodeArray::getMaxTagNum() const
	{
		int maxNumX = 0;//基础最小点
		for (std::vector<HandleNode>::const_iterator nIter = _NodeVec.begin(); nIter < _NodeVec.end(); ++nIter)
		{
			if (maxNumX<(*nIter)->TagNum())
				maxNumX = (*nIter)->TagNum();
		}
		return maxNumX;
	}

	void NodeArray::addUniArray(const NodeArray* pArray)
	{
		for (int iloop = 0; iloop < pArray->size(); ++iloop)
		{
			HandleNode pInNode = pArray->at(iloop);
			int iPos = this->Contains(pInNode->TagNum());
			if (iPos < 0)
				_NodeVec.push_back(pInNode);
		}
	}

	void NodeArray::addUniNode(const HandleNode& pNode)
	{
		int iPos = this->Contains(pNode->TagNum());
		if (iPos < 0)
			_NodeVec.push_back(pNode);
	}

	bool NodeArray::checkDuplicate() const
	{
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			for (size_t jloop = iloop + 1; jloop < _NodeVec.size(); ++jloop)
			{
				HandleNode pNode2 = _NodeVec.at(jloop);
				if (pNode->TagNum() == pNode2->TagNum())
				{
					return true;
				}
			}
		}
		return false;
	}

	SmartTower::HandleNode NodeArray::getMaxZNode() const
	{
		HandleNode pMaxZNode;
		for (std::vector<HandleNode>::const_iterator nIter = _NodeVec.begin(); nIter < _NodeVec.end(); ++nIter)
		{
			if (pMaxZNode.isNull())
				pMaxZNode = nIter->GetPointee();
			else if (pMaxZNode->z() < (*nIter)->z())
				pMaxZNode = nIter->GetPointee();
		}
		return pMaxZNode;
	}

	SmartTower::HandleNode NodeArray::get2DLeftDownNode() const
	{
		HandleNode pMinZNode;
		for (std::vector<HandleNode>::const_iterator nIter = _NodeVec.begin(); nIter < _NodeVec.end(); ++nIter)
		{
			if ((*nIter)->TagNum()>=Constants::MinAuxTagNum)
				continue;
			if (pMinZNode.isNull())
				pMinZNode = nIter->GetPointee();
			else if (pMinZNode->x() > (*nIter)->x())
				pMinZNode = nIter->GetPointee();
			else if (pMinZNode->x()== (*nIter)->x() && pMinZNode->y()>(*nIter)->y() )
				pMinZNode = nIter->GetPointee();
		}
		return pMinZNode;
	}

	SmartTower::HandleNode NodeArray::get2DDownLeftNode() const
	{
		HandleNode pMinZNode;
		for (std::vector<HandleNode>::const_iterator nIter = _NodeVec.begin(); nIter < _NodeVec.end(); ++nIter)
		{
			if ((*nIter)->TagNum() >= Constants::MinAuxTagNum)
				continue;
			if (pMinZNode.isNull())
				pMinZNode = nIter->GetPointee();
			else if (pMinZNode->y() >(*nIter)->y())
				pMinZNode = nIter->GetPointee();
			else if (pMinZNode->y() == (*nIter)->y() && pMinZNode->x() > (*nIter)->x())
				pMinZNode = nIter->GetPointee();
		}
		return pMinZNode;
	}

	SmartTower::NodeArray* NodeArray::DeepCopy() const
	{
		NodeArray* newArray = new NodeArray();
		newArray->_NodeVec.reserve(this->size());
		for (std::vector<HandleNode>::const_iterator Iter = _NodeVec.begin(); Iter < _NodeVec.end(); ++Iter)
		{
			HandleNode pNode(Iter->GetPointee());
			pNode.MakeCopy();
			newArray->push_back(pNode);
		}
		return newArray;
	}


	SmartTower::NodeArray* NodeArray::findZCoorLess(const HandleNode& pNode) const
	{
		NodeArray* resNodeArray = new NodeArray();
		if (_NodeVec.empty() || pNode.isNull())
			return resNodeArray;

		double xCoor = abs(pNode->x());
		double yCoor = abs(pNode->y());
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode tpNode = _NodeVec.at(iloop);;
			if (tpNode->z() < pNode->z() || ((tpNode->z() - pNode->z()) < Constants::DISTANCE_EPSILON && \
				abs(tpNode->x()) >= xCoor && abs(tpNode->y()) >= yCoor))
				resNodeArray->push_back(tpNode);
		}
		return resNodeArray;
	}

	CBox3D NodeArray::GetBox3D() const
	{
		CBox3D result;
		for (std::vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
			result.extend(cIter->GetPointee());
		return result;
	}

	SmartTower::NodeArray* NodeArray::findInBox(const CBox3D &box)
	{
		NodeArray* pNodeRes = new NodeArray();

		if (_NodeVec.empty())
			return pNodeRes;

		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			if (box.contains(pNode.GetPointee()))
			{
				pNodeRes->push_back(pNode);
			}
		}
		return pNodeRes;
	}

	SmartTower::NodeArray* NodeArray::colinearRangeNdoes(const HandleNode& pNodeS, const HandleNode& pNodeE)
	{
		CBox3D box;
		box.extend(pNodeS.GetPointee());
		box.extend(pNodeE.GetPointee());
		box.extend(Constants::DISTANCE_EPSILON0_001);
		NodeArray* nodesBetweenUpDown=new NodeArray();
		CVec3D upDownVec = pNodeE->Subtraction(pNodeS.GetPointee());
		for (size_t jloop = 0; jloop < _NodeVec.size(); ++jloop)
		{
			HandleNode tmpNode1 = _NodeVec.at(jloop);
			if (box.contains(tmpNode1.GetPointee()))
			{
				CVec3D tempVec = upDownVec.cross(tmpNode1->Subtraction(pNodeS.GetPointee()));
				if (tempVec.norm() < Constants::DISTANCE_EPSILON)
					nodesBetweenUpDown->push_back(tmpNode1);
			}
		}
		return nodesBetweenUpDown;
	}


	SmartTower::HandleNode NodeArray::FindEqual(double xCoor, double yCoor, double zCoor) const
	{
		for (std::vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
			if ((*cIter)->CoordEqual(xCoor, yCoor, zCoor))
				return *cIter;
		return NULL;
	}


	SmartTower::HandleNode NodeArray::findMidNode(const HandleNode& pNodeS, const HandleNode& pNodeE) const
	{
		double xcoor = (pNodeS->x() + pNodeE->x()) / 2.0;
		double ycoor = (pNodeS->y() + pNodeE->y()) / 2.0;
		double zcoor = (pNodeS->z() + pNodeE->z()) / 2.0;
		CVec3D tmpVec(xcoor, ycoor, zcoor);

		for (std::vector<HandleNode>::const_iterator cIter = _NodeVec.begin(); cIter < _NodeVec.end(); ++cIter)
		{
			if (tmpVec.distanceTo(cIter->GetPointee()) < Constants::DISTANCE_EPSILON)
				return cIter->GetPointee();
		}
		return NULL;
	}

	SmartTower::IntVector NodeArray::getTagNumVec() const
	{
		IntVector intVec;
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			intVec.push_back(pNode->TagNum());
		}
		return intVec;
	}

	void NodeArray::sortByZCoor()
	{
		std::sort(_NodeVec.begin(), _NodeVec.end(), NodeArray::NodeZCoorSort);
	}

	//function ---by raoyi  2022/1/18 21:14
	NodeArray NodeArray::findNodeArray(const IntVector &nodeNumVec) const
	{
		NodeArray resNodeArray;
        if (nodeNumVec.empty() || _NodeVec.empty())
			return resNodeArray;
		
		for (int iloop = 0; iloop<nodeNumVec.size(); ++iloop)
		{
			if (nodeNumVec[iloop]>0)
			{
                HandleNode pNode = this->find(nodeNumVec[iloop]);
                if (!pNode.isNull())
                    resNodeArray.push_back(pNode);
			}
		}
		return resNodeArray;
	}

	SmartTower::NodeArray* NodeArray::getSymArray(Symetry::Type tp) const
	{
		NodeArray* resNodes = new NodeArray();
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			resNodes->push_back(pNode->getISNode(tp));
		}
		return resNodes;
	}

	NodeArray* NodeArray::TransferToActiveSecPoints()
	{
		if (_NodeVec.empty())
			return this;
		std::map<int, HandleTowerPoint> mapSecPoints;
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pOldNode = _NodeVec.at(iloop);
			HandleTowerPoint pPoint = pOldNode->GetOriginNum();
			std::map<int, HandleTowerPoint>::iterator bFindFlag = mapSecPoints.find(pPoint->TagNum());
			if (bFindFlag != mapSecPoints.end())
				continue;
			if (pPoint->GetObjType() == TowBaseObject::XBSECPOINT && CTTAType::Instance()->ActiveSecType() == TowBaseObject::XBSECPOINT)
				return this;//如果相同则所有内容都相同
			else if (pPoint->GetObjType() == TowBaseObject::SECPOINT && CTTAType::Instance()->ActiveSecType() == TowBaseObject::SECPOINT)
				return this;//如果相同则所有内容都相同
			if (pPoint->GetObjType() == TowBaseObject::XBSECPOINT || pPoint->GetObjType() == TowBaseObject::SECPOINT)
				mapSecPoints.insert(pair<int, HandleTowerPoint>(pPoint->TagNum(), pPoint));
		}
		if (mapSecPoints.empty())
			return this;

		//需要全部变换为
		std::map<int, HandleTowerPoint> mapNewSecPoints;
		for (std::map<int, HandleTowerPoint>::iterator sIter = mapSecPoints.begin(); sIter != mapSecPoints.end(); ++sIter)
		{
			if (CTTAType::Instance()->ActiveSecType() == TowBaseObject::SECPOINT)
			{
				if (sIter->second->GetObjType() == TowBaseObject::XBSECPOINT)
				{
					CXBSecondaryPoint* pSecPoint = static_cast<CXBSecondaryPoint*>(sIter->second.GetPointee());
					HandleNode pResJ1 = this->binarySearch(pSecPoint->J1() % 10000);
					HandleNode pResJ2 = this->binarySearch(pSecPoint->J2() % 10000);
					HandleNode pMeJ2 = this->binarySearch(pSecPoint->TagNum());
					if (pResJ1.isNull() || pResJ2.isNull() || pMeJ2.isNull())
						throw exception("数据错误无法转化二类点");
					else
					{
						HandleTowerPoint pNewSecPoint = pSecPoint->ToDBSecondaryPoint(pResJ1, pResJ2, pMeJ2);
						mapNewSecPoints.insert(pair<int, HandleTowerPoint>(pNewSecPoint->TagNum(), pNewSecPoint));
					}
				}
				else
					mapNewSecPoints.insert(pair<int, HandleTowerPoint>(sIter->second->TagNum(), sIter->second));
			}
			else if (CTTAType::Instance()->ActiveSecType() == TowBaseObject::XBSECPOINT)
			{
				if (sIter->second->GetObjType() == TowBaseObject::SECPOINT)
				{
					CSecondaryPoint* pSecPoint = static_cast<CSecondaryPoint*>(sIter->second.GetPointee());
					HandleTowerPoint pNewSecPoint = pSecPoint->ToXBSecondaryPoint();
					mapNewSecPoints.insert(pair<int, HandleTowerPoint>(pNewSecPoint->TagNum(), pNewSecPoint));
				}
				else
					mapNewSecPoints.insert(pair<int, HandleTowerPoint>(sIter->second->TagNum(), sIter->second));
			}
		}

		NodeArray* pNewNodeArray = new NodeArray();
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pOldNode = _NodeVec.at(iloop);
			HandleTowerPoint pPoint = pOldNode->GetOriginNum();
			if (pPoint->GetObjType() == TowBaseObject::XBSECPOINT || pPoint->GetObjType() == TowBaseObject::SECPOINT)
			{
				std::map<int, HandleTowerPoint>::iterator findIter = mapNewSecPoints.find(pPoint->TagNum());
				if (findIter != mapNewSecPoints.end())//防止出错
					pNewNodeArray->push_back(new CNode(pOldNode->TagNum(), pOldNode->x(), pOldNode->y(), pOldNode->z(), findIter->second));
			}
			else
				pNewNodeArray->push_back(pOldNode);//其他的都要
		}
		return pNewNodeArray;
	}

	TowerPointArray* NodeArray::TransferCurrentSecPtType(const TowerPointArray* pSecArray, TowBaseObject::Type activeType)
	{
		if (pSecArray==NULL || pSecArray->empty())
			return NULL;
		//需要全部变换为
		std::map<int, HandleTowerPoint> mapNewSecPoints;
		for (int iloop = 0; iloop < pSecArray->size(); ++iloop)
		{
			HandleTowerPoint pCurPoint = pSecArray->at(iloop);
			std::map<int, HandleTowerPoint>::iterator bFindFlag = mapNewSecPoints.find(pCurPoint->TagNum());
			if (bFindFlag != mapNewSecPoints.end())
				continue;

			if (activeType == TowBaseObject::SECPOINT)
			{
				if (pCurPoint->GetObjType() == TowBaseObject::XBSECPOINT)
				{
					CXBSecondaryPoint* pSecPoint = static_cast<CXBSecondaryPoint*>(pCurPoint.GetPointee());
					HandleNode pResJ1 = this->binarySearch(pSecPoint->J1() % 10000);
					HandleNode pResJ2 = this->binarySearch(pSecPoint->J2() % 10000);
					HandleNode pResMeJ2 = this->binarySearch(pSecPoint->TagNum());
					if (pResJ1.isNull() || pResJ2.isNull() || pResMeJ2.isNull())
                        throw exception("数据错误无法转化二类点");
					else
					{
						HandleTowerPoint newSec = pSecPoint->ToDBSecondaryPoint(pResJ1, pResJ2, pResMeJ2);
						if (!newSec.isNull())
							mapNewSecPoints.insert(pair<int, HandleTowerPoint>(newSec->TagNum(), newSec));
					}
				}
				else
					mapNewSecPoints.insert(pair<int, HandleTowerPoint>(pCurPoint->TagNum(), pCurPoint.GetPointee()));
			}
			else if (activeType == TowBaseObject::XBSECPOINT)
			{
				if (pCurPoint->GetObjType() == TowBaseObject::SECPOINT)
				{
					CSecondaryPoint* pSecPoint = static_cast<CSecondaryPoint*>(pCurPoint.GetPointee());
					HandleTowerPoint newSec = pSecPoint->ToXBSecondaryPoint();
					mapNewSecPoints.insert(pair<int, HandleTowerPoint>(newSec->TagNum(), newSec));

				}
				else
				{
					mapNewSecPoints.insert(pair<int, HandleTowerPoint>(pCurPoint->TagNum(), pCurPoint.GetPointee()));
				}
			}
		}
		TowerPointArray* pNewTowerPointArray = new TowerPointArray();
		for (std::map<int, HandleTowerPoint>::iterator Iter = mapNewSecPoints.begin(); Iter != mapNewSecPoints.end(); ++Iter)
			pNewTowerPointArray->push_back(Iter->second);
		return pNewTowerPointArray;
	}

	NodeArray* NodeArray::firstQuadrantNodes() const
	{
		NodeArray* resNodeArray = new NodeArray();
		if (_NodeVec.empty())
			return resNodeArray;

		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			if (pNode->x()>-Constants::SCALAR_EPSILON && pNode->y() > -Constants::SCALAR_EPSILON)
				resNodeArray->push_back(pNode);
		}
		return resNodeArray;
	}

	NodeArray* NodeArray::secondQuadrantNodes() const
	{
		NodeArray* resNodeArray = new NodeArray();
		if (_NodeVec.empty())
			return resNodeArray;

		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			if (pNode->x()<Constants::SCALAR_EPSILON && pNode->y()>-Constants::SCALAR_EPSILON)
				resNodeArray->push_back(pNode);
		}
		return resNodeArray;
	}

	NodeArray* NodeArray::thirdQuadrantNodes() const
	{
		NodeArray* resNodeArray = new NodeArray();
		if (_NodeVec.empty())
			return resNodeArray;

		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			if (pNode->x()>-Constants::SCALAR_EPSILON && pNode->y() < Constants::SCALAR_EPSILON)
				resNodeArray->push_back(pNode);
		}
		return resNodeArray;
	}

	NodeArray* NodeArray::fourthQuadrantNodes() const
	{
		NodeArray* resNodeArray = new NodeArray();
		if (_NodeVec.empty())
			return resNodeArray;

		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			if (pNode->x() < Constants::DISTANCE_EPSILON0_001 && pNode->y() < Constants::DISTANCE_EPSILON0_001)
				resNodeArray->push_back(pNode);
		}
		return resNodeArray;
	}

	SmartTower::Symetry::Type NodeArray::getSymmetric() const
	{
		size_t xSymCount = 0;
		//size_t YSymCount=0;
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			HandleTowerPoint pPoint = pNode->GetOriginNum();
			if (pPoint->Isym() == Symetry::XYsym)
				++xSymCount;
			else
			{
				int xSymNum = pNode->ISymTagNum(Symetry::Xsym);
				if (xSymNum == pNode->TagNum())
					++xSymCount;
				else if (pPoint->Isym() == Symetry::Xsym && pNode->x() > Constants::DISTANCE_EPSILON)
					++xSymCount;
			}
		}
		if (xSymCount == _NodeVec.size())
			return Symetry::Xsym;
		else
			return Symetry::None;
	}

	SmartTower::RenumberingList NodeArray::mergeNodes()
	{
		RenumberingList resList;
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pCurNode =_NodeVec.at(iloop);
			size_t jloop = iloop + 1;
			while (jloop<_NodeVec.size())
			{
				HandleNode pTmpNode = _NodeVec.at(jloop);
				if (pTmpNode->CoordEqual(pCurNode.GetPointee()))
				{
					resList.AddOrChangePair(pTmpNode->TagNum(), pCurNode->TagNum());
					_NodeVec.erase(_NodeVec.begin() + jloop);//删除了
				}
				else
					++jloop;
			}
		}
		return resList;
	}

	SmartTower::RenumberingList NodeArray::coorEqList(const NodeArray* rhsNodes)
	{
		RenumberingList renumList;
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode ptmplNode = _NodeVec.at(iloop);
			for (int jloop = 0; jloop < rhsNodes->size(); ++jloop)
			{
				HandleNode ptmplOldNode = rhsNodes->at(jloop);
				if (ptmplNode->CoordEqual(ptmplOldNode.GetPointee()))
				{//
					bool bFlag = false;
					if (ptmplNode->TagNum() < Constants::MinAuxTagNum && ptmplOldNode->TagNum() < Constants::MinAuxTagNum)
						bFlag = true;
					else if (ptmplNode->TagNum() >= Constants::MinAuxTagNum && ptmplOldNode->TagNum() >= Constants::MinAuxTagNum)
						bFlag = true;
					if (bFlag)
					{
						renumList.AddOrChangePair(ptmplNode->TagNum(), ptmplOldNode->TagNum());
						break;
					}
				}
			}
		}
		return renumList;
	}

	void NodeArray::coorEqualRnumb(const HandleNode& pKp0, RenumberingList* pList)
	{
		HandleNode pTpKy0 = this->FindEqual(pKp0->x(), pKp0->y(), pKp0->z());
		pList->AddOrChangePair(pTpKy0->TagNum(), pKp0->TagNum());
		pList->AddOrChangePair(pTpKy0->TagNum() + 1, pKp0->TagNum() + 1);
		pList->AddOrChangePair(pTpKy0->TagNum() + 2, pKp0->TagNum() + 2);
		pList->AddOrChangePair(pTpKy0->TagNum() + 3, pKp0->TagNum() + 3);
	}

	void NodeArray::reverseVec()
	{
		size_t maxIndex = _NodeVec.size() / 2;
		for (size_t iloop = 0; iloop < maxIndex; ++iloop)
			std::swap(_NodeVec[iloop], _NodeVec[_NodeVec.size() - iloop - 1]);
	}

	//function ---by raoyi  2022/1/24 16:51
	void NodeArray::sortByCoorOrNumCmp(bool byCoor)
	{
		if (byCoor)
			std::sort(_NodeVec.begin(), _NodeVec.end(), NodeArray::CoorCmp);
		else
			std::sort(_NodeVec.begin(), _NodeVec.end(), NodeArray::NumCmp);
	}

	void NodeArray::reverseZAxis(double maxZ)
	{
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			double newZ = maxZ - pNode->z();
			pNode->setZ(newZ);
		}
	}

	void NodeArray::enlarge(double fac)
	{
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			pNode->setX(pNode->x()*fac);
			pNode->setY(pNode->y()*fac);
			pNode->setZ(pNode->z()*fac);
		}
	}

	void NodeArray::addCoor(double centX, double centY, double centZ)
	{
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			pNode->setX(pNode->x() + centX);
			pNode->setY(pNode->y() + centY);
			pNode->setZ(pNode->z() + centZ);
		}
	}

	void NodeArray::transferCoord(const Matrix3d* csMat)
	{
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			pNode->TransferCoor(csMat);
		}
	}

	void NodeArray::projectTo2D(int dp)
	{
		double x, y, z;
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			
			if (dp == 1)
			{
				x = pNode->x();
				y = - pNode->z();
				z = pNode->y();
			}
			else if (dp == 2)
			{
				x = - pNode->y();
				y = - pNode->z();
				z = pNode->x();

			}
			else if (dp == 3)
			{
				x = pNode->x();
				y = -pNode->y();
				z = 0.0;
			}
			pNode->setX(x);
			pNode->setY(y);
			pNode->setZ(0.0);
		}
	}

	bool NodeArray::isYDirCrossArm(int ingp) const
	{
		int icount = 0;
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			if (abs(abs(pNode->x()) - abs(pNode->y()))< Constants::DISTANCE_EPSILON && pNode->GetOriginNum()->Isym() == Symetry::XYsym)
			{
				if (ingp == 0 && pNode->GetOriginNum()->GetObjType() == TowBaseObject::PRIPOINT)
					++icount;
				else if (ingp>0)
					++icount;
			}
		}
		return icount>1;
	}

	void NodeArray::checkAllSpace3D() const
	{
		for (size_t iloop = 0; iloop < _NodeVec.size(); ++iloop)
		{
			HandleNode pNode = _NodeVec.at(iloop);
			if (pNode->SingularType() != CNode::Space3D)
				throw exception("若要对所有节点都施加荷载，必须将具有平面节点的杆件设置为梁单元，并将选项'计算时采用欧拉梁'打开！");
		}
	}

}