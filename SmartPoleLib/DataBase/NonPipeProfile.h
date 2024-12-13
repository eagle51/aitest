#pragma once
#include "Profile.h"


//�ǹ�״����
namespace SmartPoleCore
{
	class NonPipeProfile :public Profile
	{
	public:
		NonPipeProfile(Type itype, double High, double Wide, double UDThickness, double sidePanelThickness);
		~NonPipeProfile();

		virtual Profile* DeepCopy();
		//���¼��������ת�������Ĳ���
		virtual void calcParameter();
		//�ṩ����һ�˵Ľ������ԣ��������λ�õĽ�������
		virtual Profile* ratioProfile(Profile* pEndProfofile, double totalLen, double distanceToMe);

		int getHigh() const { return _High; }
		void setHigh(int val)
		{
			if (val == _High)
				return;
			_High = val;
		}
		int getUDThickness() const { return _UDThickness; }
		void setUDThickness(int val)
		{ 
			if (val == _UDThickness)
				return;
			_UDThickness = val;
		}
// 		virtual void outputToStream(QDataStream& dataStream) const
// 		{
// 			Profile::outputToStream(dataStream);
// 			dataStream << _High << _UDThickness;
// 		}
// 		virtual void inputFromStream(QDataStream& dataStream)
// 		{
// 			Profile::inputFromStream(dataStream);
// 			int pProfileType = _eType;
// 			dataStream >> _High >> _UDThickness;
// 		}
	private:
		void calcIx();
		void calcIy();
		void calcJxy();

		int _High;						      //��
		int _UDThickness;					  //���°��

	};
	typedef SharedHandle<NonPipeProfile>  HdlNonPipeProfile;
}