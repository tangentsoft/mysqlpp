/***********************************************************************
 result.cpp - Implements the Result, ResNSel, and ResUse classes.

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

#include "result.h"

#include "connection.h"

namespace mysqlpp {

ResNSel::ResNSel(Connection* q) :
success(q->success()),
insert_id(q->insert_id()),
rows(q->affected_rows()),
info(q->info())
{
}

ResUse::ResUse(MYSQL_RES* result, Connection* m, bool te) :
mysql(m),
throw_exceptions(te),
initialized(false),
_fields(this)
{
	if (!result) {
		mysql_res = 0;
		_types = 0;
		_names = 0;
		return;
	}
	mysql_res = result;
	_names = new FieldNames(this);
	if (_names) {
		_types = new FieldTypes(this);
	}
	_table = fields(0).table;
	initialized = true;
}

ResUse::~ResUse()
{
	if (mysql) {
		mysql->unlock();
	}
	purge();
}

void ResUse::copy(const ResUse& other)
{
	if (!other.mysql_res) {
		mysql_res = 0;
		_types = 0;
		_names = 0;
		return;
	}

	if (initialized) {
		purge();
	}

	throw_exceptions = other.throw_exceptions;
	mysql_res = other.mysql_res;
	_fields = other._fields;

	if (other._names) {
		_names = new FieldNames(*other._names);
	}
	else {
		_names = 0;
	}
	
	if (other._types) {
		_types = new FieldTypes(*other._types);
	}
	else {
		_types = 0;
	}

	_table = other._table;

	mysql = other.mysql;
	initialized = true;
}

} // end namespace mysqlpp

