	TESTC(ConstructEmpty)
	{
		tTestList lst;
		CHECK(lst.empty());
		CHECK_EQUAL(0, lst.size());
	}
	TESTC(PushFront)
	{
		tTestList lst;
		lst.push_front(5);
		CHECK(!lst.empty());
		CHECK_EQUAL(1, lst.size());
		CHECK_EQUAL(5, lst.front());
		lst.push_front(3);
		CHECK_EQUAL(2, lst.size());
		CHECK_EQUAL(3, lst.front());
	}
	TESTC(PopFront)
	{
		tTestList lst;
		lst.push_front(5);
		lst.push_front(3);
		CHECK(!lst.empty());
		lst.pop_front();
		CHECK_EQUAL(5, lst.front());
		lst.pop_front();
		CHECK(lst.empty());
	}

	TESTC(PushBack)
	{
		tTestList lst;
		lst.push_back(5);
		CHECK(!lst.empty());
		CHECK_EQUAL(1, lst.size());
		CHECK_EQUAL(5, lst.front());
		CHECK_EQUAL(5, lst.back());
		lst.push_back(3);
		CHECK_EQUAL(2, lst.size());
		CHECK_EQUAL(5, lst.front());
		CHECK_EQUAL(3, lst.back());
	}
	TESTC(PopBack)
	{
		tTestList lst;
		lst.push_front(5);
		lst.push_back(3);
		CHECK(!lst.empty());
		lst.pop_back();
		CHECK_EQUAL(5, lst.front());
		lst.pop_back();
		CHECK(lst.empty());
	}
	TESTC(PushPopInsert)
	{
		tTestList lst;
		lst.push_front(5);	// 5
		lst.push_back(3);	// 5 3
		CHECK(!lst.empty());
		lst.pop_back();		// 5
		lst.push_back(6);	// 5 6
		lst.pop_back();		// 5
		lst.push_back(7);	// 5 7
		lst.push_back(8);	// 5 7 8
		lst.pop_front();	// 7 8
		lst.push_front(1);	// 1 7 8
		lst.push_front(9);	// 9 1 7 8
		lst.push_back(2);	// 9 1 7 8 2
		lst.push_back(100);	// 9 1 7 8 2 100
		lst.push_front(10);	// 10 9 1 7 8 2 100
		tTestList::iterator it = lst.begin();
		++it;
		lst.insert(it, 11);	// 10 11 9 1 7 8 2 100
		CHECK_EQUAL(8, lst.size());
		CHECK_EQUAL(10, lst.front());
		CHECK_EQUAL(100, lst.back());
	}

	TESTC(Clear)
	{
		tTestList lst;
		lst.push_front(5);
		CHECK(!lst.empty());
		lst.clear();
		CHECK(lst.empty());
	}

	TESTC(IterEmpty)
	{
		tTestList lst;
		CHECK(lst.begin() == lst.end());
		const tTestList lst2;
		CHECK(lst2.begin() == lst2.end());
	}

	TESTC(IterOneElem)
	{
		tTestList lst;
		lst.push_back(5);
		tTestList::iterator it = lst.begin();
		CHECK(it != lst.end());
		CHECK_EQUAL(5, *it);
	}
	TESTC(IterTraverse)
	{
		tTestList lst;
		lst.push_back(2);
		lst.push_back(3);
		lst.push_back(4);
		lst.push_back(5);
		lst.push_front(1);
		tTestList::const_iterator it = lst.end();
		--it;
		CHECK_EQUAL(5, *it);
		it--;
		CHECK_EQUAL(4, *it);
		it--;
		CHECK_EQUAL(3, *it);
		it--;
		CHECK_EQUAL(2, *it);
		it--;
		CHECK(it == lst.begin());
		CHECK_EQUAL(1, *it);

		++it;
		CHECK_EQUAL(2, *it);
		CHECK_EQUAL(3, *(++it));
		CHECK_EQUAL(4, *(++it));
		it++;
		CHECK_EQUAL(5, *it++);
		CHECK(it == lst.end());
	}
	TESTC(AssignCtor)
	{
		tTestList lst(&array[0], &array[6]);
		CHECK_EQUAL(6, lst.size());
		CHECK_EQUAL(1, lst.front());
		CHECK_EQUAL(36, lst.back());
	}
	TESTC(Insert)
	{
		tTestList lst(&array[0], &array[6]);
		CHECK_EQUAL(6, lst.size());
		tTestList::iterator it = lst.end();
		--it, --it;
		tTestList::iterator it2 = lst.insert(it, 20);
		CHECK_EQUAL(20, *it2);
		CHECK_EQUAL(7, lst.size());
		--it2;
		CHECK_EQUAL(16, *it2);
		++it2, ++it2;
		CHECK_EQUAL(25, *it2);
		it = lst.end(); --it, --it, --it;
		CHECK_EQUAL(20, *it);
	}

	TESTC(Erase)
	{
		tTestList lst(&array[0], &array[6]);
		tTestList::iterator it = lst.begin(); // 1
		++it, ++it, ++it; // 16
		it = lst.erase(it); 
		CHECK_EQUAL(25, *it);
		CHECK_EQUAL(5, lst.size());
		++it;
		CHECK_EQUAL(36, *it);
		--it, --it;
		CHECK_EQUAL(9, *it);
	}
	TESTC(EraseAll)
	{
		tTestList lst(&array[0], &array[6]);
		lst.erase(lst.begin(), lst.end());
		CHECK(lst.empty());
	}
	TESTC(AssignmentOp)
	{
		tTestList lst(&array[0], &array[6]);
		tTestList lst2;
		lst2 = lst;
		CHECK_EQUAL(6, lst2.size());
		CHECK_EQUAL(1, lst2.front());
		CHECK_EQUAL(36, lst2.back());
	}

	TESTC(IterArrowOp)
	{
		struct Foo
		{
			int k;
		};
		rde::list<Foo> lst;
		Foo f;
		f.k = 11;
		lst.push_front(f);
		CHECK_EQUAL(11, lst.front().k);
		CHECK_EQUAL(11, lst.begin()->k);
		CHECK_EQUAL(11, (*lst.begin()).k);
	}
