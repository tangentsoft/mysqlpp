#ifndef MYSQLPP_SQL_STRING_H
#define MYSQLPP_SQL_STRING_H

#include "defs.h"

#include <stdio.h>
#include <string>

namespace mysqlpp {

// A specialized std::string that will convert from any valid MySQL type.

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

