/// \file const_string.h
/// \brief Declares a wrapper for <tt>const char*</tt> which behaves
/// in a way more useful to MySQL++.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#ifndef MYSQLPP_CONST_STRING_H
#define MYSQLPP_CONST_STRING_H

#include "common.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

namespace mysqlpp {

/// \brief Wrapper for <tt>const char*</tt> to make it behave in a
/// way more useful to MySQL++.
///
/// This class implements a small subset of the standard string class.
///
/// As of MySQL++ 2.3, it makes a copy of the string we are initialized
/// with, instead of just copying the pointer.  This is required to
/// avoid problems with the new SSQLS + BLOB support.
class MYSQLPP_EXPORT const_string
{
public:
	/// \brief Type of the data stored in this object, when it is not
	/// equal to SQL null.
	typedef const char value_type;

	/// \brief Type of "size" integers
	typedef unsigned int size_type;

	/// \brief Type used when returning a reference to a character in
	/// the string.
	typedef const char& const_reference;

	/// \brief Type of iterators
	typedef const char* const_iterator;

	/// \brief Same as const_iterator because the data cannot be
	/// changed.
	typedef const_iterator iterator;

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	typedef int difference_type;
	typedef const_reference reference;
	typedef const char* const_pointer;
	typedef const_pointer pointer;
#endif // !defined(DOXYGEN_IGNORE)

	/// \brief Create empty string
	const_string() :
	str_data_(0),
	length_(0)
	{
	}
	
	/// \brief Initialize string from existing C++ string
	const_string(const std::string& str) :
	str_data_(0),
	length_(str.length())
	{
		str_data_ = new char[length_ + 1];
		memcpy(str_data_, str.data(), length_);
		str_data_[length_] = '\0';
	}
	
	/// \brief Initialize string from existing C string
	const_string(const char* str) :
	str_data_(0),
	length_(size_type(strlen(str)))
	{
		str_data_ = new char[length_ + 1];
		memcpy(str_data_, str, length_);
		str_data_[length_] = '\0';
	}
	
	/// \brief Initialize string from existing C string of known length
	const_string(const char* str, size_type len) :
	str_data_(0),
	length_(size_type(len))
	{
		str_data_ = new char[length_ + 1];
		memcpy(str_data_, str, length_);
		str_data_[length_] = '\0';
	}

	/// \brief Destroy string
	~const_string()
	{
		delete[] str_data_;
	}
	
	/// \brief Assignment operator, from C string
	const_string& operator=(const char* str)
	{
		delete[] str_data_;
		length_ = size_type(strlen(str));
		str_data_ = new char[length_];
		memcpy(str_data_, str, length_);
		return *this;
	}

	/// \brief Assignment operator, from other const_string
	const_string& operator=(const const_string& cs)
	{
		delete[] str_data_;
		length_ = cs.length_;
		str_data_ = new char[length_];
		memcpy(str_data_, cs.str_data_, length_);
		return *this;
	}

	/// \brief Return number of characters in the string
	size_type length() const { return length_; }

	/// \brief Return number of characters in string
	size_type size() const { return length_; }

	/// \brief Return iterator pointing to the first character of
	/// the string
	const_iterator begin() const { return str_data_; }
	
	/// \brief Return iterator pointing to one past the last character
	/// of the string.
	const_iterator end() const { return str_data_ + size(); }
	
	/// \brief Return the maximum number of characters in the string.
	///
	/// Because this is a \c const string, this is just an alias for
	/// size(); its size is always equal to the amount of data currently
	/// stored.
	size_type max_size() const { return size(); }
	
	/// \brief Return a reference to a character within the string.
	const_reference operator [](size_type pos) const
			{ return str_data_[pos]; }
	
	/// \brief Return a reference to a character within the string.
	///
	/// Unlike \c operator[](), this function throws an 
	/// \c std::out_of_range exception if the index isn't within range.
	const_reference at(size_type pos) const
	{
		if (pos >= size())
			throw std::out_of_range("");
		else
			return str_data_[pos];
	}
	
	/// \brief Return a const pointer to the string data.  Not
	/// necessarily null-terminated!
	const char* c_str() const { return str_data_; }
	
	/// \brief Alias for \c c_str()
	const char* data() const { return str_data_; }
	
	/// \brief Lexically compare this string to another.
	///
	/// \param str string to compare against this one
	///
	/// \retval <0 if str1 is lexically "less than" str2
	/// \retval 0 if str1 is equal to str2
	/// \retval >0 if str1 is lexically "greater than" str2
	int compare(const const_string& str) const
	{
		size_type i = 0, short_len = std::min(length(), str.length());
		while ((i < short_len) && (str_data_[i] != str.str_data_[i])) {
			++i;
		}
		return str_data_[i] - str.str_data_[i];
	}

private:
	char* str_data_;
	size_type length_;
};


/// \brief Inserts a const_string into a C++ stream
inline std::ostream& operator <<(std::ostream& o,
		const const_string& str)
{
	return o << str.c_str();
}

/// \brief Calls lhs.compare(), passing rhs
inline int compare(const const_string& lhs, const const_string& rhs)
{
	return lhs.compare(rhs);
}

/// \brief Returns true if lhs is the same as rhs
inline bool operator ==(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) == 0;
}

/// \brief Returns true if lhs is not the same as rhs
inline bool operator !=(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) != 0;
}

/// \brief Returns true if lhs is lexically less than rhs
inline bool operator <(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) < 0;
}

/// \brief Returns true if lhs is lexically less or equal to rhs
inline bool operator <=(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) <= 0;
}

/// \brief Returns true if lhs is lexically greater than rhs
inline bool operator >(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) > 0;
}

/// \brief Returns true if lhs is lexically greater than or equal to rhs
inline bool operator >=(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) >= 0;
}

} // end namespace mysqlpp

#endif
