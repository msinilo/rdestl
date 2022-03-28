#include <cstdio>
#include "rdestl.h"
#define CATCH_CONFIG_RUNNER
#include "vendor/Catch/catch.hpp"

#if !RDESTL_STANDALONE
#	define SPEED_TEST	1
#endif

void RunSpeedTests();

#if RDE_DEBUG
long& g_BreakOnAlloc(_crtBreakAlloc);
#endif


int __cdecl main(int argc, const char* argv[])
{
	int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flags |= (flags & 0x0000FFFF) | _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(flags);

#if SPEED_TEST
	void Map_SpeedTest();
	Map_SpeedTest();
#endif

	int result = Catch::Session().run(argc, argv);

#if SPEED_TEST
	RunSpeedTests();
#endif

	_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	// Enable leak report if debugger attached
	flags |= _CRTDBG_LEAK_CHECK_DF;
	// Set the new bits
	_CrtSetDbgFlag(flags);

	return result < 0xFF ? result : 0xFF;
}
