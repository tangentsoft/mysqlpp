#ifndef __row2_hh__
#define __row2_hh__

#include "string.h"
#include "row1.hh"
#include "result2.hh"

inline Row::size_type Row::size() const {return res->num_fields();}

inline const ColData Row::operator [] (size_type i) const {
  return ColData(data[i].c_str(), res->types(i));
}

inline const ColData Row::operator[] (const string &i) const {
  return (*this)[res->field_num(i)];
}

inline const ColData Row::operator[] (const char *i) const {
  return (*this)[res->field_num(string(i))];
}

// MutableRow

inline MutableRow<MutableRes>::MutableRow(const MutableRes *_res) : MutableRow_base(_res)
{
  insert(begin(), _res->columns(), "");
}

template <class Res>
inline MutableColData& MutableRow_base<Res>::operator[] (const string &i) {
  return (*this)[res->field_num(i)];
}

template <class Res>
inline MutableColData& MutableRow_base<Res>::operator[] (const char *i) {
  return (*this)[res->field_num(string(i))];
}

template <class Res>
inline const MutableColData& MutableRow_base<Res>::operator[] (const string &i) const {
  return (*this)[res->field_num(i)];
}

template <class Res>
inline const MutableColData& MutableRow_base<Res>::operator[] (const char *i) const {
  return (*this)[res->field_num(string(i))];
}

// RowTemplate

#endif










