#include "util.hh"

#include <mysql++.h>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace mysqlpp;

int
main(int argc, char *argv[])
{
	try {
		Connection con(use_exceptions);
		connect_sample_db(argc, argv, con);

		Query query = con.query();

		query << "select * from stock";
		Result res = query.store();

		cout << "Query: " << query.preview() << endl;
		cout << "Records Found: " << res.size() << endl << endl;

		Row row;
		cout.setf(ios::left);
		cout << setw(17) << "Item"
				<< setw(4) << "Num"
				<< setw(7) << "Weight"
				<< setw(7) << "Price" << "Date" << endl << endl;

		Result::iterator i;

		cout.precision(3);
		for (i = res.begin(); i != res.end(); i++) {
			row = *i;
			// Regarding the (double) casts, we do this so that we
			// can set the display precision.  ColData has the nice
			// feature that it will convert to any of the basic C++
			// types.  If there is a problem in the conversion it
			// will throw an exception (which is caught below).
			// To test it try changing the 2 in row[2] to row[0].
			cout << setw(17) << row["ITEM"] << "," << setw(4) << row[1]
					<< setw(7) << (double) row[2]
					<< setw(7) << (double) row[3];
			// The ColData is implicitly converted to a date here.
			Date date = row.lookup_by_name("SDATE");
			cout.setf(ios::right);
			cout.fill('0');
			cout << setw(2) << date.month << "-" << setw(2) << date.
				day << endl;
			cout.fill(' ');
			cout.unsetf(ios::right);
		}

		return 0;
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
}
