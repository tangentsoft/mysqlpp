#include <mysql++.hh>

#define MY_DATABASE	"telcent"
#define MY_TABLE		"fax"
#define MY_HOST    "localhost"
#define MY_USER    "root"
#define MY_PASSWORD ""
#define MY_FIELD    "fax" // BLOB field
#define MY_KEY      "datet"  // PRIMARY KEY

int  main (int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "Usage : cgi_image primary_key_value" << endl << endl;
		return -1;
	}
	cout << "Content-type: image/jpeg" << endl;
  Connection con(use_exceptions);
	try {
		con.real_connect (MY_DATABASE,MY_HOST,MY_USER,MY_PASSWORD,3306,(int)0,60,NULL);
		Query query = con.query();
		query << "SELECT " << MY_FIELD << " FROM " << MY_TABLE << " WHERE " << MY_KEY << " = " << argv[1];
		ResUse res = query.use(); Row row=res.fetch_row(); long unsigned int *jj = res.fetch_lengths();
		cout << "Content-length: " << *jj << endl << endl; 
		fwrite(row.raw_data(0),1,*jj,stdout);
		return 0;
	} catch (BadQuery er) { 
#ifdef USE_STANDARD_EXCEPTION
    cerr << "Error: " << er.what() << " " << con.errnum() << endl;
#else
    cerr << "Error: " << er.error << " " << con.errnum() << endl;
#endif
    return -1;
#ifdef USE_STANDARD_EXCEPTION		
	} catch (exception &er) {
    cerr << "Error: " << er.what() << endl;
    return -1;
#endif		
        }
}
