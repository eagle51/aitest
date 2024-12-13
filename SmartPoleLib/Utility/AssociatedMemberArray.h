#pragma once
#include <vector>
#include "Vec3D.h"
#include "Member.h"
namespace SmartPoleCore
{
	//��ĳһ��������ĸ˼�����
	class AssociatedMemberArray:public SharedObject
	{
	public:
		AssociatedMemberArray(const HandleVec3D &pPoint);
		~AssociatedMemberArray();
		//������Ӹ˼���ͬʱ�������ĵ��������ɶ�����
		void addElm(HandleMember &pMemb)
		{
			if (pMemb->getStartNum() == _centrlPoint->TagNum() || pMemb->getEndNum() == _centrlPoint->TagNum())
			{
				_aCnnMembArray.push_back(pMemb);
			}
		};
		//�鿴�Ƿ����غϵ�ֱ�ߡ�
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

		//�����������
	private:
		HandleVec3D _centrlPoint;//���ĵ�
		std::vector<HandleMember> _aCnnMembArray;//���������ӵĸ˼�

		static vector<CVec3D> lineOverlapCheckArray;
		static CVec3D LineVec3d;
	};
	typedef SharedHandle<AssociatedMemberArray> HdAssociatedMembers;


	
}