#include "vendor/Catch/catch.hpp"
#include "rdestl/rde_string.h"
#include "rdestl/cow_string_storage.h"
#include <cstdio>

namespace
{
	typedef rde::basic_string<char, rde::allocator, rde::simple_string_storage<char, rde::allocator>>	simple_string;
	typedef rde::basic_string<char, rde::allocator, rde::cow_string_storage<char, rde::allocator>>		cow_string;

#define STRING_CLASS	simple_string

	TEST_CASE("simple string", "[string]")
	{
#include "StringTestInc.h"
	}

#undef STRING_CLASS
#define STRING_CLASS	cow_string

	TEST_CASE("cow string", "[string]")
	{
#include "StringTestInc.h"
	}

#undef STRING_CLASS
}
