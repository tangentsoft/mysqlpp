#ifndef __null2_hh__
#define __null2_hh__

#include "null1.hh"

template <class Type, class Behavior>
inline std::ostream& operator << (std::ostream &o, const Null<Type,Behavior> &n) {
  if (n.is_null) return Behavior::null_ostr(o);
  else return o << n.data;
}

#endif
