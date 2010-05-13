#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/radix_sorter.h"
#include "rdestl/sort.h"

#if !RDESTL_STANDALONE
#	include "core/Console.h"
#	include "core/Timer.h"
#	include "core/win32/Windows.h"
#	include <algorithm>	// sort (for comparison)
#endif

#define SORT_SPEED_TEST	0

namespace
{
	template<typename T>
	struct as_int
	{
		rde::uint32 operator()(const T& x) const
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

	TEST(RadixSortTestSigned)
	{
		static const int N = 200 * 1000;
		rde::int32* data = new rde::int32[N];
		srand(1011);
		for (int i = 0; i < N; ++i)
			data[i] = rand() - (rand() / 2);

		rde::radix_sorter<rde::int32> r;
		r.sort<r.data_signed>(data, N, as_int<rde::int32>());
		CHECK(IsSorted(data, N));

		delete[] data;
	}
	TEST(RadixSort16Bit)
	{
		static const int N = 200 * 1000;
		rde::uint16* data = new rde::uint16[N];
		srand(1011);
		for (int i = 0; i < N; ++i)
			data[i] = (rde::uint16)(rand());

		rde::radix_sorter<rde::uint16> r;
		r.sort<r.data_unsigned>(data, N, as_int<rde::uint16>());
		CHECK(IsSorted(data, N));

		delete[] data;
	}
	TEST(InsertionSort)
	{
		static const int N = 200;
		rde::uint16* data = new rde::uint16[N];
		srand(1011);
		for (int i = 0; i < N; ++i)
			data[i] = (rde::uint16)(rand());

		rde::insertion_sort(&data[0], &data[N]);
		CHECK(IsSorted(data, N));
		delete[] data;
	}
	TEST(QuickSort)
	{
		static const int N = 200 * 1000;
		rde::uint16* data = new rde::uint16[N];
		srand(1011);
		for (int i = 0; i < N; ++i)
			data[i] = (rde::uint16)(rand());

		rde::quick_sort(&data[0], &data[N]);
		CHECK(IsSorted(data, N));
		delete[] data;
	}
	TEST(HeapSort)
	{
		static const int N = 200 * 1000;
		rde::uint16* data = new rde::uint16[N];
		srand(1011);
		for (int i = 0; i < N; ++i)
			data[i] = (rde::uint16)(rand());

		rde::heap_sort(&data[0], &data[N]);
		CHECK(IsSorted(data, N));
		delete[] data;
	}
	TEST(IsSortedWhenSorted)
	{
		const int data[4] = { -1, 2, 3, 8 };
		CHECK(rde::is_sorted(&data[0], &data[4], rde::less<int>()));
	}
	TEST(IsSortedReturnsFalseWhenNotSorted)
	{
		const int data[4] = { -1, 9, 3, 8 };
		CHECK(!rde::is_sorted(&data[0], &data[4], rde::less<int>()));
	}
	// Issue reported by Daniel Treble: http://code.google.com/p/rdestl/issues/detail?id=2
	// (used to crash).
	TEST(QuickSortIssue2)
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
		rde::uint32 x;
		int y;
		float z;
	};
	struct FooToInt
	{
		RDE_FORCEINLINE rde::uint32 operator()(const Foo& f) const
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

	TEST(RadixSpeedTest)
	{
		static const int N = 300 * 1000;

		Foo* data = new Foo[N];
		srand(1011);
		for (int i = 0; i < N; ++i)
			data[i].x = rand();

		rde::Timer t;
		t.Start();
		rde::uint64 ticks = __rdtsc();
		std::sort(data, data + N);
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("Std took %dms [%f ticks per element]\n", t.GetTimeInMs(),
			double(ticks) / N);
		CHECK(rde::is_sorted(data, data + N, rde::less<Foo>()));

		rde::radix_sorter<Foo> r;
		for (int i = 0; i < N; ++i)
			data[i].x = rand();
		t.Start();
		ticks = __rdtsc();
		r.sort<r.data_unsigned>(data, N, FooToInt());
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("Radix took %dms [%f ticks per iter]\n", t.GetTimeInMs(),
			double(ticks) / N);
		CHECK(rde::is_sorted(data, data + N, rde::less<Foo>()));

		for (int i = 0; i < N; ++i)
			data[i].x = rand();
		t.Start();
		ticks = __rdtsc();
		rde::quick_sort(data, data + N);
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("RDE quick sort took %dms [%f ticks per iter]\n", t.GetTimeInMs(),
			double(ticks) / N);
		CHECK(rde::is_sorted(data, data + N, rde::less<Foo>()));

		for (int i = 0; i < N; ++i)
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

