#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/vector.h"
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

	TEST(DefaultCtorEmpty)
	{
		tTestVector v;
		CHECK(v.empty());
		CHECK_EQUAL(0, v.size());
		CHECK_EQUAL(v.begin(), v.end());
		printf("Sizeof(v) = %d\n", sizeof(v));
	}
	TEST(PushBack)
	{
		tTestVector v;
		v.push_back(2);
		CHECK_EQUAL(1, v.size());
		CHECK_EQUAL(2, v[0]);
		CHECK(!v.empty());
		CHECK(v.begin() != v.end());
	}
	TEST(PopBack)
	{
		tTestVector v;
		v.push_back(2);
		v.push_back(3);
		CHECK_EQUAL(2, v.front());
		CHECK_EQUAL(3, v.back());
		v.pop_back();
		CHECK(!v.empty());
		CHECK_EQUAL(1, v.size());
		CHECK_EQUAL(2, v[0]);
		tTestVector::const_iterator it = v.begin();
		++it;
		CHECK_EQUAL(v.end(), it);
	}
	TEST(AssignTab)
	{
		tTestVector v;
		CHECK(v.empty());
		v.assign(array, array + 6);
		CHECK_EQUAL(6, v.size());
		CHECK_EQUAL(0, memcmp(array, v.data(), sizeof(array)));
	}
	TEST(AssignTabConstruct)
	{
		tTestVector v(array, array + 6);
		CHECK_EQUAL(6, v.size());
		CHECK_EQUAL(0, memcmp(array, v.data(), sizeof(array)));
	}

	TEST(Clear)
	{
		tTestVector v(array, array + 6);
		CHECK_EQUAL(6, v.size());
		v.clear();
		CHECK_EQUAL(0, v.size());
		CHECK(v.empty());
		// Make sure it doesnt free mem.
		CHECK(v.capacity() > 0);
	}

	TEST(EraseBegin)
	{
		tTestVector v(array, array + 6);
		v.erase(v.begin()); 
		// 4, 9, 16, 25, 36
		CHECK_EQUAL(4, v.front());
	}
	TEST(Erase)
	{
		tTestVector v(array, array + 6);

		// Remove 4 & 9
		tTestVector::iterator it = v.begin();
		it += 1;
		tTestVector::iterator it2 = it;
		it2 += 2;
		v.erase(it, it2);
		CHECK_EQUAL(4, v.size());
		CHECK_EQUAL(1, v.front());
		CHECK_EQUAL(16, v[1]);
		CHECK_EQUAL(25, v[2]);
		CHECK_EQUAL(36, v[3]);
	}
	TEST(EraseSingle)
	{
		tTestVector v(array, array + 6);
		tTestVector::iterator it = v.erase(v.begin() + 1); // 4
		CHECK_EQUAL(9, *it);
		v.erase(it + 1); // 16
		
		CHECK_EQUAL(4, v.size());
		//PrintVector(v);
	}
	TEST(EraseEnd)
	{
		tTestVector v(array, array + 6);
		tTestVector::iterator it = v.begin();
		it += 2;
		v.erase(it, v.end());
		CHECK_EQUAL(2, v.size());
	}
	TEST(EraseAll)
	{
		tTestVector v(array, array + 6);
		v.erase(v.begin(), v.end());
		CHECK(v.empty());

		rde::vector<MyStruct> v2;
		v2.erase(v2.begin(), v2.end());
	}

	TEST(InsertString)
	{
		tStringVector v;
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		v.insert(0, 1, "Hello");
		CHECK_EQUAL(4, v.size());
		v.insert(4, 3, ".");
		CHECK_EQUAL(7, v.size());
		PrintVector(v);
		v.insert(1, 10, "very");
		CHECK_EQUAL(17, v.size());
		PrintVector(v);
	}

	TEST(EraseString)
	{
		tStringVector v;
		v.push_back("Hello");
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		CHECK_EQUAL(4, v.size());
		v.erase(v.begin() + 1, v.end() - 1);
		CHECK_EQUAL(2, v.size());
	}
	TEST(IndexOf)
	{
		tStringVector v;
		v.push_back("Hello");
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		CHECK_EQUAL(2, v.index_of("new"));
		CHECK_EQUAL(tStringVector::npos, v.index_of("blah"));
		CHECK_EQUAL(tStringVector::npos, v.index_of("brave", 2));
	}

	TEST(InsertFront)
	{
		tTestVector v(array, array + 6);
		v.insert(v.begin(), 2, -1);
		CHECK_EQUAL(8, v.size());
		CHECK_EQUAL(-1, v.front());
		CHECK_EQUAL(-1, v[1]);
		CHECK_EQUAL(1, v[2]);
		CHECK_EQUAL(4, v[3]);
		CHECK_EQUAL(36, v[7]);
	}
	TEST(InsertMiddle)
	{
		tTestVector v(array, array + 6);
		tTestVector::iterator it = v.begin() + 2;
		v.insert(it, 3, 5);
		CHECK_EQUAL(9, v.size());
		CHECK_EQUAL(5, v[2]);
		it = v.begin() + 2;
		v.insert(it, 4);
		CHECK_EQUAL(4, v[2]);
	}
	TEST(InsertEnd)
	{
		tTestVector v(array, array + 6);
		v.insert(v.end(), 1, 49);
		CHECK_EQUAL(7, v.size());
		CHECK_EQUAL(49, v[6]);
	}

	TEST(InsertEndRealloc)
	{
		tTestVector v;
		for (tTestVector::size_type i = 0; i < v.capacity(); ++i)
		{
			v.push_back((int)i);
		}
		v.insert(v.end(), 666);
		CHECK_EQUAL(666, v.back());
	}

	TEST(InsertToEmpty)
	{
		tTestVector v;
		v.insert(v.begin(), 1, 2);
		CHECK_EQUAL(1, v.size());
		CHECK_EQUAL(2, v.front());
	}

	TEST(ResizeSmaller)
	{
		tTestVector v(array, array + 6);
		v.resize(4);
		CHECK_EQUAL(4, v.size());
		CHECK_EQUAL(16, v[3]);
	}

	TEST(CopyConstructor)
	{
		tTestVector v(array, array + 6);
		tTestVector v2(v);
		CHECK_EQUAL(6, v2.size());
		CHECK(memcmp(v.begin(), v2.begin(), 6*sizeof(int)) == 0);
	}

	TEST(AssignmentOp)
	{
		tTestVector v(array, array + 6);
		tTestVector v2;
		v2 = v;
		CHECK_EQUAL(6, v2.size());
		CHECK(memcmp(v.begin(), v2.begin(), 6*sizeof(int)) == 0);
	}

	TEST(Tighten)
	{
		tTestVector v(array, array + 6);
		v.push_back(49);
		v.set_capacity(v.size());
		CHECK_EQUAL(7, v.capacity());
	}
	TEST(Reserve)
	{
		tTestVector v;
		CHECK_EQUAL(0, v.capacity());
		v.push_back(1); v.push_back(2); v.push_back(3);
		v.reserve(10);
		CHECK(v.capacity() >= 10);
		CHECK_EQUAL(3, v.size());
		CHECK_EQUAL(1, v[0]);
		CHECK_EQUAL(2, v[1]);
		CHECK_EQUAL(3, v[2]);
	}

	TEST(CopyEmpty)
	{
		tTestVector v;
		v.push_back(1); v.push_back(2); v.push_back(3);
		tTestVector v2;
		v = v2;
		CHECK(v.empty());
	}

	// Force few grows
	TEST(MultipleGrow)
	{
		tTestVector v;
		for (int i = 0; i < 5000; ++i)
		{
			v.push_back(i);
		}
		CHECK_EQUAL(5000, v.size());
		for (int i = 0; i < 5000; ++i)
		{
			CHECK_EQUAL(i, v[i]);
		}
	}

    //-----------------------------------------------------------------
    //Vector tests by Danushka Abeysuriya silvermace@gmail.com

    TEST(Resize)
    {
        rde::vector<int> myFoos;
        myFoos.resize(20);
        
        CHECK(myFoos.size() == 20);
        CHECK(myFoos.capacity() >= 20);
    }

    //Regression test:
    //The bug is caused when the copy constructor is invoked on a vector where 
    //the rhs is an empty (and non-preallocated) vector - i.e. there is nothing to copy
    TEST(Resize_Regression_1)
    {
        rde::vector< rde::vector<int> > myFoosFoos;
        myFoosFoos.push_back(rde::vector<int>());
        myFoosFoos.push_back(rde::vector<int>());
        myFoosFoos[1].push_back(42);
        
        CHECK(myFoosFoos.size() == 2);
        CHECK(myFoosFoos[0].size() == 0);
        CHECK(myFoosFoos[1].size() == 1);
    }

    TEST(Resize_Regression_2)
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

    TEST(Insert)
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

    TEST(VectorOfVectors)
    {
        rde::vector< rde::vector<int> > myFoosFoos;
        myFoosFoos.push_back(rde::vector<int>(10));
        myFoosFoos.push_back(rde::vector<int>(20));
        CHECK(myFoosFoos.size() == 2);
        CHECK(myFoosFoos[0].size() == 10);
        CHECK(myFoosFoos[1].size() == 20);
    }

    //typedef rde::fixed_vector<int, 3, false> fvector;

    //TEST(fixedvec_copy)
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
