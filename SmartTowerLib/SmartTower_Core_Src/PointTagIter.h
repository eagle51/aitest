#pragma once
#include "StHandle.hpp"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	//本类用于检索TowerPoint中利用的点的点号
	class PointTagIter:public SharedObject
	{
	public:
		//第一个关联节点号
		virtual void First()=0;
		//下一个关联节点号
		virtual void Next()=0;
		//是否是最后一个 
		virtual bool IsEnd()=0;
		//是否存在连个利用点J1，J2
		virtual bool HasJ1J2()=0;
		//J1点号
		virtual int J1()=0;
		//J2点号
		virtual int J2()=0;
		//当前节点号
		virtual int CurTagNum()=0;
		//改变当前的节点号
		virtual void ChangeCurTagNum(int NewTag)=0;
		PointTagIter(void);
		virtual ~PointTagIter(void)=0;
	};
	typedef StHandle<PointTagIter> HandleTagIter;
}