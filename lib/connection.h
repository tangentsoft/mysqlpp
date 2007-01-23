/// \file connection.h
/// \brief Declares the Connection class.
///
/// Every program using MySQL++ must create a Connection object, which
/// manages information about the connection to the MySQL database, and
/// performs connection-related operations once the connection is up.
/// Subordinate classes, such as Query and Row take their defaults as
/// to whether exceptions are thrown when errors are encountered from
/// the Connection object that created them, directly or indirectly.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004-2007 by Educational Technology Resources, Inc.
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

#include "common.h"

#include "lockable.h"
#include "noexceptions.h"

#include <mysql.h>

#include <deque>
#include <string>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT Query;
#endif

/// \brief Manages the connection to the MySQL database.

class MYSQLPP_EXPORT Connection : public OptionalExceptions, public Lockable
{
public:
	/// \brief Legal types of option arguments
	enum OptionArgType {
		opt_type_none,
		opt_type_string,
		opt_type_integer,
		opt_type_boolean
	};

	/// \brief Per-connection options you can set with set_option()
	///
	/// This is currently a combination of the MySQL C API
	/// \c mysql_option and \c enum_mysql_set_option enums.  It may
	/// be extended in the future.
	enum Option 
	{
		// Symbolic "first" option, before real options.  Never send
		// this to set_option()!
		opt_FIRST = -1,
		
		opt_connect_timeout = 0,
		opt_compress,
		opt_named_pipe,
		opt_init_command,
		opt_read_default_file,
		opt_read_default_group,
		opt_set_charset_dir,
		opt_set_charset_name,
		opt_local_infile,
		opt_protocol,
		opt_shared_memory_base_name,
		opt_read_timeout,
		opt_write_timeout,
		opt_use_result,
		opt_use_remote_connection,
		opt_use_embedded_connection,
		opt_guess_connection,
		opt_set_client_ip,
		opt_secure_auth,

		// Set multi-query statement support; no argument
		opt_multi_statements,

		// Set reporting of data truncation errors
		opt_report_data_truncation,

		// Enable or disable automatic reconnection to the server if
		// the connection is found to have been lost.
		opt_reconnect,

		// Number of options supported.  Never send this to
		// set_option()!
		opt_COUNT
	};

	/// \brief Create object without connecting it to the MySQL server.
	///
	/// \param te if true, exceptions are thrown on errors
	Connection(bool te = true);

	/// \brief Create object and connect to database server in one step.
	///
	/// This constructor allows you to most fully specify the options
	/// used when connecting to the MySQL database.  It is the thinnest
	/// layer in MySQL++ over the MySQL C API function
	/// \c mysql_real_connect().  The correspondence isn't exact as
	/// we have some additional parameters you'd have to set with
	/// \c mysql_option() when using the C API.
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
	/// \param socket_name Unix domain socket server is using, if
	///		connecting to MySQL server on the same host as this program
	///		running on, or 0 to use default name
	///	\param client_flag special connection flags. See MySQL C API
	///		documentation for \c mysql_real_connect() for details.
	Connection(const char* db, const char* host = "",
			const char* user = "", const char* passwd = "",
			uint port = 0, my_bool compress = 0,
			unsigned int connect_timeout = 60, cchar* socket_name = 0,
			unsigned int client_flag = 0);

	/// \brief Establish a new connection using the same parameters as
	/// an existing C API connection.
	///
	/// \param other existing Connection object
	Connection(const Connection& other);

	/// \brief Establish a new connection using the same parameters as
	/// an existing C API connection.
	///
	/// \param mysql existing MySQL C API connection object
	bool connect(const MYSQL& mysql);

	/// \brief Destroy connection object
	~Connection();

	/// \brief Connect to database after object is created.
	///
	/// It's better to use the connect-on-create constructor if you can.
	/// See its documentation for the meaning of these parameters.
	///
	/// If you call this method on an object that is already connected
	/// to a database server, the previous connection is dropped and a
	/// new connection is established.
	bool connect(cchar* db = "", cchar* host = "",
			cchar* user = "", cchar* passwd = "", uint port = 0,
			my_bool compress = 0, unsigned int connect_timeout = 60,
			cchar* socket_name = 0, unsigned int client_flag = 0);

	/// \brief Close connection to MySQL server.
	///
	/// Closes the connection to the MySQL server.
	void close()
	{
		mysql_close(&mysql_);
		is_connected_ = false;
	}
	
	/// \brief Calls MySQL C API function \c mysql_info() and returns
	/// result as a C++ string.
	std::string info();

	/// \brief return true if connection was established successfully
	///
	/// \return true if connection was established successfully
	bool connected() const
	{
		return is_connected_;
	}

	/// \brief Return true if the last query was successful
	bool success() const
	{
		return success_;
	}

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

	/// \brief Copy an existing Connection object's state into this
	/// object.
	Connection& operator=(const Connection& rhs);

	/// \brief Return error message for last MySQL error associated with
	/// this connection.
	///
	/// Simply wraps \c mysql_error() in the C API.
	const char* error()
	{
		return mysql_error(&mysql_);
	}

	/// \brief Return last MySQL error number associated with this
	/// connection
	///
	/// Simply wraps \c mysql_errno() in the C API.
	int errnum() { return mysql_errno(&mysql_); }

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
		return mysql_refresh(&mysql_, refresh_options);
	}

	/// \brief "Pings" the MySQL database
	///
	/// Wraps \c mysql_ping() in the C API.  As a result, this function
	/// will try to reconnect to the server if the connection has been
	/// dropped.
	/// 
	/// \retval 0 if server is responding, regardless of whether we had
	/// to reconnect or not
	/// \retval nonzero if either we already know the connection is down
	/// and cannot re-establish it, or if the server did not respond to
	/// the ping and we could not re-establish the connection.
	int ping();

	/// \brief Kill a MySQL server thread
	///
	/// \param pid ID of thread to kill
	///
	/// Simply wraps \c mysql_kill() in the C API.
	int kill(unsigned long pid)
	{
		return mysql_kill(&mysql_, pid);
	}

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
		return std::string(mysql_get_host_info(&mysql_));
	}

	/// \brief Returns version number of MySQL protocol this connection
	/// is using
	///
	/// Simply wraps \c mysql_get_proto_info() in the C API.
	int proto_info() 
	{
		return mysql_get_proto_info(&mysql_);
	}

	/// \brief Get the MySQL server's version number
	///
	/// Simply wraps \c mysql_get_server_info() in the C API.
	std::string server_info()
	{
		return std::string(mysql_get_server_info(&mysql_));
	}

	/// \brief Returns information about MySQL server status
	///
	/// String is similar to that returned by the \c mysqladmin
	/// \c status command.  Among other things, it contains uptime 
	/// in seconds, and the number of running threads, questions
	/// and open tables.
	std::string stat()
	{
		return std::string(mysql_stat(&mysql_));
	}

	/// \brief Create a database
	///
	/// \param db name of database to create
	///
	/// \return true if database was created successfully
	bool create_db(const std::string& db);

	/// \brief Drop a database
	///
	/// \param db name of database to destroy
	///
	/// \return true if database was created successfully
	bool drop_db(const std::string& db);

	/// \brief Change to a different database
	bool select_db(const std::string& db)
	{
		return select_db(db.c_str());
	}

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

	/// \brief Return the connection options object
	st_mysql_options get_options() const
	{
		return mysql_.options;
	}

	/// \brief Sets a connection option, with no argument
	///
	/// \param option any of the Option enum constants
	///
	/// Based on the option you give, this function calls either
	/// \c mysql_options() or \c mysql_set_server_option() in the C API.
	///
	/// There are several overloaded versions of this function.  The
	/// others take an additional argument for the option and differ
	/// only by the type of the option.  Unlike with the underlying C
	/// API, it does matter which of these overloads you call: if you
	/// use the wrong argument type or pass an argument where one is
	/// not expected (or vice versa), the call will either throw an
	/// exception or return false, depending on the object's "throw
	/// exceptions" flag.
	///
	/// This mechanism parallels the underlying C API structure fairly
	/// closely, but do not expect this to continue in the future.
	/// Its very purpose is to 'paper over' the differences among the
	/// C API's option setting mechanisms, so it may become further
	/// abstracted from these mechanisms.
	///
	/// \retval true if option was successfully set
	///
	/// If exceptions are enabled, a false return means the C API
	/// rejected the option, or the connection is not established and
	/// so the option was queued for later processing.  If exceptions
	/// are disabled, false can also mean that the argument was of the
	/// wrong type (wrong overload was called), the option value was out
	/// of range, or the option is not supported by the C API, most
	/// because it isn't a high enough version. These latter cases will
	/// cause BadOption exceptions otherwise.
	bool set_option(Option option);

	/// \brief Sets a connection option, with string argument
	bool set_option(Option option, const char* arg);

	/// \brief Sets a connection option, with integer argument
	bool set_option(Option option, unsigned int arg);

	/// \brief Sets a connection option, with Boolean argument
	bool set_option(Option option, bool arg);

	/// \brief Enable SSL-encrypted connection.
	///
	/// \param key the pathname to the key file
	/// \param cert the pathname to the certificate file
	/// \param ca the pathname to the certificate authority file
	/// \param capath directory that contains trusted SSL CA
	///        certificates in pem format.
    /// \param cipher list of allowable ciphers to use
	///
	/// Must be called before connection is established.
	///
	/// Wraps \c mysql_ssl_set() in MySQL C API.
	void enable_ssl(const char* key = 0,
			const char* cert = 0, const char* ca = 0,
			const char* capath = 0, const char* cipher = 0);

	/// \brief Return the number of rows affected by the last query
	///
	/// Simply wraps \c mysql_affected_rows() in the C API.
	my_ulonglong affected_rows()
	{
		return mysql_affected_rows(&mysql_);
	}

	/// \brief Get ID generated for an AUTO_INCREMENT column in the
	/// previous INSERT query.
	///
	/// \retval 0 if the previous query did not generate an ID.  Use
	/// the SQL function LAST_INSERT_ID() if you need the last ID
	/// generated by any query, not just the previous one.
	my_ulonglong insert_id()
	{
		return mysql_insert_id(&mysql_);
	}

	/// \brief Insert C API version we're linked against into C++ stream
	///
	/// Version will be of the form X.Y.Z, where X is the major version
	/// number, Y the minor version, and Z the bug fix number.
	std::ostream& api_version(std::ostream& os);

protected:
	/// \brief Drop the connection to the database server
	///
	/// This method is protected because it should only be used within
	/// the library.  Unless you use the default constructor, this
	/// object should always be connected.
	void disconnect();

	/// \brief Returns true if the given option is to be set once
	/// connection comes up.
	///
	/// \param option option to check for in queue
	/// \param arg argument to match against
	bool option_pending(Option option, bool arg) const;

	/// \brief For each option in pending option queue, call
	/// set_option()
	///
	/// Called within connect() method after connection is established.
	/// Despools options in the order given to set_option().
	void apply_pending_options();

	/// \brief Generic wrapper for bad_option_*()
	bool bad_option(Option option, OptionArgType type);

	/// \brief Handles call of incorrect set_option() overload
	bool bad_option_type(Option option);

	/// \brief Handles bad option values sent to set_option()
	bool bad_option_value(Option option);

	/// \brief Given option value, return its proper argument type
	OptionArgType option_arg_type(Option option);

	/// \brief Set MySQL C API connection option
	///
	/// Wraps \c mysql_options() in C API.  This is an internal
	/// implementation detail, to be used only by the public overloads
	/// above.
	bool set_option_impl(mysql_option moption, const void* arg = 0);

#if MYSQL_VERSION_ID >= 40101
	/// \brief Set MySQL C API connection option
	///
	/// Wraps \c mysql_set_server_option() in C API.  This is an
	/// internal implementation detail, to be used only by the public
	/// overloads above.
	bool set_option_impl(enum_mysql_set_option msoption);
#endif

	/// \brief Establish a new connection as a copy of an existing one
	///
	/// \param other the connection to copy
	void copy(const Connection& other);

private:
	friend class ResNSel;
	friend class ResUse;
	friend class Query;

	struct OptionInfo {
		Option option;
		OptionArgType arg_type;
		std::string str_arg;
		unsigned int int_arg;
		bool bool_arg;

		OptionInfo(Option o) :
		option(o),
		arg_type(opt_type_none),
		int_arg(0),
		bool_arg(false)
		{
		}

		OptionInfo(Option o, const char* a) :
		option(o),
		arg_type(opt_type_string),
		str_arg(a),
		int_arg(0),
		bool_arg(false)
		{
		}

		OptionInfo(Option o, unsigned int a) :
		option(o),
		arg_type(opt_type_integer),
		int_arg(a),
		bool_arg(false)
		{
		}

		OptionInfo(Option o, bool a) :
		option(o),
		arg_type(opt_type_boolean),
		int_arg(0),
		bool_arg(a)
		{
		}
	};

	MYSQL mysql_;
	bool is_connected_;
	bool connecting_;
	bool success_;
	std::deque<OptionInfo> pending_options_;
	static OptionArgType legal_opt_arg_types_[];
};


} // end namespace mysqlpp

#endif

