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

// Uncomment this macro if you want the 'item' strings to be converted
// from UTF-8 to UCS-2LE (i.e. Win32's native Unicode encoding) using
// the Win32 API function MultiByteToWideChar().  We use a native Win32
// function because Unix compatibility isn't necessary: modern Unices
// handle UTF-8 directly.
//#define USE_WIN32_UCS2

const char* kpcSampleDatabase = "mysql_cpp_data";


//// utf8_to_win32_ansi ////////////////////////////////////////////////
// Converts a Unicode string encoded in UTF-8 form (which the MySQL
// database uses) to Win32's ANSI character encoding using the current
// code page.  A small modification to this function will turn it into
// a UTF-8 to UCS-2 function, since we convert to UCS-2 before
// continuing on from there to ANSI.  See the Win32 section in the
// Unicode chapter in the user manual for the reason this function works
// as it does.

#ifdef USE_WIN32_UCS2
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
		// This last step is only necessary when outputting to the 
		// Win32 console, because it doesn't support UCS-2 by default.
		// If you somehow convince the console to accept UCS-2, or are
		// using other Win32 functions that do accept UCS-2 data, then
		// you want to use the ucs2_buf above instead.  See the MySQL++
		// user guide for more on this topic.
		CPINFOEX cpi;
		GetCPInfoEx(CP_OEMCP, 0, &cpi);
		WideCharToMultiByte(cpi.CodePage, 0, ucs2_buf, -1,
				ansi_str, ansi_len, 0, 0);
		cout << ':' << ansi_str[0] << ':' << endl;
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
// Print out a row of data from the stock table, in a format
// compatbile with the header printed out in the previous function.

void
print_stock_row(const char* item, mysqlpp::longlong num, double weight,
		double price, mysqlpp::Date date)
{
	// Output first column, the item string.  The UCS2 option shows
	// how we can convert the data to get Unicode output on Windows.
	// On modern Unices, the terminal code interprets UTF-8 data
	// directly.
#ifdef USE_WIN32_UCS2
	char item_ansi[100];
	if (utf8_to_win32_ansi(item, item_ansi, sizeof(item_ansi))) {
		cout << setw(20) << item_ansi << ' ';
	}
#else
	cout << setw(20) << item << ' ';
#endif

	// Output remaining columns
	cout << setw(9) << num << ' ' <<
			setw(9) << weight << ' ' <<
			setw(9) << price << ' ' <<
			date << endl;
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
		// Convert the Result iterator into a Row object, for easier
		// access.
		mysqlpp::Row row(*i);

		// Notice that you can use either the column index or name to
		// retrieve the data.  Also notice that we do no explicit
		// conversions to match print_stock_row()'s parameter types:
		// Row elements are ColData strings, so they auto-convert to
		// any standard C++ type.
		print_stock_row(row[0], row[1], row.lookup_by_name("weight"),
				row[3], row[4]);
	}
}


//// print_stock_table /////////////////////////////////////////////////
// Print the entire contents of the example 'stock' table.

void
print_stock_table(mysqlpp::Query& query)
{
	// You must reset the query object when re-using it.
	query.reset();

	// You can write to the query object like you would any ostream.
	query << "select * from stock";

	// Show the query string.  If you do this, you have to do it before
	// you execute() or store() or use() it.
	cout << "Query: " << query.preview() << endl;

	// Execute the query and display the result set.
	mysqlpp::Result res = query.store();
	print_stock_rows(res);
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

	if ((argc > 1) && (argv[1][0] == '-')) {
		cout << "usage: " << argv[0] <<
				" [host] [user] [password] [port]" << endl;
		cout << endl << "\tConnects to database ";
		if (kdb) {
			cout << '"' << kdb << '"';
		}
		else {
			cout << "server";
		}
		cout << " on localhost using your user" << endl;
		cout << "\tname and no password by default." << endl << endl;
		return false;
	}

	if (!kdb) {
		kdb = kpcSampleDatabase;
	}

	bool success = false;
	if (argc == 1) {
		success = con.connect(kdb);
	}
	else if (argc == 2) {
		success = con.connect(kdb, argv[1]);
	}
	else if (argc == 3) {
		success = con.connect(kdb, argv[1], argv[2]);
	}
	else if (argc == 4) {
		success = con.connect(kdb, argv[1], argv[2], argv[3]);
	}
	else if (argc >= 5) {
		success = con.real_connect(kdb, argv[1], argv[2], argv[3],
				atoi(argv[4]));
	}

	if (!success) {
		cerr << "Database connection failed." << endl << endl;
	}

	return success;
}

