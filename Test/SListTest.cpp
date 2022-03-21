#include "vendor/Catch/catch.hpp"
#include "rdestl/slist.h"

namespace
{
TEST_CASE("slist", "[list]")
{
	const int array [] = { 1, 4, 9, 16, 25, 36 };

	SECTION("ConstructEmpty")
	{
		rde::slist<int> lst;
		CHECK(lst.empty());
		CHECK(0 == lst.size());
	}
	SECTION("PushFront")
	{
		rde::slist<int> lst;
		lst.push_front(5);
		CHECK(!lst.empty());
		CHECK(1 == lst.size());
		CHECK(5 == lst.front());
		lst.push_front(3);
		CHECK(2 == lst.size());
		CHECK(3 == lst.front());
	}
	SECTION("PopFront")
	{
		rde::slist<int> lst;
		lst.push_front(5);
		lst.push_front(3);
		CHECK(!lst.empty());
		lst.pop_front();
		CHECK(5 == lst.front());
		lst.pop_front();
		CHECK(lst.empty());
	}
	SECTION("Previous")
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

	SECTION("Clear")
	{
		rde::slist<int> lst;
		lst.push_front(5);
		lst.push_front(4);
		lst.push_front(3);
		lst.push_front(2);
		lst.push_front(1);
		CHECK(!lst.empty());
		CHECK(5 == lst.size());
		lst.clear();
		CHECK(lst.empty());
		CHECK(0 == lst.size());
	}

	SECTION("AssignCtor")
	{
		rde::slist<int> lst(&array[0], &array[6]);
		CHECK(6 == lst.size());
		CHECK(1 == lst.front());
		rde::slist<int>::iterator it = lst.begin();
		int i(1);
		for (/**/; it != lst.end(); ++it, ++i)
		{
			CHECK(i*i == *it);
		}
	}

	SECTION("AssignmentOp")
	{
		rde::slist<int> lst(&array[0], &array[6]);
		rde::slist<int> lst2;
		lst2 = lst;
		CHECK(6 == lst2.size());
		CHECK(1 == lst2.front());
		rde::slist<int>::iterator it = lst.begin();
		int i(1);
		for (/**/; it != lst.end(); ++it, ++i)
		{
			CHECK(i*i == *it);
		}
	}
}
}
