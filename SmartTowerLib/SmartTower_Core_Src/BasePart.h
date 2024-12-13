#ifndef _CBasePart_H_
#define _CBasePart_H_
#include <vector>
#include "NodeArray.h"
#include "ElemArray.h"
#include "SubInteger.h"
#include "LegPartLB.h"
using namespace std;

//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////

namespace SmartTower
{
	class CBasePart:public SharedObject
	{//基类，节点分配基类
	//基类的任务就是，找出本段区域类所有段内的点，这些点用于计算本段内的风载荷
	//排序在在整体刚度矩阵中的位置，集合本段风载荷的节点力。
		friend class CNodeManager;

	protected:
		int m_upFmax;//上一段可能的最大值
		int m_Jmin;//查找之后确定，
		int m_Fmin;//腿上的最小基础节点号
		int m_Fmax;//腿上的最大基础节点号
		int m_UpAuxTagMax;//上一段腿的最大辅助材点号	
		int m_AuxMAX;//本体或接腿上的辅助材最大端节点号		
		LegPartLB m_LB;//迭代情况
		CSubInteger m_Var;//具体高低腿情况
		int m_extendNum;//延伸段最小点号

		int _PartIndex;//与那个本体相连
		bool _isActive;//本段是否参与最终计算

		HdNodeArray _bodyNodeArray;//塔身
		HdElemArray _bodyElmArray;//塔身杆件

		HdNodeArray _auxNodeArray;//辅助点
		HdElemArray _auxElmArray;//辅助杆件

		HdNodeArray _extendNodeArray;//延伸的节点数组
		HdElemArray _extendElmsArray;//延伸受力材杆件

		HdNodeArray _fdtNodeArray;//本段基础
		HdElemArray _fdtElmArray;//基础杆件

		HdNodeArray _allNodeArray;//所有受力材节点（Body+Fdt）
		HdElemArray _allElmArray;//所有受力材杆件

		
		int _cnnFmin;//连接段的Fmin
		int _cnnFmax;//连接段的Fmax

		double _TotalHBetaZNuA;		//用于中国规范中用于计算加权平均betaZ
		double _TotalHNuZ;		//用于中国规范中用于计算加权平均betaZ

		CBasePart* _cnnPart;
		//是否参与最后计算
		virtual void initial(NodeArray *gPointSet, ElemArray *gLink); //向该部分增加杆件,初始化
	public:
		CBasePart(int iJmin,int iFmin,int iFmax,int iAuxMAX,LegPartLB iLB,int iVar=0);
		//上一部分的可能最大值
		int UpMaxTagNum() const 
		{
			return m_upFmax;
		}
		int getPartIndex() const
		{
			return _PartIndex;
		}	
		//用于中国规范中用于计算加权平均betaZ
		double TotalHBetaZNuA() const 
		{ 
			return _TotalHBetaZNuA; 
		}
		//用于中国规范中用于计算加权平均betaZ
		void setTotalHBetaZNuA(double val) 
		{ 
			_TotalHBetaZNuA = val; 
		}
		//用于中国规范中用于计算加权平均betaZ
		double TotalHNuZ() const 
		{ 
			return _TotalHNuZ; 
		}
		//用于中国规范中用于计算加权平均betaZ
		void setTotalHNuZ(double val)
		{ 
			_TotalHNuZ = val;
		}
		//该段最小号
		virtual void JminTagNum(int Jmin)
		{
			m_Fmin=Jmin;
			m_Jmin=Jmin;
		};
		//返回该段最小号
		virtual int JminTagNum() const 
		{
			return m_Jmin;
		}; 
		int getLegExtendMinNum() const
		{
			if (m_extendNum <= 0)
				return m_Fmin;
			return m_extendNum;
		}
		int getFileFmin() const
		{
			if (m_extendNum == 0)
				return m_Fmin;
			return -m_Fmin;
		}
		//返回基础最小号
		virtual int FminTagNum() const 
		{
			return m_Fmin;
		};
		//返回基础最小号
		virtual void FminTagNum(int val)
		{
			m_Jmin=val;
			m_Fmin=val;
		};

		//返回最大值
		int FmaxTagNum() const 
		{
			return m_Fmax;
		};
		void FmaxTagNum(int val)
		{
			m_Fmax=val;
		};
		//获取连接的部分的编号
		virtual int ConnectionIndex() const 
		{
			if(m_LB.BodyPartIndex()==0 ||  m_LB.BodyPartIndex()>=_PartIndex)//如果大于0
				return _PartIndex-1;
			return m_LB.BodyPartIndex()-1;
		};
		void AuxMaxTagNum(int val) 
		{
			m_AuxMAX = val; 
		}
		int UpAuxTagMax() const 
		{ 
			return m_UpAuxTagMax;
		}
		//辅助才最大号码
		int AuxMaxTagNum() const 
		{
			return m_AuxMAX;
		};		
		//本体返回0；接腿返回1
		TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::BASEPART;
		}

		void LB(LegPartLB iLB)
		{
			m_LB=iLB;
		}
		//接退个数等信息
		const LegPartLB& LB() const
		{
			return m_LB;
		}
		
		//迭代信息等
		int Var()  const 
		{
			return m_Var.GetIntOrigin();
		};
		void Var(int var) 
		{
			m_Var=var;
		};
		//延伸段
		int getExtendNum() const 
		{ 
			return m_extendNum; 
		}
		//延伸段
		void setExtendNum(int val)
		{
			m_extendNum = val; 
		}
		//是否参与最后计算
		bool IsActive() const 
		{
			return _isActive;
		};	
		
		CBasePart* getCnnPart() const
		{
			return _cnnPart;
		}

#ifdef _DEBUG
		//写出东西看对不对
		void writeout(int outnum);
#endif	
		//塔身节点
		const HdNodeArray& getBodyNodeArray() const
		{
			return _bodyNodeArray;
		}
		//塔身杆件
		const HdElemArray& getBodyElmArray() const
		{
			return _bodyElmArray;
		}
		const HdNodeArray& getExtendNodeArray() const
		{
			return _extendNodeArray;
		}
		const HdElemArray& getExtendElmsArray() const
		{
			return _extendElmsArray;
		}

		//如果是塔身，就是交界地方的点或者杆件；如果是塔腿就是基础位置的点和杆件
		const HdNodeArray& getFdtNodeArray() const
		{
			return _fdtNodeArray; 
		}

		//如果是塔身，就是交界地方的点或者杆件；如果是塔腿就是基础位置的点和杆件
		const HdElemArray& getFdtElmArray() const
		{
			return _fdtElmArray;
		}

		const HdNodeArray& getAllNodeArray()const
		{
			return _allNodeArray;
		}

		const HdElemArray& getAllElmArray()const
		{
			return _allElmArray;
		}

		//辅助材节点
		const HdNodeArray& getAuxNodeArray() const
		{
			return _auxNodeArray;
		}

		//辅助材杆件
		const HdElemArray& getAuxElmArray() const
		{
			return _auxElmArray;
		}

		////点号是否属于这一段的
		bool bInPart(int iNum) const 
		{
			if(((iNum>m_upFmax)&&(iNum<=m_Fmax)) || (iNum>m_UpAuxTagMax)&&(iNum<m_AuxMAX))
				return true;
			else 
				return false;
		}
		bool InBoundarySegOrInPart(int iNum) const 
		{
			if ((iNum>=_cnnFmin && iNum<=_cnnFmax) || (iNum>m_upFmax && iNum<=m_Fmax)  || (iNum>m_UpAuxTagMax)&&(iNum<m_AuxMAX) )
				return true;
			else
				return false;
		}		
		
		bool extendNumErr() const;

		bool extendElmErr() const;
		//空的没有杆件
		virtual bool IsEmpty() const
		{
			return _bodyElmArray->Empty();
		}

		virtual CBasePart* DeepCopy() const 
		{
			return new CBasePart(*this);
		}	
		virtual ~CBasePart(void);
	};
	typedef StHandle<CBasePart> HandlePart;
}
#endif
