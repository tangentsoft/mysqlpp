/// \file query.h
/// \brief Defines the user-facing Query class, which is used to build
/// up SQL queries, and execute them.
///
/// This class is used in one of two main fashions.
///
/// First, it has several member functions that can build specific
/// query types.  For instance,
/// \link mysqlpp::Query::insert() insert() \endlink
/// allows you to build an INSERT statement for a Specialized SQL
/// Structure.
///
/// Second, one of its base classes is std::stringstream, so you may
/// build an SQL query in the same way as you'd build up any other
/// string with C++ streams.
///
/// One does not generally create Query objects directly.  Instead, call
/// mysqlpp::Connection::query() to get one tied to that connection.

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

#ifndef MYSQLPP_QUERY_H
#define MYSQLPP_QUERY_H

#include "defs.h"

#include "coldata.h"
#include "connection.h"
#include "result.h"
#include "sql_query.h"

#include <mysql.h>

/// \brief Used to define many similar member functions in class Query.
#define mysql_query_define1(RETURN, FUNC) \
  RETURN FUNC (const char* str); \
  RETURN FUNC (parms &p);\
  mysql_query_define0(RETURN,FUNC) \

/// \brief Used to define many similar member functions in class Query.
#define mysql_query_define2(FUNC) \
  template <class T1> void FUNC (T1 &con, const char* str); \
  template <class T1> void FUNC (T1 &con, parms &p, query_reset r = RESET_QUERY);\
  template <class T1> void FUNC (T1 &con, ss a)\
	{FUNC (con, parms() << a);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b)\
	{FUNC (con, parms() << a << b);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c)\
	{FUNC (con, parms() << a << b << c);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c, ss d)\
	{FUNC (con, parms() << a << b << c << d);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c, ss d, ss e)\
	{FUNC (con, parms() << a << b << c << d << e);} \
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c, ss d, ss e, ss f)\
	{FUNC (con, parms() << a << b << c << d << e << f);}\
  template <class T1> void FUNC (T1 &con,ss a,ss b,ss c,ss d,ss e,ss f,ss g)\
	{FUNC (con, parms() << a << b << c << d << e << f << g);}\
  template <class T1> void FUNC (T1 &con,ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h)\
	{FUNC (con, parms() << a << b << c << d << e << f << g << h);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i)\
	{FUNC (con, parms() << a << b << c << d << e << f << g << h << i);}\
  template <class T1> void FUNC (T1 &con, ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j)\
	{FUNC (con, parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j);}\
  template <class T1> void FUNC (T1 &con, ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k)\
	{FUNC (con, parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k);}\
  template <class T1> void FUNC (T1 &con, ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k,\
			   ss l)\
	{FUNC (con, parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k <<l);}\

namespace mysqlpp {

/// \brief A class for building and executing SQL queries.
///
/// This class is derived from SQLQuery. It adds to that a tie between
/// the query object and a MySQL++
/// \link mysqlpp::Connection Connection \endlink object, so that
/// the query can be sent to the MySQL server we're connected to.

class Query : public SQLQuery
{
private:
	Connection* mysql;
	bool throw_exceptions;

	my_ulonglong affected_rows() const;
	my_ulonglong insert_id();
	std::string info();
	bool lock();
	void unlock();

public:
	/// \brief Create a new query object attached to a connection.
	///
	/// This is the constructor used by mysqlpp::Connection::query().
	///
	/// \param c connection the finished query should be sent out on
	/// \param te if true, throw exceptions on errors
	Query(Connection * c, bool te = false) :
	SQLQuery(),
	mysql(c),
	throw_exceptions(te)
	{
		Success = true;
	}

	/// \brief Create a new query object as a copy of another.
	Query(const Query& q);

	/// \brief Get the last error message that happened on the connection
	/// we're bound to.
	std::string error();

	/// \brief Returns true if the query executed successfully.
	bool success();

	/// \brief Return the query string currently in the buffer.
	std::string preview() { return str(def); }

	/// \brief Return the query string currently in the buffer.
	std::string preview(parms& p) { return str(p); }

	/// \brief Execute a query
	///
	/// Same as execute(), except that it only returns a flag indicating
	/// whether the query succeeded or not.  It is basically a thin
	/// wrapper around the C API function \c mysql_query().
	///
	/// \param str the query to execute
	///
	/// \return true if query was executed successfully
	///
	/// \sa execute(), store(), storein(), and use()
	bool exec(const std::string& str);

	/// \brief Execute a query
	///
	/// Use this function if you don't expect the server to return a
	/// result set.  For instance, a DELETE query.  The returned ResNSel
	/// object contains status information from the server, such as
	/// whether the query succeeded, and if so how many rows were
	/// affected.
	///
	/// There are a number of overloaded versions of this function. The
	/// one without parameters simply executes a query that you have
	/// built up in the object in some way. (For instance, via the
	/// insert() method, or by using the object's stream interface.) You
	/// can also pass the function an std::string containing a SQL query,
	/// a SQLQueryParms object, or as many as 12 SQLStrings. The latter
	/// two (or is it 13?) overloads are for filling out template queries.
	///
	/// \return ResNSel status information about the query
	///
	/// \sa exec(), store(), storein(), and use()
	ResNSel execute() { return execute(def); }

	/// \brief Execute a query that can return a result set
	/// 
	/// Unlike store(), this function does not return the result set
	/// directly.  Instead, it returns an object that can walk through
	/// the result records one by one.  This is superior to store()
	/// when there are a large number of results; store() would have to
	/// allocate a large block of memory to hold all those records,
	/// which could cause problems.
	///
	/// A potential downside of this method is that MySQL database
	/// resources are tied up until the result set is completely
	/// consumed.  Do your best to walk through the result set as
	/// expeditiously as possible.
	///
	/// The name of this method comes from the MySQL C API function
	/// that initiates the retrieval process, \c mysql_use_result().
	/// This method is implemented in terms of that function.
	///
	/// This function has the same set of overloads as execute().
	///
	/// \return ResUse object that can walk through result set serially
	///
	/// \sa exec(), execute(), store() and storein()
	ResUse use() { return use(def); }

	/// \brief Execute a query that can return a result set
	///
	/// This function returns the entire result set immediately.  This
	/// is useful if you actually need all of the records at once, but
	/// if not, consider using use() instead, which returns the results
	/// one at a time.  As a result of this difference, use() doesn't
	/// need to allocate as much memory as store().
	///
	/// You must use store(), storein() or use() for \c SELECT, \c SHOW,
	/// \c DESCRIBE and \c EXPLAIN queries.  You can use these functions
	/// with other query types, but since they don't return a result
	/// set, exec() and execute() are more efficient.
	///
	/// The name of this method comes from the MySQL C API function it
	/// is implemented in terms of, \c mysql_store_result().
	///
	/// This function has the same set of overloads as execute().
	///
	/// \return Result object containing entire result set
	///
	/// \sa exec(), execute(), storein(), and use()
	Result store() { return store(def); }

	/// \brief Execute a query, storing the result set in an STL
	/// sequence container.
	///
	/// This function works much like store() from the caller's
	/// perspective, because it returns the entire result set at once.
	/// It's actually implemented in terms of use(), however, so that
	/// memory for the result set doesn't need to be allocated twice.
	///
	/// There are many overloads for this function, pretty much the same
	/// as for execute(), except that there is a Container parameter at
	/// the front of the list.  So, you can pass a container and a query
	/// string, or a container and template query parameters.
	///
	/// \param con any STL sequence container, such as \c std::vector
	/// \param r whether the query automatically resets after being used
	///
	/// \sa exec(), execute(), store(), and use()
	template <class Sequence> void storein_sequence(Sequence& con,
			query_reset r = RESET_QUERY)
	{
		storein_sequence_(con, def, r);
	}

	/// \brief Execute a query, storing the result set in an STL
	/// associative container.
	///
	/// The same thing as storein_sequence(), except that it's used with
	/// associative STL containers, such as \c std::set.  Other than
	/// that detail, that method's comments apply equally well to this
	/// one.
	template <class Set> void storein_set(Set& con,
			query_reset r = RESET_QUERY)
	{
		storein_set(con, def, r);
	}

	/// \brief Execute a query, and store the entire result set
	/// in an STL container.
	///
	/// This is a set of specialized template functions that call either
	/// storein_sequence() or storein_set(), depending on the type of
	/// container you pass it. It understands \c std::vector, \c deque,
	/// \c list, \c slist (a common C++ library extension), \c set,
	/// and \c multiset.
	///
	/// Like the functions it wraps, this is actually an overloaded set
	/// of functions. See the other functions' documentation for details.
	///
	/// Use this function if you think you might someday switch your
	/// program from using a set-associative container to a sequence
	/// container for storing result sets, or vice versa.
	///
	/// See exec(), execute(), store(), and use() for alternative
	/// query execution mechanisms.
	template<class Container> void storein(Container& con,
			query_reset r = RESET_QUERY) { storein(con, def, r); }

	/// \brief Replace an existing row's data with new data.
	///
	/// This function builds an UPDATE SQL query using the new row data
	/// for the SET clause, and the old row data for the WHERE clause.
	/// One uses it with MySQL++'s Specialized SQL Structures mechanism.
	///
	/// \param o old row
	/// \param n new row
	///
	/// \sa insert(), replace()
	template<class T> Query& update(const T& o, const T& n)
	{
		SQLQuery::update(o, n);
		return *this;
	}

	/// \brief Insert a new row.
	///
	/// This function builds an INSERT SQL query.  One uses it with
	/// MySQL++'s Specialized SQL Structures mechanism.
	///
	/// \param v new row
	///
	/// \sa replace(), update()
	template<class T> Query& insert(const T& v)
	{
		SQLQuery::insert(v);
		return *this;
	}

	/// \brief Insert multiple new rows.
	///
	/// Builds an INSERT SQL query using items from a range within an
	/// STL container.  Insert the entire contents of the container by
	/// using the begin() and end() iterators of the container as
	/// parameters to this function.
	///
	/// \param first iterator pointing to first element in range to
	///    insert
	/// \param last iterator pointing to one past the last element to
	///    insert
	///
	/// \sa replace(), update()
	template<class Iter> Query& insert(Iter first, Iter last)
	{
		SQLQuery::insert(first, last);
		return *this;
	}

	/// \brief Insert new row unless there is an existing row that
	/// matches on a unique index, in which case we replace it.
	///
	/// This function builds a REPLACE SQL query.  One uses it with
	/// MySQL++'s Specialized SQL Structures mechanism.
	///
	/// \param v new row
	///
	/// \sa insert(), update()
	template<class T> Query& replace(const T& v)
	{
		SQLQuery::replace(v);
		return *this;
	}

#if !defined(DOXYGEN_IGNORE)
	// Declare the remaining overloads.  These are hidden down here partly
	// to keep the above code clear, but also so that we may hide them
	// from Doxygen, which gets confused by macro instantiations that look
	// like method declarations.
	mysql_query_define0(std::string, preview)
	mysql_query_define1(ResNSel, execute)
	mysql_query_define1(ResUse, use)
	mysql_query_define1(Result, store)
	mysql_query_define2(storein_sequence)
	mysql_query_define2(storein_set)
	mysql_query_define2(storein)
#endif // !defined(DOXYGEN_IGNORE)
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

template<class Seq>
void Query::storein_sequence(Seq& seq, parms& p, query_reset r)
{
	r = parsed.size() ? DONT_RESET : RESET_QUERY;
	mysql->storein_sequence(seq, str(p, r));
}

template<class Set>
void Query::storein_set(Set& sett, parms& p, query_reset r)
{
	r = parsed.size() ? DONT_RESET : RESET_QUERY;
	mysql->storein_set(sett, str(p, r));
}

template<class Sequence>
void Query::storein_sequence(Sequence& seq, const char *s)
{
	mysql->storein_sequence(seq, s);
}

template<class Set>
void Query::storein_set(Set& sett, const char *s)
{
	mysql->storein_set(sett, s);
}

template<class T>
void Query::storein(T& con, parms& p, query_reset r)
{
	r = parsed.size() ? DONT_RESET : RESET_QUERY;
	mysql->storein(con, str(p, r));
}

template<class T>
void Query::storein(T& con, const char *s)
{
	mysql->storein(con, s);
}

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif

