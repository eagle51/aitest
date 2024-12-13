#ifndef _MemberInf_h_
#define	_MemberInf_h_
#include <vector>
#include "Dense"
#include "config.hpp"
#include "ISym.h"
#include "RenumberingList.h"
#include "StHandle.hpp"

using namespace Eigen;
using namespace std;

namespace SmartTower 
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月25日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	class CVec3D;
	class MemberInf:public SharedObject
	{
	public:
		MemberInf(int iJ1,int iJ2,int iNmat,Symetry::Type Isym=Symetry::None,int iType=1,
			int iM=0,int iLenmin=1,int iLenpa=0,int iSMC=0);	

		MemberInf(const MemberInf&);

		MemberInf& operator=(const MemberInf& rhs);	
		~MemberInf(void);
		//成员函数
		//节点1，小号
		int MinTagNum() const 
		{
			if(m_iJ1<m_iJ2)
				return m_iJ1;
			return m_iJ2;
		};
		int J1TagNum() const
		{
			return m_iJ1;
		}

		void J1TagNum(int iJ1)
		{			
			m_iJ1=iJ1;
		};
		//节点2,大号
		int MaxTagNum() const 
		{
			if(m_iJ1>m_iJ2)
				return m_iJ1;
			return m_iJ2;
		};
		int J2TagNum() const
		{
			return m_iJ2;
		}
		//节点2
		void J2TagNum(int iJ2)
		{
			m_iJ2=iJ2;
		};

		 //对称信息
		Symetry::Type ISYM() const 
		{
			return m_Isym;
		};

		 //对称信息
		void setISYM(Symetry::Type iIs) 
		{
			m_Isym=iIs;
		};

		//类型
		int Type() const 
		{
			return m_iType;
		};
		//类型
		void Type(int iType)
		{
			m_iType=iType;
		};
		//约束类型
		int MKind() const 
		{
			return m_iMKind;
		};
		//约束类型
		void MKind(int MKind)
		{
			m_iMKind=MKind;
		};
		//最小轴
		int L1() const
		{
			return m_iLenmin;
		};
		//最小轴
		void L1(int iL1)
		{
			m_iLenmin=iL1;
		}; 
		//平行轴
		int L2() const 
		{
			return m_iLenpa;
		} 
		//平行轴
		void L2(int iL2)
		{
			m_iLenpa=iL2;
		};
		//SMC+-1,2
		int SMC() const 
		{
			return m_iTTASMC;
		}
		//SMC+-1,2
		void SMC(int iSMC)
		{
			m_iTTASMC=iSMC;
		}

		//杆件的长度单位m
		double Length() const 
		{
			return m_dLen;
		}
		//设置杆件的长度单位m
		void Length(double dLen)
		{
			m_dLen=dLen;
		}		
		//对地余玄
		double CosZ() const 
		{ 
			return _CosZ; 
		}
		//对地余玄
		void setCosZ(double val) 
		{ 
			_CosZ = val;
		}
		//检查是否符合TTA要求的定义规则。
		bool check();
		virtual TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::MEMBER;
		};
		int OrderIndex() const 
		{ 
			return _OrderIndex; 
		}
		int solveSectCount() const;
		virtual void Renumbering(RenumberingList* plist);
	private:
		friend class MetaModel;
		 //节点1
		int m_iJ1; 
		 //节点2
		int m_iJ2; 
		//TTA统材号
		int _NMatGroupNum; 
		//对称信息
		Symetry::Type m_Isym;  
		//单元类型
		int m_iType;  
		 //约束类型
		int m_iMKind; 
		//最小轴代码
		int m_iLenmin;  
		//平行轴代码
		int m_iLenpa; 
		//SMC
		int m_iTTASMC;  
		 //杆件的长度，单位m
		double m_dLen;  		 
		//对地角度余玄		
		double _CosZ;				
		//相对次序
		int _OrderIndex;		
	};
	typedef StHandle<MemberInf> HandleMemberInf;

};
#endif