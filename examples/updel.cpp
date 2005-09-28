/***********************************************************************
 updel.cpp - Example showing how to UPDATE or DELETE rows in a database
 	while using the power of MySQL's SELECT filtering ability.

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

#include <string>

using namespace std;
using namespace mysqlpp;

#define MY_DATABASE	"telcent"
#define MY_TABLE	"nazivi"
#define MY_HOST		"localhost"
#define MY_USER		"root"
#define MY_PASSWORD ""
#define MY_FIELD    "naziv"
#define MY_QUERY    "SELECT URL from my_table as t1, my_table as t2 where t1.field = t2.field"

int
main()
{
	Connection con(use_exceptions);
	try {
		ostringstream strbuf;
		unsigned int i = 0;
		con.connect(MY_DATABASE, MY_HOST, MY_USER, MY_PASSWORD);
		Query query = con.query();
		query << MY_QUERY;
		ResUse res = query.use();
		Row row;
		strbuf << "delete from " << MY_TABLE << " where " << MY_FIELD <<
			" in (";
		//  for UPDATE just replace the above DELETE FROM with UPDATE statement
		for (; row = res.fetch_row(); i++)
			strbuf << row.at(0) << ",";
		if (!i)
			return 0;
		string output(strbuf.str());
		output.erase(output.size() - 1, 1);
		output += ")";
		query.exec(output);
		//cout << output << endl;
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
