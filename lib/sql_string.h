#ifndef MYSQLPP_SQL_STRING_H
#define MYSQLPP_SQL_STRING_H

/// \file sql_string.h
/// \brief Declares an \c std::string derivative that adds some things
/// needed within the library.
///
/// This class adds some flags needed by other parts of MySQL++, and it
/// adds conversion functions from any primitive type.  This helps in
/// inserting these primitive types into the database, because we need
/// everything in string form to build SQL queries.

#include "defs.h"

#include <stdio.h>
#include <string>

namespace mysqlpp {

/// \brief A specialized \c std::string that will convert from any
/// valid MySQL type.

class SQLString : public std::string {
public:
	bool is_string;
	bool dont_escape;
	bool processed;

	SQLString();
	SQLString(const std::string& str);
	SQLString(const char* str);
	SQLString(char i);
	SQLString(unsigned char i);
	SQLString(short int i);
	SQLString(unsigned short int i);
	SQLString(int i);
	SQLString(unsigned int i);
	SQLString(longlong i);
	SQLString(ulonglong i);
	SQLString(float i);
	SQLString(double i);

	SQLString& operator =(const char* str) {
		std::string::operator =(str);
		return *this;
	}

	SQLString& operator =(const std::string& str) {
		std::string::operator =(str);
		return *this;
	}
};

} // end namespace mysqlpp

#endif

