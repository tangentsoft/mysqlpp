#ifndef __stream2string3_hh__
#define __stream2string3_hh__

#include <strstream>

template<class Strng, class T>
Strng stream2string(const T &object) {
  std::ostrstream str;
  object.out_stream(str);
  str << ends;
  Strng s = str.str();
#ifdef __USLC__
  strstreambuf *tmpbuf = str.rdbuf();
  tmpbuf->freeze(0);
#else
  str.freeze(0);
#endif
  return s;
}

#endif
