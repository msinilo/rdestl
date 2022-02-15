#ifndef RDESTL_ALIGNMENT_H
#define RDESTL_ALIGNMENT_H

#include "rdestl_common.h"
#include <cstddef>

namespace rde
{
namespace internal
{
#pragma warning(push)
// structure was padded due to __declspec(align())
#pragma warning(disable: 4324)

template<typename T>
struct alignof_helper
{
	char	x;
	T		y;
};

#ifdef _MSC_VER
__declspec(align(16)) struct aligned16 { std::uint64_t member[2]; };
#else
struct __attribute__((aligned(16))) aligned16 { std::uint64_t member[2]; };
#endif

#pragma warning(pop)

template<size_t N> struct type_with_alignment
{
	typedef char err_invalid_alignment[N > 0 ? -1 : 1];
};
template<> struct type_with_alignment<0> {};
template<> struct type_with_alignment<1> { std::uint8_t member; };
template<> struct type_with_alignment<2> { std::uint16_t member; };
template<> struct type_with_alignment<4> { std::uint32_t member; };
template<> struct type_with_alignment<8> { std::uint64_t member; };
template<> struct type_with_alignment<16> { aligned16 member; };
} // namespace internal

template<typename T>
struct rde_alignof
{
	enum
	{
		res = offsetof(internal::alignof_helper<T>, y)
	};
};
template<typename T>
struct aligned_as
{
	typedef typename internal::type_with_alignment<rde_alignof<T>::res> res;
};

} // namespace rde

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_ALIGNMENT_H
