#include "rdestl/allocator.h"
#include "rdestl/rdestl.h"
#include <malloc.h>

namespace rde
{
//-----------------------------------------------------------------------------
void* allocator::allocate(size_t bytes, int /*flags*/)
{
	RDE_ASSERT(bytes != 0);
	return malloc(bytes);
}

//-----------------------------------------------------------------------------
#if 0
void* allocator::allocate_aligned(size_t bytes, size_t alignment, int /*flags*/)
{
	// TODO: IMPLEMENT
	return 0;
}
#endif

//-----------------------------------------------------------------------------
void allocator::deallocate(void* ptr, size_t /*bytes*/)
{
	free(ptr);
}

//-----------------------------------------------------------------------------
const char* allocator::get_name() const
{
	return m_name;
}

} // rde
