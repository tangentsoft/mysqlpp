/***********************************************************************
 query.cpp - Implements the Query class.

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

#include "query.h"

namespace mysqlpp {

Query::Query(const Query& q) :
SQLQuery(q),
mysql(q.mysql),
throw_exceptions(q.throw_exceptions)
{
}

bool Query::exec(const std::string& str)
{
	return mysql->exec(str);
}

bool Query::success()
{
	if (Success) {
		return mysql->success();
	}
	else {
		return false;
	}
}

/// \if INTERNAL
// Doxygen will not generate documentation for this section.

ResNSel Query::execute(const char* str)
{
	return mysql->execute(str);
}

ResNSel Query::execute(parms& p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return mysql->execute(str(p, r));
}

ResUse Query::use(const char* str)
{
	return mysql->use(str);
}

ResUse Query::use(parms& p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return mysql->use(str(p, r));
}

Result Query::store(const char* str)
{
	return mysql->store(str);
}

Result Query::store(parms&  p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return mysql->store(str(p, r));
}

/// \endif

my_ulonglong Query::affected_rows() const
{
	return mysql->affected_rows();
}

my_ulonglong Query::insert_id()
{
	return mysql->insert_id();
}

std::string Query::info()
{
	return mysql->info();
}

std::string Query::error()
{
	if (errmsg) {
		return std::string(errmsg);
	}
	else {
		return mysql->error();
	}
}

bool Query::lock()
{
    return mysql->lock();
}

void Query::unlock()
{
	mysql->unlock();
}

} // end namespace mysqlpp

