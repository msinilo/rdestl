#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/intrusive_list.h"

namespace
{
	struct MyNode : public rde::intrusive_list_node
	{
		explicit MyNode(int i = 0): data(i) {}
		int	data;
	};

	TEST(DefaultCtorConstructsEmptyList)
	{
		rde::intrusive_list<MyNode> l;
		CHECK(l.empty());
		CHECK_EQUAL(0, l.size());
	}
	TEST(PushBack)
	{
		rde::intrusive_list<MyNode> l;
		MyNode node;
		node.data = 5;
		l.push_back(&node);
		CHECK_EQUAL(1, l.size());
		MyNode node2;
		node2.data = 10;
		l.push_back(&node2);
		CHECK_EQUAL(2, l.size());
		CHECK_EQUAL(10, l.back()->data);
	}
	TEST(PushBackPtr)
	{
		rde::intrusive_list<MyNode> l;
		MyNode* n(new MyNode());
		l.push_back(n);
		CHECK_EQUAL(1, l.size());
		delete n;
	}

	TEST(PushFront)
	{
		rde::intrusive_list<MyNode> l;
		MyNode node;
		node.data = 5;
		l.push_front(&node);
		CHECK_EQUAL(1, l.size());
		MyNode node2;
		node2.data = 10;
		l.push_front(&node2);
		CHECK_EQUAL(2, l.size());
		CHECK_EQUAL(10, l.front()->data);
		CHECK_EQUAL(5, l.back()->data);
	}

	TEST(PopBack)
	{
		rde::intrusive_list<MyNode> l;
		MyNode node;
		node.data = 5;
		l.push_back(&node);
		CHECK_EQUAL(1, l.size());
		MyNode node2;
		node2.data = 10;
		l.push_back(&node2);
		CHECK_EQUAL(2, l.size());
		l.pop_back();
		CHECK_EQUAL(1, l.size());
		CHECK_EQUAL(5, l.front()->data);
	}

	TEST(IterEmpty)
	{
		rde::intrusive_list<MyNode> l;
		CHECK(l.begin() == l.end());
	}
	TEST(IterTest)
	{
		rde::intrusive_list<MyNode> l;
		MyNode node;
		node.data = 5;
		l.push_back(&node);
		rde::intrusive_list<MyNode>::iterator it(l.begin());
		CHECK(it != l.end());
		++it;
		CHECK(it == l.end());
		--it;
		CHECK(it != l.end());
		it++;
		CHECK(it == l.end());
		it--;
		CHECK(it != l.end());

		MyNode node2;
		node2.data = 10;
		l.push_back(&node2);
		it = l.begin();
		CHECK_EQUAL(5, it->data);
		CHECK_EQUAL(10, (++it)->data);

		MyNode* n = new MyNode(15);
		l.insert(it, n);
		CHECK_EQUAL(3, l.size());
		it = l.begin();
		++it;
		CHECK_EQUAL(15, it->data);
		CHECK_EQUAL(10, (++it)->data);
		delete n;
	}
}

