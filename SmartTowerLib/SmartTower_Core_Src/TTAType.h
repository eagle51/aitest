#pragma once
#include "TTAFormatIO.h"
#include "XBTTAPFormatIO.h"
#include "XBSecondaryPoint.h"
#include "ISym.h"
namespace SmartTower
{
	//根据TTA的类型配置不同的设置
	class CTTAType
	{
	public:
		enum TTATp
		{
			DBTTA=0,
			XBTTA=1,
		};		
		TowBaseObject::Type ActiveSecType() const 
		{ 
			return _ActiveSecType; 
		}
		void ActiveSecType(TowBaseObject::Type val)
		{  
			_ActiveSecType = val; 
		}
		TTATp getType() const	{	return _Type;	}
		void Type(TTATp val);
		HandleModelIO getTowerModelIO(CTowerModel* Model);
		static CTTAType* Instance() { return _Instance; }
	private:
		TowBaseObject::Type _ActiveSecType;		
		TTATp _Type;	
		CTTAType(void);
		~CTTAType(void);
		static CTTAType* _Instance;		
	};
}
