#ifndef MYSQLPP_QUERY_H
#define MYSQLPP_QUERY_H

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

#include "defs.h"

#include "coldata.h"
#include "connection.h"
#include "result.h"
#include "sql_query.h"

#include <mysql.h>

#define mysql_query_define1(RETURN, FUNC) \
  RETURN FUNC (const char* str); \
  RETURN FUNC (parms &p);\
  mysql_query_define0(RETURN,FUNC) \

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
	std::string preview(parms& p) { return str(p); }

	/// \brief Execute an SQL query.
	///
	/// Use this method for queries that do not return data, and for
	/// which you only require a success or failure indication. See
	/// execute() if you need more information about the status of a
	/// query.  See store(), storein(), and use() for alternative
	/// query execution methods.
	///
	/// This method is basically a thin wrapper around the MySQL C
	/// API function \c mysql_query().
	///
	/// \param str the query to execute
	///
	/// \return true if query was executed successfully
	bool exec(const std::string& str);

	/// \fn ResNSel execute()
	/// \brief Execute a query and returns status information about the
	/// results of the query.
	///
	/// Use this method for queries that do not return data as such.
	/// (For example, CREATE INDEX, or OPTIMIZE TABLE.) See exec(),
	/// store(), storein(), and use() for alternative query execution
	/// methods.
	///
	/// There are a number of overloaded versions of this function. The
	/// one without parameters simply executes a query that you have
	/// built up in the object in some way. (For instance, via the
	/// insert() method, or by using the object's stream interface.) You
	/// can also pass the function an std::string containing a SQL query,
	/// a SQLQueryParms object, or as many as 12 SQLStrings. The latter
	/// two (or is it 13?) overloads are for filling out template queries.
	///
	/// See exec(), store(), storein_sequence(), storein_set() and use()
	/// for alternate query execution mechanisms.  
	///
	/// \return mysqlpp::ResNSel object containing the state
	/// information resulting from the query
	ResNSel execute() { return execute(def); }

	/// \fn ResUse use()
	/// \brief Execute a query returning data, when you wish to deal with
	/// the data one row at a time.
	///
	/// "Use" queries tell the server to send you the results one row at
	/// a time.  (The name comes from the MySQL C API function that
	/// initiates this process, \c mysql_use_result() .) This saves memory
	/// in the client program, but it potentially allows the client to tie
	/// up a connection with the database server until the result set is
	/// consumed.
	///
	/// See the documentation for execute() regarding the various
	/// overloads.  This function has the same set of overloads.
	///
	/// See exec(), execute(), store() and storein() for alternative
	/// query execution mechanisms.
	ResUse use() { return use(def); }

	/// \fn ResUse store()
	/// \brief Execute a query, and store the entire result set
	/// immediately in memory.
	///
	/// "Store" queries tell the server to send you the result set as
	/// a single block of data.  (The name comes from the MySQL C API
	/// function that initiates this process, \c mysql_store_result() .)
	/// Compared to "use" queries, this mechanism keeps your code simpler,
	/// and minimizes the amount of resources that the database server has
	/// to keep tied up for you.  The downside is, it can cause memory
	/// problems if the result set is sufficiently large.
	///
	/// See the documentation for execute() regarding the various
	/// overloads.  This function has the same set of overloads.
	///
	/// See exec(), execute(), storein(), and use() for alternative
	/// query execution mechanisms.
	Result store() { return store(def); }

	/// \brief Execute a query, and store the entire result set
	/// in the given STL sequence container.
	///
	/// This is a sort of marriage between "use" queries and "store"
	/// queries.  It is implemented with a "use" query, but it immediately
	/// consumes the results in a loop to populate the given sequence
	/// container. (For example, an \c std::vector.)
	///
	/// There are many overloads for this function, pretty much the same
	/// as for execute(), except that there is a Container parameter at
	/// the front of the list.  So, you can pass a container and a query
	/// string, or a container and template query parameters.
	///
	/// See exec(), execute(), store(), and use() for alternative
	/// query execution mechanisms.
	///
	/// \param con any STL sequence container, such as \c std::vector
	/// \param r whether the query automatically resets after being used
	template<class T1> void storein_sequence(T1& con,
			query_reset r = RESET_QUERY)
			{ storein_sequence_(con, def, r); }

	/// \brief Execute a query, and store the entire result set
	/// in the given associative STL container.
	///
	/// The same thing as storein_sequence(), except that it's used with
	/// associative STL containers, such as \c std::set.
	template<class T1> void storein_set(T1& con,
			query_reset r = RESET_QUERY) { storein_set(con, def, r); }

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
	template<class T1> void storein(T1& con,
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

	// Declare the remaining overloads.  These are hidden down here partly
	// to keep the above code clear, but also so that we may hide them
	// from Doxygen, which gets confused by macro instantiations that look
	// like method declarations.
	/// \if INTERNAL
	mysql_query_define0(std::string, preview)
	mysql_query_define1(ResNSel, execute)
	mysql_query_define1(ResUse, use)
	mysql_query_define1(Result, store)
	mysql_query_define2(storein_sequence)
	mysql_query_define2(storein_set)
	mysql_query_define2(storein)
	/// \endif
};

/// \if INTERNAL
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

/// \endif

} // end namespace mysqlpp

#endif

