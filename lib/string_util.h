#ifndef MYSQLPP_STRING_UTIL_H
#define MYSQLPP_STRING_UTIL_H

#include "defs.h"

#include <ctype.h>

#include <string>

namespace mysqlpp {

// Strips blanks at left and right ends
extern void strip(std::string& s);

// C++ equivalent of mysql_escape_string()
extern void escape_string(std::string& s);

// Changes case of string to upper
inline void str_to_upr(std::string& s)
{
	for (unsigned int cnt=0; cnt < s.length(); cnt++) {
		char c = s[cnt]; s[cnt]=toupper(c);
	}
}

// Changes case of string to lower
inline void str_to_lwr (std::string& s)
{
	for (unsigned int cnt=0; cnt < s.length(); cnt++) {
		char c = s[cnt]; s[cnt]=tolower(c);
	}
}

// Removes all blanks
inline void strip_all_blanks (std::string& s)
{
	for (unsigned int counter=0;counter < s.size();counter++)
		if (s[counter] == ' ') { s.erase(counter,1); counter--;}
}

// Removes all non-numerics
inline void strip_all_non_num (std::string& s)
{
	for (unsigned int counter=0;counter < s.size();counter++)
		if (!isdigit(s[counter])) { s.erase(counter,1); counter--;}
}

} // end namespace mysqlpp

#endif

