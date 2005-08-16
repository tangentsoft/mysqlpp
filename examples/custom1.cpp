/***********************************************************************
 custom1.cpp - Example that produces the same results as simple1, but it
 	uses a Specialized SQL Structure to store the results instead of a
	MySQL++ Result object.
 
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
#include <custom.h>

#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
using namespace mysqlpp;

// The following is calling a very complex macro which will create
// "struct stock", which has the member variables:
//
//   string item
//   ...
//   Date sdate
//
// plus methods to help populate the class from a MySQL row
// among other things that I'll get to in a later example.
sql_create_5(stock,
			 1, 5,				// explained in the user manual
			 string, item,
			 longlong, num,
			 double, weight,
			 double, price,
			 Date, sdate)

int
main(int argc, char *argv[])
{
	// Wrap all MySQL++ interactions in one big try block, so any
	// errors are handled gracefully.
	try {						
		// Establish the connection to the database server.
		Connection con(use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		// Retrieve the entire contents of the stock table, and store
		// the data in a vector of 'stock' SSQLS structures.
		Query query = con.query();
		query << "select * from stock";
		vector<stock> res;
		query.storein(res);

		// Display the result set
		print_stock_header(res.size());
		vector<stock>::iterator it;
		for (it = res.begin(); it != res.end(); ++it) {
			print_stock_row(it->item, it->num, it->weight, it->price,
					it->sdate);
		}
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const BadConversion& er) {
		// Handle bad conversions; e.g. type mismatch populating 'stock'
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
