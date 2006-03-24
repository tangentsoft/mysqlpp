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

#include "stock.h"
#include "util.h"

#include <iostream>
#include <vector>

using namespace std;

int
main(int argc, char *argv[])
{
	try {
		mysqlpp::Connection con(mysqlpp::use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		// Get all the rows in the stock table.
		mysqlpp::Query query = con.query();
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
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
		return -1;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}
