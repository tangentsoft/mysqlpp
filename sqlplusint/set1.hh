#ifndef __set1_hh__
#define __set1_hh__
#ifdef __WIN32__
#include <Windows32/Base.h>
#include <Windows32/Defines.h>
#include <Windows32/Structures.h>
#include <winsock.h>
#define errno WSAGetLastError()
#endif
#include <set>
#include <vector>
#include <iostream.h>
#include "define_short"
#include "coldata1.hh"

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
inline MysqlSetInsert<set<T> > set_insert(set<T> *o) {
  return MysqlSetInsert<set<T> >(o);
}

template <class T>
inline MysqlListInsert<vector<T> > set_insert(vector<T> *o) {
  return MysqlListInsert<vector<T> >(o);
}

template <class Insert>
void set2container (const char *str, Insert insert);

//: A Special Set for holding mysql sets.
template <class Container = set<string> >
class Set : public Container {
public:
  Set(const char* str) {set2container(str,set_insert(this));}           //:
  Set(const string &str) {set2container(str.c_str(),set_insert(this));} //:
  Set(const ColData &str) 
    {set2container(str.c_str(),set_insert(this));}                      //:
  
  ostream& out_stream(ostream &s) const;
  
  operator string ();
};

//! with_class = Set

//:
template <class Container>
inline ostream& operator << (ostream &s, const Set<Container> &d) 
{ 
  return d.out_stream(s); 
}

#endif









