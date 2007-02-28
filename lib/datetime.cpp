/***********************************************************************
 datetime.cpp - Implements date and time classes compatible with MySQL's
	various date and time column types.

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

#define MYSQLPP_NOT_HEADER
#include "common.h"

#include "datetime.h"

#include <iomanip>

#include <time.h>

using namespace std;

namespace mysqlpp {

std::ostream& operator <<(std::ostream& os, const Date& d)
{
	char fill = os.fill('0');
	ios::fmtflags flags = os.setf(ios::right);
	os		<< setw(4) << d.year << '-' 
			<< setw(2) << d.month << '-'
			<< setw(2) << d.day;
	os.flags(flags);
	os.fill(fill);
	return os;
}


std::ostream& operator <<(std::ostream& os, const Time& t)
{
	char fill = os.fill('0');
	ios::fmtflags flags = os.setf(ios::right);
	os		<< setw(2) << t.hour << ':' 
			<< setw(2) << t.minute << ':'
			<< setw(2) << t.second;
	os.flags(flags);
	os.fill(fill);
	return os;
}


std::ostream& operator <<(std::ostream& os, const DateTime& dt)
{
	operator <<(os, Date(dt));
	os << ' ';
	return operator <<(os, Time(dt));
}


cchar* Date::convert(cchar* str)
{
	char num[5];

	num[0] = *str++;
	num[1] = *str++;
	num[2] = *str++;
	num[3] = *str++;
	num[4] = 0;
	year = short(strtol(num, 0, 10));
	if (*str == '-') str++;

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	month = short(strtol(num, 0, 10));
	if (*str == '-') str++;

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	day = short(strtol(num, 0, 10));

	return str;
}


cchar* Time::convert(cchar* str)
{
	char num[5];

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	hour = short(strtol(num,0,10));
	if (*str == ':') str++;

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	minute = short(strtol(num,0,10));
	if (*str == ':') str++;

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	second = short(strtol(num,0,10));

	return str;
}


cchar* DateTime::convert(cchar* str)
{
	Date d;
	str = d.convert(str);
	year = d.year;
	month = d.month;
	day = d.day;
	
	if (*str == ' ') ++str;

	Time t;
	str = t.convert(str);
	hour = t.hour;
	minute = t.minute;
	second = t.second;
	
	return str;
}


short int Date::compare(const Date& other) const
{
	if (year != other.year) return year - other.year;
	if (month != other.month) return month - other.month;
	return day - other.day;
}


short int Time::compare(const Time& other) const
{
	if (hour != other.hour) return hour - other.hour;
	if (minute != other.minute) return minute - other.minute;
	return second - other.second;
}


short int DateTime::compare(const DateTime& other) const
{
	Date d(*this), od(other);
	Time t(*this), ot(other);

	if (int x = d.compare(od)) {
		return x;
	}
	else {
		return t.compare(ot);
	}
}

DateTime::operator time_t() const
{
	struct tm tm;
	tm.tm_sec = second;
	tm.tm_min = minute;
	tm.tm_hour = hour;
	tm.tm_mday = day;
	tm.tm_mon = month - (tiny_int)1;
	tm.tm_year = year - 1900;
	tm.tm_isdst = -1;

	return mktime(&tm);
};

DateTime::DateTime(time_t t)
{
	struct tm tm;
#if defined(_MSC_VER) && _MSC_VER >= 1400 && !defined(_STLP_VERSION) && \
		!defined(_STLP_VERSION_STR)
	// Use thread-safe localtime() replacement included with VS2005 and
	// up, but only when using native RTL, not STLport.
	localtime_s(&tm, &t);
#elif defined(HAVE_LOCALTIME_R)
	// Detected POSIX thread-safe localtime() replacement.
	localtime_r(&t, &tm);
#else
	// No explicitly thread-safe localtime() replacement found.  This
	// may still be thread-safe, as some C libraries take special steps
	// within localtime() to get thread safety.  For example, thread-
	// local storage (TLS) in some Windows compilers.
	memcpy(&tm, localtime(&t), sizeof(tm));
#endif

	year = tm.tm_year + 1900;
	month = tm.tm_mon + 1;
	day = tm.tm_mday;
	hour = tm.tm_hour;
	minute = tm.tm_min;
	second = tm.tm_sec;
}

} // end namespace mysqlpp


