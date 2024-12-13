#pragma once
#include <exception>
#include "SharedObject.h"
#include "StHandle.hpp"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{
	//简单对象的数组，没有析构，所以要注意
	//整型数组
	class IntVector:public SharedObject
	{
	public: 
		typedef int Object;
		typedef Object * iterator;
		typedef const Object* const_iterator;

		IntVector();
		IntVector(const IntVector &rhs);
#ifdef C11Input
		IntVector(IntVector &&rhs);
#endif
		const IntVector& operator=(const IntVector& rhs);
		void reserve(int newCapacity);
		void zeros(int size);
		Object& operator[](int index)
		{
#ifdef _DEBUG
			if(index>=theSize || index<0)
				throw std::exception("exceed range");
#endif
			return objects[index];
		}
		const Object& operator[](int index) const
		{
#ifdef _DEBUG
			if(index>=theSize || index<0)
				throw std::exception("exceed range");
#endif
			return objects[index];
		}
		
		int at(int Index) const
		{
#ifdef _DEBUG
			if(Index>=theSize || Index<0)
				throw std::exception("exceed range");
#endif
			return objects[Index];
		}
		int getVal(int Index) const
		{
			if(Index>=theSize)
				return 0;
			return objects[Index];
		}
		int equalCount(const Object& x);
		int find(const Object& x) const;
		int NextObject(const Object& x,int StartIndex) const;

		bool empty() const
		{
			return size() == 0;
		}
		int size() const
		{
			return theSize;
		}
		int capacity() const
		{
			return theCapacity;
		}
		void insert(int Index,const Object& x);
		void push_back(const Object& x)
		{
			if (theSize == theCapacity)
				reserve(2*theCapacity+1);
			objects[theSize++] = x;
		}
		void pop_back()
		{
			theSize--;
		}
		void clear()
		{
			theSize=0;
		}
		//Index从0开始
		void erase(int Index);
		const Object & back() const 
		{
			return objects[theSize-1];
		}		
		iterator begin()
		{
			return &objects[0];
		}
		const_iterator begin() const
		{
			return &objects[0];
		}
		iterator end()
		{
			return &objects[theSize];
		}
		const_iterator end() const
		{
			return &objects[theSize];
		}		
		bool equal(const IntVector& rhs) const;
		//非零元素的个数
		int NumOfNonzero() const;		//
		bool isAllZeros() const;
		int MaxEffectNodeNum() const;
		int MinEffectNodeNum() const;
		//检查是否存在重复节点号,不存在返回-1，存在反回节点号
		int CheckDoublication() const;
		void ReverseVec() const;
		~IntVector(void);
	private:		
		static int step;
		int theSize;
		int theCapacity;
		Object* objects;
	};
}