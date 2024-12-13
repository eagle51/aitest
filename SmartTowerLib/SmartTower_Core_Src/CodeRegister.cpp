//#include "StdAfx.h"
#include "CodeRegister.h"


namespace SmartTower
{
	//////////////////////////////////////////
	/////////////////////////////////////////
	///该类已于2022年8月25日重构校队完成,李辉
	///产权归陕西恒巨软件科技有限公司所有
	/////////////////////////////////////
	/////////////////////////////////////

	CodeRegister::CodeRegister(void)
	{
	}


	CodeRegister::~CodeRegister(void)
	{
	}


	double CodeRegister::SpecialSupportLoadAngle = 25;
#ifdef SmartTowerFlag	
	double CodeRegister::SpecialSupportLoadFac=0.0250;
#else
	double CodeRegister::SpecialSupportLoadFac=0.0333;
#endif

#ifdef SWEPDI
	bool CodeRegister::bMaxLegForceFlag=true;

	bool CodeRegister::bMaxBracingFlag=true;

	double CodeRegister::VFaceAmplFac = 1.414;
#else
	bool CodeRegister::bMaxLegForceFlag=false;
	bool CodeRegister::bMaxBracingFlag = false;
	double CodeRegister::VFaceAmplFac=1.000;
#endif

	double CodeRegister::MinHangingPointZ=-1000.0;
#ifdef CSEPDI
	bool CodeRegister::legFaceHorCrossTotalLen=true;
#else
	bool CodeRegister::legFaceHorCrossTotalLen = false;
#endif
	

	int CodeRegister::bStatbleFacAStar=0;

	int CodeRegister::bDoubleAngleCorrection=2;

	int CodeRegister::setAllNodeFLoad=0;

	int CodeRegister::_LanguageFlag = 1;//1表示中文，0表示英文

	int CodeRegister::iSolveMode=0;

	bool CodeRegister::OnlyChinaCode=false;

	bool CodeRegister::bPLSWinPanelSolver=false;

	// #ifdef SmartTowerFlag
// 	bool CodeRegister::bCalcBoltNum=true;
// #else
	int CodeRegister::bCalcBoltNum = 0;

	int CodeRegister::OptInheritInfo=0;

//#endif	

	const double CodeRegister::EffelEffectFac = 0.3;

	double CodeRegister::EffelEffectRefHigh = 0.0;

	bool CodeRegister::bOverlapForWinWWA=false;

	//bool CodeRegister::NonCorFlag=true;

	int CodeRegister::NonlinearStep=5;


	int CodeRegister::structAnalyzeType=0;



	double CodeRegister::UnbalanceStop=175.0;

	double CodeRegister::UnbalanceWarning=50.0;

	bool CodeRegister::BeamFlag=false;


	void CodeRegister::setReadDefault()
	{
		CodeRegister::bOverlapForWinWWA = true;
		CodeRegister::BeamFlag = false;
		CodeRegister::setAllNodeFLoad = 0;
		CodeRegister::bPLSWinPanelSolver = false;
		CodeRegister::bDoubleAngleCorrection = 2;
		CodeRegister::bStatbleFacAStar = 0;
		CodeRegister::iSolveMode = 0;
		CodeRegister::MinHangingPointZ = -1000.0;
		CodeRegister::bCalcBoltNum = 0;
		CodeRegister::OptInheritInfo = 0;
	}
}
