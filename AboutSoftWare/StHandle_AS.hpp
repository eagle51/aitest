#pragma once
#include "SharedObject_AS.h"
#include "StHandle_AS.hpp"

namespace AboutSoftWare
{
	template<class T>
	class StHandle_AS
	{
	private:
		T* _px;

	public:
		StHandle_AS() :_px(0)
		{
		};
		StHandle_AS(T *p) :_px(p)
		{
			if (p != 0) // intrusive_ptr_add_ref(px);
				_px->AddReference();
		};
		StHandle_AS(const StHandle_AS &rhs) :_px(rhs._px)
		{
			if (_px != 0)  //intrusive_ptr_add_ref(px);
				_px->AddReference();
		};

		StHandle_AS(StHandle_AS &&rhs) :_px(rhs._px)
		{
			rhs._px = 0;//CXX11_RVALUE_REFERENCES
		};
		StHandle_AS &operator=(StHandle_AS &&rhs)
		{
			StHandle_AS(static_cast<StHandle_AS &&>(rhs)).swap(*this);
			return *this;
		};

		StHandle_AS &operator=(StHandle_AS const &rhs)
		{
			if (this != (&rhs))
			{
				if (_px) //intrusive_ptr_release(px);
					_px->RemoveReference();
				_px = rhs._px;
				if (_px) //intrusive_ptr_add_ref(px);
					_px->AddReference();
			}
			return *this;
		};
		StHandle_AS & operator=(T *rhs)
		{
			if (_px != rhs)//与自己相同的裸指针不能再给自己
			{
				if (_px) //intrusive_ptr_release(px);
					_px->RemoveReference();
				_px = rhs;
				if (_px) //intrusive_ptr_add_ref(px);
					_px->AddReference();
			}
			return *this;
		};
		void swap(StHandle_AS &rhs)
		{
			T * tmp = _px;
			_px = rhs._px;
			rhs._px = tmp;
		};
		void MakeCopy()
		{
			if (_px)
			{
				//自己复制自己
				T* New_px = new T(*_px);
				//intrusive_ptr_release(px);
				_px->RemoveReference();
				_px = New_px;
				if (_px) //intrusive_ptr_add_ref(px);
					_px->AddReference();
			}
		}
		T* operator->() const{ return _px; };
		T& operator*() const{ return *_px; };
		T* GetPointee() const{ return _px; };
		~StHandle_AS(void)
		{
			if (_px != 0) //intrusive_ptr_release(px);
				_px->RemoveReference();
		};

		bool isNull() const
		{
			return _px == 0;
		};
	public:
		bool operator==(const StHandle_AS& rhs) { return this->_px == rhs._px; }
		bool operator!=(const StHandle_AS& rhs) { return _px->operator!=(*(rhs._px)); }
		bool operator>(const StHandle_AS& rhs)const { return _px->operator>(rhs._px); }
		bool operator<(const StHandle_AS& rhs)const { return _px->operator<(rhs._px); }
	};
	typedef StHandle_AS<SharedObject_AS> HandleObject;
}