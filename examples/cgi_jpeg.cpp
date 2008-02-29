/***********************************************************************
 cgi_jpeg.cpp - Example code showing how to fetch JPEG data from a BLOB
 	column and send it back to a browser that requested it by ID.
	
	Use load_jpeg.cpp to load JPEG files into the database we query.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2008 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS file in
 the top directory of the distribution for details.

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
#include <ssqls.h>

#define IMG_DATABASE	"mysql_cpp_data"
#define IMG_HOST		"localhost"
#define IMG_USER		"root"
#define IMG_PASSWORD 	"nunyabinness"

sql_create_2(images,
	1, 2,
	mysqlpp::sql_int_unsigned, id,
	mysqlpp::sql_blob, data)

int main()
{
	unsigned int img_id = 0;
	char* cgi_query = getenv("QUERY_STRING");
	if (cgi_query) {
		if ((strlen(cgi_query) < 4) || memcmp(cgi_query, "id=", 3)) {
			std::cout << "Content-type: text/plain" << std::endl << std::endl;
			std::cout << "ERROR: Bad query string" << std::endl;
			return 1;
		}
		else {
			img_id = atoi(cgi_query + 3);
		}
	}
	else {
		std::cerr << "Put this program into a web server's cgi-bin "
				"directory, then" << std::endl;
		std::cerr << "invoke it with a URL like this:" << std::endl;
		std::cerr << std::endl;
		std::cerr << "    http://server.name.com/cgi-bin/cgi_jpeg?id=2" <<
				std::endl;
		std::cerr << std::endl;
		std::cerr << "This will retrieve the image with ID 2." << std::endl;
		std::cerr << std::endl;
		std::cerr << "You will probably have to change some of the #defines "
				"at the top of" << std::endl;
		std::cerr << "examples/cgi_jpeg.cpp to allow the lookup to work." <<
				std::endl;
		return 1;
	}

	try {
		mysqlpp::Connection con(IMG_DATABASE, IMG_HOST, IMG_USER,
				IMG_PASSWORD);
		mysqlpp::Query query = con.query();
		query << "SELECT * FROM images WHERE id = " << img_id;
		mysqlpp::UseQueryResult res = query.use();
		if (res) {
			images img = res.fetch_row();
			std::cout << "Content-type: image/jpeg" << std::endl;
			std::cout << "Content-length: " << img.data.length() << "\n\n";
			std::cout << img.data;
		}
		else {
			std::cout << "Content-type: text/plain" << std::endl << std::endl;
			std::cout << "ERROR: No such image with ID " << img_id << std::endl;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		std::cout << "Content-type: text/plain" << std::endl << std::endl;
		std::cout << "QUERY ERROR: " << er.what() << std::endl;
		return 1;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		std::cout << "Content-type: text/plain" << std::endl << std::endl;
		std::cout << "GENERAL ERROR: " << er.what() << std::endl;
		return 1;
	}

	return 0;
}
