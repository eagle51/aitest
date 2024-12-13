#ifndef _STObject_H_
#define _STObject_H_
#include "SharedObject.h"

namespace SmartTower
{
	
	template<class T>
	class StHandle
	{
	private:
		T* px;
	public:
		StHandle():px(0)
		{ 
		};
		StHandle(T *p):px(p)
		{
			if(p!=0) // intrusive_ptr_add_ref(px);
				px->AddReference();
		};
		StHandle(const StHandle &rhs):px(rhs.px)
		{	if(px!=0)  //intrusive_ptr_add_ref(px);
				px->AddReference();
		};
#ifdef C11Input
		StHandle(StHandle &&rhs):px(rhs.px)
		{	
			rhs.px=0;//CXX11_RVALUE_REFERENCES
		};
		StHandle &operator=(StHandle &&rhs)
		{
			StHandle( static_cast<StHandle &&>( rhs ) ).swap(*this);
			return *this;
		};
#endif
		StHandle &operator=(StHandle const &rhs)
		{
			if(this!=(&rhs))
			{
				if(px) //intrusive_ptr_release(px);
					px->RemoveReference();
				px=rhs.px;
				if(px) //intrusive_ptr_add_ref(px);
					px->AddReference();
			}
			return *this;
		};
		StHandle & operator=(T *rhs)
		{
			if(px!=rhs)//与自己相同的裸指针不能再给自己
			{
				if(px) //intrusive_ptr_release(px);
					px->RemoveReference();
				px=rhs;
				if(px) //intrusive_ptr_add_ref(px);
					px->AddReference();
			}
			return *this;
		};
		void swap(StHandle &rhs)
		{
			T * tmp = px;
			px = rhs.px;
			rhs.px = tmp;
		};
		void MakeCopy()
		{
			if(px)
			{
				//自己复制自己
				T* New_px=new T(*px);
				//intrusive_ptr_release(px);
				px->RemoveReference();
				px=New_px;
				if(px) //intrusive_ptr_add_ref(px);
					px->AddReference();
			}
		}
		T* operator->() const{return px;};
		T& operator*() const{return *px;};
		T* GetPointee() const{return px;};
		~StHandle(void)
		{
			if( px != 0 ) //intrusive_ptr_release(px);
				px->RemoveReference();
		};
		
		bool isNull() const
		{
			return px==0;
		};
	};
	typedef StHandle<SharedObject> HandleObject;
	
}
#endif  