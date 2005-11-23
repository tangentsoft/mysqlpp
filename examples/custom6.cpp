/***********************************************************************
 custom6.cpp - Same as custom1, except that it requests only a subset of
 	the stock table.  See also simple1, for another way to accomplish
	the same thing, without using SSQLS.
 
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

// To store a subset of a row, we define an SSQLS containing just the
// fields that we will store.  There are complications here that are
// covered in the user manual.
sql_create_1(stock_subset,
			 1, 0,
			 string, item)

int
main(int argc, char *argv[])
{
	try {						
		// Establish the connection to the database server.
		mysqlpp::Connection con(mysqlpp::use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		// Retrieve a subset of the stock table, and store the data in
		// a vector of 'stock_subset' SSQLS structures.
		mysqlpp::Query query = con.query();
		query << "select item from stock";
		vector<stock_subset> res;
		query.storein(res);

		// Display the result set
		cout << "We have:" << endl;
		vector<stock_subset>::iterator it;
		for (it = res.begin(); it != res.end(); ++it) {
			cout << '\t' << it->item << endl;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions; e.g. type mismatch populating 'stock'
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
