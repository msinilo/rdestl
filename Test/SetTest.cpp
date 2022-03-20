#include "vendor/Catch/catch.hpp"
#include "set.h"

namespace
{
TEST_CASE("set", "[set]")
{
	SECTION("DefaultCtorEmptySet")
	{
		rde::set<int> t;
		CHECK(t.empty());
		CHECK(0 == t.size());
	}
	SECTION("IteratorBasic")
	{
		rde::set<int> t;
		t.insert(5);
		rde::set<int>::iterator it = t.begin();
		CHECK(it != t.end());
		CHECK(5 == *it);
	}
	SECTION("Find")
	{
		rde::set<int> t;
		t.insert(5);
		t.insert(2);
		t.insert(15);
		t.insert(7);
		rde::set<int>::iterator it = t.find(5);
		CHECK(it != t.end());
		CHECK(5 == *it);
		it = t.find(100);
		CHECK(it == t.end());
	}
	SECTION("Erase")
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
		CHECK(0 == t.size());
	}
}
}
