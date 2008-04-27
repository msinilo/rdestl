#ifndef RDESTL_FIXED_SUBSTRING_H
#define RDESTL_FIXED_SUBSTRING_H

#include "rdestl/fixed_array.h"
#include "rdestl/string_utils.h"

namespace rde
{
// "Substring" of fixed size.
// Cannot grow.
// Never allocates memory.
// !! WORK IN PROGRESS
template<typename E, size_t N>
class fixed_substring : private fixed_array<E, N>
{
	typedef char ERR_N_MustBeBiggerThanZero[N > 0 ? 1 : -1];
	typedef fixed_array<E, N>	Base;
public:
	typedef E			value_type;
	typedef size_t		size_type;

	fixed_substring()
	{
		data()[0] = value_type(0);
	}
	explicit fixed_substring(const value_type* str)
	{
		assign(str);
	}

	using Base::operator[];
	using Base::data;

	void assign(const value_type* str)
	{
		RDE_ASSERT(str != data());
		const size_type len = rde::strlen(str);
		const size_type toCopy = len < N-1 ? len : N-1;
		sys::MemCpy(data(), str, toCopy);
		data()[toCopy] = value_type(0);
	}

	bool empty() const
	{
		// @todo: consider caching string len
		return rde::strlen(data()) == 0;
	}
	bool operator==(const fixed_substring& rhs) const
	{
		return rde::strcompare(data(), rhs.data()) == 0;
	}
};

} // rde

#endif // #ifndef RDESTL_FIXED_SUBSTRING_H
