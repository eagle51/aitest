#pragma once
#include <list>
#include <set>
#include <map>
#include "TowerModel.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	class DiaphragmFaceInfo:public SharedObject
	{
	private:
		int _TP;		
		int _NI;		
		int _M1;		
		int _IF1;		
		int _IF2;		
		int _M2;		
		int _IS1;
		int _IS2;
		int _N0;
		int _N1;
		int _N2;
		int _N3;
		int _N4;
	protected:
		HdTowerPointArray _pSecPointArray;
		HdMemberArray _pMemberArray;		
		virtual void MKindTransfer();
		virtual void createPointsAndMembers()=0;
	public:
		static int intstep;
		DiaphragmFaceInfo(void);
		virtual ~DiaphragmFaceInfo(void)=0;
		//表示成段生成型号，TP=30、31或32
		int TP() const 
		{ 
			return _TP; 
		}
		//表示成段生成型号，TP=30、31或32
		void TP(int val)
		{ 
			_TP = val; 
		}
		//由程序自动生成起始点节点号
		int NI() const 
		{ 
			return _NI; 
		}
		//由程序自动生成起始点节点号
		void NI(int val) 
		{ 
			_NI = val; 
		}
		//本段正面横材统材号
		int M1() const 
		{ 
			return _M1; 
		}
		//本段正面横材统材号
		void M1(int val)
		{ 
			_M1 = val;
		}
		//本段正面横材沿最小轴失稳时的计算长度系数。
		int IF1() const 
		{ 
			return _IF1; 
		}
		//本段正面横材沿最小轴失稳时的计算长度系数。
		void IF1(int val)
		{ 
			_IF1 = val; 
		}
		//本段正面横材沿平行轴失稳时的计算长度系数。
		int IF2() const 
		{ 
			return _IF2; 
		}
		//本段正面横材沿平行轴失稳时的计算长度系数。
		void IF2(int val) 
		{ 
			_IF2 = val; 
		}
		//本段侧面横材统材号
		int M2() const 
		{ 
			return _M2;
		}
		//本段侧面横材统材号
		void M2(int val) 
		{ 
			_M2 = val; 
		}
		//本段侧面横材沿最小轴失稳时的计算长度系数。
		int IS1() const 
		{ 
			return _IS1; 
		}
		//本段侧面横材沿最小轴失稳时的计算长度系数。
		void IS1(int val) 
		{ 
			_IS1 = val; 
		}
		//本段侧面横材沿平行轴失稳时的计算长度系数。
		int IS2() const 
		{ 
			return _IS2; 
		}
		//本段侧面横材沿平行轴失稳时的计算长度系数。
		void IS2(int val)
		{ 
			_IS2 = val; 
		}
		//为横隔面角点节点号，此点需在第一或第二类节点定义数组内输入。
		int N0() const 
		{ 
			return _N0; 
		}
		//为横隔面角点节点号，此点需在第一或第二类节点定义数组内输入。
		void N0(int val)
		{ 
			_N0 = val; 
		}
		//分别代表横隔面正、侧面横材上的节点。
		int N1() const 
		{ 
			return _N1; 
		}
		//分别代表横隔面正、侧面横材上的节点。
		void N1(int val) 
		{ 
			_N1 = val; 
		}
		//分别代表横隔面正、侧面横材上的节点。
		int N2() const 
		{ 
			return _N2; 
		}
		//分别代表横隔面正、侧面横材上的节点。
		void N2(int val) 
		{ 
			_N2 = val; 
		}
		//分别代表横隔面正、侧面横材上的节点。
		int N3() const 
		{ 
			return _N3; 
		}
		//分别代表横隔面正、侧面横材上的节点。
		void N3(int val) 
		{ 
			_N3 = val; 
		}
		//分别代表横隔面正、侧面横材上的节点。
		int N4() const 
		{ 
			return _N4; 
		}
		//分别代表横隔面正、侧面横材上的节点。
		void N4(int val) 
		{ 
			_N4 = val; 
		}
		virtual int getTowerPointSize()=0;
		virtual int getMemberSize()=0;
		virtual void InsertToModel(CTowerModel* TheModel);
		virtual bool CompareToMe(HandleNode cornerNode,std::map<int,HandleNode>* tmpNodeMap);
		virtual void CreateNodesAndElms(HandleNode cornerNode);
		//virtual void CreateKeyNode();
	};
	typedef StHandle<DiaphragmFaceInfo> HandleDiaphragmFace;



	class CDiaphragmFaceInsertTP30:public DiaphragmFaceInfo
	{
	protected:
		virtual void createPointsAndMembers();
	public:
		virtual int getTowerPointSize()
		{
			return 8;
		}
		virtual int getMemberSize()
		{
			return 13;
		}
		CDiaphragmFaceInsertTP30();
		~CDiaphragmFaceInsertTP30(void);	
	};


	class CDiaphragmFaceInsertTP31:public DiaphragmFaceInfo
	{
	protected:
		virtual void createPointsAndMembers();
	public:
		virtual int getTowerPointSize()
		{
			return 10;
		}
		virtual int getMemberSize()
		{
			return 17;
		}
		CDiaphragmFaceInsertTP31();
		~CDiaphragmFaceInsertTP31(void);	
	};


	class CDiaphragmFaceInsertTP32:public DiaphragmFaceInfo
	{
	protected:
		virtual void createPointsAndMembers();	
	public:
		virtual int getTowerPointSize()
		{
			return 10;
		}
		virtual int getMemberSize()
		{
			return 17;
		}
		CDiaphragmFaceInsertTP32();
		~CDiaphragmFaceInsertTP32(void);	
	};


	class DiaphragmFaceInsertManager
	{
	public:
		DiaphragmFaceInsertManager();
		~DiaphragmFaceInsertManager();
		void InsertToModel(CTowerModel* TheModel);
		void AddFace(const char* StrInfo);
		void clear()
		{
			_DiaphragmFaceInfos.clear();
		}
		bool empty() const
		{
			return _DiaphragmFaceInfos.empty();
		}
		void push_back(HandleDiaphragmFace pDF)
		{
			_DiaphragmFaceInfos.push_back(pDF);
		}
		const HandleDiaphragmFace& at(int Index) const
		{
			return _DiaphragmFaceInfos.at(Index);
		}
		int size() const
		{
			return (int)_DiaphragmFaceInfos.size();
		}
		void remove(size_t Index)
		{
			_DiaphragmFaceInfos.erase(_DiaphragmFaceInfos.begin()+Index);
		}
		void insert(size_t ipos,HandleDiaphragmFace  pDF)
		{
			_DiaphragmFaceInfos.insert(_DiaphragmFaceInfos.begin()+ipos,pDF);
		}
	private:
		void CreatePartDiaphragmFace(HandlePart pPart,HdElemArray partElms,const HdMemberArray& pGMemberArray);
		HandleNode FindNode(std::map<int,HandleNode>* tmpNodeMap,HandleNode pCoorNode);
		int FindMinNum(std::map<int,HandleTowerPoint>* tmpPointMap,int iN0Number);
		vector<StHandle<DiaphragmFaceInfo>> _DiaphragmFaceInfos;
	};
}