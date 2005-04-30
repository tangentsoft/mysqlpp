#ifndef MYSQLPP_STREAM2STRING_H
#define MYSQLPP_STREAM2STRING_H

/// \file stream2string.h
/// \brief Declares an adapter that converts something that can be
/// inserted into a C++ stream into a string type.

#include <sstream>

namespace mysqlpp {

/// \brief Converts a stream-able object to any type that can be
/// initialized from an \c std::string.
///
/// This adapter takes any object that has an \c out_stream() member
/// function and converts it to a string type.  An example of such a
/// type within the library is mysqlpp::Date.

template<class Strng, class T>
Strng stream2string(const T &object)
{
  std::ostringstream str;
  object.out_stream(str);
  str << std::ends;
  Strng s = str.str();
  return s;
}

} // end namespace mysqlpp

#endif

