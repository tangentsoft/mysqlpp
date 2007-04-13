/***********************************************************************
 util.cpp - Utility functions required by several of the example
 	programs.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004-2006 by Educational Technology Resources, Inc.
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

#include <iostream>
#include <iomanip>

// This include isn't needed by util module.  It's just a test of the
// new SSQLS feature allowing the structure to be defined in many
// modules without having a multiply-defined static variable error.
// Don't do this for VC++ 2003: it doesn't support variadic macros,
// which this feature depends on.
#if !defined(_MSC_VER) || _MSC_VER >= 1400
#	define MYSQLPP_SSQLS_NO_STATICS
#	include "stock.h"		
#endif

using namespace std;

const char* kpcSampleDatabase = "mysql_cpp_data";


//// print_stock_header ////////////////////////////////////////////////
// Display a header suitable for use with print_stock_rows().

void
print_stock_header(int rows)
{
	cout << "Records found: " << rows << endl << endl;
	cout.setf(ios::left);
	cout << setw(21) << "Item" <<
			setw(10) << "Num" <<
			setw(10) << "Weight" <<
			setw(10) << "Price" <<
			"Date" << endl << endl;
}


//// print_stock_row ///////////////////////////////////////////////////
// Print out a row of data from the stock table, in a format compatible
// with the header printed out in the previous function.

void
print_stock_row(const mysqlpp::sql_char& item, mysqlpp::sql_bigint num,
		mysqlpp::sql_double weight, mysqlpp::sql_double price,
		const mysqlpp::sql_date& date)
{
	char buf[100];
	cout << setw(20) << item << ' ' <<
			setw(9) << num << ' ' <<
			setw(9) << weight << ' ' <<
			setw(9) << price << ' ' <<
			date << endl;
}


//// print_stock_row ///////////////////////////////////////////////////
// Take a Row from the example 'stock' table, break it up into fields,
// and call the above version of this function.

void
print_stock_row(const mysqlpp::Row& row)
{
	// The brief code below illustrates several aspects of the library
	// worth noting:
	//
	// 1. You can subscript a row by integer (position of the field in
	// the row) or by string (name of field in the row).  The former is
	// more efficient, while the latter trades some efficiency for
	// robustness in the face of schema changes.  (Consider using SSQLS
	// if you need a tradeoff in between these two positions.)
	// 
	// 2. You can also get at a row's field's with Row::at(), which is
	// much like Row::operator[](int).  Besides the syntax difference,
	// the only practical difference is that only at() can access field
	// 0: this is because '0' can be converted to both int and to const
	// char*, so the compiler rightly complains that it can't decide
	// which overload to call.
	//
	// 3. Notice that we make an explicit temporary copy of the first
	// field, which is the only string field.  We must tolerate the
	// inefficiency of this copy, because Row::operator[] returns a
	// ColData object, which goes away after it is converted to some
	// other form.  So, while we could have made print_stock_row()
	// take a const char* argument (as past versions mistakenly did!)
	// this would result in a dangling pointer, since it points into the
	// ColData object, which is dead by the time the pointer is
	// evaluated in print_stock_row().  It will probably even work this
	// way, but like any memory bug, it can wreak subtle havoc.
	std::string item(row.at(0));
	print_stock_row(item, row["num"], row[2], row[3], row[4]);
}


//// print_stock_rows //////////////////////////////////////////////////
// Print out a number of rows from the example 'stock' table.

void
print_stock_rows(mysqlpp::Result& res)
{
	print_stock_header(res.size());

	// Use the Result class's read-only random access iterator to walk
	// through the query results.
	mysqlpp::Result::iterator i;
	for (i = res.begin(); i != res.end(); ++i) {
		// Notice that a dereferenced result iterator can be converted
		// to a Row object, which makes for easier element access.
		print_stock_row(*i);
	}
}


//// get_stock_table ///////////////////////////////////////////////////
// Retreive the entire contents of the example 'stock' table.

void
get_stock_table(mysqlpp::Query& query, mysqlpp::Result& res)
{
	// Reset the query object, in case we're re-using it.
	query.reset();

	// You can write to the query object like you would any ostream.
	query << "select * from stock";

	// Show the query string.  If you call preview(), it must be before
	// you call execute() or store() or use().
	cout << "Query: " << query.preview() << endl;

	// Execute the query, storing the results in memory.
	res = query.store();
}


//// print_stock_table /////////////////////////////////////////////////
// Simply retrieve and print the entire contents of the stock table.

void
print_stock_table(mysqlpp::Query& query)
{
	mysqlpp::Result res;
	get_stock_table(query, res);
	print_stock_rows(res);
}


//// print_usage ///////////////////////////////////////////////////////
// Show the program's usage message

void
print_usage(const char* program_name, const char* extra_parms)
{
	cout << "usage: " << program_name <<
			" [host [user [password [port]]]] " << extra_parms << endl;
	cout << endl;
	cout << "    If no arguments are given, connects to database "
			"server on localhost" << endl;
	cout << "    using your user name and no password.  You may give "
			"any number of" << endl;
	cout << "    these arguments, but they must be in the order "
			"listed, and you" << endl;
	cout << "    cannot skip preceding arguments." << endl;
	if (strlen(extra_parms) > 0) {
		cout << endl;
		cout << "    The extra parameter " << extra_parms <<
				" is required, regardless of which" << endl;
		cout << "    other arguments you pass." << endl;
	}
	cout << endl;
}


//// connect_to_db /////////////////////////////////////////////////////
// Establishes a connection to a MySQL database server, optionally
// attaching to database kdb.  This is basically a command-line parser
// for the examples, since the example programs' arguments give us the
// information we need to establish the server connection.

bool
connect_to_db(int argc, char *argv[], mysqlpp::Connection& con,
		const char *kdb)
{
	if (argc < 1) {
		cerr << "Bad argument count: " << argc << '!' << endl;
		return false;
	}

	if (!kdb) {
		kdb = kpcSampleDatabase;
	}

	if ((argc > 1) && (argv[1][0] == '-')) {
		print_usage(argv[0]);
		return false;
	}

	if (argc == 1) {
		con.connect(kdb);
	}
	else if (argc == 2) {
		con.connect(kdb, argv[1]);
	}
	else if (argc == 3) {
		con.connect(kdb, argv[1], argv[2]);
	}
	else if (argc == 4) {
		con.connect(kdb, argv[1], argv[2], argv[3]);
	}
	else if (argc >= 5) {
		con.connect(kdb, argv[1], argv[2], argv[3], atoi(argv[4]));
	}

	if (con) {
		return true;
	}
	else {
		cerr << "Database connection failed: " << con.error() << endl;
		return false;
	}
}
