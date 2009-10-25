#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/set.h"

namespace
{
	TEST(DefaultCtorEmptySet)
	{
		rde::set<int> t;
		CHECK(t.empty());
		CHECK_EQUAL(0, t.size());
	}
	TEST(IteratorBasic)
	{
		rde::set<int> t;
		t.insert(5);
		rde::set<int>::iterator it = t.begin();
		CHECK(it != t.end());
		CHECK_EQUAL(5, *it);
	}
	TEST(Find)
	{
		rde::set<int> t;
		t.insert(5);
		t.insert(2);
		t.insert(15);
		t.insert(7);
		rde::set<int>::iterator it = t.find(5);
		CHECK(it != t.end());
		CHECK_EQUAL(5, *it);
		it = t.find(100);
		CHECK(it == t.end());
	}
	TEST(Erase)
	{
		rde::set<int> t;
		t.insert(5);
		t.insert(2);
		t.insert(15);
		t.insert(7);
		rde::set<int>::iterator it = t.find(5);
		t.erase(it);
		it = t.find(5);
		CHECK(it == t.end());
		t.erase(2); t.erase(15); t.erase(7);
		CHECK(t.empty());
		CHECK_EQUAL(0, t.size());
	}
}
