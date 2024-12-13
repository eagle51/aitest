#pragma once
#include "TowerModelIO.h"
#include <string>
#include "SplitStr.h"
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月24日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	//东北院TTA格式输入输出 
	class CTTAFormatIO:public CTowerModelIO
	{
	public:
		virtual void Read();
		CTTAFormatIO(CTowerModel* Model,bool bTransferToXB=false);
		~CTTAFormatIO(void);
	protected:
		virtual void readGSMember(int iMemberSize,istream* fin);
	};



	class NEPDITower:public CTTAFormatIO
	{
	public:
		NEPDITower(CTowerModel* Model,bool bTransferToXB=false);
		~NEPDITower();
		virtual void Read();
	private:
		virtual void readXBMember(int iMemberSize,istream* fin);
	};
}