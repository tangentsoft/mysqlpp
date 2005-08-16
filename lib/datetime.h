/// \file datetime.h
/// \brief Declares classes to add MySQL-compatible date and time
/// types to C++'s type system.

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

#ifndef MYSQLPP_DATETIME_H
#define MYSQLPP_DATETIME_H

#include "defs.h"

#include "coldata.h"
#include "stream2string.h"
#include "tiny_int.h"

#include <string>
#include <sstream>
#include <iostream>

namespace mysqlpp {

/// \brief Base class template for MySQL++ date and time classes.
///
/// This template primarily defines the comparison operators, which are
/// all implemented in terms of compare(). Each subclass implements that
/// as a protected method, because these operators are the only
/// supported comparison method.
///
/// This template also defines interfaces for converting the object to
/// a string form, which a subclass must define.
template <class T> struct DTbase
{
	/// \brief Destroy object
	virtual ~DTbase() { }

	/// \brief Return a copy of the item in C++ string form
	operator std::string() const
	{
		return stream2string<std::string>(*this);
	}

	/// \brief Compare this object to another of the same type
	///
	/// Returns < 0 if this object is "before" the other, 0 of they are
	/// equal, and > 0 if this object is "after" the other.
	MYSQLPP_EXPORT virtual short compare(const T& other) const = 0;

	/// \brief Returns true if "other" is equal to this object
	bool operator ==(const T& other) const
	{
		return !compare(other);
	}

	/// \brief Returns true if "other" is not equal to this object
	bool operator !=(const T& other) const
	{
		return compare(other);
	}

	/// \brief Returns true if "other" is less than this object
	bool operator <(const T& other) const
	{
		return compare(other) < 0;
	}

	/// \brief Returns true if "other" is less than or equal to this object
	bool operator <=(const T& other) const
	{
		return compare(other) <= 0;
	}

	/// \brief Returns true if "other" is greater than this object
	bool operator >(const T& other) const
	{
		return compare(other) > 0;
	}

	/// \brief Returns true if "other" is greater than or equal to this object
	bool operator >=(const T& other) const
	{
		return compare(other) >= 0;
	}
};


/// \brief C++ form of MySQL's DATETIME type.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL DATETIME strings.
struct DateTime : public DTbase<DateTime>
{
	/// \brief the year
	///
	/// No surprises; the year 2005 is stored as the integer 2005.
	short int year;

	/// \brief the month, 1-12
	tiny_int month;

	/// \brief the day, 1-31
	tiny_int day;

	/// \brief hour, 0-23
	tiny_int hour;

	/// \brief minute, 0-59
	tiny_int minute;
	
	/// \brief second, 0-59
	tiny_int second;

	/// \brief Default constructor
	DateTime() :
	DTbase<DateTime>(),
	year(0),
	month(0),
	day(0),
	hour(0),
	minute(0),
	second(0)
	{
	}
	
	/// \brief Initialize object as a copy of another Date
	DateTime(const DateTime& other) :
	DTbase<DateTime>(),
	year(other.year),
	month(other.month),
	day(other.day),
	hour(other.hour),
	minute(other.minute),
	second(other.second)
	{
	}

	/// \brief Initialize object from a MySQL date-and-time string
	///
	/// String must be in the HH:MM:SS format.  It doesn't have to be
	/// zero-padded.
	DateTime(cchar* str) { convert(str); }
	
	/// \brief Initialize object from a MySQL date-and-time string
	///
	/// \sa DateTime(cchar*)
	DateTime(const ColData& str)
	{
		convert(str.c_str());
	}

	/// \brief Initialize object from a MySQL date-and-time string
	///
	/// \sa DateTime(cchar*)
	DateTime(const std::string& str)
	{
		convert(str.c_str());
	}

	/// \brief Compare this datetime to another.
	///
	/// Returns < 0 if this datetime is before the other, 0 of they are
	/// equal, and > 0 if this datetime is after the other.
	///
	/// This method is protected because it is merely the engine used
	/// by the various operators in DTbase.
	MYSQLPP_EXPORT short compare(const DateTime& other) const;

	/// \brief Parse a MySQL date and time string into this object.
	MYSQLPP_EXPORT cchar* convert(cchar*);
};


/// \brief Inserts a DateTime object into a C++ stream in a
/// MySQL-compatible format.
///
/// The date and time are inserted into the stream, in that order,
/// with a space between them.
///
/// \param os stream to insert date and time into
/// \param dt date/time object to insert into stream
MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& os,
		const DateTime& dt);


/// \brief C++ form of MySQL's DATE type.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL DATE strings.
struct Date : public DTbase<Date>
{
	/// \brief the year
	///
	/// No surprises; the year 2005 is stored as the integer 2005.
	short int year;

	/// \brief the month, 1-12
	tiny_int month;

	/// \brief the day, 1-31
	tiny_int day;

	/// \brief Default constructor
	Date() : year(0), month(0), day(0) { }

	/// \brief Initialize object
	Date(short int y, tiny_int m, tiny_int d) :
	DTbase<Date>(),
	year(y),
	month(m),
	day(d)
	{
	}
	
	/// \brief Initialize object as a copy of another Date
	Date(const Date& other) :
	DTbase<Date>(),
	year(other.year),
	month(other.month),
	day(other.day)
	{
	}

	/// \brief Initialize object from date part of date/time object
	Date(const DateTime& other) :
	DTbase<Date>(),
	year(other.year),
	month(other.month),
	day(other.day)
	{
	}

	/// \brief Initialize object from a MySQL date string
	///
	/// String must be in the YYYY-MM-DD format.  It doesn't have to be
	/// zero-padded.
	Date(cchar* str) { convert(str); }
	
	/// \brief Initialize object from a MySQL date string
	///
	/// \sa Date(cchar*)
	Date(const ColData& str) { convert(str.c_str()); }

	/// \brief Initialize object from a MySQL date string
	///
	/// \sa Date(cchar*)
	Date(const std::string& str)
	{
		convert(str.c_str());
	}

	/// \brief Compare this date to another.
	///
	/// Returns < 0 if this date is before the other, 0 of they are
	/// equal, and > 0 if this date is after the other.
	MYSQLPP_EXPORT short int compare(const Date& other) const;

	/// \brief Parse a MySQL date string into this object.
	MYSQLPP_EXPORT cchar* convert(cchar*);
};

/// \brief Inserts a Date object into a C++ stream
///
/// The format is YYYY-MM-DD, zero-padded.
///
/// \param os stream to insert date into
/// \param d date to insert into stream
MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& os,
		const Date& d);


/// \brief C++ form of MySQL's TIME type.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL TIME strings.
struct Time : public DTbase<Time>
{
	/// \brief hour, 0-23
	tiny_int hour;

	/// \brief minute, 0-59
	tiny_int minute;
	
	/// \brief second, 0-59
	tiny_int second;

	/// \brief Default constructor
	Time() : hour(0), minute(0), second(0) { }

	/// \brief Initialize object
	Time(tiny_int h, tiny_int m, tiny_int s) :
	hour(h),
	minute(m),
	second(s)
	{
	}

	/// \brief Initialize object as a copy of another Time
	Time(const Time& other) :
	DTbase<Time>(),
	hour(other.hour),
	minute(other.minute),
	second(other.second)
	{
	}

	/// \brief Initialize object from time part of date/time object
	Time(const DateTime& other) :
	DTbase<Time>(),
	hour(other.hour),
	minute(other.minute),
	second(other.second)
	{
	}

	/// \brief Initialize object from a MySQL time string
	///
	/// String must be in the HH:MM:SS format.  It doesn't have to be
	/// zero-padded.
	Time(cchar* str) { convert(str); }

	/// \brief Initialize object from a MySQL time string
	///
	/// \sa Time(cchar*)
	Time(const ColData& str) { convert(str.c_str()); }

	/// \brief Initialize object from a MySQL time string
	///
	/// \sa Time(cchar*)
	Time(const std::string& str)
	{
		convert(str.c_str());
	}

	/// \brief Parse a MySQL time string into this object.
	MYSQLPP_EXPORT cchar* convert(cchar*);

	/// \brief Compare this time to another.
	///
	/// Returns < 0 if this time is before the other, 0 of they are
	/// equal, and > 0 if this time is after the other.
	MYSQLPP_EXPORT short int compare(const Time& other) const;
};

/// \brief Inserts a Time object into a C++ stream in a MySQL-compatible
/// format.
///
/// The format is HH:MM:SS, zero-padded.
///
/// \param os stream to insert time into
/// \param t time to insert into stream
MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& os,
		const Time& t);


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_DATETIME_H)
