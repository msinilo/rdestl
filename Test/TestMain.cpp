#include <UnitTest++/src/UnitTest++.h>

#define SPEED_TEST	1

void RunSpeedTests();

int __cdecl main(int, char const *[])
{
    if (UnitTest::RunAllTests() != 0)
		return 1;

#if SPEED_TEST
	RunSpeedTests();
#endif

	return 0;
}
