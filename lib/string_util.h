/// \file string_util.h
/// \brief Declares string-handling utility functions used within
/// the library.

/***********************************************************************
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

#ifndef MYSQLPP_STRING_UTIL_H
#define MYSQLPP_STRING_UTIL_H

#include "defs.h"

#include <ctype.h>

#include <string>

namespace mysqlpp {

/// \brief Strips blanks at left and right ends
extern void strip(std::string& s);

/// \brief C++ equivalent of mysql_escape_string()
extern void escape_string(std::string& s);

/// \brief Changes case of string to upper
inline void str_to_upr(std::string& s)
{
	for (unsigned int cnt=0; cnt < s.length(); cnt++) {
		char c = s[cnt]; s[cnt]=toupper(c);
	}
}

/// \brief Changes case of string to lower
inline void str_to_lwr (std::string& s)
{
	for (unsigned int cnt=0; cnt < s.length(); cnt++) {
		char c = s[cnt]; s[cnt]=tolower(c);
	}
}

/// \brief Removes all blanks
inline void strip_all_blanks (std::string& s)
{
	for (unsigned int counter=0;counter < s.size();counter++)
		if (s[counter] == ' ') { s.erase(counter,1); counter--;}
}

/// \brief Removes all non-numerics
inline void strip_all_non_num (std::string& s)
{
	for (unsigned int counter=0;counter < s.size();counter++)
		if (!isdigit(s[counter])) { s.erase(counter,1); counter--;}
}

} // end namespace mysqlpp

#endif

