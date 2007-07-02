/// \file query.h
/// \brief Defines a class for building and executing SQL queries.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004-2006 by Educational Technology Resources, Inc.
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

#include "common.h"

#include "lockable.h"
#include "noexceptions.h"
#include "qparms.h"
#include "querydef.h"
#include "result.h"
#include "row.h"
#include "sql_string.h"

#include <mysql.h>

#include <deque>
#include <iomanip>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#ifdef HAVE_EXT_SLIST
#  include <ext/slist>
#else
#  if defined(HAVE_STD_SLIST) || defined(HAVE_GLOBAL_SLIST)
#      include <slist>
#  endif
#endif

/// \def MYSQLPP_QUERY_THISPTR
/// \brief Helper macro used inside MySQL++ to work around a VC++ 2003 bug
///
/// This macro returns '*this', either directly or upcast to Query's
/// base class to work around an error in the overloaded operator
/// lookup logic in VC++ 2003.  For an explanation of the problem, see:
/// http://groups.google.com/group/microsoft.public.vc.stl/browse_thread/thread/9a68d84644e64f15
#if defined(_MSC_VER) && (_MSC_VER < 1400)
#	define MYSQLPP_QUERY_THISPTR dynamic_cast<std::ostream&>(*this)
#else
#	define MYSQLPP_QUERY_THISPTR *this
#endif

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT Connection;
#endif

/// \brief Used for indicating whether a query object should auto-reset
enum query_reset { DONT_RESET, RESET_QUERY };

/// \brief A class for building and executing SQL queries.
///
/// This class is derived from SQLQuery. It adds to that a tie between
/// the query object and a MySQL++
/// \link mysqlpp::Connection Connection \endlink object, so that
/// the query can be sent to the MySQL server we're connected to.
///
/// One does not generally create Query objects directly. Instead, call
/// mysqlpp::Connection::query() to get one tied to that connection.
///
/// There are several ways to build and execute SQL queries with this
/// class.
///
/// The way most like other database libraries is to pass a SQL
/// statement to one of the
/// \link mysqlpp::Query::execute() exec*(), \endlink
/// \link mysqlpp::Query::store() store*(), \endlink or use() methods
/// taking a C or C++ string.  The query is executed immediately, and
/// any results returned.
///
/// For more complicated queries, you can use Query's stream interface.
/// You simply build up a query using the Query instance as you would
/// any other C++ stream object. When the query string is complete, you
/// call the overloaded version of \c exec*(), \c store*() or \c use()
/// that takes no parameters, which executes the built query and returns
/// any results.
///
/// If you are using the library's Specialized SQL Structures feature,
/// Query has several special functions for generating common SQL
/// queries from those structures. For instance, it offers the
/// \link mysqlpp::Query::insert() insert() \endlink method, which
/// builds an INSERT query to add the contents of the SSQLS to the
/// database. As with the stream interface, these methods only build
/// the query string; call one of the parameterless methods mentioned
/// previously to actually execute the query.
///
/// Finally, you can build "template queries". This is something like
/// C's \c printf() function, in that you insert a specially-formatted
/// query string into the object which contains placeholders for data.
/// You call the parse() method to tell the Query object that the query
/// string contains placeholders. Once that's done, you can call any of
/// the many overloaded methods that take a number of SQLStrings (up to
/// 25 by default) or any type that can be converted to SQLString, and
/// those parameters will be inserted into the placeholders. When you
/// call one of the parameterless functions the execute the query, the
/// final query string is assembled and sent to the server.
///
/// See the user manual for more details about these options.

class MYSQLPP_EXPORT Query : public std::ostream,
		public OptionalExceptions, public Lockable
{
public:
	/// \brief Create a new query object attached to a connection.
	///
	/// This is the constructor used by mysqlpp::Connection::query().
	///
	/// \param c connection the finished query should be sent out on
	/// \param te if true, throw exceptions on errors
	Query(Connection* c, bool te = true);

	/// \brief Create a new query object as a copy of another.
	///
	/// This is \b not a traditional copy ctor!  Its only purpose is to
	/// make it possible to assign the return of Connection::query()
	/// to an empty Query object.  In particular, the stream buffer and
	/// template query stuff will be empty in the copy, regardless of
	/// what values they have in the original.
	Query(const Query& q);

	/// \brief Assign another query's state to this object
	///
	/// The same caveats apply to this operator as apply to the copy
	/// ctor.
	Query& operator=(const Query& rhs);

	/// \brief Get the last error message that was set.
	///
	/// This class has an internal error message string, but if it
	/// isn't set, we return the last error message that happened
	/// on the connection we're bound to instead.
	std::string error();

	/// \brief Returns true if the last operation succeeded
	///
	/// Returns true if the last query succeeded, and the associated
	/// Connection object's success() method also returns true.  If
	/// either object is unhappy, this method returns false.
	bool success();

	/// \brief Treat the contents of the query string as a template
	/// query.
	///
	/// This method sets up the internal structures used by all of the
	/// other members that accept template query parameters.  See the
	/// "Template Queries" chapter in the user manual for more
	/// information.
	void parse();

	/// \brief Reset the query object so that it can be reused.
	///
	/// This erases the query string and the contents of the parameterized
	/// query element list.
	void reset();

	/// \brief Return the query string currently in the buffer.
	std::string preview() { return str(def); }

	/// \brief Return the query string currently in the buffer with
	/// template query parameter substitution.
	///
	/// \param arg0 the value to substitute for the first template query
	/// parameter
	std::string preview(const SQLString& arg0)
			{ return preview(SQLQueryParms() << arg0); }

	/// \brief Return the query string currently in the buffer.
	std::string preview(SQLQueryParms& p) { return str(p); }

	/// \brief Get built query as a null-terminated C++ string
	std::string str() { return str(def); }

	/// \brief Get built query as a null-terminated C++ string with
	/// template query parameter substitution.
	///
	/// \param arg0 the value to substitute for the first template query
	/// parameter
	std::string str(const SQLString& arg0)
			{ return preview(SQLQueryParms() << arg0); }

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param r if equal to \c RESET_QUERY, query object is cleared
	/// after this call
	std::string str(query_reset r) { return str(def, r); }

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param p template query parameters to use, overriding the ones
	/// this object holds, if any
	std::string str(SQLQueryParms& p);

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param p template query parameters to use, overriding the ones
	/// this object holds, if any
	/// \param r if equal to \c RESET_QUERY, query object is cleared
	/// after this call
	std::string str(SQLQueryParms& p, query_reset r);

	/// \brief Execute a query
	///
	/// Same as execute(), except that it only returns a flag indicating
	/// whether the query succeeded or not.  It is basically a thin
	/// wrapper around the C API function \c mysql_real_query().
	///
	/// \param str the query to execute
	///
	/// \return true if query was executed successfully
	///
	/// \sa execute(), store(), storein(), and use()
	bool exec(const std::string& str);

	/// \brief Execute built-up query
	///
	/// Use one of the execute() overloads if you don't expect the
	/// server to return a result set. For instance, a DELETE query.
	/// The returned ResNSel object contains status information from
	/// the server, such as whether the query succeeded, and if so how
	/// many rows were affected.
	///
	/// This overloaded version of execute() simply executes the query
	/// that you have built up in the object in some way. (For instance,
	/// via the insert() method, or by using the object's stream
	/// interface.)
	///
	/// \return ResNSel status information about the query
	///
	/// \sa exec(), store(), storein(), and use()
	ResNSel execute() { return execute(def); }

	/// \brief Execute query in a C++ string, or substitute string into
	/// a template query and execute it.
	///
	/// \param str If the object represents a compiled template query,
	/// substitutes this string in for the first parameter.  Otherwise,
	/// takes the string as a complete SQL query and executes it.
	ResNSel execute(const SQLString& str);

	/// \brief Execute query in a C string
	///
	/// Executes the query immediately, and returns the results.
	ResNSel execute(const char* str);

	/// \brief Execute query in a known-length string of characters.
	/// This can include null characters.
	///
	/// Executes the query immediately, and returns the results.
	ResNSel execute(const char* str, size_t len);

	/// \brief Execute a query that can return a result set
	/// 
	/// Use one of the use() overloads if memory efficiency is
	/// important.  They return an object that can walk through
	/// the result records one by one, without fetching the entire
	/// result set from the server.  This is superior to store()
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

	/// \brief Execute query in a C++ string
	///
	/// Executes the query immediately, and returns an object that
	/// lets you walk through the result set one row at a time, in
	/// sequence.  This is more memory-efficient than store().
	ResUse use(const SQLString& str);

	/// \brief Execute query in a C string
	///
	/// Executes the query immediately, and returns an object that
	/// lets you walk through the result set one row at a time, in
	/// sequence.  This is more memory-efficient than store().
	ResUse use(const char* str);

	/// \brief Execute query in a known-length C string
	///
	/// Executes the query immediately, and returns an object that
	/// lets you walk through the result set one row at a time, in
	/// sequence.  This is more memory-efficient than store().
	ResUse use(const char* str, size_t len);

	/// \brief Execute a query that can return a result set
	///
	/// Use one of the store() overloads to execute a query and retrieve
	/// the entire result set into memory.  This is useful if you
	/// actually need all of the records at once, but if not, consider
	/// using one of the use() methods instead, which returns the results
	/// one at a time, so they don't allocate as much memory as store().
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

	/// \brief Execute query in a C++ string
	///
	/// Executes the query immediately, and returns an object that
	/// contains the entire result set.  This is less memory-efficient
	/// than use(), but it lets you have random access to the results.
	Result store(const SQLString& str);

	/// \brief Execute query in a C string
	///
	/// Executes the query immediately, and returns an object that
	/// contains the entire result set.  This is less memory-efficient
	/// than use(), but it lets you have random access to the results.
	Result store(const char* str);

	/// \brief Execute query in a known-length C string
	///
	/// Executes the query immediately, and returns an object that
	/// contains the entire result set.  This is less memory-efficient
	/// than use(), but it lets you have random access to the results.
	Result store(const char* str, size_t len);

	/// \brief Execute a query, and call a functor for each returned row
	///
	/// This method wraps a use() query, calling the given functor for
	/// every returned row.  It is analogous to STL's for_each()
	/// algorithm, but instead of iterating over some range within a
	/// container, it iterates over a result set produced by a query.
	///
	/// \param query the query string
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <typename Function>
	Function for_each(const SQLString& query, Function fn)
	{	
		mysqlpp::ResUse res = use(query);
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				fn(row);
			}
		}

		return fn;
	}

	/// \brief Execute the query, and call a functor for each returned row
	///
	/// Just like for_each(const SQLString&, Function), but it uses
	/// the query string held by the Query object already
	///
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <typename Function>
	Function for_each(Function fn)
	{	
		mysqlpp::ResUse res = use();
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				fn(row);
			}
		}

		return fn;
	}

	/// \brief Run a functor for every row in a table
	///
	/// Just like for_each(Function), except that it builds a
	/// "select * from TABLE" query using the SQL table name from
	/// the SSQLS instance you pass.
	///
	/// \param ssqls the SSQLS instance to get a table name from
	/// \param fn the functor called for each row
	///
	/// \return a copy of the passed functor
	template <class SSQLS, typename Function>
	Function for_each(const SSQLS& ssqls, Function fn)
	{	
		SQLString query("select * from ");
		query += ssqls._table;
		mysqlpp::ResUse res = use(query);
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				fn(row);
			}
		}

		return fn;
	}

	/// \brief Execute a query, conditionally storing each row in a
	/// container
	///
	/// This method wraps a use() query, calling the given functor for
	/// every returned row, and storing the results in the given
	/// sequence container if the functor returns true.
	///
	/// This is analogous to the STL copy_if() algorithm, except that
	/// the source rows come from a database query instead of another
	/// container.  (copy_if() isn't a standard STL algorithm, but only
	/// due to an oversight by the standardization committee.)  This
	/// fact may help you to remember the order of the parameters: the
	/// container is the destination, the query is the source, and the
	/// functor is the predicate; it's just like an STL algorithm.
	///
	/// \param seq the destination container; needs a push_back() method
	/// \param query the query string
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <class Sequence, typename Function>
	Function store_if(Sequence& seq, const SQLString& query, Function fn)
	{	
		mysqlpp::ResUse res = use(query);
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				if (fn(row)) {
					seq.push_back(row);
				}
			}
		}

		return fn;
	}

	/// \brief Pulls every row in a table, conditionally storing each
	/// one in a container
	///
	/// Just like store_if(Sequence&, const SQLString&, Function), but
	/// it uses the SSQLS instance to construct a "select * from TABLE"
	/// query, using the table name field in the SSQLS.
	///
	/// \param seq the destination container; needs a push_back() method
	/// \param ssqls the SSQLS instance to get a table name from
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <class Sequence, class SSQLS, typename Function>
	Function store_if(Sequence& seq, const SSQLS& ssqls, Function fn)
	{	
		SQLString query("select * from ");
		query += ssqls._table;
		mysqlpp::ResUse res = use(query);
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				if (fn(row)) {
					seq.push_back(row);
				}
			}
		}

		return fn;
	}

	/// \brief Execute the query, conditionally storing each row in a
	/// container
	///
	/// Just like store_if(Sequence&, const SQLString&, Function), but
	/// it uses the query string held by the Query object already
	///
	/// \param seq the destination container; needs a push_back() method
	/// \param fn the functor called for each row
	/// \return a copy of the passed functor
	template <class Sequence, typename Function>
	Function store_if(Sequence& seq, Function fn)
	{	
		mysqlpp::ResUse res = use();
		if (res) {
			mysqlpp::NoExceptions ne(res);
			while (mysqlpp::Row row = res.fetch_row()) {
				if (fn(row)) {
					seq.push_back(row);
				}
			}
		}

		return fn;
	}

	/// \brief Return next result set, when processing a multi-query
	///
	/// There are two cases where you'd use this function instead of
	/// the regular store() functions.
	/// 
	/// First, when handling the result of executing multiple queries
	/// at once.  (See <a
	/// href="http://dev.mysql.com/doc/mysql/en/c-api-multiple-queries.html">this
	/// page</a> in the MySQL documentation for details.) 
	///
	/// Second, when calling a stored procedure, MySQL can return the
	/// result as a set of results.
	///
	/// In either case, you must consume all results before making
	/// another MySQL query, even if you don't care about the remaining
	/// results or result sets.
	///
	/// As the MySQL documentation points out, you must set the
	/// MYSQL_OPTION_MULTI_STATEMENTS_ON flag on the connection in order
	/// to use this feature.  See Connection::set_option().
	///
	/// Multi-queries only exist in MySQL v4.1 and higher.  Therefore,
	/// this function just wraps store() when built against older API
	/// libraries.
	///
	/// \return Result object containing the next result set.
	Result store_next();

	/// \brief Return whether more results are waiting for a multi-query
	/// or stored procedure response.
	///
	/// If this function returns true, you must call store_next() to
	/// fetch the next result set before you can execute more queries.
	///
	/// Wraps mysql_more_results() in the MySQL C API.  That function
	/// only exists in MySQL v4.1 and higher.  Therefore, this function
	/// always returns false when built against older API libraries.
	///
	/// \return true if another result set exists
	bool more_results();

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
	template <class Sequence>
	void storein_sequence(Sequence& con, query_reset r = RESET_QUERY)
	{
		storein_sequence(con, def, r);
	}

	/// \brief Execute a query, storing the result set in an STL
	/// associative container.
	///
	/// The same thing as storein_sequence(), except that it's used with
	/// associative STL containers, such as \c std::set.  Other than
	/// that detail, that method's comments apply equally well to this
	/// one.
	template <class Set>
	void storein_set(Set& con, query_reset r = RESET_QUERY)
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
	template <class Container>
	void storein(Container& con, query_reset r = RESET_QUERY)
	{
		storein(con, def, r);
	}

	/// \brief Specialization of storein_sequence() for \c std::vector
	template <class T>
	void storein(std::vector<T>& con, const char* s)
	{
		storein_sequence(con, s);
	}

	/// \brief Specialization of storein_sequence() for \c std::deque
	template <class T>
	void storein(std::deque<T>& con, const char* s)
	{
		storein_sequence(con, s);
	}

	/// \brief Specialization of storein_sequence() for \c std::list
	template <class T>
	void storein(std::list<T>& con, const char* s)
	{
		storein_sequence(con, s);
	}

#if defined(HAVE_EXT_SLIST)
	/// \brief Specialization of storein_sequence() for g++ STL
	/// extension \c slist
	template <class T>
	void storein(__gnu_cxx::slist<T>& con, const char* s)
	{
		storein_sequence(con, s);
	}
#elif defined(HAVE_GLOBAL_SLIST)
	/// \brief Specialization of storein_sequence() for STL
	/// extension \c slist
	///
	/// This is primarily for older versions of g++, which put \c slist
	/// in the global namespace.  This is a common language extension,
	/// so this may also work for other compilers.
	template <class T>
	void storein(slist<T>& con, const char* s)
	{
		storein_sequence(con, s);
	}
#elif defined(HAVE_STD_SLIST)
	/// \brief Specialization of storein_sequence() for STL
	/// extension \c slist
	///
	/// This is for those benighted compilers that include an \c slist
	/// implementation, but erroneously put it in the \c std namespace!
	template <class T>
	void storein(std::slist<T>& con, const char* s)
	{
		storein_sequence(con, s);
	}
#endif

	/// \brief Specialization of storein_set() for \c std::set
	template <class T>
	void storein(std::set<T>& con, const char* s)
	{
		storein_set(con, s);
	}

	/// \brief Specialization of storein_set() for \c std::multiset
	template <class T>
	void storein(std::multiset<T>& con, const char* s)
	{
		storein_set(con, s);
	}

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
	template <class T>
	Query& update(const T& o, const T& n)
	{
		reset();

		// Cast required for VC++ 2003 due to error in overloaded operator
		// lookup logic.  For an explanation of the problem, see:
		// http://groups-beta.google.com/group/microsoft.public.vc.stl/browse_thread/thread/9a68d84644e64f15
		MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
				"UPDATE " << o.table() << " SET " << n.equal_list() <<
				" WHERE " << o.equal_list(" AND ", sql_use_compare);
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
	template <class T>
	Query& insert(const T& v)
	{
		reset();

		MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
				"INSERT INTO " << v.table() << " (" <<
				v.field_list() << ") VALUES (" <<
				v.value_list() << ')';
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
	template <class Iter>
	Query& insert(Iter first, Iter last)
	{
		reset();
		if (first == last) {
			return *this;	// empty set!
		}
		
		MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
				"INSERT INTO " << first->table() << " (" <<
				first->field_list() << ") VALUES (" <<
				first->value_list() << ')';

		Iter it = first + 1;
		while (it != last) {
			MYSQLPP_QUERY_THISPTR << ",(" << it->value_list() << ')';
			++it;
		}

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
	template <class T>
	Query& replace(const T& v)
	{
		reset();

		MYSQLPP_QUERY_THISPTR << std::setprecision(16) <<
				"REPLACE INTO " << v.table() << " (" <<
				v.field_list() << ") VALUES (" << v.value_list() << ')';
		return *this;
	}

	/// \brief Return true if the last query was successful
	operator bool() { return success(); }

	/// \brief Return true if the last query failed
	bool operator !() { return !success(); }

#if !defined(DOXYGEN_IGNORE)
	// Declare the remaining overloads.  These are hidden down here partly
	// to keep the above code clear, but also so that we may hide them
	// from Doxygen, which gets confused by macro instantiations that look
	// like method declarations.
	mysql_query_define0(std::string, preview)
	mysql_query_define0(std::string, str)
	mysql_query_define1(ResNSel, execute)
	mysql_query_define1(Result, store)
	mysql_query_define1(ResUse, use)
	mysql_query_define2(storein_sequence)
	mysql_query_define2(storein_set)
	mysql_query_define2(storein)
#endif // !defined(DOXYGEN_IGNORE)

	/// \brief The default template parameters
	///
	/// Used for filling in parameterized queries.
	SQLQueryParms def;

private:
	friend class SQLQueryParms;

	/// \brief Connection to send queries through
	Connection* conn_;

	/// \brief If true, last query succeeded
	bool success_;

	/// \brief List of template query parameters
	std::vector<SQLParseElement> parse_elems_;

	/// \brief Maps template parameter position values to the
	/// corresponding parameter name.
	std::vector<std::string> parsed_names_;

	/// \brief Maps template parameter names to their position value.
	std::map<std::string, short int> parsed_nums_;

	/// \brief String buffer for storing assembled query
	std::stringbuf sbuffer_;

	//// Internal support functions
	my_ulonglong affected_rows() const;
	my_ulonglong insert_id();
	std::string info();
	char* preview_char();

	/// \brief Process a parameterized query list.
	void proc(SQLQueryParms& p);

	// Locking mechanism
	bool lock();
	void unlock();

	SQLString* pprepare(char option, SQLString& S, bool replace = true);
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

template <class Seq>
void Query::storein_sequence(Seq& seq, SQLQueryParms& p, query_reset r)
{
	r = parse_elems_.size() ? DONT_RESET : RESET_QUERY;
	storein_sequence(seq, str(p, r).c_str());
}


template <class Sequence>
void Query::storein_sequence(Sequence& con, const char* s)
{
	ResUse result = use(s);
	while (1) {
		MYSQL_ROW d = mysql_fetch_row(result.raw_result());
		if (!d)
			break;
		Row row(d, &result, mysql_fetch_lengths(result.raw_result()),
				true);
		if (!row)
			break;
		con.push_back(typename Sequence::value_type(row));
	}
}


template <class Set>
void Query::storein_set(Set& sett, SQLQueryParms& p, query_reset r)
{
	r = parse_elems_.size() ? DONT_RESET : RESET_QUERY;
	storein_set(sett, str(p, r).c_str());
}


template <class Set>
void Query::storein_set(Set& con, const char* s)
{
	ResUse result = use(s);
	while (1) {
		MYSQL_ROW d = mysql_fetch_row(result.raw_result());
		if (!d)
			return;
		Row row(d, &result, mysql_fetch_lengths(result.raw_result()),
				true);
		if (!row)
			break;
		con.insert(typename Set::value_type(row));
	}
}


template <class T>
void Query::storein(T& con, SQLQueryParms& p, query_reset r)
{
	r = parse_elems_.size() ? DONT_RESET : RESET_QUERY;
	storein(con, str(p, r).c_str());
}


#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif

