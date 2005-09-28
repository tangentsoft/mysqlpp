/***********************************************************************
 fieldinf1.cpp - Example showing how to request information about the
 	fields in a table, such as their SQL and C++-equivalent types, as
	MySQL++ sees it.

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

#include "util.h"

#include <mysql++.h>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace mysqlpp;

int
main(int argc, char *argv[])
{
	try {
		Connection con(use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		Query query = con.query();
		query << "select * from stock";
		cout << "Query: " << query.preview() << endl;

		Result res = query.store();
		cout << "Records Found: " << res.size() << endl << endl;

		cout << "Query Info:\n";
		cout.setf(ios::left);

		for (unsigned int i = 0; i < res.names().size(); i++) {
			cout << setw(2) << i
					// this is the name of the field
					<< setw(15) << res.names(i).c_str()
					// this is the SQL identifier name
					// Result::types(unsigned int) returns a mysql_type_info which in many
					// ways is like type_info except that it has additional sql type
					// information in it. (with one of the methods being sql_name())
					<< setw(15) << res.types(i).sql_name()
					// this is the C++ identifier name which most closely resembles
					// the sql name (its is implementation defined and often not very readable)
					<< setw(20) << res.types(i).name()
					<< endl;
		}

		cout << endl;

		if (res.types(0) == typeid(string)) {
			// this is demonstrating how a mysql_type_info can be 
			// compared with a C++ type_info.
			cout << "Field 'item' is of an SQL type which most "
					"closely resembles\nthe C++ string type\n";
		}

		if (res.types(1) == typeid(longlong)) {
			cout << "Field 'num' is of an SQL type which most "
					"closely resembles\nC++ long long int type\n";
		}
		else if (res.types(1).base_type() == typeid(longlong)) {
			// you have to be careful as if it can be null the actual
			// type is Null<TYPE> not TYPE.  So you should always use
			// the base_type method to get at the underlying type.
			// If the type is not null than this base type would be
			// the same as its type.
			cout << "Field 'num' base type is of an SQL type which "
					"most closely\nresembles the C++ long long int type\n";
		}
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
		return -1;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}
