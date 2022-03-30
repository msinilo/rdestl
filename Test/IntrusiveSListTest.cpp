#include "intrusive_slist.h"
#include "vendor/Catch/catch.hpp"

namespace
{
struct MyNode: public rde::intrusive_slist_node
{
	explicit MyNode(int i = 0): data(i) { }
	int	data;
};

TEST_CASE("intrusive_slist: DefaultCtorConstructsEmptyList", "[intrusive_slist]")
{
	rde::intrusive_slist<MyNode> l;
	CHECK(l.empty());
	CHECK(0 == l.size());
}
TEST_CASE("intrusive_slist: PushFront", "[intrusive_slist]")
{
	rde::intrusive_slist<MyNode> l;
	MyNode node;
	node.data = 5;
	l.push_front(&node);
	CHECK(1 == l.size());
	MyNode node2;
	node2.data = 10;
	l.push_front(&node2);
	CHECK(2 == l.size());
	CHECK(10 == l.front()->data);
}
TEST_CASE("intrusive_slist: PopFront", "[intrusive_slist]")
{
	rde::intrusive_slist<MyNode> l;
	MyNode node;
	node.data = 5;
	l.push_front(&node);
	MyNode node2;
	node2.data = 10;
	l.push_front(&node2);
	MyNode node3;
	node3.data = 15;
	l.push_front(&node3);
	CHECK(3 == l.size());
	l.pop_front();
	CHECK(2 == l.size());
	CHECK(10 == l.front()->data);
	l.pop_front();
	CHECK(1 == l.size());
	CHECK(5 == l.front()->data);
	l.pop_front();
	CHECK(l.empty());
	CHECK(0 == l.size());
}
TEST_CASE("intrusive_slist: Previous", "[intrusive_slist]")
{
	rde::intrusive_slist<MyNode> l;
	MyNode node;
	node.data = 5;
	l.push_front(&node);
	MyNode node2;
	node2.data = 10;
	l.push_front(&node2);
	rde::intrusive_slist<MyNode>::iterator it = l.begin();
	++it;
	rde::intrusive_slist<MyNode>::iterator itPrev = l.previous(it);
	CHECK(itPrev == l.begin());
}
TEST_CASE("intrusive_slist: Clear", "[intrusive_slist]")
{
	rde::intrusive_slist<MyNode> l;
	MyNode n0(5); l.push_front(&n0);
	MyNode n1(6); l.push_front(&n1);
	MyNode n2(7); l.push_front(&n2);
	MyNode n3(8); l.push_front(&n3);
	CHECK(4 == l.size());
	l.clear();
	CHECK(l.empty());
	CHECK(0 == l.size());
}
TEST_CASE("intrusive_slist: GetIterator", "[intrusive_slist]")
{
	rde::intrusive_slist<MyNode> l;
	MyNode n0(5); l.push_front(&n0);
	MyNode n1(6); l.push_front(&n1);
	MyNode* n(new MyNode(7));
	l.push_front(n);
	MyNode n2(8); l.push_front(&n2);
	rde::intrusive_slist<MyNode>::iterator it = l.get_iterator(n);
	CHECK(7 == it->data);
	delete n;
}
} //namespace
