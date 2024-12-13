#pragma once
#include "..\Utility\SharedHandle.hpp"
namespace SmartPoleCore
{
	//简单整型数组，程序里面不直接使用Vector<int>
	class IntVector:public SharedObject
	{
	public:
		typedef int* iterator;
		typedef const int* const_iterator;
		IntVector();
		IntVector(const IntVector &rhs);
		const IntVector& operator=(const IntVector& rhs);
		void reserve(int newCapacity);
		void zeros(int size);
		int& operator[](int index)
		{
			return _objects[index];
		}
		const int& operator[](int index) const
		{
			return _objects[index];
		}
		int at(int Index) const
		{
			return _objects[Index];
		}
		int getVal(int Index) const
		{
			if (Index >= _theSize)
				return 0;
			return _objects[Index];
		}
		//等于x的最多个数
		int equalCount(int x);
		//查找x的位置，找不到-1
		int find(int x) const;
		//查找大于x的第一个元素
		int nextObject(int x,int StartIndex) const;
		bool empty() const
		{
			return size() == 0;
		}
		int size() const
		{
			return _theSize;
		}
		int capacity() const
		{
			return _theCapacity;
		}
		void insert(int Index,const int& x);
		void push_back(const int& x)
		{
			if (_theSize == _theCapacity)
				reserve(2*_theCapacity+1);
			_objects[_theSize++] = x;
		}
		void pop_back()
		{
			_theSize--;
		}
		void clear()
		{
			_theSize=0;
		}
		//Index从0开始
		void erase(int Index);
		const int & back() const 
		{
			return _objects[_theSize-1];
		}		
		iterator begin()
		{
			return &_objects[0];
		}
		const_iterator begin() const
		{
			return &_objects[0];
		}
		iterator end()
		{
			return &_objects[_theSize];
		}
		const_iterator end() const
		{
			return &_objects[_theSize];
		}		
		//两个数组相同，可以顺序不同
		bool equal(const IntVector& rhs) const;
		//非零元素
		int numOfNonzero() const;		//
		//全部非零
		bool isAllZeros() const;
		//最大有效元素
		int maxEffectNodeNum() const;
		//最小有限元素
		int minEffectNodeNum() const;
		//检查是否存在重复的数字
		int checkDoublication() const;
		//反转
		void reverseVec() const;

		~IntVector(void);
	private:		
		static int step;
		int _theSize;
		int _theCapacity;
		int* _objects;
	};
	typedef SharedHandle<IntVector> HandleIntVec;
}