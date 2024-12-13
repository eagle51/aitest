#pragma once
#include "Profile.h"

//管状截面
namespace SmartPoleCore
{
	class pipeProfile :public Profile
	{
	public:
		pipeProfile(Type itype, int iwide, int thk);
		virtual~pipeProfile();

		virtual Profile* DeepCopy();

		//重新计算面积、转动惯量的参数
		virtual void calcParameter();
		//提供另外一端的截面属性，计算比例位置的截面属性
		virtual Profile* ratioProfile(Profile* pEndProfofile, double totalLen, double distanceToMe);

		//计算点在x轴的投影长度
		double getCx() const
		{
			return _Cx;
		}
		//计算点在y轴的投影长度
		double getCy() const
		{
			return _Cy;
		}
		//确定最大弯曲剪应力的参数
		double getMaxQ_It() const
		{
			return _MaxQ_It;
		}
		//确定最大扭转剪应力的参数
		double getMaxC_J() const
		{
			return _MaxC_J;
		}
		//多边形一条边的平直宽度，mm
		double getW() const 
		{ 
			return _W; 
		}
		//压弯局部稳定的强度设计值
		double getFa() const 
		{ 
			return _fa; 
		}

		void calcfa(double steelf);//压弯局部稳定的强度设计值
		double calcAng();//获取截面计算用角度
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
		void calcCxAndCy();//获取Cx和Cy;参照钢管杆设计规定8.1;
		void calcMaxQ_It();//确定最大弯曲剪应力的参数
		void calcMaxC_J();//确定最大扭转剪应力的参数
		void calcW();//确定多边形一条边的平直宽度

		void calcCirclefa(double steelf);//圆形 压弯局部稳定的强度设计值
		void calcPloygonfa(double steelf);//多边形 压弯局部稳定的强度设计值

		//由基类3个参数计算得出
		double _Cx;//计算点在x轴的投影长度，mm
		double _Cy;//计算点在y轴的投影长度，mm
		double _MaxQ_It;//确定最大弯曲剪应力的参数，1/mm2
		double _MaxC_J;//确定最大扭转剪应力的参数，1/mm3
		double _W;//多边形一条边的平直宽度，mm
		double _fa;//压弯局部稳定的强度设计值

	};
	typedef SharedHandle<pipeProfile>  HdlpipeProfile;
}