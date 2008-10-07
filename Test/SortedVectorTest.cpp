#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/sorted_vector.h"
#include <cstdio>

namespace
{
	typedef rde::sorted_vector<int, int> tVector;
	TEST(Construct)
	{
		tVector v;
		CHECK(v.empty());
	}
	TEST(Insert)
	{
		tVector v;
		CHECK_EQUAL(true, v.insert(rde::pair<int, int>(5, 5)).second);
		CHECK_EQUAL(1, v.size());
		// Try to insert again at the same key
		CHECK_EQUAL(false, v.insert(rde::pair<int, int>(5, 10)).second);
	}
	TEST(InsertAndFind)
	{
		tVector v;
		CHECK_EQUAL(true, v.insert(5, 5).second);
		tVector::iterator it = v.find(5);
		CHECK(it != v.end());
		CHECK_EQUAL(5, it->first);
		CHECK_EQUAL(5, it->second);
		CHECK_EQUAL(true, v.insert(7, 11).second);
		it = v.find(10);
		CHECK(it == v.end());
		it = v.find(7);
		CHECK(it != v.end());
		CHECK_EQUAL(7, it->first);
		CHECK_EQUAL(11, it->second);
		CHECK_EQUAL(2, v.size());
	}
	TEST(Erase)
	{
		tVector v;
		CHECK_EQUAL(true, v.insert(5, 5).second);
		CHECK_EQUAL(true, v.insert(7, 11).second);
		CHECK_EQUAL(true, v.insert(9, 20).second);
		CHECK_EQUAL(1, v.erase(7));
		CHECK_EQUAL(2, v.size());
		tVector::iterator it = v.find(7);
		CHECK(it == v.end());
	}
}
