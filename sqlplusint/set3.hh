#ifndef __set3_hh__
#define __set3_hh__

#include "set2.hh"
#include "coldata2.hh"

template <class Insert>
void set2container (const char *str, Insert insert) {
  while (1) {
    MutableColData s("");
    while (*str != ',' && *str) {
      s += *str;
      str++;
    }
    insert(s);
    if (!*str) break;
    str++;
  }
}

template <class Container>
std::ostream& Set<Container>::out_stream (std::ostream &s) const {
  typename Container::const_iterator i = begin();
  typename Container::const_iterator e = end();
  while (true) {
    s << *i;
    i++;
    if (i==e) break;
    s << ",";
  }
  return s;
}

#endif
