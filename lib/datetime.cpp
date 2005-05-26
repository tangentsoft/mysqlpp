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
#include "platform.h"

#include "datetime.h"

#include <iomanip>

using namespace std;

namespace mysqlpp {

ostream& mysql_date::out_stream(ostream& s) const
{
	char fill = s.fill('0');
	ios::fmtflags flags = s.setf(ios::right);
	s << setw(4) << year << '-' 
			<< setw(2) << month << '-'
			<< setw(2) << day;
	s.flags(flags);
	s.fill(fill);
	return s;
}

ostream& mysql_time::out_stream(ostream& s) const
{
	char fill = s.fill('0');
	ios::fmtflags flags = s.setf(ios::right);
	s << setw(2) << hour << ':' 
			<< setw(2) << minute << ':'
			<< setw(2) << second;
	s.flags(flags);
	s.fill(fill);
	return s;
}

ostream& DateTime::out_stream (ostream& s) const
{
	mysql_date::out_stream(s);
	s << " ";
	mysql_time::out_stream(s);
	return s;
}

cchar* mysql_date::convert(cchar* str)
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

cchar* mysql_time::convert(cchar* str)
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
	str = mysql_date::convert(str);
	if (*str == ' ') str++;
	str = mysql_time::convert(str);
	return str;
}

short int mysql_date::compare(const mysql_date* other) const
{
	if (year != other->year) return year - other->year;
	if (month != other->month) return month - other->month;
	return day - other->day;
}

short int mysql_time::compare(const mysql_time* other) const
{
	if (hour != other->hour) return hour - other->hour;
	if (minute != other->minute) return minute - other->minute;
	return second - other->second;
}

short int DateTime::compare(const DateTime &other) const
{
	int x;
	x = mysql_date::compare(&other);
	if (x) return x;
	return mysql_time::compare(&other);
}

} // end namespace mysqlpp

