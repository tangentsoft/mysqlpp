/// \file sql_types.h
/// \brief Declares the closest C++ equivalent of each MySQL column type

/***********************************************************************
 Copyright (c) 2006 by Educational Technology Resources, Inc.  Others
 may also hold copyrights on code in this file.  See the CREDITS file in
 the top directory of the distribution for details.

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

#if !defined(MYSQLPP_SQL_TYPES_H)
#define MYSQLPP_SQL_TYPES_H

#include "defs.h"
#include "myset.h"

#include <string>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

// Nearest C++ equivalents of MySQL data types.  These are only the "NOT
// NULL" variants.  Wrap these types in MySQL++'s Null<> template to get
// NULL-able types.
typedef signed char		sql_tinyint;
typedef unsigned char	sql_tinyint_unsigned;
typedef short			sql_smallint;
typedef unsigned short	sql_smallint_unsigned;
typedef int				sql_int;
typedef unsigned int 	sql_int_unsigned;
typedef int				sql_mediumint;
typedef unsigned int 	sql_mediumint_unsigned;
typedef longlong		sql_bigint;
typedef ulonglong		sql_bigint_unsigned;

typedef float			sql_float;
typedef double			sql_double;
typedef double			sql_decimal;

typedef Date			sql_date;
typedef Time			sql_time;
typedef Time			sql_timestamp;
typedef DateTime		sql_datetime;

typedef std::string		sql_enum;

typedef Set<std::string> sql_set;

typedef std::string		sql_blob;
typedef std::string		sql_tinyblob;
typedef std::string		sql_mediumblob;
typedef std::string		sql_longblob;

typedef std::string		sql_char;
typedef std::string		sql_varchar;

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_SQL_TYPES_H)

