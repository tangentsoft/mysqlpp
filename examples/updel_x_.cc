#include <mysql++.hh>
#include <string>

using namespace std;

#define MY_DATABASE	"telcent"
#define MY_TABLE		"nazivi"
#define MY_HOST    "localhost"
#define MY_USER    "root"
#define MY_PASSWORD ""
#define MY_FIELD    "naziv"
#define MY_QUERY    "SELECT URL from my_table as t1, my_table as t2 where t1.field = t2.field"

int  main (void) {
  Connection con(use_exceptions);
	try {
		ostringstream strbuf; unsigned int i=0;
		con.real_connect (MY_DATABASE,MY_HOST,MY_USER,MY_PASSWORD,3306,(int)0,60,NULL);
		Query query = con.query(); query << MY_QUERY; 
		ResUse res = query.use(); Row row; 
		strbuf << "delete from " << MY_TABLE << " where " << MY_FIELD << " in (";
//  for UPDATE just replace the above DELETE FROM with UPDATE statement
		for(;row=res.fetch_row();i++) strbuf <<	 row[0] << ",";	if (!i) return 0; 
		string output(strbuf.str()); output.erase(output.size()-1,1); output += ")";
		query.exec((const string&)output); // cout << output << endl;
		return 0;
	} catch (BadQuery &er) { // handle any connection or
                          // query errors that may come up
#ifdef USE_STANDARD_EXCEPTION
		cerr << "Error: " << er.what() << " " << con.errnum() << endl;
#else
		cerr << "Error: " << er.error << " " << con.errnum() << endl;
#endif
		return -1;
	} catch (BadConversion &er) { // handle bad conversions
    // we still need to cache bad conversions incase something goes 
    // wrong when the data is converted into stock
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
