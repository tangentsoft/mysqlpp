/***********************************************************************
 load_jpeg.cpp - Example showing how to insert BLOB data into the
	database from a file.

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

#include "cmdline.h"
#include "printdata.h"

#include <mysql++.h>

#include <fstream>

using namespace std;
using namespace mysqlpp;


// Pull in a state variable used by att_getopt() implementation so we
// can pick up where standard command line processing leaves off.  Feel
// free to ignore this implementation detail.
extern int ag_optind;


static bool
is_jpeg(const unsigned char* img_data)
{
	return (img_data[0] == 0xFF) && (img_data[1] == 0xD8) &&
			((memcmp(img_data + 6, "JFIF", 4) == 0) ||
			 (memcmp(img_data + 6, "Exif", 4) == 0));
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

		// Assume that the last command line argument is a file.  Try
		// to read that file's data into img_data, and check it to see
		// if it appears to be a JPEG file.  Bail otherwise.
		string img_name, img_data;
		if (argc - ag_optind >= 1) {
			img_name = argv[ag_optind];
			ifstream img_file(img_name.c_str(), ios::ate);
			if (img_file) {
				size_t img_size = img_file.tellg();
				if (img_size > 10) {
					img_file.seekg(0, ios::beg);
					unsigned char* img_buffer = new unsigned char[img_size];
					img_file.read(reinterpret_cast<char*>(img_buffer),
							img_size);
					if (is_jpeg(img_buffer)) {
						img_data.assign(
								reinterpret_cast<char*>(img_buffer),
								img_size);
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
		}
		if (img_data.empty()) {
			print_usage(argv[0], "[jpeg_file]");
			return 1;
		}

		// Insert image data into the BLOB column in the images table.
		// We're inserting it as an std::string instead of using the raw
		// data buffer allocated above because we don't want the data
		// treated as a C string, which would truncate the data at the
		// first null character.
		Query query = con.query();
		query << "INSERT INTO images (data) VALUES(\"" <<
				mysqlpp::escape << img_data << "\")";
		SimpleResult res = query.execute();

		// If we get here, insertion succeeded
		cout << "Inserted \"" << img_name <<
				"\" into images table, " << img_data.size() <<
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
