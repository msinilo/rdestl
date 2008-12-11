#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/radix_sorter.h"

#if !RDESTL_STANDALONE
#	include "core/Console.h"
#	include "core/Timer.h"
#	include "core/win32/Windows.h"
#	include <algorithm>	// sort (for comparison)
#endif

template<typename T>
struct as_int
{
	rde::uint32_t operator()(const T& x) const
	{
		return x;
	}
};

namespace
{
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
		rde::int32_t* data = new rde::int32_t[N];
		srand(1011);
		for (int i = 0; i < N; ++i)
			data[i] = rand() - (rand() / 2);

		rde::radix_sorter<rde::int32_t> r;
		r.Resize(N);
		r.Sort<r.data_signed>(data, N, as_int<rde::int32_t>());
		CHECK(IsSorted(data, N));

		delete[] data;
	}
	TEST(RadixSort16Bit)
	{
		static const int N = 200 * 1000;
		rde::uint16_t* data = new rde::uint16_t[N];
		srand(1011);
		for (int i = 0; i < N; ++i)
			data[i] = (rde::uint16_t)(rand());

		rde::radix_sorter<rde::uint16_t> r;
		r.Resize(N);
		r.Sort<r.data_unsigned>(data, N, as_int<rde::uint16_t>());
		CHECK(IsSorted(data, N));

		delete[] data;
	}

#if !RDESTL_STANDALONE
	struct Foo
	{
		rde::uint32_t x;
		int y;
		float z;
	};
	struct FooToInt
	{
		RDE_FORCEINLINE rde::uint32_t operator()(const Foo& f) const
		{
			return f.x;
		}
	};
	bool operator<(const Foo& a, const Foo& b)
	{
		return a.x < b.x;
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
		rde::uint64_t ticks = __rdtsc();
		std::sort(data, data + N);
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("Std took %dms [%f ticks per element]\n", t.GetTimeInMs(),
			double(ticks) / N);

		rde::radix_sorter<Foo> r;
		r.Resize(N);
		for (int i = 0; i < N; ++i)
			data[i].x = rand();
		t.Start();
		ticks = __rdtsc();
		r.Sort<r.data_unsigned>(data, N, FooToInt());
		ticks = __rdtsc() - ticks;
		t.Stop();
		rde::Console::Printf("Radix took %dms [%f ticks per iter]\n", t.GetTimeInMs(),
			double(ticks) / N);

		CHECK(1 == 1);
	}
#endif
}

