#pragma once
#include "TowerPoint.h"
#include "NodeMap.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	class CPrimaryPoint :public CTowerPoint
	{
	private:
		double m_dx;   //结点X座标值
		double m_dy;   //结点X座标值 
		double m_dz;   //结点X座标值
		bool _isFdtPt;
	public:
		CPrimaryPoint(int iPointNum=0,Symetry::Type isym=Symetry::None,double xcoor=0.0,double ycoor=0.0,double zcoor=0.0);
		double x()	const 	
		{
			return m_dx;
		};

		double y() const 
		{
			return m_dy;
		};

		double z() const 
		{
			return m_dz;
		};

		void x(double xcoor)
		{
			m_dx=xcoor;
		}; 

		void y(double ycoor)
		{
			m_dy=ycoor;
		}; 

		void z(double zcoor)
		{
			m_dz=zcoor;
		};
		virtual bool Equal(CTowerPoint* tmpPoint);
		virtual bool IsAbsoluteCoordinates()
		{
			return (m_dx+m_dy+m_dz)<10000;
		}
		virtual CNode* CalcCoor(NodeMap *aPointSet); //计算节点坐标值
		virtual PointTagIter* UsingTagIter();
		 //返回点的类型
		virtual TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::PRIPOINT;
		};
		virtual bool Check();
		virtual CTowerPoint* DeepCopy();
		virtual void Renumbering(RenumberingList* list);
		virtual bool isRefer(int tagNum); 
		virtual bool bDxfDimMark() const;
		virtual bool isFoundationPoint() const;
		virtual void setFoundationPointFlag(bool flag);
		~CPrimaryPoint(void);
	};
	typedef StHandle<CPrimaryPoint> HandlePrimaryPoint;

	//本类用于检索TowerPoint中利用的点的点号
	class PriTagIter:public PointTagIter
	{
	private:
		CPrimaryPoint* _PriPoint;
		int _Index;
		int _CurrentTag;
		void getCurrent();
	public:
		//第一个关联节点号
		virtual void First();
		//下一个关联节点号
		virtual void Next();
		//是否是最后一个
		virtual bool IsEnd();
		//是否存在连个利用点J1，J2
		virtual bool HasJ1J2();
		//J1点号
		virtual int J1();
		//J2点号
		virtual int J2();
		//当前节点号
		virtual int CurTagNum();
		//改变当前的节点号
		virtual void ChangeCurTagNum(int newTag);
		PriTagIter(CPrimaryPoint* priPoint);
		~PriTagIter(void);
	};
}
