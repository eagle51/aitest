#pragma once
#include <vector>
#include "Component.h"
#include "SinglePole.h"
namespace SmartPoleCore
{
	//多杆管理器
	class FamilyManager :public SharedObject
	{
		friend class ModelDateBase;
	public:
		FamilyManager();
		~FamilyManager();

		//公共杆身的个数
		size_t getBodyCount() const 
		{ 
			return _BodyCount; 
		}
		size_t getLegCount() const
		{
			return _ComponentVec.size() - _BodyCount;
		}
		//本体接腿总的个数
		size_t getTotalCount() const
		{
			return _ComponentVec.size();
		}
		HandleComponent at(int ind) const
		{
			return _ComponentVec.at(ind);
		}
		HandleComponent getLegAt(int ind) const
		{
			return _ComponentVec.at(_BodyCount + ind);
		}
		void add(const HandleComponent& rhs)
		{
			_ComponentVec.push_back(rhs);
		}
		void insert(int iPos, const HandleComponent& rhs)
		{
			_ComponentVec.insert(_ComponentVec.begin() + iPos,rhs);
		}
		int remove(const HandleComponent& rhs);

		void removeAt(int iPos)
		{
			_ComponentVec.erase(_ComponentVec.begin() + iPos);
		}
		void removeRange(int iStart, int iCount)
		{
			_ComponentVec.erase(_ComponentVec.begin() + iStart, _ComponentVec.begin() + iStart + iCount);
		}
		void clear()
		{
			_ComponentVec.clear();
		}
		int refContains(const HandleComponent& rhs) const;

		bool empty()const
		{
			return _ComponentVec.empty();
		}
		//得到第几个完整的主杆，有几个接腿就有几个主杆,注意ind
		HandleSinglePole getSingleAt(int ind) const;

		//获取最高的主杆
		HandleSinglePole getMaxHighPole() const;
		//当类型
		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::FAMILYMANAGER;
		};
	private:
		void initAndCheck();
		size_t _BodyCount;//公共杆身的个数
		std::vector<HandleComponent> _ComponentVec;
	};
	typedef SharedHandle<FamilyManager> HdFamilyManager;

}