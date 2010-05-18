
#include <cstring>

#include "allocator.h"

namespace rde
{
typedef unsigned int size_t;
    
//-----------------------------------------------------------------------------
void* allocator::allocate(size_t bytes, int /*flags*/)
{
	return operator new(bytes);
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
	if (ptr)
		operator delete(ptr);
}

//-----------------------------------------------------------------------------
const char* allocator::get_name() const
{
	return m_name;
}

} // rde
