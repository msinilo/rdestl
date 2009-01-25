#ifndef RDESTL_SORT_H
#define RDESTL_SORT_H

#include "rdestl/functional.h"

namespace rde
{
namespace internal
{
template<typename T, class TPredicate>
size_t partition(T* data, size_t low, size_t high, TPredicate pred, const T& pivot)
{
	size_t i = low;
	size_t j = high;
	while (true)
	{
		while (pred(data[i], pivot))
			++i;
		--j;
		while (pred(pivot, data[j]))
			--j;
		if (i >= j)
			return i;
		rde::swap(data[i], data[j]);
		++i;
	}
}
template<typename T, class TPredicate>
inline const T& median_of_3(T* data, size_t low, size_t mid, size_t high, TPredicate pred)
{
	if (pred(data[mid], data[low]))
		return pred(data[high], data[mid]) ? data[high] : pred(data[high], data[low]) ? data[high] : data[low];
	else
		return pred(data[high], data[mid]) ? pred(data[high], data[low]) ? data[low] : data[high] : data[mid];
}
template<typename T, class TPredicate>
void quick_sort(T* data, size_t low, size_t high, TPredicate pred)
{
	while (high - low > 16)
	{
		const size_t mid = low + (high - low) / 2 + 1;
		const T& med3 = median_of_3(data, low, mid, high - 1, pred);
		const size_t p = partition(data, low, high, pred, med3);
		quick_sort(data, p, high, pred);
		high = p;
	}
}

} // internal

template<typename T, class TPredicate>
void insertion_sort(T* begin, T* end, TPredicate pred)
{
	if (begin == end)
		return;

	const size_t num = end - begin;	
	for (size_t i = 0; i < num; ++i)
	{
		size_t j = i;
		const T& t = begin[i];
		while (j > 0 && pred(t, begin[j - 1]))
		{
			begin[j] = begin[j - 1];
			--j;
		}
		begin[j] = t;
	}
}
template<typename T>
void insertion_sort(T* begin, T* end)
{
	insertion_sort(begin, end, less<T>());
}

template<typename T, class TPredicate>
void quick_sort(T* begin, T* end, TPredicate pred)
{
	internal::quick_sort(begin, 0, end - begin, pred);
	insertion_sort(begin, end, pred);
}
template<typename T>
void quick_sort(T* begin, T* end)
{
	quick_sort(begin, end, less<T>());
}

}

#endif 
