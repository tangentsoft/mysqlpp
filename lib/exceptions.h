/// \file exceptions.h
/// \brief Declares the MySQL++-specific exception classes.
///
/// When exceptions are enabled for a given mysqlpp::OptionalExceptions
/// derivative, any of these exceptions can be thrown on error.

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

#ifndef MYSQLPP_EXCEPTIONS_H
#define MYSQLPP_EXCEPTIONS_H

#include "connection.h"

#include <exception>
#include <string>

namespace mysqlpp {

/// \brief Base class for all MySQL++ custom exceptions

class MYSQLPP_EXPORT Exception : public std::exception
{
public:
	/// \brief Create exception object as copy of another
	Exception(const Exception& e) throw() :
	std::exception(e),
	what_(e.what_)
	{
	}

	/// \brief Assign another exception object's contents to this one
	Exception& operator=(const Exception& rhs) throw()
	{
		what_ = rhs.what_;
		return *this;
	}

	/// \brief Destroy exception object
	~Exception() throw() { }

	/// \brief Returns explanation of why exception was thrown
	virtual const char* what() const throw()
	{
		return what_.c_str();
	}

protected:
	/// \brief Create exception object
	Exception(const char* w = "") throw() :
	what_(w)
	{
	}

	/// \brief Create exception object
	Exception(const std::string& w) throw() :
	what_(w)
	{
	}

	/// \brief explanation of why exception was thrown
	std::string what_;
};


/// \brief Exception thrown when a bad type conversion is attempted.

class MYSQLPP_EXPORT BadConversion : public Exception
{
public:
	const char* type_name;	///< name of type we tried to convert to
	std::string data;		///< string form of data we tried to convert
	size_t retrieved;		///< documentation needed!
	size_t actual_size;		///< documentation needed!

	/// \brief Create exception object, building error string
	/// dynamically
	///
	/// \param tn type name we tried to convert to
	/// \param d string form of data we tried to convert
	/// \param r ??
	/// \param a ??
	BadConversion(const char* tn, const char* d,
			size_t r, size_t a) :
	Exception(std::string("Bad type conversion: ") +
			std::string(d ? d : "<NULL>")),
	type_name(tn),
	data(d),
	retrieved(r),
	actual_size(a)
	{
	}

	/// \brief Create exception object, given completed error string
	///
	/// \param w the "what" error string
	/// \param tn type name we tried to convert to
	/// \param d string form of data we tried to convert
	/// \param r ??
	/// \param a ??
	BadConversion(const std::string& w, const char* tn,
				  const char* d, size_t r, size_t a) :
	Exception(w),
	type_name(tn),
	data(d),
	retrieved(r),
	actual_size(a)
	{
	}

	/// \brief Create exception object, with error string only
	///
	/// \param w the "what" error string
	///
	/// All other data members are initialize to default values
	explicit BadConversion(const char* w = "") :
	Exception(w),
	type_name("unknown"),
	data(""),
	retrieved(0),
	actual_size(0)
	{
	}

	/// \brief Destroy exception
	~BadConversion() throw() { }
};


/// \brief Exception thrown when a requested named field doesn't exist.
///
/// Thrown by Row::lookup_by_name() when you pass a field name that
/// isn't in the result set.

class MYSQLPP_EXPORT BadFieldName : public Exception
{
public:
	/// \brief Create exception object
	///
	/// \param bad_field name of field the MySQL server didn't like
	explicit BadFieldName(const char* bad_field) :
	Exception(std::string("Unknown field name: ") + bad_field)
	{
	}

	/// \brief Destroy exception
	~BadFieldName() throw() { }
};


/// \brief Exception thrown when you attempt to convert a SQL null
/// to an incompatible type.

class MYSQLPP_EXPORT BadNullConversion : public Exception
{
public:
	/// \brief Create exception object
	explicit BadNullConversion(const char* w = "") :
	Exception(w)
	{
	}
};


/// \brief Exception thrown when you pass an unrecognized option to
/// Connection::set_option().

class MYSQLPP_EXPORT BadOption : public Exception
{
public:
	/// \brief Create exception object, taking C string
	explicit BadOption(const char* w,
			Connection::Option o) :
	Exception(w),
	option_(o)
	{
	}

	/// \brief Create exception object, taking C++ string
	explicit BadOption(const std::string& w,
			Connection::Option o) :
	Exception(w),
	option_(o)
	{
	}

	/// \brief Return the option that failed
	Connection::Option what_option() const { return option_; }

private:
	Connection::Option option_;
};


/// \brief Exception thrown when not enough query parameters are
/// provided.
///
/// This is used in handling template queries.

class MYSQLPP_EXPORT BadParamCount : public Exception
{
public:
	/// \brief Create exception object
	explicit BadParamCount(const char* w = "") :
	Exception(w)
	{
	}

	/// \brief Destroy exception
	~BadParamCount() throw() { }
};


/// \brief Exception thrown when MySQL encounters a problem while
/// processing your query.
///
/// This exception is typically only thrown when the server rejects a
/// SQL query.  In v1.7, it was used as a more generic exception type,
/// for no particularly good reason.

class MYSQLPP_EXPORT BadQuery : public Exception
{
public:
	/// \brief Create exception object, taking C string
	explicit BadQuery(const char* w = "") :
	Exception(w)
	{
	}

	/// \brief Create exception object, taking C++ string
	explicit BadQuery(const std::string& w) :
	Exception(w)
	{
	}
};


/// \brief Exception thrown when there is a problem establishing the
/// database server connection.  It's also thrown if
/// Connection::shutdown() fails.

class MYSQLPP_EXPORT ConnectionFailed : public Exception
{
public:
	/// \brief Create exception object
	explicit ConnectionFailed(const char* w = "") :
	Exception(w)
	{
	}
};


/// \brief Exception thrown when the program tries to select a new
/// database and the server refuses for some reason.

class MYSQLPP_EXPORT DBSelectionFailed : public Exception
{
public:
	/// \brief Create exception object
	explicit DBSelectionFailed(const char* w = "") :
	Exception(w)
	{
	}
};


/// \brief Exception thrown when ResUse::fetch_row() walks off the end
/// of a use-query's result set.

class MYSQLPP_EXPORT EndOfResults : public Exception
{
public:
	/// \brief Create exception object
	explicit EndOfResults(const char* w = "end of results") :
	Exception(w)
	{
	}
};


/// \brief Exception thrown when Query::store_next() walks off the end
/// of a use-query's multi result sets.

class MYSQLPP_EXPORT EndOfResultSets : public Exception
{
public:
	/// \brief Create exception object
	explicit EndOfResultSets(const char* w = "end of result sets") :
	Exception(w)
	{
	}
};


/// \brief Exception thrown when a Lockable object fails.
///
/// Currently, "failure" means that the object is already locked when
/// you make a call that tries to lock it again.  In the future, that
/// case will probably result in the second thread blocking, but the
/// thread library could assert other errors that would keep this
/// exception relevant.

class MYSQLPP_EXPORT LockFailed : public Exception
{
public:
	/// \brief Create exception object
	explicit LockFailed(const char* w = "lock failed") :
	Exception(w)
	{
	}
};


/// \brief Exception thrown when you try to use an object that isn't
/// completely initialized.

class MYSQLPP_EXPORT ObjectNotInitialized : public Exception
{
public:
	/// \brief Create exception object
	explicit ObjectNotInitialized(const char* w = "") :
	Exception(w)
	{
	}
};


} // end namespace mysqlpp

#endif
