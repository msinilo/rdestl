#include "rdestl/vector.h"
#include <cstdio>
#include <numeric> 
#include <vector>
#include <windows.h>
#include <string>


namespace
{
	const size_t kCount = 100000;
	struct MyStruct
	{
		bool operator==(const MyStruct& rhs) const
		{
			return a == rhs.a;
		}

		int			a;
		float		v[3];
		float		q[4];
		MyStruct*	next;
	};

	// Helper class
	class Timer
	{
	public:
		Timer()
		: tick(0)
		{
			SetThreadAffinityMask( GetCurrentThread(), 1 );	// Tries to prevent this thread from jumping around.
			Sleep(0); // When we wake up, we're hopefully at the beginning of a timequantum.
		}
		
		void Sample()
		{
			LARGE_INTEGER stick;
			QueryPerformanceCounter(&stick);
			
			this->deltaTicks = (stick.QuadPart - this->tick);
			if( this->deltaTicks < 0 )
				this->deltaTicks = 0;
			this->tick = stick.QuadPart;
		}
		
		float DeltaTime() const
		{
			LARGE_INTEGER freq;
			QueryPerformanceFrequency( &freq );
			return float(double(this->deltaTicks) / double(freq.QuadPart));	// Dubious casting of the floats here...
		}
		
		__int64 tick;
		__int64 deltaTicks;
	};
}

namespace rde
{
	template<> struct is_pod<MyStruct>
	{
		enum { value = true };
	};
}

namespace
{
	typedef float (*TestFunc)(size_t);

	struct SpeedTest
	{
		const char*			name;
		TestFunc			func;
		std::vector<float>	times;
	};

	Timer timer;

	template<class TVector>
	float Vector_Construct(size_t num)
	{
		timer.Sample();
		for (size_t i = 0; i < num; ++i)
		{
			TVector v;
			sizeof(v);
		}
		timer.Sample();
		return timer.DeltaTime();
	}
	template<class TVector>
	float Vector_PushBack(size_t num)
	{
		timer.Sample();
		TVector v;
		for (size_t i = 0; i < num; ++i)
		{
			v.push_back("hello");
			v.push_back("world");
		}
		timer.Sample();
		return timer.DeltaTime();
	}
	template<class TVector>
	float Vector_InsertInt(size_t num)
	{
		TVector v;
		for (size_t i = 0; i < num; ++i)
			v.push_back(rand());
		timer.Sample();
		for (size_t i = 0; i < 2000; ++i)
		{
			const size_t insertIndex = i * 5;
			v.insert(v.begin() + insertIndex, int(i));
		}
		timer.Sample();
		return timer.DeltaTime();
	}
	template<class TVector>
	float Vector_InsertPOD(size_t num)
	{
		TVector v;
		for (size_t i = 0; i < num/10; ++i)
			v.push_back(MyStruct());
		timer.Sample();
		for (size_t i = 0; i < 2000; ++i)
		{
			const size_t insertIndex = i * 5;
			v.insert(v.begin() + insertIndex, MyStruct());
		}
		timer.Sample();
		return timer.DeltaTime();
	}

	template<class TVector>
	float Vector_ErasePOD(size_t num)
	{
		TVector v;
		num = num/10;
		for (size_t i = 0; i < num; ++i)
			v.push_back(MyStruct());//"hello world");
		timer.Sample();
		for (size_t i = 0; i < 2000; ++i)
		{
			const size_t idx = i * 4;
			v.erase(v.begin() + idx);
		}
		timer.Sample();
		return timer.DeltaTime();
	}
	template<class TVector>
	float Vector_EraseString(size_t num)
	{
		TVector v;
		num = num/10;
		for (size_t i = 0; i < num; ++i)
			v.push_back("hello world");
		timer.Sample();
		for (size_t i = 0; i < 2000; ++i)
		{
			const size_t idx = i * 4;
			v.erase(v.begin() + idx);
		}
		timer.Sample();
		return timer.DeltaTime();
	}

	SpeedTest s_tests[] = 
	{
		{ "STL vector: construction", Vector_Construct<std::vector<std::string> > },
		{ "RDE vector: construction", Vector_Construct<rde::vector<std::string> > },
		{ "STL vector: push_back", Vector_PushBack<std::vector<std::string> > },
		{ "RDE vector: push_back", Vector_PushBack<rde::vector<std::string> > },
		{ "STL vector: insert int", Vector_InsertInt<std::vector<int> > },
		{ "RDE vector: insert int", Vector_InsertInt<rde::vector<int> > },
		{ "STL vector: insert POD", Vector_InsertPOD<std::vector<MyStruct> > },
		{ "RDE vector: insert POD", Vector_InsertPOD<rde::vector<MyStruct> > },
		{ "STL vector: erase POD", Vector_ErasePOD<std::vector<MyStruct> > },
		{ "RDE vector: erase POD", Vector_ErasePOD<rde::vector<MyStruct> > },
		{ "STL vector: erase string", Vector_EraseString<std::vector<std::string> > },
		{ "RDE vector: erase string", Vector_EraseString<rde::vector<std::string> > },
	};
	const size_t kNumTests = sizeof(s_tests) / sizeof(s_tests[0]);

	void RunTest(SpeedTest& test, int numTries)
	{
		while (numTries-- > 0)
		{
			const float time = test.func(kCount);
			test.times.push_back(time);
		}
	}
	void ReportTestResults()
	{
		for (size_t i = 0; i < kNumTests; ++i)
		{
			const float avgTime = std::accumulate(s_tests[i].times.begin(),
				s_tests[i].times.end(), 0.f) / float(s_tests[i].times.size());
			printf("%s: avg time %f\n", s_tests[i].name, avgTime);
		}
	}
}

//-----------------------------------------------------------------------------
void RunSpeedTests()
{
	for (size_t i = 0; i < ::kNumTests; ++i)
	{
		::RunTest(s_tests[i], 4);
	}
	::ReportTestResults();
}
//-----------------------------------------------------------------------------
