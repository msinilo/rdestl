#include "vendor/Catch/catch.hpp"
#include "rb_tree.h"
#include <map>

namespace
{
	template<typename T> char HasKeyType(...);
	template<typename T> int HasKeyType(typename T::key_type*);

	bool hasKeyType = sizeof(HasKeyType<std::map<int, int> >(0)) != sizeof(char);

	void PrintNode(rde::rb_tree<int>::node* n, int left, int depth)
	{
		static const char* s_left[] ={ "[root]", "right", "left" };
		printf("%*s %d: Node %d, [%s, %s]\n", (depth*2), "", depth, n->value.key,
			s_left[left + 1], n->color == 0 ? "red" : "black");
	}

TEST_CASE("rb_tree", "[map][algorithm]")
{
	SECTION("DefaultCtorEmptyTree")
	{
		rde::rb_tree<int> t;
		CHECK(t.empty());
		CHECK(0 == t.size());
	}
	SECTION("InsertIncreasesSize")
	{
		rde::rb_tree<int> t;
		t.insert(5);
		CHECK(1 == t.size());
	}

	SECTION("InsertMore")
	{
		rde::rb_tree<int> t;
		t.insert(10);
		t.insert(14);
		t.insert(2);
		t.insert(15);
		t.insert(1);
		t.insert(7);
		t.insert(5);
		t.insert(8);
		CHECK(8 == t.size());
		t.traverse(PrintNode);
	}

	SECTION("Erase")
	{
		rde::rb_tree<int> t;
		t.insert(10);
		t.insert(14);
		t.insert(2);
		t.insert(15);
		t.insert(1);
		t.insert(7);
		t.insert(5);
		t.insert(8);
		CHECK(8 == t.size());
		t.erase(2);
		CHECK(7 == t.size());
		t.traverse(PrintNode);
	}
	SECTION("ClearMakesEmpty")
	{
		rde::rb_tree<int> t;
		t.insert(10);
		t.insert(14);
		t.insert(2);
		t.insert(15);
		t.insert(1);
		t.insert(7);
		t.insert(5);
		t.insert(8);
		CHECK(8 == t.size());
		t.clear();
		CHECK(0 == t.size());
		CHECK(t.empty());
	}


	// Make find_next_node public to test it.
	/*
	SECTION("IterationFromSmallest")
	{
		rde::rb_tree<int> t;
		t.insert(10); t.insert(14);	t.insert(2);
		t.insert(15); t.insert(1); t.insert(7);
		t.insert(5); t.insert(8);
		rde::rb_tree<int>::node* n(t.get_begin_node());
		CHECK(1 == n->key);
		n = t.find_next_node(n); CHECK(2 == n->key);
		n = t.find_next_node(n); CHECK(5 == n->key);
		n = t.find_next_node(n); CHECK(7 == n->key);
		n = t.find_next_node(n); CHECK(8 == n->key);
		n = t.find_next_node(n); CHECK(10 == n->key);
		n = t.find_next_node(n); CHECK(14 == n->key);
		n = t.find_next_node(n); CHECK(15 == n->key);
		n = t.find_next_node(n);
		CHECK(n == 0);
	}
	*/
}
}
