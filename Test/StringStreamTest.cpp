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
