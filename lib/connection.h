#ifndef MYSQLPP_CONNECTION_H
#define MYSQLPP_CONNECTION_H

/// \file connection.h
/// \brief Declares the Connection class.
///
/// Every program using MySQL++ must create a Connection object, which
/// manages information about the connection to the MySQL database.  In
/// addition, this class controls things like whether exceptions are
/// thrown when errors are encountered.

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

class Connection {
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
	///
	/// Return true if the most recent query was successful
	bool success() const
	{
		return Success;
	}

	bool lock()
	{
		if (locked) {
			return true;
		}
		locked = true;
		return false;
	}
	void unlock() { locked = false; }

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

	/// \brief Return last error message
	///
	/// Simply wraps \c mysql_error() in the C API.
	const char *error() { return mysql_error(&mysql); }

	int errnum() { return mysql_errno(&mysql); }
	int refresh(unsigned int refresh_options)
	{
		return mysql_refresh(&mysql, refresh_options);
	}
	int ping() { return mysql_ping(&mysql); }
	int kill(unsigned long pid) { return mysql_kill(&mysql, pid); }

	std::string client_info()
	{
		return std::string(mysql_get_client_info());
	}

	std::string host_info()
	{
		return std::string(mysql_get_host_info(&mysql));
	}

	int proto_info() 
	{
		return mysql_get_proto_info(&mysql);
	}

	std::string server_info()
	{
		return std::string(mysql_get_server_info(&mysql));
	}

	std::string stat() { return std::string(mysql_stat(&mysql)); }

	Result store(const std::string& str)
	{
		return store(str, throw_exceptions);
	}
	ResUse use(const std::string& str) {
		return use(str, throw_exceptions);
	}
	ResNSel execute(const std::string& str) {
		return execute(str, throw_exceptions);
	}
	bool exec(const std::string& str);
	Result store(const std::string& str, bool te);
	ResUse use(const std::string& str, bool te);
	ResNSel execute(const std::string& str, bool te);

	bool create_db(std::string db)
	{
		return !execute("CREATE DATABASE " + db);
	}
	bool drop_db(std::string db)
	{
		return !execute("DROP DATABASE " + db);
	}
	bool select_db(std::string db) { return select_db(db.c_str()); }
	bool select_db(const char *db);
	bool reload();
	bool shutdown();
	std::string infoo() { return info(); }	// ! why do we need this?
	st_mysql_options get_options() const { return mysql.options; }
	int read_options(enum mysql_option option, const char *arg)
	{
		return mysql_options(&mysql, option, arg);
	}
	my_ulonglong affected_rows() { return mysql_affected_rows(&mysql); }
	my_ulonglong insert_id() { return mysql_insert_id(&mysql); }

	template <class Sequence>
	void storein_sequence(Sequence&, const std::string&);

	template <class Set>
	void storein_set(Set&, const std::string&);

	template <class T>
	void storein(std::vector<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}

	template <class T>
	void storein(std::deque<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}

	template <class T>
	void storein(std::list<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}

#if defined(HAVE_EXT_SLIST)
	template <class T>
	void storein(__gnu_cxx::slist<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}
#elif defined(HAVE_STD_SLIST)
	template <class T>
	void storein(slist<T>& con, const std::string& s)
	{
		storein_sequence(con, s);
	}
#endif

	template <class T>
	void storein(std::set<T>& con, const std::string& s)
	{
		storein_set(con, s);
	}

	template <class T>
	void storein(std::multiset<T>& con, const std::string& s)
	{
		storein_set(con, s);
	}
};


template <class Sequence>
void Connection::storein_sequence(Sequence& seq, const std::string& str)
{
	ResUse result = use(str);
	while (1) {
		MYSQL_ROW d = mysql_fetch_row(result.mysql_result());
		if (!d)
			break;
		Row row(d, &result, mysql_fetch_lengths(result.mysql_result()),
				true);
		if (!row)
			break;
		seq.push_back(typename Sequence::value_type(row));
	}
}

template <class Set>
void Connection::storein_set(Set& sett, const std::string& str)
{
	ResUse result = use(str);
	while (1) {
		MYSQL_ROW d = mysql_fetch_row(result.mysql_result());
		if (!d)
			return;
		Row row(d, &result, mysql_fetch_lengths(result.mysql_result()),
				true);
		if (!row)
			break;
		sett.insert(typename Set::value_type(row));
	}
}

}								// end namespace mysqlpp

#endif

