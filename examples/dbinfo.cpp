/***********************************************************************
 dbinfo.cpp - Example showing how to request information about the
	database schema, such as table names, column types, etc.

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

#include "util.h"

#include <mysql++.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace mysqlpp;

vector<string> yy;

static ostream&
separator(ostream& os)
{
	os << endl << "---------------------------" << endl << endl;
	return os;
}

int
main(int argc, char* argv[])
{
	Connection con(use_exceptions);
	try {
		connect_to_db(argc, argv, con, "");

		// Show MySQL version
		cout << "MySQL version: " << con.client_info() << separator;
		Query query = con.query();

		// Show all the databases we can see
		query << "show databases";
		cout << "Query: " << query.preview() << endl;

		Result res = query.store();
		cout << "Databases found: " << res.size();

		Row row;
		cout.setf(ios::left);
		Result::iterator i;
		for (i = res.begin(); i != res.end(); ++i) {
			row = *i;
			cout << endl << '\t' << setw(17) << row.at(0);
		}
		cout << separator;
		
		// Show the tables in the mysql database
		con.select_db("mysql");

		query.reset();
		query << "show tables";
		cout << "Query: " << query.preview() << endl;

		res = query.store();
		cout << "Tables found: " << res.size();

		cout.setf(ios::left);
		for (i = res.begin(); i != res.end(); ++i) {
			row = *i;
			string xx(row.at(0));
			cout << endl << '\t' << setw(17) << row.at(0);
			yy.push_back(xx);
		}
		cout << separator;

		// Show information about each of the tables we found
		for (unsigned int j = 0; j < yy.size(); ++j) {
			query.reset();
			query << "describe " << yy[j] << "";
			cout << "Query: " << query.preview() << endl;
			res = query.store();
			unsigned int columns = res.num_fields(), counter;
			vector<int> widths;
			for (counter = 0; counter < columns; counter++) {
				string s = res.names(counter);
				if (s.compare("field") == 0) {
					widths.push_back(22);
				}
				else if (s.compare("type") == 0) {
					widths.push_back(20);
				}
				else if (s.compare("null") == 0) {
					widths.push_back(4);
				}
				else if (s.compare("key") == 0) {
					widths.push_back(3);
				}
				else if (s.compare("extra") == 0) {
					widths.push_back(0);
				}
				else {
					widths.push_back(15);
				}

				if (widths[counter]) {
					cout << '|' << setw(widths[counter]) <<
							res.names(counter) << '|';
				}
			}
			cout << endl;

			for (i = res.begin(); i != res.end(); ++i) {
				row = *i;
				for (counter = 0; counter < columns; counter++) {
					if (widths[counter]) {
						cout << ' ' << setw(widths[counter]) <<
								row.at(counter) << ' ';
					}
				}
				cout << endl;
			}

			cout << separator;
		}

		// Show the user table contents
		query.reset();
	 	query << "select * from user";
		cout << "Query: " << query.preview() << endl << endl;

		res = query.store();
		int columns = res.num_fields();
		cout << "fields = " << res.num_fields() << ", rows = " <<
				res.size() << endl;
		volatile MYSQL_RES* ress = res.raw_result();
		if (!ress)
			return -1;
		for (i = res.begin(); i != res.end(); ++i) {
			row = *i;
			for (int counter = 0; counter < columns; counter++) {
				cout << row.at(counter) << "  ";
			}
			cout << endl;
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
