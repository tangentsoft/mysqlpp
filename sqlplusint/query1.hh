#ifndef __query1_hh__
#define __query1_hh__
#ifdef __WIN32__
#include <Windows32/Base.h>
#include <Windows32/Defines.h>
#include <Windows32/Structures.h>
#include <winsock.h>
#define errno WSAGetLastError()
#endif
#include <mysql.h>
#include "define_short"
#include "sql_query1.hh"
#include "result1.hh"
#include "connection1.hh"


//: A Query class suitable for executing queries.  
// This is the class is subclassed from SQLQuery. This class unlike
// SQLQuery is loosely attached to a Mysql object so that it can
// execute queries.
class Query : public SQLQuery {
private:
  Connection   *mysql;
  bool         throw_exceptions;

  int     affected_rows() const;
  int     insert_id ();
  string  info ();
  bool    lock();
  void    unlock();

public:
  Query(Connection *m, bool te = false) 
    {mysql = m;errmsg=NULL;Success = true;throw_exceptions=te;} 
  //: Create a new query object attached to a connection.
  Query(const Query &q); //:
  Query& operator = (const Query &q); //:

  string   error ();  //: The error message if the query was not successful.
  bool     success(); //: Displays the string currently in the buffer.
  // Same thing as string().

  string   preview () {return str(def);}       //:
  string   preview (parms &p) {return str(p);} //:
	bool     exec (const string &str);
  //!dummy: MysqlResNSel execute (...);
  //: Executes the query in the string buffer.
  // Executes the query in the string buffer and returns a structure
  // that contains the information of the success of the query. Use
  // this for queries that don't return a result set such as INSERT,
  // UPDATE, etc.
  //
  // The parameters can be anything in a valid SQLQuery::str.

  //!dummy: MysqlRes store (...);
  //: Executes the query in the string buffer.
  // Executes a query in the string buffer and returns the result.
  //
  // The parameters can be anything in a valid SQLQuery::str.

  //!dummy: MysqlResUse use (...); 
  //: Executes the query in the string buffer.
  // Executes the query in the string buffer and returns the
  // results. This method used the mysql_use_result(MYSQL_RES)
  // function and thus should be used sparingly. Also the Result set is
  // a lot more limited and DOES NOT have an iterator.
  //
  // The parameters can be anything in a valid SQLQuery::str.

  mysql_query_define0(string,preview)

  mysql_query_define1(ResNSel, execute)
  mysql_query_define1(ResUse, use)
  mysql_query_define1(Result, store) 

  mysql_query_define2(storein_sequence)
  mysql_query_define2(storein_set)

  mysql_query_define2(storein)

  template <class T> Query& update(const T &o, const T &n) {
    SQLQuery::update(o,n);
    return *this;
  }
  
  template <class T> Query& insert(const T &v) {
    SQLQuery::insert(v);
    return *this;
  }

  template <class T> Query& replace(const T &v) {
    SQLQuery::replace(v);
    return *this;
  }
};

#endif





