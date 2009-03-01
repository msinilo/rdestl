#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/fixed_vector.h"
#include <xmmintrin.h>


namespace
{
	typedef rde::fixed_vector<int, 64, true>			tTestVector;
	typedef rde::fixed_vector<std::string, 64, true>	tStringVector;

	const int array [] = { 1, 4, 9, 16, 25, 36 }; 

	void PrintVector(const tStringVector& v)
	{
		for (tStringVector::const_iterator it = v.begin(); it != v.end(); ++it)
			printf("%s, ", it->c_str());
		printf("\n");
	}

	TEST(NonStandardAlign)
	{
		rde::fixed_vector<__m128, 1, false> v;
		rde::aligned_as<__m128>::res r;
		CHECK_EQUAL(16ul, sizeof(r));
		CHECK_EQUAL(16ul, sizeof(__m128));
		CHECK_EQUAL(16, rde::alignof<__m128>::res);
		CHECK(v.empty());
		CHECK_EQUAL(0, v.size());
		CHECK_EQUAL(v.begin(), v.end());
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
#if RDESTL_RECORD_WATERMARKS
		CHECK_EQUAL(6, v.get_high_watermark());
#endif
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

		//rde::vector<MyStruct> v2;
		//v2.erase(v2.begin(), v2.end());
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

	TEST(Reserve)
	{
		tTestVector v;
		CHECK_EQUAL(64, v.capacity());
		v.push_back(1); v.push_back(2); v.push_back(3);
		v.reserve(120);
		CHECK(v.capacity() >= 120);
		CHECK_EQUAL(3, v.size());
		CHECK_EQUAL(1, v[0]);
		CHECK_EQUAL(2, v[1]);
		CHECK_EQUAL(3, v[2]);
	}
#if RDESTL_RECORD_WATERMARKS
	TEST(Watermarks)
	{
		tTestVector v;
		CHECK_EQUAL(0, v.get_high_watermark());
		v.push_back(1);
		CHECK_EQUAL(1, v.get_high_watermark());
		v.push_back(2); v.push_back(3); v.push_back(4);
		CHECK_EQUAL(4, v.get_high_watermark());
		v.pop_back();
		CHECK_EQUAL(4, v.get_high_watermark());
		v.clear();
		CHECK_EQUAL(4, v.get_high_watermark());
	}
#endif
#if !RDESTL_STANDALONE && RDE_DEBUG
	int numFailedAssertions(0);
	bool AssertionHandler(const char*, const char*, int)
	{
		++numFailedAssertions;
		return true;
	}
	TEST(AssertOnOverflow)
	{
		rde::fixed_vector<int, 10, false> v;
		rde::Assertion::Handler prevHandler = rde::Assertion::SetHandler(AssertionHandler);
		for (int i = 0; i < 11; ++i)
			v.push_back(i);
		CHECK_EQUAL(1, numFailedAssertions);
		rde::Assertion::SetHandler(prevHandler);
	}
#endif
}

