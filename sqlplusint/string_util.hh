#ifndef __STRING_UTIL1__
#define __STRING_UTIL1__
#include <ctype.h>
#include <string>

extern void strip (string& s); // Strips blanks at left and right ends

extern void escape_string (string& s); // C++ equivalent of mysql_escape_string

inline void str_to_upr (string& s) { // Changes case of string to upper
	for (unsigned int cnt=0; cnt < s.length(); cnt++) {
		char c = s[cnt]; s[cnt]=toupper(c);
	}
}

inline void str_to_lwr (string& s) { // Changes case of string to lower
	for (unsigned int cnt=0; cnt < s.length(); cnt++) {
		char c = s[cnt]; s[cnt]=tolower(c);
	}
}

inline void strip_all_blanks (string& s) { // Removes all blanks
	for (unsigned int counter=0;counter < s.size();counter++)
		if (s[counter] == ' ') { s.erase(counter,1); counter--;}
}

inline void strip_all_non_num (string& s) { // Removes all non-numerics
	for (unsigned int counter=0;counter < s.size();counter++)
		if (!isdigit(s[counter])) { s.erase(counter,1); counter--;}
}


#endif
