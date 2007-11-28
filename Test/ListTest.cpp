#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/list.h"

namespace
{
	const int array [] = { 1, 4, 9, 16, 25, 36 }; 
	TEST(ConstructEmpty)
	{
		rdestl::list<int> lst;
		CHECK(lst.empty());
		CHECK_EQUAL(0ul, lst.size());
	}
	TEST(PushFront)
	{
		rdestl::list<int> lst;
		lst.push_front(5);
		CHECK(!lst.empty());
		CHECK_EQUAL(1ul, lst.size());
		CHECK_EQUAL(5, lst.front());
		lst.push_front(3);
		CHECK_EQUAL(2ul, lst.size());
		CHECK_EQUAL(3, lst.front());
	}
	TEST(PopFront)
	{
		rdestl::list<int> lst;
		lst.push_front(5);
		lst.push_front(3);
		CHECK(!lst.empty());
		lst.pop_front();
		CHECK_EQUAL(5, lst.front());
		lst.pop_front();
		CHECK(lst.empty());
	}

	TEST(PushBack)
	{
		rdestl::list<int> lst;
		lst.push_back(5);
		CHECK(!lst.empty());
		CHECK_EQUAL(1ul, lst.size());
		CHECK_EQUAL(5, lst.front());
		CHECK_EQUAL(5, lst.back());
		lst.push_back(3);
		CHECK_EQUAL(2ul, lst.size());
		CHECK_EQUAL(5, lst.front());
		CHECK_EQUAL(3, lst.back());
	}
	TEST(PopBack)
	{
		rdestl::list<int> lst;
		lst.push_front(5);
		lst.push_back(3);
		CHECK(!lst.empty());
		lst.pop_back();
		CHECK_EQUAL(5, lst.front());
		lst.pop_back();
		CHECK(lst.empty());
	}

	TEST(Clear)
	{
		rdestl::list<int> lst;
		lst.push_front(5);
		CHECK(!lst.empty());
		lst.clear();
		CHECK(lst.empty());
	}

	TEST(IterEmpty)
	{
		rdestl::list<int> lst;
		CHECK(lst.begin() == lst.end());
		const rdestl::list<int> lst2;
		CHECK(lst2.begin() == lst2.end());
	}

	TEST(IterOneElem)
	{
		rdestl::list<int> lst;
		lst.push_back(5);
		rdestl::list<int>::iterator it = lst.begin();
		CHECK(it != lst.end());
		CHECK_EQUAL(5, *it);
	}
	TEST(IterTraverse)
	{
		rdestl::list<int> lst;
		lst.push_back(2);
		lst.push_back(3);
		lst.push_back(4);
		lst.push_back(5);
		lst.push_front(1);
		rdestl::list<int>::const_iterator it = lst.end();
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
	TEST(AssignCtor)
	{
		rdestl::list<int> lst(&array[0], &array[6]);
		CHECK_EQUAL(6ul, lst.size());
		CHECK_EQUAL(1, lst.front());
		CHECK_EQUAL(36, lst.back());
	}
	TEST(Insert)
	{
		rdestl::list<int> lst(&array[0], &array[6]);
		rdestl::list<int>::iterator it = lst.end();
		--it, --it;
		rdestl::list<int>::iterator it2 = lst.insert(it, 20);
		CHECK_EQUAL(20, *it2);
		CHECK_EQUAL(7ul, lst.size());
		--it2;
		CHECK_EQUAL(16, *it2);
		++it2, ++it2;
		CHECK_EQUAL(25, *it2);
		it = lst.end(); --it, --it, --it;
		CHECK_EQUAL(20, *it);
	}

	TEST(Erase)
	{
		rdestl::list<int> lst(&array[0], &array[6]);
		rdestl::list<int>::iterator it = lst.begin(); // 1
		++it, ++it, ++it; // 16
		it = lst.erase(it); 
		CHECK_EQUAL(25, *it);
		CHECK_EQUAL(5ul, lst.size());
		++it;
		CHECK_EQUAL(36, *it);
		--it, --it;
		CHECK_EQUAL(9, *it);
	}
	TEST(EraseAll)
	{
		rdestl::list<int> lst(&array[0], &array[6]);
		lst.erase(lst.begin(), lst.end());
		CHECK(lst.empty());
	}

	TEST(IterArrowOp)
	{
		struct Foo
		{
			int k;
		};
		rdestl::list<Foo> lst;
		Foo f;
		f.k = 11;
		lst.push_front(f);
		CHECK_EQUAL(11, lst.front().k);
		CHECK_EQUAL(11, lst.begin()->k);
		CHECK_EQUAL(11, (*lst.begin()).k);
	}
}

