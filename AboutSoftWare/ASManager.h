/****************************************************************/
//Author: 	马梓淇
//Version: 	1.0
//Date:		2022.05.10
//Description:	主管理类
/****************************************************************/
#pragma once
#include "UpDateInfo.h"
#include "SoftWareBaseInfo.h"

namespace AboutSoftWare
{
	class ASManager
	{
	public:
		static ASManager* getInstance();

		bool isDefineNotUse3rdEncrypt();
		const QString& getAboutFilePath() const { return _aboutFilePath; }
		HdlSoftWareBaseInfo getSoftWareBaseInfo() const { return _softWareBaseInfo; }
		HdlVersionUpDateInfoVec getVersionUpDateInfoVec() const { return _versionUpDateInfoVec; }

		void setAboutFilePath(const QString& val) { _aboutFilePath = val; }

		void readFromJson();//读取本地AboutSoftWare文件

	private:
		ASManager();
		~ASManager() {}

		QString                         _aboutFilePath;
		HdlSoftWareBaseInfo             _softWareBaseInfo;//软件基本信息
		HdlVersionUpDateInfoVec         _versionUpDateInfoVec;//版本更新信息
	};
}
#define ASMgr AboutSoftWare::ASManager::getInstance()//管理者