#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/basic_string.h"
#include <cstdio>

namespace
{
	TEST(EmptyString)
	{
		rdestl::string s;
		CHECK(s.empty());
		CHECK_EQUAL(0ul, s.length());
	}
	TEST(LenOfCString)
	{
		rdestl::string s("alamakota");
		CHECK(!s.empty());
		CHECK_EQUAL(9ul, s.length());
	}
	TEST(CopyCtorLen)
	{
		rdestl::string s("alamakota");
		rdestl::string s2(s);
		CHECK_EQUAL(9ul, s2.length());
		CHECK_EQUAL('a', s2[0]);
		CHECK_EQUAL('l', s2[1]);
		CHECK_EQUAL('a', s2[8]);
	}
	TEST(Compare)
	{
		rdestl::string s("alamakota");
		rdestl::string s2("alamakota");
		CHECK(s.compare("ala") != 0);
		CHECK_EQUAL(0, s.compare(s2));
		rdestl::string s3("alamakot");
		CHECK_EQUAL(+1, s.compare(s3));
		CHECK_EQUAL(-1, s3.compare(s2));
	}
	TEST(EqOp)
	{
		rdestl::string s("alamakota");
		rdestl::string s2(s);
		CHECK(s == s2);
	}

	TEST(AssignString)
	{
		rdestl::string s;
		rdestl::string s2("ala");
		s = s2;
		CHECK_EQUAL('a', s[0]);
		CHECK_EQUAL('l', s[1]);
		CHECK_EQUAL('a', s[2]);
		CHECK_EQUAL(0, s.compare(s2));
		CHECK_EQUAL(0, s.compare("ala"));
		CHECK(s == s2);
	}
	TEST(AssignCString)
	{
		rdestl::string s;
		s = "alamakota";
		CHECK_EQUAL(0, s.compare("alamakota"));
		CHECK_EQUAL(9ul, s.length());
	}

	TEST(Substring)
	{
		rdestl::string s("alamakota");
		rdestl::string s2(s.substr(0, 3));
		CHECK(s2.compare("ala") == 0);
	}

	TEST(AppendStr)
	{
		rdestl::string s("ala");
		rdestl::string scopy(s);
		rdestl::string s2("makota");
		s.append(s2);
		CHECK(s.compare("alamakota") == 0);
		CHECK(s2.compare("makota") == 0);
		CHECK(scopy.compare("ala") == 0);
	}
	TEST(AppendCStr)
	{
		rdestl::string s("ala");
		rdestl::string scopy(s);
		s.append("-ma-kota");
		CHECK(s.compare("ala-ma-kota") == 0);
	}
	TEST(AppendStrNoMakeUnique)
	{
		rdestl::string s("ala");
		rdestl::string s2("makota");
		s.append(s2);
		CHECK(s.compare("alamakota") == 0);
		CHECK(s2.compare("makota") == 0);
	}
}
