/// \file row.h
/// \brief Declares the classes for holding row data from a result set.

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

#ifndef MYSQLPP_ROW_H
#define MYSQLPP_ROW_H

#include "coldata.h"
#include "exceptions.h"
#include "resiter.h"
#include "vallist.h"

#include <vector>
#include <string>

#include <string.h>

namespace mysqlpp {

class FieldNames;
class ResUse;

/// \brief Base class for class Row

template <class ThisType, class Res>
class RowTemplate
{
protected:
	/// \brief Return a pointer to this object.
	///
	/// Not sure what value this has over the 'this' pointer, but...
	virtual ThisType& self() = 0;

	/// \brief Return a const pointer to this object, for calls in
	/// const context.
	virtual const ThisType& self() const = 0;

public:
	/// \brief Destroy object
	virtual ~RowTemplate() { }

	/// \brief Get a reference to our "parent" class.
	///
	/// The meaning of this function is up to whatever class derives
	/// from this one.
	virtual const Res& parent() const = 0;

	/// \brief Get a list of the values in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, and items will be quoted and escaped.
	value_list_ba<ThisType, quote_type0> value_list(
			const char* d = ",") const
	{
		return value_list_ba<ThisType, quote_type0>(self(), d, quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// \param d delimiter to use between values
	/// \param m manipulator to use when inserting values into a stream
	template <class Manip>
	value_list_ba<ThisType, Manip> value_list(const char *d,
			Manip m) const {
		return value_list_ba<ThisType, Manip>(self(), d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// \param d delimiter to use between values
	/// \param m manipulator to use when inserting values into a stream
	/// \param vb for each true item in this list, add that value to the
	/// returned list; ignore the others
	template <class Manip>
	value_list_b<ThisType, Manip> value_list(const char *d, Manip m,
			const std::vector<bool>& vb) const
	{
		return value_list_b<ThisType, Manip>(self(), vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// \param d delimiter to use between values
	/// \param vb for each true item in this list, add that value to the
	/// returned list; ignore the others
	///
	/// Items will be quoted and escaped when inserted into a C++
	/// stream.
	value_list_b<ThisType, quote_type0>
	value_list(const char* d, const std::vector<bool>& vb) const
	{
		return value_list_b<ThisType, quote_type0>(self(), vb, d, quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// \param vb for each true item in this list, add that value to the
	/// returned list; ignore the others
	///
	/// Items will be quoted and escaped when inserted into a C++ stream,
	/// and a comma will be used as a delimiter between the items.
	value_list_b<ThisType, quote_type0> value_list(
			const std::vector<bool> &vb) const
	{
		return value_list_b<ThisType, quote_type0>(self(), vb, ",", quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the delimiter 'd' will be placed between the
	/// items, and the manipulator 'm' used before each item.
	template <class Manip>
	value_list_b<ThisType, Manip> value_list(const char *d, Manip m,
			bool t0, bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(self().size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<ThisType, Manip>(self(), vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the delimiter 'd' will be placed between the
	/// items, and items will be quoted and escaped.
	value_list_b <ThisType, quote_type0>
	value_list(const char *d, bool t0, bool t1 = false, bool t2 = false,
			bool t3 = false, bool t4 = false, bool t5 = false,
			bool t6 = false, bool t7 = false, bool t8 = false,
			bool t9 = false, bool ta = false, bool tb = false,
			bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(self().size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<ThisType, quote_type0>(self(), vb, d, quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the a comma will be placed between the items,
	/// as a delimiter, and items will be quoted and escaped.
	value_list_b<ThisType, quote_type0> value_list(bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(self().size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<ThisType, quote_type0>(self(), vb, ",", quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, the delimiter
	/// 'd' will be placed between the items, and the manipulator 'm'
	/// will be inserted before each item.
	template <class Manip>
	value_list_b<ThisType, Manip> value_list(const char *d, Manip m,
			std::string s0, std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(self(), vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<ThisType, Manip>(self(), vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, the delimiter
	/// 'd' will be placed between the items, and items will be quoted
	/// and escaped.
	value_list_b<ThisType, quote_type0> value_list(const char *d,
			std::string s0, std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(self(), vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<ThisType, quote_type0>(self(), vb, d, quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, a comma will be
	/// placed between the items as a delimiter, and items will be
	/// quoted and escaped.
	value_list_b<ThisType, quote_type0> value_list(std::string s0,
			std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(self(), vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<ThisType, quote_type0>(self(), vb, ",", quote);
	}

	/// \brief Get a list of the field names in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, and no manipulator will be used on the items.
	value_list_ba<FieldNames, do_nothing_type0> field_list(
			const char* d = ",") const
	{
		return value_list_ba<FieldNames, do_nothing_type0>
				(parent().names(), d, do_nothing);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param m manipulator to use before each item when the list is
	/// inserted into a C++ stream
	template <class Manip>
	value_list_ba<FieldNames, Manip> field_list(const char *d,
			Manip m) const
	{
		return value_list_ba<FieldNames, Manip>(parent().names(), d, m);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param m manipulator to use before each item when the list is
	/// inserted into a C++ stream
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	template <class Manip>
	value_list_b<FieldNames, Manip> field_list(const char *d, Manip m,
			const std::vector<bool>& vb) const
	{
		return value_list_b<FieldNames, Manip>(parent().names(), vb, d, m);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	///
	/// Field names will be quoted and escaped when inserted into a C++
	/// stream.
	value_list_b<FieldNames, quote_type0> field_list(const char *d,
			const std::vector<bool> &vb) const
	{
		return value_list_b<FieldNames, quote_type0>(parent().names(),
				vb, d, quote);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	///
	/// Field names will be quoted and escaped when inserted into a C++
	/// stream, and a comma will be placed between them as a delimiter.
	value_list_b<FieldNames, quote_type0> field_list(
			const std::vector<bool> &vb) const
	{
		return value_list_b<FieldNames, quote_type0>(parent().names(),
				vb, ",", quote);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, the delimiter 'd' will be placed
	/// between the items as a delimiter, and the manipulator 'm' used
	/// before each item.
	template <class Manip> value_list_b<FieldNames, Manip> field_list(
			const char *d, Manip m, bool t0, bool t1 = false,
			bool t2 = false, bool t3 = false, bool t4 = false,
			bool t5 = false, bool t6 = false, bool t7 = false,
			bool t8 = false, bool t9 = false, bool ta = false,
			bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(parent().names().size(), vb, t0, t1, t2, t3, t4,
				t5, t6, t7, t8, t9, ta, tb, tc);
		return value_list_b<FieldNames, Manip>(parent().names(), vb, d, m);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, the delimiter 'd' will be placed
	/// between the items as a delimiter, and the items will be quoted
	/// and escaped.
	value_list_b<FieldNames, quote_type0> field_list(const char *d,
			bool t0, bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(parent().names().size(), vb, t0, t1, t2, t3, t4,
				t5, t6, t7, t8, t9, ta, tb, tc);
		return value_list_b<FieldNames, quote_type0>(parent().names(),
				vb, d, quote);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, a comma will be placed between the
	/// items as a delimiter, and the items will be quoted and escaped.
	value_list_b<FieldNames, quote_type0> field_list(bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(parent().names().size(), vb, t0, t1, t2, t3, t4,
				t5, t6, t7, t8, t9, ta, tb, tc);
		return value_list_b<FieldNames, quote_type0>(parent().names(),
				vb, ",", quote);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// The 's' parameters name the field names that will be added to
	/// the returned list.  When inserted into a C++ stream, the
	/// delimiter 'd' will be placed between the items, and the
	/// manipulator 'm' will be inserted before each item.
	template <class Manip> value_list_b<FieldNames, Manip> field_list(
			const char *d, Manip m, std::string s0, std::string s1 = "",
			std::string s2 = "", std::string s3 = "",
			std::string s4 = "", std::string s5 = "",
			std::string s6 = "", std::string s7 = "",
			std::string s8 = "", std::string s9 = "",
			std::string sa = "", std::string sb = "",
			std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(parent().names(), vb, s0, s1, s2, s3, s4, s5, s6,
				s7, s8, s9, sa, sb, sc);
		return value_list_b<FieldNames, Manip>(parent().names(), vb, d, m);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// The 's' parameters name the field names that will be added to
	/// the returned list.  When inserted into a C++ stream, the
	/// delimiter 'd' will be placed between the items, and the items
	/// will be quoted and escaped.
	value_list_b<FieldNames, quote_type0> field_list(const char *d,
			std::string s0, std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(parent().names(), vb, s0, s1, s2, s3, s4, s5, s6,
				s7, s8, s9, sa, sb, sc);
		return value_list_b<FieldNames, quote_type0>(parent().names(),
				vb, d, quote);
	}
	
	/// \brief Get a list of the field names in this row
	///
	/// The 's' parameters name the field names that will be added to
	/// the returned list.  When inserted into a C++ stream, a comma
	/// will be used as a delimiter between the items, and the items
	/// will be quoted and escaped.
	value_list_b<FieldNames, quote_type0> field_list(std::string s0,
			std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(parent().names(), vb, s0, s1, s2, s3, s4, s5, s6,
				s7, s8, s9, sa, sb, sc);
		return value_list_b<FieldNames, quote_type0>(parent().names(),
				vb, ",", quote);
	}

	/// \brief Get an "equal list" of the fields and values in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, " = " is the relationship operator, and items
	/// will be quoted and escaped.
	equal_list_ba<FieldNames, ThisType, quote_type0> equal_list(
			const char* d = ",", const char* e = " = ") const
	{
		return equal_list_ba<FieldNames, ThisType, quote_type0>(
				parent().names(), self(), d, e, quote);
	}
	
	/// \brief Get an "equal list" of the fields and values in this row
	///
	/// This method's parameters govern how the returned list will
	/// behave when you insert it into a C++ stream:
	///
	/// \param d delimiter to use between items
	/// \param e the operator to use between elements
	/// \param m the manipulator to use for each element
	///
	/// For example, if d is ",", e is " = ", and m is the quote
	/// manipulator, then the field and value lists (a, b) (c, d'e)
	/// will yield an equal list that gives the following when inserted
	/// into a C++ stream:
	///
	/// \code
	///   'a' = 'c', 'b' = 'd''e'
	/// \endcode
	///
	/// Notice how the single quote was 'escaped' in the SQL way to
	/// avoid a syntax error.
	template <class Manip>
	equal_list_ba<FieldNames, ThisType, Manip> equal_list(const char* d,
			const char* e, Manip m) const 
	{
		return equal_list_ba<FieldNames, ThisType, Manip>(
				parent().names(), self(), d, e, m);
	}
};


/// \brief Manages rows from a result set.
class Row : public const_subscript_container<Row, ColData,
		const ColData>, public RowTemplate<Row, ResUse>
{
private:
	std::vector<std::string> data;
	std::vector<bool> is_nulls;
	const ResUse* res;
	bool throw_exceptions;
	bool initialized;

public:
	/// \brief Default constructor
	Row() { }
	
	/// \brief Create a row object
	///
	/// \param d MySQL C API row data
	/// \param r result set that the row comes from
	/// \param jj length of each item in d
	/// \param te if true, throw exceptions on errors
	Row(MYSQL_ROW d, const ResUse* r, unsigned long* jj, bool te = false) :
	res(r),
	throw_exceptions(te),
	initialized(false)
	{
		if (!d || !r) {
			if (throw_exceptions) {
				throw BadQuery("ROW or RES is NULL");
			}
			else {
				return;
			}
		}
		data.clear();
		is_nulls.clear();
		initialized = true;
		for (unsigned int i = 0; i < size(); i++) {
			data.insert(data.end(),
					(d[i] ?  std::string(d[i], jj[i]) : std::string("NULL")));
			is_nulls.insert(is_nulls.end(), d[i] ? false : true);
		}
	}

	/// \brief Destroy object
	~Row()
	{
		data.clear();
		is_nulls.clear();
		initialized = false;
	}

	/// \brief Get a const reference to this object.
	const Row & self() const
	{
		return *this;
	}

	/// \brief Get a reference to this object.
	Row& self()
	{
		return *this;
	}

	/// \brief Get a reference to our parent class.
	const ResUse& parent() const
	{
		return *res;
	}

	/// \brief Get the number of fields in the row.
	size_type size() const;

	/// \brief Get the value of a field given its index.
	///
	/// If the array index is out of bounds, the C++ standard says that
	/// the underlying vector container should throw an exception.
	/// Whether it actually does is probably implementation-dependent.
	///
	/// Note that we return the
	/// \link mysqlpp::ColData_Tmpl ColData \endlink object by value.
	/// The purpose of ColData is to make it easy to convert the string
	/// data returned by the MySQL server to some more appropriate type,
	/// so you're almost certain to use this operator in a construct
	/// like this:
	///
	/// \code
	///  string s = row[2];
	/// \endcode
	///
	/// That accesses the third field in the row, returns a temporary
	/// ColData object, which is then automatically converted to a
	/// \c std::string and copied into \c s.  That works fine, but
	/// beware of this similar but incorrect construct:
	///
	/// \code
	///  const char* pc = row[2];
	/// \endcode
	///
	/// This one line of code does what you expect, but \c pc is then a
	/// dangling pointer: it points to memory owned by the temporary
	/// ColData object, which will have been destroyed by the time you
	/// get around to actually \e using the pointer.
	const ColData operator [](size_type i) const;

	/// \brief Get the value of a field given its field name.
	///
	/// This function is rather inefficient.  You should use operator[]
	/// if you're using Rows directly, or SSQLS for efficient named
	/// access to row elements.
	const ColData lookup_by_name(const char*) const;

	/// \brief Return the value of a field given its index, in raw form.
	///
	/// This is the same thing as operator[], except that the data isn't
	/// converted to a ColData object first.  Also, this method does not
	/// check for out-of-bounds array indices.
	const char* raw_data(int i) const
	{
		return data[i].data();
	}

	/// \brief Returns true if there is data in the row.
	operator bool() const
	{
		return data.size();
	}
};

} // end namespace mysqlpp

#endif
