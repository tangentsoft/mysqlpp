/***********************************************************************
 multiquery.cpp - Example showing how to iterate over result sets upon
    execution of a query that returns more than one result set.  You can
	get multiple result sets when executing multiple separate SQL
	statments in a single query, or when dealing with the results of
	calling a stored procedure.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, (c) 2004, 2005 by Educational Technology Resources, Inc.,
 and (c) 2005 by Arnon Jalon.  Others may also hold copyrights on
 code in this file.  See the CREDITS file in the top directory of
 the distribution for details.

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

#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
using namespace mysqlpp;


typedef vector<int> IntVectorType;


static void
print_header(IntVectorType& widths, Result& res)
{
	cout << "  |" << setfill(' ');
	for (size_t i = 0; i < res.names().size(); i++) {
		cout << " " << setw(widths.at(i)) << res.names(i) << " |";
	}
	cout << endl;
}


static void
print_row(IntVectorType& widths, Row& row)
{
	cout << "  |" << setfill(' ');
	for (size_t i = 0; i < row.size(); i++) {
		cout << " " << setw(widths.at(i)) << row.raw_data(i) << " |";
	}
	cout << endl;
}


static void
print_row_separator(IntVectorType& widths)
{
	cout << "  +" << setfill('-');
	for (size_t i = 0; i < widths.size(); i++) {
		cout << "-" << setw(widths.at(i)) << '-' << "-+";
	}
	cout << endl;
}


static void
print_result(Result& res, int index)
{
	// Show how many rows are in result, if any
	int num_results = res.size();
	if (res && (num_results > 0)) {
		cout << "Result set " << index << " has " << num_results <<
				" row" << (num_results == 1 ? "" : "s") << ':' << endl;
	}
	else {
		cout << "Result set " << index << " is empty." << endl;
		return;
	}

	// Figure out the widths of the result set's columns
	IntVectorType widths;
	int size = res.columns();
	for (int i = 0; i < size; i++) {
		mysql_type_info mti(res.fields(i));
		widths.push_back((res.names(i).size() > mti.max_length()) ?
				res.names(i).size() : mti.max_length());
	}

	// Print result set header
	print_row_separator(widths);
	print_header(widths, res);
	print_row_separator(widths);

	// Display the result set contents
	for (int i = 0; i < num_results; ++i) {
		Row row = res.fetch_row();
		print_row(widths, row);
	}

	// Print result set footer
	print_row_separator(widths);
}


static void
print_multiple_results(Query& query)
{
	try {
		// Execute query and print all result sets
		Result res = query.store();
		print_result(res, 0);
		for (int i = 1; query.more_results(); ++i) {
			res = query.store_next();
			print_result(res, i);
		}
	}
	catch (Exception& err) {
		// Something bad happened....
		cerr << "Multi-query failure: " << err.what() << endl;
		exit(1);
	}
}


int
main(int argc, char *argv[])
{
	Connection con;
	try {
		// Enable multi-queries.  Notice that we can set connection
		// options before the connection is established, which the
		// underlying MySQL C API does not allow.  In this particular
		// case, this is not a mere nicety: the multi-query option has
		// a side effect of setting one of the flags used when 
		// establishing the database server connection.  We could set it
		// directly, but then we couldn't use connect_to_db().
		con.set_option(Connection::opt_multi_statements, true);

		// Connect to database
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}
	}
	catch (const BadOption& err) {
		if (err.what_option() == Connection::opt_multi_statements) {
			cerr << "This example only works when MySQL++ is built "
					"against MySQL C API" << endl;
			cerr << "version 4.1.01 or later." << endl;
		}
		else {
			cerr << "Unexpected option failure: " << err.what() << endl;
		}
		return 1;
	}
	catch (const ConnectionFailed& err) {
		cerr << "Failed to connect to database server: " <<
				err.what() << endl;
		return 1;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return 1;
	}

	// Set up query with multiple queries.
	Query query = con.query();
	query << "DROP TABLE IF EXISTS test_table;" << endl <<
			"CREATE TABLE test_table(id INT);" << endl <<
			"INSERT INTO test_table VALUES(10);" << endl <<
			"UPDATE test_table SET id=20 WHERE id=10;" << endl <<
			"SELECT * FROM test_table;" << endl <<
			"DROP TABLE test_table" << endl;
	cout << "Multi-query: " << endl << query.preview() << endl;

	// Execute statement and display all result sets.
	print_multiple_results(query);

#if MYSQL_VERSION_ID >= 50000
	// If it's MySQL v5.0 or higher, also test stored procedures, which
	// return their results the same way multi-queries do.
	query.reset();
	query << "DROP PROCEDURE IF EXISTS get_stock;" << endl <<
			"CREATE PROCEDURE get_stock" << endl <<
			"( i_item varchar(20) )" << endl <<
			"BEGIN" << endl <<
			"SET i_item = concat('%', i_item, '%');" << endl <<
			"SELECT * FROM stock WHERE lower(item) like lower(i_item);" << endl <<
			"END" << endl <<
			";";
	cout << "Stored procedure query: " << endl << query.preview() << endl;

	// Create the stored procedure.
	print_multiple_results(query);

	// Call the stored procedure and display its results.
	query.reset();
	query << "CALL get_stock('relish')";
	cout << "Query: " << query.preview() << endl;
	print_multiple_results(query);
#endif

	return 0;
}
