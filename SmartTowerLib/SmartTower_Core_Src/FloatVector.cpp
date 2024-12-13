//#include "StdAfx.h"
#include "FloatVector.h"
#include <complex>

//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{ 

	FloatVector::FloatVector(): 
	theSize(0),theCapacity(1),objects(0)
	{
		 objects = new Object[theCapacity];
		 objects[0]=0.00000000;
	}

	FloatVector::FloatVector( const FloatVector &rhs ): 
	theSize(rhs.theSize), theCapacity(rhs.theCapacity)
	{
		objects = new Object[theCapacity];
		for (int loop=0;loop<theSize;loop++)
			objects[loop]=rhs.objects[loop];
	}
#ifdef C11Input
	FloatVector::FloatVector(FloatVector &&rhs)
	{
		theSize=rhs.theSize;
		theCapacity=rhs.theCapacity;
		objects=rhs.objects;
		rhs.theSize=0;
		rhs.theCapacity=0;
		rhs.objects=0;
	}
#endif

	FloatVector::~FloatVector(void)
	{
		delete [] objects;		
	}

	const FloatVector & FloatVector::operator=( const FloatVector & rhs )
	{
		if (objects != rhs.objects)
		{
			delete [] objects;
			theSize = rhs.theSize;
			theCapacity = rhs.theCapacity;
			objects = new Object[theCapacity];
			for (int loop=0;loop<theSize;loop++)
				objects[loop]=rhs.objects[loop];
		}
		return *this;
	}

	void FloatVector::reserve( int newCapacity )
	{
		if (newCapacity <= theCapacity)
			return;
		Object *oldArray = objects;
		objects = new Object[newCapacity];
		for (int k = 0; k < theSize; k++)
			objects[k] = oldArray[k];
		if (theCapacity > 0)
			delete oldArray;
		theCapacity = newCapacity;
	}
	void FloatVector::zeros( int size )
	{
		reserve(size);
		theSize=size;
		for (int k = 0; k < theSize; k++)
			objects[k] = 0;
	}
	void FloatVector::erase( int Index )
	{
		for (int loop=Index+1;loop<theSize;loop++)		
			objects[loop-1]=objects[loop];
		theSize--;
	}

	void FloatVector::insert(int Index,const Object& x)
	{
		if ( Index < 0 || Index > theSize )
			return;

		if ( theCapacity <= theSize ) 
		{
			Object* old_items = objects;
			theCapacity *= step;
			if ( theCapacity < 4 ) 
				theCapacity = 4;
			objects = new Object[ theCapacity ];
			for (int loop=0;loop<Index;loop++)
				objects[loop]=old_items[loop];
			for (int loop=Index;loop<theSize;loop++)
				objects[loop+1]=old_items[loop];
			delete [] old_items;
		} 
		else 
		{
			for ( int i = theSize; i > Index; i-- ) 
				objects[i]=objects[i-1];
		}
		theSize++;
		objects[Index]=x;
			
	}

	double FloatVector::minCoeff() const
	{
		double Result=objects[0];
		for (int k = 0; k < theSize; k++)
			if(Result>objects[k])
				Result=objects[k];
		return Result;
	}

	double FloatVector::maxCoeff() const
	{
		double result=objects[0];
		for (int k = 0; k < theSize; k++)
			if(result<objects[k])
				result=objects[k];
		return result;
	}

	int FloatVector::find(const Object& x, double esp /*= 0.000001*/)
	{
		for (int k = 0; k < theSize; k++)
		{
			if (std::abs(objects[k] - x) < esp)
				return step;
		}
		return -1;
	}

	int FloatVector::step = 2;

}