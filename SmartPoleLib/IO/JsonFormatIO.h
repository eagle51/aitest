#pragma once
#include "json.h"
#include "value.h"
#include "PoleIO.h"
// #include "CircleFlangeConnect.h"
// #include "PlateConnect.h"
// #include "SquareFlangeConnect.h"

namespace SmartPoleCore
{
	class JsonFormatIO :public PoleIO
	{
	public:
		JsonFormatIO();
		~JsonFormatIO();
		virtual void read(const char* username = 0);
		virtual void write(const char* username = 0);
	private:
 		void readPointArray(Json::Value& root);
		void writePointArray(Json::Value& root);
		void readNodeArray(Json::Value& root);
		void writeNodeArray(Json::Value& root);
		void readMemberArray(Json::Value& root);
		void readMembers(HdMemberArray& pMemberArray, const Json::Value& jsonMembers);
		void writeMemberArray(Json::Value& root);
		void wirteMembers(const HdMemberArray& pMemberArray, Json::Value& jsonMembers);
		void readFamily(Json::Value& root);
		void writeFamily(Json::Value& root);
		void readNGPGroup(Json::Value& root);
		void writeNGPGroup(Json::Value& root);
		void readWinCond(Json::Value& root);//工况荷载信息
		void writeWinCond(Json::Value& root);
		void readNodeLoad(Json::Value& root);
		void writeNodeLoad(Json::Value& root);//点荷载
		void readLodRpnInfos(Json::Value& root);
		void writeLodRpnInfos(Json::Value& root); //荷载分配信息
		void readDesignInfos(Json::Value& root);
		void writeDesignInfos(Json::Value& root); //荷载分配信息

		//挠度（结果界面中）
		void readDeflection(Json::Value& root);
		void writeDeflection(Json::Value& root); //荷载分配信息

		//连接计算
// 		void writeFlangeConnectToJson(Json::Value&  root, HdlFlangeConnect member);
// 		void readFlangeConnectToJson(const Json::Value&  root, HdlFlangeConnect member);
// 		void writePlateConnectToJson(Json::Value&  root, HdlPlateConnect PlateConnectDB);
// 		void readPlateConnectToJson(const Json::Value&  root, HdlPlateConnect PlateConnectDB);
// 		void writeSquareFlangeConnectToJson(Json::Value&  root, HdlSquareFlangeConnect squFlangeConnectDB);
// 		void readSquareFlangeConnectToJson(const Json::Value&  root, HdlSquareFlangeConnect squFlangeConnectDB);
// 		void writeFlangePlateDBToJson(Json::Value&  root, HdlFlangePlateDB FlangePlateDB);
// 		void readFlangePlateDBToJson(const Json::Value&  root, HdlFlangePlateDB FlangePlateDB);
// 		void writeStiPlateDBToJson(Json::Value&  root, HdlStiPlateDB StiPlateDB);
// 		void readStiPlateDBToJson(const Json::Value&  root, HdlStiPlateDB StiPlateDB);
		//快速建模
		void writeMainPoleRapid(Json::Value& root);
		void readMainPoleRapid(Json::Value& root);
		void writeLinearCroArmArray(Json::Value& root);
		void readLinearCroArmArray(Json::Value& root);
		void writeTubularHoriArray(Json::Value& root);
		void readTubularHoriArray(Json::Value& root);
		void writeRapidDesign(Json::Value& root);
		void readRapidDesign(Json::Value& root);
	};

}

