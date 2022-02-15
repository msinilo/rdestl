#ifndef RDESTL_PAIR_H
#define RDESTL_PAIR_H

// Use std::pair (it's bigger, but the actual code is 'good enough'
// and it comes with more 'modern C++' bells and whistles (most notably, piecewise construct)
//#define RDESTL_USE_STD_PAIR 1

#include "type_traits.h"

#ifdef RDESTL_USE_STD_PAIR
#include <utility>
#endif

namespace rde
{

#ifdef RDESTL_USE_STD_PAIR
template<typename T1, typename T2>
using pair = std::pair<T1, T2>;
#else

//=============================================================================
template<typename T1, typename T2>
struct pair
{
	typedef T1	first_type;
	typedef T2	second_type;

	pair() { /**/ }
	pair(const T1& a, const T2& b): first(a), second(b) { /**/ }
	explicit pair(const T1& a): first(a) { /**/ }

	pair(const pair<T1, T2>& rhs): first(rhs.first), second(rhs.second) { /**/ }
	pair(pair<T1, T2>&& rhs): first(std::move(rhs.first)), second(std::move(rhs.second)) { /**/ }

	template<class... Args2>
	pair(T1&& first_args, Args2&&... second_args)
		: first(std::forward<T1>(first_args))
		, second(std::forward<Args2>(second_args)...) { /**/ }
	pair& operator=(const pair<T1, T2>& rhs)
	{
		first = rhs.first;
		second = rhs.second;
		return *this;
	}
	pair& operator=(pair<T1, T2>&& rhs)
	{
		first = std::move(rhs.first);
		second = std::move(rhs.second);
		return *this;
	}

	T1	first;
	T2	second;
};

#endif // #ifdef RDESTL_USE_STD_PAIR

//=============================================================================
// Pair is POD if every element is POD/fundamental
template<typename T1, typename T2>
struct is_pod<pair<T1, T2>>
{
	enum {
		value = (is_pod<T1>::value || is_fundamental<T1>::value) &&
		(is_pod<T2>::value || is_fundamental<T2>::value)
	};
};

//-----------------------------------------------------------------------------
template<typename T1, typename T2>
pair<T1, T2> make_pair(const T1& a, const T2& b)
{
	return pair<T1, T2>(a, b);
}

} // namespace rde

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_PAIR_H
