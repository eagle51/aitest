#include "JsonFormatIO.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "NonPipeProfile.h"
#include "pipeProfile.h"
#include "EXPO.h"
// #include "VInsulatorRepartition.h"
// #include "TriInsulatorRepartition.h"
// #include "NormalLoadRepartition.h"
// #include "MaterialDataBase.h"
// #include "BoltDataBase.h"
// #include "MainPoleRapid.h"
// #include "SinglePoleRapid.h"
// #include "TubularHoriStretcher.h"



namespace SmartPoleCore
{
	JsonFormatIO::JsonFormatIO()
	{

	}


	JsonFormatIO::~JsonFormatIO()
	{
	}

	void JsonFormatIO::readPointArray(Json::Value& root)
	{
		HdPointArray pPrimaryPoints = new PointArray();
		ModelDateBase::Instance()->setPointArray(pPrimaryPoints);
		const Json::Value& jsonPrimaryPoints = root["02_PointArray"];
		if (jsonPrimaryPoints.isNull())
			return;
		int arraycount = jsonPrimaryPoints.size();
		for (int iloop = 0; iloop < arraycount; ++iloop)
		{
			const Json::Value& priPoint = jsonPrimaryPoints[iloop];
			HandlePoint pPoint = new Point(priPoint["1_Number"].asInt(),
				priPoint["2_X"].asDouble(),
				priPoint["3_Y"].asDouble(),
				priPoint["4_Z"].asDouble());
			pPoint->setFixed(priPoint["5_isFixed"].asBool());
			pPrimaryPoints->add(pPoint);
		}
	}

	void JsonFormatIO::writePointArray(Json::Value& root)
	{
		HdPointArray pPrimaryPoints = ModelDateBase::Instance()->getPointArray();
		if (pPrimaryPoints.isNull() || pPrimaryPoints->empty())
			return;
		Json::Value jsonPrimaryPoints(Json::arrayValue);
		for (int iloop = 0; iloop < pPrimaryPoints->count(); ++iloop)
		{
			HandlePoint pPriPoint = pPrimaryPoints->getAt(iloop);
			if (pPriPoint.isNull())
				continue;
			Json::Value priPoint(Json::objectValue);
			priPoint["1_Number"] = Json::Value(pPriPoint->TagNum());
			priPoint["2_X"] = Json::Value(pPriPoint->x());
			priPoint["3_Y"] = Json::Value(pPriPoint->y());
			priPoint["4_Z"] = Json::Value(pPriPoint->z());
			priPoint["5_isFixed"] = Json::Value(pPriPoint->isFixed());
			jsonPrimaryPoints.append(priPoint);
		}
		root["02_PointArray"] = jsonPrimaryPoints;
	}

	void JsonFormatIO::readNodeArray(Json::Value& root)
	{
//  		HdNodeArray pSecondaryPoints = new PointArray();
// 		ModelDateBase::Instance()->setNodeArray(pSecondaryPoints);
//  		const Json::Value& jsonSecondaryPoints = root["SecondaryPoints"];
//  		if (jsonSecondaryPoints.isNull())
//  			return;
//  		int arraycount = jsonSecondaryPoints.size();
//  		for (int iloop = 0; iloop < arraycount; ++iloop)
//  		{
//  			const Json::Value& secPoint = jsonSecondaryPoints[iloop];
//  			Symetry::Type Isym = Symetry::Tansfer(secPoint["Symmetry"].asInt());
//  			HandlePoint tmpSecPoint = new SecondaryPoint(secPoint["Number"].asInt(),
//  				secPoint["J1"].asInt(), 
//  				secPoint["J2"].asInt(),
//  				secPoint["J3"].asInt(),
//  				secPoint["J4"].asInt(), 
//  				secPoint["J5"].asInt(), 
//  				Isym,
//  				secPoint["SymAxis"].asInt());
//  
//  			pSecondaryPoints->Add(tmpSecPoint);
//  		}
	}

	void JsonFormatIO::writeNodeArray(Json::Value& root)
	{
// 		HdPointArray pSecondaryPoints = _SubstationModel->getSecondaryPointArray();
// 		if (pSecondaryPoints.isNull() || pSecondaryPoints->Empty())
// 			return;
// 		Json::Value jsonSecPoints(Json::arrayValue);
// 		for (int iloop = 0; iloop < pSecondaryPoints->Count(); ++iloop)
// 		{
// 			HandlePoint pTPriPoint = pSecondaryPoints->GetAt(iloop);
// 			SecondaryPoint* pSecPoint = static_cast<SecondaryPoint*>(pTPriPoint.GetPointee());
// 			Json::Value secPoint(Json::objectValue);
// 			secPoint["Number"] = Json::Value(pSecPoint->TagNum());
// 			secPoint["Symmetry"] = Json::Value(pSecPoint->ISym());
// 			secPoint["J1"] = Json::Value(pSecPoint->J1());
// 			secPoint["J2"] = Json::Value(pSecPoint->J2());
// 			secPoint["J3"] = Json::Value(pSecPoint->J3());
// 			secPoint["J4"] = Json::Value(pSecPoint->J4());
// 			secPoint["J5"] = Json::Value(pSecPoint->J5());
// 			jsonSecPoints.append(secPoint);
// 		}
// 		root["SecondaryPoints"] = jsonSecPoints;
	}

	void JsonFormatIO::readMemberArray(Json::Value& root)
	{
		HdMemberArray pMenberArray = new MemberArray;
		ModelDateBase::Instance()->setMemberArray(pMenberArray);
		const Json::Value& PrimaryMembersValue = root["03_Members"];
		if (PrimaryMembersValue.isNull())
			return;
		this->readMembers(pMenberArray, PrimaryMembersValue);
	}

	void JsonFormatIO::readMembers(HdMemberArray& pMemberArray, const Json::Value& jsonMembers)
	{
		HdPointArray pPrimaryPoints = ModelDateBase::Instance()->getPointArray();
		int arraycount = jsonMembers.size();
		for (int iloop = 0; iloop < arraycount; ++iloop)
		{
			const Json::Value& memberVal = jsonMembers[iloop];

			HandlePoint pStartPt = pPrimaryPoints->hasPoint(memberVal["01_StartNum"].asInt());//不用setNum 
			HandlePoint pEndPt = pPrimaryPoints->hasPoint(memberVal["02_EndNum"].asInt());
			Profile::Type PoleEdgeTemp = Profile::Type(memberVal["04_sPoleEdge"].asInt());
			HandleProfile startProfile, endProfile;
			if (PoleEdgeTemp == Profile::Type::CSone || PoleEdgeTemp == Profile::Type::CStwo)
			{
				startProfile = new NonPipeProfile(PoleEdgeTemp, memberVal["05_sNonPipeHigh"].asInt()
													, memberVal["06_sNonPipeWide"].asInt(), memberVal["09_sUDThickness"].asInt(), memberVal["10_sSideThickness"].asInt());
				endProfile = new NonPipeProfile(PoleEdgeTemp, memberVal["07_eNonPipeHigh"].asInt()
													, memberVal["08_eNonPipeWide"].asInt(), memberVal["11_eUDThickness"].asInt(), memberVal["12_eSideThickness"].asInt());
			}
			else
			{
				startProfile = new pipeProfile(PoleEdgeTemp, memberVal["05_sPipeWide"].asInt(), memberVal["06_sPipeThk"].asInt());
				endProfile = new pipeProfile(PoleEdgeTemp, memberVal["07_ePipeWide"].asInt(), memberVal["08_ePipeThk"].asInt());
			}

			HandleMember tempMenb = new Member(Member::TYPE(memberVal["03_MemberType"].asInt()), pStartPt, startProfile, pEndPt, endProfile);
// 			Connect::Type cnnSType = Connect::Type(memberVal["13_StartConnect"].asInt());
// 			Connect::Type cnnEType = Connect::Type(memberVal["14_EndConnect"].asInt());
			
//			tempMenb->setEndConnect(new Connect(cnnEType));

// 			int tempNum = 0;
//  			if (cnnSType == Connect::Type::Flange)
//  			{
//  				HdlFlangeConnect FlangeConnect = new CircleFlangeConnect(cnnSType);
//  				readFlangeConnectToJson(memberVal["StartConnect"][tempNum], FlangeConnect);
//  				tempMenb->setStartConnect(FlangeConnect.GetPointee());
//  			}
// 			else if (cnnSType == Connect::Type::StiPlate || cnnSType == Connect::Type::TroughPlate  || cnnSType == Connect::Type::DoublePlate)
// 			{
// 				HdlPlateConnect plateConnect = new PlateConnect(cnnSType);
// 				readPlateConnectToJson(memberVal["StartConnect"][tempNum], plateConnect);
// 				tempMenb->setStartConnect(plateConnect.GetPointee());
// 			}
// 			else if (cnnSType == Connect::Type::SquareFlange)
// 			{
// 				HdlSquareFlangeConnect squareFlangeConnect = new SquareFlangeConnect(cnnSType);
// 				readSquareFlangeConnectToJson(memberVal["StartConnect"][tempNum], squareFlangeConnect);
// 				tempMenb->setStartConnect(squareFlangeConnect.GetPointee());
// 			}
// 
// 			if (cnnEType == Connect::Type::Flange)
// 			{
// 				HdlFlangeConnect FlangeConnect = new CircleFlangeConnect(cnnEType);
// 				readFlangeConnectToJson(memberVal["EndConnect"][tempNum], FlangeConnect);
// 				tempMenb->setEndConnect(FlangeConnect.GetPointee());
// 			}
// 			else if (cnnEType == Connect::Type::StiPlate || cnnEType == Connect::Type::TroughPlate || cnnEType == Connect::Type::DoublePlate)
// 			{
// 				HdlPlateConnect plateConnect = new PlateConnect(cnnEType);
// 				readPlateConnectToJson(memberVal["EndConnect"][tempNum], plateConnect);
// 				tempMenb->setEndConnect(plateConnect.GetPointee());
// 			}
// 			else if (cnnEType == Connect::Type::SquareFlange)
// 			{
// 				HdlSquareFlangeConnect squareFlangeConnect = new SquareFlangeConnect(cnnEType);
// 				readSquareFlangeConnectToJson(memberVal["EndConnect"][tempNum], squareFlangeConnect);
// 				tempMenb->setEndConnect(squareFlangeConnect.GetPointee());
// 			}
// 
// 			HandleMaterial pMat;
// 			if (memberVal["15_Marterial"].isString())
// 				pMat = MaterialDataBase::Instance()->Find(memberVal["15_Marterial"].asCString());
// 			tempMenb->setMatProp(pMat);
			pMemberArray->add(tempMenb);
 		}
	}

	void JsonFormatIO::writeMemberArray(Json::Value& root)
	{
		HdMemberArray pMemberArray = ModelDateBase::Instance()->getMemberArray();
		if (pMemberArray.isNull() || pMemberArray->empty())
			return;
		Json::Value jsonMembers(Json::arrayValue);
		this->wirteMembers(pMemberArray, jsonMembers);
		root["03_Members"] = jsonMembers;
	}


	void JsonFormatIO::wirteMembers(const HdMemberArray& pMemberArray, Json::Value& jsonMembers)
	{
		for (int iloop = 0; iloop < pMemberArray->count(); iloop++)
		{
			HandleMember pMemb = pMemberArray->getAt(iloop);
			if (pMemb.isNull())
				continue;
			Json::Value memberVal(Json::objectValue);
			memberVal["01_StartNum"] = pMemb->getStartNum();
			memberVal["02_EndNum"] = pMemb->getEndNum();
			memberVal["03_MemberType"] = pMemb->getTYPE();
// 			if (pMemb->getStartConnect().isNull())//备注：未在member的构造函数中默认初始值，所以必须判断是否为空
// 				memberVal["13_StartConnect"] = -1; //UNDEFINED，看read中
// 			else
// 				memberVal["13_StartConnect"] = pMemb->getStartConnect()->getCnnType();
// 			if (pMemb->getEndConnect().isNull())
// 				memberVal["14_EndConnect"] = -1;
// 			else
// 				memberVal["14_EndConnect"] = pMemb->getEndConnect()->getCnnType();
// 			if (pMemb->getMatProp().isNull())
// 				memberVal["15_Marterial"] = "Q235";
// 			else
// 			   memberVal["15_Marterial"] = pMemb->getMatProp()->getName();

			if (!pMemb->getStartProfile().isNull())
			{
				memberVal["04_sPoleEdge"] = pMemb->getStartProfile()->getType();
				if (pMemb->getStartProfile()->isPipe())
				{
					HdlpipeProfile pStartPipeProfile = dynamic_cast<pipeProfile*>(pMemb->getStartProfile().GetPointee());
					memberVal["05_sPipeWide"] = pStartPipeProfile->getUIWide();
					memberVal["06_sPipeThk"] = pStartPipeProfile->getThk();

					HdlpipeProfile pEndPipeProfile = dynamic_cast<pipeProfile*>(pMemb->getEndProfile().GetPointee());
					memberVal["07_ePipeWide"] = pEndPipeProfile->getUIWide();
					memberVal["08_ePipeThk"] = pEndPipeProfile->getThk();
				}
				else
				{
					HdlNonPipeProfile pStartNonPipeProfile = dynamic_cast<NonPipeProfile*>(pMemb->getStartProfile().GetPointee());
					memberVal["05_sNonPipeHigh"] = pStartNonPipeProfile->getHigh();
					memberVal["06_sNonPipeWide"] = pStartNonPipeProfile->getUIWide();
					memberVal["09_sUDThickness"] = pStartNonPipeProfile->getUDThickness();
					memberVal["10_sSideThickness"] = pStartNonPipeProfile->getThk();
					HdlNonPipeProfile pEndNonPipeProfofile = dynamic_cast<NonPipeProfile*>(pMemb->getEndProfile().GetPointee());
					memberVal["07_eNonPipeHigh"] = pEndNonPipeProfofile->getHigh();
					memberVal["08_eNonPipeWide"] = pEndNonPipeProfofile->getUIWide();
					memberVal["11_eUDThickness"] = pEndNonPipeProfofile->getUDThickness();
					memberVal["12_eSideThickness"] = pEndNonPipeProfofile->getThk();
				}
			}
// 			if (!pMemb->getStartConnect().isNull())
// 			{
// 				if (pMemb->getStartConnect()->getCnnType() == Connect::Type::Flange)
// 				{
// 					HdlFlangeConnect flangeConnect = dynamic_cast<CircleFlangeConnect*>(pMemb->getStartConnect().GetPointee());
// 					if (!flangeConnect.isNull())
// 						writeFlangeConnectToJson(memberVal["StartConnect"], flangeConnect);
// 				}
// 				else if (pMemb->getStartConnect()->getCnnType() == Connect::Type::StiPlate || pMemb->getStartConnect()->getCnnType() == Connect::Type::TroughPlate || pMemb->getStartConnect()->getCnnType() == Connect::Type::DoublePlate)
// 				{
// 					HdlPlateConnect plateConnect = dynamic_cast<PlateConnect*>(pMemb->getStartConnect().GetPointee());
// 					if (!plateConnect.isNull())
// 						writePlateConnectToJson(memberVal["StartConnect"], plateConnect);
// 				}
// 				else if (pMemb->getStartConnect()->getCnnType() == Connect::Type::SquareFlange)
// 				{
// 					HdlSquareFlangeConnect squareFlangeConnect = dynamic_cast<SquareFlangeConnect*>(pMemb->getStartConnect().GetPointee());
// 					if (!squareFlangeConnect.isNull())
// 						writeSquareFlangeConnectToJson(memberVal["StartConnect"], squareFlangeConnect);
// 				}
// 			}
// 			if (!pMemb->getEndConnect().isNull())
// 			{
// 				if (pMemb->getEndConnect()->getCnnType() == Connect::Type::Flange)
// 				{
// 					HdlFlangeConnect flangeConnect = dynamic_cast<CircleFlangeConnect*>(pMemb->getEndConnect().GetPointee());
// 					if (!flangeConnect.isNull())
// 						writeFlangeConnectToJson(memberVal["EndConnect"], flangeConnect);
// 				}
// 				else if (pMemb->getEndConnect()->getCnnType() == Connect::Type::StiPlate || pMemb->getEndConnect()->getCnnType() == Connect::Type::TroughPlate || pMemb->getEndConnect()->getCnnType() == Connect::Type::DoublePlate)
// 				{
// 					HdlPlateConnect plateConnect = dynamic_cast<PlateConnect*>(pMemb->getEndConnect().GetPointee());
// 					if (!plateConnect.isNull())
// 						writePlateConnectToJson(memberVal["EndConnect"], plateConnect);
// 				}
// 				else if (pMemb->getEndConnect()->getCnnType() == Connect::Type::SquareFlange)
// 				{
// 					HdlSquareFlangeConnect squareFlangeConnect = dynamic_cast<SquareFlangeConnect*>(pMemb->getEndConnect().GetPointee());
// 					if (!squareFlangeConnect.isNull())
// 						writeSquareFlangeConnectToJson(memberVal["EndConnect"], squareFlangeConnect);
// 				}
// 			}

			jsonMembers.append(memberVal);
		}
	}


	void JsonFormatIO::read(const char* username /*= 0*/)
	{
		string filename = string(ModelDateBase::Instance()->getFilename()) + ".Pole";
 		if (username!=NULL)
 			filename = username;
 
 		std::ifstream  fin(filename.c_str(), std::ios::in);
 		if (fin.fail())
 			throw std::exception("模型文件打不开");
 
 		Json::Reader reader;
 		Json::Value root;
 		if (!reader.parse(fin, root, false))
 			throw std::exception("数据格式错误");
 		fin.close();
 
 		this->readFamily(root);
		this->readNGPGroup(root);
 		this->readPointArray(root);
 		this->readMemberArray(root);
		this->readWinCond(root);
		this->readNodeLoad(root);
		this->readLodRpnInfos(root);
		this->readDeflection(root);
		this->readDesignInfos(root);

		this->readRapidDesign(root);
		this->readMainPoleRapid(root);
		this->readTubularHoriArray(root);
		this->readLinearCroArmArray(root);

	}

	void JsonFormatIO::write(const char* username /*= 0*/)
	{
		string filename = string(ModelDateBase::Instance()->getFilename()) + PoleIO::suffix;
		if (username != NULL)
			filename = username;

		std::ofstream fout(filename.c_str());
		if (!fout)
			throw exception("无法写入模型文件,该文件锁定");

		Json::Value root(Json::objectValue);
		root["00 Company Name:"] = "HENGJU SOFTWARE";
		root["00 Program:"] = "SmartPole";
		root["00 Version:"] = 1.01;
		this->writeFamily(root);
		this->writeNGPGroup(root);
		this->writePointArray(root);
		this->writeMemberArray(root);
		this->writeDesignInfos(root);

		this->writeWinCond(root);
		this->writeNodeLoad(root);
		this->writeLodRpnInfos(root);
		this->writeDeflection(root);

		this->writeRapidDesign(root);
		this->writeMainPoleRapid(root);
		this->writeTubularHoriArray(root);
		this->writeLinearCroArmArray(root);

		Json::StyledStreamWriter styled_writer;;
		styled_writer.write(fout, root);
	}

	// @ function:  从本地文件读到内核中second--- by raoyi  2020/03/25 18:27
	void JsonFormatIO::readFamily(Json::Value& root)
	{
		HdFamilyManager pFamilyManager = new FamilyManager();
		ModelDateBase::Instance()->setFamilyManager(pFamilyManager);
		const Json::Value& jsonNodeManager = root["01_FamilyManager"];
		if (jsonNodeManager.isNull())
			return;
		int arraycount = jsonNodeManager.size();
		for (int iloop = 0; iloop < arraycount; ++iloop)
		{
			const Json::Value& jsonComp = jsonNodeManager[iloop];
			//Symetry::Type Isym = Symetry::Tansfer(priPoint["Symmetry"].asInt());
			HandleComponent pComp = new Component(
										jsonComp["1_MaxNumber"].asInt(),
										jsonComp["2_ConnectIndex"].asInt());
			pFamilyManager->add(pComp);
		}
	}

	// @ function:从内核写入json文件中first  --- by raoyi  2020/03/25 18:27
	void JsonFormatIO::writeFamily(Json::Value& root)
	{
		HdFamilyManager pFamilyManager = ModelDateBase::Instance()->getFamilyManager();
		if (pFamilyManager.isNull() || pFamilyManager->getTotalCount() < 1)
			return;
		Json::Value jsonNodeManager(Json::arrayValue);
		for (size_t iloop = 0; iloop < pFamilyManager->getTotalCount(); iloop++)
		{
			Json::Value partVal(Json::objectValue);
			HandleComponent pComponent  = pFamilyManager->at(iloop);
			partVal["1_MaxNumber"] = pComponent ->getMaxNumber();
			partVal["2_ConnectIndex"] = pComponent->getUserDefineCnnIndex();
			jsonNodeManager.append(partVal);
		};
		root["01_FamilyManager"] = jsonNodeManager;
	}

	// @ function:  --- by raoyi  2020/03/25 21:14
	void JsonFormatIO::readNGPGroup(Json::Value& root)
	{
		HdNGPManager pNGPManager = new NGPManager();
		ModelDateBase::Instance()->setNGPManager(pNGPManager);
		const Json::Value& jsonNGPManager = root["01_NGPGroup"];
		if (jsonNGPManager.isNull())
			return;
		int iStartCaseIndex = jsonNGPManager["1_StartCaseIndex"].asInt();
		int iEndCaseIndex = jsonNGPManager["2_EndCaseIndex"].asInt();
		int iStartLegIndex = jsonNGPManager["3_StartLegIndex"].asInt();
		int iEndLegIndex = jsonNGPManager["4_EndLegIndex"].asInt();
		pNGPManager->setStartCaseNum(iStartCaseIndex);
		pNGPManager->setEndCaseNum(iEndCaseIndex);
		pNGPManager->setStartCaseNum(iStartLegIndex);
		pNGPManager->setEndCaseNum(iEndLegIndex);

		const Json::Value& AllNGPInfo = jsonNGPManager["5_NGPList"];
		if (AllNGPInfo.isNull())
			return;
		int arraycount = AllNGPInfo.size();
		for (int iloop = 0; iloop < arraycount; ++iloop)
		{
			const Json::Value& curNGP = AllNGPInfo[iloop];
			HandleNGP pNGP = new NGP_Info(curNGP["1_StartLeg"].asInt(), 
				                          curNGP["2_EndLeg"].asInt(), 
										  curNGP["3_StartCase"].asInt(), 
										  curNGP["4_EndCase"].asInt());
			pNGPManager->push_back(pNGP);
		}
	}

	// @ function:  --- by raoyi  2020/03/25 21:14
	void JsonFormatIO::writeNGPGroup(Json::Value& root)
	{
		HdNGPManager pNGPManager = ModelDateBase::Instance()->getNGPManager();
		if (pNGPManager.isNull())
			return;
		Json::Value jsonNGPManager(Json::objectValue);
		jsonNGPManager["1_StartCaseIndex"] = pNGPManager->getStartCaseNum();
		jsonNGPManager["2_EndCaseIndex"] = pNGPManager->getEndCaseNum();
		jsonNGPManager["3_StartLegIndex"] = pNGPManager->getStartLegNum();
		jsonNGPManager["4_EndLegIndex"] = pNGPManager->getEndLegNum();
		Json::Value AllNGPInfo(Json::arrayValue);
		for (size_t iloop = 0; iloop<pNGPManager->size(); ++iloop)
		{
			HandleNGP pNGP = pNGPManager->getAt(iloop);
			Json::Value curNGP(Json::objectValue);
			curNGP["1_StartLeg"] = pNGP->getStartLegIndex();
			curNGP["2_EndLeg"] = pNGP->getEndLegIndex();
			curNGP["3_StartCase"] = pNGP->getStartCaseIndex();
			curNGP["4_EndCase"] = pNGP->getEndCaseIndex();
			AllNGPInfo.append(curNGP);
		}
		if (pNGPManager->size()>0)
			jsonNGPManager["5_NGPList"] = AllNGPInfo;
		root["01_NGPGroup"] = jsonNGPManager;
	}

	// @ function:  --- by raoyi  2020/03/25 21:16
	void JsonFormatIO::readWinCond(Json::Value& root)
	{
// 		HdWinCondManager pWinConds = new WinCondManager();
// 		ModelDateBase::Instance()->setWinCondArray(pWinConds);
// 		const Json::Value& jsonWinConds = root["04_CaseList"];
// 		if (jsonWinConds.isNull())
// 			return;
// 		int arraycount = jsonWinConds.size();
// 		for (int iloop = 0; iloop < arraycount; ++iloop)
// 		{
// 			const Json::Value& WinCondVal = jsonWinConds[iloop];
// 			HDWinCond pWinCond = new CWinConditon(WinCondVal["1_Speed"].asDouble(), 
// 				                                  WinCondVal["2_WinAmplifFac"].asDouble(), 
// 												  WinCondVal["3_Angle"].asDouble(),
// 												  WinCondVal["4_ImportantFac"].asDouble(),
// 												  WinCondVal["5_WeightAmplFac"].asDouble(),
// 												  0,
// 												  WinCondVal["6_Temperature"].asDouble());
// 			string strnote = WinCondVal["7_Note"].asString();
// 			pWinCond->setNote(strnote.c_str());
// 			pWinConds->push_back(pWinCond);
// 		};
	}

	// @ function:  --- by raoyi  2020/03/25 21:17
	void JsonFormatIO::writeWinCond(Json::Value& root)
	{
// 		HdWinCondManager pWinConds = ModelDateBase::Instance()->getWinCondArray();
// 		if (pWinConds.isNull() || pWinConds->empty())
// 			return;
// 		Json::Value jsonWinConds(Json::arrayValue);
// 		for (size_t iloop = 0; iloop < pWinConds->size(); iloop++)
// 		{
// 			HDWinCond pWinCond = pWinConds->at(iloop);
// 			Json::Value WinCondVal(Json::objectValue);
// 			WinCondVal["1_Speed"] = pWinCond->getSpeed();
// 			WinCondVal["2_WinAmplifFac"] = pWinCond->getWinAmplifFac();
// 			WinCondVal["3_Angle"] = pWinCond->getAngle();
// 			WinCondVal["4_ImportantFac"] = pWinCond->getImpFac();
// 			WinCondVal["5_WeightAmplFac"] = pWinCond->getWeightAmplFac();
// 			WinCondVal["6_Temperature"] = pWinCond->getTemperature();
// 			WinCondVal["7_Note"] = string(pWinCond->getNote());
// 			jsonWinConds.append(WinCondVal);
// 		};
// 		root["04_CaseList"] = jsonWinConds;
	}

	// @ function:  --- by raoyi  2020/03/25 21:21
	void JsonFormatIO::readNodeLoad(Json::Value& root)
	{
// 		HdNodeLoadManager pNodeLoads = new NodeLoadManager();
// 		ModelDateBase::Instance()->setWireLoadManager(pNodeLoads);
// 		const Json::Value& jsonNodeLoads = root["05_NodeLoads"];
// 		if (jsonNodeLoads.isNull())
// 			return;
// 		int arraycount = jsonNodeLoads.size();
// 		for (int iloop = 0; iloop < arraycount; ++iloop)
// 		{
// 			const Json::Value& jsonNodeLoad = jsonNodeLoads[iloop];
// 			const Json::Value& jsonLoadVectorVal = jsonNodeLoad["2_LoadVector"];
// 			int iForceTag = jsonNodeLoad["1_NodeNumber"].asInt();
// 			int iCaseCount = jsonLoadVectorVal.size();
// 			HandleLoadVec pNodeLoad = new NodeLoadArray(iForceTag, iCaseCount);
// 			pNodeLoads->push_back(pNodeLoad);
// 			for (int icaseloop = 0; icaseloop < iCaseCount; ++icaseloop)
// 			{
// 				const Json::Value& curVal = jsonLoadVectorVal[icaseloop];
// 				double fx = curVal["1_Fx"].asDouble();
// 				double fy = curVal["2_Fy"].asDouble();
// 				double fz = curVal["3_Fz"].asDouble();
// 				pNodeLoad->setNCaseForce(icaseloop, fx, fy, fz);
// 			}
// 			GPointFlag::GPointType GType = GPointFlag::GPointType(jsonNodeLoad["3_GPointType"].asInt());
// 			pNodeLoad->setHangFlag(GType);
// 		};
	}

	// @ function:  --- by raoyi  2020/03/25 21:21
	void JsonFormatIO::writeNodeLoad(Json::Value& root)
	{
// 		HdNodeLoadManager pNodeLoads = ModelDateBase::Instance()->getWireLoadManager();
// 		if (pNodeLoads.isNull() || pNodeLoads->size() < 1)
// 			return;
// 		Json::Value jsonNodeLoads(Json::arrayValue);
// 		for (size_t iloop = 0; iloop < pNodeLoads->size(); iloop++)
// 		{
// 			HandleLoadVec pNodeLoad = pNodeLoads->at(iloop);
// 			Json::Value jsonNodeLoad(Json::objectValue);
// 			jsonNodeLoad["1_NodeNumber"] = pNodeLoad->ForceTag();
// 			Json::Value jsonLoadVectorVal(Json::arrayValue);
// 			for (int caseloop = 0; caseloop < pNodeLoad->iGetCaseSize(); caseloop++)
// 			{
// 				NodeLoad curLoad = pNodeLoad->getNCaseForce(caseloop);
// 				Json::Value curVal(Json::objectValue);
// 				curVal["1_Fx"] = curLoad.Fx();
// 				curVal["2_Fy"] = curLoad.Fy();
// 				curVal["3_Fz"] = curLoad.Fz();
// 				jsonLoadVectorVal.append(curVal);
// 			};
// 			jsonNodeLoad["2_LoadVector"] = jsonLoadVectorVal;
// 			jsonNodeLoad["3_GPointType"] = pNodeLoad->getHangFlag();
// 			jsonNodeLoads.append(jsonNodeLoad);
// 		};
// 		root["05_NodeLoads"] = jsonNodeLoads;
	}

	// @ function:  --- by raoyi  2020/03/25 21:26
	void JsonFormatIO::readLodRpnInfos(Json::Value& root)
	{
// 		HdLodRpnManager hdloadRpnManager = new CLoadRepartitionManager();
// 		ModelDateBase::Instance()->setLoadRpnManager(hdloadRpnManager);
// 		const Json::Value& jsonhdloadRpn = root["06_LoadRepartition"];
// 		if (jsonhdloadRpn.isNull())
// 			return;
// 
// 		int arraycount = jsonhdloadRpn.size();
// 		for (int iloop = 0; iloop < arraycount; ++iloop)
// 		{
// 			const Json::Value& curVal = jsonhdloadRpn[iloop];
// 			int iForceTag = curVal["1_ForcePoint"].asInt();
// 			string InsulatorTypeStr = curVal["2_InsulatorType"].asString();
// 			if (InsulatorTypeStr == "NormalIInsulator")
// 			{
// 				const Json::Value& DistributionInfos = curVal["DistributionPoint"];
// 				int countinfo = DistributionInfos.size();
// 				HdNormalRepartition pLR = new CNormalLoadRepartition(iForceTag, countinfo);
// 				for (int iTmpLoop = 0; iTmpLoop < countinfo; ++iTmpLoop)
// 				{
// 					const Json::Value& splitInfo = DistributionInfos[iTmpLoop];
// 					HandleLoadRatioInfo pLRI = new LoadRatioInfo(splitInfo["HangPoint"].asInt(), splitInfo["Ratio"].asDouble());
// 					pLR->push_back(pLRI);
// 				}
// 				hdloadRpnManager->push_back(pLR.GetPointee());
// 			}
// 			else if (InsulatorTypeStr == "MergeLoad")
// 			{
// 				const Json::Value& DistributionInfos = curVal["DistributionPoint"];
// 				int countinfo = DistributionInfos.size();
// 				IntVector vtlaVec;
// 				for (int iTmpLoop = 0; iTmpLoop < countinfo; ++iTmpLoop)
// 					vtlaVec.push_back(DistributionInfos[iTmpLoop].asInt());
// 
// 				HandleLoadRepartiton pRpt = new MergeLoadRepartition(iForceTag, vtlaVec);
// 				hdloadRpnManager->push_back(pRpt.GetPointee());
// 			}
// 			else if (InsulatorTypeStr == "XYZRatioRpt")
// 			{
// 				const Json::Value& DistributionInfos = curVal["DistributionPoint"];
// 				int countinfo = DistributionInfos.size();
// 				HdXYZRatioRepartition pLR = new XYZRatioLoadRepartition(iForceTag, countinfo);
// 				for (int iTmpLoop = 0; iTmpLoop < countinfo; ++iTmpLoop)
// 				{
// 					const Json::Value& splitInfo = DistributionInfos[iTmpLoop];
// 					HandleXYZLoadRatioInfo pLRI = new XYZLoadRatioInfo(
// 						splitInfo["TagNum"].asInt(),
// 						splitInfo["XRatio"].asDouble(),
// 						splitInfo["YRatio"].asDouble(),
// 						splitInfo["ZRatio"].asDouble()
// 						);
// 					pLR->push_back(pLRI);
// 				}
// 				hdloadRpnManager->push_back(pLR.GetPointee());
// 			}
// 			else
// 			{
// 				HdVInsRepartition pRpt = new VInsulatorRepartition(iForceTag, curVal["LeftHangingPoint"].asInt(), curVal["RightHangingPoint"].asInt());
// 				hdloadRpnManager->push_back(pRpt.GetPointee());
// 			}
// 		}
	}

	// @ function:  --- by raoyi  2020/03/25 21:27
	void JsonFormatIO::writeLodRpnInfos(Json::Value& root)
	{
// 		HdLodRpnManager hdloadRpnManager = ModelDateBase::Instance()->getLoadRpnManager();
// 		if (hdloadRpnManager.isNull() || hdloadRpnManager->size() < 1)
// 			return;
// 		//荷载分配信息
// 		Json::Value jsonhdloadRpn(Json::arrayValue);
// 		for (size_t iloop = 0; iloop < hdloadRpnManager->size(); iloop++)
// 		{
// 			HandleLoadRepartiton LR = hdloadRpnManager->at(iloop);
// 			if (LR.isNull())
// 				continue;
// 			else if (LR->GetObjType() == PoleObject::VInsulatorRp)
// 			{
// 				VInsulatorRepartition* vLR = static_cast<VInsulatorRepartition*>(LR.GetPointee());
// 				Json::Value curVal(Json::objectValue);
// 				curVal["2_InsulatorType"] = "VInsulator";//表示V串
// 				curVal["1_ForcePoint"] = vLR->ForceTag();
// 				curVal["LeftHangingPoint"] = vLR->LeftHangingPoint();
// 				curVal["RightHangingPoint"] = vLR->RightHangingPoint();
// 				jsonhdloadRpn.append(curVal);
// 			}
// 			else if (LR->GetObjType() == PoleObject::TriInsulator)
// 			{
// 				TriInsulatorRepartition* TriLR = static_cast<TriInsulatorRepartition*>(LR.GetPointee());
// 				Json::Value curVal(Json::objectValue);
// 				curVal["2_InsulatorType"] = "TriInsulator";//表示三角架
// 				curVal["1_ForcePoint"] = TriLR->ForceTag();
// 				curVal["LeftHangingPoint"] = TriLR->LeftHangingPoint();
// 				curVal["RightHangingPoint"] = TriLR->RightHangingPoint();
// 				jsonhdloadRpn.append(curVal);
// 			}
// 			else if (LR->GetObjType() == PoleObject::NORMALLoadRp)
// 			{
// 				CNormalLoadRepartition* NLR = static_cast<CNormalLoadRepartition*>(LR.GetPointee());
// 				Json::Value curVal(Json::objectValue);
// 				curVal["2_InsulatorType"] = "NormalIInsulator";//表示三角架
// 				curVal["1_ForcePoint"] = NLR->ForceTag();
// 				Json::Value DistributionInfos(Json::arrayValue);
// 				for (size_t iloop2 = 0; iloop2 < NLR->size(); iloop2++)
// 				{
// 					HandleLoadRatioInfo LRInfo = NLR->at(iloop2);
// 					Json::Value splitInfo(Json::objectValue);
// 					splitInfo["HangPoint"] = LRInfo->ForceTag();
// 					splitInfo["Ratio"] = LRInfo->GetInfo();
// 					DistributionInfos.append(splitInfo);
// 				}
// 				curVal["DistributionPoint"] = DistributionInfos;
// 				jsonhdloadRpn.append(curVal);
// 			}
// 			else if (LR->GetObjType() == PoleObject::XYZRatioRpt)
// 			{
// 				XYZRatioLoadRepartition* NLR = static_cast<XYZRatioLoadRepartition*>(LR.GetPointee());
// 				Json::Value curVal(Json::objectValue);
// 				curVal["2_InsulatorType"] = "XYZRatioRpt";//表示三角架
// 				curVal["1_ForcePoint"] = NLR->ForceTag();
// 				Json::Value DistributionInfos(Json::arrayValue);
// 				for (size_t iloop2 = 0; iloop2 < NLR->size(); iloop2++)
// 				{
// 					HandleXYZLoadRatioInfo LRInfo = NLR->at(iloop2);
// 					Json::Value splitInfo(Json::objectValue);
// 					splitInfo["TagNum"] = LRInfo->ForceTag();
// 					splitInfo["XRatio"] = LRInfo->RatioX();
// 					splitInfo["YRatio"] = LRInfo->RatioY();
// 					splitInfo["ZRatio"] = LRInfo->RatioZ();
// 					DistributionInfos.append(splitInfo);
// 				}
// 				curVal["DistributionPoint"] = DistributionInfos;
// 				jsonhdloadRpn.append(curVal);
// 			}
// 			else if (LR->GetObjType() == PoleObject::MergeLoad)
// 			{
// 				MergeLoadRepartition* NLR = static_cast<MergeLoadRepartition*>(LR.GetPointee());
// 				Json::Value curVal(Json::objectValue);
// 				curVal["2_InsulatorType"] = "MergeLoad";//表示三角架
// 				curVal["1_ForcePoint"] = NLR->ForceTag();
// 				Json::Value DistributionInfos(Json::arrayValue);
// 				for (int iloop2 = 0; iloop2 < NLR->MergeNodeCount(); iloop2++)
// 					DistributionInfos.append(NLR->LoadNumAt(iloop2));
// 				curVal["DistributionPoint"] = DistributionInfos;
// 				jsonhdloadRpn.append(curVal);
// 			}
// 		}
// 		root["06_LoadRepartition"] = jsonhdloadRpn;
	}

	// @ function:  --- by raoyi  2020/05/18 17:02
	void JsonFormatIO::readDeflection(Json::Value& root)
	{
// 		const Json::Value& jsonNodeManager = root["06_Deflection"];
// 		jsonNodeManager["1_Deflection"].asDouble();
// 		jsonNodeManager["2_AmplFac"].asDouble();
	}

	// @ function:  --- by raoyi  2020/05/18 17:02
	void JsonFormatIO::writeDeflection(Json::Value& root)
	{
// 		Json::Value partVal(Json::objectValue);
// 		partVal["1_Deflection"] = DisplacementColorPolicy::WarningVal;
// 		partVal["2_AmplFac"] = DisplacementColorPolicy::AmplFac;
// 		root["06_Deflection"] = partVal;
	}

	// @ function: 法兰  --- by lsq 2020/4/24 15:42
// 	void JsonFormatIO::writeFlangeConnectToJson(Json::Value& root, HdlFlangeConnect FlangeConnectDB)
// 	{
// 		Json::Value item;
// 
// 		if (FlangeConnectDB->getBoltRank().isNull())
// 			item["BoltRank"] = "4.8";
// 		else
// 			item["BoltRank"] = FlangeConnectDB->getBoltRank()->Rank();
// 
// 		if (FlangeConnectDB->getBoltGeometry().isNull())
// 			item["BoltGeometry"] = "M16";
// 		else
// 			item["BoltGeometry"] = FlangeConnectDB->getBoltGeometry()->getName();
// 
// 		item["rotPos"] = FlangeConnectDB->getRotPos();
// 		item["MinM"] = FlangeConnectDB->getMinM();
// 		item["MaxM"] = FlangeConnectDB->getMaxM();
// 		item["T"] = FlangeConnectDB->getT();
// 		item["Tau"] = FlangeConnectDB->getTau();
// 		item["TWell"] = FlangeConnectDB->getTWell();
// 		item["BoltNum"] = FlangeConnectDB->getBoltNum();
// 
// 		writeStiPlateDBToJson(item["StiPlateDB"], FlangeConnectDB->getStiPlateDB());
// 		writeFlangePlateDBToJson(item["FlangePlateDB"], FlangeConnectDB->getFlangePlateDB());
// 		root.append(item);
// 	}
// 	// @ function:   --- by lsq 2020/4/24 16:13
// 	void JsonFormatIO::readFlangeConnectToJson(const Json::Value& root, HdlFlangeConnect FlangeConnect)
// 	{
// 		HandleBoltGeometry boltGeometry = nullptr;
// 		HandleBoltRank boltRank = nullptr;
// 		if (root["BoltGeometry"].isString())
// 			boltGeometry = BoltDataBase::Instance()->Find(root["BoltGeometry"].asCString());
// 		if (root["BoltRank"].asCString())
// 			boltRank = BoltDataBase::Instance()->FindBoltRank(root["BoltRank"].asCString());
// 
// 
// 		FlangeConnect->setBoltRank(boltRank);
// 		FlangeConnect->setBoltGeometry(boltGeometry);
// 		FlangeConnect->setMinM(root["MinM"].asDouble());
// 		FlangeConnect->setMaxM(root["MaxM"].asDouble());
// 		FlangeConnect->setT(root["T"].asDouble());
// 		FlangeConnect->setTau(root["Tau"].asDouble());
// 		FlangeConnect->setTWell(root["TWell"].asDouble());
// 		FlangeConnect->setRotPos(root["rotPos"].asDouble());
// 		FlangeConnect->setBoltNum(root["BoltNum"].asDouble());
// 
// 		int temp = 0;
// 		HdlStiPlateDB stiplateDB = new StiffeningPlate();
// 		readStiPlateDBToJson(root["StiPlateDB"][temp], stiplateDB);
// 		FlangeConnect->setStiPlateDB(stiplateDB);
// 
// 		HdlFlangePlateDB flangeplateDB = new FlangePlate();
// 		readFlangePlateDBToJson(root["FlangePlateDB"][temp], flangeplateDB);
// 		FlangeConnect->setFlangePlateDB(flangeplateDB);
// 
// 	}
// 	// @ function:   --- by lsq 2020/4/24 15:57
// 	void JsonFormatIO::writeFlangePlateDBToJson(Json::Value& root, HdlFlangePlateDB FlangePlateDB)
// 	{
// 		Json::Value item;
// 
// 		if (FlangePlateDB->getFlangeType().isNull())
// 			item["FlangeMarterial"] = "Q235";
// 		else
// 			item["FlangeMarterial"] = FlangePlateDB->getFlangeType()->getName();
// 
// 		item["FlangeT"] = FlangePlateDB->getFlangeT();
// 		item["FlangeOutDia"] = FlangePlateDB->getFlangeOutDia();
// 		item["BoltCoreDia"] = FlangePlateDB->getBoltCoreDia();
// 		item["Dia"] = FlangePlateDB->getDia();
// 
// 		root.append(item);
// 	}
// 	// @ function:   --- by lsq 2020/4/24 16:17
// 	void JsonFormatIO::readFlangePlateDBToJson(const Json::Value& root, HdlFlangePlateDB FlangePlateDB)
// 	{
// 		HandleMaterial pMat;
// 		if (root["FlangeMarterial"].isString())
// 			pMat = MaterialDataBase::Instance()->Find(root["FlangeMarterial"].asCString());
// 		FlangePlateDB->setFlangeType(pMat);
// 
// 		FlangePlateDB->setFlangeT(root["FlangeT"].asDouble());
// 		FlangePlateDB->setFlangeOutDia(root["FlangeOutDia"].asDouble());
// 		FlangePlateDB->setBoltCoreDia(root["BoltCoreDia"].asDouble());
// 		FlangePlateDB->setDia(root["Dia"].asDouble());
// 
// 	}
// 
// 	// @ function:   --- by lsq 2020/4/24 15:58
// 	void JsonFormatIO::writeStiPlateDBToJson(Json::Value& root, HdlStiPlateDB StiPlateDB)
// 	{
// 		Json::Value item;
// 
// 		if (StiPlateDB->getStiPlateType().isNull())
// 			item["StiPlateMarterial"] = "Q235";
// 		else
// 			item["StiPlateMarterial"] = StiPlateDB->getStiPlateType()->getName();
// 
// 		item["StiPlateT"] = StiPlateDB->getStiPlateT();
// 		item["StiPlateH"] = StiPlateDB->getStiPlateH();
// 		item["StiPlateW"] = StiPlateDB->getStiPlateW();
// // 		item["S1"] = StiPlateDB->getS1();
// // 		item["S2"] = StiPlateDB->getS2();
// 		root.append(item);
// 	}
// 	// @ function:   --- by lsq 2020/4/24 16:20
// 	void JsonFormatIO::readStiPlateDBToJson(const Json::Value& root, HdlStiPlateDB StiPlateDB)
// 	{
// 		HandleMaterial pMat;
// 		if (root["StiPlateMarterial"].isString())
// 			pMat = MaterialDataBase::Instance()->Find(root["StiPlateMarterial"].asCString());
// 		StiPlateDB->setStiPlateType(pMat);
// 
// 		StiPlateDB->setStiPlateT(root["StiPlateT"].asDouble());
// 		StiPlateDB->setStiPlateH(root["StiPlateH"].asDouble());
// 		StiPlateDB->setStiPlateW(root["StiPlateW"].asDouble());
// // 		StiPlateDB->setS1(root["S1"].asDouble());
// // 		StiPlateDB->setS2(root["S2"].asDouble());
// 	}
// 
// 	// @ function:   --- by lsq 2020/4/24 17:30
// 	void JsonFormatIO::writePlateConnectToJson(Json::Value& root, HdlPlateConnect PlateConnectDB)
// 	{
// 		Json::Value item;
// 		item["WellDis"] = PlateConnectDB->getWellDis();
// 		item["WellH"] = PlateConnectDB->getWellH();
// 		item["RootL"] = PlateConnectDB->getRootL();
// 		item["InStileT"] = PlateConnectDB->getInStileT();
// 		item["WellT"] = PlateConnectDB->getWellT();
// 		item["CrossWellT"] = PlateConnectDB->getCrossWellT();
// 		item["OutStileT"] = PlateConnectDB->getOutStileT();
// 		item["StileT"] = PlateConnectDB->getStileT();
// 		item["StileH"] = PlateConnectDB->getStileH();
// 		item["BoltColumn"] = PlateConnectDB->getBoltColumn();
// 		item["BoltRow"] = PlateConnectDB->getBoltRow();
// 
// 		if (PlateConnectDB->getSideType().isNull())
// 			item["SideType"] = "Q235";
// 		else
// 			item["SideType"] = PlateConnectDB->getSideType()->getName();
// 
// 		if (PlateConnectDB->getSteelType().isNull())
// 			item["SteelType"] = "Q235";
// 		else
// 			item["SteelType"] = PlateConnectDB->getSteelType()->getName();
// 
// 		if (PlateConnectDB->getBoltRank().isNull())
// 			item["BoltRank"] = "4.8";
// 		else
// 			item["BoltRank"] = PlateConnectDB->getBoltRank()->Rank();
// 
// 		if (PlateConnectDB->getBoltGeometry().isNull())
// 			item["BoltGeometry"] = "M16";
// 		else
// 			item["BoltGeometry"] = PlateConnectDB->getBoltGeometry()->getName();
// 
// 		root.append(item);
// 	}
// 	// @ function:   --- by lsq 2020/4/24 17:51
// 	void JsonFormatIO::readPlateConnectToJson(const Json::Value& root, HdlPlateConnect PlateConnectDB)
// 	{
// 		PlateConnectDB->setWellDis(root["WellDis"].asDouble());
// 		PlateConnectDB->setWellH(root["WellH"].asDouble());
// 		PlateConnectDB->setRootL(root["RootL"].asDouble());
// 		PlateConnectDB->setInStileT(root["InStileT"].asDouble());
// 		PlateConnectDB->setWellT(root["WellT"].asDouble());
// 		PlateConnectDB->setCrossWellT(root["CrossWellT"].asDouble());
// 		PlateConnectDB->setOutStileT(root["OutStileT"].asDouble());
// 		PlateConnectDB->setBoltColumn(root["BoltColumn"].asInt());
// 		PlateConnectDB->setBoltRow(root["BoltRow"].asInt());
// 		PlateConnectDB->setStileT(root["StileT"].asDouble());
// 		PlateConnectDB->setStileH(root["StileH"].asDouble());
// 	
// 		HandleMaterial pSideMat;
// 		if (root["SideType"].isString())
// 			pSideMat = MaterialDataBase::Instance()->Find(root["SideType"].asCString());
// 		PlateConnectDB->setSideType(pSideMat);
// 
// 		HandleMaterial pMat;
// 		if (root["SteelType"].isString())
// 			pMat = MaterialDataBase::Instance()->Find(root["SteelType"].asCString());
// 		PlateConnectDB->setSteelType(pMat);
// 
// 		HandleBoltGeometry boltGeometry = nullptr;
// 		if (root["BoltGeometry"].isString())
// 			boltGeometry = BoltDataBase::Instance()->Find(root["BoltGeometry"].asCString());
// 
// 		HandleBoltRank boltRank = nullptr;
// 		if (root["BoltRank"].asCString())
// 			boltRank = BoltDataBase::Instance()->FindBoltRank(root["BoltRank"].asCString());
// 
// 		PlateConnectDB->setBoltRank(boltRank);
// 		PlateConnectDB->setBoltGeometry(boltGeometry);
// 	}
// 	// @ function:   --- by lsq 2020/4/24 18:01
// 	void JsonFormatIO::writeSquareFlangeConnectToJson(Json::Value& root, HdlSquareFlangeConnect SquFlangeConnectDB)
// 	{
// 		Json::Value item;
// 		item["B"] = SquFlangeConnectDB->getB();
// 		item["Tb"] = SquFlangeConnectDB->getTb();
// 		item["H"] = SquFlangeConnectDB->getH();
// 		item["Th"] = SquFlangeConnectDB->getTh();
// 
// 		item["MultBoltPit"] = SquFlangeConnectDB->getMultBoltPit();
// 		item["MultBoltDisEdge"] = SquFlangeConnectDB->getMultBoltDisEdge();
// 		item["MultBoltDisFlange"] = SquFlangeConnectDB->getMultBoltDisFlange();
// 		item["L"] = SquFlangeConnectDB->getL();
// 		item["Dis"] = SquFlangeConnectDB->getDis();
// 
// 		writeStiPlateDBToJson(item["StiPlateDB"], SquFlangeConnectDB->getStiPlateDB());
// 		writeFlangePlateDBToJson(item["FlangePlateDB"], SquFlangeConnectDB->getFlangePlateDB());
// 
// 		if (SquFlangeConnectDB->getBoltRank().isNull())
// 			item["BoltRank"] = "4.8";
// 		else
// 			item["BoltRank"] = SquFlangeConnectDB->getBoltRank()->Rank();
// 
// 		if (SquFlangeConnectDB->getBoltGeometry().isNull())
// 			item["BoltGeometry"] = "M16";
// 		else
// 			item["BoltGeometry"] = SquFlangeConnectDB->getBoltGeometry()->getName();
// 
// 		root.append(item);
// 	}
// 	// @ function:   --- by lsq 2020/4/24 18:08
// 	void JsonFormatIO::readSquareFlangeConnectToJson(const Json::Value& root, HdlSquareFlangeConnect SquFlangeConnect)
// 	{
// 		SquFlangeConnect->setB(root["B"].asDouble());
// 		SquFlangeConnect->setTb(root["Tb"].asDouble());
// 		SquFlangeConnect->setH(root["H"].asDouble());
// 		SquFlangeConnect->setTh(root["Th"].asDouble());
// 
// 		SquFlangeConnect->setMultBoltPit(root["MultBoltPit"].asDouble());
// 		SquFlangeConnect->setMultBoltDisEdge(root["MultBoltDisEdge"].asDouble());
// 		SquFlangeConnect->setMultBoltDisFlange(root["MultBoltDisFlange"].asDouble());
// 		SquFlangeConnect->setL(root["L"].asDouble());
// 		SquFlangeConnect->setDis(root["Dis"].asDouble());
// 
// 		int temp = 0;
// 		HdlStiPlateDB stiplateDB = new StiffeningPlate();
// 		readStiPlateDBToJson(root["StiPlateDB"][temp], stiplateDB);
// 		SquFlangeConnect->setStiPlateDB(stiplateDB);
// 
// 		HdlFlangePlateDB flangeplateDB = new FlangePlate();
// 		readFlangePlateDBToJson(root["FlangePlateDB"][temp], flangeplateDB);
// 		SquFlangeConnect->setFlangePlateDB(flangeplateDB);
// 
// 		HandleBoltGeometry boltGeometry = nullptr;
// 		if (root["BoltGeometry"].isString())
// 			boltGeometry = BoltDataBase::Instance()->Find(root["BoltGeometry"].asCString());
// 
// 		HandleBoltRank boltRank = nullptr;
// 		if (root["BoltRank"].asCString())
// 			boltRank = BoltDataBase::Instance()->FindBoltRank(root["BoltRank"].asCString());
// 
// 
// 		SquFlangeConnect->setBoltRank(boltRank);
// 		SquFlangeConnect->setBoltGeometry(boltGeometry);
// 
// 	}

	// @ function:  --- by raoyi  2020/05/11 16:07
	void JsonFormatIO::writeMainPoleRapid(Json::Value& root)
	{
// 		HdlMainPoleRapidArray pMainPoleRapidArray = SinglePoleRapid::Instance()->getMainPoleRapidArray();
// 		//不用判断非空，DB中new过
// 		Json::Value jsonMainPoleRapidArray(Json::arrayValue);
// 		for (int iLoop = 0; iLoop < pMainPoleRapidArray->count(); ++iLoop)
// 		{
// 			HdlMainPoleRapid pMainPoleRapid = pMainPoleRapidArray->getAt(iLoop);
// 			if (pMainPoleRapid.isNull())
// 				continue;
// 			Json::Value JsonMainPole(Json::objectValue);
// 			JsonMainPole["1_Length"] = pMainPoleRapid->getL();
// 			JsonMainPole["2_Thinkness"] =pMainPoleRapid->getT();
// 			if (pMainPoleRapid->getSteelMType().isNull())
// 				JsonMainPole["3_Marterial"] = "Undefined";
// 			else
// 				JsonMainPole["3_Marterial"] = pMainPoleRapid->getSteelMType()->getName();
// 			jsonMainPoleRapidArray.append(JsonMainPole);
// 		}
// 		root["10_MainPoleRapid"] = jsonMainPoleRapidArray;
	}

	// @ function:  --- by raoyi  2020/05/11 16:07
	void JsonFormatIO::readMainPoleRapid(Json::Value& root)
	{
// 		HdlMainPoleRapidArray pMainPoleRapidArray = new MainPoleRapidArray();
// 		SinglePoleRapid::Instance()->setMainPoleRapidArray(pMainPoleRapidArray);
// 		const Json::Value& jsonMainPoleRapid = root["10_MainPoleRapid"];
// 		if (jsonMainPoleRapid.isNull())
// 			return;
// 		int arraycount = jsonMainPoleRapid.size();
// 		for (int iLoop = 0; iLoop < arraycount; ++iLoop)
// 		{
// 			const Json::Value& JsonMainPole = jsonMainPoleRapid[iLoop];
// 			HdlMainPoleRapid pMainPole = new MainPoleRapid(JsonMainPole["1_Length"].asInt(),
// 				JsonMainPole["2_Thinkness"].asDouble());
// 
// 			HandleMaterial pMat;
// 			if (JsonMainPole["3_Marterial"].isString())
// 				pMat = MaterialDataBase::Instance()->Find(JsonMainPole["3_Marterial"].asCString());
// 			pMainPole->setSteelMType(pMat);
// 
// 			pMainPoleRapidArray->add(pMainPole);
// 		}
	}

	// @ function:  --- by raoyi  2020/05/11 17:05
	void JsonFormatIO::writeLinearCroArmArray(Json::Value& root)
	{
// 		HdlLinearCroArmArray pLinearCroArmArray = SinglePoleRapid::Instance()->getLinearCroArmArray();
// 		//不用判断非空，DB中new过
// 		Json::Value jsonLinearCroArmArray(Json::arrayValue);
// 		for (int iLoop = 0; iLoop < pLinearCroArmArray->count(); ++iLoop)
// 		{
// 			HdlLinearCrossArms pLinearCroArm = pLinearCroArmArray->getAt(iLoop);
// 			if (pLinearCroArm.isNull())
// 				continue;
// 			Json::Value JsonVal(Json::objectValue);
// // 			JsonVal["01_SNum"] = pLinearCroArm->getSNum();
// // 			JsonVal["02_ENum"] = pLinearCroArm->getENum();
// 			JsonVal["03_CrossArmProfileType"] = pLinearCroArm->getCrossSection();//在LinearCrossArms的构造函数中已初始化，所以不用判断空
// //			JsonVal["04_ENumVice"] = pLinearCroArm->getENumVice();
// 			JsonVal["05_HangPointHeight"] = pLinearCroArm->getLinePointElev();
// 			JsonVal["06_HorLength"] = pLinearCroArm->getLineLevelLength();
// 			JsonVal["07_UpWardHeight"] = pLinearCroArm->getLineUpwarpingHeight();
// 			JsonVal["08_SpaceAngle"] = pLinearCroArm->getSpatialAngle();
// 			JsonVal["09_RootHigh"] = pLinearCroArm->getRootHigh();
// 			JsonVal["10_RootWide"] = pLinearCroArm->getRootWide();
// 			JsonVal["11_TipHigh"] = pLinearCroArm->getTipHigh();
// 			JsonVal["12_TipWide"] = pLinearCroArm->getTipWide();
// 			JsonVal["13_UDThickness"] = pLinearCroArm->getUDThickness();
// 			JsonVal["14_SidePlateThickness"] = pLinearCroArm->getSidePanelThickness();
// 			JsonVal["15_StiPlateWide"] = pLinearCroArm->getEvenPlateWide();
// 			JsonVal["16_StiPlateThickness"] = pLinearCroArm->getEvenPlateThickness();
// 			JsonVal["17_RootCNNPlateLength"] = pLinearCroArm->getTipEvenPlateLength();
// 			JsonVal["18_RootCNNPlateThickness"] = pLinearCroArm->getTipEvenPlateThickness();
// 			if (pLinearCroArm->getSteelMType().isNull())
// 				JsonVal["19_CroArmMaterial"] = "Q235";
// 			else
// 				JsonVal["19_CroArmMaterial"] = pLinearCroArm->getSteelMType()->getName();
// 			JsonVal["20_GrooveOutWidth"] = pLinearCroArm->getCOutWidth();
// 			JsonVal["21_GrooveRootlength"] = pLinearCroArm->getCRootlength();
// 			JsonVal["22_GrooveCoverPlateThk"] = pLinearCroArm->getCCoverPlateT();
// 			JsonVal["23_GrooveSidePlateThk"] = pLinearCroArm->getCSidePlateT();
// 			JsonVal["24_CrossArmSymmetric"] = pLinearCroArm->getSymmetric();
// 			JsonVal["25_HangingLeft"] = pLinearCroArm->getHangingLeft();
// 			JsonVal["26_HangingRight"] = pLinearCroArm->getHangingRight();
// 			jsonLinearCroArmArray.append(JsonVal);
// 		}
// 		root["11_LinearCroArm"] = jsonLinearCroArmArray;
	}

	// @ function:  --- by raoyi  2020/05/11 17:05
	void JsonFormatIO::readLinearCroArmArray(Json::Value& root)
	{
// 		HdlLinearCroArmArray pLinearCroArmArray = new LinearCrossArmArray();
// 		SinglePoleRapid::Instance()->setLinearCroArmArray(pLinearCroArmArray);
// 
// 		const Json::Value& jsonLinearCroArmArray = root["11_LinearCroArm"];
// 		if (jsonLinearCroArmArray.isNull())
// 			return;
// 		int arraycount = jsonLinearCroArmArray.size();
// 		for (int iLoop = 0; iLoop < arraycount; ++iLoop)
// 		{
// 			const Json::Value& JsonVal = jsonLinearCroArmArray[iLoop];
// 			HdlLinearCrossArms pLinearCroArm = new LinearCrossArms();
// // 			pLinearCroArm->setSNum(JsonVal["01_SNum"].asInt());
// // 			pLinearCroArm->setENum(JsonVal["02_ENum"].asInt());
// 			Profile::Type crossArmProfileType = Profile::Type(JsonVal["03_CrossArmProfileType"].asInt());
// 			pLinearCroArm->setCrossSection(crossArmProfileType);
// //			pLinearCroArm->setENumVice(JsonVal["04_ENumVice"].asInt());
// 			pLinearCroArm->setLinePointElev(JsonVal["05_HangPointHeight"].asDouble());
// 			pLinearCroArm->setLineLevelLength(JsonVal["06_HorLength"].asDouble());
// 			pLinearCroArm->setLineUpwarpingHeight(JsonVal["07_UpWardHeight"].asDouble());
// 			pLinearCroArm->setSpatialAngle(JsonVal["08_SpaceAngle"].asDouble());
// 			pLinearCroArm->setRootHigh(JsonVal["09_RootHigh"].asDouble());
// 			pLinearCroArm->setRootWide(JsonVal["10_RootWide"].asDouble());
// 			pLinearCroArm->setTipHigh(JsonVal["11_TipHigh"].asDouble());
// 			pLinearCroArm->setTipWide(JsonVal["12_TipWide"].asDouble());
// 			pLinearCroArm->setUDThickness(JsonVal["13_UDThickness"].asDouble());
// 			pLinearCroArm->setSidePanelThickness(JsonVal["14_SidePlateThickness"].asDouble());
// 			pLinearCroArm->setEvenPlateWide(JsonVal["15_StiPlateWide"].asDouble());
// 			pLinearCroArm->setEvenPlateThickness(JsonVal["16_StiPlateThickness"].asDouble());
// 			pLinearCroArm->setTipEvenPlateLength(JsonVal["17_RootCNNPlateLength"].asDouble());
// 			pLinearCroArm->setTipEvenPlateThickness(JsonVal["18_RootCNNPlateThickness"].asDouble());
// 			HandleMaterial pMat;
// 			if (JsonVal["19_CroArmMaterial"].isString())
// 				pMat = MaterialDataBase::Instance()->Find(JsonVal["19_CroArmMaterial"].asCString());
// 			pLinearCroArm->setSteelMType(pMat);
// 			pLinearCroArm->setCOutWidth(JsonVal["20_GrooveOutWidth"].asDouble());
// 			pLinearCroArm->setCRootlength(JsonVal["21_GrooveRootlength"].asDouble());
// 			pLinearCroArm->setCCoverPlateT(JsonVal["22_GrooveCoverPlateThk"].asDouble());
// 			pLinearCroArm->setCSidePlateT(JsonVal["23_GrooveSidePlateThk"].asDouble());
// 			BaseRapid::SymmetricType crossArmSymmetric = BaseRapid::SymmetricType(JsonVal["24_CrossArmSymmetric"].asInt());
// 			pLinearCroArm->setSymmetric(crossArmSymmetric);
// 			GPointFlag::GPointType hangingLeft = GPointFlag::GPointType(JsonVal["25_HangingLeft"].asInt());
// 			pLinearCroArm->setHangingLeft(hangingLeft);
// 			GPointFlag::GPointType hangingRight = GPointFlag::GPointType(JsonVal["26_HangingRight"].asInt());
// 			pLinearCroArm->setHangingRight(hangingRight);
// 			pLinearCroArmArray->add(pLinearCroArm);
// 		}
	}


	// @ function:  --- by raoyi  2020/05/11 18:26
	void JsonFormatIO::writeTubularHoriArray(Json::Value& root)
	{
// 		HdlTubularHoriArray pTubularHoriArray = SinglePoleRapid::Instance()->getTubularHoriArray();
// 		//不用判断非空，DB中new过
// 		Json::Value jsonTubularHoriArray(Json::arrayValue);
// 		for (int iLoop = 0; iLoop < pTubularHoriArray->count(); ++iLoop)
// 		{
// 			HdlTubularHori pTubular = pTubularHoriArray->getAt(iLoop);
// 			if (pTubular.isNull())
// 				continue;
// 			Json::Value JsonVal(Json::objectValue);
// // 			JsonVal["01_SNum"] = pTubular->getSNum();
// // 			JsonVal["02_ENum"] = pTubular->getENum();
// 			JsonVal["03_CrossArmProfileType"] = pTubular->getNumEdges();//仅仅用profile类型，不用智能指针，而Material没有枚举，所以用智能指针
// 			if (pTubular->getSteelMType().isNull())
// 				JsonVal["04_CroArmMaterial"] = "Q235";
// 			else
// 				JsonVal["04_CroArmMaterial"] = pTubular->getSteelMType()->getName();
// 			JsonVal["05_HangPointHeight"] = pTubular->getPointElev();
// 			JsonVal["06_HorLength"] = pTubular->getLevelLength();
// 			JsonVal["07_UpWardHeight"] = pTubular->getUpwarpingHeight();
// 			JsonVal["08_SpaceAngle"] = pTubular->getSpatialAngle();
// 			JsonVal["09_CrossArmRootDia"] = pTubular->getCrossArmSD();
// 			JsonVal["10_CrossArmTipDia"] = pTubular->getCrossArmED();
// 			JsonVal["11_WallThickness"] = pTubular->getWallThickness();
// 			JsonVal["12_InStiPlateThickness"] = pTubular->getInStiPlate();
// 			JsonVal["13_SidePlateThickness"] = pTubular->getSidePlateT();
// 			JsonVal["14_ArmLength"] = pTubular->getArmLength();
// 			JsonVal["15_CrossArmSymmetric"] = pTubular->getSymmetric();
// 			JsonVal["16_HangingLeft"] = pTubular->getHangingLeft();
// 			JsonVal["17_HangingRight"] = pTubular->getHangingRight();
// 			jsonTubularHoriArray.append(JsonVal);
// 		}
// 		root["12_TubularHor"] = jsonTubularHoriArray;
	}

	// @ function:  --- by raoyi  2020/05/11 18:26
	void JsonFormatIO::readTubularHoriArray(Json::Value& root)
	{
// 		HdlTubularHoriArray pTubularHoriArray = new TubularHoriArray();
// 		SinglePoleRapid::Instance()->setTubularHoriArray(pTubularHoriArray);
// 
// 		const Json::Value& jsonTubularHoriArray = root["12_TubularHor"];
// 		if (jsonTubularHoriArray.isNull())
// 			return;
// 		int arraycount = jsonTubularHoriArray.size();
// 		for (int iLoop = 0; iLoop < arraycount; ++iLoop)
// 		{
// 			const Json::Value& JsonVal = jsonTubularHoriArray[iLoop];
// 			HdlTubularHori pTubular = new TubularHoriStretcher();
// // 			pTubular->setSNum(JsonVal["01_SNum"].asInt());
// // 			pTubular->setENum(JsonVal["02_ENum"].asInt());
// 			Profile::Type TubularProfileType = Profile::Type(JsonVal["03_CrossArmProfileType"].asInt());
// 			pTubular->setNumEdges(TubularProfileType);
// 			HandleMaterial pMat;
// 			if (JsonVal["04_CroArmMaterial"].isString())
// 				pMat = MaterialDataBase::Instance()->Find(JsonVal["04_CroArmMaterial"].asCString());
// 			pTubular->setSteelMType(pMat);
// 
// 			pTubular->setPointElev(JsonVal["05_HangPointHeight"].asDouble());
// 			pTubular->setLevelLength(JsonVal["06_HorLength"].asDouble());
// 			pTubular->setUpwarpingHeight(JsonVal["07_UpWardHeight"].asDouble());
// 			pTubular->setSpatialAngle(JsonVal["08_SpaceAngle"].asDouble());
// 			pTubular->setCrossArmSD(JsonVal["09_CrossArmRootDia"].asDouble());
// 			pTubular->setCrossArmED(JsonVal["10_CrossArmTipDia"].asDouble());
// 			pTubular->setWallThickness(JsonVal["11_WallThickness"].asDouble());
// 			pTubular->setInStiPlate(JsonVal["12_InStiPlateThickness"].asDouble());
// 			pTubular->setSidePlateT(JsonVal["13_SidePlateThickness"].asDouble());
// 			pTubular->setArmLength(JsonVal["14_ArmLength"].asDouble());
// 			BaseRapid::SymmetricType pTubularSymmetric = BaseRapid::SymmetricType(JsonVal["15_CrossArmSymmetric"].asInt());
// 			pTubular->setSymmetric(pTubularSymmetric);
// 			GPointFlag::GPointType hangingLeft = GPointFlag::GPointType(JsonVal["16_HangingLeft"].asInt());
// 			pTubular->setHangingLeft(hangingLeft);
// 			GPointFlag::GPointType hangingRight = GPointFlag::GPointType(JsonVal["17_HangingRight"].asInt());
// 			pTubular->setHangingRight(hangingRight);
// 			pTubularHoriArray->add(pTubular);
// 		}
	}

	// @ function:  --- by raoyi  2020/05/11 19:05
	void JsonFormatIO::writeRapidDesign(Json::Value& root)
	{
// 		HdlRapidDesign pRapidDesign = SinglePoleRapid::Instance()->getRapidDesign();
// 		//不用判断非空，DB中new过
// 		Json::Value JsonVal(Json::objectValue);
// 		JsonVal["01_CenterOrFace"] = pRapidDesign->getToCorW();
// 		JsonVal["02_TowerType"] = pRapidDesign->getIsTangentTower();
// 		JsonVal["03_ProfileType"] = pRapidDesign->getNumEdges();
// 		JsonVal["04_MainPoleCNNType"] = pRapidDesign->getMainPoleConnType();
// 		JsonVal["05_TubeCNNType"] = pRapidDesign->getTubeConnType();
// 		JsonVal["06_LineCNNype"] = pRapidDesign->getLineConnType();
// 		JsonVal["07_TopDiameter"] = pRapidDesign->getTopD();
// 		JsonVal["08_RootDiameter"] = pRapidDesign->getBotD();
// 		JsonVal["09_IsTubular"] = pRapidDesign->getIstubular();
// 		JsonVal["10_Isline"] = pRapidDesign->getIsline();
// 		JsonVal["11_TotalHeight"] = pRapidDesign->getTotalH();
// 
// 		root["13_RapidDesign"] = JsonVal;

	}

	// @ function:  --- by raoyi  2020/05/11 19:05
	void JsonFormatIO::readRapidDesign(Json::Value& root)
	{
// 		HdlRapidDesign pRapidDesign = new SinglePoleRapidDesign();
// 		SinglePoleRapid::Instance()->setRapidDesign(pRapidDesign);
// 
// 		const Json::Value& JsonVal = root["13_RapidDesign"];
// 		if (JsonVal.isNull())
// 			return;
// 
// 		pRapidDesign->setToCorW(JsonVal["01_CenterOrFace"].asBool());
// 		pRapidDesign->setIsTangentTower(JsonVal["02_TowerType"].asInt());
// 		Profile::Type TubularProfileType = Profile::Type(JsonVal["03_ProfileType"].asInt());
// 		pRapidDesign->setNumEdges(TubularProfileType);
// 		Connect::Type MainPoleCNNType = Connect::Type(JsonVal["04_MainPoleCNNType"].asInt());
// 		pRapidDesign->setMainPoleConnType(MainPoleCNNType);
// 		Connect::Type TubeCNNType = Connect::Type(JsonVal["05_TubeCNNType"].asInt());
// 		pRapidDesign->setTubeConnType(TubeCNNType);
// 		Connect::Type LineCNNype = Connect::Type(JsonVal["06_LineCNNype"].asInt());
// 		pRapidDesign->setLineConnType(LineCNNype);
// 
// 		pRapidDesign->setTopD(JsonVal["07_TopDiameter"].asDouble());
// 		pRapidDesign->setBotD(JsonVal["08_RootDiameter"].asDouble());
// 		pRapidDesign->setIstubular(JsonVal["09_IsTubular"].asBool());
// 		pRapidDesign->setIsline(JsonVal["10_Isline"].asBool());
// 		pRapidDesign->setTotalH(JsonVal["11_TotalHeight"].asDouble());
	}
	// @ function:   --- by lsq 2020/6/30 14:46
	void JsonFormatIO::readDesignInfos(Json::Value& root)
	{
		HandleDesign pDesign = new DesignParameter();
		ModelDateBase::Instance()->setDesign(pDesign);

		const Json::Value& JsonVal = root["14_DesignInfo"];
		if (JsonVal.isNull())
			return;

		if (JsonVal["01_ProjectName"].isString())
			pDesign->setProjectName(JsonVal["01_ProjectName"].asCString());
		if (JsonVal["02_Designer"].isString())
			pDesign->setDesigner(JsonVal["02_Designer"].asCString());
		if (JsonVal["03_DesigneCon"].isString())
			pDesign->setDesigneCon(JsonVal["03_DesigneCon"].asCString());

		pDesign->setNonlinearFlag(JsonVal["04_NonlinearFlag"].asBool());
		pDesign->setElemSize(JsonVal["05_ElemSize"].asDouble());
		pDesign->setVoltage(JsonVal["06_Voltage"].asInt());
		pDesign->setTerrainType(CEXPO::Transfer(JsonVal["07_TerrainType"].asInt()));
		pDesign->setUserBetaZ(JsonVal["09_UserBetaZ"].asDouble());
		pDesign->setDeflection(JsonVal["10_Deflection"].asDouble());
		pDesign->setAltitude(JsonVal["11_Altitude"].asDouble());
		pDesign->setIsLinearPole(JsonVal["12_IsLinearPole"].asInt());

	}
	// @ function:   --- by lsq 2020/6/30 14:46
	void JsonFormatIO::writeDesignInfos(Json::Value& root)
	{
		HandleDesign pDesign = ModelDateBase::Instance()->getDesign();
		Json::Value JsonVal(Json::objectValue);
		JsonVal["01_ProjectName"] = pDesign->getProjectName();
		JsonVal["02_Designer"] = pDesign->getDesigner();
		JsonVal["03_DesigneCon"] = pDesign->getDesigneCon();
		JsonVal["04_NonlinearFlag"] = pDesign->getNonlinearFlag();
		JsonVal["05_ElemSize"] = pDesign->getElemSize();
		JsonVal["06_Voltage"] = pDesign->getVoltage();
		JsonVal["07_TerrainType"] = pDesign->getTerrainType().Index();
		JsonVal["08_AutoBetaZFlag"] = pDesign->getAutoBetaZFlag();
		JsonVal["09_UserBetaZ"] = pDesign->getUserBetaZ();
		JsonVal["10_Deflection"] = pDesign->getDeflection();
		JsonVal["11_Altitude"] = pDesign->getAltitude();
		JsonVal["12_IsLinearPole"] = pDesign->getIsLinearPole();

		root["14_DesignInfo"] = JsonVal;
	}

}