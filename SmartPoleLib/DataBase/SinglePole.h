#pragma once
#include <set>
//#include "NGP_Info.h"
#include "PointArray.h"
#include "MemberArray.h"
#include "NodeArray.h"
#include "ElemArray.h"
//#include "NodeLoadManager.h"
// #include "ElmInnerForceArray.h"
// #include "NodeDisplacementArray.h"
// #include "AssociatedMemberArray.h"
// #include "DesignParameter.h"
// #include "WinCondManager.h"
// #include "SixDomain.h"
// #include "IProcessBar.h"
// #include "CalcConnect.h"


namespace SmartPoleCore
{
	//单杆计算，这里面存储了一个完整的单杆
	class Component;
	class SinglePole:public SharedObject
	{
	public:
		SinglePole();
		~SinglePole();
		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::SinglePoleResult;
		};
		const HdPointArray& getPointArray()
		{
			return _PointArray;
		}
		void setPointArray(const HdPointArray& val)
		{
			_PointArray = val;
		}
		const HdMemberArray& getMemberArray()
		{
			return _MemberArray;
		}
		void setMemberArray(const HdMemberArray& val)
		{
			_MemberArray = val;
		}
		const HdNodeArray& getNodeArray() const
		{
			return _NodeArray;
		}
		void setNodeArray(const HdNodeArray& val)
		{
			_NodeArray = val;
		}
		const HdElemArray& getElmArray() const
		{
			return _pElmArray;
		}
		void setElmArray(const HdElemArray& val)
		{
			_pElmArray = val;
		}
		void addMembArray(const HdPointArray& pPointArray,const HdMemberArray& pMembArray);
		void addComponent(const Component* pComp);
		//对单前单杆进行计算
// 		void solve(const HandleDesign& pDesignPara, const HdWinCondManager& pWinConds, const HdNodeLoadManager& pWireLoad, const HandleNGP& pCurNGP,
// 			ostream& fout, const HdProcessBar& pBar);
		//全部清空
		void clearAll();
		//清空计算结果
		void clearResult();
// 		const HdNodeLoadManager& getPoleLoadArray() const
// 		{
// 			return _PoleLoadArray;
// 		}
// 		const HdDisplacementArray& getDisplacementArray() 
// 		{ 
// 			return _DisplacementArray;
// 		}
// 		const HdInnerForceArray& getInnerForceArray()  
// 		{ 
// 			return _InnerForceArray;
// 		}
// 		const HdDisplacementArray& getReactionForceArray() const 
// 		{ 
// 			return _ReactionForceArray;
// 		}

// 		void outputToStream(QDataStream& dataStream) const;
// 		void inputFromStream(QDataStream& dataStream);

		void checkMember(int legNum);

		void ConnectCalc(HandleMember pMemb, bool isStart, int LegNum, bool isOptimization);

	private:
		void test();
		//
// 		void mashSinglePole(const HandleDesign& pDesignPara);
// 		//求节点荷载
// 		void solveNodeLoad(const HdWinCondManager& pWinConds, const HdNodeLoadManager& pWireLoad, const HandleDesign& pDesignPara, const HandleNGP& pCurNGP, ostream& fout);

		//void check
		//寻找连接节点
		void findCnnAndCheckCoincident();
		//分解几何模型，对几何模型先打断
		void decomposedGeoModel();
		//校验模型完整性，没有孤立杆件，或者没有连接不完整性
		void verifyModelIntegrity();
		//划分网格
//		void meshElement(const HandleDesign& pDesignPara);
// 		//求节点荷载
// 		void solveWinLoad(const HdWinCondManager& pWinConds, const HandleDesign& pDesignPara, const HandleNGP& pCurNGP, ostream& fout);
// 		//有限元求解
// 		void solveFEM(const HandleDesign& pDesignPara,  const HandleNGP& pCurNGP, const HdProcessBar& pBar);
		//
		void checkAll();
		
		//找到各个节点连接的杆件
		void findCnnMembSet(std::map<int, AssociatedMemberArray>* pCnnSet);
		//登记跟某个点关联的所有杆件
		void registerCnnMemb(const HandlePoint& pCentrlPoint, std::map<int, AssociatedMemberArray>* pCnnSet, std::set<Member*>* pRegisterSet);
		//检查重合先活着共线
		void checkCollinear(std::map<int, AssociatedMemberArray>* pCnnSet);
// 		void addNodeToSolver(SixFEM::SixDomain* pDomian);
// 		void addElmToSolver(SixFEM::SixDomain* pDomian,const HandleDesign& pDesignPara);
		//当前输入条件
		HdPointArray _PointArray;
		HdMemberArray _MemberArray;

		HdNodeArray _NodeArray;
		HdElemArray _pElmArray;

//		HdNodeLoadManager _PoleLoadArray;//整个杆的荷载，最终荷载
		
// 		HdInnerForceArray _InnerForceArray;//内力计算结果，允许只存储
// 		HdDisplacementArray _DisplacementArray;//位移计算结果
// 		HdDisplacementArray _ReactionForceArray;//反力
	};
	typedef SharedHandle<SinglePole> HandleSinglePole; 

}
