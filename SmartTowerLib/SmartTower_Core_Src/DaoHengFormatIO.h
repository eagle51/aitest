#pragma once
#include "TTAFormatIO.h"
namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月27日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	//东北院TTA格式输入输出
	class DaoHengFormatIO:public CTTAFormatIO
	{
	public:
		DaoHengFormatIO(CTowerModel* pModel,bool bTransferToXB=false);
		~DaoHengFormatIO(void);
		virtual void Read();
	protected:
		virtual void readBodyPart(int iBodySize,HdNodeManager pNodeManager,istream* fin);
		virtual void readLegPart(int iLegSize,HdNodeManager pNodeManager,istream* fin);
		virtual void readMember(int iMemberSize,istream* fin);
		virtual void readGSMember(int iMemberSize,istream* fin);
		virtual void readGSAuxMembers(int numAuxMember,istream* fin);	

	private:
		bool _isMYLFreeFormat;
	};


	class HenanTTAFormat:public CTTAFormatIO
	{
	public:
		HenanTTAFormat(CTowerModel* pModel,bool bTransferToXB=false);
		~HenanTTAFormat(void);
		virtual void Read();
	protected:
		virtual void readMember(int iMemberSize,istream* fin);
		virtual void readGSMember(int iMemberSize,istream* fin);
	};



	class GDEPDITTAFormart:public CTTAFormatIO
	{
	public:
		GDEPDITTAFormart(CTowerModel* pModel,bool bTransferToXB=false);
		~GDEPDITTAFormart(void);
		virtual void Read();
	protected:
		virtual void readMember(int iMemberSize,istream* fin);
	};
}