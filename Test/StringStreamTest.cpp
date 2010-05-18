#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/sstream.h"

namespace
{
    //-----------------------------------------------------------------
    //stringstream tests
    TEST(ss_empty)
    {
        rde::stringstream ss("");
        int x(123);
        ss >> x;
        assert(x == 123);
        
        x = 321;
        ss.reset(NULL);
        ss >> x; 
        assert(x == 321);
        
        x = 321;
        ss.reset("456");
        ss >> x;
        assert(x == 456);
    }

    TEST(ss_basic)
    {
        rde::stringstream ss("42");
        int x(0);
        ss >> x;
        assert(x == 42);
        
        ss.reset("4242.4242");
        float y(0);
        ss >> y;
        assert(y == 4242.4242f);
        
        ss.reset("-1");
        long z(0);
        ss >> z;
        assert(z == -1);
        
        ss.reset("helloworld");
        rde::string w;
        ss >> w;
        assert(w.compare(rde::string("helloworld")) == 0);
    }

    TEST(ss_mixed)
    {
        int x(0);
        float y(0.0f);
        rde::string z;
        rde::stringstream ss(" 1     2.34 hello\r\nworld 4   ");
        
        ss >> x;
        assert(x == 1);   
        
        ss >> y;
        assert(y == 2.34f);
        ss >> z;
        assert(z.compare(rde::string("hello")) == 0);
        
        ss >> z;
        assert(z.compare(rde::string("world")) == 0);
        
        ss >> x;
        assert(x == 4);
    }

};