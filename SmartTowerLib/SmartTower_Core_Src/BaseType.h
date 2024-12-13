//铁塔设计系统可能发出的窗口类型，
#pragma once
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月27日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	struct TowBaseObject
	{
		enum Type
		{
			UNDEFINE		=-1,
			TOWERPOINT      =0,
			PRIPOINT		=1,
			SECPOINT		=2, 
			XBSECPOINT	    =3,
			AUXPOINT        =4,
			NODE            =5,
			MEMBER          =6,
			ELEMENT         =7,
			BASEPART        =8,
			LEGPART         =9,
			WINCOND         =10, 
			WINPANEL        =11,
			NGPINFO         =12,
			DRAWFACE        =13,
			REDUNDANTDATA   =14,
			GUYSCABLE       =15,
			NodeLoadVec     =16,
			FDTDISPLOAD     =17,  
			SLTFACES         =50,
			SLTSUBFACE       =51,
			SLTSECTNUMB      =52,


			LOADRepartition       =20,
			NORMALLoadRp          =21,
			VInsulatorRp          =22,
			TriInsulator          =23,
			VTLARpt               =24,
			MergeLoad             =25,
			XYZRatioRpt           =26,
			CantileverBeamRpt      =27,

			NODELOAD2MODE        =30,
			NODELOADCASEMODE     =31,
			AuxDispSubLegForGL  =32,
			AuxRedundantFacePos =33,
			AuxRedundantsDataManager =34,

			HANGPLATEAPPEND = 40,//挂线板
			HANGBEAMAPPEND = 41,//挂线角钢
			FOOTPLATEAPPEND = 42,//地脚螺栓塔脚板
		};
	};		
};