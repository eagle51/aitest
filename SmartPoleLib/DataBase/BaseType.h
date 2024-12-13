#pragma once

namespace SmartPoleCore
{
	struct PoleObject
	{
		enum Type
		{
			UNDEFINE = -1,
			VEC3D = 1,
			POINT = 2,
			NODE = 5,
			MEMBER = 6,
			ELEMENT = 7,
			COMPONENT = 8,

			FAMILYMANAGER = 9,
			WINCOND = 10,
			WINPANEL = 11,
			NGPINFO = 12,
			DRAWFACE = 13,
			REDUNDANTDATA = 14,
			GUYSCABLE = 15,
			NodeLoadVec = 16,
			FDTDISPLOAD = 17,

			LOADRepartition = 20,
			NORMALLoadRp = 21,
			VInsulatorRp = 22,
			TriInsulator = 23,
			VTLARpt = 24,
			MergeLoad = 25,
			XYZRatioRpt = 26,
			CantileverBeamRpt = 27,

			NODELOAD2MODE = 30,
			NODELOADCASEMODE = 31,

			SinglePoleResult =40,

		};
	};		
};