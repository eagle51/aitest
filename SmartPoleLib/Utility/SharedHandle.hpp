#pragma once
#include "SharedObject.h"
//#define C11Input 
namespace SmartPoleCore
{
	template<class T>
	class SharedHandle
	{
	private:
		T* _px;
	public:
		SharedHandle():_px(0)
		{
		};
		SharedHandle(T *p):_px(p)
		{
			if(p!=0) // intrusive_ptr_add_ref(px);
				_px->AddReference();
		};
		SharedHandle(const SharedHandle &rhs):_px(rhs._px)
		{	
			if(_px!=0)  //intrusive_ptr_add_ref(px);
				_px->AddReference();
		};
#ifdef C11Input
		SharedHandle(SharedHandle &&rhs) :_px(rhs._px)
		{
			rhs._px = 0;//CXX11_RVALUE_REFERENCES
		};
		SharedHandle &operator=(SharedHandle &&rhs)
		{
			SharedHandle(static_cast<SharedHandle &&>(rhs)).Swap(*this);
			return *this;
		};
#endif
		SharedHandle &operator=(const SharedHandle  &rhs)
		{
			if(this!=(&rhs))
			{
				if(_px) //intrusive_ptr_release(px);
					_px->RemoveReference();
				_px=rhs._px;
				if(_px) //intrusive_ptr_add_ref(px);
					_px->AddReference();
			}
			return *this;
		};
		SharedHandle & operator=(T *rhs)
		{
			if(_px!=rhs)//与自己相同的裸指针不能再给自己
			{
				if(_px) //intrusive_ptr_release(px);
					_px->RemoveReference();
				_px=rhs;
				if(_px) //intrusive_ptr_add_ref(px);
					_px->AddReference();
			}
			return *this;
		};
		void Swap(SharedHandle &rhs)
		{
			T * tmp = _px;
			_px = rhs._px;
			rhs._px = tmp;
		};
		void MakeCopy()
		{
			if(_px)
			{
				//自己复制自己
				T* New_px=new T(*_px);
				//intrusive_ptr_release(px);
				_px->RemoveReference();
				_px=New_px;
				if(_px) //intrusive_ptr_add_ref(px);
					_px->AddReference();
			}
		}
		T* operator->() const{return _px;};
		T& operator*() const{return *_px;};
		T* GetPointee() const{return _px;};
		~SharedHandle(void)
		{
			if( _px != 0 ) //intrusive_ptr_release(px);
				_px->RemoveReference();
		};
		
		bool isNull() const
		{
			return _px==0;
		};
	};
	typedef SharedHandle<SharedObject> HandleObject;
}