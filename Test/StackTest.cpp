#include "stack.h"
#include "vendor/Catch/catch.hpp"

namespace
{
TEST_CASE("stack", "[allocator]")
{
	SECTION("DefaultCtorMakesEmptyStack")
	{
		rde::stack<int> s;
		CHECK(s.empty());
		CHECK(0 == s.size());
	}
	SECTION("PushIncreasesSize")
	{
		rde::stack<int> s;
		s.push(1);
		CHECK(1 == s.size());
		CHECK(!s.empty());
		s.push(2);
		CHECK(2 == s.size());
		CHECK(!s.empty());
	}
	SECTION("PopDecreasesSize")
	{
		rde::stack<int> s;
		s.push(1);
		s.push(2);
		s.pop();
		CHECK(1 == s.size());
		s.pop();
		CHECK(0 == s.size());
		CHECK(s.empty());
	}
	SECTION("TopReturnsTop")
	{
		rde::stack<int> s;
		s.push(1);
		s.push(2);
		s.push(3);
		s.push(4);
		CHECK(4 == s.top()); s.pop();
		CHECK(3 == s.top()); s.pop();
		s.push(5);
		CHECK(5 == s.top()); s.pop();
		CHECK(2 == s.top()); s.pop();
		CHECK(1 == s.top()); s.pop();
		CHECK(s.empty());
	}
}
} //namespace
