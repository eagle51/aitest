#pragma once
#include "TTAFormatIO.h"
namespace SmartTower
{//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月29日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	//浙江院的TST数据格式
	class ZheJiangTSTFormatIO:public CTTAFormatIO
	{
	public:
		ZheJiangTSTFormatIO(CTowerModel* pModel,bool bTransferToXB=false); 
		~ZheJiangTSTFormatIO(void);
	protected:
		virtual void readMember(int iMemberSize,istream* fin);
	};
}
