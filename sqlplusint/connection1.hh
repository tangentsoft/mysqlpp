#ifndef __connection1_hh__
#define __connection1_hh__
#ifdef __WIN32__
#include <Windows32/Base.h>
#include <Windows32/Defines.h>
#include <Windows32/Structures.h>
#include <winsock.h>
#define errno WSAGetLastError()
#endif
#include <mysql.h>
#include <vector.h>
#include <deque.h>
#include <list.h>
#include <slist.h>
#include <set.h>
#include <map.h>
#include <multiset.h>
#include "define_short"
#include "exceptions.hh"
#include "query1.hh"
#include "result1.hh"

//: The main database handle
class Connection {
  friend ResNSel;
  friend ResUse;
  friend Query;
  
private:
  bool throw_exceptions;
  MYSQL mysql;
  bool is_connected;
  bool locked;
  bool Success;
	
public: 
  Connection () : throw_exceptions(true), locked(false) 
    {mysql_init(&mysql);} //:
  Connection (bool te) : throw_exceptions(te), is_connected(false), locked(true), Success(false) //:
    {mysql_init(&mysql);} //:
  Connection (const char *db, const char *host = "", const char *user = "", 
	      const char *passwd = "", bool te = true); //:
  Connection (const char *db, const char *host, const char *user, 
	      const char *passwd, uint port, my_bool compress = 0,
	      unsigned int connect_timeout = 60, bool te = true,
	      cchar *socket_name = "",unsigned int client_flag=0); //:

  bool   real_connect (cchar *db = "", cchar *host = "", 
		       cchar *user = "", cchar *passwd = "", uint port = 0,
		       my_bool compress = 0, unsigned int connect_timeout = 60,
		       cchar *socket_name= "", unsigned int client_flag=0); //:
				
  ~Connection (); //:
  void         close() {mysql_close(&mysql);}	 //:
  string       info ();	//:

  bool   connected() const {return is_connected;}
  //: returns true if a successful connection was made

  bool   success() {return Success;}
  //: returns true of the last query was successful

  bool   connect (cchar *db = "", cchar *host = "", 
		  cchar *user = "", cchar *passwd = "");
  //:

  bool   lock() {if (locked) return true; locked = true; return false;}
  void   unlock() {locked = false;}

  void purge (void) {mysql_close(&mysql); }
  //:

  inline Query query();
  //:

  operator bool () {return success();}                  //: returns success()
  string error () {return string(mysql_error(&mysql));} //: last error message()
	int errnum () {return mysql_errno(&mysql);}
	int   refresh (unsigned int refresh_options){ return mysql_refresh (&mysql,refresh_options); }
	int ping (void) { return mysql_ping(&mysql);}
	int kill (unsigned long pid) { return mysql_kill (&mysql,pid);}
  string client_info () {return string(mysql_get_client_info());} //:
  
  string host_info () {return string(mysql_get_host_info(&mysql));} //:
  
  int    proto_info () {return mysql_get_proto_info(&mysql);} //:
  
  string server_info () {return string(mysql_get_server_info(&mysql));} //:
 
  string stat() {return string(mysql_stat(&mysql));} //:
 
  Result  store(const string &str) {return store(str, throw_exceptions);} //:
  ResUse  use(const string &str)   {return use(str, throw_exceptions);} //:
  ResNSel execute(const string &str) {return execute(str, throw_exceptions);} //:
	bool exec (const string &str);
  Result  store(const string &str, bool te); //:
  ResUse  use(const string &str, bool te); //:
  ResNSel execute(const string &str, bool te); //:
 
  bool   create_db (string db) {return !(execute( "CREATE DATABASE " + db ));} //:
  bool   drop_db (string db) {return !(execute( "DROP DATABASE " + db ));} //:
  bool   select_db (string db) {return select_db(db.c_str());} //:
  bool   select_db (const char *db); //:
  bool   reload(); //:
  bool   shutdown (); //:
	string infoo (void) {return info ();}
	st_mysql_options get_options (void) const {return mysql.options;}
	int read_options(enum mysql_option option,const char *arg) {return  mysql_options(&mysql, option,arg);}
  int          affected_rows()  {return mysql_affected_rows((MYSQL*) &mysql);}
  int          insert_id () {return mysql_insert_id(&mysql);}

  template <class Sequence> void storein_sequence(Sequence &, const string &); //:
  template <class Set>      void storein_set(Set &, const string &);  //:

  //!dummy: void storein(TYPE &con, const string &s);
  //: Stores the results in TYPE.  
  // Stores the result in TYPE. TYPE must be some sort of STL container.  

  template <class T>        void storein(vector<T> &con, const string &s)
    {storein_sequence(con,s);}
  template <class T>        void storein(deque<T> &con, const string &s)
     {storein_sequence(con,s);}
  template <class T>        void storein(list<T> &con, const string &s)
    {storein_sequence(con,s);}
  template <class T>        void storein(slist<T> &con, const string &s)
    {storein_sequence(con,s);}
  template <class T>        void storein(set<T> &con, const string &s)
    {storein_set(con,s);}
  template <class T>        void storein(multiset<T> &con, const string &s)
    {storein_set(con,s);}
};

//! with_class = Connection 

//: For backwards companablity, don't use
typedef Connection Mysql;

#endif





