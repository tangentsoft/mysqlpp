#include "util.h"

#include <mysql++.h>

#include <iostream>

using namespace std;

int
main(int argc, char *argv[])
{
	try {
		mysqlpp::Connection con(mysqlpp::use_exceptions);
		if (!connect_to_db(argc, argv, con, "")) {
			return 1;
		}

		bool created = false;
		try {
			con.select_db(kpcSampleDatabase);
		}
		catch (mysqlpp::BadQuery &) {
			// Couldn't switch to the sample database, so assume that it
			// doesn't exist and create it.  If that doesn't work, exit
			// with an error.
			if (con.create_db(kpcSampleDatabase)) {
				cerr << "Failed to create sample database." << endl;
				return 1;
			}
			else if (!con.select_db(kpcSampleDatabase)) {
				cerr << "Failed to select sample database." << endl;
				return 1;
			}
			else {
				created = true;
			}
		}

		mysqlpp::Query query = con.query();	// create a new query object

		try {
			query.execute("drop table stock");
		}
		catch (mysqlpp::BadQuery&) {
			// ignore any errors
		}

		// Send the query to create the table and execute it.
		query << "create table stock  (item char(20) not null, num bigint,"
			<< "weight double, price double, sdate date)";
		query.execute();

		// Set up the template query to insert the data.  The parse
		// call tells the query object that this is a template and
		// not a literal query string.
		query << "insert into %5:table values (%0q, %1q, %2, %3, %4q)";
		query.parse();

		// This is setting the parameter named table to stock.
		query.def["table"] = "stock";

		// The last parameter "table" is not specified here.  Thus the
		// default value for "table" is used, which is "stock".  Also,
		// the bad grammar in the second row is intentional -- it is
		// fixed by the custom3 example.
		query.execute("Hamburger Buns", 56, 1.25, 1.1, "1998-04-26");
		query.execute("Hotdogs' Buns", 65, 1.1, 1.1, "1998-04-23");
		query.execute("Dinner Rolls", 75, .95, .97, "1998-05-25");
		query.execute("White Bread", 87, 1.5, 1.75, "1998-09-04");

		if (created) {
			cout << "Created";
		}
		else {
			cout << "Reinitialized";
		}
		cout << " sample database successfully." << endl;
	}
	catch (mysqlpp::BadQuery& er) {
		// Handle any connection or query errors that may come up
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
	catch (mysqlpp::BadConversion& er) {
		// Handle bad conversions
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
		return -1;
	}
	catch (exception& er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
}
