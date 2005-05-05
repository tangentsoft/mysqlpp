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

	// Execute the query and save the results.
	mysqlpp::Result res = query.store();
	cout << "Records Found: " << res.size() << endl << endl;

	// Display a header for the stock table
	cout.setf(ios::left);
	cout << setw(21) << "Item" <<
			setw(10) << "Num" <<
			setw(10) << "Weight" <<
			setw(10) << "Price" <<
			"Date" << endl << endl;

#ifdef USE_WIN32_UCS2
	char ansi[100];
#endif

	// Use the Result class's read-only random access iterator to walk
	// through the query results.
	mysqlpp::Row row;
	mysqlpp::Result::iterator i;
	for (i = res.begin(); i != res.end(); ++i) {
		row = *i;

		// Output first column, the item string.  The UCS2 option shows
		// how we can convert the data to get Unicode output on Windows.
		// On Unix, the terminal code interprets UTF-8 data directly.
#ifdef USE_WIN32_UCS2
		if (utf8_to_win32_ansi(row[0].c_str(), ansi, sizeof(ansi))) {
			cout << setw(20) << ansi << ' ';
		}
#else
		cout << setw(20) << row[0].c_str() << ' ';
#endif

		// Note that you can use either the column index or name to
		// retrieve the data.
		cout << setw(9) << row[1].c_str() << ' ' <<
				setw(9) << row.lookup_by_name("weight").c_str() << ' ' <<
				setw(9) << row[3].c_str() << ' ' <<
				row[4] << endl;
	}
}


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

