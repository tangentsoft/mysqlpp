#include <iostream>
#include <iomanip>
#include <sqlplus.hh>

using namespace std;

int main() {
  // The full format for the Connection constructor is
  // Connection(cchar *db, cchar *host="", 
  //            cchar *user="", cchar *passwd="") 
  // You may need to specify some of them if the database is not on
  // the local machine or you database username is not the same as your
  // login name, etc..
  try {
		Connection con("mysql_cpp_data");
		Query query = con.query();
		// This creates a query object that is bound to con.

		query << "select * from stock";
		// You can write to the query object like you would any other ostrem

		Result res = query.store();
		// Query::store() executes the query and returns the results

		cout << "Query: " << query.preview() << endl;
		// Query::preview() simply returns a string with the current query
		// string in it.

		cout << "Records Found: " << res.size() << endl << endl;
  
		Row row;
		cout.setf(ios::left);
		cout << setw(17) << "Item" 
			<< setw(4)  << "Num"
			<< setw(7)  << "Weight"
			<< setw(7)  << "Price" 
			<< "Date" << endl
			<< endl;
  
		Result::iterator i;
		// The Result class has a read-only Random Access Iterator
		for (i = res.begin(); i != res.end(); i++) {
			row = *i;
			cout << setw(17) << row[0].c_str()
				<< setw(4)  << row[1].c_str()
				<< setw(7)  << row.lookup_by_name("weight").c_str()
				// you can use either the index number or column name when
				// retrieving the colume data as demonstrated above.
				<< setw(7)  << row[3].c_str()
				<< row[4] << endl;
		}
  } catch (BadQuery &er) { // handle any connection or
                          // query errors that may come up
#ifdef USE_STANDARD_EXCEPTION
    cerr << "Error: " << er.what() << endl;
#else
    cerr << "Error: " << er.error << endl;
#endif
    return -1;
  } catch (BadConversion &er) { // handle bad conversions
#ifdef USE_STANDARD_EXCEPTION
    cerr << "Error: " << er.what() << "\"." << endl
         << "retrieved data size: " << er.retrieved
         << " actual data size: " << er.actual_size << endl;
#else
    cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
         << er.type_name << "\"." << endl;
#endif
    return -1;
#ifdef USE_STANDARD_EXCEPTION		
  } catch (exception &er) {
    cerr << "Error: " << er.what() << endl;
    return -1;
#endif		
  }
	return 0;
}
