#ifndef __stream2string_hh__
#define __stream2string_hh__

#include <sstream>

namespace mysqlpp {

template<class Strng, class T>
Strng stream2string(const T &object);

template<class Strng, class T>
Strng stream2string(const T &object)
{
  std::ostringstream str;
  object.out_stream(str);
  str << std::ends;
  Strng s = str.str();
  return s;
}

}; // end namespace mysqlpp

#endif

