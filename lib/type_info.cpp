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
#include "sql_types.h"
#include "type_info.h"

#include <mysql.h>

#include <string>

using namespace std;

namespace mysqlpp {

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
	sql_type_info("DECIMAL NOT NULL", typeid(sql_decimal), 0),
	sql_type_info("TINYINT NOT NULL", typeid(sql_tinyint), 1, true),
	sql_type_info("SMALLINT NOT NULL", typeid(sql_smallint), 2, true),
	sql_type_info("INT NOT NULL", typeid(sql_int), 3, true),
	sql_type_info("FLOAT NOT NULL", typeid(sql_float), 4, true),
	sql_type_info("DOUBLE NOT NULL", typeid(sql_double), 5, true),
	sql_type_info("NULL NOT NULL", typeid(void), 6),
	sql_type_info("TIMESTAMP NOT NULL", typeid(sql_timestamp), 7),
	sql_type_info("BIGINT NOT NULL", typeid(sql_bigint), 8, true),
	sql_type_info("MEDIUMINT NOT NULL", typeid(sql_mediumint), 9),
	sql_type_info("DATE NOT NULL", typeid(sql_date), 10, true),
	sql_type_info("TIME NOT NULL", typeid(sql_time), 11, true),
	sql_type_info("DATETIME NOT NULL", typeid(sql_datetime), 12, true),
	sql_type_info("ENUM NOT NULL", typeid(sql_enum), 13, true),
	sql_type_info("SET NOT NULL", typeid(sql_set), 14, true),
	sql_type_info("TINYBLOB NOT NULL", typeid(sql_tinyblob), 15),
	sql_type_info("MEDIUMBLOB NOT NULL", typeid(sql_mediumblob), 16),
	sql_type_info("LONGBLOB NOT NULL", typeid(sql_longblob), 17),
	sql_type_info("BLOB NOT NULL", typeid(sql_blob), 18),
	sql_type_info("VARCHAR NOT NULL", typeid(sql_varchar), 19, true),
	sql_type_info("CHAR NOT NULL", typeid(sql_char), 20),
	sql_type_info("CHAR NOT NULL", typeid(sql_char), 21),
	sql_type_info("TINYINT UNSIGNED NOT NULL", typeid(sql_tinyint_unsigned), 22, true),
	sql_type_info("SMALLINT UNSIGNED NOT NULL", typeid(sql_smallint_unsigned), 23, true),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(sql_int_unsigned), 24),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(sql_int_unsigned), 25),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(sql_int_unsigned), 26),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(sql_int_unsigned), 27),
	sql_type_info("INT UNSIGNED NOT NULL", typeid(sql_int_unsigned), 28, true),
	sql_type_info("BIGINT UNSIGNED NOT NULL", typeid(sql_bigint_unsigned), 29, true),
	sql_type_info("MEDIUMINT UNSIGNED NOT NULL", typeid(sql_mediumint_unsigned), 30),

	sql_type_info("DECIMAL NULL", typeid(Null<sql_decimal>), 0),
	sql_type_info("TINYINT NULL", typeid(Null<sql_tinyint>), 1, true),
	sql_type_info("SMALLINT NULL", typeid(Null<sql_smallint>), 2, true),
	sql_type_info("INT NULL", typeid(Null<sql_int>), 3, true),
	sql_type_info("FLOAT NULL", typeid(Null<sql_float>), 4, true),
	sql_type_info("DOUBLE NULL", typeid(Null<sql_double>), 5, true),
	sql_type_info("NULL NULL", typeid(Null<void>), 6),
	sql_type_info("TIMESTAMP NULL", typeid(Null<sql_timestamp>), 7),
	sql_type_info("BIGINT NULL", typeid(Null<sql_bigint>), 8, true),
	sql_type_info("MEDIUMINT NULL", typeid(Null<sql_mediumint>), 9),
	sql_type_info("DATE NULL", typeid(Null<sql_date>), 10, true),
	sql_type_info("TIME NULL", typeid(Null<sql_time>), 11, true),
	sql_type_info("DATETIME NULL", typeid(Null<sql_datetime>), 12, true),
	sql_type_info("ENUM NULL", typeid(Null<sql_enum>), 13, true),
	sql_type_info("SET NULL", typeid(Null<sql_set>), 14, true),
	sql_type_info("TINYBLOB NULL", typeid(Null<sql_tinyblob>), 15),
	sql_type_info("MEDIUMBLOB NULL", typeid(Null<sql_mediumblob>), 16),
	sql_type_info("LONGBLOB NULL", typeid(Null<sql_longblob>), 17),
	sql_type_info("BLOB NULL", typeid(Null<sql_blob>), 18),
	sql_type_info("VARCHAR NULL", typeid(Null<sql_varchar>), 19, true),
	sql_type_info("CHAR NULL", typeid(Null<sql_char>), 20),
	sql_type_info("CHAR NULL", typeid(Null<sql_char>), 21),
	sql_type_info("TINYINT UNSIGNED NULL", typeid(Null<sql_tinyint_unsigned>), 22, true),
	sql_type_info("SMALLINT UNSIGNED NULL", typeid(Null<sql_smallint_unsigned>), 23, true),
	sql_type_info("INT UNSIGNED NULL", typeid(Null<sql_int_unsigned>), 24),
	sql_type_info("INT UNSIGNED NULL", typeid(Null<sql_int_unsigned>), 25),
	sql_type_info("INT UNSIGNED NULL", typeid(Null<sql_int_unsigned>), 26),
	sql_type_info("INT UNSIGNED NULL", typeid(Null<sql_int_unsigned>), 27),
	sql_type_info("INT UNSIGNED NULL", typeid(Null<sql_int_unsigned>), 28, true),
	sql_type_info("BIGINT UNSIGNED NULL", typeid(Null<sql_bigint_unsigned>), 29, true),
	sql_type_info("MEDIUMINT UNSIGNED NULL", typeid(Null<sql_mediumint_unsigned>), 30),
};

const mysql_type_info::sql_type_info_lookup
		mysql_type_info::lookups(mysql_type_info::types, 62);

/// \if INTERNAL
// Doxygen will not generate documentation for this section.

mysql_ti_sql_type_info_lookup::mysql_ti_sql_type_info_lookup(
		const sql_type_info types[], const int size)
{
	for (int i = 0; i != size; i++) {
		if (types[i].default_) {
			map_[types[i].c_type_] = i;
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
	const type_info& ti = base_type().c_type();
	return ti == typeid(string) ||
			ti == typeid(sql_date) ||
			ti == typeid(sql_time) ||
			ti == typeid(sql_datetime) ||
			ti == typeid(sql_set);
}

bool mysql_type_info::escape_q() const
{
	const type_info& ti = c_type();
	return ti == typeid(string) ||
			ti == typeid(sql_enum) ||
			ti == typeid(sql_blob) ||
			ti == typeid(sql_tinyblob) ||
			ti == typeid(sql_mediumblob) ||
			ti == typeid(sql_longblob) ||
			ti == typeid(sql_char) ||
			ti == typeid(sql_varchar);
}

} // end namespace mysqlpp

