#include "vendor/Catch/catch.hpp"
#include "sstream.h"

namespace
{
TEST_CASE("sstream", "[string]")
{
    SECTION("Empty")
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
