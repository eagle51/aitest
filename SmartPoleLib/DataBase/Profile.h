#pragma once
#include <string>
//#include <QDataStream>
#include <SharedHandle.hpp>
//#include "ElmInnerForce.h"
#include "..\Utility\Constants.h"

using namespace std;
namespace SmartPoleCore
{
	class Profile:public SharedObject
	{
	public:
		enum Type
		{
			zero          = 0,
			four          = 4,
			six           = 6,
			eight         = 8,
			twelve        = 12,
			sixteen       = 16,
			CSone         = 100,
			CStwo         = 200,
			UNDEFINED     =-1,//未定义
		};
		Profile(Type itype,int iwide, int thk, double dArea=0, double IX = 0, double IY = 0, double IXY = 0);
		
		virtual Profile* DeepCopy() = 0;

		//重新计算面积、转动惯量的参数
		virtual void calcParameter()=0;
		//提供另外一端的截面属性，计算比例位置的截面属性
		virtual Profile* ratioProfile(Profile* pEndProfofile, double totalLen, double distanceToMe) = 0;
		Profile::Type getType() const
		{
			return _eType;
		}
		const char*  getTypeName() const;

		void setType(Profile::Type val)
		{
			if (val == _eType)
				return;
			_eType = val;
		}
		
		//管状截面边数，非管状为9999;
		int getEdgeNum();
		Profile::Type transfer(int val) ;
		
		//宽度
		int getWide() const 
		{ 
			return _iWide;
		}
		//宽度
		void setWide(int val)
		{
			_iWide = val; 
		}
		int getUIWide() const
		{ 
			return _UIWide; 
		}
		void setUIWide(int val)
		{
			_UIWide = val;
			if (_UIWide < SmartPoleCore::Constants::SysMaxTagNum)
				_iWide = _UIWide;
			else
				_iWide = 0;
		}
		//侧板厚度
		int getThk() const
		{ 
			return _iThickness; 
		}
		//厚度
		void setThk(int val)
		{ 
			_iThickness = val;
		}
		double getArea() const 
		{ 
			return _fArea; 
		}
		void setArea(double val)
		{ 
			_fArea = val;
		}
		double getIxx() const
		{ 
			return _fIxx;
		}
		void setIxx(double val)
		{
			_fIxx = val; 
		}
		double getIyy() const
		{
			return _fIyy; 
		}
		void setIyy(double val) 
		{ 
			_fIyy = val; 
		}
		double getJxy() const 
		{
			return _dJxy; 
		}
		void setJxy(double val) 
		{ 
			_dJxy = val;
		}

		virtual ~Profile(void);

		bool isPipe() //判断该截面是否是管状截面
		{
			if ((_eType < SmartPoleCore::Profile::CSone) && (_eType != SmartPoleCore::Profile::UNDEFINED))
				return true;
			return false;
		}
		//起端最大控制内力
// 		const InnerEndForce& getCtrlForce() const
// 		{
// 			return _CtrlForce;
// 		}
// 		//终端最大控制内力
// 		void setCtrlForce(const InnerEndForce& val)
// 		{
// 			_CtrlForce = val;
// 		}

		void clearForce()
		{
			//_CtrlForce.zero();
			_iCtrlCase = -1;
			_dEffic = 0;
		}

		//控制工况
		int getCtrlCase() const
		{
			return _iCtrlCase;
		}
		//控制工况
		void setCtrlCase(int val)
		{
			_iCtrlCase = val;
		}
 		//应力比
 		double getEffic() const
 		{
 			return _dEffic;
 		}
 		//应力比
 		void setEffic(double val)
 		{
 			_dEffic = val;
 		}

// 		virtual void outputToStream(QDataStream& dataStream) const
// 		{
// 			//dataStream << _iWide << _iThickness << _eType << _fArea << _fIxx << _fIyy << _dJxy ;
// 		}
// 		virtual void inputFromStream(QDataStream& dataStream)
// 		{
// 			int pProfileType = _eType;
// 			//dataStream >> _iWide >> _iThickness >> pProfileType >> _fArea >> _fIxx >> _fIyy >> _dJxy;
// 		}

	protected:
		int _UIWide;
		//以下此3个参数为截面的尺寸参数，其他参数均由此计算
		int _iWide;			//对于管状截面，圆的外接圆直径或多边形两对应边，外边至外边的距离，对于非管状截面，为截面的宽度，mm
		int _iThickness;	//对于管状截面，壁厚，对于非管状截面，为截面的左右板厚，mm
		Profile::Type _eType;  //截面的类型

		double _fArea;		//毛截面面积,单位吗mm^2
		double _fIxx;		//惯性矩mm^4
		double _fIyy;		//惯性矩mm^4
		double _dJxy;       //转动惯量

		//控制信息
//		InnerEndForce _CtrlForce; //起端控制内力
		int _iCtrlCase; //起端控制工况
		double _dEffic;  //起端应力比
	
	private:
		friend class ProfileDataBase;
	};
	typedef SharedHandle<Profile> HandleProfile;
}