#pragma once
#include "Profile.h"

//��״����
namespace SmartPoleCore
{
	class pipeProfile :public Profile
	{
	public:
		pipeProfile(Type itype, int iwide, int thk);
		virtual~pipeProfile();

		virtual Profile* DeepCopy();

		//���¼��������ת�������Ĳ���
		virtual void calcParameter();
		//�ṩ����һ�˵Ľ������ԣ��������λ�õĽ�������
		virtual Profile* ratioProfile(Profile* pEndProfofile, double totalLen, double distanceToMe);

		//�������x���ͶӰ����
		double getCx() const
		{
			return _Cx;
		}
		//�������y���ͶӰ����
		double getCy() const
		{
			return _Cy;
		}
		//ȷ�����������Ӧ���Ĳ���
		double getMaxQ_It() const
		{
			return _MaxQ_It;
		}
		//ȷ�����Ťת��Ӧ���Ĳ���
		double getMaxC_J() const
		{
			return _MaxC_J;
		}
		//�����һ���ߵ�ƽֱ��ȣ�mm
		double getW() const 
		{ 
			return _W; 
		}
		//ѹ��ֲ��ȶ���ǿ�����ֵ
		double getFa() const 
		{ 
			return _fa; 
		}

		void calcfa(double steelf);//ѹ��ֲ��ȶ���ǿ�����ֵ
		double calcAng();//��ȡ��������ýǶ�
// 		virtual void outputToStream(QDataStream& dataStream) const
// 		{
// 			Profile::outputToStream(dataStream);
// 			dataStream << _Cx << _Cy << _MaxQ_It << _MaxC_J << _W << _fa;
// 		}
// 		void inputFromStream(QDataStream& dataStream)
// 		{
// 			Profile::inputFromStream(dataStream);
// 			int pProfileType = _eType;
// 			dataStream >> _Cx >> _Cy >> _MaxQ_It >> _MaxC_J >> _W >> _fa;
// 		}
	private:
		void calcCxAndCy();//��ȡCx��Cy;���ոֹܸ���ƹ涨8.1;
		void calcMaxQ_It();//ȷ�����������Ӧ���Ĳ���
		void calcMaxC_J();//ȷ�����Ťת��Ӧ���Ĳ���
		void calcW();//ȷ�������һ���ߵ�ƽֱ���

		void calcCirclefa(double steelf);//Բ�� ѹ��ֲ��ȶ���ǿ�����ֵ
		void calcPloygonfa(double steelf);//����� ѹ��ֲ��ȶ���ǿ�����ֵ

		//�ɻ���3����������ó�
		double _Cx;//�������x���ͶӰ���ȣ�mm
		double _Cy;//�������y���ͶӰ���ȣ�mm
		double _MaxQ_It;//ȷ�����������Ӧ���Ĳ�����1/mm2
		double _MaxC_J;//ȷ�����Ťת��Ӧ���Ĳ�����1/mm3
		double _W;//�����һ���ߵ�ƽֱ��ȣ�mm
		double _fa;//ѹ��ֲ��ȶ���ǿ�����ֵ

	};
	typedef SharedHandle<pipeProfile>  HdlpipeProfile;
}