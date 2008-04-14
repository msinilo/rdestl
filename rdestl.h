#ifndef RDESTL_H
#define RDESTL_H

#ifndef RDESTL_STANDALONE
#	define RDESTL_STANDALONE	0
#endif

#if RDESTL_STANDALONE
#	include <cassert>
#	include <cstring>
#	define RDE_ASSERT		assert
// @note; __forceinline for MSVC!
#	define RDE_FORCEINLINE	inline
#	ifdef _DEBUG
#		undef RDE_DEBUG
#		define RDE_DEBUG	1
#	endif
	namespace rde 
	{ 
		namespace sys 
		{
			RDE_FORCEINLINE void MemCpy(void* to, const void* from, size_t bytes)
			{
				memcpy(to, from, bytes);
			}
			RDE_FORCEINLINE void MemMove(void* to, const void* from, size_t bytes)
			{
				memmove(to, from, bytes);
			}
			RDE_FORCEINLINE void MemSet(void* buf, unsigned char value, size_t bytes)
			{
				memset(buf, value, bytes);
			}
		} // sys
	}
#else
#	include "core/RdeDebug.h"
#	include "core/System.h"
#endif

#endif // #ifndef RDESTL_H
