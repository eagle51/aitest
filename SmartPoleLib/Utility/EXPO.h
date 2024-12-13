#pragma once
namespace SmartPoleCore
{
	class CEXPO
	{
	private:			
		char _Type;
		CEXPO(char Type);
	public:
		static CEXPO A;
		static CEXPO B;
		static CEXPO C;
		static CEXPO D;
		static CEXPO E;//BsEn2012存在第五种类型,Table4.1
		static CEXPO Create(char Type);
		static CEXPO Transfer(int Type);
		friend bool operator == (const CEXPO& lhs,const CEXPO& rhs)
		{
			return lhs._Type==rhs._Type;
		}
		int Index() const
		{
			return int(_Type-'A');
		}
		char Type() const
		{
			return _Type;
		}
		~CEXPO(void);
	};

}