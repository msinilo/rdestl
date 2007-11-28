#ifndef RDESTL_ALGORITHM_H
#define RDESTL_ALGORITHM_H

#include "rdestl/int_to_type.h"
#include "rdestl/iterator.h"
#include "rdestl/type_traits.h"
#include "rdestl/utility.h"

namespace rdestl
{

template<typename T> RDE_FORCEINLINE
void copy_construct(T* mem, const T& orig)
{
	new (mem) T(orig);
}

template<typename T> RDE_FORCEINLINE
void construct(T* mem)
{
	internal::construct(mem, int_to_type<has_trivial_constructor<T>::value>());
}

// @todo: add typetraits
template<typename T> RDE_FORCEINLINE
void destruct(T* mem)
{
	internal::destruct(mem, int_to_type<has_trivial_destructor<T>::value>());
}

template<typename T>
void copy_n(const T* first, size_t n, T* result)
{
	internal::copy_n(first, n, result, int_to_type<has_trivial_copy<T>::value>());
}

template<typename T>
void copy(const T* first, const T* last, T* result)
{
	internal::copy(first, last, result, int_to_type<has_trivial_copy<T>::value>());
}

template<typename T>
void copy_construct_n(T* first, size_t n, T* result)
{
	internal::copy_construct_n(first, n, result, int_to_type<has_trivial_copy<T>::value>());
}
 
template<typename T>
void move_n(const T* from, size_t n, T* result)
{
	RDE_ASSERT(from != result || n == 0);
	// Overlap? 
	if (result > from && result < from + n)
	{
		internal::move_n(from, n, result, int_to_type<has_trivial_copy<T>::value>());
	}
	else
	{
		internal::copy_n(from, n, result, int_to_type<has_trivial_copy<T>::value>());
	}
}

template<typename T>
void move(const T* first, const T* last, T* result)
{
	RDE_ASSERT(first != result || first == last);
	if (result > first && result < last)
	{
		internal::move(first, last, result, int_to_type<has_trivial_copy<T>::value>());
	}
	else
	{
		internal::copy(first, last, result, int_to_type<has_trivial_copy<T>::value>());
	}
}

template<typename T>
void construct_n(T* first, size_t n)
{
	internal::construct_n(first, n, int_to_type<has_trivial_constructor<T>::value>());
}

template<typename T>
void destruct_n(T* first, size_t n)
{
	internal::destruct_n(first, n, int_to_type<has_trivial_destructor<T>::value>());
}

template<typename T> RDE_FORCEINLINE
void fill_n(T* first, size_t n, const T& val)
{
	for (size_t i = 0; i < n; ++i)
		first[i] = val;
}

//-----------------------------------------------------------------------------

template<typename TIter, typename TDist> inline
void distance(TIter first, TIter last, TDist& dist)
{
	internal::distance(first, last, dist, iterator_traits<TIter>::iterator_category());
}

template<typename TIter, typename TDist> inline
void advance(TIter& iter, TDist off)
{
	internal::advance(iter, off, iterator_traits<TIter>::iterator_category());
}

//-----------------------------------------------------------------------------
template<class TIter, typename T, class TPred> inline
TIter lower_bound(TIter first, TIter last, const T& val, const TPred& pred)
{
	internal::test_ordering(first, last, pred);
	int dist(0);
	distance(first, last, dist);
	while (dist > 0)
	{
		const int halfDist = dist >> 1;
		TIter mid = first;
		advance(mid, halfDist);
		if (internal::debug_pred(pred, *mid, val))
			first = ++mid, dist -= halfDist + 1;
		else
			dist = halfDist;
	}
	return first;
}

//-----------------------------------------------------------------------------
template<class TIter, typename T, class TPred> inline
TIter upper_bound(TIter first, TIter last, const T& val, const TPred& pred)
{
	internal::test_ordering(first, last, pred);
	int dist(0);
	distance(first, last, dist);
	while (dist > 0)
	{
		const int halfDist = dist >> 1;
		TIter mid = first;
		advance(mid, halfDist);
		if (!internal::debug_pred(pred, val, *mid))
			first = ++mid, dist -= halfDist + 1;
		else
			dist = halfDist;
	}
	return first;
}

//-----------------------------------------------------------------------------
template<class TIter, typename T>
TIter find(TIter first, TIter last, const T& val)
{
	while (first != last)
	{
		if (*first == val)
			return first;
		++first;
	}
	return last;
}

//-----------------------------------------------------------------------------
template<class TIter, typename T, class TPred>
TIter find_if(TIter first, TIter last, const T& val, const TPred& pred)
{
	while (first != last)
	{
		if (pred(*first, val))
			return first;
		++first;
	}
	return last;
}

} // namespace rdestl

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_ALGORITHM_H

