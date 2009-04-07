#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/fixed_substring.h"

namespace
{
	TEST(DefaultCtorGivesEmptyString)
	{
		rde::fixed_substring<char, 16> str;
		CHECK(str.empty());
	}
	TEST(FromCStrNotEmpty)
	{
		rde::fixed_substring<char, 16> str("Hello world");
		CHECK(!str.empty());
	}
	TEST(FromCStrCopiesContents)
	{
		rde::fixed_substring<char, 16> str("Hello world");
		CHECK_EQUAL(0, rde::strcompare(str.data(), "Hello world"));
	}
	TEST(InequalityOp)
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 9> str2("World");
		CHECK(str2 != str);
	}
	TEST(CopyCtorGivesEqualStrings)
	{
		rde::fixed_substring<char, 16> str("Hello world");
		rde::fixed_substring<char, 16> str2(str);
		CHECK(!str2.empty());
		CHECK(str == str2);
	}
	TEST(Append)
	{
		rde::fixed_substring<char, 9> str("Hello ");
		str.append("world");
		CHECK_EQUAL(0, rde::strcompare(str.data(), "Hello wor"));
	}
	TEST(CopyCtorEqualSizes)
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 9> str2(str);
		CHECK(str == str2);
	}
	TEST(CopyCtorDifferentSizes)
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 8> str2(str);
		CHECK(str == str2);
		rde::fixed_substring<char, 3> str3(str);
		CHECK_EQUAL(0, rde::strcompare(str3.data(), "Hel"));
	}
	TEST(AssignmentOpEqualSizes)
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 9> str2;
		CHECK(str != str2);
		str2 = str;
		CHECK(str == str2);
	}
	TEST(AssignmentOpDifferentSizes)
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 5> str2;
		CHECK(str != str2);
		str2 = str;
		CHECK(str == str2);
	}
}

