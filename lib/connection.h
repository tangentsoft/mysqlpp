/// \file connection.h
/// \brief Declares the Connection class.
///
/// Every program using MySQL++ must create a Connection object, which
/// manages information about the connection to the MySQL database.  In
/// addition, this class controls things like whether exceptions are
/// thrown when errors are encountered.

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

#ifndef MYSQLPP_CONNECTION_H
#define MYSQLPP_CONNECTION_H

#include "platform.h"

#include "exceptions.h"
#include "result.h"

#include <mysql.h>

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>

#ifdef HAVE_EXT_SLIST
#  include <ext/slist>
#else
#  ifdef HAVE_STD_SLIST
#      include <slist>
#  endif
#endif

namespace mysqlpp {

class Query;

/// \brief Manages the connection to the MySQL database.

class Connection
{
private:
	friend class ResNSel;
	friend class ResUse;
	friend class Query;

	bool throw_exceptions;
	MYSQL mysql;
	bool is_connected;
	bool locked;
	bool Success;

public:
	/// \brief Create object without connecting it to the MySQL server.
	///
	/// Use real_connect() method to establish the connection.
	Connection();

	/// Same as
	/// \link mysqlpp::Connection::Connection() default ctor \endlink
	/// except that it allows you to choose whether exceptions are enabled.
	///
	/// \param te if true, exceptions are thrown on errors
	Connection(bool te);

	/// \brief For connecting to database without any special options.
	///
	/// This constructor takes the minimum parameters needed for most
	/// programs' use of MySQL.  There is a <a href="#a3">more complicated</a>
	/// constructor that lets you specify everything that the C API
	/// function \c mysql_real_connect() does.
	///
	/// \param db name of database to use
	/// \param host host name or IP address of MySQL server, or 0
	/// 	if server is running on the same host as your program
	/// \param user user name to log in under, or 0 to use the user
	///		name this program is running under
	/// \param passwd password to use when logging in
	/// \param te if true, throw exceptions on errors
	Connection(const char* db, const char* host = "",
			const char* user = "", const char* passwd = "",
			bool te = true);

	/// \brief Connect to database, allowing you to specify all
	/// connection parameters.
	///
	/// This constructor allows you to most fully specify the options
	/// used when connecting to the MySQL database.  It is the thinnest
	/// layer in MySQL++ over the MySQL C API function \c mysql_real_connect().
	///
	/// \param db name of database to use
	/// \param host host name or IP address of MySQL server, or 0
	/// 	if server is running on the same host as your program
	/// \param user user name to log in under, or 0 to use the user
	///		name this program is running under
	/// \param passwd password to use when logging in
	/// \param port TCP port number MySQL server is listening on, or 0
	///		to use default value
	/// \param compress if true, compress data passing through
	///		connection, to save bandwidth at the expense of CPU time
	/// \param connect_timeout max seconds to wait for server to
	///		respond to our connection attempt
	/// \param te if true, throw exceptions on errors
	/// \param socket_name Unix domain socket server is using, if
	///		connecting to MySQL server on the same host as this program
	///		running on, or 0 to use default name
	///	\param client_flag special connection flags. See MySQL C API
	///		documentation for \c mysql_real_connect() for details.
	Connection(const char* db, const char* host, const char* user,
			const char* passwd, uint port, my_bool compress = 0,
			unsigned int connect_timeout = 60, bool te = true,
			cchar* socket_name = 0, unsigned int client_flag = 0);

	~Connection();

	/// \brief Open connection to MySQL database
	///
	/// Open connection to the MySQL server, using defaults for all but the
	/// most common parameters.  It's better to use one of the
	/// connect-on-create constructors if you can.
	///
	/// See <a href="#a2">this</a> for parameter documentation.
	bool connect(cchar* db = "", cchar* host = "", cchar* user = "",
			cchar* passwd = "");

	/// \brief Connect to database after object is created.
	///
	/// It's better to use one of the connect-on-create constructors
	/// if you can.
	///
	/// Despite the name, this function is not a direct wrapper for the
	/// MySQL C API function \c mysql_real_connect(). It also sets some
	/// connection-related options using \c mysql_options().
	///
	/// See <a href="#a3">this</a> for parameter documentation.
	bool real_connect(cchar* db = "", cchar* host = "",
			cchar* user = "", cchar* passwd = "", uint port = 0,
			my_bool compress = 0, unsigned int connect_timeout = 60,
			cchar* socket_name = 0, unsigned int client_flag = 0);

	/// \brief Close connection to MySQL server.
	///
	/// Closes the connection to the MySQL server.
	void close()
	{
		mysql_close(&mysql);
		is_connected = false;
	}
	
	/// \brief Calls MySQL C API function \c mysql_info() and returns
	/// result as a C++ string.
	std::string info();

	/// \brief return true if connection was established successfully
	///
	/// \return true if connection was established successfully
	bool connected() const
	{
		return is_connected;
	}

	/// \brief Return true if the last query was successful
	bool success() const
	{
		return Success;
	}

	/// \brief Lock the object.
	///
	/// Apparently supposed to prevent multiple simultaneous connections
	/// since only connection-related functions change the lock status
	/// but it's never actually checked!  See Wishlist for plan to fix
	/// this.
	bool lock()
	{
		if (locked) {
			return true;
		}
		locked = true;
		return false;
	}

	/// \brief Unlock the object
	///
	/// See lock() documentation for caveats.
	void unlock() { locked = false; }

	/// \brief Alias for close()
	void purge() { close(); }

	/// \brief Return a new query object.
	///
	/// The returned query object is tied to this MySQL connection,
	/// so when you call a method like
	/// \link mysqlpp::Query::execute() execute() \endlink
	/// on that object, the query is sent to the server this object
	/// is connected to.
	Query query();

	/// \brief Alias for success()
	///
	/// Alias for success() member function. Allows you to have code
	/// constructs like this:
	///
	/// \code
	///	    Connection conn;
	///	    .... use conn
	///	    if (conn) {
	///	        ... last SQL query was successful
	///	    }
	///	    else {
	///	        ... error occurred in SQL query
	///	    }
	/// \endcode
	operator bool() { return success(); }

	/// \brief Return error message for last MySQL error associated with
	/// this connection.
	///
	/// Simply wraps \c mysql_error() in the C API.
	const char *error() { return mysql_error(&mysql); }

	/// \brief Return last MySQL error number associated with this
	/// connection
	///
	/// Simply wraps \c mysql_errno() in the C API.
	int errnum() { return mysql_errno(&mysql); }

	/// \brief Wraps MySQL C API function \c mysql_refresh()
	///
	/// The corresponding C API function is undocumented.  All I know
	/// is that it's used by \c mysqldump and \c mysqladmin, according
	/// to MySQL bug database entry http://bugs.mysql.com/bug.php?id=9816
	/// If that entry changes to say that the function is now documented,
	/// reevaluate whether we need to wrap it.  It may be that it's not
	/// supposed to be used by regular end-user programs.
	int refresh(unsigned int refresh_options)
	{
		return mysql_refresh(&mysql, refresh_options);
	}

	/// \brief "Pings" the MySQL database
	///
	/// If server doesn't respond, this function tries to reconnect.
	/// 
	/// \retval 0 if server is responding, regardless of whether we had
	/// to reconnect or not
	/// \retval nonzero if server did not respond to ping and we could
	/// not re-establish the connection
	///
	/// Simply wraps \c mysql_ping() in the C API.
	int ping() { return mysql_ping(&mysql); }

	/// \brief Kill a MySQL server thread
	///
	/// \param pid ID of thread to kill
	///
	/// Simply wraps \c mysql_kill() in the C API.
	int kill(unsigned long pid) { return mysql_kill(&mysql, pid); }

	/// \brief Get MySQL client library version
	///
	/// Simply wraps \c mysql_get_client_info() in the C API.
	std::string client_info()
	{
		return std::string(mysql_get_client_info());
	}

	/// \brief Get information about the network connection
	///
	/// String contains info about type of connection and the server
	/// hostname.
	///
	/// Simply wraps \c mysql_get_host_info() in the C API.
	std::string host_info()
	{
		return std::string(mysql_get_host_info(&mysql));
	}

	/// \brief Returns version number of MySQL protocol this connection
	/// is using
	///
	/// Simply wraps \c mysql_get_proto_info() in the C API.
	int proto_info() 
	{
		return mysql_get_proto_info(&mysql);
	}

	/// \brief Get the MySQL server's version number
	///
	/// Simply wraps \c mysql_get_server_info() in the C API.
	std::string server_info()
	{
		return std::string(mysql_get_server_info(&mysql));
	}

	/// \brief Returns information about MySQL server status
	///
	/// String is similar to that returned by the \c mysqladmin
	/// \c status command.  Among other things, it contains uptime 
	/// in seconds, and the number of running threads, questions
	/// and open tables.
	std::string stat() { return std::string(mysql_stat(&mysql)); }

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
	/// \return Result object containing entire result set
	///
	/// \sa exec(), execute(), storein(), and use()
	Result store(const std::string& str)
	{
		return store(str, throw_exceptions);
	}

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
	/// \return ResUse object that can walk through result set serially
	///
	/// \sa exec(), execute(), store() and storein()
	ResUse use(const std::string& str)
	{
		return use(str, throw_exceptions);
	}

	/// \brief Execute a query
	///
	/// Use this function if you don't expect the server to return a
	/// result set.  For instance, a DELETE query.  The returned ResNSel
	/// object contains status information from the server, such as
	/// whether the query succeeded, and if so how many rows were
	/// affected.
	///
	/// \param str the query to execute
	///
	/// \return ResNSel status information about the query
	///
	/// \sa exec(), store(), storein(), and use()
	ResNSel execute(const std::string& str)
	{
		return execute(str, throw_exceptions);
	}

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

	/// \brief Same as store(str) except that it allows you to turn off
	/// exceptions for this query
	///
	/// \param str query to execute
	/// \param te if true, no exceptions will be thrown on errors.
	Result store(const std::string& str, bool te);

	/// \brief Same as use(str) except that it allows you to turn off
	/// exceptions for this query
	///
	/// \param str query to execute
	/// \param te if true, no exceptions will be thrown on errors.
	ResUse use(const std::string& str, bool te);

	/// \brief Same as execute(str) except that it allows you to turn off
	/// exceptions for this query
	///
	/// \param str query to execute
	/// \param te if true, no exceptions will be thrown on errors.
	ResNSel execute(const std::string& str, bool te);

	/// \brief Create a database
	///
	/// \param db name of database to create
	///
	/// \return \e false if database was created successfully. (Yes,
	/// false! This behavior will change in the next major version.)
	bool create_db(std::string db)
	{
		return !execute("CREATE DATABASE " + db);
	}

	/// \brief Drop a database
	///
	/// \param db name of database to destroy
	///
	/// \return \e false if database was created successfully. (Yes,
	/// false! This behavior will change in the next major version.)
	bool drop_db(std::string db)
	{
		return !execute("DROP DATABASE " + db);
	}

	/// \brief Change to a different database
	bool select_db(std::string db) { return select_db(db.c_str()); }

	/// \brief Change to a different database
	bool select_db(const char* db);

	/// \brief Ask MySQL server to reload the grant tables
	/// 
	/// User must have the "reload" privilege.
	///
	/// Simply wraps \c mysql_reload() in the C API.  Since that
	/// function is deprecated, this one is, too.  The MySQL++
	/// replacement is execute("FLUSH PRIVILEGES").
	bool reload();
	
	/// \brief Ask MySQL server to shut down.
	///
	/// User must have the "shutdown" privilege.
	///
	/// Simply wraps \c mysql_shutdown() in the C API.
	bool shutdown();

	/// \brief Alias for info()
	///
	/// This probably shouldn't exist.  It will be removed in the next
	/// major version of the library unless a good justification is
	/// found.
	std::string infoo() { return info(); }

	/// \brief Return the connection options object
	st_mysql_options get_options() const { return mysql.options; }

	/// \brief Sets the given MySQL connection option
	///
	/// Not sure why this is named as it is; set_option() would be a
	/// better name.  It may change in the next major version of the
	/// library.
	///
	/// Simply wraps \c mysql_option() in the C API.
	int read_options(enum mysql_option option, const char* arg)
	{
		return mysql_options(&mysql, option, arg);
	}

	/// \brief Return the number of rows affected by the last query
	///
	/// Simply wraps \c mysql_affected_rows() in the C API.
	my_ulonglong affected_rows() { return mysql_affected_rows(&mysql); }

	/// \brief Get ID generated for an AUTO_INCREMENT column in the
	/// previous INSERT query.
	///
	/// \retval 0 if the previous query did not generate an ID.  Use
	/// the SQL function LAST_INSERT_ID() if you need the last ID
	/// generated by any query, not just the previous one.
	my_ulonglong insert_id() { return mysql_insert_id(&mysql); }

	/// \brief Execute a query, storing the result set in an STL
	/// sequence container.
	///
	/// This function works much like store() from the caller's
	/// perspective, because it returns the entire result set at once.
	/// It's actually implemented in terms of use(), however, so that
	/// memory for the result set doesn't need to be allocated twice.
	///
	/// \param con any STL sequence container, such as \c std::vector
	/// \param s query string to execute
	///
	/// \sa exec(), execute(), store(), and use()
	template <class Sequence>
	void storein_sequence(Sequence& con, const std::string& s);

	/// \brief Execute a query, storing the result set in an STL
	/// associative container.
	///
	/// \param con any STL associative container, such as \c std::set
	/// \param s query string to execute
	///
	/// The same thing as storein_sequence(), except that it's used with
	/// associative STL containers, such as \c std::set.  Other than
	/// that detail, that method's comments apply equally well to this
	/// one.
	template <class Set>
	void storein_set(Set& con, const std::string& s);

	/// \brief Specialization of storein_sequence() for \c std::vector
	template <class T>
	void storein(std::vector<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}

	/// \brief Specialization of storein_sequence() for \c std::deque
	template <class T>
	void storein(std::deque<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}

	/// \brief Specialization of storein_sequence() for \c std::list
	template <class T>
	void storein(std::list<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}

#if defined(HAVE_EXT_SLIST)
	/// \brief Specialization of storein_sequence() for g++ STL
	/// extension \c slist
	template <class T>
	void storein(__gnu_cxx::slist<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}
#elif defined(HAVE_STD_SLIST)
	/// \brief Specialization of storein_sequence() for STL
	/// extension \c slist
	///
	/// This is primarily for older versions of g++, which didn't put
	/// \c slist in a private namespace.  This is a common language
	/// extension, so this may also work for other compilers.
	template <class T>
	void storein(slist<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}
#endif

	/// \brief Specialization of storein_set() for \c std::set
	template <class T>
	void storein(std::set<T>& con, const std::string& s)
	{
		storein_set(con, s);
	}

	/// \brief Specialization of storein_set() for \c std::multiset
	template <class T>
	void storein(std::multiset<T>& con, const std::string& s)
	{
		storein_set(con, s);
	}
};


template <class Sequence>
void Connection::storein_sequence(Sequence& con, const std::string& s)
{
	ResUse result = use(s);
	while (1) {
		MYSQL_ROW d = mysql_fetch_row(result.mysql_result());
		if (!d)
			break;
		Row row(d, &result, mysql_fetch_lengths(result.mysql_result()),
				true);
		if (!row)
			break;
		con.push_back(typename Sequence::value_type(row));
	}
}

template <class Set>
void Connection::storein_set(Set& con, const std::string& s)
{
	ResUse result = use(s);
	while (1) {
		MYSQL_ROW d = mysql_fetch_row(result.mysql_result());
		if (!d)
			return;
		Row row(d, &result, mysql_fetch_lengths(result.mysql_result()),
				true);
		if (!row)
			break;
		con.insert(typename Set::value_type(row));
	}
}

} // end namespace mysqlpp

#endif

