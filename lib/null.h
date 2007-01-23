/// \file null.h
/// \brief Declares classes that implement SQL "null" semantics within
/// C++'s type system.
///
/// This is required because C++'s own NULL type is not semantically
/// the same as SQL nulls.

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

#ifndef MYSQLPP_NULL_H
#define MYSQLPP_NULL_H

#include "exceptions.h"

#include <iostream>

namespace mysqlpp {


/// \brief The type of the global mysqlpp::null object.
///
/// This class is for internal use only.  Normal code should use
/// Null instead.
class MYSQLPP_EXPORT null_type
{
public:
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	template <class Type> operator Type()
	{
		throw BadNullConversion();
		return Type();
	}
#endif // !defined(DOXYGEN_IGNORE)
};

/// \brief Global 'null' instance.  Use wherever you need a SQL null.
/// (As opposed to a C++ language null pointer or null character.)
const null_type null = null_type();


/// \brief Class for objects that define SQL null in terms of
/// MySQL++'s null_type.
///
/// Returns a null_type instance when you ask what null is, and is
/// "(NULL)" when you insert it into a C++ stream.
///
/// Used for the behavior parameter for template Null
struct NullisNull
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	static null_type null_is() { return null_type(); }

	static std::ostream& null_ostr(std::ostream& o)
	{
		o << "(NULL)";
		return o;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Class for objects that define SQL null as 0.
///
/// Returns 0 when you ask what null is, and is zero when you insert it
/// into a C++ stream.
///
/// Used for the behavior parameter for template Null
struct NullisZero
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	static int null_is() { return 0; }
	
	static std::ostream& null_ostr(std::ostream& o)
	{
		o << 0;
		return o;
	}
#endif // !defined(DOXYGEN_IGNORE)
};

/// \brief Class for objects that define SQL null as a blank C string.
///
/// Returns "" when you ask what null is, and is empty when you insert
/// it into a C++ stream.
///
/// Used for the behavior parameter for template Null
struct NullisBlank
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	static const char *null_is() { return ""; }
	
	static std::ostream& null_ostr(std::ostream& o)
	{
		o << "";
		return o;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Class for holding data from a SQL column with the NULL
/// attribute.
///
/// This template is necessary because there is nothing in the C++ type
/// system with the same semantics as SQL's null.  In SQL, a column can
/// have the optional 'NULL' attribute, so there is a difference in 
/// type between, say an \c int column that can be null and one that
/// cannot be.  C++'s NULL constant does not have these features.
///
/// It's important to realize that this class doesn't hold nulls,
/// it holds data that \e can \e be null.  It can hold a non-null
/// value, you can then assign null to it (using MySQL++'s global
/// \c null object), and then assign a regular value to it again; the
/// object will behave as you expect throughout this process.
///
/// Because one of the template parameters is a C++ type, the typeid()
/// for a null \c int is different than for a null \c string, to pick
/// two random examples.  See type_info.cpp for the table SQL types that
/// can be null.
template <class Type, class Behavior = NullisNull> class Null
{
public:
	/// \brief The object's value, when it is not SQL null
	Type data;
	
	/// \brief If set, this object is considered equal to SQL null.
	///
	/// This flag affects how the Type() and << operators work.
	bool is_null;

	/// \brief Type of the data stored in this object, when it is not
	/// equal to SQL null.
	typedef Type value_type;

	/// \brief Default constructor
	///
	/// "data" member is left uninitialized by this ctor, because we
	/// don't know what to initialize it to.
	Null() :
	is_null(false)
	{
	}

	/// \brief Initialize the object with a particular value.
	///
	/// The object is marked as "not null" if you use this ctor.  This
	/// behavior exists because the class doesn't encode nulls, but
	/// rather data which \e can \e be null.  The distinction is
	/// necessary because 'NULL' is an optional attribute of SQL
	/// columns.
	Null(const Type& x) :
	data(x),
	is_null(false)
	{
	}

	/// \brief Construct a Null equal to SQL null
	///
	/// This is typically used with the global \c null object. (Not to
	/// be confused with C's NULL type.)  You can say something like...
	/// \code
	/// Null<int> foo = null;
	/// \endcode
	/// ...to get a null \c int.
	Null(const null_type& n) :
	is_null(true)
	{
	}

	/// \brief Converts this object to Type
	///
	/// If is_null is set, returns whatever we consider that null "is",
	/// according to the Behavior parameter you used when instantiating
	/// this template.  See NullisNull, NullisZero and NullisBlank.
	///
	/// Otherwise, just returns the 'data' member.
	operator Type&()
	{
		if (is_null)
			return data = Behavior::null_is();
		else
			return data;
	}

	/// \brief Assign a value to the object.
	///
	/// This marks the object as "not null" as a side effect.
	Null& operator =(const Type& x)
	{
		data = x;
		is_null = false;
		return *this;
	}

	/// \brief Assign SQL null to this object.
	///
	/// This just sets the is_null flag; the data member is not
	/// affected until you call the Type() operator on it.
	Null& operator =(const null_type& n)
	{
		is_null = true;
		return *this;
	}
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

// Specialization the Null template for \c void
template <> class Null<void>
{
public:
	bool is_null;
	typedef void value_type;

	Null() :
	is_null(false)
	{
	}
	
	Null(const null_type&) :
	is_null(true)
	{
	}

	Null& operator =(const null_type&)
	{
		is_null = true;
		return *this;
	}
};

#endif // !defined(DOXYGEN_IGNORE)


/// \brief Inserts null-able data into a C++ stream if it is not
/// actually null.  Otherwise, insert something appropriate for null
/// data.
template <class Type, class Behavior>
inline std::ostream& operator <<(std::ostream& o,
		const Null<Type, Behavior>& n)
{
	if (n.is_null)
		return Behavior::null_ostr(o);
	else
		return o << n.data;
}

} // end namespace mysqlpp

#endif
