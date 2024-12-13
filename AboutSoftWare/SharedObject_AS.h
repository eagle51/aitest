#pragma once

namespace AboutSoftWare
{
	class SharedObject_AS
	{
	private:
		mutable int _refCount;

	public:
		SharedObject_AS() :_refCount(0){};//shareable(true)
		SharedObject_AS(const SharedObject_AS&) :_refCount(0){};//,shareable(true)
		SharedObject_AS& operator=(const SharedObject_AS& rhs){ return *this; };
		virtual ~SharedObject_AS(void) = 0; //析构函数需要用虚函数，避免泄露

		void AddReference(){ ++_refCount; };
		void RemoveReference(){ if (--_refCount == 0) delete this; }
		int GetRefCount(){ return _refCount; }

	};
}