#ifndef MYSQLPP_EXCEPTIONS_H
#define MYSQLPP_EXCEPTIONS_H

#include <exception>
#include <string>

namespace mysqlpp {

//: Exception thrown when a BadQuery is encountered
class BadQuery : public std::exception {
public:
  BadQuery(const std::string &er = "") : error(er) {}
  ~BadQuery() throw () {}
  const std::string error; //: The error message
  virtual const char* what( void ) const throw () { return error.c_str(); }
};

//: Exception structure thrown when a bad conversion takes place
class BadConversion : public std::exception {
  const std::string _what;
public:
  const char*  type_name;  //:
  const std::string data;       //:
  size_t       retrieved;  //:
  size_t       actual_size;//:
  BadConversion(const char* tn, const char* d, size_t r, size_t a)
    : _what(std::string("Tried to convert \"") + std::string(d ? d : "") + "\" to a \"" + std::string(tn ? tn : "")),
      type_name(tn), data(d), retrieved(r), actual_size(a) {};

  BadConversion(const std::string &wt, const char* tn, const char* d, size_t r, size_t a)
    : _what(wt), type_name(tn), data(d), retrieved(r), actual_size(a) {};

  BadConversion(const std::string& wt = "")
    : _what(wt), type_name("unknown"), data(""), retrieved(0), actual_size(0) {};
  ~BadConversion() throw () {}

  virtual const char* what( void ) const throw () { return _what.c_str(); }
};

//: Thrown when a *Null* value is trying to be converted into a type
//: it can't convert to.
class BadNullConversion : public std::exception {
  const std::string _what;
public:
  BadNullConversion(const std::string &wt = "") : _what(wt) {}
  ~BadNullConversion() throw () {}
  virtual const char* what( void ) const throw () { return _what.c_str(); }
};

//: Exception thrown when not enough parameters are provided
// Thrown when not enough parameters are provided for a
// template query.
class SQLQueryNEParms : public std::exception {
  const std::string _what;
public:
  SQLQueryNEParms(const char *c) : _what(std::string(c ? c : "")), error(c) {}
  ~SQLQueryNEParms() throw () {}
  const char* error; //:
  virtual const char* what( void ) const throw () { return _what.c_str(); }
};

//: Exception thrown when lookup_by_name can't find the specified field.
class BadFieldName : public std::exception {
  std::string _what;
public:
  BadFieldName(const char *bad_field)
	{
      _what = "Unknown field name: ";
      _what += bad_field;
    }
  ~BadFieldName() throw() {}
  virtual const char* what( void ) const throw () { return _what.c_str(); }
};

} // end namespace mysqlpp

#endif

