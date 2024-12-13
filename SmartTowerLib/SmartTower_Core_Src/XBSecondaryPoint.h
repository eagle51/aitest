#pragma once
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
	class CXBSecondaryPoint:public CTowerPoint
	{
	private:
		int _J1;   //第一点
		int _J2;   //第二点
		int _J3;   //第三点
		int _J4;   //第四点
		int _J5;   //第五点
	public:
		CXBSecondaryPoint(int iPointNum=0,int iJ1=0,int iJ2=0,int iJ3=0,int iJ4=0,int iJ5=0,Symetry::Type iIIS=Symetry::None);
		const int J1() 
		{
			return _J1;
		};
		void J1(int J1)
		{
			_J1=J1;
		}
		const int J2()
		{
			return _J2;
		};
		void J2(int J2)
		{
			_J2=J2;
		}
		const int J3()
		{
			return _J3;
		}
		void J3(int J3)
		{
			_J3=J3;
		}
		const int J4()
		{
			return _J4;
		};
		void J4(int J4)
		{
			_J4=J4;
		}
		const int J5()
		{
			return _J5;
		}
		void J5(int J5)
		{
			_J5=J5;
		}
		virtual bool Equal(CTowerPoint* tmpPoint);
		virtual TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::XBSECPOINT;
		}; //返回点的类型
		//virtual CNode *CalcCoor(HdNodeArray *aPointSet); //计算节点坐标值
		virtual CNode *CalcCoor(NodeMap *aPointSet); //计算节点坐标值
		virtual PointTagIter* UsingTagIter();
		virtual bool Check();
		virtual CTowerPoint* DeepCopy();
		virtual void Renumbering(RenumberingList* list);
		virtual bool isRefer(int tagNum); 
		virtual CTowerPoint* globalToDBSecondaryPoint();
		virtual CTowerPoint* ToDBSecondaryPoint(const HandleNode& pJ1Node,const HandleNode& pJ2Node,const HandleNode& meNode);
		virtual bool bDxfDimMark() const;
		~CXBSecondaryPoint(void);
	};

	typedef StHandle<CXBSecondaryPoint> HandleXBSecPoint;

	//本类用于检索TowerPoint中利用的点的点号
	class XBSecTagIter:public PointTagIter
	{
	private:
		CXBSecondaryPoint* _XBSecPoint;
		int _Index;
		int _CurTag;
		void Current();
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
			return _XBSecPoint->J1()%10000;
		}
		//J2点号
		virtual int J2()
		{
			return _XBSecPoint->J2()%10000;
		}
		//当前节点号
		virtual int CurTagNum();
		//改变当前的节点号
		virtual void ChangeCurTagNum(int NewTag);
		XBSecTagIter(CXBSecondaryPoint* XBSecPoint);		
		~XBSecTagIter(void);
	};

}