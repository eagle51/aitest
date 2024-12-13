#pragma once
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月30日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	class CSubInteger 
	{
	private:
		long m_Int;
		static long a[];   
	public:
		CSubInteger(long intOrgin = 0) :m_Int(intOrgin){};
		~CSubInteger(void);
		void SetIntOrigin(long intOrgin){ m_Int = intOrgin; };
		long GetIntOrigin() const
		{
			return m_Int;
		} 
		//个位填0，十位填1，百位2...
		int GetSubInt(const size_t iPosition) const 
		{
			return (m_Int%a[iPosition+1])/(a[iPosition]);
		};
		void SetSubInt(const size_t iPosition,const size_t must0_9);
		CSubInteger &operator=(long intOrgin)
		{
			this->m_Int = intOrgin;
			return *this;
		};
	};
}
