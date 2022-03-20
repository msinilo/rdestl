#include "vendor/Catch/catch.hpp"
#include "algorithm.h"
#include "functional.h"
#include "list.h"

namespace
{
TEST_CASE("algorithm", "[utility]")
{
	SECTION("LowerBound")
	{
		const int array[] ={ 1, 4, 9, 16, 25, 36 };
		const int* it = rde::lower_bound(&array[0], &array[6], 11, rde::less<int>());
		CHECK(16 == *it);
		const int* it2 = &array[0];
		rde::less<int> pred;
		while (it2 != it)
		{
			CHECK(pred(*it2, *it));
			++it2;
		}
	}
	SECTION("LowerBoundInputIter")
	{
		const int array[] ={ 1, 4, 9, 16, 25, 36 };
		rde::list<int> lst(&array[0], &array[6]);
		rde::list<int>::iterator it = rde::lower_bound(lst.begin(), lst.end(), 11, rde::less<int>());
		CHECK(16 == *it);
		rde::list<int>::iterator it2 = lst.begin();
		rde::less<int> pred;
		while (it2 != it)
		{
			CHECK(pred(*it2, *it));
			++it2;
		}
	}
	SECTION("UpperBound")
	{
		const int array[] ={ 1, 2, 3, 3, 3, 5, 8 };
		const int N = sizeof(array) / sizeof(int);
		const int* it = rde::upper_bound(&array[0], &array[N], 5, rde::less<int>());
		CHECK(8 == *it);
		it = rde::upper_bound(&array[0], &array[N], 6, rde::less<int>());
		CHECK(8 == *it);
		it = rde::upper_bound(&array[0], &array[N], 2, rde::less<int>());
		const int* it2 = &array[0];
		rde::less<int> pred;
		while (it2 != it)
		{
			CHECK(!pred(2, *it2));
			++it2;
		}
	}
	SECTION("Find")
	{
		const int array[] ={ 1, 4, 9, 16, 25, 36 };
		const int* it = rde::find(&array[0], &array[6], 16);
		CHECK(it == &array[3]);
		CHECK(16 == *it);
	}
	SECTION("FindIf")
	{
		const int array[] ={ 1, 4, 9, 16, 25, 36 };
		const int* it = rde::find_if(&array[0], &array[6], 16, rde::equal_to<int>());
		CHECK(it == &array[3]);
		CHECK(16 == *it);
	}
	SECTION("AbsGeneric")
	{
		CHECK(5.f == rde::abs(5.f));
		CHECK(5.f == rde::abs(-5.f));
		CHECK(0.f == rde::abs(0.f));
	}
	SECTION("AbsInt")
	{
		CHECK(5 == rde::abs(5));
		CHECK(5 == rde::abs(-5));
		CHECK(0 == rde::abs(0));
	}
	SECTION("AbsShort")
	{
		CHECK(5 == rde::abs<short>(5));
		short x(-5);
		CHECK(5 == rde::abs(x));
		x = 0;
		CHECK(0 == rde::abs(x));
	}
	SECTION("MinFloat")
	{
		CHECK(2.f == rde::min(2.f, 3.f));
	}
	// http://code.google.com/p/rdestl/issues/detail?id=7
	SECTION("UnsignedIntsHaveTrivialCopy")
	{
		CHECK(1 == rde::has_trivial_copy<unsigned char>::value);
		CHECK(1 == rde::has_trivial_copy<unsigned short>::value);
		CHECK(1 == rde::has_trivial_copy<unsigned int>::value);
		CHECK(1 == rde::has_trivial_copy<unsigned long>::value);
	}
	// http://code.google.com/p/rdestl/issues/detail?id=8
	SECTION("MoveNOverlap")
	{
		int tab[] ={ 0, 1, 2, 3, 4, 5, 6 };
		const int* src = &tab[1];
		int* dst = &tab[0];

		rde::move_n(src, 3, dst);

		CHECK(1 == tab[0]);
		CHECK(2 == tab[1]);
		CHECK(3 == tab[2]);
	}
	SECTION("MoveOverlap")
	{
		int tab[] ={ 0, 1, 2, 3, 4, 5, 6 };
		const int* src = &tab[1];
		int* dst = &tab[0];

		rde::move(src, src + 3, dst);

		CHECK(1 == tab[0]);
		CHECK(2 == tab[1]);
		CHECK(3 == tab[2]);
	}

	// http://msinilo.pl/blog/?p=956&cpage=1#comment-48615
	SECTION("Regression_MovePreservesOrder")
	{
		int tab[] ={ 0, 1, 2, 3, 4, 5, 6 };
		const int* src = &tab[0];
		int* dst = &tab[3];

		rde::internal::move(src, src + 3, dst, rde::int_to_type<false>());

		CHECK(0 == tab[3]);
		CHECK(1 == tab[4]);
		CHECK(2 == tab[5]);
	}
}
}
