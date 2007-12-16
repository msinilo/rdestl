#ifndef RDESTL_STRING_H
#define RDESTL_STRING_H

#include "rdestl/allocator.h"
#include "rdestl/rdestl.h"

namespace rdestl
{
//=============================================================================
struct string_rep
{
	// @todo: thread-safe version.
	void AddRef() { ++refs; }
	bool Release() { --refs; return refs <= 0; }

	long	refs;
	int		size;
	int		capacity;
	// [data]
};
typedef char ERR_InvalidStringRepSize[sizeof(string_rep) == 12 ? 1 : -1];

//-----------------------------------------------------------------------------
template<typename T> int strlen(const T* str)
{
	int len(0);
	while (*str++)
		++len;
	return len;
}

//-----------------------------------------------------------------------------
template<typename T> int strcompare(const T* s1, const T* s2, int len)
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
// @note: this one is totally _not_ std::string compatible for the time being!
// one way conversion should work, ie RDESTL --> STL.
template<typename E, class TAllocator = rdestl::allocator>
class string
{
public:
	typedef E				value_type;
	typedef size_t			size_type;
	typedef TAllocator		allocator_type;
	static const size_type	kGranularity = 32;	

	explicit string(const allocator_type& allocator = allocator_type())
	:	m_allocator(allocator)
	{
		init_string(0);
	}
	// yeah, EXPLICIT.
	explicit string(const value_type* str, const allocator_type& allocator = allocator_type())
	:	m_allocator(allocator)
	{
		const int len = strlen(str);
		init_string(len);
		memcpy(m_data, str, len*sizeof(value_type));
		get_rep()->size = len;
		m_data[len] = 0;
	}
	string(const value_type* str, size_type len, 
		const allocator_type& allocator = allocator_type())
	:	m_allocator(allocator)
	{
		init_string(len);
		memcpy(m_data, str, len*sizeof(value_type));
		get_rep()->size = len;
		m_data[len] = 0;
	}
	string(const string& str, const allocator_type& allocator = allocator_type())
	:	m_data(str.m_data),
		m_allocator(allocator)
	{
		get_rep()->AddRef();
	}
	~string()
	{
		release_string();
	}

	// No operator returning ref for the time being. It's dangerous with COW.
	value_type operator[](size_type i) const
	{
		RDE_ASSERT(i < length());
		RDE_ASSERT(invariant());
		return m_data[i];
	}

	string& operator=(const string& rhs)
	{
		if (m_data != rhs.m_data)
		{
			release_string();
			m_data = rhs.m_data;
			get_rep()->AddRef();
		}
		return *this;
	}
	string& operator=(const value_type* str)
	{
		return assign(str);
	}

	string& assign(const value_type* str, size_type len)
	{
		// Do not use with str = str2.c_str()!
		RDE_ASSERT(str != m_data);
		release_string();
		init_string(len);
		memcpy(m_data, str, len*sizeof(value_type));
		get_rep()->size = len;
		m_data[len] = 0;
		return *this;
	}
	string& assign(const value_type* str)
	{
		return assign(str, strlen(str));
	}

	string substr(size_type begin, size_type end = length()) const
	{
		RDE_ASSERT(end >= begin && end <= length() && begin >= 0);
		return string(m_data + begin, end - begin);
	}

	int compare(const value_type* str) const
	{
		const size_type thisLen = length();
		const size_type strLen = strlen(str);
		if (thisLen < strLen)
			return -1;
		if (thisLen > strLen)
			return 1;

		return strcompare(m_data, str, thisLen);
	}
	int compare(const string& rhs) const
	{
		// COW optimization.
		if (m_data == rhs.m_data)
			return 0;
		const size_type thisLen = length();
		const size_type rhsLen = rhs.length();
		if (thisLen < rhsLen)
			return -1;
		if (thisLen > rhsLen)
			return 1;

		return strcompare(m_data, rhs.m_data, thisLen);
	}

	size_type length() const
	{
		return get_rep()->size;
	}
	bool empty() const	{ return length() == 0; }

private:
	bool invariant() const
	{
		RDE_ASSERT(m_data);
		string_rep* rep = get_rep();
		RDE_ASSERT(rep->refs >= 1);
		RDE_ASSERT(rep->size <= rep->capacity);
		if (!empty())
			RDE_ASSERT(m_data[length()] == 0);
		return true;
	}
	string_rep* get_rep() const
	{
		return reinterpret_cast<string_rep*>(m_data) - 1;
	}
	void init_string(int capacity)
	{
		if (capacity != 0)
		{
			capacity = (capacity+kGranularity-1) & ~(kGranularity-1);
			if (capacity < kGranularity)
				capacity = kGranularity;

			const int toAlloc = sizeof(string_rep) + sizeof(value_type)*capacity;
			void* mem = m_allocator.allocate(toAlloc);
			string_rep* rep = reinterpret_cast<string_rep*>(mem);
			rep->refs = 1;
			rep->size = 0;
			rep->capacity = capacity;
			m_data = reinterpret_cast<value_type*>(rep + 1);
		}
		else
		{
			string_rep* rep = reinterpret_cast<string_rep*>(m_buffer);
			rep->refs = 1;
			rep->size = 0;
			rep->capacity = 0;
			m_data = reinterpret_cast<value_type*>(rep + 1);
		}
		*m_data = 0;
	}
	void release_string()
	{
		string_rep* rep = get_rep();
		if (rep->Release() && rep->capacity != 0)
			m_allocator.deallocate(rep, rep->capacity);
	}

	value_type*		m_data;
	char			m_buffer[12+1];
	allocator_type	m_allocator;
};

//-----------------------------------------------------------------------------
template<typename E, class TAllocator>
bool operator==(const string<E, TAllocator>& lhs, const string<E, TAllocator>& rhs)
{
	return lhs.compare(rhs) == 0;
}

//-----------------------------------------------------------------------------
template<typename E, class TAllocator>
bool operator!=(const string<E, TAllocator>& lhs, const string<E, TAllocator>& rhs)
{
	return !(lhs == rhs);
}

//-----------------------------------------------------------------------------
template<typename E, class TAllocator>
bool operator<(const string<E, TAllocator>& lhs, const string<E, TAllocator>& rhs)
{
	return lhs.compare(rhs) < 0;
}

//-----------------------------------------------------------------------------
template<typename E, class TAllocator>
bool operator>(const string<E, TAllocator>& lhs, const string<E, TAllocator>& rhs)
{
	return lhs.compare(rhs) > 0;
}

}  // namespace rdestl

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_STRING_H
