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

	// @ function: ��ȡJson��ʽ����    -- by mzq 2020/02/06 18:10
	void UpDateInfo::readFromJson(const QJsonObject& root)
	{
		_moduleType = Module::strToType(root[QStringLiteral("00ģ������")].toString());
		_updateInfo = root[QStringLiteral("01������Ϣ")].toString();
	}




	/**********************************�ָ���****************************************/
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
			throw std::exception("�����ʧ��-����Ƿ�����ӦΪ�����������ά������");

		_vec.insert(_vec.begin() + curInd, value);
	}
	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfo::del(int curInd)
	{
		if (curInd >= (int)_vec.size() || curInd < 0)
			throw std::exception("��ɾ��ʧ��-ɾ���Ƿ�����ӦΪ�����������ά������");

		_vec.erase(_vec.begin() + curInd);
	}

	//@function: �Ƿ����ָ��ģ��ĸ�������   -- by mzq 2022/5/12 15:39
	bool VersionUpDateInfo::hasModule(Module::Type moduleType/*ģ������*/)
	{
		for (unsigned int loop = 0; loop < _vec.size(); loop++)
		{
			HdlUpDateInfo upDateInfo = _vec[loop];
			if (upDateInfo->getModuleType() == moduleType)
				return true;
		}
		return false;
	}

	//@function: ����ģ�����ͻ�ȡ�����ĸ�����Ϣ   -- by mzq 2022/5/12 16:30
	std::vector<HdlUpDateInfo> VersionUpDateInfo::getUpDateInfoByModule(Module::Type moduleType/*ģ������*/)
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

	//version�汾ת��Ϊintֵ100 000 000 000, ���֧��4λ�汾�ţ��ӽڵ����Ϊ100���������Զ���ȫ������ԱȰ汾��С��
	//�磺V1.5.2.1->001 005 002 001��V1.5->001 005 000 000��V1.30.44->1560��V1->1000   -- by mzq 2022/10/3 11:27
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
		_version = root[QStringLiteral("00�汾��Ϣ")].toString();
		_versionTime = root[QStringLiteral("01�汾ʱ��")].toString();
		const QJsonArray& jsonArr = root[QStringLiteral("02������Ϣ")].toArray();
		for (unsigned int loop = 0; loop < jsonArr.size(); loop++)
		{
			HdlUpDateInfo single = new UpDateInfo();
			single->readFromJson(jsonArr[loop].toObject());
			_vec.push_back(single);
		}
	}




	/**********************************�ָ���****************************************/
	// @function:   -mzq 2020/2/13 19:46
	VersionUpDateInfoVec::VersionUpDateInfoVec()
	{

	}


	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfoVec::insert(HdlVersionUpDateInfo value, int curInd)
	{
		if (curInd > (int)_vec.size() || curInd < 0)
			throw std::exception("�����ʧ��-����Ƿ�����ӦΪ�����������ά������");

		_vec.insert(_vec.begin() + curInd, value);
	}

	// @function:   -mzq 2020/2/13 19:42
	void VersionUpDateInfoVec::del(int curInd)
	{
		if (curInd >= (int)_vec.size() || curInd < 0)
			throw std::exception("��ɾ��ʧ��-ɾ���Ƿ�����ӦΪ�����������ά������");

		_vec.erase(_vec.begin() + curInd);
	}

	//@function: ��ȡ���а汾�Ķ�Ӧģ�������Ŀ����   -- by mzq 2022/5/19 17:31
	int VersionUpDateInfoVec::getAllModuleUpdateCount(Module::Type moduleType/*ģ������*/) const
	{
		int res = 0;
		for (unsigned int iloop = 0; iloop < _vec.size(); iloop++)
		{
			std::vector<HdlUpDateInfo> vec = _vec[iloop]->getUpDateInfoByModule(moduleType);
			res += vec.size();
		}
		return res;
	}

	//@function: ����ģ������ɸѡ������Ϣ   -- by mzq 2022/5/12 15:27
	std::vector<HdlVersionUpDateInfo> VersionUpDateInfoVec::getUpDateInfoByModule(Module::Type moduleType/*ģ������*/)
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

	//@function: ��ǰ�汾�Ƿ�ȱȽ϶�����»����   -- by mzq 2022/9/2 14:35
	bool VersionUpDateInfoVec::isNewerOrEqual(StHandle_AS<VersionUpDateInfoVec> compare/*�Ƚ϶���*/)
	{
		if (_vec.size() == 0)
		{
			QMessageBox::warning(nullptr, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����ǰ�汾��Ϣ��ȡ����"), QStringLiteral("ȷ��"));
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