#pragma once
#include "config.hpp"
#include "TowerPoint.h"
#include "Node.h"
#include "NodeMap.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	class CSecondaryPoint :public CTowerPoint
	{
	private: 
		int m_iJ1;   //第一点
		int m_iJ2;   //第二点
		double m_fRatio;  //比例
	public:
		CSecondaryPoint(int iPointNum=0,Symetry::Type isym=Symetry::None,int StartPoint=0,int EndPoint=0,double fRatio=1);
		int StartPoint() const
		{
			return m_iJ1;
		}
		int EndPoint() const 
		{
			return m_iJ2;
		}
		double Ratio() const 
		{
			return m_fRatio;
		}
		void StartPoint(int startPoint)
		{
			m_iJ1=startPoint;
		}
		void EndPoint(int endPoint)
		{
			m_iJ2=endPoint;
		}
		void Ratio(double fRatio)
		{
			m_fRatio=fRatio;
		}
		virtual bool Equal(CTowerPoint* tmpPoint);
		virtual TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::SECPOINT;
		}; //返回点的类型
		//virtual CNode *CalcCoor(HdNodeArray *aPointSet); //计算节点坐标值
		virtual CNode *CalcCoor(NodeMap *aPointSet); //计算节点坐标值
		virtual PointTagIter* UsingTagIter();
		virtual bool Check();
		virtual CTowerPoint* DeepCopy();
		virtual bool isRefer(int tagNum); 
		virtual void Renumbering(RenumberingList* list);
		virtual CTowerPoint* ToXBSecondaryPoint();
		virtual void TryChangeRatioToInt();
		~CSecondaryPoint(void);
	};
	typedef StHandle<CSecondaryPoint> HandleSecondaryPoint;

	//本类用于检索TowerPoint中利用的点的点号
	class SecTagIter:public PointTagIter
	{
	private:
		CSecondaryPoint* _SecPoint;
		int _Index;
	public:
		//第一个关联节点号
		virtual void First();
		//下一个关联节点号
		virtual void Next();
		//是否是最后一个
		virtual bool IsEnd();
		//是否存在连个利用点J1，J2
		virtual bool HasJ1J2()
		{
			return true;
		}
		//J1点号
		virtual int J1()
		{
			return _SecPoint->StartPoint();
		}
		//J2点号
		virtual int J2()
		{
			return _SecPoint->EndPoint();
		}
		//当前节点号
		virtual int CurTagNum();
		//改变当前的节点号
		virtual void ChangeCurTagNum(int newTag);
		SecTagIter(CSecondaryPoint* SecPoint);
		~SecTagIter(void);
	};
}