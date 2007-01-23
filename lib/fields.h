/// \file fields.h
/// \brief Declares a class for holding information about a set of
/// fields.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004-2007 by Educational Technology Resources, Inc.
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

#ifndef MYSQLPP_FIELDS_H
#define MYSQLPP_FIELDS_H

#include "resiter.h"

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT ResUse;
#endif

/// \brief A container similar to \c std::vector for holding
/// mysqlpp::Field records.

class MYSQLPP_EXPORT Fields : public const_subscript_container<Fields, Field>
{
public:
	/// \brief Default constructor
	Fields() { }
	
	/// \brief Create a field list from a result set
	Fields(ResUse* r) :
	res_(r)
	{
	}

	/// \brief Returns a field given its index.
	const Field& at(Fields::size_type i) const;

	/// \brief Returns a field given its index.
	const Field& at(int i) const
	{
		return at(static_cast<size_type>(i));
	}

	size_type size() const;	///< get the number of fields

private:
	mutable ResUse* res_;
};

} // end namespace mysqlpp

#endif
