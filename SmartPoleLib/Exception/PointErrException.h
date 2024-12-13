#pragma once
#include "SmartException.h"
#include "Point.h"
namespace SmartPoleCore
{	
	class PointErrException:public SmartException
	{
	public:
		enum ErrType
		{
			DuplicateNum=0,         //节点号重复输入
			NumErr=1,               //节点编号不符合规则
			RefPointErr=2,          //利用的点无法（生成）找到！_
			RatioErr=3,             //利用的点重合，或者比例错误
			FoundationZErr=4,       //同一呼高基础点节点高度Z相同
			FoundationIsymErr=5,	//基础点的对称性不为4
			CoorErr=6,              //坐标值错误
			SymAxisErr =7,          //对称性错误，对称点重合
			IsymErr=8,              //对称轴符号错误
			IsLinkMem = 9,              //是否连接杆件
		};
		PointErrException(const HandlePoint& ErrPoint,ErrType ErrInfo);
		~PointErrException(void);
		virtual bool isRefPointErr() const
		{
			if (_errInFo == RefPointErr)
				return true;
			return false;
		}

		virtual bool isSymErr() const
		{
			if (_errInFo == IsymErr)
				return true;
			return false;
		}
		virtual bool isDuplicate() const
		{
			if (_errInFo == DuplicateNum)
				return true;
			return false;
		}
	private:
		void getErrInfo();
		ErrType _errInFo;
	};

}