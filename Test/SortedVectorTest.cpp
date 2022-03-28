#include <cstdio>
#include "sorted_vector.h"
#include "vendor/Catch/catch.hpp"

namespace
{
	typedef rde::sorted_vector<int, int> tVector;

TEST_CASE("sorted_vector", "[vector]")
{
	SECTION("Construct")
	{
		tVector v;
		CHECK(v.empty());
	}
	SECTION("Insert")
	{
		tVector v;
		CHECK(true == v.insert(rde::pair<int, int>(5, 5)).second);
		CHECK(1 == v.size());
		// Try to insert again at the same key
		CHECK(false == v.insert(rde::pair<int, int>(5, 10)).second);
	}
	SECTION("InsertAndFind")
	{
		tVector v;
		CHECK(true == v.insert(5, 5).second);
		tVector::iterator it = v.find(5);
		CHECK(it != v.end());
		CHECK(5 == it->first);
		CHECK(5 == it->second);
		CHECK(true == v.insert(7, 11).second);
		it = v.find(10);
		CHECK(it == v.end());
		it = v.find(7);
		CHECK(it != v.end());
		CHECK(7 == it->first);
		CHECK(11 == it->second);
		CHECK(2 == v.size());
	}
	SECTION("Erase")
	{
		tVector v;
		CHECK(true == v.insert(5, 5).second);
		CHECK(true == v.insert(7, 11).second);
		CHECK(true == v.insert(9, 20).second);
		CHECK(1 == v.erase(7));
		CHECK(2 == v.size());
		tVector::iterator it = v.find(7);
		CHECK(it == v.end());
	}
	// Regression test, insert would fail on the 3rd element.
	SECTION("RegressionInsert")
	{
		tVector v;
		CHECK(true == v.insert(268, 5).second);
		CHECK(true == v.insert(502, 11).second);
		CHECK(true == v.insert(123, 20).second);
		CHECK(3 == v.size());
	}
}
}
