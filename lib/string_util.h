#ifndef MYSQLPP_STRING_UTIL_H
#define MYSQLPP_STRING_UTIL_H

#include <ctype.h>
#include <string>

namespace mysqlpp {

extern void strip (std::string& s); // Strips blanks at left and right ends

extern void escape_string (std::string& s); // C++ equivalent of mysql_escape_string

inline void str_to_upr (std::string& s) { // Changes case of string to upper
	for (unsigned int cnt=0; cnt < s.length(); cnt++) {
		char c = s[cnt]; s[cnt]=toupper(c);
	}
}

inline void str_to_lwr (std::string& s) { // Changes case of string to lower
	for (unsigned int cnt=0; cnt < s.length(); cnt++) {
		char c = s[cnt]; s[cnt]=tolower(c);
	}
}

inline void strip_all_blanks (std::string& s) { // Removes all blanks
	for (unsigned int counter=0;counter < s.size();counter++)
		if (s[counter] == ' ') { s.erase(counter,1); counter--;}
}

inline void strip_all_non_num (std::string& s) { // Removes all non-numerics
	for (unsigned int counter=0;counter < s.size();counter++)
		if (!isdigit(s[counter])) { s.erase(counter,1); counter--;}
}

}; // end namespace mysqlpp

#endif

