#ifndef __manip1_hh__
#define __manip1_hh__

#include <datetime1.hh>
#include <define_short.hh>
#include <mysql++-windows.hh>
#include <sql_string1.hh>
#include <sql_query1.hh>
#include <set1.hh>

#include <mysql.h>

#include <iostream>

//quote manipulaor

extern bool dont_quote_auto;

class SQLQueryParms;

enum quote_type0 {mysql_quote};

struct quote_type1 {
  std::ostream *ostr;
  quote_type1(std::ostream *o) : ostr(o) {}
}; 

inline quote_type1 operator << (std::ostream &o, quote_type0 esc) {
  return quote_type1(&o);
}

struct quote_type2 {
  SQLQueryParms *qparms;
  quote_type2(SQLQueryParms *p) : qparms(p) {}
};

inline quote_type2 operator << (SQLQueryParms &p, quote_type0 esc) {
  return quote_type2(&p);
}

SQLQueryParms& operator << (quote_type2 p, SQLString &in);

template <class T>
inline std::ostream& operator << (quote_type1 o, const T &in) {
  return *o.ostr << in;
}

std::ostream& operator << (std::ostream& o,const mysql_ColData<std::string>& in);

std::ostream& operator << (std::ostream& o, const mysql_ColData<const_string>& in);

SQLQuery& operator << (SQLQuery& o, const mysql_ColData<std::string>& in);

SQLQuery& operator << (SQLQuery& o, const mysql_ColData<const_string>& in);

template <>
std::ostream& operator << (quote_type1 o, const std::string &in);

template <>
std::ostream& operator << (quote_type1 o, const char* const &in);

template <>
std::ostream& operator << (quote_type1 o, const mysql_ColData<std::string>& in);

template <>
std::ostream& operator << (quote_type1 o, const mysql_ColData<const_string>& in);

template <>
inline std::ostream& operator << (quote_type1 o, char* const &in) {
  return operator << (o, const_cast<const char* const &>(in));
}

template <>
inline std::ostream& operator << (quote_type1 o, const Date &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
inline std::ostream& operator << (quote_type1 o, const Time &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
inline std::ostream& operator << (quote_type1 o, const DateTime &in) {
  return *o.ostr << "'" << in << "'";
}

template <class ST>
inline std::ostream& operator << (quote_type1 o, const MysqlSet<ST> &in) {
  return *o.ostr << "'" << in << "'";
}

// quote only manipulator

enum quote_only_type0 {mysql_quote_only};

struct quote_only_type1 {
  std::ostream *ostr;
  quote_only_type1(std::ostream *o) : ostr(o) {}
}; 

inline quote_only_type1 operator << (std::ostream &o, quote_only_type0 esc) {
  return quote_only_type1(&o);
}

struct quote_only_type2 {
  SQLQueryParms *qparms;
  quote_only_type2(SQLQueryParms *p) : qparms(p) {}
};

inline quote_only_type2 operator << (SQLQueryParms &p, quote_only_type0 esc) {
  return quote_only_type2(&p);
}

SQLQueryParms & operator << (quote_only_type2 p, SQLString &in);

template <class T>
inline std::ostream& operator << (quote_only_type1 o, const T &in) {
  return *o.ostr << in;
}

template <>
inline std::ostream& operator << (quote_only_type1 o, const std::string &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
std::ostream& operator << (quote_only_type1 o, const mysql_ColData<std::string>& in);

template <>
std::ostream& operator << (quote_only_type1 o, const mysql_ColData<const_string>& in);

template <>
inline std::ostream& operator << (quote_only_type1 o, const Date &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
inline std::ostream& operator << (quote_only_type1 o, const Time &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
inline std::ostream& operator << (quote_only_type1 o, const DateTime &in) {
  return *o.ostr << "'" << in << "'";
}

template <class ST>
inline std::ostream& operator << (quote_only_type1 o, const MysqlSet<ST> &in) {
  return *o.ostr << "'" << in << "'";
}

// quote double (") only manipulator

enum quote_double_only_type0 {mysql_quote_double_only};
struct quote_double_only_type1 {
  std::ostream *ostr;
  quote_double_only_type1(std::ostream *o) : ostr(o) {}
}; 

inline quote_double_only_type1 operator << (std::ostream &o,
					    quote_double_only_type0 esc) {
  return quote_double_only_type1(&o);
}

struct quote_double_only_type2 {
  SQLQueryParms *qparms;
  quote_double_only_type2(SQLQueryParms *p) : qparms(p) {}
};

inline quote_double_only_type2 operator << (SQLQueryParms &p, 
					     quote_double_only_type0 esc) {
  return quote_double_only_type2(&p);
}

SQLQueryParms & operator << (quote_double_only_type2 p, SQLString &in);

template <class T>
inline std::ostream& operator << (quote_double_only_type1 o, const T &in) {
  return *o.ostr << in;
}

template <>
inline std::ostream& operator << (quote_double_only_type1 o, const std::string &in) {
  return *o.ostr << "\"" << in << "\"";
}

template <>
std::ostream& operator << (quote_double_only_type1 o, const mysql_ColData<std::string>& in);

template <>
std::ostream& operator << (quote_double_only_type1 o, const mysql_ColData<const_string>& in);

template <>
inline std::ostream& operator << (quote_double_only_type1 o, const Date &in) {
  return *o.ostr << "\"" << in << "\"";
}

template <>
inline std::ostream& operator << (quote_double_only_type1 o, const Time &in) {
  return *o.ostr << "\"" << in << "\"";
}

template <>
inline std::ostream& operator << (quote_double_only_type1 o,
			     const DateTime &in) {
  return *o.ostr << "\"" << in << "\"";
}

template <class ST>
inline std::ostream& operator << (quote_double_only_type1 o,
			     const MysqlSet<ST> &in) {
  return *o.ostr << "\"" << in << "\"";
}

// escape manipulator

enum escape_type0 {mysql_escape};
struct escape_type1 {
  std::ostream *ostr;
  escape_type1(std::ostream *o) : ostr(o) {}
}; 

inline escape_type1 operator << (std::ostream &o, escape_type0 esc) {
  return escape_type1(&o);
}

struct escape_type2 {
  SQLQueryParms *qparms;
  escape_type2(SQLQueryParms *p) : qparms(p) {}
};

inline escape_type2 operator << (SQLQueryParms &p, escape_type0 esc) {
  return escape_type2(&p);
}

SQLQueryParms & operator << (escape_type2 p, SQLString &in);

template <class T>
inline std::ostream& operator << (escape_type1 o, const T &in) {
  return *o.ostr << in;
}

template <>
std::ostream& operator << (escape_type1 o, const std::string &in);

template <>
std::ostream& operator << (escape_type1 o, const char* const &in);

template <>
std::ostream& operator << (escape_type1 o, const mysql_ColData<std::string>& in);

template <>
std::ostream& operator << (escape_type1 o, const mysql_ColData<const_string>& in);

template <>
inline std::ostream& operator << (escape_type1 o, char* const &in) {
  return operator << (o, const_cast<const char* const &>(in));
}


//do nothing manipulator

enum do_nothing_type0 {mysql_do_nothing};
struct do_nothing_type1 {
  std::ostream *ostr;
  do_nothing_type1(std::ostream *o) : ostr(o) {}
}; 

inline do_nothing_type1 operator << (std::ostream &o, do_nothing_type0 esc) {
  return do_nothing_type1(&o);
}

template <class T>
inline std::ostream& operator << (do_nothing_type1 o, const T &in) {
  return *o.ostr << in;
}

struct do_nothing_type2 {
  SQLQueryParms *qparms;
  do_nothing_type2(SQLQueryParms *p) : qparms(p) {}
};

inline do_nothing_type2 operator << (SQLQueryParms &p, do_nothing_type0 esc) {
  return do_nothing_type2(&p);
}

inline SQLQueryParms & operator << (do_nothing_type2 p, SQLString &in) {
  in.processed = true;
  return *p.qparms << in;
}

enum ignore_type0 {mysql_ignore};

struct ignore_type2 {
  SQLQueryParms *qparms;
  ignore_type2(SQLQueryParms *p) : qparms(p) {}
};

inline ignore_type2 operator << (SQLQueryParms &p, ignore_type0 esc) {
  return ignore_type2(&p);
}

inline SQLQueryParms & operator << (ignore_type2 p, SQLString &in) {
  return *p.qparms << in;
}

#endif

