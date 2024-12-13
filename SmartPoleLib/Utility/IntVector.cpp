//#include "StdAfx.h"
#include "IntVector.h"

namespace SmartPoleCore
{

	IntVector::IntVector()
		: _theSize(0),_theCapacity(4),_objects(0)
	{
		 _objects = new int[_theCapacity];
	}

	IntVector::IntVector( const IntVector &rhs )
		: _theSize(rhs._theSize), _theCapacity(rhs._theCapacity)
	{
		_objects = new int[_theCapacity];
		for (int loop=0;loop<_theSize;loop++)
			_objects[loop]=rhs._objects[loop];
	}

	IntVector::~IntVector(void)
	{
		delete [] _objects;		
	}

	const IntVector & IntVector::operator=( const IntVector & rhs )
	{
		if (_objects != rhs._objects)
		{
			delete [] _objects;
			_theSize = rhs._theSize;
			_theCapacity = rhs._theCapacity;
			_objects = new int[_theCapacity];
			for (int loop=0;loop<_theSize;loop++)
				_objects[loop]=rhs._objects[loop];
		}
		return *this;
	}

	void IntVector::reserve( int newCapacity )
	{
		if (newCapacity <= _theCapacity)
			return;
		int *oldArray = _objects;
		_objects = new int[newCapacity];
		for (int k = 0; k < _theSize; k++)
			_objects[k] = oldArray[k];
		_theCapacity = newCapacity;
		delete oldArray;	
	}
	void IntVector::zeros( int size )
	{
		reserve(size);
		_theSize=size;
		for (int k = 0; k < _theSize; k++)
			_objects[k] = 0;
	}
	void IntVector::erase( int Index )
	{
		for (int loop=Index+1;loop<_theSize;loop++)		
			_objects[loop-1]=_objects[loop];
		_theSize--;
	}

	void IntVector::insert(int Index,const int& x)
	{
		if ( Index < 0 || Index > _theSize )
			return;

		if ( _theCapacity <= _theSize ) 
		{
			int* old_items = _objects;
			_theCapacity *= step;
			if ( _theCapacity < 4 ) 
				_theCapacity = 4;
			_objects = new int[ _theCapacity ];
			for (int loop=0;loop<Index;loop++)
				_objects[loop]=old_items[loop];
			for (int loop=Index;loop<_theSize;loop++)
				_objects[loop+1]=old_items[loop];
			delete [] old_items;
		} 
		else 
		{
			for ( int i = _theSize; i > Index; i-- ) 
				_objects[i]=_objects[i-1];
		}
		_theSize++;
		_objects[Index]=x;
			
	}

	int IntVector::checkDoublication() const
	{
		for (int Iter=0;Iter<_theSize;Iter++)
		{
			for (int Iter2=Iter+1;Iter2<_theSize;Iter2++)
			{
				if(_objects[Iter]==_objects[Iter2])
					return _objects[Iter];
			}
		}
		return -1;
	}

	int IntVector::numOfNonzero() const
	{
		int res=0;
		for (int Iter=0;Iter<_theSize;Iter++)
		{
			if (_objects[Iter]!=0)
				++res;
		}
		return res;
	}

	bool IntVector::isAllZeros() const
	{
		for (int Iter=0;Iter<_theSize;Iter++)
		{
			if (_objects[Iter]!=0)
				return false;
		}
		return true;
	}

	int IntVector::maxEffectNodeNum() const
	{
		int res=9;
		if (_theSize>0)
			res=_objects[0];
		for (int Iter=1;Iter<_theSize;Iter++)
		{
			if (_objects[Iter]>res)
				res=_objects[Iter];
		}
		return res;
	}
	int IntVector::minEffectNodeNum() const
	{
		int res=0;
		if (_theSize>0)
			res=_objects[0];
		for (int Iter=1;Iter<_theSize;Iter++)
		{
			if (_objects[Iter]<res && _objects[Iter]>9)
				res=_objects[Iter];
		}
		return res;
	}

	int IntVector::find( int x ) const
	{
		for (int Iter=0;Iter<_theSize;Iter++)
		{
			if (_objects[Iter]==x)
				return Iter;
		}
		return -1;
	}

	int IntVector::equalCount(int x )
	{
		int rescount=0;
		for (int Iter=0;Iter<_theSize;Iter++)
		{
			if (_objects[Iter]==x)
				++rescount;
		}
		return rescount;
	}
	int IntVector::nextObject(int x,int StartIndex ) const
	{
		for (int Iter=StartIndex;Iter<_theSize;Iter++)
		{
			if (_objects[Iter]==x)
				return Iter;
		}
		return -1;
	}
	void IntVector::reverseVec() const
	{
		int MaxIndex=_theSize/2;
		for (int iloop=0;iloop<MaxIndex;++iloop)
		{
			int itemp=_objects[iloop];
			_objects[iloop]=_objects[_theSize-iloop-1];
			_objects[_theSize-iloop-1]=itemp;
		}
	}

	bool IntVector::equal(const IntVector& rhs) const
	{
		if(this->size()!=rhs.size())
			return false;
		for(int iloop=0;iloop<this->size();iloop++)
		{
			bool bEqual=false;
			int curNum=this->at(iloop);
			for(int iloop2=0;iloop2<rhs.size();iloop2++)
			{
				if(rhs.at(iloop2)==curNum)
				{
					bEqual=true;
					break;
				}
			}
			if(!bEqual)
				return false;
		}
		return true;
	}


	int IntVector::step=2;

}