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
	//���˼��㣬������洢��һ�������ĵ���
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
		//�Ե�ǰ���˽��м���
// 		void solve(const HandleDesign& pDesignPara, const HdWinCondManager& pWinConds, const HdNodeLoadManager& pWireLoad, const HandleNGP& pCurNGP,
// 			ostream& fout, const HdProcessBar& pBar);
		//ȫ�����
		void clearAll();
		//��ռ�����
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
// 		//��ڵ����
// 		void solveNodeLoad(const HdWinCondManager& pWinConds, const HdNodeLoadManager& pWireLoad, const HandleDesign& pDesignPara, const HandleNGP& pCurNGP, ostream& fout);

		//void check
		//Ѱ�����ӽڵ�
		void findCnnAndCheckCoincident();
		//�ֽ⼸��ģ�ͣ��Լ���ģ���ȴ��
		void decomposedGeoModel();
		//У��ģ�������ԣ�û�й����˼�������û�����Ӳ�������
		void verifyModelIntegrity();
		//��������
//		void meshElement(const HandleDesign& pDesignPara);
// 		//��ڵ����
// 		void solveWinLoad(const HdWinCondManager& pWinConds, const HandleDesign& pDesignPara, const HandleNGP& pCurNGP, ostream& fout);
// 		//����Ԫ���
// 		void solveFEM(const HandleDesign& pDesignPara,  const HandleNGP& pCurNGP, const HdProcessBar& pBar);
		//
		void checkAll();
		
		//�ҵ������ڵ����ӵĸ˼�
		void findCnnMembSet(std::map<int, AssociatedMemberArray>* pCnnSet);
		//�ǼǸ�ĳ������������и˼�
		void registerCnnMemb(const HandlePoint& pCentrlPoint, std::map<int, AssociatedMemberArray>* pCnnSet, std::set<Member*>* pRegisterSet);
		//����غ��Ȼ��Ź���
		void checkCollinear(std::map<int, AssociatedMemberArray>* pCnnSet);
// 		void addNodeToSolver(SixFEM::SixDomain* pDomian);
// 		void addElmToSolver(SixFEM::SixDomain* pDomian,const HandleDesign& pDesignPara);
		//��ǰ��������
		HdPointArray _PointArray;
		HdMemberArray _MemberArray;

		HdNodeArray _NodeArray;
		HdElemArray _pElmArray;

//		HdNodeLoadManager _PoleLoadArray;//�����˵ĺ��أ����պ���
		
// 		HdInnerForceArray _InnerForceArray;//����������������ֻ�洢
// 		HdDisplacementArray _DisplacementArray;//λ�Ƽ�����
// 		HdDisplacementArray _ReactionForceArray;//����
	};
	typedef SharedHandle<SinglePole> HandleSinglePole; 

}
