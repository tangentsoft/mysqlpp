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

/// \brief Base class for mysql_date and mysql_time
struct mysql_dt_base
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	virtual std::ostream& out_stream(std::ostream&) const = 0;
	operator std::string() const
	{
		return stream2string<std::string>(*this);
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Base class template for MySQL++ date and time classes.
///
/// This template defines the comparison operators, which are all
/// implemented in terms of compare(). Each subclass implements that
/// as a protected method, because these operators are the only
/// supported comparison method.
template <class T> struct DTbase
{
	/// \brief Compare this object to another of the same type
	///
	/// Returns < 0 if this object is "before" the other, 0 of they are
	/// equal, and > 0 if this object is "after" the other.
	virtual short int compare(const T& other) const = 0;

	/// \brief Returns true if "other" is equal to this object
	bool operator ==(const T& other) const { return !compare(other); }

	/// \brief Returns true if "other" is not equal to this object
	bool operator !=(const T& other) const { return compare(other); }

	/// \brief Returns true if "other" is less than this object
	bool operator <(const T& other) const { return compare(other) < 0; }

	/// \brief Returns true if "other" is less than or equal to this object
	bool operator <=(const T& other) const { return compare(other) <= 0; }

	/// \brief Returns true if "other" is greater than this object
	bool operator >(const T& other) const { return compare(other) > 0; }

	/// \brief Returns true if "other" is greater than or equal to this object
	bool operator >=(const T& other) const { return compare(other) >= 0; }
};


/// \brief Base class of Date
struct mysql_date : virtual public mysql_dt_base
{
	/// \brief the year
	///
	/// No surprises; the year 2005 is stored as the integer 2005.
	short int year;

	/// \brief the month, 1-12
	tiny_int month;

	/// \brief the day, 1-31
	tiny_int day;

	/// \brief Insert the date into a stream
	///
	/// The format is YYYY-MM-DD, zero-padded.
	///
	/// \param os stream to insert date into
	std::ostream& out_stream(std::ostream& os) const;

	/// \brief Parse a MySQL date string into this object.
	cchar* convert(cchar*);

protected:
	/// \brief Compare this date to another.
	///
	/// Returns < 0 if this date is before the other, 0 of they are
	/// equal, and > 0 if this date is after the other.
	///
	/// This method is protected because it is merely the engine used
	/// by the various operators in DTbase.
	short int compare(const mysql_date* other) const;
};


/// \brief Holds MySQL dates.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL DATE strings.

struct Date : public mysql_date, public DTbase<Date>
{
	/// \brief Default constructor
	Date() { };

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
	Date(const std::string& str) { convert(str.c_str()); }

	/// \brief Compare this date to another
	///
	/// \sa mysql_date::compare() for implementation
	short int compare(const Date& other) const
	{
		return mysql_date::compare(&other);
	}
};


/// \brief Inserts a Date object into a C++ stream in a MySQL-compatible
/// format.

inline std::ostream& operator <<(std::ostream& s, const Date& d)
{
	return d.out_stream(s);
}


/// \brief Base class of Time

struct mysql_time : virtual public mysql_dt_base
{
	/// \brief hour, 0-23
	tiny_int hour;

	/// \brief minute, 0-59
	tiny_int minute;
	
	/// \brief second, 0-59
	tiny_int second;

	/// \brief Insert the time into a stream
	///
	/// The format is HH:MM:SS, zero-padded.
	///
	/// \param os stream to insert time into
	std::ostream& out_stream(std::ostream& os) const;

	/// \brief Parse a MySQL time string into this object.
	cchar* convert(cchar*);

protected:
	/// \brief Compare this time to another.
	///
	/// Returns < 0 if this time is before the other, 0 of they are
	/// equal, and > 0 if this time is after the other.
	///
	/// This method is protected because it is merely the engine used
	/// by the various operators in DTbase.
	short int compare(const mysql_time* other) const;
};


/// \brief Holds MySQL times.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL TIME strings.
struct Time : public mysql_time, public DTbase<Time>
{
	/// \brief Default constructor
	Time() { };

	/// \brief Initialize object from a MySQL time string
	///
	/// String must be in the HH:MM:SS format.  It doesn't have to be
	/// zero-padded.
	Time(cchar* str)
	{
		convert(str);
	}

	/// \brief Initialize object from a MySQL time string
	///
	/// \sa Time(cchar*)
	Time(const ColData& str);

	/// \brief Initialize object from a MySQL time string
	///
	/// \sa Time(cchar*)
	Time(const std::string& str);

	/// \brief Compare this time to another
	///
	/// \sa mysql_time::compare() for implementation
	short int compare(const Time& other) const
	{
		return mysql_time::compare(&other);
	}
};


/// \brief Inserts a Time object into a C++ stream in a MySQL-compatible
/// format.

inline std::ostream& operator <<(std::ostream& s, const Time& d)
{
	return d.out_stream(s);
}


/// \brief A combination of the Date and Time classes for holding
/// MySQL DateTimes.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL DATETIME strings.

struct DateTime : public mysql_date, public mysql_time,
		public DTbase<DateTime>
{
	/// \brief Default constructor
	DateTime() { }
	
	/// \brief Initialize object from a MySQL date-and-time string
	///
	/// String must be in the HH:MM:SS format.  It doesn't have to be
	/// zero-padded.
	DateTime(cchar* str)
	{
		convert(str);
	}
	
	/// \brief Initialize object from a MySQL date-and-time string
	///
	/// \sa DateTime(cchar*)
	DateTime(const ColData& str);

	/// \brief Initialize object from a MySQL date-and-time string
	///
	/// \sa DateTime(cchar*)
	DateTime(const std::string& str);

	/// \brief Compare this datetime to another.
	///
	/// Returns < 0 if this datetime is before the other, 0 of they are
	/// equal, and > 0 if this datetime is after the other.
	///
	/// This method is protected because it is merely the engine used
	/// by the various operators in DTbase.
	short int compare(const DateTime& other) const;

	/// \brief Insert the date and time into a stream
	///
	/// The date and time are inserted into the stream, in that order,
	/// with a space between them.
	///
	/// \param os stream to insert date and time into
	std::ostream& out_stream(std::ostream& os) const;

	/// \brief Parse a MySQL date and time string into this object.
	cchar* convert(cchar*);
};


/// \brief Inserts a DateTime object into a C++ stream in a
/// MySQL-compatible format.
inline std::ostream& operator <<(std::ostream& s, const DateTime& d)
{
	return d.out_stream(s);
}


inline Time::Time(const ColData& str)
{
	convert(str.c_str());
}


inline Time::Time(const std::string& str)
{
	convert(str.c_str());
}


inline DateTime::DateTime(const ColData& str)
{
	convert(str.c_str());
}


inline DateTime::DateTime(const std::string& str)
{
	convert(str.c_str());
}

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_DATETIME_H)
