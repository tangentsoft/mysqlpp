#ifndef __set1_hh__
#define __set1_hh__

#include <platform.h>

#include <coldata.h>
#include <stream2string.h>

#include <iostream>
#include <set>
#include <vector>

namespace mysqlpp {

template <class T, class value_type = typename T::value_type>
class ListInsert {
private:
  T *object;
public:
  ListInsert(T *o) {object = o;}
  void operator () (const value_type &data) {object->push_back(data);}
};

template <class T, class key_type = typename T::key_type> 
class SetInsert {
private:
  T *object;
public:
  SetInsert(T *o) {object = o;}
  void operator () (const key_type &data) {object->insert(data);}
};

template <class T>
inline SetInsert<std::set<T> > set_insert(std::set<T> *o) {
  return SetInsert<std::set<T> >(o);
}

template <class T>
inline ListInsert<std::vector<T> > set_insert(std::vector<T> *o) {
  return ListInsert<std::vector<T> >(o);
}

template <class Insert>
void set2container (const char *str, Insert insert);

//: A Special Set for holding mysql sets.
template <class Container = std::set<std::string> >
class Set : public Container {
public:
  Set(const char* str) {set2container(str,set_insert(this));}           //:
  Set(const std::string &str) {set2container(str.c_str(),set_insert(this));} //:
  Set(const ColData &str) 
    {set2container(str.c_str(),set_insert(this));}                      //:
  
  std::ostream& out_stream(std::ostream &s) const;
  
  operator std::string ();
};

//! with_class = Set

//:
template <class Container>
inline std::ostream& operator << (std::ostream &s, const Set<Container> &d)
{ 
  return d.out_stream(s); 
}


template <class Container> 
inline Set<Container>::operator std::string () {
  return stream2string<std::string>(*this);
}


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
  typename Container::const_iterator i = Container::begin();
  typename Container::const_iterator e = Container::end();
  while (true) {
    s << *i;
    i++;
    if (i==e) break;
    s << ",";
  }
  return s;
}

}; // end namespace mysqlpp

#endif

