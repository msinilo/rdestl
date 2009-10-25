#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/fixed_list.h"
#include "rdestl/list.h"
#include "rdestl/rdestl.h"
#if !RDESTL_STANDALONE
#	include "core/Console.h"
#	include "core/Random.h"
#	include "core/Timer.h"
#	include "rdestl/vector.h"
#endif

namespace
{
	const int array [] = { 1, 4, 9, 16, 25, 36 }; 

#define CONCAT_(x, y)	x ## y
#define CONCAT2_(x, y)	CONCAT_(x, y)
#define TESTC(x)		TEST(CONCAT2_(x, POSTFIX))

#define POSTFIX	PtrBased
#define tTestList	rde::list<int>
#include "ListTestInc.h"

#undef tTestList
#undef POSTFIX
#define POSTFIX	Linear
#define tTestList	rde::fixed_list<int, 8>
#include "ListTestInc.h"

	TEST(FixedListEraseInsertIndicesReuse)
	{
		typedef rde::fixed_list<int, 4> TList;
		TList lst;
		for (int i = 0; i < 4; ++i)
			lst.push_back((i + 1) * 2);

		TList::iterator it = lst.begin();
		it = lst.erase(it);
		++it, ++it;
		lst.erase(it);
		CHECK_EQUAL(2, lst.size());
		lst.push_back(999);
		lst.push_back(888);
		CHECK_EQUAL(4, lst.size());
		CHECK_EQUAL(4, lst.front());
		it = lst.begin(); ++it;
		CHECK_EQUAL(6, *it);
		CHECK_EQUAL(888, lst.back());
	}
#if !RDESTL_STANDALONE
	// Tests if list can be copied by memcopying.
	TEST(FixedListRelocation)
	{
		typedef rde::fixed_list<int, 1000> TList;
		TList lst;
		rde::Random::Init(1001);
		for (int i = 0; i < 1000; ++i)
			lst.push_back(rde::Random::NextInt());

		void* mem2 = new char[sizeof(lst)];
		rde::Sys::MemCpy(mem2, &lst, sizeof(lst));
		TList* lst2 = (TList*)mem2;
		CHECK_EQUAL(lst.size(), lst2->size());
		TList::iterator it = lst.begin();
		TList::iterator it2 = lst2->begin();
		while (it != lst.end())
		{
			CHECK_EQUAL(*it, *it2);
			++it;
			++it2;
		}
		CHECK(it2 == lst2->end());
		delete[] (char*)mem2;
	}

	struct Observer
	{
		virtual void Notify()
		{

		}
	};
#if 0
	TEST(FixedListVersusVectorPerformance)
	{
		typedef rde::fixed_list<Observer*, 256>	ObserverList;
		typedef rde::vector<Observer*>			ObserverVector;

		rde::Timer timer;
		timer.Start();
		ObserverVector vObservers;
		for (int i = 0; i < 256; ++i)
			vObservers.push_back(new Observer);

		static const int kNumFrames = 100000;
		for (int i = 0; i < kNumFrames; ++i)
		{
			Observer* o = vObservers.front();
			vObservers.erase(vObservers.begin());
			vObservers.push_back(o);
		}
		timer.Stop();
		rde::Console::Printf("Vector took %d ms\n", timer.GetTimeInMs());

		timer.Start();
		ObserverList lObservers;
		for (int i = 0; i < 256; ++i)
			lObservers.push_back(new Observer);

		for (int i = 0; i < kNumFrames; ++i)
		{
			Observer* o = lObservers.front();
			lObservers.pop_front();
			lObservers.push_back(o);
		}
		timer.Stop();
		rde::Console::Printf("List took %d ms\n", timer.GetTimeInMs());

		for (ObserverVector::iterator it = vObservers.begin(); it != vObservers.end(); ++it)
			delete *it;
		for (ObserverList::iterator it = lObservers.begin(); it != lObservers.end(); ++it)
			delete *it;

		CHECK(1 == 1);	// Just to shut compiler up (unused variable testResults).
	}
#endif

#endif // RDESTL_STANDALONE
}


