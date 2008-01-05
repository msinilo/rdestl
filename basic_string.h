#ifndef RDESTL_BASIC_STRING_H
#define RDESTL_BASIC_STRING_H

#include "rdestl/allocator.h"
#include "rdestl/rdestl.h"

namespace rdestl
{
// @todo: wrapper over memcpy

// @todo: move to another file
//-----------------------------------------------------------------------------
template<typename T> int strlen(const T* str)
{
	int len(0);
	while (*str++)
		++len;
	return len;
}

//-----------------------------------------------------------------------------
template<typename T> int strcompare(const T* s1, const T* s2, size_t len)
{
	for (/**/; len != 0; --len)
	{
		const T c1 = *s1;
		const T c2 = *s2;
		if (c1 != c2)
			return (c1 < c2 ? -1 : 1);

		++s1;
		++s2;
	}
	return 0;
}

//=============================================================================
struct string_rep
{
	// @todo: thread-safe version.
	void add_ref() { ++refs; }
	bool release() { --refs; return refs <= 0; }

	void init(short new_capacity = 0)
	{
		refs = 1;
		size = 0;
		capacity = new_capacity;
	}

	short	refs;
	short	size;
	short	capacity;
	// [data]
	static const size_t kMaxCapacity = (1 << (sizeof(short) << 3)) >> 1;
};
typedef char ERR_InvalidStringRepSize[sizeof(string_rep) == 6 ? 1 : -1];

//=============================================================================
// @todo: move to own header.
template<typename E, class TAllocator>
class cow_string_storage
{
	typedef char ERR_CharTypeTooBigSeeM_BufferComment[sizeof(E) <= 2 ? 1 : -1];
public:
	typedef E					value_type;
	typedef size_t				size_type;
	typedef TAllocator			allocator_type;
	typedef const value_type*	const_iterator;
	static const size_type	kGranularity = 32;	

	explicit cow_string_storage(const allocator_type& allocator)
	:	m_allocator(allocator)
	{
		construct_string(0);
	}
	cow_string_storage(const value_type* str, const allocator_type& allocator)
	:	m_allocator(allocator)
	{
		const int len = strlen(str);
		construct_string(len);
		memcpy(m_data, str, len*sizeof(value_type));
		RDE_ASSERT(len < string_rep::kMaxCapacity);
		get_rep()->size = static_cast<short>(len);
		m_data[len] = 0;
	}
	cow_string_storage(const value_type* str, size_type len, 
		const allocator_type& allocator)
	:	m_allocator(allocator)
	{
		construct_string(len);
		memcpy(m_data, str, len*sizeof(value_type));
		RDE_ASSERT(len < string_rep::kMaxCapacity);
		get_rep()->size = static_cast<short>(len);
		m_data[len] = 0;
	}
	cow_string_storage(const cow_string_storage& rhs, const allocator_type& allocator)
	:	m_data(rhs.m_data),
		m_allocator(allocator)
	{
		get_rep()->add_ref();
	}
	~cow_string_storage()
	{
		release_string();
	}

	// @note: doesnt copy allocator
	cow_string_storage& operator=(const cow_string_storage& rhs)
	{
		if (m_data != rhs.m_data)
		{
			release_string();
			m_data = rhs.m_data;
			get_rep()->add_ref();
		}
		return *this;
	}

	void assign(const value_type* str, size_type len)
	{
		// Do not use with str = str2.c_str()!
		RDE_ASSERT(str != m_data);
		release_string();
		construct_string(len);
		memcpy(m_data, str, len*sizeof(value_type));
		get_rep()->size = len;
		m_data[len] = 0;
	}

	void append(const value_type* str, size_type len)
	{
		const size_type prevLen = length();
		const size_type newCapacity = prevLen + len;
		make_unique(newCapacity);
		string_rep* rep = get_rep();
		RDE_ASSERT(rep->capacity >= short(newCapacity));
		const size_type newLen = prevLen + len;
		RDE_ASSERT(short(newLen) <= rep->capacity);
		memcpy(m_data + prevLen, str, len);
		m_data[newLen] = 0;
		rep->size = newLen;
	}

	inline const value_type* c_str() const
	{
		return m_data;
	}
	inline size_type length() const
	{
		return get_rep()->size;
	}
	const allocator_type& get_allocator() const	{ return m_allocator; }

protected:
	bool invariant() const
	{
		RDE_ASSERT(m_data);
		string_rep* rep = get_rep();
		RDE_ASSERT(rep->refs >= 1);
		RDE_ASSERT(rep->size <= rep->capacity);
		if (length() != 0)
			RDE_ASSERT(m_data[length()] == 0);
		return true;
	}
private:
	RDE_FORCEINLINE string_rep* get_rep() const
	{
		return reinterpret_cast<string_rep*>(m_data) - 1;
	}
	void construct_string(size_type capacity)
	{
		if (capacity != 0)
		{
			capacity = (capacity+kGranularity-1) & ~(kGranularity-1);
			if (capacity < kGranularity)
				capacity = kGranularity;
			RDE_ASSERT(capacity < string_rep::kMaxCapacity);

			const size_type toAlloc = sizeof(string_rep) + sizeof(value_type)*capacity;
			void* mem = m_allocator.allocate(toAlloc);
			string_rep* rep = reinterpret_cast<string_rep*>(mem);
			rep->init(static_cast<short>(capacity));
			m_data = reinterpret_cast<value_type*>(rep + 1);
		}
		else	// empty string, no allocation needed. Use our internal buffer.
		{
			string_rep* rep = reinterpret_cast<string_rep*>(m_buffer);
			rep->init();
			m_data = reinterpret_cast<value_type*>(rep + 1);
		}
		*m_data = 0;
	}
	void release_string()
	{
		string_rep* rep = get_rep();
		if (rep->release() && rep->capacity != 0)
		{
			// Make sure it was a dynamically allocated data.
			RDE_ASSERT(rep != reinterpret_cast<string_rep*>(&m_buffer[0]));
			m_allocator.deallocate(rep, rep->capacity);
		}
	}
	void make_unique(size_type capacity_hint)
	{
		string_rep* rep = get_rep();
		RDE_ASSERT(rep->refs >= 1);

		capacity_hint = (capacity_hint+kGranularity-1) & ~(kGranularity-1);
		if (capacity_hint < kGranularity)
			capacity_hint = kGranularity;
		RDE_ASSERT(capacity_hint < string_rep::kMaxCapacity);
		// Reallocate string only if we truly need to make it unique
		// (it's shared) or if our current buffer is to small.
		if (rep->refs > 1 || short(capacity_hint) > rep->capacity)
		{
			if (capacity_hint > 0)
			{
				void* newMem = m_allocator.allocate(capacity_hint);
				string_rep* newRep = reinterpret_cast<string_rep*>(newMem);
				newRep->init(capacity_hint);
				value_type* newData = reinterpret_cast<value_type*>(newRep + 1);
				memcpy(newData, m_data, rep->size*sizeof(value_type));
				newRep->size = rep->size;
				newData[rep->size] = 0;
				release_string();
				m_data = newData;
			}
			else
			{
				release_string();
				string_rep* rep = reinterpret_cast<string_rep*>(m_buffer);
				rep->init();
				m_data = reinterpret_cast<value_type*>(rep + 1);
				*m_data = 0;
			}
		}
	}

	E*			m_data;
	// @note: hack-ish. sizeof(string_rep) bytes for string_rep, than place for terminating
	// character (up to 2-bytes!)
	char		m_buffer[sizeof(string_rep)+2]; 
	TAllocator	m_allocator;
};

//=============================================================================
// @note: this one is totally _not_ std::string compatible for the time being!
// one way conversion should work, ie RDESTL --> STL.
template<typename E, 
	class TAllocator = rdestl::allocator,
	typename TStorage = cow_string_storage<E, TAllocator> >
class basic_string : private TStorage
{
public:
	typedef typename TStorage::value_type		value_type;
	typedef typename TStorage::size_type		size_type;
	typedef typename TStorage::const_iterator	const_iterator;
	typedef typename TStorage::allocator_type	allocator_type;

	explicit basic_string(const allocator_type& allocator = allocator_type())
	:	TStorage(allocator)
	{
		/**/
	}
	// yeah, EXPLICIT.
	explicit basic_string(const value_type* str, 
		const allocator_type& allocator = allocator_type())
	:	TStorage(str, allocator)
	{
		/**/
	}
	basic_string(const value_type* str, size_type len, 
		const allocator_type& allocator = allocator_type())
	:	TStorage(str, len, allocator)
	{
		/**/
	}
	basic_string(const basic_string& str, 
		const allocator_type& allocator = allocator_type())
	:	TStorage(str, allocator)
	{
		/**/
	}
	~basic_string()
	{
		/**/
	}

	// No operator returning ref for the time being. It's dangerous with COW.
	value_type operator[](size_type i) const
	{
		RDE_ASSERT(i < length());
		RDE_ASSERT(invariant());
		return c_str()[i];
	}

	basic_string& operator=(const basic_string& rhs)
	{
		RDE_ASSERT(rhs.invariant());
		TStorage& s = *this;
		s = rhs;
		RDE_ASSERT(invariant());
		return *this;
	}
	basic_string& operator=(const value_type* str)
	{
		return assign(str);
	}

	basic_string& assign(const value_type* str, size_type len)
	{
		TStorage::assign(str, len);
		RDE_ASSERT(invariant());
		return *this;
	}
	basic_string& assign(const value_type* str)
	{
		return assign(str, strlen(str));
	}

	basic_string substr(size_type begin, size_type end = length()) const
	{
		RDE_ASSERT(end >= begin && end <= length() && begin >= 0);
		return basic_string(c_str() + begin, end - begin);
	}

	void append(const value_type* str, size_type len)
	{
		TStorage::append(str, len);
	}
	void append(const basic_string& str)
	{
		append(str.c_str(), str.length());
	}
	void append(const value_type* str)
	{
		append(str, strlen(str));
	}
	basic_string& operator+=(const basic_string& rhs)
	{
		append(str);
		return *this;
	}

	int compare(const value_type* str) const
	{
		const size_type thisLen = length();
		const size_type strLen = strlen(str);
		if (thisLen < strLen)
			return -1;
		if (thisLen > strLen)
			return 1;

		return strcompare(c_str(), str, thisLen);
	}
	int compare(const basic_string& rhs) const
	{
		// COW optimization.
		// Commented out, COW as policy now.
		//if (c_str() == rhs.c_str())
		//	return 0;

		const size_type thisLen = length();
		const size_type rhsLen = rhs.length();
		if (thisLen < rhsLen)
			return -1;
		if (thisLen > rhsLen)
			return 1;

		return strcompare(c_str(), rhs.c_str(), thisLen);
	}

	// @note: do NOT const_cast!
	const value_type* c_str() const
	{
		RDE_ASSERT(invariant());
		return TStorage::c_str();
	}
	const_iterator begin() const	{ RDE_ASSERT(invariant()); return c_str(); }
	const_iterator end() const		{ RDE_ASSERT(invariant()); return c_str() + length(); }

	size_type length() const
	{
		return TStorage::length();
	}
	bool empty() const	{ return length() == 0; }

	const allocator_type& get_allocator() const	{ return TStorage::get_allocator; }

private:
	bool invariant() const
	{
		return TStorage::invariant();
	}
};
// @todo: move to string.h, move COW stuff to seperated file.
typedef basic_string<char>	string;

//-----------------------------------------------------------------------------
template<typename E, class TStorage, class TAllocator>
bool operator==(const basic_string<E, TStorage, TAllocator>& lhs, 
				const basic_string<E, TStorage, TAllocator>& rhs)
{
	return lhs.compare(rhs) == 0;
}

//-----------------------------------------------------------------------------
template<typename E, class TStorage, class TAllocator>
bool operator!=(const basic_string<E, TStorage, TAllocator>& lhs, 
				const basic_string<E, TStorage, TAllocator>& rhs)
{
	return !(lhs == rhs);
}

//-----------------------------------------------------------------------------
template<typename E, class TStorage, class TAllocator>
bool operator<(const basic_string<E, TStorage, TAllocator>& lhs, 
				const basic_string<E, TStorage, TAllocator>& rhs)
{
	return lhs.compare(rhs) < 0;
}

//-----------------------------------------------------------------------------
template<typename E, class TStorage, class TAllocator>
bool operator>(const basic_string<E, TStorage, TAllocator>& lhs, 
				const basic_string<E, TStorage, TAllocator>& rhs)
{
	return lhs.compare(rhs) > 0;
}

}  // namespace rdestl

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_BASIC_STRING_H
