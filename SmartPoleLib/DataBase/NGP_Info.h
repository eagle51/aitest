#pragma once
#include "SharedHandle.hpp"

namespace SmartPoleCore
{
	//荷载分组数据
	//规定了工况从哪里算到哪里，接腿从哪算到哪
	class NGP_Info:public SharedObject
	{
	private:
		friend class FamilyManager;
		friend class NGPManager;
		//从1开始
		int _StartLeg;
		//从1开始
		int _EndLeg;
		//从1开始
		int _StartCase;
		//从1开始
		int _EndCase;

		//使用第几列的BetaZ
		int _BetaZIndex;
		//是否使用
		//bool _isActive;


		//考虑到界面有个总的工况计算范围，和总的接腿计算范围，因此需要有个设置，计算实际计算接腿范围和工况计算范围
		//从0开始
		int _ActiveStartLeg;
		//从1开始
		int _ActiveEndLeg;
		int _ActiveStartCase;
		int _ActiveEndCase;
		double _ActiveRefMaxZ;		
		//从1开始
		void setActiveRangeLeg(int startleg,int endleg);
		void setActiveRangeCase(int startcase,int endcase);
		void setActiveRefMaxZ(double val) 
		{ 
			_ActiveRefMaxZ = val; 
		}
	public:
		NGP_Info(int StartLeg,int EndLeg,int StartCase=1,int EndCase=1);
		//是否在范围之内
		bool isLegInNGP(int LegIndex)
		{
			return LegIndex>=_StartLeg && LegIndex<=_EndLeg;
		}
		//当前这个荷载分组是否有效
		bool currentNGPIsActive() const
		{
			if(_ActiveEndLeg<0 || _ActiveEndCase<0)//必须都可以才能计算
				return false;
			return true;
		}
		//是否有存在活动的腿
		bool hasActiveLeg() const
		{
			if(_ActiveEndLeg<0)
				return false;
			return true;
		}
		//从0开始
		int getActiveStartCase() const
		{
			return _ActiveStartCase;
		}
		//从1开始
		int getActiveEndCase() const
		{
			return _ActiveEndCase;
		}
		//从0开始
		int getActiveStartLeg() const
		{
			return _ActiveStartLeg;
		}
		//从1开始
		int getActiveEndLeg() const
		{
			return _ActiveEndLeg;
		}
		//从1开始
		void setStartLegIndex(int val)
		{
			_StartLeg=val;
			if(_StartLeg<1)
				_StartLeg=1;
		}
		//从1开始
		int getStartLegIndex() const
		{
			return _StartLeg;
		}
		//从1开始
		void setEndLegIndex(int val)
		{
			_EndLeg=val;
			if(_EndLeg<1)
				_EndLeg=1;
		}
		//从1开始
		int getEndLegIndex() const
		{
			return _EndLeg;
		}
		//从1开始
		void setStartCaseIndex(int val)
		{
			_StartCase=val;
			if(_StartCase<1)
				_StartCase=1;
		}
		//从1开始
		int getStartCaseIndex() const
		{
			return _StartCase;
		}
		//从1开始
		void setEndCaseIndex(int val)
		{
			_EndCase=val;
			if(_EndCase<1)
				_EndCase=1;
		}
		//从1开始
		int getEndCaseIndex() const
		{
			return _EndCase;
		}
		//从1开始
		int getBetaZIndex() const 
		{ 
			return _BetaZIndex;
		}

		void setBetaZIndex(int val) 
		{
			if(val<1)
				_BetaZIndex=1;
			else
				_BetaZIndex = val;
		}

		bool equal(NGP_Info* rhs) const
		{
			return _StartLeg==rhs->_StartLeg && _EndLeg==rhs->_EndLeg && _StartCase==rhs->_StartCase && _EndCase==rhs->_EndCase;
		}
		double getActiveRefMaxZ() const 
		{ 
			return _ActiveRefMaxZ; 
		}
		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::NGPINFO;
		}
		~NGP_Info(void);
	};
	typedef SharedHandle<NGP_Info> HandleNGP;
}