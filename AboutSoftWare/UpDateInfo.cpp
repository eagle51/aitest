#include "UpDateInfo.h"
#include <QString>
#include "QMessageBox"

namespace AboutSoftWare 
{
	// @ function:    -- by mzq 2020/02/06 18:10
	UpDateInfo::UpDateInfo()
		:_moduleType(Module::All),
		_updateInfo("")
	{

	}

	// @ function: 读取Json格式数据    -- by mzq 2020/02/06 18:10
	void UpDateInfo::readFromJson(const QJsonObject& root)
	{
		_moduleType = Module::strToType(root[QStringLiteral("00模块类型")].toString());
		_updateInfo = root[QStringLiteral("01更新信息")].toString();
	}




	/**********************************分割线****************************************/
	// @function:   -mzq 2020/2/13 19:46
	VersionUpDateInfo::VersionUpDateInfo()
		:_version(""),
		_versionTime("")
	{

	}

	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfo::insert(HdlUpDateInfo value, int curInd)
	{
		if (curInd > (int)_vec.size() || curInd < 0)
			throw std::exception("点插入失败-插入非法（不应为负或大于容器维数）！");

		_vec.insert(_vec.begin() + curInd, value);
	}
	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfo::del(int curInd)
	{
		if (curInd >= (int)_vec.size() || curInd < 0)
			throw std::exception("点删除失败-删除非法（不应为负或大于容器维数）！");

		_vec.erase(_vec.begin() + curInd);
	}

	//@function: 是否包含指定模块的更新内容   -- by mzq 2022/5/12 15:39
	bool VersionUpDateInfo::hasModule(Module::Type moduleType/*模块类型*/)
	{
		for (unsigned int loop = 0; loop < _vec.size(); loop++)
		{
			HdlUpDateInfo upDateInfo = _vec[loop];
			if (upDateInfo->getModuleType() == moduleType)
				return true;
		}
		return false;
	}

	//@function: 根据模块类型获取所属的更新信息   -- by mzq 2022/5/12 16:30
	std::vector<HdlUpDateInfo> VersionUpDateInfo::getUpDateInfoByModule(Module::Type moduleType/*模块类型*/)
	{
		std::vector<HdlUpDateInfo> res;
		for (unsigned int loop = 0; loop < _vec.size(); loop++)
		{
			HdlUpDateInfo upDateInfo = _vec[loop];
			if (upDateInfo->getModuleType() == moduleType || moduleType == Module::All)
				res.push_back(upDateInfo);
		}
		return res;
	}

	//version版本转换为int值100 000 000 000, 最多支持4位版本号，子节点最多为100，不够的自动补全，方便对比版本大小，
	//如：V1.5.2.1->001 005 002 001，V1.5->001 005 000 000，V1.30.44->1560，V1->1000   -- by mzq 2022/10/3 11:27
	long long VersionUpDateInfo::versionToLonglong()
	{
		QString versionQStr = _version;
		versionQStr.remove(QString("V"));
		versionQStr.remove(QString("v"));
		QStringList list = versionQStr.split('.');

		long long res;
		if (list.size() == 1)
			res = list[0].toLongLong() * 1000000000;
		else if (list.size() == 2)
			res = list[0].toLongLong() * 1000000000 + list[1].toLongLong() * 1000000;
		else if (list.size() == 3)
			res = list[0].toLongLong() * 1000000000 + list[1].toLongLong() * 1000000 + list[2].toLongLong() * 1000;
		else if (list.size() == 4)
			res = list[0].toLongLong() * 1000000000 + list[1].toLongLong() * 1000000 + list[2].toLongLong() * 1000 + list[3].toLongLong();
		return res;
	}

	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfo::readFromJson(const QJsonObject& root)
	{
		_vec.clear();
		_version = root[QStringLiteral("00版本信息")].toString();
		_versionTime = root[QStringLiteral("01版本时间")].toString();
		const QJsonArray& jsonArr = root[QStringLiteral("02更新信息")].toArray();
		for (unsigned int loop = 0; loop < jsonArr.size(); loop++)
		{
			HdlUpDateInfo single = new UpDateInfo();
			single->readFromJson(jsonArr[loop].toObject());
			_vec.push_back(single);
		}
	}




	/**********************************分割线****************************************/
	// @function:   -mzq 2020/2/13 19:46
	VersionUpDateInfoVec::VersionUpDateInfoVec()
	{

	}


	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfoVec::insert(HdlVersionUpDateInfo value, int curInd)
	{
		if (curInd > (int)_vec.size() || curInd < 0)
			throw std::exception("点插入失败-插入非法（不应为负或大于容器维数）！");

		_vec.insert(_vec.begin() + curInd, value);
	}

	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfoVec::del(int curInd)
	{
		if (curInd >= (int)_vec.size() || curInd < 0)
			throw std::exception("点删除失败-删除非法（不应为负或大于容器维数）！");

		_vec.erase(_vec.begin() + curInd);
	}

	//@function: 获取所有版本的对应模块更新条目数量   -- by mzq 2022/5/19 17:31
	int VersionUpDateInfoVec::getAllModuleUpdateCount(Module::Type moduleType/*模块类型*/) const
	{
		int res = 0;
		for (unsigned int iloop = 0; iloop < _vec.size(); iloop++)
		{
			std::vector<HdlUpDateInfo> vec = _vec[iloop]->getUpDateInfoByModule(moduleType);
			res += vec.size();
		}
		return res;
	}

	//@function: 根据模块类型筛选更新信息   -- by mzq 2022/5/12 15:27
	std::vector<HdlVersionUpDateInfo> VersionUpDateInfoVec::getUpDateInfoByModule(Module::Type moduleType/*模块类型*/)
	{
		std::vector<HdlVersionUpDateInfo> res;
		for (unsigned int loop = 0; loop < _vec.size(); loop++)
		{
			HdlVersionUpDateInfo versionUpDateInfo = _vec[loop];
			if (moduleType == Module::All)
			{
				res.push_back(versionUpDateInfo);
			} 
			else
			{
				HdlVersionUpDateInfo newInfo = new VersionUpDateInfo();
				newInfo->setVersion(versionUpDateInfo->getVersion());
				newInfo->setVersionTime(versionUpDateInfo->getVersionTime());

				std::vector<HdlUpDateInfo> infoVec = versionUpDateInfo->getUpDateInfoByModule(moduleType);
				for (unsigned int loop = 0; loop < infoVec.size(); loop++)
				{
					HdlUpDateInfo info = infoVec[loop];
					newInfo->add(info);
				}
				res.push_back(newInfo);
			}
		}
		return res;
	}

	//@function: 当前版本是否比比较对象更新或相等   -- by mzq 2022/9/2 14:35
	bool VersionUpDateInfoVec::isNewerOrEqual(StHandle_AS<VersionUpDateInfoVec> compare/*比较对象*/)
	{
		if (_vec.size() == 0)
		{
			QMessageBox::warning(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("软件当前版本信息获取有误"), QStringLiteral("确认"));
			return false;
		}
		if (compare->getSize() == 0)
			return true;
			
		if (_vec[0]->versionToLonglong() >= compare->at(0)->versionToLonglong())
			return true;
		return false;
		
	}

	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfoVec::readFromJson(const QJsonArray& root)
	{
		_vec.clear();
		for (unsigned int loop = 0; loop < root.size(); loop++)
		{
			HdlVersionUpDateInfo single = new VersionUpDateInfo();
			single->readFromJson(root[loop].toObject());
			_vec.push_back(single);
		}
	}
}