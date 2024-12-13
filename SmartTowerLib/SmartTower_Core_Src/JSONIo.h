#pragma once
#include "json.h"
#include "value.h"
#include "TowerModel.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
using namespace std;
namespace SmartTower
{	
	class JSONIo
	{
	public:
		JSONIo(CTowerModel* pModel);
		~JSONIo(void);
		virtual void read(const char* username);
	private:
		//读取
		virtual void readHeadStr(Json::Value& root);
		//读取
		virtual void readPart(Json::Value& root);
		//读取一类点
		virtual void readPrimaryPoints(Json::Value& root);
		//读取二类点
		virtual void readSecPoint(Json::Value& root);
		//读取西北院二类点
		virtual void readXBSecPoint(Json::Value& root);
		//读取受力材信息
		virtual void readPrimaryMember(Json::Value& root);

		virtual void readMembers(HdMemberArray& pMemberArray,const Json::Value& jsonMembers);
		//
		CTowerModel* _pModel;
	};

}

