#pragma once
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	class Symetry
	{
	public:
		enum Type
		{
			None=0,
			Xsym=1,
			Ysym=2,
			Osym=3,
			XYsym=4,
		};
		static Type Tansfer(int tag)
		{
			switch(tag)
			{ 
			case 0:
				return None;
			case 1:
				return Xsym;
			case 2:
				return Ysym;
			case 3:
				return Osym;
			case 4:
				return XYsym;
			default:
				return None;
			}
		}
		//对称关系，如果是自身为None，当不对称,且无关时XYsym
		static Type Relationship(int TagNumlhs,int TagNumRhs)
		{
			switch(TagNumRhs-TagNumlhs)
			{
			case 0:
				return None;
			case 1:
			case -1:
				return Xsym;
			case 2:
			case -2:
				return Ysym;
			case 3:
			case -3:
				return Osym;
			default:
				return XYsym;
			}
		}
		static int Count(Type tp)
		{
			if(tp==Symetry::None)
				return 1;
			else if(tp==Symetry::XYsym)
				return 4;
			else
				return 2;
		}
	};
}