#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/slist.h"

namespace
{
	const int array [] = { 1, 4, 9, 16, 25, 36 }; 
	TEST(ConstructEmpty)
	{
		rdestl::slist<int> lst;
		CHECK(lst.empty());
		CHECK_EQUAL(0ul, lst.size());
	}
	TEST(PushFront)
	{
		rdestl::slist<int> lst;
		lst.push_front(5);
		CHECK(!lst.empty());
		CHECK_EQUAL(1ul, lst.size());
		CHECK_EQUAL(5, lst.front());
		lst.push_front(3);
		CHECK_EQUAL(2ul, lst.size());
		CHECK_EQUAL(3, lst.front());
	}
	TEST(PopFront)
	{
		rdestl::slist<int> lst;
		lst.push_front(5);
		lst.push_front(3);
		CHECK(!lst.empty());
		lst.pop_front();
		CHECK_EQUAL(5, lst.front());
		lst.pop_front();
		CHECK(lst.empty());
	}
	TEST(Previous)
	{
		rdestl::slist<int> lst;
		lst.push_front(5);
		lst.push_front(10);
		lst.push_front(15);
		rdestl::slist<int>::iterator it = lst.begin();
		++it; 
		rdestl::slist<int>::iterator itAfterBegin = it;
		++it;
		rdestl::slist<int>::iterator itPrev = lst.previous(it);
		CHECK(itPrev == itAfterBegin);
		itPrev = lst.previous(itPrev);
		CHECK(itPrev == lst.begin());
	}

	TEST(Clear)
	{
		rdestl::slist<int> lst;
		lst.push_front(5);
		lst.push_front(4);
		lst.push_front(3);
		lst.push_front(2);
		lst.push_front(1);
		CHECK(!lst.empty());
		CHECK_EQUAL(5ul, lst.size());
		lst.clear();
		CHECK(lst.empty());
		CHECK_EQUAL(0ul, lst.size());
	}
}
