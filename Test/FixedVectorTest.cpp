#include "vendor/Catch/catch.hpp"
#include "rdestl/fixed_vector.h"
#include "rdestl/pair.h"
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

TEST_CASE("fixed_vector", "[vector]")
{
	SECTION("NonStandardAlign")
	{
		rde::fixed_vector<__m128, 1, false> v;
		rde::aligned_as<__m128>::res r;
		CHECK(16ul == sizeof(r));
		CHECK(16ul == sizeof(__m128));
		CHECK(16 == rde::rde_alignof<__m128>::res);
		CHECK(v.empty());
		CHECK(0 == v.size());
		CHECK(v.begin() == v.end());
	}

	SECTION("DefaultCtorEmpty")
	{
		tTestVector v;
		CHECK(v.empty());
		CHECK(0 == v.size());
		CHECK(v.begin() == v.end());
		printf("Sizeof(v) = %d\n", sizeof(v));
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
#if RDESTL_RECORD_WATERMARKS
		CHECK(6 == v.get_high_watermark());
#endif
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

		//rde::vector<MyStruct> v2;
		//v2.erase(v2.begin(), v2.end());
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

	SECTION("Reserve")
	{
		tTestVector v;
		CHECK(64 == v.capacity());
		v.push_back(1); v.push_back(2); v.push_back(3);
		v.reserve(120);
		CHECK(v.capacity() >= 120);
		CHECK(3 == v.size());
		CHECK(1 == v[0]);
		CHECK(2 == v[1]);
		CHECK(3 == v[2]);
	}

    SECTION("EmplaceBack")
    {
        rde::fixed_vector<rde::pair<int, int>, 8, false> v;

        v.emplace_back(1, 2);
        v.emplace_back(3, 4);
        v.emplace_back(5, 6);

        CHECK(3 == v.size());
        CHECK(v[0].first == 1); CHECK(v[0].second == 2);
        CHECK(v[1].first == 3); CHECK(v[1].second == 4);
        CHECK(v[2].first == 5); CHECK(v[2].second == 6);
    }
#if RDESTL_RECORD_WATERMARKS
	SECTION("Watermarks")
	{
		tTestVector v;
		CHECK(0 == v.get_high_watermark());
		v.push_back(1);
		CHECK(1 == v.get_high_watermark());
		v.push_back(2); v.push_back(3); v.push_back(4);
		CHECK(4 == v.get_high_watermark());
		v.pop_back();
		CHECK(4 == v.get_high_watermark());
		v.clear();
		CHECK(4 == v.get_high_watermark());
	}
#endif
#if !RDESTL_STANDALONE && RDE_DEBUG
	int numFailedAssertions(0);
	bool AssertionHandler(const char*, const char*, int)
	{
		++numFailedAssertions;
		return true;
	}
	SECTION("AssertOnOverflow")
	{
		rde::fixed_vector<int, 10, false> v;
		rde::Assertion::Handler prevHandler = rde::Assertion::SetHandler(AssertionHandler);
		for (int i = 0; i < 11; ++i)
			v.push_back(i);
		CHECK(1 == numFailedAssertions);
		rde::Assertion::SetHandler(prevHandler);
	}
#endif
}
}
