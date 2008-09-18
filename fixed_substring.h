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
class fixed_substring : private fixed_array<E, N + 1>
{
	typedef fixed_array<E, N + 1>	Base;
public:
	typedef E		value_type;
	typedef int		size_type;

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
		const size_type len = (size_type)rde::strlen(str);
		const size_type toCopy = len < N ? len : N;
		Sys::MemCpy(data(), str, toCopy * sizeof(value_type));
		data()[toCopy] = value_type(0);
	}
	void append(const value_type* str)
	{
		const size_type strLen = (size_type)rde::strlen(str);
		const size_type ourLen = (size_type)rde::strlen(data());
		size_type toAppend = strLen;
		if (ourLen + toAppend > N)
			toAppend = N - ourLen;
		Sys::MemCpy(data() + ourLen, str, toAppend * sizeof(value_type));
		data()[ourLen + toAppend] = value_type(0);
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
