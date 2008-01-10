#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/fixed_array.h"
#include <cstdio>

namespace
{
	TEST(Size)
	{
		rde::fixed_array<int, 5> a;
		CHECK_EQUAL(5ul, a.size());
	}
	TEST(ArrayCtor)
	{
		const int a[] = { 1, 2, 3, 4, 5 };
		rde::fixed_array<int, 5> arr(a);
		CHECK_EQUAL(5ul, arr.size());
		CHECK_EQUAL(0, memcmp(arr.data(), &a[0], sizeof(a)));
	}
	TEST(Subscription)
	{
		const int a[] = { 1, 2, 3, 4, 5 };
		rde::fixed_array<int, 5> arr(a);
		CHECK_EQUAL(3, arr[2]);
		arr[2] = 6;
		CHECK_EQUAL(6, arr[2]);
	}
	TEST(FrontBack)
	{
		const int a[] = { 1, 2, 3, 4, 5 };
		rde::fixed_array<int, 5> arr(a);
		CHECK_EQUAL(1, arr.front());
		CHECK_EQUAL(5, arr.back());
		arr.front() = -1;
		CHECK_EQUAL(-1, arr.front());
		arr.back() = 6;
		CHECK_EQUAL(6, arr.back());
	}

}
