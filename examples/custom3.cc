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
		connect_sample_db(argc, argv, con);

		Query query = con.query();
		query << "select * from stock where item = \"Hotdogs' Buns\" ";

		// Is the query was successful?  If not throw a bad query.
		Result res = query.store();
		if (res.empty()) {
			throw BadQuery("Hotdogs' Buns not found in table, run reset-db");
		}

		// because there should only be one row in this query we don't
		// need to use a vector.  Just store the first row directly in
		// "row".  We can do this because one of the constructors for
		// stock takes a Row as an parameter.
		stock row = res[0];

		// Now we need to create a copy so that the replace query knows
		// what the original values are.
		stock row2 = row;

		row.item = "Hotdog Buns";	// now change item

		// form the query to replace the row
		// the table name is the name of the struct by default
		query.update(row2, row);

		// show the query about to be executed
		cout << "Query : " << query.preview() << endl;

		// execute a query that does not return a result set
		query.execute();

		// now print the new table;
		print_stock_table(query);
	}
	catch (BadQuery& er) {
		// handle any connection or query errors that may come up
#ifdef USE_STANDARD_EXCEPTION
		cerr << "Error: " << er.what() << endl;
#else
		cerr << "Error: " << er.error << endl;
#endif
		return -1;
	}
	catch (BadConversion& er) {
		// handle bad conversions
#ifdef USE_STANDARD_EXCEPTION
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
#else
		cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
			<< er.type_name << "\"." << endl;
#endif
		return -1;
	}
#ifdef USE_STANDARD_EXCEPTION
	catch (exception& er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
#endif

	return 0;
}

