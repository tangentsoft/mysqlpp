#ifndef __stream2string3_hh__
#define __stream2string3_hh__

#include <strstream.h>

template<class Strng, class T>
Strng stream2string(const T &object) {
  ostrstream str;
  object.out_stream(str);
  str << ends;
  Strng s = str.str();
  str.freeze(0);
  return s;
}

#endif
