#pragma once
#include <vector>
#include "IntVector.h"
#include "TowerModel.h"
#include "SplitStr.h"
#include "MemberArray.h"

namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月23日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	class CTowerModelIO:public SharedObject
	{
	public:
		CTowerModelIO(CTowerModel* Model,bool bTransferToB=false);
		virtual void Read()=0;
		virtual ~CTowerModelIO(void);

		void setTowerModel(CTowerModel* val)
		{
			_ThisModel = val;
		}
		void modelFileHasPostfix(bool val) 
		{ 
			_hasPostfix = val; 
		}
		static bool beDatWriteMembProp;
	protected:
		virtual void readHeadDatInfos(istream* fin);
		virtual void readMutiPart(int iBaseLegFlag,istream* fin);
		virtual void readBodyPart(int iBodySize,HdNodeManager pNodeManager,istream* fin);
		virtual void readLegPart(int iLegSize,HdNodeManager pNodeManager,istream* fin);
		virtual void readNGP(int iNGP,bool bMutiBetaZ,istream* fin);
		virtual void readPrimaryPoint(int iPrimaryPointSize,istream* fin);
		virtual void readSecPoint(int iSecondaryPointSize,istream* fin);
		virtual void readXBSecPoint(int iSecondaryPointSize,istream* fin);
		virtual void readMember(int iMemberSize,istream* fin);
		virtual void readXBMember(int iMemberSize,istream* fin);

		int _IBOLT; //螺栓次序		
		int _INA; //排序	
		bool _isSmartTowerNewFormat;
		bool _bTransferTXBDB;
		CSplitStr _SubStrs;
		bool _hasPostfix;	
		bool _GroupNumGT9000;
		CTowerModel* _ThisModel;		
	};
	typedef StHandle<CTowerModelIO> HandleModelIO;
}