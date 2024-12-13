#pragma once
namespace SmartTower
{
	class Version
	{
	public:
		enum Type
		{
			VErr = 0,
			V322 = 322,
			V400 = 400,
			V401 = 401,
			V419 = 419,
			V420 = 420,
			V500 = 500,
		};
		enum StType
		{ 
			V1000   =   1000,
			V201907 = 201907,
			V202302 = 202302,
			V202305 = 202305,
		};
		static Type Creator(int tag)
		{
			switch(tag)
			{
			case 322:
				return V322;
			case 400:
				return V400;
			case 401:
				return V401;
			case 419:
				return V419;
			case 420:
				return V420;
			case 500:
				return V500;
			default:
				return VErr;
			}
		}
	};

}