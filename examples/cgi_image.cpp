/***********************************************************************
 cgi_image.cpp - Example code showing how to fetch BLOB data from a
	MySQL table and build a reply suitable for making this a CGI
	handler.  Something very much like this could pull dynamic images
	from a database to populate web pages, for instance.

	See load_file.cpp for BLOB data insert example.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
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

using namespace std;
using namespace mysqlpp;

#define MY_DATABASE	"telcent"
#define MY_TABLE	"fax"
#define MY_HOST		"localhost"
#define MY_USER		"root"
#define MY_PASSWORD ""
#define MY_FIELD    "fax"		// BLOB field
#define MY_KEY      "datet"		// PRIMARY KEY

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		cerr << "Usage : cgi_image primary_key_value" << endl << endl;
		return -1;
	}

	cout << "Content-type: image/jpeg" << endl;
	Connection con(use_exceptions);
	try {
		con.real_connect(MY_DATABASE, MY_HOST, MY_USER, MY_PASSWORD, 3306,
						 0, 60, NULL);
		Query query = con.query();
		query << "SELECT " << MY_FIELD << " FROM " << MY_TABLE << " WHERE "
			<< MY_KEY << " = " << argv[1];
		ResUse res = query.use();
		Row row = res.fetch_row();
		long unsigned int *jj = res.fetch_lengths();
		cout << "Content-length: " << *jj << endl << endl;
		fwrite(row.raw_data(0), 1, *jj, stdout);
		return 0;
	}
	catch (BadQuery& er) {
		cerr << "Error: " << er.what() << " " << con.errnum() << endl;
		return -1;
	}
	catch (exception& er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
}

