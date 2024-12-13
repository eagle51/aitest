#pragma once
#include <vector>
#include "NodeMap.h"
#include "Box3D.h"
#include "IntVector.h"

namespace SmartTower
{
	class TowerPointArray;
	class NodeArray:public SharedObject
	{
	public:
		NodeArray();
		NodeArray(const NodeArray* pArray); 
		virtual ~NodeArray();
		void push_back(const HandleNode& rhs)
		{
			_NodeVec.push_back(rhs);
		}

		void AddNodeMap(const NodeMap* pMap);

		void Insert(int iPos, const HandleNode& rhs)
		{
			_NodeVec.insert(_NodeVec.begin() + iPos, rhs);
		}

		int Remove(const HandleNode& pPoint);

		void RemoveAt(int iPos)
		{
			_NodeVec.erase(_NodeVec.begin() + iPos);
		}

		void RemoveRange(int iStart, int iCount)
		{
			_NodeVec.erase(_NodeVec.begin() + iStart, _NodeVec.begin() + iStart + iCount);
		}

		const HandleNode& operator[](int ipos) const
		{	// subscript
			return _NodeVec[ipos];
		}

		HandleNode& operator[](int ipos)
		{	// subscript
			return _NodeVec[ipos];
		}

		void removeAllOrgiinNode(int keyTagNum);

		int find(const HandleNode& rhs) const;

		int Contains(int tagNum) const;

		//对排序数组操作,仅相等的,已经排序用这个查找快
		HandleNode binarySearch(int tagNum) const;

		HandleNode find(int tagNum) const;

		int size() const
		{
			return int(_NodeVec.size());
		}
		void clear()
		{
			_NodeVec.clear();
		}
		const HandleNode& at(int iPos) const
		{
			return _NodeVec[iPos];
		}
		const HandleNode& front() const
		{
			return _NodeVec.front();
		}
		const HandleNode& back() const
		{
			return _NodeVec.back();
		}
		void pop_back()
		{
			_NodeVec.pop_back();
		}
		bool empty() const
		{
			return _NodeVec.empty();
		}
		//获取最小点号
		int getMinTagNum() const;
		int getMaxTagNum() const;
		//合并数组
		void addArray(const NodeArray* pArray)
		{
			_NodeVec.insert(_NodeVec.end(), pArray->_NodeVec.begin(), pArray->_NodeVec.end());
		}
		//合并，并且号码唯一
		void addUniArray(const NodeArray* pArray);

		//增加唯一一个点
		void addUniNode(const HandleNode& pNode);
		//存在重复节点号，返回true。
		bool checkDuplicate() const;
		//
		HandleNode getMaxZNode() const;

		HandleNode get2DLeftDownNode() const;

		HandleNode get2DDownLeftNode() const;

		HandleNode FindEqual(double xCoor, double yCoor, double zCoor) const;

		HandleNode findMidNode(const HandleNode& pNodeS, const HandleNode&  pNodeE) const;

		CBox3D GetBox3D() const;


		//查找大约等于largerQualNum，小于lessMaxNum的点  [largerQualNum,lessMaxNum)
		NodeArray* filterRange(int largerQualNum, int lessMaxNum);
		//对排序数组操作   [minNum,maxNum)
		NodeArray* binaryRange(int minNum, int maxNum) const;
		//对排序数组操作
		NodeArray* binarySearch(const NodeArray* pArray, Symetry::Type tp) const;
		//
		void findDiaphramFaceNode(const HandleNode& upNode, NodeArray* resVec);

		NodeArray* DeepCopy() const;

		NodeArray* findZCoorLess(const HandleNode& pNode) const;

		NodeArray* findInBox(const CBox3D &Box);

		NodeArray* colinearRangeNdoes(const HandleNode& pNodeS, const HandleNode&  pNodeE);

		NodeArray findNodeArray(const IntVector &nodeNumVec)const;

		NodeArray* findAuxPertainToRange(int minExtendNum, int maxExtendNum);

		NodeArray* TransferToActiveSecPoints();

		NodeArray* getSymArray(Symetry::Type tp) const;

		TowerPointArray* TransferCurrentSecPtType(const TowerPointArray* pSecArray, TowBaseObject::Type activeType);

		//求出第一象限的点
		NodeArray* firstQuadrantNodes()const;
		NodeArray* secondQuadrantNodes()const;
		NodeArray* thirdQuadrantNodes()const;
		NodeArray* fourthQuadrantNodes()const;

		IntVector getTagNumVec() const;

		Symetry::Type getSymmetric() const;

		RenumberingList mergeNodes();
		RenumberingList coorEqList(const NodeArray* rhsNodes);
		void coorEqualRnumb(const HandleNode& pKp0, RenumberingList* pList);
		//反序存储
		void reverseVec();
		//通过Z坐标对点排序
		void sortByZCoor();
		//排序
		void sortByCoorOrNumCmp(bool byCoor=true);
		//Z轴进行反向
		void reverseZAxis(double maxZ);
		//放大
		void enlarge(double fac);
		//
		void addCoor(double addx,double addy=0,double addz=0);
		void transferCoord(const Matrix3d* csMat);
		void projectTo2D(int dp);
		bool isYDirCrossArm(int ingp) const;
		//
		void checkAllSpace3D() const;

	private:
		std::vector<HandleNode> _NodeVec;

		static bool NodeZCoorSort(const HandleNode &pPoint1, const HandleNode &pPoint2)
		{
			return pPoint1->z() < pPoint2->z();
		}
		static bool NumCmp(const HandleNode &pPoint1, const HandleNode &pPoint2)
		{
			return pPoint1->TagNum() <= pPoint2->TagNum();
		};

		static bool CoorCmp(const HandleNode &pPoint1, const HandleNode &pPoint2)
		{
			if (pPoint1->z()<pPoint2->z())
				return true;
			else if (pPoint1->z()>pPoint2->z())
				return false;
			else if (pPoint1->x() > pPoint2->x())
				return true;
			else if (pPoint1->x()<pPoint2->x())
				return false;
			else
				return pPoint1->y()>pPoint2->y();
		}
	};
	typedef StHandle<NodeArray> HdNodeArray;
}