/***********************************************************************
 util.h - Declares functions and such required by several of the
 	example programs.

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

#if !defined(MYSQLPP_UTIL_H)
#define MYSQLPP_UTIL_H

#include <mysql++.h>

extern const char* kpcSampleDatabase;

void print_stock_header(int rows);
void print_stock_row(const mysqlpp::Row& r);
void print_stock_row(const mysqlpp::sql_char& item,
		mysqlpp::sql_bigint num, mysqlpp::sql_double weight,
		mysqlpp::sql_double price, const mysqlpp::sql_date& date);
void print_stock_rows(mysqlpp::Result& res);
void print_stock_table(mysqlpp::Query& query);
void get_stock_table(mysqlpp::Query& query, mysqlpp::Result& res);
void print_usage(const char* program_name,
		const char* extra_parms = "");
bool connect_to_db(int argc, char *argv[], mysqlpp::Connection& con,
		const char* kdb = 0);

#endif // !defined(MYSQLPP_UTIL_H)

