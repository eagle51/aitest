#pragma once
#include <string>
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	class LegPartLB
	{

	public:		
		LegPartLB(int iLBFlag=0);
		LegPartLB(int allIterFlag,int iFoundationNum,int iBodyPartIndex);
		~LegPartLB(void); 
		//是否全迭代,个位数如果为1表示全迭代，
		//如果为2则为高低腿迭代，中间用插值法
		int AllIterFlag() const 
		{ 
			return _allIterFlag; 
		}
		void AllIterFlag(int val) 
		{ 
			_allIterFlag = val; 
			if(_allIterFlag>2)
				_allIterFlag=1;
			else if(_allIterFlag<0)
				_allIterFlag=1;
		}
		int getCurrentIterFlag(bool isLastLoop, int onlyHLFlag) const;

		//一组腿的数目
		int FoundationNum() const 
		{
			return _FoundationNum;
		}
		void FoundationNum(int val)
		{ 
			_FoundationNum = val;
		}
		//本体的索引号
		int BodyPartIndex() const 
		{ 
			return _BodyPartIndex; 
		}
		void BodyPartIndex(int val)
		{ 
			_BodyPartIndex = val; 
		}
		void LBFlag(int Flag)
		{
			if(Flag>999 || BodyGreaterThan10)
			{
				_BodyPartIndex=Flag%100;
				_FoundationNum=(Flag%10000-_BodyPartIndex)/100;
				AllIterFlag(Flag/10000);
			}
			else
			{
				if(Flag<0)Flag=0;
				_BodyPartIndex=Flag%10;
				_FoundationNum=(Flag%100-_BodyPartIndex)/10;
				AllIterFlag(Flag/100);
			}
		}
		int LBFlag() const
		{
			if(_BodyPartIndex>9 || _FoundationNum>9 || BodyGreaterThan10)
			{
				return _allIterFlag*10000+_FoundationNum*100+_BodyPartIndex;
			}
			else
			{
				return _allIterFlag*100+_FoundationNum*10+_BodyPartIndex;
			}
		}
		const char* getDaoLBFlag(int curBodyIndex=0) const;
		static bool BodyGreaterThan10;
	private:
		static std::string lbCharStr;
		int _allIterFlag;		
		int _FoundationNum;		
		int _BodyPartIndex;		
	};

}