#ifndef __stream2string3_hh__
#define __stream2string3_hh__

#include <sstream>

template<class Strng, class T>
Strng stream2string(const T &object) {
  std::ostringstream str;
  object.out_stream(str);
  str << std::ends;
  Strng s = str.str();
  return s;
}

#endif
