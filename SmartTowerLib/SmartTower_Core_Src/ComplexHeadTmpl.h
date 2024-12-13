#pragma once
#include "MetaModel.h"
#include "LineTextCounter.h"
#include "PrimaryPointExt.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年9月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	class ComplexHeadTmpl:public SharedObject
	{
	public:
		ComplexHeadTmpl(void);
		~ComplexHeadTmpl(void);
		HandleDimensionVal getDimensionValAt(int idx)
		{
			return _DimensionVals.at(idx);
		}
		size_t DimensionValCount() const
		{
			return _DimensionVals.size();
		}
		ComplexHeadTmpl* DeepCopy() const;
		//约定第一个点必须是塔头插入点,必须是控制坐标,XYZ
		void setInsertPosition(double kpX, double kpY, double kpZ);
		SmartTower::CVec3D getInsertCoor() const;
		void getNewTempl();
		bool read(const char* filename);//读取模板
	private:
		string _filename;	
		double _InsertPositionZ;
		std::vector<HandleDimensionVal> _DimensionVals;
		std::vector<HandleLineFormatVar> _FormatVars;
		std::vector<HandlePrimaryPointExt> _PrimaryPointArray;
		HdTowerPointArray _SecondaryPointArray;   //第二类节点动态数组	
		HdMemberArray _MenberArray;//杆件集合
	};
	typedef StHandle<ComplexHeadTmpl> HandleCplxHeadTmpl;
}