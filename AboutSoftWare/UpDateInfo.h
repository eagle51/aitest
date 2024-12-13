/****************************************************************/
//Author:   马梓淇
//Version:  1.0
//Date:		2022.5.17
//Description:	模块更新信息		
/****************************************************************/
#pragma once
#include "StHandle_AS.hpp"
#include "EnumLib_AS.h"

#include <QJsonObject>
#include <QJsonArray>

namespace AboutSoftWare
{
	//某个模块的更新信息
	class UpDateInfo :public SharedObject_AS
	{
	public:
		UpDateInfo();
		~UpDateInfo(){};

		Module::Type getModuleType() const { return _moduleType; }
		const QString& getUpdateInfo() const { return _updateInfo; }

		void setModuleType(Module::Type val) { _moduleType = val; }
		void setUpdateInfo(const QString& val) { _updateInfo = val; }

	public:
		void readFromJson(const QJsonObject& root);

	private:
		Module::Type            _moduleType;//模块类型
		QString                 _updateInfo;//更新信息

	};
	typedef StHandle_AS<UpDateInfo> HdlUpDateInfo;


	//某个版本的更新信息
	class VersionUpDateInfo :public SharedObject_AS
	{
	public:
		VersionUpDateInfo();
		~VersionUpDateInfo(){};

		const QString& getVersion() const { return _version; }
		const QString& getVersionTime() const { return _versionTime; }

		void setVersion(const QString& val) { _version = val; }
		void setVersionTime(const QString& val) { _versionTime = val; }

		void add(HdlUpDateInfo value){ _vec.push_back(value); }
		void insert(HdlUpDateInfo value, int curInd);
		void del(int curInd);
		HdlUpDateInfo at(size_t index){ return _vec.at(index); }
		void clear(){ _vec.clear(); }
		int getSize(){ return int(_vec.size()); }

		bool hasModule(Module::Type moduleType/*模块类型*/);//是否包含指定模块的更新内容

		std::vector<HdlUpDateInfo> getUpDateInfoByModule(Module::Type moduleType/*模块类型*/);//根据模块类型获取所属的更新信息

		//version版本转换为int值100 000 000 000, 最多支持4位版本号，子节点最多为100，不够的自动补全，方便对比版本大小，
		//如：V1.5.2.1->001 005 002 001，V1.5->001 005 000 000，V1.30.44->1560，V1->1000
		long long versionToLonglong();

	public:
		void readFromJson(const QJsonObject& root);

	private:
		QString                         _version;//版本信息,需要以V开头，比如：V1.5, V1.6.5
		QString				    		_versionTime;// 版本时间
		std::vector<HdlUpDateInfo>      _vec;//更新信息

	};
	typedef StHandle_AS<VersionUpDateInfo> HdlVersionUpDateInfo;


	//整个软件的更新信息汇总
	class VersionUpDateInfoVec :public SharedObject_AS
	{
	public:
		VersionUpDateInfoVec();
		~VersionUpDateInfoVec(){};

		void add(HdlVersionUpDateInfo value){ _vec.push_back(value); }
		void insert(HdlVersionUpDateInfo value, int curInd);
		void del(int curInd);
		HdlVersionUpDateInfo at(size_t index){ return _vec.at(index); }
		void clear(){ _vec.clear(); }
		int getSize(){ return int(_vec.size()); }

		//获取所有版本的对应模块更新条目数量
		int getAllModuleUpdateCount(Module::Type moduleType/*模块类型*/) const;

		//根据模块类型筛选更新信息
		std::vector<HdlVersionUpDateInfo> getUpDateInfoByModule(Module::Type moduleType/*模块类型*/);

		bool isNewerOrEqual(StHandle_AS<VersionUpDateInfoVec> compare/*比较对象*/);//当前版本是否比比较对象更新或相等

	public:
		void readFromJson(const QJsonArray& root);

	private:
		std::vector<HdlVersionUpDateInfo>      _vec;//版本信息应该降序排列，容器中的第一个应为最新版本

	};
	typedef StHandle_AS<VersionUpDateInfoVec> HdlVersionUpDateInfoVec;
}