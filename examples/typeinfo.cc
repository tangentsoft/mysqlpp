
#include <iostream>
#include <iomanip>
#include <mysql++>

int main() {
  try { // its in one big try block

    Connection con(use_exceptions);
    con.connect("mysql_cpp_data");
    Query query = con.query();
    query << "select * from stock";
    Result res = query.store();
    
    cout << mysql_type_info(type_id(double)).sql_name << endl;

    return 0;
  } catch (BadQuery er) { // handle any connection or
                          // query errors that may come up
    cerr << "Error: " << er.error << endl;
    return -1;
  } catch (BadConversion er) { // handle bad conversions
    cerr << "Error: Tried to convert \"" << er.data << "\" to a \"" 
	 << er.type_name << "\"." << endl;
    return -1;
  }
}


