#pragma once
#include "config.hpp"
#include "ISym.h"
#include "StHandle.hpp"
#include "PointTagIter.h"
#include "RenumberingList.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
using namespace std;
namespace SmartTower
{
	class CNode;
	class NodeMap;
	class CTowerPoint:public SharedObject
	{
	public:
		int TagNum() const  
		{
			return m_iPointNum;
		}
		//对称信心
		Symetry::Type Isym() const 
		{
			return m_Isym;
		}
		void TagNum(int iPointNum)
		{
			m_iPointNum=iPointNum;
		}
		void Isym(Symetry::Type is)
		{
			m_Isym=is;
		}
		virtual bool IsAbsoluteCoordinates()
		{
			return false;
		}
		virtual TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::UNDEFINE;
		}
		virtual CNode *CalcCoor(NodeMap *aPointSet)=0;
		virtual PointTagIter* UsingTagIter()=0;
		CTowerPoint(int iPointNum=0,Symetry::Type isym=Symetry::None);
		virtual ~CTowerPoint(void);
		//必须各项属性都正确为真
		virtual bool Equal(CTowerPoint* tmpPoint)=0;
		//校验是错误
		virtual bool Check()=0;
		//深复制
		virtual CTowerPoint* DeepCopy()=0;
		//重名名
		virtual void Renumbering(RenumberingList* list)=0;
		//是否引用某个点
		virtual bool isRefer(int tagNum)=0; 
		//是否需要DXF标注
		virtual bool bDxfDimMark() const;
		//是否为基础作用点
		virtual bool isFoundationPoint() const;
		//设置为基础点
		virtual void setFoundationPointFlag(bool flag);
	protected:
		int m_iPointNum;   //第一类节点号
		Symetry::Type m_Isym;		//对称信息
		bool checkIS(int isym)
		{
			if (isym > 4 || isym < 0)
				return true;
			return false;
		}
	};
	typedef StHandle<CTowerPoint> HandleTowerPoint;

};