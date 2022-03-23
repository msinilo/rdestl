#include "vendor/Catch/catch.hpp"
#include "fixed_array.h"
#include <cstdio>

namespace
{
TEST_CASE("fixed_array", "[array]")
{
	SECTION("Size")
	{
		rde::fixed_array<int, 5> a;
		CHECK(5 == a.size());
	}

	// https://github.com/msinilo/rdestl/issues/17
	SECTION("Aggregate initialization")
	{
		const int a[] ={ 1, 2, 3, 4, 5 };
		rde::fixed_array<int, 5> arr ={ 1, 2, 3, 4, 5 };
		CHECK(5 == arr.size());
		CHECK(0 == memcmp(arr.data(), &a[0], sizeof(a)));
	}

	SECTION("Accessors")
	{
		rde::fixed_array<int, 5> arr ={ 1, 2, 3, 4, 5 };

		SECTION("Subscript operator")
		{
			CHECK(3 == arr[2]);
			arr[2] = 6;
			CHECK(6 == arr[2]);
		}

		SECTION("Front/Back)")
		{
			CHECK(1 == arr.front());
			CHECK(5 == arr.back());
			arr.front() = -1;
			CHECK(-1 == arr.front());
			arr.back() = 6;
			CHECK(6 == arr.back());
		}
	}
}
}
