#pragma once
#include "TowerPoint.h"
#include "NodeMap.h"
#include "PrimaryPoint.h"
#include "FloatVector.h"
namespace SmartTower
{
	class PrimaryPointExt :public SharedObject
	{
	private:
		int _iPointNum;   //第一类节点号		
		Symetry::Type _Isym; //对称信息		
		double _dx;   //结点X座标值		
		double _dy;   //结点X座标值		
		double _dz;   //结点X座标值		
		int _IndexX;//变量索引X		
		int _IndexY;//变量索引Y		
		int _IndexZ;//变量索引Z		
	public:
		PrimaryPointExt(int iPointNum=0,Symetry::Type isym=Symetry::None,double xcoor=0.0,double ycoor=0.0,double zcoor=0.0,
			int indexX=0,int indexY=0,int indexZ=0);
		//用于创建一类点数组
		virtual CPrimaryPoint* Creator(const FloatVector& paraVec);
		//校核数据是否正确
		virtual bool Check();
		int TagNum() const 
		{
			return _iPointNum;
		}
		void TagNum(int val) 
		{ 
			_iPointNum = val;
		}
		Symetry::Type Isym() const
		{
			return _Isym; 
		}
		void Isym(Symetry::Type val) 
		{ 
			_Isym = val; 
		}
		double x() const 
		{ 
			return _dx; 
		}
		void x(double val) 
		{
			_dx = val; 
		}
		double y() const 
		{
			return _dy;
		}
		void y(double val)
		{ 
			_dy = val; 
		}
		double z() const 
		{ 
			return _dz; 
		}
		void z(double val) 
		{ 
			_dz = val; 
		}
		int IndexX() const 
		{ 
			return _IndexX; 
		}
		void IndexX(int val) 
		{ 
			_IndexX = val;
		}
		int IndexY() const 
		{ 
			return _IndexY;
		}
		void IndexY(int val) 
		{ 
			_IndexY = val;
		} 
		int IndexZ() const 
		{ 
			return _IndexZ;
		}
		void IndexZ(int val)
		{ 
			_IndexZ = val;
		}
		virtual ~PrimaryPointExt(void);
	};
	typedef StHandle<PrimaryPointExt> HandlePrimaryPointExt;

	
}
