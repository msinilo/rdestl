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
}

