#include "fixed_substring.h"
#include "vendor/Catch/catch.hpp"

namespace
{
TEST_CASE("fixed_substring", "[string]")
{
	SECTION("DefaultCtorGivesEmptyString")
	{
		rde::fixed_substring<char, 16> str;
		CHECK(str.empty());
	}
	SECTION("FromCStrNotEmpty")
	{
		rde::fixed_substring<char, 16> str("Hello world");
		CHECK(!str.empty());
	}
	SECTION("FromCStrCopiesContents")
	{
		rde::fixed_substring<char, 16> str("Hello world");
		CHECK(0 == rde::strcompare(str.data(), "Hello world"));
	}
	SECTION("InequalityOp")
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 9> str2("World");
		CHECK(str2 != str);
	}
	SECTION("CopyCtorGivesEqualStrings")
	{
		rde::fixed_substring<char, 16> str("Hello world");
		rde::fixed_substring<char, 16> str2(str);
		CHECK(!str2.empty());
		CHECK(str == str2);
	}
	SECTION("Append")
	{
		rde::fixed_substring<char, 9> str("Hello ");
		str.append("world");
		CHECK(0 == rde::strcompare(str.data(), "Hello wor"));
	}
	SECTION("CopyCtorEqualSizes")
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 9> str2(str);
		CHECK(str == str2);
	}
	SECTION("CopyCtorDifferentSizes")
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 8> str2(str);
		CHECK(str == str2);
		rde::fixed_substring<char, 3> str3(str);
		CHECK(0 == rde::strcompare(str3.data(), "Hel"));
	}
	SECTION("AssignmentOpEqualSizes")
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 9> str2;
		CHECK(str != str2);
		str2 = str;
		CHECK(str == str2);
	}
	SECTION("AssignmentOpDifferentSizes")
	{
		rde::fixed_substring<char, 9> str("Hello");
		rde::fixed_substring<char, 5> str2;
		CHECK(str != str2);
		str2 = str;
		CHECK(str == str2);
	}
}
} //namespace
