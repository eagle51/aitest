#ifndef _CLegPart_H_
#define _CLegPart_H_
#include "BasePart.h"
using namespace std;
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月27日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	class CLegPart: public CBasePart 
	{		
	private: 
		//载荷开始工况号
		int m_iStartCaseNum;
		//载荷终止工况号
		int m_iEndCaseNum;

		virtual void initial(NodeArray *gPointSet, ElemArray *gLinks); //向该部分增加杆件,初始化
	public:		
 		CLegPart(int iJmin,int iFmin,int iFmax,int iAuxMAX,LegPartLB iLB,int iVar=0,int startCaseNum=0,int endCaseNum=0);
		double GetMaxZ() const;
		int getMaxZTag() const;
		//空的没有杆件
		virtual bool IsEmpty() const
		{
			return _bodyElmArray->Empty() && _fdtElmArray->Empty();
		}
		//从1开始
		void setStartCaseNum(int val) 
		{
			m_iStartCaseNum=val;
		}
		//从1开始
		int getStartCaseNum() const 
		{
			return m_iStartCaseNum;
		}
		//从1开始
		void setEndCaseNum(int val) 
		{
			m_iEndCaseNum=val;
		}
		//从1开始
		int getEndCaseNum() const 
		{
			return m_iEndCaseNum;
		}		
		//最小号
		virtual void JminTagNum(int Jmin)
		{
			m_Jmin=Jmin;
		};
		//返回该段最小号
		virtual int JminTagNum() const 
		{
			return m_Jmin;
		}; 
		//返回基础最小号
		virtual int FminTagNum() const 
		{
			return m_Fmin;
		};
		//返回基础最小号
		virtual void FminTagNum(int val)
		{
			m_Fmin=val;
		};
		virtual int ConnectionIndex() const 
		{
			return m_LB.BodyPartIndex()-1;			
		};
		TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::LEGPART;
		}
		virtual CBasePart* DeepCopy() const 
		{
			return new CLegPart(*this);
		}
		//距离dZCoor最近的上基础点
		HandleNode getUpFdtPoint(double dZCoor);
		//距离dZCoor最近的下基础点
		HandleNode getDownFdtPoint(double dZCoor);
		//获取已经存在的主材上端点
		HandleNode getExistLegUpNode(int fdtNum = -1) const;
		//获取可能存在的最大的主材上端点
		HandleNode getMayLegUpNode()const;
		//获取可能存在的最大斜材上端点
		HandleNode findMayBracingUpNode(const HandleNode& upKeyNode,bool bFornt)const;

		int MaxActiveAuxNum() const;

		virtual ~CLegPart(void);
	};
	typedef StHandle<CLegPart> HandleLegPart;
}
#endif