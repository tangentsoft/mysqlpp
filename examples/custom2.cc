
#include <iostream>
#include <vector>
#include <mysql++.hh>
#include <custom.hh>
#include "util.hh"

#include <string>

using namespace std;

// util.hh/cc contains the print_stock_table function

sql_create_5(stock, 1, 5, string, item, longlong, num, 
	     double, weight, double, price, Date, sdate)

int main(int argc, char *argv[]) {
  try { // its in one big try block
	Connection con(use_exceptions);
	connect_sample_db(argc, argv, con);

    Query query = con.query();

    stock row;
    // create an empty stock object
    
    /*    row.item = "Hot Dogs";
    row.num = 100;
    row.weight = 1.5;
    row.price = 1.75;
    row.sdate = "1998-09-25"; */
    row.set("Hot Dogs", 100, 1.5, 1.75, "1998-09-25");
    // populate stock

    query.insert(row);
    // form the query to insert the row
    // the table name is the name of the struct by default
    cout << "Query : " << query.preview() << endl;
    // show the query about to be executed
    query.execute();
    // execute a query that does not return a result set

    print_stock_table(query);
    // now print the new table;

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
}
