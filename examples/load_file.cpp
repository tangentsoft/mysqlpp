#include <mysql++.h>

#include <sys/stat.h>

#include <fstream>

#include <errno.h>
#include <stdlib.h>

using namespace std;
using namespace mysqlpp;

const char MY_DATABASE[] = "telcent";
const char MY_TABLE[] = "fax";
const char MY_HOST[] = "localhost";
const char MY_USER[] = "root";
const char MY_PASSWORD[] = "";
const char MY_FIELD[] = "fax";	// BLOB field

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		cerr << "Usage : load_file full_file_path" << endl << endl;
		return -1;
	}

	Connection con(use_exceptions);
	try {
		con.real_connect(MY_DATABASE, MY_HOST, MY_USER, MY_PASSWORD, 3306,
						 0, 60, NULL);
		Query query = con.query();
		ostringstream strbuf;
		ifstream In(argv[1], ios::in | ios::binary);
		struct stat for_len;
		if ((In.rdbuf())->is_open()) {
			if (stat(argv[1], &for_len) == -1)
				return -1;
			unsigned int blen = for_len.st_size;
			if (!blen)
				return -1;
			char *read_buffer = new char[blen];
			In.read(read_buffer, blen);
			string fill(read_buffer, blen);
			strbuf << "INSERT INTO " << MY_TABLE << " (" << MY_FIELD <<
				") VALUES(\"" << mysqlpp::escape << fill << "\")" << ends;
			query.exec(strbuf.str());
			delete[]read_buffer;
		}
		else
			cerr << "Your binary file " << argv[1] <<
				"could not be open, errno = " << errno;
		return 0;

	}
	catch (BadQuery& er) {
		// handle any connection or query errors that may come up
		cerr << "Error: " << er.what() << " " << con.errnum() << endl;
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
}
