#include "JSONIo.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <exception>
#include "TTAType.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{	
	JSONIo::JSONIo(CTowerModel* pModel)
		:_pModel(pModel)
	{

	}

	JSONIo::~JSONIo(void)
	{

	}

	void JSONIo::read(const char* username)
	{
		std::ifstream  fin(username, std::ios::in);
		if(fin.fail())
			throw std::exception("无法打开json文件");

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(fin, root, false))
            throw std::exception("无法打开json文件");
		
		int ICODE=1004;
		if(!root["Code"].isNull())
			ICODE=root["Code"].asInt();
		int IBOLTFlag=334;
		if(!root["IBOLTFlag"].isNull())
			IBOLTFlag=root["IBOLTFlag"].asInt();


		this->readHeadStr(root);
		this->readPart(root);
		this->readPrimaryPoints(root);
		this->readSecPoint(root);
		this->readXBSecPoint(root);
		this->readPrimaryMember(root);
		fin.close();
	}


	void JSONIo::readPart( Json::Value& root )
	{
		HdNodeManager pNodeManager=new CNodeManager();
		_pModel->setNodeManager(pNodeManager);
		const Json::Value& jsonNodeManager=root["NodeManager"];
		if(jsonNodeManager.isNull())
			return;
		Json::Value BodyVal=jsonNodeManager["BodyCount"];
		Json::Value LegVal=jsonNodeManager["LegCount"];
		int BodyCount=0;
		int LegCount=0;
		if(!BodyVal.isNull())
			BodyCount=BodyVal.asInt();
		if(!LegVal.isNull())
			LegCount=LegVal.asInt();

		const Json::Value& jsonPartList=jsonNodeManager["PartList"];
		if(jsonPartList.isNull())
			return;
		for(int iloop=0;iloop<BodyCount;iloop++)
		{
			const Json::Value& partVal=jsonPartList[iloop];
			pNodeManager->AddPart(new CBasePart(partVal["Jmin"].asInt(),partVal["Fmin"].asInt(),
				partVal["Fmax"].asInt(),partVal["AuxMax"].asInt(),partVal["LB"].asInt(),0));
		}
		for(int iloop=BodyCount;iloop<(BodyCount+LegCount);iloop++)
		{
			const Json::Value& partVal=jsonPartList[iloop];
			pNodeManager->AddPart(new CLegPart(partVal["Jmin"].asInt(),partVal["Fmin"].asInt(),
				partVal["Fmax"].asInt(),partVal["AuxMax"].asInt(),partVal["LB"].asInt(),partVal["Var"].asInt()));
		}

		Json::Value startLegVal=jsonNodeManager["StartLegIndex"];
		Json::Value endLegVal=jsonNodeManager["EndLegIndex"];
		int startLegIndex=1;
		int endLegIndex=1;
		if(!startLegVal.isNull())
			startLegIndex=startLegVal.asInt();
		if(!endLegVal.isNull())
			endLegIndex=endLegVal.asInt();
		pNodeManager->setStartLegNum(startLegIndex);
		pNodeManager->setEndLegNum(endLegIndex);
	}

	void JSONIo::readPrimaryPoints( Json::Value& root )
	{
		HdTowerPointArray pPrimaryPoints=new TowerPointArray();
		_pModel->setPrimaryPointArray(pPrimaryPoints);
		const Json::Value& jsonPrimaryPoints=root["PrimaryPoints"];
		if(jsonPrimaryPoints.isNull())
			return;
		int arraycount=jsonPrimaryPoints.size();
		for (int iloop=0;iloop<arraycount;++iloop)
		{
			const Json::Value& priPoint=jsonPrimaryPoints[iloop];
			Symetry::Type Isym=Symetry::Tansfer(priPoint["Symmetry"].asInt());
			HandleTowerPoint pPoint=new CPrimaryPoint(priPoint["Number"].asInt(),Isym,priPoint["x"].asDouble(),priPoint["y"].asDouble(),priPoint["z"].asDouble());
			if(pPoint->TagNum()<Constants::MinAuxTagNum)
				pPrimaryPoints->push_back(pPoint);
		}
	}

	void JSONIo::readSecPoint( Json::Value& root )
	{
		const Json::Value& jsonSecondaryPoints=root["SecondaryPoints"];
		if(jsonSecondaryPoints.isNull())
			return;
		HdTowerPointArray pSecondaryPoints=new TowerPointArray();
		_pModel->setSecondaryPointArray(pSecondaryPoints);
		CTTAType::Instance()->ActiveSecType(TowBaseObject::SECPOINT);
		int arraycount=jsonSecondaryPoints.size();
		for (int iloop=0;iloop<arraycount;++iloop)
		{
			const Json::Value& secPoint=jsonSecondaryPoints[iloop];
			Symetry::Type Isym=Symetry::Tansfer(secPoint["Symmetry"].asInt());
			HandleTowerPoint tmpSecPoint=new CSecondaryPoint(secPoint["Number"].asInt(),Isym,secPoint["J1"].asInt(),secPoint["J2"].asInt(),secPoint["Ratio"].asDouble());
			if(tmpSecPoint->TagNum()<Constants::MinAuxTagNum)
				pSecondaryPoints->push_back(tmpSecPoint);	
		}
	}

	void JSONIo::readXBSecPoint( Json::Value& root )
	{
		const Json::Value& jsonSecondaryPoints=root["XBSecondaryPoints"];
		if(jsonSecondaryPoints.isNull())
			return;
		HdTowerPointArray pSecondaryPoints=new TowerPointArray();
		_pModel->setSecondaryPointArray(pSecondaryPoints);
		CTTAType::Instance()->ActiveSecType(TowBaseObject::XBSECPOINT);
		int arraycount=jsonSecondaryPoints.size();
		for (int iloop=0;iloop<arraycount;++iloop)
		{
			const Json::Value& secPoint=jsonSecondaryPoints[iloop];
			Symetry::Type Isym=Symetry::Tansfer(secPoint["Symmetry"].asInt());
			HandleTowerPoint tmpSecPoint=new CXBSecondaryPoint(secPoint["Number"].asInt(),secPoint["J1"].asInt(),secPoint["J2"].asInt(),
				secPoint["J3"].asInt(),secPoint["J4"].asInt(),secPoint["J5"].asInt(),Isym);
			if(tmpSecPoint->TagNum()<Constants::MinAuxTagNum)
				pSecondaryPoints->push_back(tmpSecPoint);	
		}
	}

	void JSONIo::readPrimaryMember( Json::Value& root )
	{
		HdMemberArray pMenberArray=new MemberArray;
		_pModel->setMemberArray(pMenberArray);
		const Json::Value& PrimaryMembersValue=root["PrimaryMembers"];
		if(PrimaryMembersValue.isNull())
			return;
		this->readMembers(pMenberArray,PrimaryMembersValue);
	}

	void JSONIo::readMembers( HdMemberArray& pMemberArray,const Json::Value& jsonMembers )
	{
		int arraycount=jsonMembers.size();
		for (int iloop=0;iloop<arraycount;++iloop)
		{
			const Json::Value& memberVal=jsonMembers[iloop];
			Symetry::Type Isym=Symetry::Tansfer(memberVal["Symmetry"].asInt());
			HandleMemberInf tempMenb=new MemberInf(memberVal["J1"].asInt(),memberVal["J2"].asInt(),0,
				Isym,memberVal["Type"].asInt(),memberVal["M"].asInt(),memberVal["L1"].asInt(),memberVal["L2"].asInt(),memberVal["SMC"].asInt());
			pMemberArray->push_back(tempMenb);
		}
	}


	void JSONIo::readHeadStr( Json::Value& root )
	{
		HandleStringContainer CaptionStrs=new StringContainer();
		_pModel->SetCaptionStrs(CaptionStrs);
		const Json::Value& jsonHeadStr=root["PROJECTNOTE"];
		if(jsonHeadStr.isNull())
			return;
		CaptionStrs->addString(jsonHeadStr["PROJECT"].asCString(),0);
		CaptionStrs->addString(jsonHeadStr["CONDITION"].asCString(),1);
		CaptionStrs->addString(jsonHeadStr["DESIGN"].asCString(),2);		
	}
}
