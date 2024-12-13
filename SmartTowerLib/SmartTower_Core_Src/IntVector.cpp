//#include "StdAfx.h"
#include "IntVector.h"
//////////////////////////////////////////
/////////////////////////////////////////
///该类已于2022年8月28日重构校队完成,李辉
///产权归陕西恒巨软件科技有限公司所有
/////////////////////////////////////
/////////////////////////////////////
namespace SmartTower
{

	IntVector::IntVector(): 
	theSize(0),theCapacity(4),objects(0)
	{
		 objects = new Object[theCapacity]; 
	}

	IntVector::IntVector( const IntVector &rhs ): 
	theSize(rhs.theSize), theCapacity(rhs.theCapacity)
	{
		objects = new Object[theCapacity];
		for (int loop=0;loop<theSize;loop++)
			objects[loop]=rhs.objects[loop];
	}
#ifdef C11Input
	IntVector::IntVector(IntVector &&rhs)
	{
		theSize=rhs.theSize;
		theCapacity=rhs.theCapacity;
		objects=rhs.objects;
		rhs.theSize=0;
		rhs.theCapacity=0;
		rhs.objects=0;
	}
#endif

	IntVector::~IntVector(void)
	{
		delete [] objects;		
	}

	const IntVector & IntVector::operator=( const IntVector & rhs )
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

	void IntVector::reserve( int newCapacity )
	{
		if (newCapacity <= theCapacity)
			return;
		Object *oldArray = objects;
		objects = new Object[newCapacity];
		for (int k = 0; k < theSize; k++)
			objects[k] = oldArray[k];
		theCapacity = newCapacity;
		delete oldArray;	
	}
	void IntVector::zeros( int size )
	{
		reserve(size);
		theSize=size;
		for (int k = 0; k < theSize; k++)
			objects[k] = 0;
	}
	void IntVector::erase( int Index )
	{
		for (int loop=Index+1;loop<theSize;loop++)		
			objects[loop-1]=objects[loop];
		theSize--;
	}

	void IntVector::insert(int Index,const Object& x)
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

	int IntVector::CheckDoublication() const
	{
		for (int Iter=0;Iter<theSize;Iter++)
		{
			for (int Iter2=Iter+1;Iter2<theSize;Iter2++)
			{
				if(objects[Iter]==objects[Iter2])
					return objects[Iter];
			}
		}
		return -1;
	}

	int IntVector::NumOfNonzero() const
	{
		int res=0;
		for (int Iter=0;Iter<theSize;Iter++)
		{
			if (objects[Iter]!=0)
				++res;
		}
		return res;
	}

	bool IntVector::isAllZeros() const
	{
		for (int Iter=0;Iter<theSize;Iter++)
		{
			if (objects[Iter]!=0)
				return false;
		}
		return true;
	}

	int IntVector::MaxEffectNodeNum() const
	{
		int res=9;
		if (theSize>0)
			res=objects[0];
		for (int Iter=1;Iter<theSize;Iter++)
		{
			if (objects[Iter]>res)
				res=objects[Iter];
		}
		return res;
	}
	int IntVector::MinEffectNodeNum() const
	{
		int res=0;
		if (theSize>0)
			res=objects[0];
		for (int Iter=1;Iter<theSize;Iter++)
		{
			if (objects[Iter]<res && objects[Iter]>9)
				res=objects[Iter];
		}
		return res;
	}

	int IntVector::find( const Object& x ) const
	{
		for (int Iter=0;Iter<theSize;Iter++)
		{
			if (objects[Iter]==x)
				return Iter;
		}
		return -1;
	}

	int IntVector::equalCount( const Object& x )
	{
		int rescount=0;
		for (int Iter=0;Iter<theSize;Iter++)
		{
			if (objects[Iter]==x)
				++rescount;
		}
		return rescount;
	}
	int IntVector::NextObject( const Object& x,int StartIndex ) const
	{
		for (int Iter=StartIndex;Iter<theSize;Iter++)
		{
			if (objects[Iter]==x)
				return Iter;
		}
		return -1;
	}
	void IntVector::ReverseVec() const
	{
		int MaxIndex=theSize/2;
		for (int iloop=0;iloop<MaxIndex;++iloop)
		{
			int itemp=objects[iloop];
			objects[iloop]=objects[theSize-iloop-1];
			objects[theSize-iloop-1]=itemp;
		}
	}

	bool IntVector::equal(const IntVector& rhs) const
	{
		if(this->size()!=rhs.size())
			return false;
		for(int iloop=0;iloop<this->size();iloop++)
		{
			bool bEqual=false;
			int CurNum=this->at(iloop);
			for(int iloop2=0;iloop2<rhs.size();iloop2++)
			{
				if(rhs.at(iloop2)==CurNum)
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