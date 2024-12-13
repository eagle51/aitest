#pragma once
#include "TowerModelIO.h"
#include "DiaphragmFaceInsert.h"
namespace SmartTower
{

	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月24日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	//西北院TTAP格式输入输出
	class CXBTTAPFormatIO:public CTowerModelIO
	{
	public:		
		CXBTTAPFormatIO(CTowerModel* Model, bool bTransferToDB = false);
		~CXBTTAPFormatIO(void);
		virtual void Read();

	protected:
		DiaphragmFaceInsertManager _DiaphragmFaceInfos;
		virtual void readXBMember(int iMemberSize,istream* fin);
		virtual void readDiaphragmFaceInfos(int iNSE,istream* fin);
		size_t _GInsertionPointNum;
		size_t _GInsertionMembNum;
		Version::Type _curVersionType;
		//bool _NeedTransferMatFlag;
	};
}