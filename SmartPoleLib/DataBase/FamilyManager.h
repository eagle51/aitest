#pragma once
#include <vector>
#include "Component.h"
#include "SinglePole.h"
namespace SmartPoleCore
{
	//��˹�����
	class FamilyManager :public SharedObject
	{
		friend class ModelDateBase;
	public:
		FamilyManager();
		~FamilyManager();

		//��������ĸ���
		size_t getBodyCount() const 
		{ 
			return _BodyCount; 
		}
		size_t getLegCount() const
		{
			return _ComponentVec.size() - _BodyCount;
		}
		//��������ܵĸ���
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
		//�õ��ڼ������������ˣ��м������Ⱦ��м�������,ע��ind
		HandleSinglePole getSingleAt(int ind) const;

		//��ȡ��ߵ�����
		HandleSinglePole getMaxHighPole() const;
		//������
		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::FAMILYMANAGER;
		};
	private:
		void initAndCheck();
		size_t _BodyCount;//��������ĸ���
		std::vector<HandleComponent> _ComponentVec;
	};
	typedef SharedHandle<FamilyManager> HdFamilyManager;

}