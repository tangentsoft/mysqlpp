/***********************************************************************
 field_names.cpp - Implements the FieldNames class.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2007 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS file in
 the top directory of the distribution for details.

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

#define MYSQLPP_NOT_HEADER
#include "common.h"

#include "field_names.h"
#include "result.h"

#include <algorithm>

namespace mysqlpp {

void 
FieldNames::init(const ResultBase* res)
{
	int num = res->num_fields();
	reserve(num);

	for (int i = 0; i < num; i++) {
		std::string p(res->fields().at(i).name());
		str_to_lwr(p);
		push_back(p);
	}
}


unsigned int
FieldNames::operator [](const std::string& s) const
{
	std::string temp(s);
	str_to_lwr(temp);
	return static_cast<unsigned int>(std::find(begin(), end(), temp) - begin());
}

} // end namespace mysqlpp
