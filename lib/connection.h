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

class MysqlChild {
  public:
	virtual void parent_leaving() = 0;
	virtual ~MysqlChild() { }
};

//: The main database handle
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
	Connection();
	Connection(bool te);
	Connection(const char* db, const char* host = "",
			const char* user = "", const char* passwd = "",
			bool te = true);
	Connection(const char* db, const char* host, const char* user,
			const char* passwd, uint port, my_bool compress = 0,
			unsigned int connect_timeout = 60, bool te = true,
			cchar* socket_name = "", unsigned int client_flag = 0);
	~Connection();

	bool real_connect(cchar* db = "", cchar* host = "",
			cchar* user = "", cchar* passwd = "", uint port = 0,
			my_bool compress = 0, unsigned int connect_timeout = 60,
			cchar* socket_name = "", unsigned int client_flag = 0);

	void close()
	{
		mysql_close(&mysql);
		is_connected = false;
	}
	
	std::string info();

	//: returns true if a successful connection was made
	bool connected() const
	{
		return is_connected;
	}

	//: returns true of the last query was successful
	bool success() const
	{
		return Success;
	}

	bool connect(cchar* db = "", cchar* host = "", cchar* user = "",
			cchar* passwd = "");

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

	Query query();

	//: returns success()
	operator bool() { return success(); }

	//: last error message()
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
	std::string infoo() { return info(); }	//! what's the justification?
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

