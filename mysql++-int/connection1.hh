#ifndef __connection1_hh__
#define __connection1_hh__

#include <mysql.h>
#include <vector.h>
#include <deque.h>
#include <list.h>
#include <slist.h>
#include <set.h>
#include <map.h>
#include <multiset.h>
#include "define_short"
#include "query1.hh"
#include "result1.hh"

#include "connection0.hh" 
#include "tracker.h"

//: Exception thrown when a BadQuery is encountered
struct BadQuery {
  BadQuery(string er) : error(er) {}
  string error; //: The error message
};

class MutableRes;

//: The main database handle
class Connection {
  friend ResNSel;
  friend ResUse;
  friend Query;
  
private:
  set<MysqlChild*> children;
  static pointer_tracker<MYSQL, Connection> others;
  
  bool throw_exceptions;
  MYSQL mysql;
  bool is_connected;
  bool locked;
  bool Success;

  int          affected_rows() const {return mysql_affected_rows(&mysql);}
  int          insert_id () {return mysql_insert_id(&mysql);}
  string       info ();
  void         close() {mysql_close(&mysql);}

public:
  Connection () : throw_exceptions(false), locked(false) //:
    {others.insert(&mysql,this);}
  Connection (bool te) : throw_exceptions(te), locked(false) //:
    {others.insert(&mysql,this);}
  Connection (const char *db, const char *host = "", const char *user = "", 
	      const char *passwd = "", bool te = false); 
  Connection (const char *db, const char *host, const char *user, 
	      const char *passwd = "", uint port = 3306, my_bool compress = 1,
	      unsigned int connect_timeout = 5, bool te = false,
	      cchar *socket_name = "");

  bool   real_connect (cchar *db = "", cchar *host = "", 
		       cchar *user = "", cchar *passwd = "", uint port = 0,
		       my_bool compress = 0, unsigned int connect_timeout = 60,
		       cchar *socket_name= "");
				
  ~Connection (); //:

  bool   connected() const {return is_connected;}
  //: returns true if a successful connection was made

  bool   success() {return Success;}
  //: returns true of the last query was successful

  bool   connect (cchar *db = "", cchar *host = "", 
		  cchar *user = "", cchar *passwd = "");
  //:

  bool   lock() {if (locked) return true; locked = true; return false;}
  void   unlock() {locked = false;}

  void purge (MYSQL *m) {mysql_close(&mysql); }
  //:

  void add_child(MysqlChild *child) {children.insert(child);}
  //:

  void remove_child(MysqlChild *child) {children.erase(child); }
  //: 

  inline Query query();
  //:

  operator bool () {return success();}                  //: returns success()
  string error () {return string(mysql_error(&mysql));} //: last error message()
	int errnum () {return mysql_errno(&mysql);}

  string clinet_info () {return string(mysql_get_client_info());} //:
  
  string host_info () {return string(mysql_get_host_info(&mysql));} //:
  
  int    proto_info () {return mysql_get_proto_info(&mysql);} //:
  
  string server_info () {return string(mysql_get_server_info(&mysql));} //:
 
  string stat() {return string(mysql_stat(&mysql));} //:
 
  Result  store(const string &str) {return store(str, throw_exceptions);} //:
  ResUse  use(const string &str)   {return use(str, throw_exceptions);} //:
  ResNSel execute(const string &str) {return execute(str, throw_exceptions);} //:
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

  template <class Sequence> void storein_sequence(Sequence &, const string &); //:
  template <class Set>      void storein_set(Set &, const string &);  //:

  //!dummy: void storein(TYPE &con, const string &s);
  //: Stores the results in TYPE.  
  // Stores the result in TYPE. TYPE must be some sort of STL container.  

                            void storein(MutableRes &con, const string &s);
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





