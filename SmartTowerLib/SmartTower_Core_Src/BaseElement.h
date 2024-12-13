#ifndef	_BaseElement_h_
#define	_BaseElement_h_
#include "Sparse"
#include "Node.h"
#include "MemberInf.h"

namespace SmartTower
{    
	class CBaseElement:public SharedObject
	{ //单元基类
	protected:
		static double _ZEpsion;
		static long long _MoveValue;
		double m_dLength;  //单元长度m 
		double c[3]; //三个方向的余玄。
		long long _ELmkey;	
		StHandle<CNode> m_piNodeJ1;
		StHandle<CNode> m_piNodeJ2; 
		StHandle<MemberInf> m_pBelongToMenb;//					
		void CalcLength();
	public:
		double CosZZ() const
		{
			return c[2];
		};
		double CosYY() const
		{
			return c[1];
		};
		double CosXX() const
		{
			return c[0];
		};
		int StartNum() const 
		{
			return m_piNodeJ1->TagNum();
		};
		int EndNum() const 
		{
			return m_piNodeJ2->TagNum();
		};
		//起点
		const HandleNode& StartNode() const 
		{
			return m_piNodeJ1;
		};
		//起点
		const HandleNode& EndNode() const
		{
			return m_piNodeJ2;
		};
		const HandleMemberInf& GetMenber() const 
		{
			return m_pBelongToMenb;
		};
		CVec3D getLineVec() const
		{
			return CVec3D(c[0],c[1],c[2]);
		}
		bool CheckOnLine(const HandleNode& pNode);
		bool isColinear(const CBaseElement* pLine);
		//获取杆件长度m
		double getLen() const 
		{
			return m_dLength;
		}
		virtual TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::ELEMENT;
		}
		long long  getKey() const 
		{
			return _ELmkey;
		}
		bool operator==(const CBaseElement &rhs) const 
		{
			return (m_piNodeJ1->TagNum()==rhs.m_piNodeJ1->TagNum() && m_piNodeJ2->TagNum()==rhs.m_piNodeJ2->TagNum());
		};
		static long long CalcElmKey(int iStartNum,int iEndNum)
		{
			if(iEndNum>iStartNum)
				return _MoveValue*iEndNum+iStartNum;
			return _MoveValue*iStartNum+iEndNum;
		}
		 CBaseElement(const HandleNode &pStartPoint,const HandleNode &pEndPoint,const HandleMemberInf &pBelongToMenb);
		 virtual ~CBaseElement(void);
	};
	typedef StHandle<CBaseElement> HandleElement;
}
#endif
