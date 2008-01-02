#ifndef RDESTL_VECTOR_H
#define RDESTL_VECTOR_H

#include "rdestl/algorithm.h"
#include "rdestl/allocator.h"
#include "rdestl/rdestl.h"

namespace rdestl
{
//=============================================================================
struct base_vector
{
	typedef size_t				size_type;
	static const size_type		npos = size_type(-1);
};

//=============================================================================
// Simplified vector class.
// Mimics std::vector.
template<typename T, class TAllocator = rdestl::allocator>
class vector : public base_vector
{
public:
	typedef T			value_type;
	typedef T*			iterator;
	typedef const T*	const_iterator;
	typedef TAllocator	allocator_type;

	explicit vector(const allocator_type& allocator = allocator_type())
	:	m_begin(0),
		m_end(0),
		m_capacity(0),
		m_allocator(allocator)
	{
		/**/
	}
	explicit vector(size_t initialSize, const allocator_type& allocator = allocator_type())
	:	m_begin(0),
		m_end(0),
		m_capacity(0),
		m_allocator(allocator)
	{
		resize(initialSize);
	}
	vector(const T* first, const T* last, const allocator_type& allocator = allocator_type())
	:	m_begin(0),
		m_end(0),
		m_capacity(0),
		m_allocator(allocator)
	{
		assign(first, last);
	}
	// @note: allocator is not copied from rhs.
	// @note: will not perform default constructor for newly created objects.
	vector(const vector& rhs, const allocator_type& allocator = allocator_type())
	:	m_begin(0),
		m_allocator(allocator)
#if RDE_DEBUG
		, m_capacity(0)
#endif
	{
		reallocate_discard_old(rhs.capacity());
		rdestl::copy_construct_n(rhs.m_begin, rhs.size(), m_begin);
		m_end = m_begin + rhs.size();
		RDE_ASSERT(invariant());
	}
	~vector()
	{
		if (m_begin != 0)
			destroy(m_begin, size());
	}

	// @note: allocator is not copied!
	// @note: will not perform default constructor for newly created objects.
	vector& operator=(const vector& rhs)
	{
		const size_type newSize = rhs.size();
		if (newSize > m_capacity)
		{
			reallocate_discard_old(rhs.capacity());
		}
		rdestl::copy_construct_n(rhs.m_begin, rhs.size(), m_begin);
		m_end = m_begin + rhs.size();
		RDE_ASSERT(invariant());
		
		return *this;
	}

	// @note: swap() not provided for the time being.

	iterator begin()				{ return m_begin; }
	const_iterator begin() const	{ return m_begin; }
	iterator end()					{ return m_end; }
	const_iterator end() const		{ return m_end; }
	size_type size() const			{ return size_type(m_end - m_begin); }
	size_type empty() const			{ return size() == 0; }
	size_type capacity() const		{ return m_capacity; }

	T* data()				{ return empty() ? 0 : m_begin; }
	const T* data() const	{ return empty() ? 0 : m_begin; }

	const T& front() const
	{
		RDE_ASSERT(!empty());
		return *begin();
	}
	const T& back() const
	{
		RDE_ASSERT(!empty());
		return *(end() - 1);
	}

	T& operator[](size_type i)
	{
		RDE_ASSERT(i < size());
		return m_begin[i];
	}
	const T& operator[](size_type i) const
	{
		RDE_ASSERT(i < size());
		return m_begin[i];
	}

	void push_back(const T& v)
	{
		if (size() == m_capacity)
			grow();
		rdestl::copy_construct(m_end, v);
		++m_end;
	}
	void pop_back()
	{
		RDE_ASSERT(!empty());
		--m_end;
		rdestl::destruct(m_end);
	}

	void assign(const T* first, const T* last)
	{
		// Iterators cant be in range!
		RDE_ASSERT(!validate_iterator(first));
		RDE_ASSERT(!validate_iterator(last));

		const size_type count = size_type(last - first);
		RDE_ASSERT(count > 0);
		if (count > m_capacity)
			grow_discard_old(count);
		rdestl::copy_n(first, count, m_begin);
		m_end = m_begin + count;
	}

	void insert(size_type index, size_type n, const T& val)
	{
		const size_type indexEnd = index + n;
		const size_type prevSize = size();
		if (prevSize + n > m_capacity)
			grow(prevSize + n);

		// Past 'end', needs to copy construct.
		if (indexEnd > prevSize)
		{
			const size_type numCopy		= prevSize - index;
			const size_type numAppend	= indexEnd - prevSize;
			RDE_ASSERT(numCopy >= 0 && numAppend >= 0);
			RDE_ASSERT(numAppend + numCopy == n);
			iterator itOut = m_begin + prevSize;
			for (size_type i = 0; i < numAppend; ++i, ++itOut)
				rdestl::copy_construct(itOut, val);
			rdestl::copy_construct_n(m_begin + index, numCopy, itOut);
			for (size_type i = 0; i < numCopy; ++i)
				m_begin[index + i] = val;
		}
		else
		{
			rdestl::copy_construct_n(m_end - n, n, m_end);
			iterator insertPos = m_begin + index;
			rdestl::move_n(insertPos, prevSize - indexEnd, insertPos + n);
			for (size_type i = 0; i < n; ++i)
				insertPos[i] = val;
		}
		m_end += n; 
	}
	// @pre validate_iterator(it)
	// @note use push_back for maximum efficiency if it == end()!
	void insert(iterator it, size_type n, const T& val)
	{
		RDE_ASSERT(validate_iterator(it));
		insert(size_type(it - m_begin), n, val);
	}
	iterator insert(iterator it, const T& val)
	{
		RDE_ASSERT(validate_iterator(it));
		const size_type index = it - m_begin;
		const size_type prevSize = size();
		RDE_ASSERT(index <= prevSize);
		const size_type toMove = prevSize - index;
		// @todo: optimize for toMove==0 --> push_back here?
		if (prevSize == m_capacity)
		{
			grow();
			it = m_begin + index;
		}
		else
			rdestl::construct(m_begin + prevSize);

		// @note: conditional vs empty loop, what's better?
		if (toMove > 0)
		{
			rdestl::internal::move_n(it, toMove, it + 1, int_to_type<has_trivial_copy<T>::value>());
		}
		*it = val;
		++m_end;
		return it;
	}

	// @pre validate_iterator(it)
	// @pre it != end()
	iterator erase(iterator it)
	{
		RDE_ASSERT(validate_iterator(it));
		RDE_ASSERT(it != end());  
		// Move everything down, overwriting *it
		rdestl::copy(it + 1, m_end, it);
		--m_end;
		rdestl::destruct(m_end);
		return it;
	}
	iterator erase(iterator first, iterator last)
	{
		RDE_ASSERT(validate_iterator(first));
		RDE_ASSERT(validate_iterator(last));
		if (last <= first)
			return end();
		
		const size_type indexFirst = size_type(first - m_begin);
		const size_type toRemove = size_type(last - first);
		if (toRemove > 0)
		{
			//const size_type toEnd = size_type(m_end - last);
			rdestl::move(last, m_end, first);
			shrink(size() - toRemove);
		}
		return m_begin + indexFirst;
	}

	// *Much* quicker version of erase, doesnt preserve collection order.
	void erase_unordered(iterator it)
	{
		RDE_ASSERT(validate_iterator(it));
		RDE_ASSERT(it != end());
		const iterator itNewEnd = end() - 1;
		if (it != itNewEnd)
			*it = *itNewEnd;
		pop_back();
	}

	void resize(size_type n)
	{
		if (n > size())
			insert(m_end, n - size(), value_type());
		else
			shrink(n);

		// slower version
		//erase(m_begin + n, m_end);
	}
	void reserve(size_type n)
	{
		reallocate(n);
	}

	// Removes all elements from this vector (calls their destructors).
	// Doesnt release memory.
	void clear()
	{
		shrink(0);
		RDE_ASSERT(invariant());
	}

	void set_capacity(size_type newCapacity)
	{
		reallocate(newCapacity, true);
	}

	size_type index_of(const T& item, size_type index = 0) const
	{
		RDE_ASSERT(index >= 0 && index < size());
		for ( ; index < size(); ++index)
			if (m_begin[index] == item)
				return index;
		return npos;
	}

	const allocator_type& get_allocator() const	{ return m_allocator; }
	void set_allocator(const allocator_type& allocator)
	{
		m_allocator = allocator;
	}

	bool validate_iterator(const_iterator it) const
	{
		return it >= begin() && it <= end();
	}

private:
	inline void reallocate(size_type newCapacity, bool canShrink = false)
	{
		if (canShrink || newCapacity > m_capacity)
		{
			T* newBegin = static_cast<T*>(m_allocator.allocate(newCapacity * sizeof(T)));
			const size_type newSize = std::min(size(), newCapacity);
			// Copy old data if needed.
			if (m_begin)
			{
				rdestl::copy_construct_n(m_begin, newSize, newBegin);
				destroy(m_begin, size());
			}
			m_begin = newBegin;
			m_end = m_begin + newSize;
			m_capacity = newCapacity;
			RDE_ASSERT(invariant());
		}
	}

	// Reallocates memory, doesnt copy contents of old buffer.
	RDE_FORCEINLINE void reallocate_discard_old(size_type newCapacity)
	{
		RDE_ASSERT(newCapacity > m_capacity);
		T* newBegin = static_cast<T*>(m_allocator.allocate(newCapacity * sizeof(T)));
		const size_type currSize = size();
		if (m_begin)
			destroy(m_begin, currSize);
		m_begin = newBegin;
		m_end = m_begin + currSize;
		m_capacity = newCapacity;
		RDE_ASSERT(invariant());
	}

	RDE_FORCEINLINE void destroy(T* ptr, size_type n)
	{
		rdestl::destruct_n(ptr, n);
		m_allocator.deallocate(ptr, n * sizeof(T));
	}

	RDE_FORCEINLINE size_type compute_new_capacity(size_type newMinCapacity) const
	{
		return (newMinCapacity > m_capacity * 2 ? newMinCapacity : (m_capacity == 0 ? 16 : m_capacity * 2));
	}
	RDE_FORCEINLINE void grow(size_type newMinCapacity = 0)
	{
		reallocate(compute_new_capacity(newMinCapacity));
	}
	RDE_FORCEINLINE void grow_discard_old(size_type newMinCapacity = 0)
	{
		reallocate_discard_old(compute_new_capacity(newMinCapacity));
	}
	RDE_FORCEINLINE void shrink(size_type newSize)
	{
		RDE_ASSERT(newSize <= size());
		const size_type toShrink = size() - newSize;
		rdestl::destruct_n(m_begin + newSize, toShrink);
		m_end = m_begin + newSize;
	}

	bool invariant() const
	{
		return m_end >= m_begin;
	}

	T*				m_begin;
	T*				m_end;
	size_type		m_capacity;
	allocator_type	m_allocator;
};

} // namespace rdestl

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_VECTOR_H
