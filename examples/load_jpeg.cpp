/***********************************************************************
 load_jpeg.cpp - Example showing how to insert BLOB data into the
	database from a file.

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

#include "cmdline.h"
#include "images.h"
#include "printdata.h"

#include <fstream>

using namespace std;
using namespace mysqlpp;


// Pull in a state variable used by att_getopt() implementation so we
// can pick up where standard command line processing leaves off.  Feel
// free to ignore this implementation detail.
extern int ag_optind;


static bool
is_jpeg(const char* img_data)
{
	const unsigned char* idp =
			reinterpret_cast<const unsigned char*>(img_data);
	return (idp[0] == 0xFF) && (idp[1] == 0xD8) &&
			((memcmp(idp + 6, "JFIF", 4) == 0) ||
			 (memcmp(idp + 6, "Exif", 4) == 0));
}


int
main(int argc, char *argv[])
{
	// Get database access parameters from command line
	const char* db = 0, *server = 0, *user = 0, *pass = "";
	if (!parse_command_line(argc, argv, &db, &server, &user, &pass,
			"[jpeg_file]")) {
		return 1;
	}

	try {
		// Establish the connection to the database server.
		mysqlpp::Connection con(db, server, user, pass);

		// Try to create a new item in the images table, based on what
		// we got on the command line.
		images img(mysqlpp::null, mysqlpp::null);
		const char* img_name = "NULL";
		if (argc - ag_optind >= 1) {
			// We received at least one non-option argument on the
			// command line, so treat it as a file name 
			img_name = argv[ag_optind];
			ifstream img_file(img_name, ios::ate | ios::binary);
			if (img_file) {
				size_t img_size = img_file.tellg();
				if (img_size > 10) {
					img_file.seekg(0, ios::beg);
					char* img_buffer = new char[img_size];
					img_file.read(img_buffer, img_size);
					if (is_jpeg(img_buffer)) {
						img.data = mysqlpp::sql_blob(img_buffer, img_size);
					}
					else {
						cerr << '"' << img_file <<
								"\" isn't a JPEG!" << endl;
					}
					delete[] img_buffer;
				}
				else {
					cerr << "File is too short to be a JPEG!" << endl;
				}
			}

			if (img.data.data.empty()) {
				// File name was bad, or file contents aren't JPEG.  
				print_usage(argv[0], "[jpeg_file]");
				return 1;
			}
		}
		// else, no image given on command line, so insert SQL NULL
		// instead of image data.  NULL BLOB columns in SSQLS is
		// legal as of MySQL++ v3.0.7.

		// Insert image data or SQL NULL into the images.data BLOB
		// column.  By inserting it as an SSQLS with a mysqlpp::sql_blob
		// member, we avoid truncating it at the first embedded C null
		// character ('\0'), as would happen if we used the raw
		// character buffer we allocated above.
		Query query = con.query();
		query.insert(img);
		SimpleResult res = query.execute();

		// Report successful insertion
		cout << "Inserted \"" << img_name <<
				"\" into images table, " << img.data.data.size() <<
				" bytes, ID " << res.insert_id() << endl;
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
