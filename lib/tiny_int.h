/// \file tiny_int.h
/// \brief Declares class for holding a SQL tiny_int

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

#ifndef MYSQLPP_TINY_INT_H
#define MYSQLPP_TINY_INT_H

namespace mysqlpp {

/// \brief Class for holding an SQL \c tiny_int object.
///
/// This is required because the closest C++ type, \c char, doesn't
/// have all the right semantics.  For one, inserting a \c char into a
/// stream won't give you a number.
///
/// Several of the functions below accept a \c short \c int argument,
/// but internally we store the data as a \c char. Beware of integer
/// overflows!

class tiny_int
{
public:
	/// \brief Default constructor
	///
	/// Value is uninitialized
	tiny_int() { }
	
	/// \brief Create object from any integral type that can be
	/// converted to a \c short \c int.
	tiny_int(short int v) :
	value_(char(v))
	{
	}
	
	/// \brief Return value as a \c short \c int.
	operator short int() const
	{
		return static_cast<short int>(value_);
	}

	/// \brief Assign a \c short \c int to the object.
	tiny_int& operator =(short int v)
	{
		value_ = char(v);
		return *this;
	}

	/// \brief Add another value to this object
	tiny_int& operator +=(short int v)
	{
		value_ += char(v);
		return *this;
	}

	/// \brief Subtract another value to this object
	tiny_int& operator -=(short int v)
	{
		value_ -= char(v);
		return *this;
	}

	/// \brief Multiply this value by another object
	tiny_int& operator *=(short int v)
	{
		value_ *= char(v);
		return *this;
	}

	/// \brief Divide this value by another object
	tiny_int& operator /=(short int v)
	{
		value_ /= char(v);
		return *this;
	}

	/// \brief Divide this value by another object and store the
	/// remainder
	tiny_int& operator %=(short int v)
	{
		value_ %= char(v);
		return *this;
	}

	/// \brief Bitwise AND this value by another value
	tiny_int& operator &=(short int v)
	{
		value_ &= char(v);
		return *this;
	}

	/// \brief Bitwise OR this value by another value
	tiny_int& operator |=(short int v)
	{
		value_ |= char(v);
		return *this;
	}

	/// \brief Bitwise XOR this value by another value
	tiny_int& operator ^=(short int v)
	{
		value_ ^= char(v);
		return *this;
	}

	/// \brief Shift this value left by \c v positions
	tiny_int& operator <<=(short int v)
	{
		value_ <<= char(v);
		return *this;
	}

	/// \brief Shift this value right by \c v positions
	tiny_int& operator >>=(short int v)
	{
		value_ >>= char(v);
		return *this;
	}

	/// \brief Add one to this value and return that value
	tiny_int& operator ++()
	{
		value_++;
		return *this;
	}

	/// \brief Subtract one from this value and return that value
	tiny_int& operator --()
	{
		value_--;
		return *this;
	}

	/// \brief Add one to this value and return the previous value
	tiny_int operator ++(int)
	{
		tiny_int tmp = value_;
		value_++;
		return tmp;
	}

	/// \brief Subtract one from this value and return the previous
	/// value
	tiny_int operator --(int)
	{
		tiny_int tmp = value_;
		value_--;
		return tmp;
	}

	/// \brief Return this value minus \c i
	tiny_int operator -(const tiny_int& i) const
	{
		return value_ - i;
	}
	
	/// \brief Return this value plus \c i
	tiny_int operator +(const tiny_int& i) const
	{
		return value_ + i;
	}
	
	/// \brief Return this value multiplied by \c i
	tiny_int operator *(const tiny_int& i) const
	{
		return value_ * i;
	}
	
	/// \brief Return this value divided by \c i
	tiny_int operator /(const tiny_int& i) const
	{
		return value_ / i;
	}
	
	/// \brief Return the modulus of this value divided by \c i
	tiny_int operator %(const tiny_int& i) const
	{
		return value_ % i;
	}
	
	/// \brief Return this value bitwise OR'd by \c i
	tiny_int operator |(const tiny_int& i) const
	{
		return value_ | i;
	}
	
	/// \brief Return this value bitwise AND'd by \c i
	tiny_int operator &(const tiny_int& i) const
	{
		return value_ & i;
	}
	
	/// \brief Return this value bitwise XOR'd by \c i
	tiny_int operator ^(const tiny_int& i) const
	{
		return value_ ^ i;
	}
	
	/// \brief Return this value bitwise shifted left by \c i
	tiny_int operator <<(const tiny_int& i) const
	{
		return value_ << i;
	}
	
	/// \brief Return this value bitwise shifted right by \c i
	tiny_int operator >>(const tiny_int& i) const
	{
		return value_ >> i;
	}

private:
	char value_;
};

} // end namespace mysqlpp

#endif
