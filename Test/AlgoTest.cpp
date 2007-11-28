#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/algorithm.h"
#include "rdestl/functional.h"
#include "rdestl/list.h"

namespace
{
	TEST(LowerBound)
	{
		const int array [] = { 1, 4, 9, 16, 25, 36 }; 
		const int* it = rdestl::lower_bound(&array[0], &array[6], 11, rdestl::less<int>());
		CHECK_EQUAL(16, *it);
		const int* it2 = &array[0];
		rdestl::less<int> pred;
		while (it2 != it)
		{
			CHECK(pred(*it2, *it));
			++it2;
		}
	}
	TEST(LowerBoundInputIter)
	{
		const int array [] = { 1, 4, 9, 16, 25, 36 }; 
		rdestl::list<int> lst(&array[0], &array[6]);
		rdestl::list<int>::iterator it = rdestl::lower_bound(lst.begin(), lst.end(), 11, rdestl::less<int>());
		CHECK_EQUAL(16, *it);
		rdestl::list<int>::iterator it2 = lst.begin();
		rdestl::less<int> pred;
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
		const int* it = rdestl::upper_bound(&array[0], &array[N], 5, rdestl::less<int>());
		CHECK_EQUAL(8, *it);
		it = rdestl::upper_bound(&array[0], &array[N], 6, rdestl::less<int>());
		CHECK_EQUAL(8, *it);
		it = rdestl::upper_bound(&array[0], &array[N], 2, rdestl::less<int>());
		const int* it2 = &array[0];
		rdestl::less<int> pred;
		while (it2 != it)
		{
			CHECK(!pred(2, *it2));
			++it2;
		}
	}
	TEST(Find)
	{
		const int array [] = { 1, 4, 9, 16, 25, 36 }; 
		const int* it = rdestl::find(&array[0], &array[6], 16);
		CHECK_EQUAL(it, &array[3]);
		CHECK_EQUAL(16, *it);
	}
	TEST(FindIf)
	{
		const int array [] = { 1, 4, 9, 16, 25, 36 }; 
		const int* it = rdestl::find(&array[0], &array[6], 16);
		CHECK_EQUAL(it, &array[3]);
		CHECK_EQUAL(16, *it);
	}
}
