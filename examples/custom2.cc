#include "util.hh"

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

		// create an empty stock object
		stock row;

		// populate stock
		row.set("Hot Dogs", 100, 1.5, 1.75, "1998-09-25");

		// form the query to insert the row
		// the table name is the name of the struct by default
		query.insert(row);

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


