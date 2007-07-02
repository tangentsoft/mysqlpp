/***********************************************************************
 cgi_jpeg.cpp - Example code showing how to fetch JPEG data from a BLOB
 	column and send it back to a browser that requested it by ID.
	
	Use load_jpeg.cpp to load JPEG files into the database we query.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004-2007 by Educational Technology Resources, Inc.
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
#include <custom.h>

using namespace std;
using namespace mysqlpp;

#define IMG_DATABASE	"mysql_cpp_data"
#define IMG_HOST		"localhost"
#define IMG_USER		"root"
#define IMG_PASSWORD 	"nunyabinness"

sql_create_2(images,
	1, 2,
	mysqlpp::sql_int_unsigned, id,
	mysqlpp::sql_blob, data)

int
main()
{
	unsigned int img_id = 0;
	char* cgi_query = getenv("QUERY_STRING");
	if (cgi_query) {
		if ((strlen(cgi_query) < 4) || memcmp(cgi_query, "id=", 3)) {
			cout << "Content-type: text/plain" << endl << endl;
			cout << "ERROR: Bad query string" << endl;
			return 1;
		}
		else {
			img_id = atoi(cgi_query + 3);
		}
	}
	else {
		cerr << "Put this program into a web server's cgi-bin "
				"directory, then" << endl;
		cerr << "invoke it with a URL like this:" << endl;
		cerr << endl;
		cerr << "    http://server.name.com/cgi-bin/cgi_jpeg?id=2" <<
				endl;
		cerr << endl;
		cerr << "This will retrieve the image with ID 2." << endl;
		cerr << endl;
		cerr << "You will probably have to change some of the #defines "
				"at the top of" << endl;
		cerr << "examples/cgi_jpeg.cpp to allow the lookup to work." <<
				endl;
		return 1;
	}

	Connection con(use_exceptions);
	try {
		con.connect(IMG_DATABASE, IMG_HOST, IMG_USER, IMG_PASSWORD);
		Query query = con.query();
		query << "SELECT * FROM images WHERE id = " << img_id;
		ResUse res = query.use();
		if (res) {
			images img = res.fetch_row();
			cout << "Content-type: image/jpeg" << endl;
			cout << "Content-length: " << img.data.length() << "\n\n";
			cout << img.data;
		}
		else {
			cout << "Content-type: text/plain" << endl << endl;
			cout << "ERROR: No such image with ID " << img_id << endl;
		}
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cout << "Content-type: text/plain" << endl << endl;
		cout << "QUERY ERROR: " << er.what() << endl;
		return 1;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cout << "Content-type: text/plain" << endl << endl;
		cout << "GENERAL ERROR: " << er.what() << endl;
		return 1;
	}

	return 0;
}
