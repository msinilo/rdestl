#include "rdestl/allocator.h"
#include "rdestl/rdestl.h"
#include <malloc.h>

namespace rdestl
{
//-----------------------------------------------------------------------------
void* allocator::allocate(size_t bytes, int /*flags*/)
{
	RDE_ASSERT(bytes != 0);
	return malloc(bytes);
}

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

}
