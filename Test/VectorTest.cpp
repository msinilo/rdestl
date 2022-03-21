#include "vendor/Catch/catch.hpp"
#include "rdestl/vector.h"
#include "rdestl/pair.h"
#include <cstdio>

struct MyStruct
{
	int i;
	char c;
	float f;
};
namespace rde
{
	template<> struct is_pod<MyStruct>
	{
		enum { value = true };
	};
}

namespace
{
	typedef rde::vector<int>			tTestVector;
	typedef rde::vector<std::string>	tStringVector;

	const int array [] = { 1, 4, 9, 16, 25, 36 };
	void PrintVector(const tTestVector& v)
	{
		for (tTestVector::const_iterator it = v.begin(); it != v.end(); ++it)
			printf("%d, ", *it);
		printf("\n");
	}
	void PrintVector(const tStringVector& v)
	{
		for (tStringVector::const_iterator it = v.begin(); it != v.end(); ++it)
			printf("%s, ", it->c_str());
		printf("\n");
	}

TEST_CASE("vector", "[vector]")
{
	SECTION("DefaultCtorEmpty")
	{
		tTestVector v;
		CHECK(v.empty());
		CHECK(0 == v.size());
		CHECK(v.begin() == v.end());
		printf("Sizeof(v) = %zd\n", sizeof(v));
	}
	SECTION("PushBack")
	{
		tTestVector v;
		v.push_back(2);
		CHECK(1 == v.size());
		CHECK(2 == v[0]);
		CHECK(!v.empty());
		CHECK(v.begin() != v.end());
	}
	SECTION("PopBack")
	{
		tTestVector v;
		v.push_back(2);
		v.push_back(3);
		CHECK(2 == v.front());
		CHECK(3 == v.back());
		v.pop_back();
		CHECK(!v.empty());
		CHECK(1 == v.size());
		CHECK(2 == v[0]);
		tTestVector::const_iterator it = v.begin();
		++it;
		CHECK(v.end() == it);
	}
	SECTION("AssignTab")
	{
		tTestVector v;
		CHECK(v.empty());
		v.assign(array, array + 6);
		CHECK(6 == v.size());
		CHECK(0 == memcmp(array, v.data(), sizeof(array)));
	}
	SECTION("AssignTabConstruct")
	{
		tTestVector v(array, array + 6);
		CHECK(6 == v.size());
		CHECK(0 == memcmp(array, v.data(), sizeof(array)));
	}

	SECTION("Clear")
	{
		tTestVector v(array, array + 6);
		CHECK(6 == v.size());
		v.clear();
		CHECK(0 == v.size());
		CHECK(v.empty());
		// Make sure it doesnt free mem.
		CHECK(v.capacity() > 0);
	}

	SECTION("EraseBegin")
	{
		tTestVector v(array, array + 6);
		v.erase(v.begin());
		// 4, 9, 16, 25, 36
		CHECK(4 == v.front());
	}
	SECTION("Erase")
	{
		tTestVector v(array, array + 6);

		// Remove 4 & 9
		tTestVector::iterator it = v.begin();
		it += 1;
		tTestVector::iterator it2 = it;
		it2 += 2;
		v.erase(it, it2);
		CHECK(4 == v.size());
		CHECK(1 == v.front());
		CHECK(16 == v[1]);
		CHECK(25 == v[2]);
		CHECK(36 == v[3]);
	}
	SECTION("EraseSingle")
	{
		tTestVector v(array, array + 6);
		tTestVector::iterator it = v.erase(v.begin() + 1); // 4
		CHECK(9 == *it);
		v.erase(it + 1); // 16

		CHECK(4 == v.size());
		//PrintVector(v);
	}
	SECTION("EraseEnd")
	{
		tTestVector v(array, array + 6);
		tTestVector::iterator it = v.begin();
		it += 2;
		v.erase(it, v.end());
		CHECK(2 == v.size());
	}
	SECTION("EraseAll")
	{
		tTestVector v(array, array + 6);
		v.erase(v.begin(), v.end());
		CHECK(v.empty());

		rde::vector<MyStruct> v2;
		v2.erase(v2.begin(), v2.end());
	}

	SECTION("InsertString")
	{
		tStringVector v;
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		v.insert(0, 1, "Hello");
		CHECK(4 == v.size());
		v.insert(4, 3, ".");
		CHECK(7 == v.size());
		PrintVector(v);
		v.insert(1, 10, "very");
		CHECK(17 == v.size());
		PrintVector(v);
	}

	SECTION("EraseString")
	{
		tStringVector v;
		v.push_back("Hello");
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		CHECK(4 == v.size());
		v.erase(v.begin() + 1, v.end() - 1);
		CHECK(2 == v.size());
	}
	SECTION("IndexOf")
	{
		tStringVector v;
		v.push_back("Hello");
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		CHECK(2 == v.index_of("new"));
		CHECK(tStringVector::npos == v.index_of("blah"));
		CHECK(tStringVector::npos == v.index_of("brave", 2));
	}

	SECTION("InsertFront")
	{
		tTestVector v(array, array + 6);
		v.insert(v.begin(), 2, -1);
		CHECK(8 == v.size());
		CHECK(-1 == v.front());
		CHECK(-1 == v[1]);
		CHECK(1 == v[2]);
		CHECK(4 == v[3]);
		CHECK(36 == v[7]);
	}
	SECTION("InsertMiddle")
	{
		tTestVector v(array, array + 6);
		tTestVector::iterator it = v.begin() + 2;
		v.insert(it, 3, 5);
		CHECK(9 == v.size());
		CHECK(5 == v[2]);
		it = v.begin() + 2;
		v.insert(it, 4);
		CHECK(4 == v[2]);
	}
	SECTION("InsertEnd")
	{
		tTestVector v(array, array + 6);
		v.insert(v.end(), 1, 49);
		CHECK(7 == v.size());
		CHECK(49 == v[6]);
	}

	SECTION("InsertEndRealloc")
	{
		tTestVector v;
		for (tTestVector::size_type i = 0; i < v.capacity(); ++i)
		{
			v.push_back((int)i);
		}
		v.insert(v.end(), 666);
		CHECK(666 == v.back());
	}

	SECTION("InsertToEmpty")
	{
		tTestVector v;
		v.insert(v.begin(), 1, 2);
		CHECK(1 == v.size());
		CHECK(2 == v.front());
	}

	SECTION("ResizeSmaller")
	{
		tTestVector v(array, array + 6);
		v.resize(4);
		CHECK(4 == v.size());
		CHECK(16 == v[3]);
	}

	SECTION("CopyConstructor")
	{
		tTestVector v(array, array + 6);
		tTestVector v2(v);
		CHECK(6 == v2.size());
		CHECK(memcmp(v.begin(), v2.begin(), 6*sizeof(int)) == 0);
	}

	SECTION("AssignmentOp")
	{
		tTestVector v(array, array + 6);
		tTestVector v2;
		v2 = v;
		CHECK(6 == v2.size());
		CHECK(memcmp(v.begin(), v2.begin(), 6*sizeof(int)) == 0);
	}

	SECTION("Tighten")
	{
		tTestVector v(array, array + 6);
		v.push_back(49);
		v.set_capacity(v.size());
		CHECK(7 == v.capacity());
	}
	SECTION("Reserve")
	{
		tTestVector v;
		CHECK(0 == v.capacity());
		v.push_back(1); v.push_back(2); v.push_back(3);
		v.reserve(10);
		CHECK(v.capacity() >= 10);
		CHECK(3 == v.size());
		CHECK(1 == v[0]);
		CHECK(2 == v[1]);
		CHECK(3 == v[2]);
	}

	SECTION("CopyEmpty")
	{
		tTestVector v;
		v.push_back(1); v.push_back(2); v.push_back(3);
		tTestVector v2;
		v = v2;
		CHECK(v.empty());
	}

	// Force few grows
	SECTION("MultipleGrow")
	{
		tTestVector v;
		for (int i = 0; i < 5000; ++i)
		{
			v.push_back(i);
		}
		CHECK(5000 == v.size());
		for (int i = 0; i < 5000; ++i)
		{
			CHECK(i == v[i]);
		}
	}

    //-----------------------------------------------------------------
    //Vector tests by Danushka Abeysuriya silvermace@gmail.com

    SECTION("Resize")
    {
        rde::vector<int> myFoos;
        myFoos.resize(20);

        CHECK(myFoos.size() == 20);
        CHECK(myFoos.capacity() >= 20);
    }

    //Regression test:
    //The bug is caused when the copy constructor is invoked on a vector where
    //the rhs is an empty (and non-preallocated) vector - i.e. there is nothing to copy
    SECTION("Resize_Regression_1")
    {
        rde::vector< rde::vector<int> > myFoosFoos;
        myFoosFoos.push_back(rde::vector<int>());
        myFoosFoos.push_back(rde::vector<int>());
        myFoosFoos[1].push_back(42);

        CHECK(myFoosFoos.size() == 2);
        CHECK(myFoosFoos[0].size() == 0);
        CHECK(myFoosFoos[1].size() == 1);
    }

    SECTION("Resize_Regression_2")
    {
        rde::vector< rde::vector<int> > myFoosFoos;
        rde::vector<int> a;
        rde::vector<int> b;

        a.push_back(42);
        a.push_back(24);

        myFoosFoos.push_back(a);
        myFoosFoos.push_back(b);
        myFoosFoos[1].push_back(4224);

        CHECK(myFoosFoos.size() == 2);

        CHECK(myFoosFoos[0].size() == 2);
        CHECK(myFoosFoos[0][0] == 42);
        CHECK(myFoosFoos[0][1] == 24);

        CHECK(myFoosFoos[1].size() == 1);
        CHECK(myFoosFoos[1][0] == 4224);
    }

    SECTION("Insert")
    {
        rde::vector<int> myFoos;

        myFoos.push_back(42);
        CHECK(myFoos.size() == 1);
        CHECK(myFoos[0] == 42);

        myFoos.insert(myFoos.begin(), 24);
        CHECK(myFoos.size() == 2);
        CHECK(myFoos[0] == 24);
        CHECK(myFoos[1] == 42);

        myFoos.clear();
        CHECK(myFoos.size() == 0);

        myFoos.insert(myFoos.begin(), 42);
        CHECK(myFoos.size() == 1);
        CHECK(myFoos[0] == 42);
    }

    SECTION("VectorOfVectors")
    {
        rde::vector< rde::vector<int> > myFoosFoos;
        myFoosFoos.push_back(rde::vector<int>(10));
        myFoosFoos.push_back(rde::vector<int>(20));
        CHECK(myFoosFoos.size() == 2);
        CHECK(myFoosFoos[0].size() == 10);
        CHECK(myFoosFoos[1].size() == 20);
    }

    SECTION("MoveConstructorExplicit")
    {
        rde::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        CHECK(2 == v.size());

        rde::vector<int> v2(std::move(v));
        CHECK(0 == v.size());
        CHECK(2 == v2.size());
        CHECK(1 == v2[0]);
        CHECK(2 == v2[1]);
    }
    SECTION("MoveAssignment")
    {
        rde::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        CHECK(2 == v.size());

        rde::vector<int> v2;
        v2 = std::move(v);
        CHECK(0 == v.size());
        CHECK(2 == v2.size());
        CHECK(1 == v2[0]);
        CHECK(2 == v2[1]);
    }
    SECTION("EmplaceBack")
    {
        rde::vector<rde::pair<int, int> > v;

        v.emplace_back(1, 2);
        v.emplace_back(3, 4);
        v.emplace_back(5, 6);

        CHECK(3 == v.size());
        CHECK(v[0].first == 1); CHECK(v[0].second == 2);
        CHECK(v[1].first == 3); CHECK(v[1].second == 4);
        CHECK(v[2].first == 5); CHECK(v[2].second == 6);
    }

    //typedef rde::fixed_vector<int, 3, false> fvector;

    //SECTION("fixedvec_copy")
    //{
    //    fvector a;
    //    fvector b;
    //    a.push_back(42);
    //    a.push_back(24);
    //    a.push_back(4224);
    //
    //    b = a;
    //
    //    CHECK(b.size() == 3);
    //    CHECK(b[0] == 42);
    //    CHECK(b[1] == 24);
    //    CHECK(b[2] == 4224);
    //
    //    b = fvector();
    //    CHECK(b.size() == 3);
    //}
}
}
