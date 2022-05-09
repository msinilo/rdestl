/**
 * by Danushka Abeysuriya aka. 'silvermace'
 * Contact: silvermace@gmail.com
 *
 * Edited 18 May 2010 by silvermace
 * Edited 25 April 2022 by Stelio Kontos
 */

#ifndef RDESTL_STRINGSTREAM_H
#define RDESTL_STRINGSTREAM_H

#include "rdestl_common.h"
#include "rde_string.h"
#include "vector.h"

namespace rde
{

/**
 * this is a super cut down replacement for std::stringstream
 * current limitations:
 * - only converts from rde::string -> arithmetic types
 * - supports bool, short, ushort, int/long, uint/ulong, long long, float, double, and long double
 * - unsigned long long (_ULonglong/unsigned __int64) not currently supported
 * - long double values are treated as doubles
 * - untested unicode support, no support for custom allocator
 * - supports only narrow conversions (8-bit char size)
 */
template<typename E, typename TAlloc = rde::allocator>
struct basic_stringstream
{
	typedef E									value_type;
	typedef rde::vector<value_type, TAlloc>		buffer_type;
	typedef typename buffer_type::size_type		size_type;
	typedef basic_string<value_type, TAlloc>	string_type;

	basic_stringstream() { }
	explicit basic_stringstream(const value_type* inp) { init(inp); }
	explicit basic_stringstream(const string_type& inp) { init(inp.c_str()); }

	bool good() const { return !buffer.empty() ? cursor != buffer.end() : false; }
	bool eof() const { return !good(); }

	explicit operator bool() const { return good(); }

	void reset() { init(nullptr); }
	void reset(const value_type* inp)
	{
		init(inp);
	}

	// Extraction operator>> (input)

	basic_stringstream& operator>>(short& x)
	{
		if (next()) { x = static_cast<short>(atoi(current.c_str())); }
		return *this;
	}
	basic_stringstream& operator>>(unsigned short& x)
	{
		if (next()) { x = static_cast<unsigned short>(atoi(current.c_str())); }
		return *this;
	}
	basic_stringstream& operator>>(int& x)
	{
		if (next()) { x = static_cast<int>(atoll(current.c_str())); }
		return *this;
	}
	basic_stringstream& operator>>(unsigned int& x)
	{
		if (next()) { x = static_cast<unsigned int>(atoll(current.c_str())); }
		return *this;
	}
	basic_stringstream& operator>>(long& x)
	{
		if (next()) { x = static_cast<long>(atoll(current.c_str())); }
		return *this;
	}
	basic_stringstream& operator>>(unsigned long& x)
	{
		if (next()) { x = static_cast<unsigned long>(atoll(current.c_str())); }
		return *this;
	}
	basic_stringstream& operator>>(long long& x)
	{
		if (next()) { x = static_cast<long long>(atoll(current.c_str())); }
		return *this;
	}

	// TODO unsigned long long (_ULonglong)
	basic_stringstream& operator>>(unsigned long long& x)
	{
		static_assert(false == true, "unsigned 64-bit types not supported");
		(void)x;
		return *this;
	}

	basic_stringstream& operator>>(float& x)
	{
		if (next()) { x = static_cast<float>(atof(current.c_str())); }
		return *this;
	}
	basic_stringstream& operator>>(double& x)
	{
		if (next()) { x = static_cast<double>(atof(current.c_str())); }
		return *this;
	}
	basic_stringstream& operator>>(long double& x)
	{
		if (next()) { x = static_cast<double>(atof(current.c_str())); }
		return *this;
	}

	basic_stringstream& operator>>(bool& x)
	{
		if (next()) { x = static_cast<bool>(atoi(current.c_str())); }
		return *this;
	}

	basic_stringstream& operator>>(rde::string& x)
	{
		if (next()) { x = current; }
		return *this;
	}

private:
	//Setup our data buffer and cursor
	void init(const value_type* inp)
	{
		if (!inp || !strlen(inp))
		{
			cursor = buffer.end();
			return;
		}
		const size_type len = strlen(inp);
		buffer.resize(len);
		memmove(buffer.begin(), inp, len);
		cursor = buffer.begin();
		current.clear();
		ltrim();
	}

	bool is_whitespace(const value_type& ch) const
	{
		// TODO whitespace can mean different things based on locale
		return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n');
	}

	//Trim whitespace from the left->right
	void ltrim()
	{
		while (is_whitespace(*cursor))
			++cursor;
	}

	//Read in the next token into current
	bool next()
	{
		if (buffer.empty())
			return false;

		current.clear();
		for (; cursor != buffer.end(); ++cursor)
		{
			if (!is_whitespace(*cursor))
			{
				current.append(*cursor);
			}
			else
			{
				ltrim();
				break;
			}
		}

		return !current.empty();
	}

	string_type current;
	buffer_type buffer;
	typename buffer_type::const_iterator cursor;
};

typedef basic_stringstream<char>	stringstream;

} //namespace rde

#endif // #ifndef RDESTL_STRINGSTREAM_H
