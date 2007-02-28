/***********************************************************************
 connection.cpp - Implements the Connection class.

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

#define MYSQLPP_NOT_HEADER
#include "common.h"

#include "connection.h"

#include "query.h"
#include "result.h"

#if defined(HAVE_MYSQL_SHUTDOWN_LEVEL_ARG)
#	define SHUTDOWN_ARG ,SHUTDOWN_DEFAULT
#else
#	define SHUTDOWN_ARG
#endif

using namespace std;

namespace mysqlpp {

/// \brief Sets a variable to a given value temporarily.
///
/// Saves existing value, sets new value, and restores old value when
/// the object is destroyed.  Used to set a flag in an exception-safe
/// manner.
template <class T>
class scoped_var_set
{
public:
	/// \brief Create object, saving old value, setting new value
	scoped_var_set(T& var, T new_value) :
	var_(var)
	{
		old_value_ = var_;
		var_ = new_value;
	}

	/// \brief Destroy object, restoring old value
	~scoped_var_set()
	{
		var_ = old_value_;
	}

private:
	T& var_;
	T old_value_;
};


// Initialize table of legal option argument types.
Connection::OptionArgType
Connection::legal_opt_arg_types_[Connection::opt_COUNT] = {
	Connection::opt_type_integer,	// opt_connect_timeout
	Connection::opt_type_none,		// opt_compress
	Connection::opt_type_none,		// opt_named_pipe
	Connection::opt_type_string,	// opt_init_command
	Connection::opt_type_string,	// opt_read_default_file
	Connection::opt_type_string,	// opt_read_default_group
	Connection::opt_type_string,	// opt_set_charset_dir
	Connection::opt_type_string,	// opt_set_charset_name
	Connection::opt_type_integer,	// opt_local_infile
	Connection::opt_type_integer,	// opt_protocol
	Connection::opt_type_string,	// opt_shared_memory_base_name
	Connection::opt_type_integer,	// opt_read_timeout
	Connection::opt_type_integer,	// opt_write_timeout
	Connection::opt_type_none,		// opt_use_result
	Connection::opt_type_none,		// opt_use_remote_connection
	Connection::opt_type_none,		// opt_use_embedded_connection
	Connection::opt_type_none,		// opt_guess_connection
	Connection::opt_type_string,	// opt_set_client_ip
	Connection::opt_type_boolean,	// opt_secure_auth
	Connection::opt_type_boolean,	// opt_multi_statements
	Connection::opt_type_boolean,	// opt_report_data_truncation
	Connection::opt_type_boolean,   // opt_reconnect
};


Connection::Connection(bool te) :
OptionalExceptions(te),
Lockable(false),
is_connected_(false),
connecting_(false),
success_(false)
{
	mysql_init(&mysql_);
}


Connection::Connection(const char* db, const char* host,
		const char* user, const char* passwd, uint port,
		my_bool compress, unsigned int connect_timeout,
		cchar* socket_name, unsigned int client_flag) :
OptionalExceptions(),
Lockable(false),
connecting_(false)
{
	mysql_init(&mysql_);
	if (connect(db, host, user, passwd, port, compress,
			connect_timeout, socket_name, client_flag)) {
		unlock();
		success_ = is_connected_ = true;
	}
	else {
		unlock();
		success_ = is_connected_ = false;
		if (throw_exceptions()) {
			throw ConnectionFailed(error());
		}
	}
}


Connection::Connection(const Connection& other) :
OptionalExceptions(),
Lockable(false),
is_connected_(false)
{
	copy(other);
}


Connection::~Connection()
{
	disconnect();
}


Connection&
Connection::operator=(const Connection& rhs)
{
	copy(rhs);
	return *this;
}


bool
Connection::connect(cchar* db, cchar* host, cchar* user,
		cchar* passwd, uint port, my_bool compress,
		unsigned int connect_timeout, cchar* socket_name,
		unsigned int client_flag)
{
	lock();

	// Drop previous connection, if any
	if (connected()) {
		disconnect();
	}

	// Set pre-connection options
	scoped_var_set<bool> sb(connecting_, true);
	set_option(opt_read_default_file, "my");
	set_option(opt_connect_timeout, connect_timeout);
	if (compress) {
		set_option(opt_compress);
	}
#if MYSQL_VERSION_ID >= 40101
	if (option_pending(opt_multi_statements, true)) {
		client_flag |= CLIENT_MULTI_STATEMENTS;
	}
#endif

	// Establish connection
	if (mysql_real_connect(&mysql_, host, user, passwd, db, port,
			socket_name, client_flag)) {
		unlock();
		success_ = is_connected_ = true;
		apply_pending_options();

		if (db && db[0]) {
			// Also attach to given database
			success_ = select_db(db);
		}
	}
	else {
		unlock();
		success_ = is_connected_ = false;
		if (throw_exceptions()) {
			throw ConnectionFailed(error());
		}
	}

	return success_;
}


bool
Connection::connect(const MYSQL& mysql)
{
	return connect(mysql.db, mysql.host, mysql.user, mysql.passwd,
			mysql.port, mysql.options.compress,
			mysql.options.connect_timeout, mysql.unix_socket,
			mysql.client_flag);
}


void
Connection::copy(const Connection& other)
{
	if (connected()) {
		disconnect();
	}

	mysql_init(&mysql_);
	set_exceptions(other.throw_exceptions());

	if (other.connected()) {
		// Try to reconnect to server using same parameters
		connect(other.mysql_);
	}
	else {
		is_connected_ = false;
		connecting_ = false;
		success_ = false;
	}
}


void
Connection::disconnect()
{
	mysql_close(&mysql_);
	is_connected_ = false;
}


bool
Connection::create_db(const std::string& db)
{
	Query q(this, throw_exceptions());
	return q.exec("CREATE DATABASE " + db);
}


bool
Connection::drop_db(const std::string& db)
{
	Query q(this, throw_exceptions());
	return q.exec("DROP DATABASE " + db);
}


bool
Connection::select_db(const char *db)
{
	if (connected()) {
		bool suc = !(mysql_select_db(&mysql_, db));
		if (throw_exceptions() && !suc) {
			throw DBSelectionFailed(error());
		}
		else {
			return suc;
		}
	}
	else {
		if (throw_exceptions()) {
			throw DBSelectionFailed("MySQL++ connection not established");
		}
		else {
			return false;
		}
	}
}


bool
Connection::reload()
{
	if (connected()) {
		bool suc = !mysql_reload(&mysql_);
		if (throw_exceptions() && !suc) {
			// Reloading grant tables through this API isn't precisely a
			// query, but it's acceptable to signal errors with BadQuery
			// because the new mechanism is the FLUSH PRIVILEGES query.
			// A program won't have to change when doing it the new way.
			throw BadQuery(error());
		}
		else {
			return suc;
		}
	}
	else {
		if (throw_exceptions()) {
			throw BadQuery("MySQL++ connection not established");
		}
		else {
			return false;
		}
	}
}


bool
Connection::shutdown()
{
	if (connected()) {
		bool suc = !(mysql_shutdown(&mysql_ SHUTDOWN_ARG));
		if (throw_exceptions() && !suc) {
			throw ConnectionFailed(error());
		}
		else {
			return suc;
		}
	}
	else {
		if (throw_exceptions()) {
			throw ConnectionFailed("MySQL++ connection not established");
		}
		else {
			return false;
		}
	}
}


string
Connection::info()
{
	const char* i = mysql_info(&mysql_);
	if (!i) {
		return string();
	}
	else {
		return string(i);
	}
}


Query
Connection::query()
{
	return Query(this, throw_exceptions());
}


bool
Connection::set_option(Option option)
{
	if (connected() || connecting_) {
		// Try to set the option immediately
		switch (option) {
			case opt_compress:
				return set_option_impl(MYSQL_OPT_COMPRESS);

			case opt_named_pipe:
				return set_option_impl(MYSQL_OPT_NAMED_PIPE);

#if MYSQL_VERSION_ID >= 40101
			case opt_use_result:
				return set_option_impl(MYSQL_OPT_USE_RESULT);

			case opt_use_remote_connection:
				return set_option_impl(MYSQL_OPT_USE_REMOTE_CONNECTION);

			case opt_use_embedded_connection:
				return set_option_impl(MYSQL_OPT_USE_EMBEDDED_CONNECTION);

			case opt_guess_connection:
				return set_option_impl(MYSQL_OPT_GUESS_CONNECTION);
#endif
			default:
				return bad_option(option, opt_type_none);
		}
	}
	else {
		// Connection isn't established yet, so save option in queue.
		// See apply_pending_options() for despooling logic.
		pending_options_.push_back(OptionInfo(option));
		return false;
	}
}


bool
Connection::set_option(Option option, const char* arg)
{
	if (connected() || connecting_) {
		// Try to set the option immediately
		switch (option) {
			case opt_init_command:
				return set_option_impl(MYSQL_INIT_COMMAND, arg);

			case opt_read_default_file:
				return set_option_impl(MYSQL_READ_DEFAULT_FILE, arg);

			case opt_read_default_group:
				return set_option_impl(MYSQL_READ_DEFAULT_GROUP, arg);

			case opt_set_charset_dir:
				return set_option_impl(MYSQL_SET_CHARSET_DIR, arg);

			case opt_set_charset_name:
				return set_option_impl(MYSQL_SET_CHARSET_NAME, arg);

#if MYSQL_VERSION_ID >= 40100
			case opt_shared_memory_base_name:
				return set_option_impl(MYSQL_SHARED_MEMORY_BASE_NAME, arg);
#endif

#if MYSQL_VERSION_ID >= 40101
			case opt_set_client_ip:
				return set_option_impl(MYSQL_SET_CLIENT_IP, arg);
#endif
			default:
				return bad_option(option, opt_type_string);
		}
	}
	else {
		// Connection isn't established yet, so save option in queue.
		// See apply_pending_options() for despooling logic.
		pending_options_.push_back(OptionInfo(option, arg));
		return false;
	}
}


bool
Connection::set_option(Option option, unsigned int arg)
{
	if (connected() || connecting_) {
		// Try to set the option immediately
		switch (option) {
			case opt_connect_timeout:
				return set_option_impl(MYSQL_OPT_CONNECT_TIMEOUT, &arg);

			case opt_local_infile:
				return set_option_impl(MYSQL_OPT_LOCAL_INFILE, &arg);

#if MYSQL_VERSION_ID >= 40100
			case opt_protocol:
				return set_option_impl(MYSQL_OPT_PROTOCOL, &arg);
#endif
#if MYSQL_VERSION_ID >= 40101
			case opt_read_timeout:
				return set_option_impl(MYSQL_OPT_READ_TIMEOUT, &arg);

			case opt_write_timeout:
				return set_option_impl(MYSQL_OPT_WRITE_TIMEOUT, &arg);
#endif
			default:
				return bad_option(option, opt_type_integer);
		}
	}
	else {
		// Connection isn't established yet, so save option in queue.
		// See apply_pending_options() for despooling logic.
		pending_options_.push_back(OptionInfo(option, arg));
		return false;
	}
}


bool
Connection::set_option(Option option, bool arg)
{
	if (connected() || connecting_) {
		// Try to set the option immediately
		my_bool my_arg = arg;
		switch (option) {
#if MYSQL_VERSION_ID >= 40101
			case opt_secure_auth:
				return set_option_impl(MYSQL_SECURE_AUTH, &my_arg);

			case opt_multi_statements:
				return set_option_impl(arg ?
						MYSQL_OPTION_MULTI_STATEMENTS_ON :
						MYSQL_OPTION_MULTI_STATEMENTS_OFF);
#endif
#if MYSQL_VERSION_ID >= 50003
			case opt_report_data_truncation:
				return set_option_impl(MYSQL_REPORT_DATA_TRUNCATION, &my_arg);
#endif
#if MYSQL_VERSION_ID >= 50013
			case opt_reconnect:
				return set_option_impl(MYSQL_OPT_RECONNECT, &my_arg);
#endif
			case opt_FIRST:	// warning eater when using old C APIs
			default:
				return bad_option(option, opt_type_boolean);
		}
	}
	else {
		// Connection isn't established yet, so save option in queue.
		// See apply_pending_options() for despooling logic.
		pending_options_.push_back(OptionInfo(option, arg));
		return false;
	}
}


bool
Connection::set_option_impl(mysql_option moption, const void* arg)
{
	return !mysql_options(&mysql_, moption,
			static_cast<const char*>(arg));
}


#if MYSQL_VERSION_ID >= 40101
bool
Connection::set_option_impl(enum_mysql_set_option msoption)
{
	return !mysql_set_server_option(&mysql_, msoption);
}
#endif


bool
Connection::bad_option(Option option, OptionArgType type)
{
	if (option_arg_type(option) == type) {
		return bad_option_value(option);
	}
	else {
		return bad_option_type(option);
	}
}


bool
Connection::bad_option_type(Option option)
{
	if (throw_exceptions()) {
		// Option value is legal, but it was given the wrong argument
		// type.
		OptionArgType type = option_arg_type(option);
		ostringstream os;
		os << "option " << option;
		if (type == opt_type_none) {
			os << " does not take an argument";
		}
		else {
			os << " requires an argument of type " << type;
		}
		throw BadOption(os.str(), option);
	}

	return false;
}


bool
Connection::bad_option_value(Option option)
{
	if (throw_exceptions()) {
		// If we get here, option value is in range and it was given the
		// right argument type, so the reason the option was unhandled
		// must be that it's not supported in the C API we're built
		// against.
		ostringstream os;
		os << "option " << option << " not supported in MySQL C API v";
		api_version(os);
		throw BadOption(os.str(), option);
	}

	return false;
}


Connection::OptionArgType
Connection::option_arg_type(Option option)
{
	if ((option > opt_FIRST) && (option < opt_COUNT)) {
		return legal_opt_arg_types_[option];
	}
	else {
		// Non-optional exception.  Something is wrong with the library
		// internals if this one is thrown.
		BadOption("bad value given to option_arg_type()", option);
		return opt_type_none;		// warning eater
	}
}


void
Connection::apply_pending_options()
{
	if (pending_options_.size() == 0) {
		return;
	}

	for (deque<OptionInfo>::const_iterator it(pending_options_.begin());
			it != pending_options_.end(); ++it) {
		// Try to set the option
		bool success;
		switch (it->arg_type) {
			case opt_type_none:
			default:
				success = set_option(it->option);
				break;

			case opt_type_string:
				success = set_option(it->option, it->str_arg.c_str());
				break;

			case opt_type_integer:
				success = set_option(it->option, it->int_arg);
				break;

			case opt_type_boolean:
				success = set_option(it->option, it->bool_arg);
				break;
		}

		// If option failed and exceptions are enabled, throw an
		// exception, because otherwise the failure will be lost.
		if (!success && throw_exceptions()) {
			ostringstream os;
			os << "Failed to set pending option " << it->option;
			if (it->arg_type != opt_type_none) {
				os << " = ";
				switch (it->arg_type) {
					case opt_type_string:	os << it->str_arg; break;
					case opt_type_integer:	os << it->int_arg; break;
					case opt_type_boolean:	os << it->bool_arg; break;
					case opt_type_none:		// warning eater
					default:				os << "unknown"; break;
				}
			}
			throw BadOption(os.str(), it->option);
		}
	}

	pending_options_.clear();
}


bool
Connection::option_pending(Option option, bool arg) const
{
	for (deque<OptionInfo>::const_iterator it(pending_options_.begin());
			it != pending_options_.end(); ++it) {
		if (it->option == option) {
			// Found the option, but return true only if the pending
			// option was given a bool argument equal to the value
			// passed to this function.
			return it->arg_type == opt_type_boolean &&
					it->bool_arg == arg;
		}
	}

	return false;
}


void
Connection::enable_ssl(const char* key, const char* cert,
		const char* ca, const char* capath, const char* cipher)
{
#if defined(HAVE_MYSQL_SSL_SET)
	mysql_ssl_set(&mysql_, key, cert, ca, capath, cipher);
#endif
}


ostream&
Connection::api_version(ostream& os)
{
	const int major = MYSQL_VERSION_ID / 10000;
	const int minor = (MYSQL_VERSION_ID - (major * 10000)) / 100;
	const int bug = MYSQL_VERSION_ID - (major * 10000) - (minor * 100);

	os << major << '.' << minor << '.' << bug;

	return os;
}


int
Connection::ping()
{
	if (connected()) {
		return mysql_ping(&mysql_);
	}
	else {
		// Not connected, and we've forgotten everything we need in
		// order to re-connect, if we once were connected.
		return 1;
	}
}


} // end namespace mysqlpp

