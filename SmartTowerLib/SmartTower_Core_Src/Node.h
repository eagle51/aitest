#ifndef _CNode_H_
#define _CNode_H_
#include "Vec3D.h"
#include "StHandle.hpp"
#include "IntVector.h"
#include "TowerPoint.h"

//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月29日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////

using namespace Eigen;
using namespace std;
namespace SmartTower
{
	//包含点的局部坐标系
	class GBCoordinateSystem:public SharedObject 
	{
	private:
		CVec3D _OriginalCoor;		
		CVec3D _AxisX;				
		CVec3D _AxisY;		
		CVec3D _AxisZ;		
		static Matrix3d* _TransferMat;
	public:		
		//定义SSE求解集，使得每一个类初始化后都可以达到16bit倍数
		//GBCoordinateSystem();
		GBCoordinateSystem(CVec3D *pOrigin,CVec3D *pPointX,CVec3D *pPointY);
		CVec3D OriginalCoor() const 
		{ 
			return _OriginalCoor; 
		}

		const CVec3D& AxisX() const
		{ 
			return _AxisX; 
		}

		const CVec3D& AxisY() const 
		{ 
			return _AxisY; 
		}

		const CVec3D& AxisZ() const 
		{ 
			return _AxisZ; 
		}

		void Rot(const CVec3D& axisV,double degree);

		//Returns a coordinate transformation matrix defined by the given coordinate system.
		//With the returned matrix points can be transformed from the given coordinate system 
		//to the world coordinate system. 
		const Matrix3d* TranferMat() const;
		
		~GBCoordinateSystem(void);
	};
	typedef StHandle<GBCoordinateSystem> HandleCoorSystem;

	class dofset
	{
	private:
		int _dofflag;
	public:
		static const int max_known_dof;
		static const int Xdisp;
		static const int Ydisp;
		static const int Zdisp;
		static const int XYZdisp;
		static const int Xrot;
		static const int Yrot;
		static const int Zrot;
		static const int XYZrot;
		static const int FixedAll;//bUsed
		dofset(int flag=0);
		void DofClear(){_dofflag=0;}
		void Mark(int iflag){_dofflag|=iflag;}//固定某个自由度,
		void UnMark(int iflag){_dofflag = _dofflag ^ (_dofflag & iflag);}//取消某个自由度固定
		int isFixed(int iflag){return (_dofflag & iflag);}//查看是否固定
		int Doflist(){return _dofflag;}
		bool HasFixedDOF(){return _dofflag>0;}
		~dofset(void);
	};		
	
	//采用侵入式引用计数
	class CNode:public SharedObject,public dofset,public CVec3D
	{

	public:
		enum SingularKind
		{		
			//孤立点
			Singular=0,	
			//线上的点
			Line1D_X=2,
			Line1D_Y=3,
			Line1D_Z=4,
			//平面内上的点
			Plane2D  =5,
			Plane2D_X=6,
			Plane2D_Y=7,
			Plane2D_Z=8,	
			//空间点
			Space3D  =10,
		};


		CNode(int iNum=0,double xcoor=0,double ycoor=0,double zcoor=0,HandleTowerPoint iOriginNum=0);

#ifdef TowerNewDeleteOp
		inline static void * operator new(size_t size)
		{
			return sgi_std::alloc::allocate(size);
		}
		inline static void operator delete(void *p, size_t size)
		{
			sgi_std::alloc::deallocate(p,size);
		}
#endif
		 int TagNum()  const 
		 {
			 return _TagNum;
		 }
		const HandleTowerPoint& GetOriginNum() const 
		 {
			 return _OriginNum;
		 }
		 void SetDofStartIndex(int iIndex)
		 {
			 _DoFStartIndex=iIndex;
		 }//设置开始的自由度
		 void SetDofNum(int iNumDof)
		 {
			 _DOFCount=iNumDof;
		 }//设置节点的自由度类型
		 inline int GetDofStart() const 
		 {
			 return _DoFStartIndex;
		 }
		 inline int GetDofNum() const 
		 {
			 return _DOFCount;
		 }
		 //获取x自由度
		 inline int GetXDofIndex() const 
		 {
			 return (_DoFStartIndex);
		 };
		 //获取y自由度
		 inline int GetYDofIndex() const 
		 {
			 return (_DoFStartIndex+1);
		 };
		 //获取z自由度
		 inline int GetZDofIndex() const 
		 {
			 return (_DoFStartIndex+2); 
		 };
		 //获取x旋转自由度
		 inline int GetRotXDofIndex() const 
		 {
			 return (_DoFStartIndex+3);
		 };
		 //获取y旋转自由度
		 inline int GetRotYDofIndex() const 
		 {
			 return (_DoFStartIndex+4);
		 };
		 //获取z旋转自由度
		 inline int GetRotZDofIndex() const 
		 {
			 return (_DoFStartIndex+5);
		 };
		 ~CNode(void);
		 virtual TowBaseObject::Type GetObjType() const
		 {
			 return TowBaseObject::NODE;
		 };
		 bool isValid()
		 {
			 if(abs(this->x())<1000.0 && abs(this->y())<1000.0 && abs(this->y())<1000.0)
				 return true;
			 return false;
		 }
		 //根据对称性获取对称点的节点号
		 int ISymTagNum(Symetry::Type isym);

		 CNode* getISNode(Symetry::Type isym);

		 int CurrentaffiliateSymetryFlag() const
		 {
			 return _TagNum%10;
		 }

		 int LogicalAffiliateSymetryFlag() const;

		 bool IsFirstQuadrant() const;

		 SingularKind SingularType() const 
		 { 
			 return _SingularType;
		 }
		 void SingularType(SingularKind val) 
		 { 
			 _SingularType = val;
		 }	
		 //检查模型中是否有对称性为Isym的点
		 bool HasRealNode(Symetry::Type isym);
		 //友元类主类，对外界是看不到自由度索引的，但是可以看到是否约束
	private:
		int _TagNum;    //节点号
		HandleTowerPoint _OriginNum;     //原来的号码是多少,第一类点货第二类点号是多少
		int _DoFStartIndex;//自由度开始号码
		int _DOFCount;//自由度数目，如果是梁节点则是6个自由度，如果是杆节点则是3个自由度。
		SingularKind _SingularType;	
	}; 
	typedef StHandle<CNode> HandleNode;		
}
#endif

