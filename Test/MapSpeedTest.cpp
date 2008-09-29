#include "rdestl/hash_map.h"

#if !RDESTL_STANDALONE

//#include "rdestl/map.h"
//#include "rdestl/ternary_search_tree.h"
#include "rdestl/vector.h"
#include <hash_map>
#include <map>
#include <string>

#include "core/Console.h"
#include "core/Random.h"
#include "core/StackTrace.h"
#include "core/Timer.h"
#include "core/win32/Windows.h"

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
			ProfileResult("STD hashmap"),
			ProfileResult("STD map"),
			ProfileResult("RDE map"),
			ProfileResult("TST")
		};
		for (int i = 0; i < kNumTests; ++i)
		{
			MapSpeedTest<tMap, tMap::value_type>(results[0]);
			MapSpeedTest<tMap4, tMap4::value_type>(results[1]);
			MapSpeedTest<tMap5, tMap5::value_type>(results[2]);
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
}

void Map_SpeedTest()
{
	::MapSpeedTest();
}

#endif



