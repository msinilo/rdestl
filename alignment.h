#ifndef RDESTL_ALIGNMENT_H
#define RDESTL_ALIGNMENT_H

#include "rdestl/rdestl.h"
#include <cstddef>

namespace rde
{
namespace internal
{
	template<typename T>
	struct alignof_helper
	{
		char	x;
		T		y;
	};
	template<size_t N> struct type_with_alignment
	{
		typedef char err_invalid_alignment;
	};
	template<> struct type_with_alignment<0> {};
	template<> struct type_with_alignment<1> { uint8_t member; };
	template<> struct type_with_alignment<2> { uint16_t member; };
	template<> struct type_with_alignment<4> { uint32_t member; };
	template<> struct type_with_alignment<8> { uint64_t member; };
}
template<typename T>
struct alignof
{
	enum 
	{
		res = offsetof(internal::alignof_helper<T>, y)
	};
};
template<typename T>
struct aligned_as
{
	typedef typename internal::type_with_alignment<alignof<T>::res> res;
};

}

#endif
