#include "util.h"

#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

const char* kpcSampleDatabase = "mysql_cpp_data";

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

	// Use the Result class's read-only random access iterator to walk
	// through the query results.
	mysqlpp::Row row;
	mysqlpp::Result::iterator i;
	for (i = res.begin(); i != res.end(); ++i) {
		row = *i;

		// Note that you can use either the column index or name to
		// retrieve the data.
		cout << setw(20) << row[0].c_str() << ' ' <<
				setw(9) << row[1].c_str() << ' ' <<
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

