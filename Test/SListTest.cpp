#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/slist.h"

namespace
{
	const int array [] = { 1, 4, 9, 16, 25, 36 }; 
	TEST(ConstructEmpty)
	{
		rde::slist<int> lst;
		CHECK(lst.empty());
		CHECK_EQUAL(0, lst.size());
	}
	TEST(PushFront)
	{
		rde::slist<int> lst;
		lst.push_front(5);
		CHECK(!lst.empty());
		CHECK_EQUAL(1, lst.size());
		CHECK_EQUAL(5, lst.front());
		lst.push_front(3);
		CHECK_EQUAL(2, lst.size());
		CHECK_EQUAL(3, lst.front());
	}
	TEST(PopFront)
	{
		rde::slist<int> lst;
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
		rde::slist<int> lst;
		lst.push_front(5);
		lst.push_front(10);
		lst.push_front(15);
		rde::slist<int>::iterator it = lst.begin();
		++it; 
		rde::slist<int>::iterator itAfterBegin = it;
		++it;
		rde::slist<int>::iterator itPrev = lst.previous(it);
		CHECK(itPrev == itAfterBegin);
		itPrev = lst.previous(itPrev);
		CHECK(itPrev == lst.begin());
	}

	TEST(Clear)
	{
		rde::slist<int> lst;
		lst.push_front(5);
		lst.push_front(4);
		lst.push_front(3);
		lst.push_front(2);
		lst.push_front(1);
		CHECK(!lst.empty());
		CHECK_EQUAL(5, lst.size());
		lst.clear();
		CHECK(lst.empty());
		CHECK_EQUAL(0, lst.size());
	}

	TEST(AssignCtor)
	{
		rde::slist<int> lst(&array[0], &array[6]);
		CHECK_EQUAL(6, lst.size());
		CHECK_EQUAL(1, lst.front());
		rde::slist<int>::iterator it = lst.begin();
		int i(1);
		for (/**/; it != lst.end(); ++it, ++i)
		{
			CHECK_EQUAL(i*i, *it);
		}
	}

	TEST(AssignmentOp)
	{
		rde::slist<int> lst(&array[0], &array[6]);
		rde::slist<int> lst2;
		lst2 = lst;
		CHECK_EQUAL(6, lst2.size());
		CHECK_EQUAL(1, lst2.front());
		rde::slist<int>::iterator it = lst.begin();
		int i(1);
		for (/**/; it != lst.end(); ++it, ++i)
		{
			CHECK_EQUAL(i*i, *it);
		}
	}
}
