#pragma once
#include "PointArray.h"
#include "MemberArray.h"
#include "NodeArray.h"
#include "ElemArray.h"
// #include "ElmInnerForceArray.h"
// #include "NodeDisplacementArray.h"
namespace SmartPoleCore
{
	//多杆的划分器，多杆之间根据点号进行管理
	class Component:public SharedObject
	{
	public:
		Component(int maxNum = 0, int cnnIndex = -1);
		~Component();
		//几何模型最大点号，不超过10000
		int getMaxNumber() const 
		{
			return _MaxNumber;
		}
		//几何模型最大点号，不超过10000
		void setMaxNumber(int val)
		{ 
			_MaxNumber = val; 
		}
		//用户定义的挂接本体序号，起始0，不连接填-1；界面显示时+1
		int getUserDefineCnnIndex() const 
		{ 
			return _UserCnnIndex;
		}
		//所连接的本体号，起始0，不连接填-1；界面显示+1
		int getCnnIndex() const
		{
			if (_PartPos == 0)//第一行时
				return -1;
			else if (_UserCnnIndex < 0)
				return _PartPos - 1;
			return _UserCnnIndex;
		}
		//如果连接序号没有指定那就是接腿
		bool isLegType() const
		{
			return _UserCnnIndex >=0;
		}
		//连接本体号，起始0，不连接填-1
		void setUserDefineCnnIndex(int val) 
		{ 
			_UserCnnIndex = val; 
		}
		const HdPointArray& getPointArray() const
		{
			return _PointArray;
		}
		const HdMemberArray& getMemberArray() const
		{
			return _MemberArray;
		}
		int getPosition() const
		{
			return _PartPos;
		}
		bool getIsDraw() const { return _isDraw; }
		void setIsDraw(bool val) { _isDraw = val; }
		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::COMPONENT;
		};
	private:
		friend class FamilyManager;

		void initAndCheck(int upMaxNum);

		bool _isDraw;

		int _PartPos;//位置
		int _upMaxNumber;//上一段最大号码

		int _MaxNumber;//最大号码
		int _UserCnnIndex;//用户定义的挂接序号
		HdPointArray _PointArray;
		HdMemberArray _MemberArray;
	};

	typedef SharedHandle<Component> HandleComponent;

}
