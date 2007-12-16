#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/vector.h"
#include <cstdio>

	struct MyStruct
	{
		int i;
		char c;
		float f;
	};
namespace rdestl
{
	template<> struct is_pod<MyStruct>
	{
		enum { value = true };
	};
}

namespace
{
	typedef rdestl::vector<int>			tTestVector;
	typedef rdestl::vector<std::string>	tStringVector;

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
		CHECK_EQUAL(0ul, v.size());
		CHECK_EQUAL(v.begin(), v.end());
		printf("Sizeof(v) = %d\n", sizeof(v));
	}
	TEST(PushBack)
	{
		tTestVector v;
		v.push_back(2);
		CHECK_EQUAL(1ul, v.size());
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
		CHECK_EQUAL(1ul, v.size());
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
		CHECK_EQUAL(6ul, v.size());
		CHECK_EQUAL(0, memcmp(array, v.data(), sizeof(array)));
	}
	TEST(AssignTabConstruct)
	{
		tTestVector v(array, array + 6);
		CHECK_EQUAL(6ul, v.size());
		CHECK_EQUAL(0, memcmp(array, v.data(), sizeof(array)));
	}

	TEST(Clear)
	{
		tTestVector v(array, array + 6);
		CHECK_EQUAL(6ul, v.size());
		v.clear();
		CHECK_EQUAL(0ul, v.size());
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
		CHECK_EQUAL(4ul, v.size());
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
		
		CHECK_EQUAL(4ul, v.size());
		//PrintVector(v);
	}
	TEST(EraseEnd)
	{
		tTestVector v(array, array + 6);
		tTestVector::iterator it = v.begin();
		it += 2;
		v.erase(it, v.end());
		CHECK_EQUAL(2ul, v.size());
	}
	TEST(EraseAll)
	{
		tTestVector v(array, array + 6);
		v.erase(v.begin(), v.end());
		CHECK(v.empty());

		rdestl::vector<MyStruct> v2;
		v2.erase(v2.begin(), v2.end());
	}

	TEST(InsertString)
	{
		tStringVector v;
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		v.insert(size_t(0), 1, "Hello");
		CHECK_EQUAL(4ul, v.size());
		v.insert(4, 3, ".");
		CHECK_EQUAL(7ul, v.size());
		PrintVector(v);
		v.insert(1, 10, "very");
		CHECK_EQUAL(17ul, v.size());
		PrintVector(v);
	}

	TEST(EraseString)
	{
		tStringVector v;
		v.push_back("Hello");
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		CHECK_EQUAL(4ul, v.size());
		v.erase(v.begin() + 1, v.end() - 1);
		CHECK_EQUAL(2ul, v.size());
	}
	TEST(IndexOf)
	{
		tStringVector v;
		v.push_back("Hello");
		v.push_back("brave");
		v.push_back("new");
		v.push_back("world");
		CHECK_EQUAL(2ul, v.index_of("new"));
		CHECK_EQUAL(tStringVector::npos, v.index_of("blah"));
		CHECK_EQUAL(tStringVector::npos, v.index_of("brave", 2));
	}

	TEST(InsertFront)
	{
		tTestVector v(array, array + 6);
		v.insert(v.begin(), 2, -1);
		CHECK_EQUAL(8ul, v.size());
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
		CHECK_EQUAL(9ul, v.size());
		CHECK_EQUAL(5, v[2]);
		it = v.begin() + 2;
		v.insert(it, 4);
		CHECK_EQUAL(4, v[2]);
	}
	TEST(InsertEnd)
	{
		tTestVector v(array, array + 6);
		v.insert(v.end(), 1, 49);
		CHECK_EQUAL(7ul, v.size());
		CHECK_EQUAL(49, v[6]);
	}
	TEST(InsertToEmpty)
	{
		tTestVector v;
		v.insert(v.begin(), 1, 2);
		CHECK_EQUAL(1ul, v.size());
		CHECK_EQUAL(2, v.front());
	}

	TEST(ResizeSmaller)
	{
		tTestVector v(array, array + 6);
		v.resize(4);
		CHECK_EQUAL(4ul, v.size());
		CHECK_EQUAL(16, v[3]);
	}

	TEST(CopyConstructor)
	{
		tTestVector v(array, array + 6);
		tTestVector v2(v);
		CHECK_EQUAL(6ul, v2.size());
		CHECK(memcmp(v.begin(), v2.begin(), 6*sizeof(int)) == 0);
	}

	TEST(AssignmentOp)
	{
		tTestVector v(array, array + 6);
		tTestVector v2;
		v2 = v;
		CHECK_EQUAL(6ul, v2.size());
		CHECK(memcmp(v.begin(), v2.begin(), 6*sizeof(int)) == 0);
	}

	TEST(Tighten)
	{
		tTestVector v(array, array + 6);
		v.push_back(49);
		v.set_capacity(v.size());
		CHECK_EQUAL(7ul, v.capacity());
	}
	TEST(Reserve)
	{
		tTestVector v;
		CHECK_EQUAL(0ul, v.capacity());
		v.push_back(1); v.push_back(2); v.push_back(3);
		v.reserve(10);
		CHECK(v.capacity() >= 10);
		CHECK_EQUAL(3ul, v.size());
		CHECK_EQUAL(1, v[0]);
		CHECK_EQUAL(2, v[1]);
		CHECK_EQUAL(3, v[2]);
	}
}
