#ifndef __set2_hh__
#define __set2_hh__

#include "set1.hh"
#include "stream2string3.hh"

template <class Container> 
inline Set<Container>::operator string () {
  return stream2string<string>(*this);
}

#endif
