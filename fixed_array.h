#ifndef RDESTL_FIXED_ARRAY_H
#define RDESTL_FIXED_ARRAY_H

#include "algorithm.h"

namespace rde
{

//=============================================================================
template<typename T, size_t N>
class fixed_array
{
	typedef char ERR_N_MustBeBiggerThanZero[N > 0 ? 1 : -1];

public:
	typedef T			value_type;
	typedef T*			iterator;
	typedef const T*	const_iterator;
	typedef size_t		size_type;

	T m_data[N];

	// ctor, dtor, and assignment operator intentionally omitted

	iterator begin()				{ return &m_data[0]; }
	const_iterator begin() const	{ return &m_data[0]; }
	iterator end()					{ return begin() + N; }
	const_iterator end() const		{ return begin() + N; }
	size_type size() const			{ return N; }

	T* data()						{ return &m_data[0]; }
	const T* data() const			{ return &m_data[0]; }

	T& front()						{ return *begin(); }
	const T& front() const			{ return *begin(); }
	T& back()						{ return *(end() - 1); }
	const T& back() const			{ return *(end() - 1); }

	RDE_FORCEINLINE T& operator[](size_type i)
	{
		RDE_ASSERT(i < size());
		return m_data[i];
	}
	RDE_FORCEINLINE const T& operator[](size_type i) const
	{
		RDE_ASSERT(i < size());
		return m_data[i];
	}

	RDE_FORCEINLINE void fill(const T& value)
	{
		rde::fill_n(m_data, N, value);
	}
};

#if RDE_HAS_CPP11
// A condensed alias using standard container nomenclature
template<typename T, size_t N>
using array = fixed_array<T, N>;
#endif

} // namespace rde

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_FIXED_ARRAY_H
