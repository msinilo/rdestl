#ifndef RDESTL_H
#define RDESTL_H

#ifndef RDESTL_STANDALONE
#	define RDESTL_STANDALONE	1
#endif

#if RDESTL_STANDALONE
#	include <cassert>
#	define RDE_ASSERT		assert
// @note; __forceinline for MSVC!
#	define RDE_FORCEINLINE	inline
#	ifdef _DEBUG
#		undef RDE_DEBUG
#		define RDE_DEBUG	1
#	endif
#else
#	include "core/RdeDebug.h"
#endif

#endif // #ifndef RDESTL_H
