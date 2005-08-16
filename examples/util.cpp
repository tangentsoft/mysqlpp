/***********************************************************************
 util.cpp - Utility functions required by several of the example
 	programs.

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

#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

const char* kpcSampleDatabase = "mysql_cpp_data";


//// utf8_to_win32_ansi ////////////////////////////////////////////////
// Converts a Unicode string encoded in UTF-8 form (which the MySQL
// database uses) to Win32's ANSI character encoding using the current
// code page.  A small modification to this function will turn it into
// a UTF-8 to UCS-2 function, since that's an intermediate form within
// this function.  The Unicode chapter in the user manual explains why
// that double conversion is necessary.

#ifdef MYSQLPP_PLATFORM_WINDOWS
static bool
utf8_to_win32_ansi(const char* utf8_str, char* ansi_str,
		int ansi_len)
{
	wchar_t ucs2_buf[100];
	static const int ub_chars = sizeof(ucs2_buf) / sizeof(ucs2_buf[0]);

	int err = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1,
			ucs2_buf, ub_chars);
	if (err == 0) {
		cerr << "Unknown error in Unicode translation: " <<
				GetLastError() << endl;
		return false;
	}
	else if (err == ERROR_NO_UNICODE_TRANSLATION) {
		cerr << "Bad data in UTF-8 string" << endl;
		return false;
	}
	else {
		CPINFOEX cpi;
		GetCPInfoEx(CP_OEMCP, 0, &cpi);
		WideCharToMultiByte(cpi.CodePage, 0, ucs2_buf, -1,
				ansi_str, ansi_len, 0, 0);
		return true;
	}
}
#endif


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
print_stock_row(const std::string& item, mysqlpp::longlong num,
		double weight, double price, const mysqlpp::Date& date)
{
	// Output item field.  We treat it separately because there is
	// Unicode data in this field in the sample database.
#ifdef MYSQLPP_PLATFORM_WINDOWS
	// We're running on Windows, so convert the first column from UTF-8
	// to UCS-2, and then to the local ANSI code page.  The user manual
	// explains why this double conversion is required.
	char item_ansi[100];
	if (utf8_to_win32_ansi(item.c_str(), item_ansi, sizeof(item_ansi))) {
		cout << setw(20) << item_ansi << ' ';
	}
#else
	// Just send string to console.  On modern Unices, the terminal code
	// interprets UTF-8 directly, so no special handling is required.
	cout << setw(20) << item << ' ';
#endif

	// Output remaining columns
	cout << setw(9) << num << ' ' <<
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
		cout << "usage: " << argv[0] <<
				" [host] [user] [password] [port]" << endl;
		cout << endl << "\tConnects to database ";
		if (strlen(kdb) > 0) {
			cout << '"' << kdb << '"';
		}
		else {
			cout << "server";
		}
		cout << " on localhost using your user" << endl;
		cout << "\tname and no password by default." << endl << endl;
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
