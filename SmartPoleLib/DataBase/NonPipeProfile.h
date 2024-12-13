#pragma once
#include "Profile.h"


//非管状截面
namespace SmartPoleCore
{
	class NonPipeProfile :public Profile
	{
	public:
		NonPipeProfile(Type itype, double High, double Wide, double UDThickness, double sidePanelThickness);
		~NonPipeProfile();

		virtual Profile* DeepCopy();
		//重新计算面积、转动惯量的参数
		virtual void calcParameter();
		//提供另外一端的截面属性，计算比例位置的截面属性
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

		int _High;						      //高
		int _UDThickness;					  //上下板厚

	};
	typedef SharedHandle<NonPipeProfile>  HdlNonPipeProfile;
}