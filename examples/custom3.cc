
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

int main() {
  try { // its in one big try block

    Connection con(use_exceptions);
    con.connect("mysql_cpp_data");
    Query query = con.query();

    query << "select * from stock where item = \"Hotdogs' Buns\" ";
    
    Result res = query.store();
    if (res.empty()) 
      throw BadQuery("Hotdogs' Buns not found in table, run reset-db");
    // here we are testing if the query was successful, if not throw a bad query
    stock row = res[0];
    // because there should only be one row in this query we don't
    // need to use a vector.  Just store the first row directly in
    // "row".  We can do this because one of the constructors for
    // stock takes a Row as an parameter.

    stock row2 = row;
    // Now we need to create a copy so that the replace query knows
    // what the original values are.

    row.item = "Hotdog Buns"; // now change item

    query.update(row2, row);
    // form the query to replace the row
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
