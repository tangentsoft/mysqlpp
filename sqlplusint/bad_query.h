#ifndef __BAD_QUERY_EXCEPT__KK
#define __BAD_QUERY_EXCEPT__KK
#include <string>

namespace mysqlpp {

//: Exception thrown when a BadQuery is encountered
struct BadQuery {
  BadQuery(string er) : error(er) {}
  string error; //: The error message
};

}; // end namespace mysqlpp

#endif
