#include "util.h"

#include <mysql++.h>

#include <iostream>
#include <iomanip>

using namespace std;

int
main(int argc, char *argv[])
{
	try {
		// Connect to the sample database
		mysqlpp::Connection con(mysqlpp::use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		// Create a query object that is bound to con.
		mysqlpp::Query query = con.query();

		// Print out the stock table
		print_stock_table(query);
	}
	catch (mysqlpp::BadQuery& er) {
		// handle any connection or query errors that may come up
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
	catch (mysqlpp::BadConversion& er) {
		// handle bad conversions
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
		return -1;
	}
	catch (exception & er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}
