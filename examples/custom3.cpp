#include "util.h"

#include <mysql++.h>
#include <custom.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace mysqlpp;

sql_create_5(stock,
			1, 5,
			string, item,
			longlong, num,
			double, weight,
			double, price,
			Date, sdate)

int
main(int argc, char *argv[])
{
	try {
		Connection con(use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		Query query = con.query();
		query << "select * from stock where item = \"Hotdogs' Buns\" ";

		// Is the query was successful?  If not throw a bad query.
		Result res = query.store();
		if (res.empty()) {
			throw BadQuery("Hotdogs' Buns not found in table, run resetdb");
		}

		// Because there should only be one row in the result set, we
		// don't need to use a vector.  Just store the first row
		// directly in "row".  We can do this because one of the
		// constructors for stock takes a Row as a parameter.
		stock row = res[0];

		// Create a copy so that the replace query knows what the
		// original values are.
		stock row2 = row;

		// Change item column, fixing the spelling.
		row.item = "Hotdog Buns";

		// Form the query to replace the row.  The table name is the
		// name of the struct by default.
		query.update(row2, row);

		// Show the query about to be executed.
		cout << "Query : " << query.preview() << endl;

		// Call execute(), since the query won't return a result set.
		query.execute();

		// Now print the new table
		print_stock_table(query);
	}
	catch (BadQuery& er) {
		// handle any connection or query errors that may come up
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
	catch (BadConversion& er) {
		// handle bad conversions
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
		return -1;
	}
	catch (exception& er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}

