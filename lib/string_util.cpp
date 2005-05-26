/***********************************************************************
 string_util.cpp - Implements utility functions for manipulating
	C++ strings.

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

#include "string_util.h"

namespace mysqlpp {

void strip(std::string& s)
{
	size_t i, j = s.size() - 1;
	if (!s.size()) {
		return;
	}
	for (i = 0; s[i] == ' '; i++) ;
	if (i) {
		s.erase(0, i);
	}
	j = s.size();
	if (!j) {
		return;
	}
	j--;
	for (i = j; i && s[i] == ' '; i--) ;
	if (i != j) {
		s.erase(i + 1, static_cast<size_t> (-1));
	}
}

void escape_string(std::string & s)
{
	if (!s.size()) {
		return;
	}

	for (unsigned int i = 0; i < s.size(); i++) {
		switch (s[i]) {
			case '\0':		// Must be escaped for "mysql"
				s[i] = '\\';
				s.insert(i, "0", 1);
				i++;
				break;
			case '\n':		// Must be escaped for logs
				s[i] = '\\';
				s.insert(i, "n", 1);
				i++;
				break;
			case '\r':
				s[i] = '\\';
				s.insert(i, "r", 1);
				i++;
				break;
			case '\\':
				s[i] = '\\';
				s.insert(i, "\\", 1);
				i++;
				break;
			case '\"':
				s[i] = '\\';
				s.insert(i, "\"", 1);
				i++;
				break;
			case '\'':		// Better safe than sorry
				s[i] = '\\';
				s.insert(i, "\'", 1);
				i++;
				break;
			case '\032':	// This gives problems on Win32
				s[i] = '\\';
				s.insert(i, "Z", 1);
				i++;
				break;
			default:
				break;
		}
	}
}

} // end namespace mysqlpp

