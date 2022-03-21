	SECTION("DefaultConstructor")
	{
		tMap h;
		CHECK(h.empty());
		CHECK(0 == h.size());
		CHECK(0 == h.bucket_count());
		CHECK(0 == h.used_memory());
	}
	SECTION("ConstructorInitialCapacity")
	{
		tMap h(256);
		CHECK(h.empty());
		CHECK(0 == h.size());
		CHECK(h.bucket_count() >= 256);
		CHECK((h.bucket_count() * h.kNodeSize) == h.used_memory());
	}
	SECTION("Insert")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		CHECK(2 == h.size());
		CHECK(!h.empty());
		CHECK(h.bucket_count() > 0);
	}

    SECTION("Emplace")
    {
        tMap h;
        h.emplace(std::string("hello"), 5);
        h.emplace(std::string("world"), 10);
        CHECK(2 == h.size());
        CHECK(!h.empty());
        CHECK(h.bucket_count() > 0);
    }

	SECTION("Clear")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		CHECK(!h.empty());
		h.clear();
		CHECK(h.empty());
	}

	SECTION("IterEmpty")
	{
		tMap h;
		CHECK(h.begin() == h.end());
	}
	SECTION("IterTraverse")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap::iterator it = h.begin();
		CHECK(it != h.end());
		++it;
		CHECK(it != h.end());
		CHECK((it->second == 5 || it->second == 10));
		++it;
		CHECK(it == h.end());
	}
	SECTION("EraseSimple")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap::iterator it = h.begin();
		h.erase(it);
		CHECK(1 == h.size());
	}
	SECTION("Find")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap::iterator it = h.find("brave");
		CHECK(it != h.end());
		CHECK(7 == it->second);
		it = h.find("uhhuh");
		CHECK(it == h.end());
		it = h.find("BrAvE");
		CHECK(it == h.end());
	}
	SECTION("EraseFind")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap::iterator it = h.find("brave");
		h.erase(it);
		CHECK(2 == h.size());
		it = h.find("hello");
		h.erase(it);
		CHECK(1 == h.size());
		it = h.begin();
		CHECK(10 == it->second);
		CHECK(it->first == "world");
	}

	SECTION("EraseFindKey")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		CHECK(1 == h.erase("hello"));
		CHECK(0 == h.erase("hello"));
		CHECK(1 == h.size());
	}
	SECTION("StatsPoorHash")
	{
		tPoorlyHashedMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		CHECK(h.bucket_count() * h.kNodeSize == h.used_memory());
		CHECK(3 == h.nonempty_bucket_count());	// every entry in own bucket.
		//CHECK(2ul == h.collisions());
	}
	SECTION("IterPoorHash")
	{
		tPoorlyHashedMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("world"), 10));
		tPoorlyHashedMap::iterator it = h.begin();
		CHECK(it != h.end());
		++it;
		CHECK(it != h.end());
		CHECK((it->second == 5 || it->second == 10));
		++it;
		CHECK(it == h.end());
	}
	SECTION("ErasePoorHash")
	{
		tPoorlyHashedMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		h.insert(rde::make_pair(std::string("crashtest"), 15));
		tPoorlyHashedMap::iterator it = h.find("brave");
		h.erase(it);
		CHECK(3 == h.size());

		it = h.find("hello");
		h.erase(it);
		CHECK(2 == h.size());
		it = h.begin();
		CHECK(it != h.end());
		CHECK(10 == it->second);
		CHECK(it->first == "world");
	}
	// Special case reported via blog.
	SECTION("InsertAfterEraseAndClear")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("world"), 10));
		h.erase("hello");
		h.clear();
		CHECK(h.empty());
		h.insert(rde::make_pair(std::string("hello"), 5));
		CHECK(1 == h.size());
	}
	SECTION("SwapTest")
	{
		tMap a;
		a.insert(rde::make_pair(std::string("hello"), 5));
		a.insert(rde::make_pair(std::string("brave"), 7));
		a.insert(rde::make_pair(std::string("world"), 10));
		tMap b;
		b.insert(rde::make_pair(std::string("something else"), 12));
		a.swap(b);
		CHECK(1 == a.size());
		CHECK(3 == b.size());
		CHECK(12 == a["something else"]);
		CHECK(5 == b["hello"]);
		CHECK(7 == b.find("brave")->second);
		CHECK(10 == b["world"]);
		CHECK(b.find("something else") == b.end());

		// Make sure swap didn't mess with internal state.
		b.insert(rde::make_pair(std::string("hello2"), 15));
		CHECK(15 == b["hello2"]);
		a.insert(rde::make_pair(std::string("hello"), 25));
		tMap::iterator it = a.find("hello");
		CHECK(it != a.end());
		CHECK(25 == it->second);

	}
	SECTION("SwapEmpty")
	{
		tMap a;
		a.insert(rde::make_pair(std::string("hello"), 5));
		tMap b;
		a.swap(b);
		CHECK(a.empty());
		CHECK(!b.empty());
	}
	SECTION("FindEmpty")
	{
		tMap h;
		tMap::iterator it = h.find("Meh");
		CHECK(it == h.end());
	}
	SECTION("AssignmentOp")
	{
		tMap h;
		h.insert(rde::make_pair(std::string("hello"), 5));
		h.insert(rde::make_pair(std::string("brave"), 7));
		h.insert(rde::make_pair(std::string("new"), 9));
		h.insert(rde::make_pair(std::string("world"), 10));
		tMap h2;
		h2 = h;
		CHECK(!h2.empty());
		CHECK(h.size() == h2.size());
		CHECK(h.bucket_count() == h2.bucket_count());
		for (tMap::const_iterator it = h.begin(); it != h.end(); ++it)
		{
			tMap::const_iterator it2 = h2.find(it->first);
			CHECK(it2 != h2.end());
			CHECK(it->first == it2->first);
			CHECK(it->second == it2->second);
		}
		tMap::const_iterator it = h2.find("Meh");
		CHECK(it == h2.end());
		it = h2.find("brave");
		CHECK(7 == it->second);
	}

    SECTION("Subscript Operator")
    {
        tMap h;
        const std::string strHello("hello");
        h[strHello] = 2;
        h[std::string("world")] = 5;
        h[std::string("test")] = h[strHello] + 10;
        CHECK(!h.empty());
        CHECK(3 == h.size());
        CHECK(h.find(strHello) != h.end());
        CHECK(2 == h.find(strHello)->second);
        CHECK(5 == h.find("world")->second);
        CHECK(12 == h.find("test")->second);
    }

	//-----------------------------------------------------------------
	//Regression test:
	//added by Danushka Abeysuriya (silvermace@gmail.com)
	//See vec_resize_special_a unit test - believe this is a similar problem
	//When an uninitialized/unused hash_map instance is the rhs of a copy-constructor
	//for a init'ed/used hash_map the rehash method falls over because the un-init'd
	//hash_maps node ptr is NULL
	SECTION("HashMapResize_Regression_1")
	{
		rde::hash_map<int,int> a;

		a.insert(rde::pair<int,int>(42,24));
		CHECK(a.size() == 1);

		a = rde::hash_map<int,int>();
		CHECK(a.size() == 0);
	}
