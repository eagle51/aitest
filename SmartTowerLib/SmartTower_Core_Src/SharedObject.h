#pragma once
#include "BaseType.h"
namespace SmartTower
{
	//STObject
	class SharedObject 
	{
	private:
		mutable int refCount;
	public:
		SharedObject():refCount(0){};//shareable(true)
		SharedObject(const SharedObject&):refCount(0){};//,shareable(true)
		SharedObject& operator=(const SharedObject& rhs){return *this;};
		virtual ~SharedObject(void)=0; //析构函数需要用虚函数，避免泄露
		void AddReference(){++refCount;};
		void RemoveReference(){	if (--refCount==0) delete this;}
		int GetRefCount(){return refCount;}	
		
		virtual TowBaseObject::Type GetObjType() const
		{
			return TowBaseObject::UNDEFINE;
		} 
	};
}