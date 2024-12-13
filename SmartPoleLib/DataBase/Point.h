#pragma once
#include <map>
#include "Vec3D.h"
#include "..\Utility\RenumberingList.h"
#include "..\Utility\Constants.h"

namespace SmartPoleCore
{
	class CNode;
	class PointMap;
	//钢管杆的输入参考点(界面中参数)
	class Point :public CVec3D
	{
	private:
		double _refX;   //关键点X坐标值
		double _refY;   //关键点Y坐标值
		double _refZ;   //关键点Z坐标值
		bool _isFixed;
		double _DrawZ;  //结点Z坐标值(绘图用这个)

	public:
		Point(int iPointNum = 0, double xcoor = 0.0, double ycoor = 0.0, double zcoor = 0.0,bool bfixed=false);
		Point(const CVec3D* coorPt, int newTagNum, bool bfixed = false);

		//参考X坐标
		double getRefX()	const
		{
			return _refX;
		};
		//参考Y坐标
		double getRefY() const
		{
			return _refY;
		};
		//参考Z坐标
		double getRefZ() const
		{
			return _refZ;
		};
		//绘图Z坐标
		double getDrawZ() const
		{
			return _DrawZ;
		};
		//参考X坐标
		void setRefX(double xcoor)
		{
			_refX=xcoor;
		}; 
		//参考Y坐标
		void setRefY(double ycoor)
		{
			_refY=ycoor;
		}; 
		//参考Z坐标
		void setRefZ(double zcoor)
		{
			_refZ=zcoor;
		};
		//绘图Z坐标
		void setDrawZ(double zcoor)
		{
			_DrawZ = zcoor;
		};
		//是否固定
		bool isFixed() const 
		{ 
			return _isFixed; 
		}
		//设置固定方式
		void setFixed(bool val)
		{
			_isFixed = val;
		}
		//重命名
		virtual void Renumbering(RenumberingList* list);
		//跟某个点关联
		virtual bool IsReferTo(int tagNum);
		//计算点的坐标
		virtual bool CalcCoor(const PointMap* aPointSet);
		//校验是否符合规则
		virtual bool Check();

		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::POINT;
		};
		//绝对坐标点
		bool isAbsoluteCoordinates()
		{
			return (abs(_refX) + abs(_refY) + abs(_refZ)) < SmartPoleCore::Constants::SysMaxTagNum;
		}
		~Point(void);

// 		virtual void outputToStream(QDataStream& dataStream) const
// 		{
// 			dataStream << _refX << _refY << _refZ << _isFixed;
// 		}
// 		virtual void inputFromStream(QDataStream& dataStream)
// 		{
// 			dataStream >> _refX >> _refY >> _refZ >> _isFixed;
// 		}
	};
	typedef SharedHandle<Point> HandlePoint;
}
