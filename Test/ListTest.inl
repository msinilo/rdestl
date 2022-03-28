SECTION("ConstructEmpty")
{
	tTestList lst;
	CHECK(lst.empty());
	CHECK(0 == lst.size());
}
SECTION("PushFront")
{
	tTestList lst;
	lst.push_front(5);
	CHECK(!lst.empty());
	CHECK(1 == lst.size());
	CHECK(5 == lst.front());
	lst.push_front(3);
	CHECK(2 == lst.size());
	CHECK(3 == lst.front());
}
SECTION("PopFront")
{
	tTestList lst;
	lst.push_front(5);
	lst.push_front(3);
	CHECK(!lst.empty());
	lst.pop_front();
	CHECK(5 == lst.front());
	lst.pop_front();
	CHECK(lst.empty());
}

SECTION("PushBack")
{
	tTestList lst;
	lst.push_back(5);
	CHECK(!lst.empty());
	CHECK(1 == lst.size());
	CHECK(5 == lst.front());
	CHECK(5 == lst.back());
	lst.push_back(3);
	CHECK(2 == lst.size());
	CHECK(5 == lst.front());
	CHECK(3 == lst.back());
}
SECTION("PopBack")
{
	tTestList lst;
	lst.push_front(5);
	lst.push_back(3);
	CHECK(!lst.empty());
	lst.pop_back();
	CHECK(5 == lst.front());
	lst.pop_back();
	CHECK(lst.empty());
}
SECTION("PushPopInsert")
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
	CHECK(8 == lst.size());
	CHECK(10 == lst.front());
	CHECK(100 == lst.back());
}

SECTION("Clear")
{
	tTestList lst;
	lst.push_front(5);
	CHECK(!lst.empty());
	lst.clear();
	CHECK(lst.empty());
}

SECTION("IterEmpty")
{
	tTestList lst;
	CHECK(lst.begin() == lst.end());
	const tTestList lst2;
	CHECK(lst2.begin() == lst2.end());
}

SECTION("IterOneElem")
{
	tTestList lst;
	lst.push_back(5);
	tTestList::iterator it = lst.begin();
	CHECK(it != lst.end());
	CHECK(5 == *it);
}
SECTION("IterTraverse")
{
	tTestList lst;
	lst.push_back(2);
	lst.push_back(3);
	lst.push_back(4);
	lst.push_back(5);
	lst.push_front(1);
	tTestList::const_iterator it = lst.end();
	--it;
	CHECK(5 == *it);
	it--;
	CHECK(4 == *it);
	it--;
	CHECK(3 == *it);
	it--;
	CHECK(2 == *it);
	it--;
	CHECK(it == lst.begin());
	CHECK(1 == *it);

	++it;
	CHECK(2 == *it);
	CHECK(3 == *(++it));
	CHECK(4 == *(++it));
	it++;
	CHECK(5 == *it++);
	CHECK(it == lst.end());
}
SECTION("AssignCtor")
{
	tTestList lst(&array[0], &array[6]);
	CHECK(6 == lst.size());
	CHECK(1 == lst.front());
	CHECK(36 == lst.back());
}
SECTION("Insert")
{
	tTestList lst(&array[0], &array[6]);
	CHECK(6 == lst.size());
	tTestList::iterator it = lst.end();
	--it, --it;
	tTestList::iterator it2 = lst.insert(it, 20);
	CHECK(20 == *it2);
	CHECK(7 == lst.size());
	--it2;
	CHECK(16 == *it2);
	++it2, ++it2;
	CHECK(25 == *it2);
	it = lst.end(); --it, --it, --it;
	CHECK(20 == *it);
}

SECTION("Erase")
{
	tTestList lst(&array[0], &array[6]);
	tTestList::iterator it = lst.begin(); // 1
	++it, ++it, ++it; // 16
	it = lst.erase(it);
	CHECK(25 == *it);
	CHECK(5 == lst.size());
	++it;
	CHECK(36 == *it);
	--it, --it;
	CHECK(9 == *it);
}
SECTION("EraseAll")
{
	tTestList lst(&array[0], &array[6]);
	lst.erase(lst.begin(), lst.end());
	CHECK(lst.empty());
}
SECTION("AssignmentOp")
{
	tTestList lst(&array[0], &array[6]);
	tTestList lst2;
	lst2 = lst;
	CHECK(6 == lst2.size());
	CHECK(1 == lst2.front());
	CHECK(36 == lst2.back());
}

SECTION("IterArrowOp")
{
	struct Foo
	{
		int k;
	};
	rde::list<Foo> lst;
	Foo f;
	f.k = 11;
	lst.push_front(f);
	CHECK(11 == lst.front().k);
	CHECK(11 == lst.begin()->k);
	CHECK(11 == (*lst.begin()).k);
}
