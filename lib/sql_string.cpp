/***********************************************************************
 sql_string.cpp - Implements the SQLString template.

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

#include "sql_string.h"

#include <sstream>

using namespace std;

namespace mysqlpp {

SQLString::SQLString() :
is_string(false),
dont_escape(false),
processed(false)
{
}

SQLString::SQLString(const string& str) :
string(str),
is_string(true),
dont_escape(false),
processed(false)
{
}

SQLString::SQLString(const char* str) : 
string(str),
is_string(true),
dont_escape(false),
processed(false)
{
}

SQLString::SQLString(char i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << static_cast<short int>(i);
	assign(outs.str());
}

SQLString::SQLString(unsigned char i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << static_cast<unsigned short int>(i);
	assign(outs.str());
}

SQLString::SQLString(short int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(unsigned short int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(unsigned int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(longlong i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(ulonglong i) :
is_string(false),
dont_escape(false),
processed(false) 
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(float f) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << f;
	assign(outs.str());
}

SQLString::SQLString(double f) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << f;
	assign(outs.str());
}

} // end namespace mysqlpp

