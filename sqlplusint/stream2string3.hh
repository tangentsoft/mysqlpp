#ifndef __stream2string3_hh__
#define __stream2string3_hh__

#include <strstream>

template<class Strng, class T>
Strng stream2string(const T &object) {
  std::ostrstream str;
  object.out_stream(str);
  str << std::ends;
  Strng s = str.str();
  str.freeze(0);
  return s;
}

#endif
