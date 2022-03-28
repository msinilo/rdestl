#include <cstdio>
#include "fixed_array.h"
#include "vendor/Catch/catch.hpp"

namespace
{
TEST_CASE("fixed_array", "[array]")
{
	SECTION("Construction")
	{
		const int a[] ={ 1, 2, 3, 4, 5 };

		// https://github.com/msinilo/rdestl/issues/17
		SECTION("Aggregate initialization")
		{
			rde::fixed_array<int, 5> arr ={ 1, 2, 3, 4, 5 };
			CHECK(5 == arr.size());
			CHECK(0 == memcmp(arr.data(), &a[0], sizeof(a)));
		}

		SECTION("ArrayCtor")
		{
			rde::fixed_array<int, 5> arr;
			arr.from_raw_array(a);
			CHECK(5 == arr.size());
			CHECK(0 == memcmp(arr.data(), &a[0], sizeof(a)));
		}
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

		SECTION("Front/Back")
		{
			CHECK(1 == arr.front());
			CHECK(5 == arr.back());
			arr.front() = -1;
			CHECK(-1 == arr.front());
			arr.back() = 6;
			CHECK(6 == arr.back());
		}
	}

	SECTION("Size")
	{
		rde::fixed_array<int, 5> a;
		CHECK(5 == a.size());
	}

	SECTION("Fill")
	{
		const int a[] = { 1, 1, 1, 1, 1 };
		rde::fixed_array<int, 5> arr = { 0, 0, 0, 0, 0 };
		REQUIRE(0 != memcmp(arr.data(), &a[0], sizeof(a)));
		arr.fill(1);
		CHECK(0 == memcmp(arr.data(), &a[0], sizeof(a)));
	}
}
} //namespace
