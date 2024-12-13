#pragma once
#include <vector>
#include "Vec3D.h"
#include "Member.h"
namespace SmartPoleCore
{
	//与某一点相关联的杆件集合
	class AssociatedMemberArray:public SharedObject
	{
	public:
		AssociatedMemberArray(const HandleVec3D &pPoint);
		~AssociatedMemberArray();
		//添加连接杆件，同时设置中心点的最大自由度类型
		void addElm(HandleMember &pMemb)
		{
			if (pMemb->getStartNum() == _centrlPoint->TagNum() || pMemb->getEndNum() == _centrlPoint->TagNum())
			{
				_aCnnMembArray.push_back(pMemb);
			}
		};
		//查看是否有重合的直线。
		virtual bool bCheckLineOverlap() const;
		//
		int getCnnMembCount() const
		{
			return int(_aCnnMembArray.size());
		}
		const HandleMember& getCnnMembAt(int index) const
		{
			return _aCnnMembArray.at(index);
		}
		HandlePoint getEndPointAt(int index) const
		{
			HandleMember pTmpMemb = _aCnnMembArray.at(index);
			if (pTmpMemb->getStartNum() == _centrlPoint->TagNum())
				return pTmpMemb->getEndPt();
			return pTmpMemb->getStartPt();
		}
		void setCrossArmByLegDiameter();

		//计算基础反力
	private:
		HandleVec3D _centrlPoint;//中心点
		std::vector<HandleMember> _aCnnMembArray;//与中心连接的杆件

		static vector<CVec3D> lineOverlapCheckArray;
		static CVec3D LineVec3d;
	};
	typedef SharedHandle<AssociatedMemberArray> HdAssociatedMembers;


	
}