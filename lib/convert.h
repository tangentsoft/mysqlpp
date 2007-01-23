/// \file convert.h
/// \brief Declares various string-to-integer type conversion templates.
///
/// These templates are the mechanism used within mysqlpp::ColData_Tmpl
/// for its string-to-\e something conversions.

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

#ifndef MYSQLPP_CONVERT_H
#define MYSQLPP_CONVERT_H

#include "common.h"

#include <stdlib.h>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

template <class Type> class mysql_convert;

#define mysql__convert(TYPE, FUNC) \
  template <> \
  class mysql_convert<TYPE> {\
  public:\
    mysql_convert(const char* str, const char *& end) { \
      num_ = FUNC(str, const_cast<char **>(&end));}\
    operator TYPE () {return num_;}\
  private:\
    TYPE num_;\
  };\

#if defined(_MSC_VER)
#	pragma warning(disable: 4244)
#endif

	mysql__convert(float, strtod)
	mysql__convert(double, strtod)

#if defined(_MSC_VER)
#	pragma warning(default: 4244)
#endif

#undef mysql__convert
#define mysql__convert(TYPE, FUNC) \
  template <> \
  class mysql_convert<TYPE> {\
  public:\
    mysql_convert(const char* str, const char *& end) { \
      num_ = FUNC(str, const_cast<char **>(&end),10);}\
    operator TYPE () {return num_;}\
  private:\
    TYPE num_;\
  };\

#if defined(_MSC_VER)
#	pragma warning(disable: 4244)
#endif

	mysql__convert(char, strtol)
	mysql__convert(signed char, strtol)
	mysql__convert(int, strtol)
	mysql__convert(short int, strtol)
	mysql__convert(long int, strtol)

	mysql__convert(unsigned char, strtoul)
	mysql__convert(unsigned int, strtoul)
	mysql__convert(unsigned short int, strtoul)
	mysql__convert(unsigned long int, strtoul)

#if defined(_MSC_VER)
#	pragma warning(default: 4244)
#endif

#if !defined(NO_LONG_LONGS)
#if defined(_MSC_VER)
// Handle 64-bit ints the VC++ way
mysql__convert(longlong, _strtoi64)
mysql__convert(ulonglong, _strtoui64)
#else
// No better idea, so assume the C99 way.  If your compiler doesn't
// support this, please provide a patch to extend this ifdef, or define
// NO_LONG_LONGS.
mysql__convert(longlong, strtoll)
mysql__convert(ulonglong, strtoull)
#endif
#endif // !defined(NO_LONG_LONGS)

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif

