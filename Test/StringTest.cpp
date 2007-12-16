#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/string.h"
#include <cstdio>

namespace
{
	TEST(EmptyString)
	{
		rdestl::string<char> s;
		CHECK(s.empty());
		CHECK_EQUAL(0ul, s.length());
	}
	TEST(LenOfCString)
	{
		rdestl::string<char> s("alamakota");
		CHECK(!s.empty());
		CHECK_EQUAL(9ul, s.length());
	}
	TEST(CopyCtorLen)
	{
		rdestl::string<char> s("alamakota");
		rdestl::string<char> s2(s);
		CHECK_EQUAL(9ul, s2.length());
		CHECK_EQUAL('a', s2[0]);
		CHECK_EQUAL('l', s2[1]);
		CHECK_EQUAL('a', s2[8]);
	}
	TEST(Compare)
	{
		rdestl::string<char> s("alamakota");
		rdestl::string<char> s2("alamakota");
		CHECK(s.compare("ala") != 0);
		CHECK_EQUAL(0, s.compare(s2));
		rdestl::string<char> s3("alamakot");
		CHECK_EQUAL(+1, s.compare(s3));
		CHECK_EQUAL(-1, s3.compare(s2));
	}
	TEST(EqOp)
	{
		rdestl::string<char> s("alamakota");
		rdestl::string<char> s2(s);
		CHECK(s == s2);
	}
	TEST(Substring)
	{
		rdestl::string<char> s("alamakota");
		rdestl::string<char> s2(s.substr(0, 3));
		CHECK(s2.compare("ala") == 0);
	}
}
