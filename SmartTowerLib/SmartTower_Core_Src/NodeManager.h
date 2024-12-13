#pragma once
#include "LegPart.h"
#include "IntVector.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////

namespace SmartTower
{
	//该类的主要作用是管理节点分配表，计算接腿数目，检查节点分配表的正确性等。
	class CNodeManager:public SharedObject
	{		
	private:
		friend class CTowerModel;
		//friend class CSingleTower;
		bool _isNormalTower;
		size_t _BodyCount; //本体数
		vector<HandlePart> _PartArray;//本体+接腿总数
		//开始计算腿号从0开始
		size_t m_iStartLegNum;
		//终止接腿号
		size_t m_iEndLegNum;
		//向该部分增加杆件,初始化
		void initial(NodeArray *gPointSet,ElemArray *gLinks);//向该部分增加杆件,初始化

		void setPartIndexAndUpMaxNum(); 
		void checkConnectNumErr();
		void setBodyTransitionTminTmax();
		void setMutiTowerLB();
	public:
		static bool AddAuxPoint;
		CNodeManager(void);
		virtual ~CNodeManager(void);
		
		HandlePart InPart(int iNum);

		bool IsAFoundationNode(const HandleNode& pNode)  const;

		HandlePart At(size_t Index) const
		{
			return _PartArray[Index];
		}

		const HandlePart& getEndPart() const
		{
			return _PartArray.back();
		}

		const HandlePart& getFrontPart() const
		{
			return _PartArray.front();
		}

		HandlePart getEndBodyPart() const
		{
			return _PartArray.at(_BodyCount-1);
		}

		HandlePart getStartLegPart() const
		{
			return _PartArray.at(_BodyCount);
		}
		HandleLegPart getLegPartThrZ(double zCoor) const;

		HandleLegPart getMaxHighLegPart() const;

		HandleLegPart getLegPart_At(size_t idx) const;

		void AddPart(HandlePart pPart,int iPos=-1);

		int ReomvePart(HandlePart pPart);

		size_t getCount() const
		{
			return _PartArray.size();
		};

		size_t getBodyCount() const
		{
			return _BodyCount;
		};
		
		bool isLegEmpty() const
		{
			return (_PartArray.size() - _BodyCount) < 1;
		}

		size_t getLegCount() const
		{
			return (_PartArray.size()-_BodyCount);
		};

		bool IsSglTower() const
		{
			return (_BodyCount==0 && _PartArray.size()==1);
		};

		bool hasLegExtendBody() const;

		bool IsLegSingleIter() const; 

		bool IsNormalTower() const
		{
			return _isNormalTower;
		}
		void setIsNormalTower(bool val)
		{ 
			_isNormalTower= val; 
		}
		int getTTA_NE() const;

		//开始计算腿号从1开始
		void setStartLegNum(size_t iStartLegNum)
		{
			if(iStartLegNum<1 || IsSglTower())
				m_iStartLegNum=1;
			else if(iStartLegNum>getLegCount())
				m_iStartLegNum=getLegCount();
			else
				m_iStartLegNum=iStartLegNum;
			if(getLegCount()==0)
				m_iStartLegNum=1;
		};

		//开始计算腿号从1开始
		int getStartLegNum() const
		{
			return int(m_iStartLegNum);
		};

		//结束计算腿号从1开始
		void setEndLegNum(size_t iEndLegNum)
		{
			if(iEndLegNum<m_iStartLegNum || IsSglTower())
				m_iEndLegNum=m_iStartLegNum;
			else if(iEndLegNum>getLegCount())
				m_iEndLegNum=getLegCount();
			else
				m_iEndLegNum=iEndLegNum;
			if(getLegCount()==0)
				m_iEndLegNum=1;
		};

		//结束计算腿号从0开始
		int getEndLegNum() const
		{
			return int(m_iEndLegNum);
		};
		CBox3D getLegMaxBox() const;

		virtual CNodeManager* DeepCopy();

		bool ConnectToPart(int partIndex,HandlePart pLegPart);

        double getTowerSlope(NodeArray* pNodeArray)const;

		IntVector maxHighCnnPartVec();
	};
	typedef StHandle<CNodeManager> HdNodeManager;
}

