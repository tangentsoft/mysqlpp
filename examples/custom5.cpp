/***********************************************************************
 custom5.cpp - Example showing how to use the equal_list() member of
 	some SSQLS types to build SELECT queries with custom WHERE clauses.
 
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, (c) 2004, 2005 by Educational Technology Resources, Inc., and
 (c) 2005 by Chris Frey.  Others may also hold copyrights on code in
 this file.  See the CREDITS file in the top directory of the
 distribution for details.

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
#include <custom.h>

#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
using namespace mysqlpp;

sql_create_5(stock,
			 1, 5,
			 string, item,
			 longlong, num,
			 double, weight,
			 double, price,
			 Date, sdate)

int
main(int argc, char *argv[])
{
	try {
		Connection con(use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		// Get all the rows in the stock table.
		Query query = con.query();
		query << "select * from stock";
		vector<stock> res;
		query.storein(res);

		if (res.size() > 0) {
			// Build a select query using the data from the first row
			// returned by our previous query.
			query.reset();
			query << "select * from stock where " <<
				res[0].equal_list(" and ", stock_weight, stock_price);

			// Display the finished query.
			cout << "Custom query:\n" << query.preview() << endl;
		}

		return 0;
	}
	catch (BadQuery& er) {
		// Handle any connection or query errors
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
	catch (BadConversion& er) {
		// Handle bad conversions
		cerr << "Error: " << er.what() << "\"." << endl <<
				"retrieved data size: " << er.retrieved <<
				" actual data size: " << er.actual_size << endl;
		return -1;
	}
	catch (exception& er) {
		// Catch-all for any other standard C++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
}

