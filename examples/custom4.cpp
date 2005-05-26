/***********************************************************************
 custom4.cpp - Example very similar to custom1.cpp, except that it
	stores its result set in an STL set container.  This demonstrates
	how one can manipulate MySQL++ result sets in a very natural C++
	style.

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
#include <string>
#include <vector>

using namespace std;
using namespace mysqlpp;

sql_create_5(stock,
	1,	// This number is used to make a SSQLS less-than-comparable.
		// When comparing two SSQLS structures, the first N elements are
		// compared.  In this instance, we are saying that we only want
		// the first element ('item') to be used when comparing two
		// stock structures.

	5,	// Each SSQLS structure includes a number of constructors.  Some
		// of these are fixed in nature, but one of these will have this
		// number of arguments, one for each of the first N elements in
		// the structure; it is an initialization ctor.  Since N is the
		// same as the number of structure elements in this instance,
		// that ctor will be able to fully initialize the structure. This
		// behavior is not always wanted, however, so the macro allows
		// you make the constructor take fewer parameters, leaving the
		// remaining elements uninitialized.  An example of when this is
		// necessary is when you have a structure containing only two
		// integer elements: one of the other ctors defined for SSQLS
		// structures takes two ints, so the compiler barfs if you pass
		// 2 for this argument.  You would need to pass 0 here to get
		// that SSQLS structure to compile.
	string, item,
	longlong, num,
	double, weight,
	double, price,
	Date, sdate)

int
main(int argc, char *argv[])
{
	try {
		// Establish the connection to the database server.
		Connection con(use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		// Retrieve all rows from the stock table and put them in an
		// STL set.  Notice that this works just as well as storing them
		// in a vector, which we did in custom1.cpp.  It works because
		// SSQLS objects are less-than comparable.
		Query query = con.query();
		query << "select * from stock";
		set<stock> res;
		query.storein(res);

		// Display the result set.  Since it is an STL set and we set up
		// the SSQLS to compare based on the item column, the rows will
		// be sorted by item.
		print_stock_header(res.size());
		set<stock>::iterator it;
		cout.precision(3);
		for (it = res.begin(); it != res.end(); ++it) {
			print_stock_row(it->item.c_str(), it->num, it->weight,
					it->price, it->sdate);
		}

		// Use set's find method to look up a stock item by item name.
		// This also uses the SSQLS comparison setup.
		it = res.find(stock("Hotdog Buns"));
		if (it != res.end()) {
			cout << endl << "Currently " << it->num <<
					" hotdog buns in stock." << endl;
		}
		else {
			cout << endl << "Sorry, no hotdog buns in stock." << endl;
		}
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

	return 0;
}

