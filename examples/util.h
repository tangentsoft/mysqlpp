#ifndef _util_hh_
#define _util_hh_

#include <mysql++.h>

extern const char* kpcSampleDatabase;

void print_stock_table(mysqlpp::Query& query);
bool connect_to_db(int argc, char *argv[], mysqlpp::Connection& con,
		const char* kdb = 0);

#endif
