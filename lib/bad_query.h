#ifndef MYSQLPP_BAD_QUERY_H
#define MYSQLPP_BAD_QUERY_H
#include <string>

namespace mysqlpp {

//: Exception thrown when a BadQuery is encountered
struct BadQuery {
  BadQuery(string er) : error(er) {}
  string error; //: The error message
};

} // end namespace mysqlpp

#endif

