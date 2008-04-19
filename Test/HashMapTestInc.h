	TESTC(DefaultConstructor)
	{
		tMap h;
		CHECK(h.empty());
		CHECK_EQUAL(0ul, h.size());
		CHECK_EQUAL(0ul, h.bucket_count());
		CHECK_EQUAL(0ul, h.used_memory());
	}
	TESTC(ConstructorInitialCapacity)
	{
		tMap h(256);
		CHECK(h.empty());
		CHECK_EQUAL(0ul, h.size());
		CHECK(h.bucket_count() >= 256);
		CHECK_EQUAL(h.bucket_count() * h.kNodeSize, h.used_memory());
	}
	TESTC(Insert)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		CHECK_EQUAL(2ul, h.size());
		CHECK(!h.empty());
		CHECK(h.bucket_count() > 0);
	}
	TESTC(Clear)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		CHECK(!h.empty());
		h.clear();
		CHECK(h.empty());
	}

	TESTC(IterEmpty)
	{
		tMap h;
		CHECK(h.begin() == h.end());
	}
	TESTC(IterTraverse)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap::iterator it = h.begin();
		CHECK(it != h.end());
		++it;
		CHECK(it != h.end());
		CHECK(it->second == 5 || it->second == 10);
		++it;
		CHECK(it == h.end());
	}
	TESTC(EraseSimple)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap::iterator it = h.begin();
		h.erase(it);
		CHECK_EQUAL(1ul, h.size());
	}
	TESTC(Find)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap::iterator it = h.find("brave");
		CHECK(it != h.end());
		CHECK_EQUAL(7, it->second);
		it = h.find("uhhuh");
		CHECK(it == h.end());
		it = h.find("BrAvE");
		CHECK(it == h.end());
	}
	TESTC(EraseFind)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
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

	TESTC(EraseFindKey)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		CHECK_EQUAL(1ul, h.erase("hello"));
		CHECK_EQUAL(0ul, h.erase("hello"));
		CHECK_EQUAL(1ul, h.size());
	}
	TESTC(StatsPoorHash)
	{
		tPoorlyHashedMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		// @todo: different for hash_map/hash_map2 :/
		//CHECK_EQUAL(h.bucket_count() * h.kNodeSize + h.kNodeSize*2, h.used_memory());
		CHECK_EQUAL(2ul, h.collisions());
	}
	TESTC(IterPoorHash)
	{
		tPoorlyHashedMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		tPoorlyHashedMap::iterator it = h.begin();
		CHECK(it != h.end());
		++it;
		CHECK(it != h.end());
		CHECK(it->second == 5 || it->second == 10);
		++it;
		CHECK(it == h.end());
	}
	TESTC(ErasePoorHash)
	{
		tPoorlyHashedMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		h.insert(rde::make_pair(std::string("crashtest"), 15));
		tPoorlyHashedMap::iterator it = h.find("brave");
		h.erase(it);
		CHECK_EQUAL(3ul, h.size());

		it = h.find("hello");
		h.erase(it);
		CHECK_EQUAL(2ul, h.size());
		it = h.begin();
		CHECK(it != h.end());
		CHECK_EQUAL(10, it->second);
		CHECK(it->first == "world");
	}
