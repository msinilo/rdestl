#include "sstream.h"
#include "vendor/Catch/catch.hpp"

namespace
{
TEST_CASE("sstream", "[string][stream]")
{
	SECTION("Initialization")
	{
		int x(0);

		SECTION("Default ctor")
		{
			rde::stringstream ss;
			CHECK(!ss.good());

			ss >> x;
			CHECK(x == 0);
		}

		SECTION("With value_type ctor")
		{
			const char* ch = "1";
			rde::stringstream ss1(ch);
			CHECK(ss1.good());

			ss1 >> x;
			CHECK(x == 1);

			rde::stringstream ss2("2");
			CHECK(ss2.good());

			ss2 >> x;
			CHECK(x == 2);
		}

		SECTION("With string_type ctor")
		{
			rde::string s("1");
			rde::stringstream ss1(s);
			CHECK(ss1.good());

			ss1 >> x;
			CHECK(x == 1);

			rde::stringstream ss2(rde::string("2"));
			CHECK(ss2.good());

			ss2 >> x;
			CHECK(x == 2);
		}
	}

	SECTION("Good/EOF")
	{
		rde::stringstream ss1("1");
		CHECK(ss1.good() == true);
		CHECK(ss1.eof() == false);

		rde::stringstream ss2;
		CHECK(ss2.good() == false);
		CHECK(ss2.eof() == true);
	}

	SECTION("Whitespace/Trim")
	{
		rde::stringstream ss("1");
		REQUIRE(ss.good() == true);

		SECTION("A non-null string with strlen > 0 containing only whitespace characters != good()")
		{
			ss = rde::stringstream();
			CHECK(ss.good() == false);

			ss = rde::stringstream(" ");
			CHECK(ss.good() == false);

			ss = rde::stringstream("     ");
			CHECK(ss.good() == false);

			ss = rde::stringstream(" \t\r\n");
			CHECK(ss.good() == false);
		}
	}

	SECTION("Reset")
	{
		rde::stringstream ss("1");
		REQUIRE(ss.good() == true);

		ss.reset();
		CHECK(ss.good() == false);

		ss.reset("1");
		CHECK(ss.good() == true);

		ss.reset("");
		CHECK(ss.good() == false);
	}

	// https://github.com/msinilo/rdestl/issues/13
	SECTION("Operator bool and !bool")
	{
		bool isgood = false;
		bool isbad = true;

		SECTION("Implicit bool conversions work as expected")
		{
			rde::stringstream ssGood("1");
			CHECK(ssGood);

			rde::stringstream ssBad("");
			CHECK(!ssBad);

			CHECK(rde::stringstream("1"));
			CHECK(!rde::stringstream(""));
		}

		SECTION("Ternary operator")
		{
			rde::stringstream ssGood("1");
			rde::stringstream ssBad;

			isgood = ssGood ? true : false;
			isbad = !ssGood ? true : false;
			CHECK(isgood == true);
			CHECK(isbad == false);

			isgood = ssBad ? true : false;
			isbad = !ssBad ? true : false;
			CHECK(isgood == false);
			CHECK(isbad == true);
		}

		SECTION("If statement")
		{
			rde::stringstream ssGood("1");
			if (ssGood)
				isgood = true;
			else isgood = false;
			CHECK(isgood == true);

			rde::stringstream ssBad;
			if (ssBad)
				isbad = false;
			else isbad = true;
			CHECK(isbad == true);
		}

		SECTION("Inline assignment inside if statement")
		{
			rde::stringstream ss;

			if ((ss = rde::stringstream("1")))
				isgood = true;
			else isgood = false;
			REQUIRE(ss.good() == true);
			CHECK(isgood == true);

			if (!(ss = rde::stringstream("1")))
				isgood = false;
			else isgood = true;
			REQUIRE(ss.good() == true);
			CHECK(isgood == true);

			if ((ss = rde::stringstream()))
				isbad = false;
			else isbad = true;
			REQUIRE(ss.good() == false);
			CHECK(isbad == true);

			if (!(ss = rde::stringstream()))
				isbad = true;
			else isbad = false;
			REQUIRE(ss.good() == false);
			CHECK(isbad == true);
		}

		SECTION("Illegal comparisons should not compile")
		{
			rde::stringstream ssGood("1");
			rde::stringstream ssBad("");
			bool shouldbefalse = false;

			//
			// NOTE:
			// The following assertions should not compile!
			// If tests build with them uncommented, that means
			// the compiler is incorrectly performing an implicit conversion
			// from `stringstream` to `bool` in order to satisfy an illegal
			// comparison between different types. ~SK
			//

			//shouldbefalse = ssBad == false;
			//shouldbefalse = ssBad != true;
			//shouldbefalse = ssGood == true;
			//shouldbefalse = ssGood != false;
			//CHECK(ssGood == true);
			//CHECK(ssBad == false);

			REQUIRE(shouldbefalse == false);
		}
	}

	SECTION("Extraction operator>> - Extract formatted data from an input stream")
	{
		rde::stringstream ss;

		SECTION("integer")
		{
			int16_t  i16Val(0);
			uint16_t u16Val(0);
			int32_t  i32Val(0);
			uint32_t u32Val(0);
			int64_t  i64Val(0);
			//uint64_t u64Val(0);

			SECTION("short")
			{
				ss.reset("0");
				ss >> i16Val;
				CHECK(0 == i16Val);

				ss.reset("-32768");
				ss >> i16Val;
				CHECK(INT16_MIN == i16Val);

				ss.reset("32767");
				ss >> i16Val;
				CHECK(INT16_MAX == i16Val);
			}

			SECTION("unsigned short")
			{
				ss.reset("0");
				ss >> u16Val;
				CHECK(0 == u16Val);

				ss.reset("65535");
				ss >> u16Val;
				CHECK(UINT16_MAX == u16Val);
			}

			SECTION("mixed shorts")
			{
				ss.reset("-32768 32767 0 65535");

				ss >> i16Val;
				CHECK(INT16_MIN == i16Val);
				ss >> i16Val;
				CHECK(INT16_MAX == i16Val);
				ss >> u16Val;
				CHECK(0 == u16Val);
				ss >> u16Val;
				CHECK(UINT16_MAX == u16Val);
			}

			SECTION("int")
			{
				ss.reset("0");
				ss >> i32Val;
				CHECK(0 == i32Val);

				ss.reset("-2147483648");
				ss >> i32Val;
				CHECK(INT32_MIN == i32Val);

				ss.reset("2147483647");
				ss >> i32Val;
				CHECK(INT32_MAX == i32Val);
			}

			SECTION("unsigned int")
			{
				ss.reset("0");
				ss >> u32Val;
				CHECK(0 == u32Val);

				ss.reset("4294967295");
				ss >> u32Val;
				CHECK(UINT32_MAX == u32Val);
			}

			SECTION("mixed ints")
			{
				ss.reset("-2147483648 2147483647 0 4294967295");

				ss >> i32Val;
				CHECK(INT32_MIN == i32Val);
				ss >> i32Val;
				CHECK(INT32_MAX == i32Val);
				ss >> u32Val;
				CHECK(0 == u32Val);
				ss >> u32Val;
				CHECK(UINT32_MAX == u32Val);
			}

			SECTION("long long")
			{
				ss.reset("0");
				ss >> i64Val;
				CHECK(0 == i64Val);

				ss.reset("-9223372036854775808");
				ss >> i64Val;
				CHECK(INT64_MIN == i64Val);

				ss.reset("9223372036854775807");
				ss >> i64Val;
				CHECK(INT64_MAX == i64Val);
			}

			// TODO: unsigned long long (_ULonglong)
			//SECTION("unsigned long long")
			//{
			//	REQUIRE(0 == u64Val);
			//
			//	ss.reset("0");
			//	ss >> u64Val;
			//	CHECK(0 == u64Val);
			//
			//	ss.reset("18446744073709551615");
			//	ss >> u64Val;
			//	CHECK(UINT64_MAX == u64Val);
			//}

			SECTION("mixed long long")
			{
				ss.reset("-9223372036854775808 9223372036854775807");
				//ss.reset("-9223372036854775808 9223372036854775807 0 18446744073709551615");

				ss >> i64Val;
				CHECK(INT64_MIN == i64Val);
				ss >> i64Val;
				CHECK(INT64_MAX == i64Val);
				//ss >> u64Val;
				//CHECK(0 == u64Val);
				//ss >> u64Val;
				//CHECK(UINT64_MAX == u64Val);
			}
		}

		SECTION("floating point")
		{
			float       fltVal(0.0f);
			double      dblVal(0.0);
			long double ldblVal(0.0);

			SECTION("float")
			{
				ss.reset("1.192092896e-07f");
				ss >> fltVal;
				CHECK(FLT_EPSILON == fltVal);

				ss.reset("-3.402823466e+38f");
				ss >> fltVal;
				CHECK(-FLT_MAX == fltVal);

				ss.reset("1.175494351e-38f");
				ss >> fltVal;
				CHECK(FLT_MIN == fltVal);

				ss.reset("3.402823466e+38f");
				ss >> fltVal;
				CHECK(FLT_MAX == fltVal);
			}

			SECTION("double")
			{
				ss.reset("2.2204460492503131e-016");
				ss >> dblVal;
				CHECK(DBL_EPSILON == dblVal);

				ss.reset("-1.7976931348623158e+308");
				ss >> dblVal;
				CHECK(-DBL_MAX == dblVal);

				ss.reset("2.2250738585072014e-308");
				ss >> dblVal;
				CHECK(DBL_MIN == dblVal);

				ss.reset("1.7976931348623158e+308");
				ss >> dblVal;
				CHECK(DBL_MAX == dblVal);
			}

			SECTION("long double")
			{
				ss.reset("2.2204460492503131e-016");
				ss >> ldblVal;
				CHECK(LDBL_EPSILON == ldblVal);

				ss.reset("-1.7976931348623158e+308");
				ss >> ldblVal;
				CHECK(-LDBL_MAX == ldblVal);

				ss.reset("2.2250738585072014e-308");
				ss >> ldblVal;
				CHECK(LDBL_MIN == ldblVal);

				ss.reset("1.7976931348623158e+308");
				ss >> ldblVal;
				CHECK(LDBL_MAX == ldblVal);
			}
		}

		SECTION("boolean")
		{
			bool bVal(false);
			REQUIRE(false == bVal);

			ss.reset("1");
			ss >> bVal;
			CHECK(true == bVal);

			ss.reset("0");
			ss >> bVal;
			CHECK(false == bVal);
		}
	}

	SECTION("Original tests")
	{
		SECTION("Extraction operators with empty stringstream")
		{
			rde::stringstream ss("");
			int x(123);
	
			ss >> x;
			REQUIRE(x == 123);
	
			x = 321;
			ss.reset(NULL);
			ss >> x;
			REQUIRE(x == 321);
	
			x = 321;
			ss.reset("456");
			ss >> x;
			REQUIRE(x == 456);
		}
	
		SECTION("Basic")
		{
			rde::stringstream ss("42");
			int x(0);
			ss >> x;
			REQUIRE(x == 42);
	
			ss.reset("4242.4242");
			float y(0);
			ss >> y;
			REQUIRE(y == 4242.4242f);
	
			ss.reset("-1");
			long z(0);
			ss >> z;
			REQUIRE(z == -1);
	
			ss.reset("helloworld");
			rde::string w;
			ss >> w;
			REQUIRE(w.compare(rde::string("helloworld")) == 0);
		}
	
		SECTION("Mixed")
		{
			int x(0);
			float y(0.0f);
			rde::string z;
			rde::stringstream ss(" 1     2.34 hello\r\nworld 4   ");
	
			ss >> x;
			REQUIRE(x == 1);
	
			ss >> y;
			REQUIRE(y == 2.34f);
			ss >> z;
			REQUIRE(z.compare(rde::string("hello")) == 0);
	
			ss >> z;
			REQUIRE(z.compare(rde::string("world")) == 0);
	
			ss >> x;
			REQUIRE(x == 4);
		}
	}
}
} //namespace
