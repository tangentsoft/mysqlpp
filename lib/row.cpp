/***********************************************************************
 row.cpp - Implements the Row class.

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

#include "row.h"
#include "result.h"
#include "exceptions.h"

namespace mysqlpp {

Row::size_type Row::size() const
{
	return res->num_fields();
}

const ColData Row::operator [](size_type i) const
{
	if (!initialized) {
		if (throw_exceptions)
			throw BadQuery("Row not initialized");
		else
			return ColData();
	}
	
	return ColData(data.at(i).c_str(), res->types(i), is_nulls[i]);
}

const ColData Row::lookup_by_name(const char* i) const
{
	int si = res->field_num(std::string(i));
	if (si < res->num_fields()) {
		return (*this)[si];
	}
	else {
		throw BadFieldName(i);
	}
}

} // end namespace mysqlpp

