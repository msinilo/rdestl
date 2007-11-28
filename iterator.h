#ifndef RDESTL_ITERATOR_H
#define RDESTL_ITERATOR_H

#include "rdestl/rdestl.h"

namespace rdestl
{

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iterator_tag: public forward_iterator_tag {};
struct random_access_iterator_tag: public bidirectional_iterator_tag {};

template<typename IterT>
struct iterator_traits 
{
   typedef typename IterT::iterator_category iterator_category;
};

template<typename T>          
struct iterator_traits<T*> 
{
   typedef random_access_iterator_tag iterator_category;
};

namespace internal
{
	template<typename TIter, typename TDist> RDE_FORCEINLINE
	void distance(TIter first, TIter last, TDist& dist, rdestl::random_access_iterator_tag)
	{
		dist = TDist(last - first);
	}
	template<typename TIter, typename TDist> RDE_FORCEINLINE
	void distance(TIter first, TIter last, TDist& dist, rdestl::input_iterator_tag)
	{
		dist = 0;
		while (first != last)
		{
			++dist;
			++first;
		}
	}

	template<typename TIter, typename TDist> RDE_FORCEINLINE
	void advance(TIter& iter, TDist d, rdestl::random_access_iterator_tag)
	{
		iter += d;
	}
	template<typename TIter, typename TDist> RDE_FORCEINLINE
	void advance(TIter& iter, TDist d, rdestl::bidirectional_iterator_tag)
	{
		if (d >= 0)
		{
			while (d--)
				++iter;
		}
		else
		{
			while (d++)
				--iter;
		}
	}
	template<typename TIter, typename TDist> RDE_FORCEINLINE
	void advance(TIter& iter, TDist d, rdestl::input_iterator_tag)
	{
		RDE_ASSERT(d >= 0);
		while (d--)
			++iter;
	}
} // namespace internal
} // namespace rdestl

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_ITERATOR_H
