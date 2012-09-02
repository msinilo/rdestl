#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/algorithm.h"
#include "rdestl/functional.h"
#include "rdestl/list.h"

namespace
{
	TEST(LowerBound)
	{
		const int array [] = { 1, 4, 9, 16, 25, 36 }; 
		const int* it = rde::lower_bound(&array[0], &array[6], 11, rde::less<int>());
		CHECK_EQUAL(16, *it);
		const int* it2 = &array[0];
		rde::less<int> pred;
		while (it2 != it)
		{
			CHECK(pred(*it2, *it));
			++it2;
		}
	}
	TEST(LowerBoundInputIter)
	{
		const int array [] = { 1, 4, 9, 16, 25, 36 }; 
		rde::list<int> lst(&array[0], &array[6]);
		rde::list<int>::iterator it = rde::lower_bound(lst.begin(), lst.end(), 11, rde::less<int>());
		CHECK_EQUAL(16, *it);
		rde::list<int>::iterator it2 = lst.begin();
		rde::less<int> pred;
		while (it2 != it)
		{
			CHECK(pred(*it2, *it));
			++it2;
		}
	}
	TEST(UpperBound)
	{
		const int array[] = { 1, 2, 3, 3, 3, 5, 8 };
		const int N = sizeof(array) / sizeof(int);
		const int* it = rde::upper_bound(&array[0], &array[N], 5, rde::less<int>());
		CHECK_EQUAL(8, *it);
		it = rde::upper_bound(&array[0], &array[N], 6, rde::less<int>());
		CHECK_EQUAL(8, *it);
		it = rde::upper_bound(&array[0], &array[N], 2, rde::less<int>());
		const int* it2 = &array[0];
		rde::less<int> pred;
		while (it2 != it)
		{
			CHECK(!pred(2, *it2));
			++it2;
		}
	}
	TEST(Find)
	{
		const int array [] = { 1, 4, 9, 16, 25, 36 }; 
		const int* it = rde::find(&array[0], &array[6], 16);
		CHECK_EQUAL(it, &array[3]);
		CHECK_EQUAL(16, *it);
	}
	TEST(FindIf)
	{
		const int array [] = { 1, 4, 9, 16, 25, 36 }; 
		const int* it = rde::find_if(&array[0], &array[6], 16, rde::equal_to<int>());
		CHECK_EQUAL(it, &array[3]);
		CHECK_EQUAL(16, *it);
	}
	TEST(AbsGeneric)
	{
		CHECK_EQUAL(5.f, rde::abs(5.f));
		CHECK_EQUAL(5.f, rde::abs(-5.f));
		CHECK_EQUAL(0.f, rde::abs(0.f));
	}
	TEST(AbsInt)
	{
		CHECK_EQUAL(5, rde::abs(5));
		CHECK_EQUAL(5, rde::abs(-5));
		CHECK_EQUAL(0, rde::abs(0));
	}
	TEST(AbsShort)
	{
		CHECK_EQUAL(5, rde::abs<short>(5));
		short x(-5);
		CHECK_EQUAL(5, rde::abs(x));
		x = 0;
		CHECK_EQUAL(0, rde::abs(x));
	}
	TEST(MinFloat)
	{
		CHECK_EQUAL(2.f, rde::min(2.f, 3.f));
	}
	// http://code.google.com/p/rdestl/issues/detail?id=7
	TEST(UnsignedIntsHaveTrivialCopy)
	{
		CHECK_EQUAL(1, rde::has_trivial_copy<unsigned char>::value);
		CHECK_EQUAL(1, rde::has_trivial_copy<unsigned short>::value);
		CHECK_EQUAL(1, rde::has_trivial_copy<unsigned int>::value);
		CHECK_EQUAL(1, rde::has_trivial_copy<unsigned long>::value);
	}
}
