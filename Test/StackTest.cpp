#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/stack.h"

namespace
{
	TEST(DefaultCtorMakesEmptyStack)
	{
		rde::stack<int> s;
		CHECK(s.empty());
		CHECK_EQUAL(0, s.size());
	}
	TEST(PushIncreasesSize)
	{
		rde::stack<int> s;
		s.push(1);
		CHECK_EQUAL(1, s.size());
		CHECK(!s.empty());
		s.push(2);
		CHECK_EQUAL(2, s.size());
		CHECK(!s.empty());
	}
	TEST(PopDecreasesSize)
	{
		rde::stack<int> s;
		s.push(1);
		s.push(2);
		s.pop();
		CHECK_EQUAL(1, s.size());
		s.pop();
		CHECK_EQUAL(0, s.size());
		CHECK(s.empty());
	}
	TEST(TopReturnsTop)
	{
		rde::stack<int> s;
		s.push(1);
		s.push(2);
		s.push(3);
		s.push(4);
		CHECK_EQUAL(4, s.top()); s.pop();
		CHECK_EQUAL(3, s.top()); s.pop();
		s.push(5);
		CHECK_EQUAL(5, s.top()); s.pop();
		CHECK_EQUAL(2, s.top()); s.pop();
		CHECK_EQUAL(1, s.top()); s.pop();
		CHECK(s.empty());
	}
}
