/// \file defs.h
/// \brief Standard definitions used all across the library,
/// particularly things that don't fit well anywhere else.

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

#if !defined(MYSQLPP_DEFS_H)
#define MYSQLPP_DEFS_H

#include "platform.h"

#include <mysql.h>

namespace mysqlpp {

/// \brief Alias for 'true', to make code requesting exceptions more
/// readable.
const bool use_exceptions = true;

/// \brief Used to disambiguate overloads of equal_list() in SSQLSes.
enum sql_cmp_type {sql_use_compare};

#if !defined(DOXYGEN_IGNORE)
// Figure out how to get large integer support on this system.  Suppress
// refman documentation for these typedefs, as they're system-dependent.
#if defined(NO_LONG_LONGS)
// Alias "longlong" and "ulonglong" to the regular "long" counterparts
typedef unsigned long ulonglong;
typedef long longlong;
#elif defined(_MSC_VER)
// It's VC++, so we'll use Microsoft's 64-bit integer types
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
// No better idea, so assume the C99 convention.  If your compiler
// doesn't support this, please provide a patch to extend this ifdef, or
// define NO_LONG_LONGS.
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif
#endif // !defined(DOXYGEN_IGNORE)

/// \brief Alias for MYSQL_FIELD
typedef MYSQL_FIELD Field;
/// \brief Contraction for 'const char*'
typedef const char cchar;
#ifndef uint
/// \brief Contraction for 'unsigned int'
typedef unsigned int uint;
#endif

} // end namespace mysqlpp

#endif

