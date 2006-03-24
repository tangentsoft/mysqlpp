/// \file result.h
/// \brief Declares classes for holding SQL query result sets.

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

#ifndef MYSQLPP_RESULT_H
#define MYSQLPP_RESULT_H

#include "defs.h"

#include "exceptions.h"
#include "fields.h"
#include "field_names.h"
#include "field_types.h"
#include "noexceptions.h"
#include "resiter.h"
#include "row.h"

#include <mysql.h>

#include <map>
#include <set>
#include <string>

namespace mysqlpp {

class Connection;

/// \brief A basic result set class, for use with "use" queries.
///
/// A "use" query is one where you make the query and then process just
/// one row at a time in the result instead of dealing with them all as
/// a single large chunk.  (The name comes from the MySQL C API function
/// that initiates this action, \c mysql_use_result().)  By calling
/// fetch_row() until it throws a mysqlpp::BadQuery exception (or an
/// empty row if exceptions are disabled), you can process the result
/// set one row at a time.

class ResUse : public OptionalExceptions
{
public:
	/// \brief Default constructor
	ResUse() :
	OptionalExceptions(),
	conn_(0),
	result_(0),
	initialized_(false),
	names_(0),
	types_(0),
	fields_(this)
	{
	}
	
	/// \brief Create the object, fully initialized
	MYSQLPP_EXPORT ResUse(MYSQL_RES* result, Connection* c = 0, bool te = true);
	
	/// \brief Create a copy of another ResUse object
	ResUse(const ResUse& other) :
	OptionalExceptions(),
	initialized_(false)
	{
		copy(other);
		other.result_ = 0;
	}
	
	/// \brief Destroy object
	MYSQLPP_EXPORT virtual ~ResUse();

	/// \brief Copy another ResUse object's data into this object
	ResUse& operator =(const ResUse& other);

	/// \brief Return raw MySQL C API result set
	MYSQL_RES* raw_result()
	{
		return result_;
	}

	/// \brief Wraps mysql_fetch_row() in MySQL C API.
	///
	/// This is not a thin wrapper. It does a lot of error checking before
	/// returning the mysqlpp::Row object containing the row data.
	Row fetch_row()
	{
		if (!result_) {
			if (throw_exceptions()) {
				throw BadQuery("Results not fetched");
			}
			else {
				return Row();
			}
		}
		MYSQL_ROW row = mysql_fetch_row(result_);
		unsigned long* length = mysql_fetch_lengths(result_);
		if (!row || !length) {
			if (throw_exceptions()) {
				throw EndOfResults();
			}
			else {
				return Row();
			}
		}
		return Row(row, this, length, throw_exceptions());
	}

	/// \brief Wraps mysql_fetch_lengths() in MySQL C API.
	unsigned long *fetch_lengths() const
	{
		return mysql_fetch_lengths(result_);
	}

	/// \brief Wraps mysql_fetch_field() in MySQL C API.
	Field& fetch_field() const
	{
		return *mysql_fetch_field(result_);
	}

	/// \brief Wraps mysql_field_seek() in MySQL C API.
	void field_seek(int field)
	{
		mysql_field_seek(result_, field);
	}

	/// \brief Wraps mysql_num_fields() in MySQL C API.
	int num_fields() const
	{
		return mysql_num_fields(result_);
	}
	
	/// \brief Documentation needed!
	void parent_leaving()
	{
		conn_ = 0;
	}

	/// \brief Free all resources held by the object.
	///
	/// This class's destructor is little more than a call to purge(),
	/// so you can think of this as a way to re-use a ResUse object,
	/// to avoid having to completely re-create it.
	void purge()
	{
		if (result_) {
			mysql_free_result(result_);
			result_ = 0;
		}

		delete names_;
		names_ = 0;

		delete types_;
		types_ = 0;

		table_.erase();
	}

	/// \brief Return true if we have a valid result set
	///
	/// This operator is primarily used to determine if a query was
	/// successful:
	///
	/// \code
	///   Query q("....");
	///   if (q.use()) {
	///       ...
	/// \endcode
	///
	/// Query::use() returns a ResUse object, and it won't contain a
	/// valid result set if the query failed.
	operator bool() const
	{
		return result_;
	}
	
	/// \brief Return the number of columns in the result set.
	unsigned int columns() const
	{
		return num_fields();
	}

	/// \brief Get the name of table that the result set comes from.
	std::string& table()
	{
		return table_;
	}

	/// \brief Return the name of the table
	///
	/// This is only valid 
	const std::string& table() const
	{
		return table_;
	}

	/// \brief Get the index of the named field.
	///
	/// This is the inverse of field_name().
	MYSQLPP_EXPORT inline int field_num(const std::string&) const;

	/// \brief Get the name of the field at the given index.
	///
	/// This is the inverse of field_num().
	MYSQLPP_EXPORT inline std::string& field_name(int);

	/// \brief Get the name of the field at the given index.
	MYSQLPP_EXPORT inline const std::string& field_name(int) const;

	/// \brief Get the names of the fields within this result set.
	MYSQLPP_EXPORT inline FieldNames& field_names();

	/// \brief Get the names of the fields within this result set.
	MYSQLPP_EXPORT inline const FieldNames& field_names() const;

	/// \brief Reset the names in the field list to their original
	/// values.
	MYSQLPP_EXPORT inline void reset_field_names();

	/// \brief Get the MySQL type for a field given its index.
	MYSQLPP_EXPORT inline mysql_type_info& field_type(int i);

	/// \brief Get the MySQL type for a field given its index.
	MYSQLPP_EXPORT inline const mysql_type_info& field_type(int) const;

	/// \brief Get a list of the types of the fields within this
	/// result set.
	MYSQLPP_EXPORT inline FieldTypes& field_types();

	/// \brief Get a list of the types of the fields within this
	/// result set.
	MYSQLPP_EXPORT inline const FieldTypes& field_types() const;

	/// \brief Reset the field types to their original values.
	MYSQLPP_EXPORT inline void reset_field_types();

	/// \brief Alias for field_num()
	MYSQLPP_EXPORT inline int names(const std::string & s) const;

	/// \brief Alias for field_name()
	MYSQLPP_EXPORT inline std::string& names(int i);

	/// \brief Alias for field_name()
	MYSQLPP_EXPORT inline const std::string& names(int i) const;

	/// \brief Alias for field_names()
	MYSQLPP_EXPORT inline FieldNames& names();

	/// \brief Alias for field_names()
	MYSQLPP_EXPORT inline const FieldNames& names() const;

	/// \brief Alias for reset_field_names()
	MYSQLPP_EXPORT inline void reset_names();

	/// \brief Alias for field_type()
	MYSQLPP_EXPORT inline mysql_type_info& types(int i);

	/// \brief Alias for field_type()
	MYSQLPP_EXPORT inline const mysql_type_info& types(int i) const;

	/// \brief Alias for field_types()
	MYSQLPP_EXPORT inline FieldTypes& types();

	/// \brief Alias for field_types()
	MYSQLPP_EXPORT inline const FieldTypes& types() const;

	/// \brief Alias for reset_field_types()
	MYSQLPP_EXPORT inline void reset_types();

	/// \brief Get the underlying Fields structure.
	const Fields& fields() const
	{
		return fields_;
	}

	/// \brief Get the underlying Field structure given its index.
	const Field& fields(unsigned int i) const
	{
		return fields_.at(i);
	}
	
	/// \brief Returns true if the other ResUse object shares the same
	/// underlying C API result set as this one.
	///
	/// This works because the underlying result set is stored as a
	/// pointer, and thus can be copied and then compared.
	bool operator ==(const ResUse& other) const
	{
		return result_ == other.result_;
	}
	
	/// \brief Returns true if the other ResUse object has a different
	/// underlying C API result set from this one.
	bool operator !=(const ResUse& other) const
	{
		return result_ != other.result_;
	}

protected:
	Connection* conn_;			///< server result set comes from
	mutable MYSQL_RES* result_;	///< underlying C API result set
	bool initialized_;			///< if true, object is fully initted
	mutable FieldNames* names_;	///< list of field names in result
	mutable FieldTypes* types_;	///< list of field types in result
	Fields fields_;				///< list of fields in result
	std::string table_;			///< table result set comes from

	/// \brief Copy another ResUse object's contents into this one.
	///
	/// Self-copy is not allowed.
	MYSQLPP_EXPORT void copy(const ResUse& other);
};


/// \brief This class manages SQL result sets. 
///
/// Objects of this class are created to manage the result of "store"
/// queries, where the result set is handed to the program as single
/// block of row data. (The name comes from the MySQL C API function
/// \c mysql_store_result() which creates these blocks of row data.)
///
/// This class is a random access container (in the STL sense) which
/// is neither less-than comparable nor assignable.  This container
/// provides a reverse random-access iterator in addition to the normal
/// forward one.

class Result : public ResUse,
		public const_subscript_container<Result, Row, const Row>
{
public:
	/// \brief Default constructor
	Result()
	{
	}
	
	/// \brief Fully initialize object
	Result(MYSQL_RES* result, bool te = true) :
	ResUse(result, 0, te)
	{
	}

	/// \brief Initialize object as a copy of another Result object
	Result(const Result& other) :
	ResUse(other),
	const_subscript_container<Result, Row, const Row>() // no copying here
	{
		conn_ = 0;
	}

	/// \brief Destroy result set
	virtual ~Result() { }

	/// \brief Wraps mysql_fetch_row() in MySQL C API.
	///
	/// This is simply the const version of the same function in our
	/// \link mysqlpp::ResUse parent class \endlink . Why this cannot
	/// actually \e be in our parent class is beyond me.
	const Row fetch_row() const
	{
		if (!result_) {
			if (throw_exceptions()) {
				throw BadQuery("Results not fetched");
			}
			else {
				return Row();
			}
		}
		MYSQL_ROW row = mysql_fetch_row(result_);
		unsigned long* length = mysql_fetch_lengths(result_);
		if (!row || !length) {
			if (throw_exceptions()) {
				throw EndOfResults();
			}
			else {
				return Row();
			}
		}
		return Row(row, this, length, throw_exceptions());
	}

	/// \brief Wraps mysql_num_rows() in MySQL C API.
	my_ulonglong num_rows() const
	{
		if (initialized_)
			return mysql_num_rows(result_);
		else
			return 0;
	}

	/// \brief Wraps mysql_data_seek() in MySQL C API.
	void data_seek(uint offset) const
	{
		mysql_data_seek(result_, offset);
	}

	/// \brief Alias for num_rows(), only with different return type.
	size_type size() const
	{
		return size_type(num_rows());
	}

	/// \brief Alias for num_rows(), only with different return type.
	size_type rows() const
	{
		return size_type(num_rows());
	}

	/// \brief Get the row with an offset of i.
	const Row at(size_type i) const
	{
		data_seek(i);
		return fetch_row();
	}
};


/// \brief Swaps two ResUse objects
inline void swap(ResUse& x, ResUse& y)
{
	ResUse tmp = x;
	x = y;
	y = tmp;
}

/// \brief Swaps two Result objects
inline void swap(Result& x, Result& y)
{
	Result tmp = x;
	x = y;
	y = tmp;
}

/// \brief Holds the information on the success of queries that
/// don't return any results.
class ResNSel
{
public:
	bool success;			///< if true, query was successful
	my_ulonglong insert_id;	///< last value used for AUTO_INCREMENT field
	my_ulonglong rows;		///< number of rows affected
	std::string info;		///< additional info about query result

	ResNSel() :
	success(false)
	{
	}

	/// \brief Initialize object
	MYSQLPP_EXPORT ResNSel(Connection* q);

	/// \brief Returns true if the query was successful
	operator bool() { return success; }
};


inline int ResUse::field_num(const std::string& i) const
{
	if (!names_) {
		names_ = new FieldNames(this);
	}

	size_t index = (*names_)[i];
	if ((index >= names_->size()) && throw_exceptions()) {
		throw BadFieldName(i.c_str());
	}
	
	return int(index);
}

inline std::string& ResUse::field_name(int i)
{
	if (!names_) {
		names_ = new FieldNames(this);
	}
	return (*names_)[i];
}

inline const std::string& ResUse::field_name(int i) const
{
	if (!names_) {
		names_ = new FieldNames(this);
	}
	return (*names_)[i];
}

inline FieldNames& ResUse::field_names()
{
	if (!names_) {
		names_ = new FieldNames(this);
	}
	return *names_;
}

inline const FieldNames& ResUse::field_names() const
{
	if (!names_) {
		names_ = new FieldNames(this);
	}
	return *names_;
}

inline void ResUse::reset_field_names()
{
	delete names_;
	names_ = 0;
	names_ = new FieldNames(this);
}

inline mysql_type_info& ResUse::field_type(int i)
{
	if (!types_) {
		types_ = new FieldTypes(this);
	}
	return (*types_)[i];
}

inline const mysql_type_info& ResUse::field_type(int i) const
{
	if (!types_) {
		types_ = new FieldTypes(this);
	}
	return (*types_)[i];
}

inline FieldTypes& ResUse::field_types()
{
	if (!types_) {
		types_ = new FieldTypes(this);
	}
	return *types_;
}

inline const FieldTypes& ResUse::field_types() const
{
	if (!types_) {
		types_ = new FieldTypes(this);
	}
	return *types_;
}

inline void ResUse::reset_field_types()
{
	delete types_;
	types_ = 0;
	types_ = new FieldTypes(this);
}

inline int ResUse::names(const std::string& s) const
{
	return field_num(s);
}

inline std::string& ResUse::names(int i)
{
	return field_name(i);
}

inline const std::string& ResUse::names(int i) const
{
	return field_name(i);
}

inline FieldNames& ResUse::names()
{
	return field_names();
}

inline const FieldNames& ResUse::names() const
{
	return field_names();
}

inline void ResUse::reset_names()
{
	reset_field_names();
}

inline mysql_type_info& ResUse::types(int i)
{
	return field_type(i);
}

inline const mysql_type_info& ResUse::types(int i) const
{
	return field_type(i);
}

inline FieldTypes& ResUse::types()
{
	return field_types();
}

inline const FieldTypes& ResUse::types() const
{
	return field_types();
}

inline void ResUse::reset_types()
{
	reset_field_types();
}

inline ResUse& ResUse::operator =(const ResUse& other)
{
	if (this == &other) {
		return *this;
	}
	copy(other);
	other.result_ = 0;
	return *this;
}

} // end namespace mysqlpp

#endif
