#include "hash_map.h"

#if !RDESTL_STANDALONE

#include "khash.h"

//#include "map.h"
//#include "ternary_search_tree.h"
#include "vector.h"
#include <hash_map>
#include <map>
#include <string>

#include "core/Console.h"
#include "core/Random.h"
#include "core/StackTrace.h"
#include "core/Timer.h"
#include "core/win32/Windows.h"

#include "google/dense_hash_map"

namespace
{
	struct hasher
	{
		rde::hash_value_t operator()(const std::string& s) const
		{
			size_t len = s.length();
			rde::hash_value_t hash(0);
			for (size_t i = 0; i < len; ++i)
			{
				hash *= 31;
				hash += s[i];
			}
			return hash;
		}
	};

	typedef rde::hash_map<std::string, int, hasher>	tMap;
	typedef rde::hash_map<std::string, int, hasher, 4>	tMapSmall;

	struct mycomp
	{ // define hash function for strings
		enum
		{ // parameters for hash table
			bucket_size = 4, // 0 < bucket_size
			min_buckets = 8
		}; // min_buckets = 2 ^^ N, 0 < N
		size_t operator()(const std::string &s1) const
		{
			return h(s1);
		}
		bool operator()(const std::string& s1, const std::string& s2)
		{ // test if s1 ordered before s2
			return s1 < s2;
		}
		hasher h;
	}; 
	// STL hashmap
	typedef stdext::hash_map<std::string, int, mycomp>	tMap4;
	// STL map
	typedef std::map<std::string, int>					tMap5;
	// RDE map
	//typedef rde::map<std::string, int>					tMap6;
	// TST
	//typedef rde::ternary_search_tree<int>				tMap7;

	template<typename T>
	struct HasUsedMemoryMethod
	{
		template<typename U, size_t (U::*)() const> struct SFINAE {};
		template<typename U> static char Test(SFINAE<U, &U::used_memory>*);
		template<typename U> static int Test(...);
		static const bool Has = sizeof(Test<T>(0)) == sizeof(char);
	};

	template<typename TMap>
	void ReportMemUsage(const TMap& m, rde::int_to_type<true>)
	{
		rde::Console::Printf("Used %d bytes (%dKB)\n", m.used_memory(), m.used_memory()>>10);
	}
	template<typename TMap>
	void ReportMemUsage(const TMap&, rde::int_to_type<false>)
	{
	}
	template<typename TMap>
	void ReportMemUsage(const TMap& m)
	{
		ReportMemUsage(m, 
			rde::int_to_type<HasUsedMemoryMethod<TMap>::Has>());
	}

	struct ProfileResult
	{
		explicit ProfileResult(const char* name_)
		: enumTime(0), findTime(0), removeTime(0), name(name_) {}
		long		enumTime;
		long		findTime;
		long		removeTime;
		std::string	name;
	};

	rde::Timer timer;

	template<typename TPair>
	TPair MakePair(const char* k, int i)
	{
		return TPair(k, i);
	}

	template<typename TMap, class TPair>
	int MapSpeedTest(ProfileResult& res)
	{
		TMap fileMap;
		rde::vector<std::string> allFiles;

		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile("c:\\Windows\\System32\\*.*", &ffd);
		if (hFind)
		{
			timer.Start();
			int i(0);
			do
			{
				if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					TPair p = MakePair<TPair>(ffd.cFileName, i);
					fileMap.insert(p);
					allFiles.push_back(std::string(ffd.cFileName));
					++i;
				}
			} while (FindNextFile(hFind, &ffd) != 0);
			timer.Stop();
			res.enumTime += timer.GetTimeInMs();
			FindClose(hFind);
		}
		ReportMemUsage(fileMap);
		const int numLookUps = (int)allFiles.size() * 10;
		int notFound(0);
		timer.Start();
		for (int i = 0; i < numLookUps; ++i)
		{
			const int j = rand() % allFiles.size();
			const std::string& str = allFiles[j];
			if (fileMap.find(str) == fileMap.end())
				++notFound;
		}
		timer.Stop();
		RDE_ASSERT(notFound==0);
		res.findTime += timer.GetTimeInMs();

		timer.Start();
		for (int i = 0; i < numLookUps; ++i)
		{
			const int j = rand() % allFiles.size();
			const std::string& str = allFiles[j];
			fileMap.erase(str);
		}
		timer.Stop();
		res.removeTime += timer.GetTimeInMs();

		return notFound;
	}

	void MapSpeedTest()
	{
		const int kNumTests = 10;
		ProfileResult results[] = 
		{
			ProfileResult("RDE closed hashmap, pow2 num of buckets"),
			ProfileResult("RDE, no caching"),
			ProfileResult("STD hashmap"),
			ProfileResult("STD map"),
			ProfileResult("RDE map"),
			ProfileResult("TST")
		};
		for (int i = 0; i < kNumTests; ++i)
		{
			MapSpeedTest<tMap, tMap::value_type>(results[0]);
			MapSpeedTest<tMapSmall, tMapSmall::value_type>(results[1]);
			MapSpeedTest<tMap4, tMap4::value_type>(results[2]);
			MapSpeedTest<tMap5, tMap5::value_type>(results[3]);
			//MapSpeedTest<tMap6, tMap6::value_type>(results[3]);
			//MapSpeedTest<tMap6, tMap6::pair_type>(results[5]);
		}
		for (int i = 0; i < RDE_ARRAY_COUNT(results); ++i)
		{
			rde::Console::Printf("%d: %s: enumeration %dms, find %dms, remove %dms, total: %dms\n", i, 
				results[i].name.c_str(),
				results[i].enumTime, results[i].findTime, results[i].removeTime,
				(results[i].enumTime + results[i].findTime + results[i].removeTime));
		}
	} 

struct eqstr {
	inline bool operator()(const char *s1, const char *s2) const {
		return strcmp(s1, s2) == 0;
    }
};

	struct hasher2
	{
		// DJB2
		rde::hash_value_t operator()(const char* s) const
		{
			rde::hash_value_t hash(0);
			char c;
			while ((c = *s++) != 0)
			{
				hash = (hash << 5) + hash + c;
			}
			return hash;
		}
	};


	typedef rde::hash_map<unsigned, int> inthash;
	typedef rde::hash_map<const char*, int, hasher2, 4, eqstr> strhash; 
	typedef rde::hash_map<const char*, int, hasher2, 6, eqstr> strhash2; 

	int test_int(int N, const unsigned *data)
	{
		int i, ret;
		inthash *h = new inthash;
		for (i = 0; i < N; ++i) {
			
			rde::pair<inthash::iterator, bool> p = h->insert(rde::pair<unsigned, int>(data[i], i));
			if (p.second == false) h->erase(p.first);
		}
		ret = h->size();
		delete h;

		return ret;
	}

	int test_str(int N, char * const *data)
	{
		int i, ret;
		strhash *h = new strhash;
		for (i = 0; i < N; ++i) {
			rde::pair<strhash::iterator, bool> p = h->insert(rde::pair<const char*, int>(data[i], i));
			if (p.second == false) h->erase(p.first);
		}
		ret = h->size();
		ReportMemUsage(*h);
		delete h;
		return ret; 
	} 
	int test_str3(int N, char * const *data)
	{
		int i, ret;
		strhash2 *h = new strhash2;
		for (i = 0; i < N; ++i) {
			rde::pair<strhash2::iterator, bool> p = h->insert(rde::pair<const char*, int>(data[i], i));
			if (p.second == false) h->erase(p.first);
		}
		ret = h->size();
		ReportMemUsage(*h);
		delete h;
		return ret; 
	} 

	static int data_size = 5000000;//000;
	static unsigned *int_data;
	static char **str_data; 
	void udb_init_data()
	{	
		int i;
		char buf[256];
		rde::Console::Printf("[benchmark] generating data... ");
		rde::Random::Init(11);
		int_data = (unsigned*)calloc(data_size, sizeof(unsigned));
		str_data = (char**)calloc(data_size, sizeof(char*));
		for (i = 0; i < data_size; ++i) {
			//int_data[i] = (unsigned)(data_size * drand48() / 4) * 271828183u;
			int_data[i] = rde::Random::NextInt();
			sprintf(buf, "%x", int_data[i]);
			str_data[i] = strdup(buf);
		}
		printf("done!\n");
	} 
KHASH_MAP_INIT_INT(int, int)
KHASH_MAP_INIT_STR(str, int)

int test_int2(int N, const unsigned *data)
{
	int i, ret;
	khash_t(int) *h;
	unsigned k;
	h = kh_init(int);
	for (i = 0; i < N; ++i) {
		k = kh_put(int, h, data[i], &ret);
		if (!ret) kh_del(int, h, k);
		else kh_value(h, k) = i;
	}
	ret = (int)kh_size(h);
	kh_destroy(int, h);
	return ret;
} 
int test_intstd(int N, const unsigned *data)
{
		int i, ret;
		typedef stdext::hash_map<unsigned, int> inthash2;
		inthash2* h = new inthash2;
		for (i = 0; i < N; ++i) {
			std::pair<inthash2::iterator, bool> p = h->insert(std::pair<unsigned, int>(data[i], i));
			if (p.second == false) h->erase(p.first);
		}
		ret = (int)h->size();
		delete h;
		return ret;
}

int test_intg(int N, const unsigned *data)
{
	typedef google::dense_hash_map<unsigned, int> inthashg;

	int i, ret;
	inthashg *h = new inthashg;
	h->set_empty_key(0xffffffffu);
	h->set_deleted_key(0xfffffffeu);
	for (i = 0; i < N; ++i) {
		std::pair<inthashg::iterator, bool> p = h->insert(std::pair<unsigned, int>(data[i], i));
		if (p.second == false) h->erase(p.first);
	}
	ret = (int)h->size();
	delete h;
	return ret;
}
 

int test_str2(int N, char * const *data)
{
	int i, ret;
	khash_t(str) *h;
	unsigned k;
	h = kh_init(str);
	for (i = 0; i < N; ++i) {
		k = kh_put(str, h, data[i], &ret);
		if (!ret) kh_del(str, h, k);
		else kh_value(h, k) = i;
	}
	ret = (int)kh_size(h);
	kh_destroy(str, h);
	return ret;
} 
} // namespace

void Map_SpeedTest()
{
#if 0
	//::MapSpeedTest();
	//udb_init_data();

	//std::sort(int_data, int_data + data_size);
	//int numDuplicates(0);
	//for (int i = 0; i < data_size - 1; ++i)
	//{
	//	int j = i + 1;
	//	while (int_data[i] == int_data[j])
	//	{
	//		++j;
	//		++numDuplicates;
	//	}
	//}
	//rde::Console::Printf("ND: %d\n", numDuplicates);

	rde::Timer timer;
	int r(0);
	//timer.Start();
	//int r = test_int(data_size, int_data);
	//timer.Stop();
	//rde::Console::Printf("Int test: %d [%d]\n", timer.GetTimeInMs(), r);

	//timer.Start();
	//r = test_int2(data_size, int_data);
	//timer.Stop();
	//rde::Console::Printf("Int test: %d [%d]\n", timer.GetTimeInMs(), r);

	//timer.Start();
	//r = test_intg(data_size, int_data);
	//timer.Stop();
	//rde::Console::Printf("GInt test: %d [%d]\n", timer.GetTimeInMs(), r);

	////timer.Start();
	////r = test_intstd(data_size, int_data);
	////timer.Stop();
	////rde::Console::Printf("Int test: %d [%d]\n", timer.GetTimeInMs(), r);

	//timer.Start();
	//r = test_str(data_size, str_data);
	//timer.Stop();
	//rde::Console::Printf("Str test: %d [%d]\n", timer.GetTimeInMs(), r);

	//timer.Start();
	//r = test_str3(data_size, str_data);
	//timer.Stop();
	//rde::Console::Printf("Str test: %d [%d]\n", timer.GetTimeInMs(), r);
#endif
}

#endif



