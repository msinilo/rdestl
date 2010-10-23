#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/hash_map.h"
#include "rdestl/stack_allocator.h"
#include <cstdio>
#include <iostream>

namespace
{
	struct hasher
	{
		rde::hash_value_t operator()(const std::string& s) const
		{
			size_t len = s.length();
			rde::hash_value_t hash(0);
			for (size_t i = 0; i < len; ++i)
			{
				hash *= 31;
				hash += s[i];
			}
			return hash;
		}
	};
	struct poor_hasher
	{
		rde::hash_value_t operator()(const std::string& s) const
		{
			return s == "crashtest" ? 4 : 1;
		}
	};

#define CONCAT_(x, y)	x ## y
#define CONCAT2_(x, y)	CONCAT_(x, y)
#define TESTC(x)		TEST(CONCAT2_(x, POSTFIX))

#undef tMap
#undef tPoorlyHashedMap
#undef POSTFIX
#define POSTFIX				Closed
#define tMap				rde::hash_map<std::string, int, hasher, 6>
#define tPoorlyHashedMap	rde::hash_map<std::string, int, poor_hasher, 6>
#include "HashMapTestInc.h"

// Instantiate to check all methods.
template rde::hash_map<std::string, int, hasher, 6>;

    // Partially ripped from Google's hash tests.
	TEST(GoogleIntHash)
    {
		rde::hash_map<int, int> m;
		CHECK(m.empty());
		m.insert(rde::make_pair(1, 0));
		CHECK(!m.empty());
		m.insert(rde::make_pair(11, 0));
		m.insert(rde::make_pair(111, 0));
		m.insert(rde::make_pair(1111, 0));
		m.insert(rde::make_pair(11111, 0));
		m.insert(rde::make_pair(111111, 0));
		m.insert(rde::make_pair(1111111, 0));
		m.insert(rde::make_pair(11111111, 0));
		m.insert(rde::make_pair(111111111, 0));
		m.insert(rde::make_pair(1111111111, 0));
		CHECK_EQUAL(10, m.size());
		m.erase(11111);
		CHECK_EQUAL(9, m.size());
		m.insert(rde::make_pair(11111, 0));
		CHECK_EQUAL(10, m.size());
		m.erase(11111);
		m.insert(rde::make_pair(11111, 0));
		CHECK_EQUAL(10, m.size());
		CHECK_EQUAL(0, m.erase(-11111));
		CHECK_EQUAL(10, m.size());
		m.erase(1);
		CHECK_EQUAL(9, m.size());
		rde::hash_map<int, int>::iterator it = m.find(1111);
		m.erase(it);
		CHECK_EQUAL(8, m.size());
		it = m.find(22222);
		m.erase(it);
		CHECK_EQUAL(8, m.size());
		m.erase(m.begin(), m.end());
		CHECK(m.empty());
	}
	TEST(GoogleIntHashFixed)
    {
		typedef rde::hash_map<int, int, rde::hash<int>, 6, rde::equal_to<int>, rde::stack_allocator<1000> > TestMap;
		TestMap m;
		CHECK(m.empty());
		m.insert(rde::make_pair(1, 0));
		CHECK(!m.empty());
		m.insert(rde::make_pair(11, 0));
		m.insert(rde::make_pair(111, 0));
		m.insert(rde::make_pair(1111, 0));
		m.insert(rde::make_pair(11111, 0));
		m.insert(rde::make_pair(111111, 0));
		m.insert(rde::make_pair(1111111, 0));
		m.insert(rde::make_pair(11111111, 0));
		m.insert(rde::make_pair(111111111, 0));
		m.insert(rde::make_pair(1111111111, 0));
		CHECK_EQUAL(10, m.size());
		m.erase(11111);
		CHECK_EQUAL(9, m.size());
		m.insert(rde::make_pair(11111, 0));
		CHECK_EQUAL(10, m.size());
		m.erase(11111);
		m.insert(rde::make_pair(11111, 0));
		CHECK_EQUAL(10, m.size());
		CHECK_EQUAL(0, m.erase(-11111));
		CHECK_EQUAL(10, m.size());
		m.erase(1);
		CHECK_EQUAL(9, m.size());
		TestMap::iterator it = m.find(1111);
		m.erase(it);
		CHECK_EQUAL(8, m.size());
		it = m.find(22222);
		m.erase(it);
		CHECK_EQUAL(8, m.size());
		m.erase(m.begin(), m.end());
		CHECK(m.empty());
	}
}	

