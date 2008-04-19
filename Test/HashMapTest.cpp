#include <UnitTest++/src/UnitTest++.h>
#include "rdestl/hash_map.h"
#include "rdestl/hash_map2.h"
#include <cstdio>

#if !RDESTL_STANDALONE
#	include "rdestl/vector.h"
#	include "core/Timer.h"
#	include "profile/Profile.h"
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

namespace
{
	struct hasher
	{
		size_t operator()(const std::string& s) const
		{
			size_t len = s.length();
			size_t hash(0);
			for (size_t i = 0; i < len; ++i)
			{
				hash *= 31;
				hash += s[i];
			}
			return hash;
		}
	};
	struct poor_hasher
	{
		size_t operator()(const std::string& s) const
		{
			return s == "crashtest" ? 4 : 1;
		}
	};

#define CONCAT_(x, y)	x ## y
#define CONCAT2_(x, y)	CONCAT_(x, y)
#define TESTC(x)		TEST(CONCAT2_(x, POSTFIX))

#define POSTFIX				Linked
#define tMap				rde::hash_map<std::string, int, hasher>
#define tPoorlyHashedMap	rde::hash_map<std::string, int, poor_hasher>
#include "HashMapTestInc.h"
	
#undef tMap
#undef tPoorlyHashedMap
#undef POSTFIX
#define POSTFIX				Closed
#define tMap				rde::hash_map2<std::string, int, hasher>
#define tPoorlyHashedMap	rde::hash_map2<std::string, int, poor_hasher>
#include "HashMapTestInc.h"


#if !RDESTL_STANDALONE
	#undef tMap
#	define tMap				rde::hash_map<std::string, int, hasher>
	typedef rde::hash_map2<std::string, int, hasher>		tMap2;
	template<typename TMAP>
	void HashMapSpeedTest(profile::BlockInfo& blockEnumerate, profile::BlockInfo& blockIter,
		profile::BlockInfo& blockRemove)
	{
		TMAP fileMap;
		rde::vector<std::string> allFiles;

		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile("c:\\Windows\\System32\\*.*", &ffd);
		if (hFind)
		{
			int i(0);
			//double deltaTicks[3];
			{
				profile::ScopedProfile prof(blockEnumerate);
				do
				{
					if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						fileMap.insert(rde::make_pair(std::string(ffd.cFileName), i));
						allFiles.push_back(std::string(ffd.cFileName));
						++i;
					}
				} while (FindNextFile(hFind, &ffd) != 0);
				FindClose(hFind);
			}
			//printf("Enumerating took %.3f ticks\n", deltaTicks[0]);
			printf("%d node(s) found, hash map uses %d buckets and ~%d bytes [%d collision(s)]\n", fileMap.size(), 
				fileMap.bucket_count(), fileMap.used_memory(), fileMap.collisions());

			// Iteration
			{
				profile::ScopedProfile prof(blockIter);
				for (TMAP::iterator it = fileMap.begin(); it != fileMap.end(); ++it)
				{
					tolower(it->first.c_str()[0]);
				}
			}
			//printf("Iteration took %.3f ticks\n", deltaTicks[1]);

			{
				profile::ScopedProfile prof(blockRemove);
				// Now try to remove some % of randomly selected items.
				const int numToRemove = int(0.45f * float(allFiles.size()));
				//printf("Removing %d randomly selected item(s)\n", numToRemove);
				int notFound(0);
				for (int i = 0; i < numToRemove; ++i)
				{
					const int j = rand() % allFiles.size();
					const std::string& str = allFiles[j];
					if (!fileMap.erase(str))
						++notFound;
				}
			}
			//printf("Removal took %.3f ticks [%d node(s) not removed]\n", deltaTicks[2], notFound);
		}
	}

	void ReportProfileBlock(const profile::BlockInfo& info)
	{
		char buf[256];
		info.Report(buf, RDE_ARRAY_COUNT(buf));
		printf("%s", buf);
	}

	TEST(HashMapSpeedTest)
	{
		profile::BlockInfo enumLinked("LL hashmap - enumeration");
		profile::BlockInfo enumClosed("Closed hashmap - enumeration");
		profile::BlockInfo iterLinked("LL hashmap - iteration");
		profile::BlockInfo iterClosed("Closed hashmap - iteration");
		profile::BlockInfo removeLinked("LL hashmap - removal");
		profile::BlockInfo removeClosed("Closed hashmap - removal");
		profile::BlockInfo blockLinked("Linked-list hashmap - TOTAL");
		profile::BlockInfo blockClosed("Closed hashmap - TOTAL");
		
		const int kNumTests = 10;
		for (int i = 0; i < kNumTests; ++i)
		{
			{
				profile::ScopedProfile prof(blockLinked);
				HashMapSpeedTest<tMap>(enumLinked, iterLinked, removeLinked);
			}
			{
				profile::ScopedProfile prof(blockClosed);
				HashMapSpeedTest<tMap2>(enumClosed, iterClosed, removeClosed);
			}
		}

		profile::BlockInfo::EnumerateAll(ReportProfileBlock);

		CHECK(1);
	} 
#endif
}
