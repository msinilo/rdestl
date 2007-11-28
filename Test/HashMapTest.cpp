#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/hash_map.h"
#include <cstdio>

namespace
{
	struct hasher
	{
		size_t operator()(const std::string& s) const
		{
			size_t len = s.length();
			size_t hash(0);
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
		size_t operator()(const std::string&) const
		{
			return 1;
		}
	};
	typedef rdestl::hash_map<std::string, int, hasher> tMap;

	TEST(DefaultConstructor)
	{
		tMap h;
		CHECK(h.empty());
		CHECK_EQUAL(0ul, h.size());
		CHECK_EQUAL(0ul, h.bucket_count());
		CHECK_EQUAL(0ul, h.used_memory());
	}
	TEST(ConstructorInitialCapacity)
	{
		tMap h(100);
		CHECK(h.empty());
		CHECK_EQUAL(0ul, h.size());
		CHECK(h.bucket_count() >= 100);
		CHECK_EQUAL(h.bucket_count() * h.kNodeSize, h.used_memory());
	}
	TEST(Insert)
	{
		tMap h(100);
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		CHECK_EQUAL(2ul, h.size());
		CHECK(!h.empty());
		CHECK(h.bucket_count() > 0);
	}
	TEST(Clear)
	{
		tMap h(100);
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		CHECK(!h.empty());
		h.clear();
		CHECK(h.empty());
	}
	TEST(IterEmpty)
	{
		tMap h(100);
		CHECK(h.begin() == h.end());
	}
	TEST(IterTraverse)
	{
		tMap h(100);
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		tMap::iterator it = h.begin();
		CHECK(it != h.end());
		++it;
		CHECK(it != h.end());
		CHECK(it->second == 5 || it->second == 10);
		++it;
		CHECK(it == h.end());
	}
	TEST(EraseSimple)
	{
		tMap h(100);
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		tMap::iterator it = h.begin();
		h.erase(it);
		CHECK_EQUAL(1ul, h.size());
	}
	TEST(Find)
	{
		tMap h(100);
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("brave"), 7));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		tMap::iterator it = h.find("brave");
		CHECK(it != h.end());
		CHECK_EQUAL(7, it->second);
	}
	TEST(EraseFind)
	{
		tMap h(100);
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("brave"), 7));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		tMap::iterator it = h.find("brave");
		h.erase(it);
		CHECK_EQUAL(2ul, h.size());
		it = h.find("hello");
		h.erase(it);
		CHECK_EQUAL(1ul, h.size());
		it = h.begin();
		CHECK_EQUAL(10, it->second);
		CHECK(it->first == "world");
	}
	TEST(EraseFindKey)
	{
		tMap h(100);
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("brave"), 7));
		CHECK_EQUAL(1ul, h.erase("hello"));
		CHECK_EQUAL(0ul, h.erase("hello"));
		CHECK_EQUAL(1ul, h.size());
	}
	TEST(StatsPoorHash)
	{
		rdestl::hash_map<std::string, int, poor_hasher> h;
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("brave"), 7));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		CHECK_EQUAL(h.bucket_count() * h.kNodeSize + h.kNodeSize*2, h.used_memory());
		CHECK_EQUAL(2ul, h.collisions());
	}
	TEST(IterPoorHash)
	{
		rdestl::hash_map<std::string, int, poor_hasher> h;
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		rdestl::hash_map<std::string, int, poor_hasher>::iterator it = h.begin();
		CHECK(it != h.end());
		++it;
		CHECK(it != h.end());
		CHECK(it->second == 5 || it->second == 10);
		++it;
		CHECK(it == h.end());
	}
	TEST(ErasePoorHash)
	{
		rdestl::hash_map<std::string, int, poor_hasher> h;
		h.insert(rdestl::make_pair(std::string("hello"), 5));
		h.insert(rdestl::make_pair(std::string("brave"), 7));
		h.insert(rdestl::make_pair(std::string("world"), 10));
		rdestl::hash_map<std::string, int, poor_hasher>::iterator it = h.find("brave");
		h.erase(it);
		CHECK_EQUAL(2ul, h.size());

		it = h.find("hello");
		h.erase(it);
		CHECK_EQUAL(1ul, h.size());
		it = h.begin();
		CHECK(it != h.end());
		CHECK_EQUAL(10, it->second);
		CHECK(it->first == "world");
	}
}
