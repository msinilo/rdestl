#include "radix_sorter.h"
#include "sort.h"

#if !RDESTL_STANDALONE
// FIXME: "core/*" Files missing? ~SK
#	include "core/Console.h"
#	include "core/Timer.h"
#	include "core/win32/Windows.h"
#	include <algorithm>	// sort (for comparison)
#endif

#include "vendor/Catch/catch.hpp"

#define SORT_SPEED_TEST	0

namespace
{
	template<typename T>
	struct as_int
	{
		std::uint32_t operator()(const T& x) const
		{
			return x;
		}
	};

	template<typename T>
	bool IsSorted(T* data, size_t num)
	{
		for (size_t i = 1; i < num; ++i)
		{
			RDE_ASSERT(data[i - 1] <= data[i]);
			if (data[i - 1] > data[i])
				return false;
		}
		return true;
	}

TEST_CASE("sort", "[algorithm]")
{
	SECTION("RadixSortTestSigned")
	{
		static const size_t N = 200 * 1000;
		std::int32_t* data = new std::int32_t[N];
		srand(1011);
		for (size_t i = 0; i < N; ++i)
			data[i] = rand() - (rand() / 2);

		rde::radix_sorter<std::int32_t> r;
		r.sort<r.data_signed>(data, N, as_int<std::int32_t>());
		CHECK(IsSorted(data, N));

		delete[] data;
	}
	SECTION("RadixSort16Bit")
	{
		static const size_t N = 200 * 1000;
		std::uint16_t* data = new std::uint16_t[N];
		srand(1011);
		for (size_t i = 0; i < N; ++i)
			data[i] = (std::uint16_t)(rand());

		rde::radix_sorter<std::uint16_t> r;
		r.sort<r.data_unsigned>(data, N, as_int<std::uint16_t>());
		CHECK(IsSorted(data, N));

		delete[] data;
	}
	SECTION("InsertionSort")
	{
		static const size_t N = 200;
		std::uint16_t* data = new std::uint16_t[N];
		srand(1011);
		for (size_t i = 0; i < N; ++i)
			data[i] = (std::uint16_t)(rand());

		rde::insertion_sort(&data[0], &data[N]);
		CHECK(IsSorted(data, N));
		delete[] data;
	}
	SECTION("QuickSort")
	{
		static const size_t N = 200 * 1000;
		std::uint16_t* data = new std::uint16_t[N];
		srand(1011);
		for (size_t i = 0; i < N; ++i)
			data[i] = (std::uint16_t)(rand());

		rde::quick_sort(&data[0], &data[N]);
		CHECK(IsSorted(data, N));
		delete[] data;
	}
	SECTION("HeapSort")
	{
		static const size_t N = 200 * 1000;
		std::uint16_t* data = new std::uint16_t[N];
		srand(1011);
		for (size_t i = 0; i < N; ++i)
			data[i] = (std::uint16_t)(rand());

		rde::heap_sort(&data[0], &data[N]);
		CHECK(IsSorted(data, N));
		delete[] data;
	}
	SECTION("IsSortedWhenSorted")
	{
		const int data[4] = { -1, 2, 3, 8 };
		CHECK(rde::is_sorted(&data[0], &data[4], rde::less<int>()));
	}
	SECTION("IsSortedReturnsFalseWhenNotSorted")
	{
		const int data[4] = { -1, 9, 3, 8 };
		CHECK(!rde::is_sorted(&data[0], &data[4], rde::less<int>()));
	}
	// Issue reported by Daniel Treble: http://code.google.com/p/rdestl/issues/detail?id=2
	// (used to crash).
	SECTION("QuickSortIssue2")
	{
		rde::vector<int> foo;
		foo.push_back(70);
		foo.push_back(60);
		foo.push_back(50);
		foo.push_back(40);
	    rde::quick_sort(foo.begin(), foo.end());
		CHECK(rde::is_sorted(foo.begin(), foo.end(), rde::less<int>()));
	}


#if !RDESTL_STANDALONE && SORT_SPEED_TEST
	struct Foo
	{
		std::uint32_t x;
		int y;
		float z;
	};
	struct FooToInt
	{
		RDE_FORCEINLINE std::uint32_t operator()(const Foo& f) const
		{
			return f.x;
		}
	};
	bool operator<(const Foo& a, const Foo& b)
	{
		return a.x < b.x;
	}
	bool operator>(const Foo& a, const Foo& b)
	{
		return a.x > b.x;
	}

	SECTION("RadixSpeedTest")
	{
		static const size_t N = 300 * 1000;

		Foo* data = new Foo[N];
		srand(1011);
		for (size_t i = 0; i < N; ++i)
			data[i].x = rand();

		rde::Timer t;
		t.Start();
		std::uint64_t ticks = __rdtsc();
		std::sort(data, data + N);
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("Std took %dms [%f ticks per element]\n", t.GetTimeInMs(),
			double(ticks) / N);
		CHECK(rde::is_sorted(data, data + N, rde::less<Foo>()));

		rde::radix_sorter<Foo> r;
		for (size_t i = 0; i < N; ++i)
			data[i].x = rand();
		t.Start();
		ticks = __rdtsc();
		r.sort<r.data_unsigned>(data, N, FooToInt());
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("Radix took %dms [%f ticks per iter]\n", t.GetTimeInMs(),
			double(ticks) / N);
		CHECK(rde::is_sorted(data, data + N, rde::less<Foo>()));

		for (size_t i = 0; i < N; ++i)
			data[i].x = rand();
		t.Start();
		ticks = __rdtsc();
		rde::quick_sort(data, data + N);
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("RDE quick sort took %dms [%f ticks per iter]\n", t.GetTimeInMs(),
			double(ticks) / N);
		CHECK(rde::is_sorted(data, data + N, rde::less<Foo>()));

		for (size_t i = 0; i < N; ++i)
			data[i].x = rand();
		t.Start();
		ticks = __rdtsc();
		rde::heap_sort(data, data + N);
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("RDE heap sort took %dms [%f ticks per iter]\n", t.GetTimeInMs(),
			double(ticks) / N);
		CHECK(rde::is_sorted(data, data + N, rde::less<Foo>()));
	}
#endif
}
}
