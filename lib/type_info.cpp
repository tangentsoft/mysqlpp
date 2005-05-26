/***********************************************************************
 type_info.cpp - Implements the mysql_type_info class.

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

#include "platform.h"

#include "datetime.h"
#include "myset.h"
#include "type_info.h"

#include <mysql.h>

#include <string>

using namespace std;

namespace mysqlpp {

/// \if INTERNAL
// Doxygen will not generate documentation for this section.

typedef string Enum;

/// \endif


// The first half of this array roughly parallels enum_field_types
// in mysql/mysql_com.h.  It is a lookup table used by the type() method
// below when translating from SQL type information to the closest
// C++ equivalent.
//
// The second half of the list parallels the first, to handle null-able
// versions of the types in the first half.  This is required because
// SQL's 'null' concept does not map neatly into the C++ type system, so
// null-able versions of these types have to have a different C++ type,
// implemented using the Null template.  See null.h for further details.
//
// Types marked true (the "default" field) are added to a lookup map in
// the mysql_type_info_lookup class in order to provide reverse lookup
// of C++ types to SQL types.  Put another way, if you take the subset
// of all items marked true, the typeid() of each item must be unique.
const mysql_type_info::sql_type_info mysql_type_info::types[62] = {
	sql_type_info("DECIMAL NOT NULL", typeid(double), 0),
	sql_type_info("TINYINT NOT NULL", typeid(signed char), 1, true),
	sql_type_info("SMALLINT NOT NULL", typeid(short int), 2, true),
	sql_type_info("INT NOT NULL", typeid(int), 3, true),
	sql_type_info("FLOAT NOT NULL", typeid(float), 4, true),
	sql_type_info("DOUBLE NOT NULL", typeid(double), 5, true),
	sql_type_info("NULL NOT NULL", typeid(void), 6),
	sql_type_info("TIMESTAMP NOT NULL", typeid(Time), 7),
	sql_type_info("BIGINT NOT NULL", typeid(longlong), 8, true),
	sql_type_info("MEDIUMINT NOT NULL", typeid(int), 9),
	sql_type_info("DATE NOT NULL", typeid(Date), 10, true),
	sql_type_info("TIME NOT NULL", typeid(Time), 11, true),
	sql_type_info("DATETIME NOT NULL", typeid(DateTime), 12, true),
	sql_type_info("ENUM NOT NULL", typeid(Enum), 13, true),
	sql_type_info("SET NOT NULL", typeid(Set < string >), 14, true),
	sql_type_info("TINYBLOB NOT NULL", typeid(string), 15),
	sql_type_info("MEDIUMBLOB NOT NULL", typeid(string), 16),
	sql_type_info("LONGBLOB NOT NULL", typeid(string), 17),
	sql_type_info("BLOB NOT NULL", typeid(string), 18),
	sql_type_info("VARCHAR NOT NULL", typeid(string), 19, true),
	sql_type_info("CHAR NOT NULL", typeid(string), 20),
	sql_type_info("CHAR NOT NULL", typeid(string), 21),
	sql_type_info("TINYINT UNSIGNED NOT NULL", typeid(unsigned char), 22, true),
	sql_type_info("SMALLINT UNSIGNED NOT NULL", typeid(unsigned short int), 23, true),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 24),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 25),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 26),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 27),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 28, true),
	sql_type_info("BIGINT UNSIGNED NOT NULL", typeid(ulonglong), 29, true),
	sql_type_info("MEDIUMINT UNSIGNED NOT NULL", typeid(unsigned int), 30),

	sql_type_info("DECIMAL NULL", typeid(Null < double >), 0),
	sql_type_info("TINYINT NULL", typeid(Null < signed char >), 1, true),
	sql_type_info("SMALLINT NULL", typeid(Null < short int >), 2, true),
	sql_type_info("INT NULL", typeid(Null < int >), 3, true),
	sql_type_info("FLOAT NULL", typeid(Null < float >), 4, true),
	sql_type_info("DOUBLE NULL", typeid(Null < double >), 5, true),
	sql_type_info("NULL NULL", typeid(Null < void >), 6),
	sql_type_info("TIMESTAMP NULL", typeid(Null < Time >), 7),
	sql_type_info("BIGINT NULL", typeid(Null < longlong >), 8, true),
	sql_type_info("MEDIUMINT NULL", typeid(Null < int >), 9),
	sql_type_info("DATE NULL", typeid(Null < Date >), 10, true),
	sql_type_info("TIME NULL", typeid(Null < Time >), 11, true),
	sql_type_info("DATETIME NULL", typeid(Null < DateTime >), 12, true),
	sql_type_info("ENUM NULL", typeid(Null < Enum >), 13, true),
	sql_type_info("SET NULL", typeid(Null < Set < string > >), 14, true),
	sql_type_info("TINYBLOB NULL", typeid(Null < string >), 15),
	sql_type_info("MEDIUMBLOB NULL", typeid(Null < string >), 16),
	sql_type_info("LONGBLOB NULL", typeid(Null < string >), 17),
	sql_type_info("BLOB NULL", typeid(Null < string >), 18),
	sql_type_info("VARCHAR NULL", typeid(Null < string >), 19, true),
	sql_type_info("CHAR NULL", typeid(Null < string >), 20),
	sql_type_info("CHAR NULL", typeid(Null < string >), 21),
	sql_type_info("TINYINT UNSIGNED NULL", typeid(Null < unsigned char >), 22, true),
	sql_type_info("SMALLINT UNSIGNED NULL", typeid(Null < unsigned short int >), 23, true),
	sql_type_info("INT UNSIGNED NULL", typeid(Null < unsigned int >), 24),
	sql_type_info("INT UNSIGNED NULL", typeid(Null < unsigned int >), 25),
	sql_type_info("INT UNSIGNED NULL", typeid(Null < unsigned int >), 26),
	sql_type_info("INT UNSIGNED NULL", typeid(Null < unsigned int >), 27),
	sql_type_info("INT UNSIGNED NULL", typeid(Null < unsigned int >), 28, true),
	sql_type_info("BIGINT UNSIGNED NULL", typeid(Null < ulonglong >), 29, true),
	sql_type_info("MEDIUMINT UNSIGNED NULL", typeid(Null < unsigned int >), 30),
};

const mysql_type_info::sql_type_info_lookup
		mysql_type_info::lookups(mysql_type_info::types, 62);

/// \if INTERNAL
// Doxygen will not generate documentation for this section.

mysql_ti_sql_type_info_lookup::mysql_ti_sql_type_info_lookup(
		const sql_type_info types[], const int size)
{
	for (int i = 0; i != size; i++) {
		if (types[i]._default) {
			_map[types[i]._c_type] = i;
		}
	}
}

/// \endif

unsigned char mysql_type_info::type(enum_field_types t,
		bool _unsigned, bool _null) 
{
	if (_null) {
		if (_unsigned) {
			return unsigned_null_offset + t;
		}
		else {
			if (t < 200)
				return null_offset + t;
			else
				return null_offset + (t - 234);
		}
	}
	else {
		if (_unsigned) {
			return unsigned_offset + t;
		}
		else {
			if (t < 200)
				return offset + t;
			else
				return offset + (t - 234);
		}
	}
}

bool mysql_type_info::quote_q() const
{
	if (base_type().c_type() == typeid(string) ||
			base_type().c_type() == typeid(Date) ||
			base_type().c_type() == typeid(Time) ||
			base_type().c_type() == typeid(DateTime) ||
			base_type().c_type() == typeid(Enum) ||
			base_type().c_type() == typeid(Set<string>)) {
		return true;
	}
	else {
		return false;
	}
}

bool mysql_type_info::escape_q() const
{
	if (c_type() == typeid(string))
		return true;
	else
		return false;
}

} // end namespace mysqlpp

