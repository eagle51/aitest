#pragma once
#include "SmartException.h"
#include "Point.h"
namespace SmartPoleCore
{	
	class PointErrException:public SmartException
	{
	public:
		enum ErrType
		{
			DuplicateNum=0,         //�ڵ���ظ�����
			NumErr=1,               //�ڵ��Ų����Ϲ���
			RefPointErr=2,          //���õĵ��޷������ɣ��ҵ���_
			RatioErr=3,             //���õĵ��غϣ����߱�������
			FoundationZErr=4,       //ͬһ���߻�����ڵ�߶�Z��ͬ
			FoundationIsymErr=5,	//������ĶԳ��Բ�Ϊ4
			CoorErr=6,              //����ֵ����
			SymAxisErr =7,          //�Գ��Դ��󣬶ԳƵ��غ�
			IsymErr=8,              //�Գ�����Ŵ���
			IsLinkMem = 9,              //�Ƿ����Ӹ˼�
		};
		PointErrException(const HandlePoint& ErrPoint,ErrType ErrInfo);
		~PointErrException(void);
		virtual bool isRefPointErr() const
		{
			if (_errInFo == RefPointErr)
				return true;
			return false;
		}

		virtual bool isSymErr() const
		{
			if (_errInFo == IsymErr)
				return true;
			return false;
		}
		virtual bool isDuplicate() const
		{
			if (_errInFo == DuplicateNum)
				return true;
			return false;
		}
	private:
		void getErrInfo();
		ErrType _errInFo;
	};

}