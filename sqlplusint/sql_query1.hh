#ifndef __sql_query_1_hh
#define __sql_query_1_hh

#include <strstream>
#include <vector>
#include <map>
#include "define_short"
#include "sql_query0.hh"
#include "sql_string1.hh"

class Query;
class SQLQuery;


//! with_class = SQLQuery

//: This class holds the parameter values for filling template queries. 
// It is a subclass of a vector of *SQLStrings*.
class SQLQueryParms : public std::vector<SQLString> {
  friend class Query;
private:
  typedef const SQLString&      ss;
  SQLQuery                 *parent;
public:
  SQLQueryParms () : parent(NULL) {} 
  SQLQueryParms (SQLQuery *p) : parent(p) {}
  bool  bound() {return parent;}
  void               clear() {erase(begin(),end());} //: Clears the list
  SQLString &operator [] (size_type n) {
    if (n >= size()) insert(end(),(n+1) - size(), "");
    return std::vector<SQLString>::operator [] (n);
  } //: Access element number n
  const SQLString &operator [] (size_type n) const 
    {return std::vector<SQLString>::operator [] (n);}
  //: Access element number n
  SQLString &operator [] (const char *str); 
  //: Access the value of the element with a key of str.

  const SQLString &operator [] (const char *str) const; 
  //: Access the value of the element with a key of str.

  SQLQueryParms &operator << (const SQLString &str)
    {push_back(str);return *this;}  //: Adds an element to the list

  SQLQueryParms &operator += (const SQLString &str)
    {push_back(str);return *this;}  //: Adds an element to the list

  SQLQueryParms operator + (const SQLQueryParms &other) const;

  //!dummy: void set (ss a, [ss b] ,... , [ss l]);
  //: Set the elements.
  // Sets element 0 to a, element 1 to b, etc. May specify up to a dozen elements

  void set (ss a)
    {clear(); *this << a;}
  void set (ss a, ss b)
    {clear(); *this << a << b;}
  void set (ss a, ss b, ss c)
    {clear(); *this << a << b << c;}
  void set (ss a, ss b, ss c, ss d)
    {clear(); *this << a << b << c << d;}
  void set (ss a, ss b, ss c, ss d, ss e)
    {clear(); *this << a << b << c << d << e;}
  void set (ss a, ss b, ss c, ss d, ss e, ss f)
    {clear(); *this << a << b << c << d << e << f;}
  void set (ss a, ss b, ss c, ss d, ss e, ss f, ss g)
    {clear(); *this << a << b << c << d << e << f << g;}
  void set (ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h)
    {clear(); *this << a << b << c << d << e << f << g << h;}
  void set (ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i)
    {clear(); *this << a << b << c << d << e << f << g << h << i;}
  void set (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j)
    {clear(); *this <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j;}
  void set (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k)
    {clear(); *this <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k;}
  void set (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k,ss l)
    {clear(); *this <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k <<l;}
};

//:
enum query_reset {DONT_RESET, RESET_QUERY};

struct SQLParseElement {
  SQLParseElement(std::string b, char o, char n) : before(b),option(o),num(n) {}
  std::string before;
  char   option;
  char   num;
};

//! with_class = Query

//: The "pure" query class 

// This is the "pure" query class. It is used to form queries to send
// to the *Connection* object. The *Query* class can be used if you
// wish to also be able to execute the queries without having to send
// them to the *Connection* object.
//
// This class is subclassed from *strstream*. This means that you can
// write to it like a stream to avoid having to piece parts together
// by creating you own *strstream* or by using *sprintf*. Although you
// can read from query because it is a stream this is _not_
// recommended. I can not guarantee the predictability of the class if
// you do and will offer no help if you run into problems when you do
// this. However, fell free to use any of the stream methods to write
// to it. Just make sure that the write buffer points to the end of
// your query before you try to use any of the *SQLQuery* specific
// methods except for *error()* and *success()*.

class SQLQuery : public std::strstream { 
  friend class SQLQueryParms;
private:
  char* preview_char();
protected:
  bool    Success;
  char*   errmsg;
  std::vector<SQLParseElement> parsed;
  std::vector<std::string>          parsed_names;
  std::map<std::string,int>         parsed_nums;
  typedef  const SQLString&  ss;
  typedef  SQLQueryParms  parms;
  void     proc(parms &p);

public:
  SQLQuery(): Success(false), errmsg(NULL), def(this) {parsed.empty(); parsed_names.empty(); parsed_nums.empty();} //:
  SQLQuery(const SQLQuery &q); //:
  SQLQuery& operator = (const SQLQuery &q); //:

  SQLQueryParms def; //: The default template parameters set. 

  void     parse();  

  std::string   error () const {return errmsg;}
  bool     success() const {return Success;}

  operator bool () {return success();}
  bool operator !    () {return !success();}

  //!dummy: string str (query_reset r = DONT_RESET);
  //: Returns the full query string.
  // Returns the full query string, replacing it with the default
  // template parameters if necessary. *query_reset* can either be
  // DONT_RESET or RESET_QUERY. If it is set the RESET_QUERY then
  // reset() is called after it has finished returning the query. If
  // there is not enough  parameters then it will return a
  // empty string and *success()* would be false.

  //!dummy: string str (const SQLString &parm0, ...,const SQLString &parm11);
  //: Like str(query_reset) but sets the parameters.
  // Like str(query_reset) but sets query parameters 0 to (up to)
  // 11. To parm0, parm1 etc. This is archived by overloaded methods
  // and templates and not the ...
  //
  // It will combine this with def. If any of the required parameters
  // are null it will produce an error and return an empty string.

  //!dummy: string str(const SQLQueryParms & p, query_reset r = DONT_RESET);
  //: Like *str(query_reset)* but sets the parameter.
  // Like *query(query_reset res)* but sets the parameters based on
  // combining p and def.
  //
  // If any of the required parameters are null it will produce an
  // error and return an empty string.

  void     reset (); //: Resets the query. 
  // This means erasing the string and the default template parameters. 

  template <class T> SQLQuery& update(const T &o, const T &n) {
    reset();
    *this << "UPDATE " << o.table() << " SET " << n.equal_list() 
          << " WHERE " << o.equal_list(" AND ", sql_use_compare);
    return *this;
  } //:
  template <class T> SQLQuery& insert(const T &v) {
    reset();
    *this << "INSERT INTO " << v.table() << " (" << v.field_list()
          << ") VALUES (" << v.value_list() << ")";
    return *this;
  } //:
  template <class T> SQLQuery& replace(const T &v) {
    reset();
    *this << "REPLACE INTO " << v.table() << " (" << v.field_list()
          << ") VALUES (" << v.value_list() << ")";
    return *this;
  } //:

  mysql_query_define_const1(std::string,str)
};  

#endif
