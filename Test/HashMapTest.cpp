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

	// Reported by Shiran Ben-Israel
	TEST(ShiranIssue)
	{
		static const int MAX_KEYS = 1024;
		int aKeys[MAX_KEYS];
		for (int i = 0; i < MAX_KEYS; i++)
		{
			int iValue = 0;
			bool bInsertedIntoArray = false;
			while (bInsertedIntoArray == false)
			{
				iValue = rand()%32767;
				bool bAlreadyInArray = false;
				for (int j = 0; j < i; j++)
				{
					if (aKeys[j] == iValue)
					{
						bAlreadyInArray = true;
					}
					if (bAlreadyInArray == true)
						break;
				}
				if (bAlreadyInArray==false)
				{
					aKeys[i] = iValue;
					bInsertedIntoArray = true;
				}
			}
		}
		typedef rde::hash_map<int, int> HashMap;
		typedef rde::hash_map<int, int>::iterator HashMapIterator;
		HashMap m_mHashMap;
		HashMapIterator itr = m_mHashMap.begin();

		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_mHashMap.insert(rde::make_pair(aKeys[i],0));
		}
 
		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_mHashMap.erase(aKeys[i]);
			HashMapIterator itr = m_mHashMap.find(aKeys[i]);
			RDE_ASSERT(itr == m_mHashMap.end());
			CHECK(itr == m_mHashMap.end());
		}
	}
}	

