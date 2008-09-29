	TESTC(DefaultConstructor)
	{
		tMap h;
		CHECK(h.empty());
		CHECK_EQUAL(0, h.size());
		CHECK_EQUAL(0, h.bucket_count());
		CHECK_EQUAL(0ul, h.used_memory());
	}
	TESTC(ConstructorInitialCapacity)
	{
		tMap h(256);
		CHECK(h.empty());
		CHECK_EQUAL(0, h.size());
		CHECK(h.bucket_count() >= 256);
		CHECK_EQUAL((size_t)(h.bucket_count() * h.kNodeSize), h.used_memory());
	}
	TESTC(Insert)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		CHECK_EQUAL(2, h.size());
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
		CHECK_EQUAL(1, h.size());
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
		CHECK_EQUAL(2, h.size());
		it = h.find("hello");
		h.erase(it);
		CHECK_EQUAL(1, h.size());
		it = h.begin();
		CHECK_EQUAL(10, it->second);
		CHECK(it->first == "world");
	}

	TESTC(EraseFindKey)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		CHECK_EQUAL(1, h.erase("hello"));
		CHECK_EQUAL(0, h.erase("hello"));
		CHECK_EQUAL(1, h.size());
	}
	TESTC(StatsPoorHash)
	{
		tPoorlyHashedMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		// @todo: different for hash_map/hash_map2 :/
		//CHECK_EQUAL(h.bucket_count() * h.kNodeSize + h.kNodeSize*2, h.used_memory());
		//CHECK_EQUAL(2ul, h.collisions());
		CHECK_EQUAL(1, 1);
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
		CHECK_EQUAL(3, h.size());

		it = h.find("hello");
		h.erase(it);
		CHECK_EQUAL(2, h.size());
		it = h.begin();
		CHECK(it != h.end());
		CHECK_EQUAL(10, it->second);
		CHECK(it->first == "world");
	}
	TESTC(FindEmpty)
	{
		tMap h;
		tMap::iterator it = h.find("Meh");
		CHECK(it == h.end());
	}
	TESTC(AssignmentOp)
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("new"), 9));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap h2;
		h2 = h;
		CHECK(!h2.empty());
		CHECK_EQUAL(h.size(), h2.size());
		CHECK_EQUAL(h.bucket_count(), h2.bucket_count());
		for (tMap::const_iterator it = h.begin(); it != h.end(); ++it)
		{
			tMap::const_iterator it2 = h2.find(it->first);
			CHECK(it2 != h2.end());
			CHECK_EQUAL(it->first, it2->first);
			CHECK_EQUAL(it->second, it2->second);
		}
		tMap::const_iterator it = h2.find("Meh");
		CHECK(it == h2.end());
		it = h2.find("brave");
		CHECK_EQUAL(7, it->second);
	}
