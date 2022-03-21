#ifndef RDESTL_COMMON_H
#define RDESTL_COMMON_H

#ifndef RDESTL_STANDALONE
#	define RDESTL_STANDALONE	1
#endif

#ifdef _MSC_VER
#	if _MSC_VER < 1600
#		error platform not supported
#	elif _MSC_VER < 1700
#		define RDE_COMPILER_MSVC_2010 1
#	endif
#endif

#ifdef __cplusplus
#	if __cplusplus <= 199711L // C++98 and earlier
#		define RDE_HAS_CPP98 1
#	elif __cplusplus >= 201103L // C++11 and later
#		define RDE_HAS_CPP11 1
#	endif
#endif

#if RDESTL_STANDALONE

#	ifdef _MSC_VER
#		define NOMINMAX
#		undef min
#		undef max
#		ifndef _ALLOW_RTCc_IN_STL
#		define _ALLOW_RTCc_IN_STL
#		endif
#		include <cassert>
#		include <cstring>
#		define RDE_FORCEINLINE	__forceinline
#	else
#		include <assert.h>
#		include <cstdlib>
#		include <cstring>
#		define RDE_FORCEINLINE	inline
#	endif

#	ifdef _DEBUG
#		undef RDE_DEBUG
#		define RDE_DEBUG	1
#	endif

#	define RDE_ASSERT	assert

#include <cstdint>

namespace rde
{
	namespace Sys
	{

		RDE_FORCEINLINE void MemCpy(void* to, const void* from, size_t bytes)
		{
			std::memcpy(to, from, bytes);
		}
		RDE_FORCEINLINE void MemMove(void* to, const void* from, size_t bytes)
		{
			std::memmove(to, from, bytes);
		}
		RDE_FORCEINLINE void MemSet(void* buf, unsigned char value, size_t bytes)
		{
			std::memset(buf, value, bytes);
		}

	} // namespace Sys
} // namespace rde

#endif // #if RDESTL_STANDALONE

namespace rde
{

enum e_noinitialize { noinitialize };

} // namespace rde

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_COMMON_H
