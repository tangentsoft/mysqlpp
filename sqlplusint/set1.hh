#ifndef __set1_hh__
#define __set1_hh__

#include <coldata1.hh>
#include <define_short.hh>
#include <mysql++-windows.hh>

#include <iostream>
#include <set>
#include <vector>

template <class T, class value_type = typename T::value_type>
class MysqlListInsert {
private:
  T *object;
public:
  MysqlListInsert(T *o) {object = o;}
  void operator () (const value_type &data) {object->push_back(data);}
};

template <class T, class key_type = typename T::key_type> 
class MysqlSetInsert {
private:
  T *object;
public:
  MysqlSetInsert(T *o) {object = o;}
  void operator () (const key_type &data) {object->insert(data);}
};

template <class T>
inline MysqlSetInsert<std::set<T> > set_insert(std::set<T> *o) {
  return MysqlSetInsert<std::set<T> >(o);
}

template <class T>
inline MysqlListInsert<std::vector<T> > set_insert(std::vector<T> *o) {
  return MysqlListInsert<std::vector<T> >(o);
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

#endif









