#pragma once
#include <vector>
#include "BaseElement.h"
#include "Box3D.h"
#include "MemberArray.h"
#include "IntVector.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日初步改造------------------
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////

namespace SmartTower
{
	class NodeArray;
	class ElemArray: public SharedObject
	{
	public:
		ElemArray();
		ElemArray(const ElemArray* pArray);
		virtual ~ElemArray();
		const HandleElement& operator[](int ipos) const
		{	// subscript
			return _ElemVec[ipos];
		}

		HandleElement& operator[](int ipos)
		{	// subscript
			return _ElemVec[ipos];
		}
		void replaceElm(int ipos, const HandleElement& rhs)
		{
			_ElemVec[ipos] = rhs;
		}
		void push_back(const HandleElement& rhs)
		{
			_ElemVec.push_back(rhs);
		}
		void Insert(int iPos, const HandleElement& rhs)
		{
			_ElemVec.insert(_ElemVec.begin() + iPos, rhs);
		}
		void remove(const HandleMemberInf& pMemb1);
		int Remove(const HandleElement& pPoint);
		void RemoveAt(int iPos)
		{
			_ElemVec.erase(_ElemVec.begin() + iPos);
		}
		void RemoveRange(int iStart, int iCount)
		{
			_ElemVec.erase(_ElemVec.begin() + iStart, _ElemVec.begin() + iStart + iCount);
		}
		int RefContains(const HandleElement& rhs) const;
		int Contains(int j1,int j2) const;


		HandleNode findMayLegUpNode(int iFdtNum = Constants::MinAuxTagNum) const;


		HandleNode findBracingUpNode(bool bFornt, int ifdtNum = -1);

		HandleNode findFdtLegNode(int fdtTagNum = -1) const;
		//找到距离轴线最近的点
		HandleNode findMidAxisNode(const HandleNode& pDownNode, bool isXZero, double epsilonZero = 0.0);


		//二分法查找
		HandleElement BinarySearch(int j1, int j2) const;
		//for循环查找
		HandleElement find(int j1, int j2) const;

		HandleElement BinarySearch(const HandleElement& pElm, Symetry::Type iYsm) const;

		//查找同一跟杆件，且起端相连的辅助材，用于
		HandleElement findMembStartNumEqual(int j1, int j2) const;
		//基础点主材杆件
		HandleElement getFdtLegElm(int fdtTagNum = -1) const;
		//基础点主材杆件
		HandleMemberInf getFdtLegMemb(int fdtTagNum = -1) const;
		//基础点主材杆件
		HandleElement getFdtFrontBracingElm(int fdtTagNum = -1) const;
		//基础点正面斜材杆件
		HandleMemberInf getfdtFrontTp2Memb(int fdtTagNum = -1) const;
		//基础点主材杆件
		HandleElement getFdtLatrelBracingElm(int fdtTagNum = -1) const;
		//基础点侧面斜材杆件
		HandleMemberInf getfdtLatrelTp2Memb(int fdtTagNum = -1) const;

		int size() const
		{
			return int(_ElemVec.size());
		}
		void clear()
		{
			_ElemVec.clear();
		}
		
		const HandleElement& at(int iPos) const
		{
			return _ElemVec[iPos];
		}
		bool Empty() const
		{
			return _ElemVec.empty();
		}
		const HandleElement& front() const
		{
			return _ElemVec.front();
		}
		const HandleElement& back() const
		{
			return _ElemVec.back();
		}

		void pop_back()
		{
			_ElemVec.pop_back();
		}


		void addArray(const ElemArray* pArray)
		{
			_ElemVec.insert(_ElemVec.end(), pArray->_ElemVec.begin(), pArray->_ElemVec.end());
		}
		//合并，并且号码唯一
		void addUniArray(const ElemArray* pArray);

		int getMinConnectTagNum() const;

		CBox3D GetBox() const;

		ElemArray* findColinear(const CVec3D* pStartNode, const CVec3D* pEndNode) const;

		//查找终点大约等于largerQualNum，小于lessMaxNum的点
		ElemArray* filterRange(int largerQualNum, int lessMaxNum) const;

		//所有BinarySearch，用于排序后的杆件
		//查找再minNum和maxNum之间的杆件，针对排序杆件
		ElemArray* BinaryRange(int minNum, int maxNum) const;

		ElemArray* findDiaphramElms(const HandleNode& pKeyNode) const;

		ElemArray* findInBox3D(const CBox3D& rangeBox) const;

		ElemArray* findZCoorLess(const HandleNode& pNode) const;

		//获取所有与tagNum相连的杆件
		ElemArray* findCnnElms(int tagNum);

		//寻找第一象限跟fdt相连接的赶紧
		ElemArray* findFdtFirstMainElms(int fdtNum)const;

		//求出第一象限的杆件
		ElemArray* firstQuadrantElms() const;
		//求出第二象限的杆件
		ElemArray* secondQuadrantElms() const;
		//求出第三象限的杆件
		ElemArray* thirdQuadrantElms() const;
		//求出第四象限的杆件
		ElemArray* fourthQuadrantElms() const;

		//查找连接杆件
		NodeArray* getConnectNodeArray() const;
		//查找原始杆件
		MemberArray* getMemberArray()const;
		//通过类型查找杆件
		ElemArray* findElmByType(int iType) const;
		//通过类型查找杆件
		ElemArray* findElmByMaterial(double matVal) const;
		void setWinSegIndex(int val);
		//有受力材
		bool hasMainElm() const;
		//最小Z坐标
		double getMinZ() const;
		//通过点号排序
		void sortByEndNumLessCmp() ; 
		void sortByElmCoorY() ;
		void sortByElmCoorOther() ;
		void sortByElmCoorLeg() ;
	private:
		std::vector<HandleElement> _ElemVec;
	};
	typedef StHandle<ElemArray> HdElemArray;




}