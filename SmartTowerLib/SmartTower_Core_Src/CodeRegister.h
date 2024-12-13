#pragma once

namespace SmartTower
{	
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月25日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////
	class CodeRegister
	{
	public:	
		static int _LanguageFlag;
		static int NonlinearStep;		
		static int structAnalyzeType;
		static double UnbalanceWarning;
		static double UnbalanceStop;

		static double SpecialSupportLoadAngle;
		static double SpecialSupportLoadFac;
		static bool bMaxLegForceFlag;
		static bool bMaxBracingFlag;
		static double VFaceAmplFac;
		//是否采用梁标记
		static bool BeamFlag;
		//只有国内规范
		static bool OnlyChinaCode;
		//钢材稳定系数A*类
		static int bStatbleFacAStar;
		//2012规范双角钢计算方法
		static int bDoubleAngleCorrection;
		//荷载施加方式，0是TTA，1代表on code,2代表 onface
		static int setAllNodeFLoad;
		//求解模式0,代表内力计算，1代表BetaZ1.0基础作用力，2代表-1/2+1，3代表覆冰塔重增大系数1.0,
		//4代表埃菲尔效应计算上0.3,5代表埃菲尔效应计算下0.3,6代表临时计算用
		static int iSolveMode;
		//埃菲尔效应参考高度
		static double EffelEffectRefHigh;
		//埃菲尔效应的折减系数
		static const double EffelEffectFac;
		//验算时重新计算螺栓数量
		static int bCalcBoltNum;
		//是否优化选材
		static int OptInheritInfo;
		static void setReadDefault();
		static bool bPLSWinPanelSolver;

		static bool bOverlapForWinWWA;//是否考虑风压重复统计的问题，True表示重复统计，False表示不重复统计
		static double MinHangingPointZ;
		static bool legFaceHorCrossTotalLen;
		
		CodeRegister(void);
		~CodeRegister(void);
	};
}