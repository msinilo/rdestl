#include "vendor/Catch/catch.hpp"
#include "rdestl/intrusive_list.h"

namespace
{
	struct MyNode : public rde::intrusive_list_node
	{
		explicit MyNode(int i = 0): data(i) {}
		int	data;
	};

TEST_CASE("intrusive_list", "[list]")
{
	SECTION("DefaultCtorConstructsEmptyList")
	{
		rde::intrusive_list<MyNode> l;
		CHECK(l.empty());
		CHECK(0 == l.size());
	}
	SECTION("PushBack")
	{
		rde::intrusive_list<MyNode> l;
		MyNode node;
		node.data = 5;
		l.push_back(&node);
		CHECK(1 == l.size());
		MyNode node2;
		node2.data = 10;
		l.push_back(&node2);
		CHECK(2 == l.size());
		CHECK(10 == l.back()->data);
	}
	SECTION("PushBackPtr")
	{
		rde::intrusive_list<MyNode> l;
		MyNode* n(new MyNode());
		l.push_back(n);
		CHECK(1 == l.size());
		delete n;
	}

	SECTION("PushFront")
	{
		rde::intrusive_list<MyNode> l;
		MyNode node;
		node.data = 5;
		l.push_front(&node);
		CHECK(1 == l.size());
		MyNode node2;
		node2.data = 10;
		l.push_front(&node2);
		CHECK(2 == l.size());
		CHECK(10 == l.front()->data);
		CHECK(5 == l.back()->data);
	}

	SECTION("PopBack")
	{
		rde::intrusive_list<MyNode> l;
		MyNode node;
		node.data = 5;
		l.push_back(&node);
		CHECK(1 == l.size());
		MyNode node2;
		node2.data = 10;
		l.push_back(&node2);
		CHECK(2 == l.size());
		l.pop_back();
		CHECK(1 == l.size());
		CHECK(5 == l.front()->data);
	}

	SECTION("IterEmpty")
	{
		rde::intrusive_list<MyNode> l;
		CHECK(l.begin() == l.end());
	}
	SECTION("IterTest")
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
		CHECK(5 == it->data);
		CHECK(10 == (++it)->data);

		MyNode* n = new MyNode(15);
		l.insert(it, n);
		CHECK(3 == l.size());
		it = l.begin();
		++it;
		CHECK(15 == it->data);
		CHECK(10 == (++it)->data);
		delete n;
	}
}
}
