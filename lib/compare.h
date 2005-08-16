/// \file compare.h
/// 
/// Declares several function objects and templates for creating
/// function objects for comparing various things.  These are useful
/// when using STL algorithms like std::find_if() on containers
/// of data retreived from a database with MySQL++.

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

#ifndef MYSQLPP_COMPARE_H
#define MYSQLPP_COMPARE_H

#include "row.h"

#include <cstring>
#include <functional>

namespace mysqlpp {

/// \brief Template for making function objects that can compare
/// something against a Row element.
///
/// \sa mysql_cmp

template <class BinaryPred, class CmpType>
class MysqlCmp : public std::unary_function<const Row&, bool>
{
protected:
	/// \brief Index of field within Row object to compare against
	unsigned int index;

	/// \brief Predicate function to use for the comparison
	BinaryPred func;

	/// \brief What to compare the Row's field against
	CmpType cmp2;
	
public:
	/// \brief MysqlCmp constructor
	///
	/// \param i field number within a row to compare against
	/// \param f predicate function
	/// \param c what to compare row element against
	///
	/// operator() for this object compares Row[i] to c using f.
	MysqlCmp(uint i, const BinaryPred& f,
			const CmpType& c) :
	index(i),
	func(f),
	cmp2(c)
	{
	}
	
	/// \brief Run the predicate function on this row and the object's
	/// data, and return its value.
	///
	/// See the constructor's parameters for what we compare against.
	bool operator ()(const Row& cmp1) const
	{
		return func(cmp2, cmp1[this->index]);
	}
};


/// \brief const char* specialization of MysqlCmp
///
/// \sa mysql_cmp_cstr

template <class BinaryPred>
class MysqlCmpCStr : public MysqlCmp<BinaryPred, const char*>
{
public:
	/// \brief MysqlCmpCStr constructor
	///
	/// \param i field number within a row to compare against
	/// \param f predicate function
	/// \param c what to compare row element against
	///
	/// operator() for this object compares Row[i] to c using f.
	MysqlCmpCStr(uint i, const BinaryPred& f,
			const char *c) :
	MysqlCmp<BinaryPred, const char*>(i, f, c)
	{
	}
	
	/// \brief Run the predicate function on this row and the object's
	/// data, and return its value.
	///
	/// See the constructor's parameters for what we compare against.
	bool operator ()(const Row& cmp1) const
	{
		return MysqlCmp<BinaryPred, const char*>::func(
				MysqlCmp<BinaryPred, const char*>::cmp2,
				cmp1[this->index]);
	}
};


/// \brief Template for function objects that compare any two objects,
/// as long as they can be converted to SQLString.
///
/// This is a more generic form of mysql_cmp_cstr(), and is therefore
/// less efficient.  Use this form only when necessary.
///
/// \param i field index number
/// \param func one of the functors in compare.h, or any compatible functor
/// \param cmp2 what to compare to
template <class BinaryPred, class CmpType> MysqlCmp<BinaryPred, CmpType>
mysql_cmp(uint i, const BinaryPred& func, const CmpType& cmp2)
{
	return MysqlCmp<BinaryPred, CmpType>(i, func, cmp2);
}


/// \brief Template for function objects that compare any two things
/// that can be converted to <tt>const char*</tt>.
///
/// \param i field index number
/// \param func one of cstr_equal_to, cstr_not_equal_to,
///     cstr_less, cstr_less_equal, cstr_less_equal, or
///     cstr_less_equal.
/// \param cmp2 what to compare to
///
/// \sa mysql_cmp()
template <class BinaryPred> MysqlCmpCStr<BinaryPred>
mysql_cmp_cstr(uint i, const BinaryPred& func, const char* cmp2)
{
	return MysqlCmpCStr<BinaryPred>(i, func, cmp2);
}


/// \brief Base class for the other predicate types defined in compare.h

typedef std::binary_function<const char*, const char*, bool>
		bin_char_pred;


/// \brief Function object that returns true if one const char* is equal
/// to another.
struct cstr_equal_to : bin_char_pred
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	bool operator ()(const char* x, const char* y) const
	{
		return !std::strcmp(x, y);
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Function object that returns true if one const char* is not
/// equal to another.
struct cstr_not_equal_to : bin_char_pred
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	bool operator ()(const char* x, const char* y) const
	{
		return std::strcmp(x, y) != 0;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Function object that returns true if one const char* is
/// lexically "less than" another.
struct cstr_less : bin_char_pred
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	bool operator ()(const char* x, const char* y) const
	{
		return std::strcmp(x, y) > 0;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Function object that returns true if one const char* is
/// lexically "less than or equal to" another.
struct cstr_less_equal : bin_char_pred
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	bool operator ()(const char* x, const char* y) const
	{
		return std::strcmp(x, y) >= 0;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Function object that returns true if one const char* is
/// lexically "greater than" another.
struct cstr_greater : bin_char_pred
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	bool operator ()(const char* x, const char* y) const
	{
		return std::strcmp(x, y) < 0;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


/// \brief Function object that returns true if one const char* is
/// lexically "greater than or equal to" another.
struct cstr_greater_equal : bin_char_pred
{
#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	bool operator ()(const char* x, const char* y) const
	{
		return std::strcmp(x, y) <= 0;
	}
#endif // !defined(DOXYGEN_IGNORE)
};


} // end namespace mysqlpp

#endif
