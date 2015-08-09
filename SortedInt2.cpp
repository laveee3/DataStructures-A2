// FILE: SortedInt2.cpp
//       Implementation file for the SortedInt2 class
//       (See SortedInt2.h for documentation.)
// INVARIANT for the SortedInt2 class:
// (1) Entries of the SortedInt2 are stored in a one-dimensional,
//     run-time (dynamic) array of integers referenced by the
//     member variable data (i.e., the starting address of the
//     the array is stored in data); the capacity of the array
//     is stored in the member variable capacity.
// (2) # of entries the SortedInt2 currently contains is stored
//     in the member variable used.
// (3) Entries of SortedInt2 are stored sorted in non-decreasing
//     order from data[0] through data[used - 1].
// (4) We DON'T care what is stored in any of the array elements
//     from data[used] through data[capacity - 1].
//
// DOCUMENTATION for private member (helper) function:
//   void resize(int new_capacity)
//     Pre:  (none)
//     Post: The capacity (size of the dynamic array) of the
//           invoking SortedInt2 is changed to new_capacity. The
//           collection represented by the invoking SortedInt2
//           remains unchanged.
//     Note: For new capacity to be valid, it should not be less
//           than used (so that the existing collection can still
//           be represented) AND should be at least 1 (0 is not
//           good because we don't want to request a dynamic array
//           of size 0). Caller-supplied new_capacity is checked
//           and adjusted (where necessary) to ensure valid in two
//           stages as follows:
//           Stage 1: set new_capacity to used if it is < used
//           Stage 2: if new_capacity < 1, set it to 1
//     Xtra: With the above, resize can actually be used to shrink
//           the size of the dynamic array (to either used or 1,
//           whichever is the greater) used by a SortedInt2; for
//           such a case, one can simply call resize with any
//           invalid new_capacity (say 0). However, using resize
//           as such should be limited to situations where dynamic
//           memory usage is critical (because resizing is really
//           a time-consuming operation).
//

#include "SortedInt2.h"
#include <iostream>
#include <cassert>

using namespace std;

void SortedInt2::resize(int new_capacity)
{
	if (new_capacity < used)
		new_capacity = used;

	if (new_capacity < 1)
		new_capacity = 1;

	int *larger_array = new int[new_capacity];
	for ( int i = 0; i < used; i++)
		larger_array[i] = data[i];
	
	delete [] data;
	data = larger_array;
	capacity = new_capacity;
//	delete [] larger_array;
}

SortedInt2::SortedInt2(int init_capacity):capacity(init_capacity),used(0)
{
	if (init_capacity < 1)
		init_capacity = DEFAULT_CAPACITY;
	data = new int[init_capacity];
}

SortedInt2::SortedInt2(const SortedInt2& src):capacity(src.capacity), used(src.used)
{
    data = new int[src.capacity];
	copy(src.data, src.data + used, data);
}

SortedInt2::~SortedInt2()
{
  	delete [] data;
}

bool SortedInt2::isEmpty() const
{
    if ( used == 0)
        return true;
    else
        return false;
}

int SortedInt2::size() const
{
   return used;
}

int SortedInt2::valAt(int position) const
{
   assert (size() != 0);
   assert (position >= 1); 
   assert (position <= size());
	
   return data[position-1];
	
}

int SortedInt2::findMin() const
{
   assert (size() != 0);

   return data[0];
}

int SortedInt2::findMax() const
{
   assert (size() != 0);

   return data[used - 1];
}

double SortedInt2::findMed() const
{
	assert (size() != 0);

	if ((used % 2) == 0)
		return ((data[used / 2] + data [(used / 2) - 1]) * 0.5);
	else
		return data[used / 2];
}

double SortedInt2::findAvg() const
{
	assert (size() != 0);

	double sum = 0;
	for (int  i = 0; i < used; i++)
		sum = sum + data[i];
	return (sum/used);
}

int SortedInt2::findFreq(int target) const
{
	int freq = 0;
	for (int  i = 0; i < used; i++)
	{
		if (data[i] == target)
		freq = freq + 1;
	}
	return freq;
}

SortedInt2& SortedInt2::operator=(const SortedInt2& rhs)
{
   int *new_data;
	
	if (this == &rhs)
    return *this;
	
	if (capacity != rhs.capacity)
	{
		new_data = new int[rhs.capacity];
		delete [] data;
		data = new_data;
		capacity = rhs.capacity;
	}
	
//	delete [] new_data;	
	used = rhs.used;
	copy(rhs.data, rhs.data + used, data);
	return *this;
}

void SortedInt2::reset()
{
    used = 0;
}


void SortedInt2::insert(int newInt)
{
	if (used == capacity)
		resize((capacity * 1.5) + 1);

	int i;
	for ( i = used; i > 0 && (data[i-1] > newInt); --i)
	{
		data[i] = data[i-1];
	}
	data[i] = newInt;
	used++;
}

int SortedInt2::delMin()
{
    assert (size() != 0);

	int min = SortedInt2::findMin();
	int freq = SortedInt2::findFreq(min);

	for (int i = 0; i < (used - freq); i++)
	{
		data[i] = data[freq + i];
	}	
	used = used - freq;
	return freq;
}

int SortedInt2::delMax()
{
    assert (size() != 0);

	int max = SortedInt2::findMax();
	int freq = SortedInt2::findFreq(max);
	
	used = used - freq;
	return freq;
}


bool SortedInt2::delOne(int target)
{
    int posi = -1;
    for (int i = 0; i < used; i++)
    {
        if (data[i] == target)
				posi = i;        
    }

	if (posi != -1)
	{	
		for (int i = posi; i < used; i++)
		{
			data[i] = data[i+1];	
		}
		used--;
        return true;
	}	
    else
        return false;
}

int SortedInt2::delAll(int target)
{
   int freq = SortedInt2::findFreq(target);
   if (freq >= 1)
	{
		for (int i = 0; i < used; i++)
		{	
			if (data[i] == target)
			{
				for (; i < (used - freq); i++)
					data[i] = data[freq + i];				
			}
		}
		used = used - freq;
		return freq;
	}
	else
		return 0;
}


void SortedInt2::operator+=(SortedInt2 addend)
{
	for ( int i = 0; i < addend.size(); i++)
		insert(addend.data[i]);
}

bool operator==(const SortedInt2& lhs, const SortedInt2& rhs)
{
	if (lhs.size() == rhs.size())
	{
		for (int i = 0; i < lhs.used; i++)
		{
			if (lhs.data[i] != rhs.data[i])
				return false;
			else {}
		}	
		return true;		
	}
	else
		return false;
}

SortedInt2 operator+(const SortedInt2& s1, const SortedInt2& s2)
{
    SortedInt2 s3;

    int first = s1.size();
    int second = s2.size();
    for (int i = 1; i <= first; i++)
    {
	    s3.insert(s1.valAt(i));
    }

    for (int i = 1; i <= second; i++)
	{
	    s3.insert(s2.valAt(i));
    }

    return s3;

}
