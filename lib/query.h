#ifndef __query1_hh__
#define __query1_hh__

#include <platform.h>

#include <coldata.h>
#include <connection.h>
#include <result.h>
#include <sql_query.h>

#include <mysql.h>

#define mysql_query_define1(RETURN, FUNC) \
  RETURN FUNC () {return FUNC (def);} \
  RETURN FUNC (const char* str); \
  RETURN FUNC (parms &p);\
  mysql_query_define0(RETURN,FUNC) \

#define mysql_query_define2(FUNC) \
  template <class T1> void FUNC (T1 &con, query_reset r = RESET_QUERY) {FUNC (con, def,r);} \
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

//: A Query class suitable for executing queries.  
// This is the class is subclassed from SQLQuery. This class unlike
// SQLQuery is loosely attached to a Mysql object so that it can
// execute queries.
class Query : public SQLQuery {
private:
  Connection   *mysql;
  bool         throw_exceptions;

  my_ulonglong affected_rows() const;
  my_ulonglong insert_id ();
  std::string  info ();
  bool    lock();
  void    unlock();

public:
  Query(Connection *m, bool te = false) :
  SQLQuery(),
  mysql(m),
  throw_exceptions(te)
  {
	  Success = true;
  }

  //: Create a new query object attached to a connection.
  Query(const Query &q);

  std::string   error ();  //: The error message if the query was not successful.
  bool     success(); //: Displays the string currently in the buffer.
  // Same thing as string().

  std::string   preview () {return str(def);}       //:
  std::string   preview (parms &p) {return str(p);} //:
	bool     exec (const std::string &str);

  mysql_query_define0(std::string,preview)

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

template <class Seq>
void Query::storein_sequence (Seq &seq,parms &p, query_reset r) {
  r = (parsed.size()) ? DONT_RESET : RESET_QUERY;
  mysql->storein_sequence (seq, str(p,r));
}

template <class Set>
void Query::storein_set (Set &sett, parms &p, query_reset r) {
  r = (parsed.size()) ? DONT_RESET : RESET_QUERY;
  mysql->storein_set (sett, str(p,r));
}

template <class Sequence>
void Query::storein_sequence (Sequence &seq, const char *s) {
  mysql->storein_sequence (seq, s);
}

template <class Set>
void Query::storein_set (Set &sett, const char * s) {
  mysql->storein_set (sett, s);
}

template <class T>
void Query::storein (T &con, parms &p, query_reset r) {
  r = (parsed.size()) ? DONT_RESET : RESET_QUERY;
  mysql->storein (con, str(p,r));
}

template <class T>
void Query::storein (T &con, const char *s) {
  mysql->storein (con, s);
}

}; // end namespace mysqlpp

#endif

