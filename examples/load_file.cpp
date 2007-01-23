/***********************************************************************
 load_file.cpp - Example showing how to insert BLOB data into the
	database from a file.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004-2006 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#include <mysql++.h>

#include <sys/stat.h>

#include <fstream>

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
		con.connect(MY_DATABASE, MY_HOST, MY_USER, MY_PASSWORD);
		Query query = con.query();
		ifstream In(argv[1], ios::in | ios::binary);
		if (In) {
			struct stat for_len;
			if (stat(argv[1], &for_len) < 0) {
				cerr << "stat() failed for " << argv[1] << '!' << endl;
				return -1;
			}

			unsigned int blen = for_len.st_size;
			if (blen == 0) {
				cerr << "Sorry, " << argv[1] << " is empty; I won't "
						"insert such a thing." << endl;
				return -1;
			}

			char* read_buffer = new char[blen];
			In.read(read_buffer, blen);
			string fill(read_buffer, blen);
			ostringstream strbuf;
			strbuf << "INSERT INTO " << MY_TABLE << " (" << MY_FIELD <<
					") VALUES(\"" << mysqlpp::escape << fill << "\")" <<
					ends;
			query.exec(strbuf.str());
			delete[] read_buffer;
		}
		else {
			cerr << "Failed to open " << argv[1] << '.' << endl;
		}
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
		return -1;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}
