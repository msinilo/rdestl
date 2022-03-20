#include "rdestl/map.h"
#include "rdestl/rde_string.h"
#include "vendor/Catch/catch.hpp"

namespace
{
	typedef rde::map<int, int> tMap;
	template rde::map<int, int>;
	template rde::map<int, rde::string>;
	template rde::map<rde::string, rde::string>;

TEST_CASE("map", "[map]")
{
	SECTION("DefaultCtorEmptyMap")
	{
		tMap m;
		CHECK(m.empty());
		CHECK(0 == m.size());
		CHECK(m.begin() == m.end());
	}
	SECTION("InsertIncreasesSize")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		CHECK(1 == m.size());
		m.insert(tMap::value_type(6, 10));
		CHECK(2 == m.size());
	}
	SECTION("SameKeyDoesntGetInsertedTwice")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		CHECK(1 == m.size());
		m.insert(tMap::value_type(5, 100));
		CHECK(1 == m.size());
	}
	SECTION("DoubleInsertReturnsValidIterator")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		CHECK(1 == m.size());
		tMap::iterator it = m.insert(tMap::value_type(5, 100));
		CHECK(5 == it->first);
		CHECK(10 == it->second);
	}
	SECTION("ClearZerosMap")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		m.insert(tMap::value_type(6, 10));
		m.clear();
		CHECK(m.empty());
		CHECK(0 == m.size());
		CHECK(m.begin() == m.end());
	}
	SECTION("EraseIfElementPresent")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		m.insert(tMap::value_type(6, 13));
		CHECK(1 == m.erase(5));
		CHECK(1 == m.size());
		tMap::iterator it = m.begin();
		CHECK(it != m.end());
		CHECK(6 == it->first);
		CHECK(13 == it->second);
	}
	SECTION("EraseIfElementAbsent")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		m.insert(tMap::value_type(6, 13));
		CHECK(0 == m.erase(7));
		CHECK(2 == m.size());
	}
	SECTION("SubscriptionOpInsertsIfAbsent")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		m[6] = 13;
		CHECK(2 == m.size());
		tMap::const_iterator it = m.find(6);
		CHECK(13 == it->second);
	}
	SECTION("SubscriptionOpUpdatesIfPresent")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		m[5] = 13;
		CHECK(1 == m.size());
		tMap::const_iterator it = m.find(5);
		CHECK(13 == it->second);
	}
	SECTION("FindReturnsEndOnAbsentElement")
	{
		tMap m;
		m.insert(tMap::value_type(5, 10));
		tMap::const_iterator it = m.find(6);
		CHECK(it == m.end());
	}
	SECTION("ConstructFromRange")
	{
		tMap::value_type data[4] =
		{
			tMap::value_type(1, 1), tMap::value_type(2, 2), tMap::value_type(3, 3),
			tMap::value_type(3, 4)
		};
		tMap m(&data[0], &data[4]);
		CHECK(3 == m.size());	// last entry has duplicated key, shouldn't get inserted
		CHECK(3 == m[3]);
	}

	tMap::value_type data4[4] =
	{
		tMap::value_type(1, 1), tMap::value_type(2, 2), tMap::value_type(3, 3),
		tMap::value_type(4, 4)
	};
	SECTION("IteratorIteratesAllElements")
	{
		bool hadElement[4] = { false, false, false, false };
		tMap m(&data4[0], &data4[4]);
		tMap::const_iterator it = m.begin();
		for (/**/; it != m.end(); ++it)
		{
			CHECK(it->first >= 1); CHECK(it->first <= 4);
			hadElement[it->first - 1] = true;
		}
		for (int i = 0; i < 4; ++i)
		{
			CHECK(hadElement[i]);
		}
	}
	SECTION("IteratorDoesntIterateOverDeletedElements")
	{
		tMap m(&data4[0], &data4[4]);
		m.erase(2);
		bool hadElement[4] = { false, false, false, false };
		tMap::const_iterator it = m.begin();
		for (/**/; it != m.end(); ++it)
		{
			CHECK(it->first >= 1); CHECK(it->first <= 4);
			hadElement[it->first - 1] = true;
		}
		for (int i = 0; i < 4; ++i)
		{
			CHECK((i == 1 || hadElement[i]));
		}
	}
	SECTION("SwapSwaps")
	{
		tMap m1(&data4[0], &data4[4]);
		tMap m2;
		m1.swap(m2);
		CHECK(m1.empty());
		CHECK(m1.begin() == m1.end());

		CHECK(4 == m2.size());
		bool hadElement[4] = { false, false, false, false };
		tMap::const_iterator it = m2.begin();
		for (/**/; it != m2.end(); ++it)
		{
			CHECK(it->first >= 1); CHECK(it->first <= 4);
			hadElement[it->first - 1] = true;
		}
		for (int i = 0; i < 4; ++i)
		{
			CHECK(hadElement[i]);
		}
	}
}
}
