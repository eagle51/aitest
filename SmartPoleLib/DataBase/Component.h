#pragma once
#include "PointArray.h"
#include "MemberArray.h"
#include "NodeArray.h"
#include "ElemArray.h"
// #include "ElmInnerForceArray.h"
// #include "NodeDisplacementArray.h"
namespace SmartPoleCore
{
	//��˵Ļ����������֮����ݵ�Ž��й���
	class Component:public SharedObject
	{
	public:
		Component(int maxNum = 0, int cnnIndex = -1);
		~Component();
		//����ģ������ţ�������10000
		int getMaxNumber() const 
		{
			return _MaxNumber;
		}
		//����ģ������ţ�������10000
		void setMaxNumber(int val)
		{ 
			_MaxNumber = val; 
		}
		//�û�����Ĺҽӱ�����ţ���ʼ0����������-1��������ʾʱ+1
		int getUserDefineCnnIndex() const 
		{ 
			return _UserCnnIndex;
		}
		//�����ӵı���ţ���ʼ0����������-1��������ʾ+1
		int getCnnIndex() const
		{
			if (_PartPos == 0)//��һ��ʱ
				return -1;
			else if (_UserCnnIndex < 0)
				return _PartPos - 1;
			return _UserCnnIndex;
		}
		//����������û��ָ���Ǿ��ǽ���
		bool isLegType() const
		{
			return _UserCnnIndex >=0;
		}
		//���ӱ���ţ���ʼ0����������-1
		void setUserDefineCnnIndex(int val) 
		{ 
			_UserCnnIndex = val; 
		}
		const HdPointArray& getPointArray() const
		{
			return _PointArray;
		}
		const HdMemberArray& getMemberArray() const
		{
			return _MemberArray;
		}
		int getPosition() const
		{
			return _PartPos;
		}
		bool getIsDraw() const { return _isDraw; }
		void setIsDraw(bool val) { _isDraw = val; }
		virtual PoleObject::Type GetObjType() const
		{
			return PoleObject::COMPONENT;
		};
	private:
		friend class FamilyManager;

		void initAndCheck(int upMaxNum);

		bool _isDraw;

		int _PartPos;//λ��
		int _upMaxNumber;//��һ��������

		int _MaxNumber;//������
		int _UserCnnIndex;//�û�����Ĺҽ����
		HdPointArray _PointArray;
		HdMemberArray _MemberArray;
	};

	typedef SharedHandle<Component> HandleComponent;

}
