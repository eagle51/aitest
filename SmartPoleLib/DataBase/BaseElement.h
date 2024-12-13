#ifndef	_BaseElement_POLE_h_
#define	_BaseElement_POLE_h_
//#include "Sparse"
#include "Node.h"
#include "Member.h"
#include "..\Utility\Constants.h"

namespace SmartPoleCore
{    
	//有限元中的单元
	//默认第三个点为定义XOZ面，因此默认横挡垂直还是Z轴，水平还是Y轴
	class BaseElement :public SharedObject
	{
	protected:
		static double _ZEpsion;
		//static Matrix3d TransMat;
		static int _MoveValue;
		double m_dLength;
		CVec3D linVec;
		HandleNode _pNodeJ1;
		int _J1PointNum;

		HandleNode _pNodeJ2;
		int _J2PointNum;

		//本段的截面类型
		HandleProfile _CurProfile;
		//本段的材料
		//HandleMaterial _CurMatriel;

		HandleMember m_pBelongToMenb;//	

		int _Id;
	public:
		
		BaseElement(const HandleNode &startPoint, const HandleNode &endPoint, const HandleMember &belongToMenb, const HandleProfile& pProfile,int startPointNum=0,int endPointNum=0);
		
		bool isVertical() const
		{
			return linVec.isVertical();
		}
		int getId() const 
		{ 
			return _Id; 
		}
		void setId(int val)
		{ 
			_Id = val;
		}
// 		HandleMaterial getCurMatriel() const
// 		{
// 			return _CurMatriel;
// 		}
		HandleProfile getCurProfile() const
		{
			return _CurProfile;
		}
		//最小号
		int minNum() const
		{
			if (_pNodeJ1->TagNum() < _pNodeJ2->TagNum())
				return _pNodeJ1->TagNum();
			return _pNodeJ2->TagNum();
		}
		//起节点号
		int StartNum() const 
		{
			return _pNodeJ1->TagNum();
		};
		//要校验的起端对应的截面号或者点号，不需要校验或者内部杆件为0
		int StartPointNum() const
		{
			return _J1PointNum;
		}

		//最大号
		int maxNum() const
		{
			if (_pNodeJ1->TagNum() > _pNodeJ2->TagNum())
				return _pNodeJ1->TagNum();
			return _pNodeJ2->TagNum();
		}
		//要校验的终端对应的截面号或者点号，不需要校验或者内部杆件为0
		int EndPointNum() const
		{
			return _J2PointNum;
		}
		//终点号
		int EndNum() const 
		{
			return _pNodeJ2->TagNum();
		};
		//起点的x比终点的x小，x相同的情况下，z小，z相同的情况下y小
		const HandleNode& StartNode() const 
		{
			return _pNodeJ1;
		};
		//起点
		const HandleNode& EndNode() const
		{
			return _pNodeJ2;
		};
		const HandleMember& GetMember() const
		{
			return m_pBelongToMenb;
		};
		const CVec3D& getLineVec() const
		{
			return  linVec;
		}

		double getMidZ() const
		{
			return 0.5*(_pNodeJ1->z() + _pNodeJ2->z());
		}

		bool CheckOnLine(const HandleNode& pNode);

		//获取杆件长度m
		double getLen() const 
		{
			return m_dLength;
		}
		//获取杆件重量，单位kN
		double getWeight() const
		{ 
			return _CurProfile->getArea()*0.000001*m_dLength*SmartPoleCore::Constants::SteelDensity*SmartPoleCore::Constants::g*0.001;
		}
		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::ELEMENT;
		}
		bool numberEqual(const BaseElement* rhs) const 
		{
			return (this->minNum() == rhs->minNum() && this->maxNum() == rhs->maxNum());
		};
		virtual ~BaseElement(void);
	};
	typedef SharedHandle<BaseElement> HandleElement;
}
#endif
