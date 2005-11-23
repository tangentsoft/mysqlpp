/***********************************************************************
 custom2.cpp - Example showing how to insert a row using the Specialized
 	SQL Structures feature of MySQL++.

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
#include <string>
#include <vector>

using namespace std;

sql_create_5(stock,
			1, 5,
			string, item,
			mysqlpp::longlong, num,
			double, weight,
			double, price,
			mysqlpp::Date, sdate)

int
main(int argc, char *argv[])
{
	try {
		// Establish the connection to the database server.
		mysqlpp::Connection con(mysqlpp::use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		// Create and populate a stock object.  We could also have used
		// the set() member, which takes the same parameters as this
		// constructor.
		stock row("Hot Dogs", 100, 1.5, 1.75, "1998-09-25");

		// Form the query to insert the row into the stock table.
		mysqlpp::Query query = con.query();
		query.insert(row);

		// Show the query about to be executed.
		cout << "Query: " << query.preview() << endl;

		// Execute the query.  We use execute() because INSERT doesn't
		// return a result set.
		query.execute();

		// Print the new table.
		mysqlpp::Result res;
		get_stock_table(query, res);
		print_stock_rows(res);
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
