#pragma once
#include <vector>
#include "NGP_Info.h"
#include "..\Utility\IntVector.h"
#include "..\Utility\IProcessBar.h"
using namespace std;
namespace SmartPoleCore
{
	class NGPManager:public SharedObject
	{
	private:
		//载荷开始工况号,从1开始
		int m_iStartCaseNum;
		//载荷终止工况号,从1开始
		int m_iEndCaseNum;
		//开始计算接腿号,从1开始
		int m_iStartLegNum;
		//终止计算接腿号,从1开始
		int m_iEndLegNum;
		vector<HandleNGP> _NGPArray;
		static IntVector ActiveCaseFlag;
		static IntVector ActiveLegFlag;
	public:		
		//从1开始
		void setStartCaseNum(int iStartCaseNum)
		{
			if(iStartCaseNum<1)
				iStartCaseNum=1;
			m_iStartCaseNum=iStartCaseNum;
		};
		//从1开始
		int getStartCaseNum() const
		{
			return m_iStartCaseNum;
		};
		//从1开始
		void setEndCaseNum(int iEndCaseNum)
		{
			if (iEndCaseNum < 1)
				iEndCaseNum = 1;
			m_iEndCaseNum=iEndCaseNum;
		};
		//从1开始
		int getEndCaseNum() const
		{
			return m_iEndCaseNum;
		};

		void setStartLegNum(int iStartLegNum)
		{
			if (iStartLegNum < 1)
				iStartLegNum = 1;
			m_iStartLegNum = iStartLegNum;
		};
		int getStartLegNum() const
		{
			return m_iStartLegNum;
		};
		void setEndLegNum(int iEndLegNum)
		{
			if (iEndLegNum < 1)
				iEndLegNum = 1;
			m_iEndLegNum = iEndLegNum;
		};
		int getEndLegNum() const
		{
			return m_iEndLegNum;
		};

		//添加不检查
		void push_back(const HandleNGP& pNGP)
		{
			_NGPArray.push_back(pNGP);
		}
		//插入检查
		void insert(const HandleNGP& pNGP,int iPos)
		{
			_NGPArray.insert(_NGPArray.begin()+iPos,pNGP);
		}

		int Has(const HandleNGP& pNGP);

		void AddUnique(const HandleNGP& pNGP)
		{
			if(Has(pNGP)<0)
				_NGPArray.push_back(pNGP);
		}
		void pop_back()
		{
			_NGPArray.pop_back();
		}
		void clear()
		{
			_NGPArray.clear();
		}
		const HandleNGP& back()
		{
			return _NGPArray.back();
		}
		const HandleNGP& front()
		{
			return _NGPArray.front();
		}
		int removeRef(const HandleNGP& pNGP);

		void remove(int iPos)
		{
			_NGPArray.erase(_NGPArray.begin()+iPos);
		}
		void setTotalProcess(const HdProcessBar& pBar);
		HandleNGP getActiveNGP(int Index) const
		{
			if (_NGPArray.empty())
				return new NGP_Info(m_iStartLegNum, m_iEndLegNum, m_iStartCaseNum, m_iEndCaseNum);
			else
				return _NGPArray[Index];
		}
		const HandleNGP& getAt(int Index)
		{
			return _NGPArray[Index];
		}
		size_t size() const
		{
			return _NGPArray.size();
		}
		bool empty() const
		{
			return _NGPArray.empty();
		}
		void Check(int TotalLegCount,int TotalCaseCount);
		bool HasOverLapCase() const;
		//partIndex从1开始
		HandleNGP getPartNGP(int partIndex) const;
		NGPManager* DeepCopy();
		NGPManager(int iStartCaseNum=1,int iEndCaseNum=1);
		~NGPManager(void);
	};
	typedef SharedHandle<NGPManager> HdNGPManager;
}