/***********************************************************************
 simple1.cpp - Example showing the simplest way to get data from a MySQL
    database with MySQL++.

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

int
main(int argc, char *argv[])
{
	// Connect to the sample database.
	mysqlpp::Connection con(false);
	if (!connect_to_db(argc, argv, con)) {
		return 1;
	}

	// Retrieve one row from the sample stock table set up by resetdb
	mysqlpp::Query query = con.query();
	query << "select * from stock";
	mysqlpp::Result res = query.store();

	// Display results
	if (res) {
		// Display header
		cout.setf(ios::left);
		cout << setw(21) << "Item" <<
				setw(10) << "Num" <<
				setw(10) << "Weight" <<
				setw(10) << "Price" <<
				"Date" << endl << endl;

		// Get each row in result set, and print its contents
		mysqlpp::Row row;
		mysqlpp::Row::size_type i;
		for (i = 0; row = res.at(i); ++i) {
			cout << setw(20) << row["item"] << ' ' <<
					setw(9) << row["num"] << ' ' <<
					setw(9) << row["weight"] << ' ' <<
					setw(9) << row["price"] << ' ' <<
					setw(9) << row["sdate"] <<
					endl;
		}
	}
	else {
		cerr << "Failed to get stock item: " << query.error() << endl;
		return 1;
	}

	return 0;
}
