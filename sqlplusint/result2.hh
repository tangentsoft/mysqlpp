#ifndef __result2_hh__
#define __result2_hh__

#include "connection2.hh"
#include "result1.hh"
#include "row2.hh"
#include "fields2.hh"

// field name info

inline int ResUse::field_num(const string &i) const {
  if (!_names) _names = new FieldNames(this);
  return (*_names)[i];
}

inline string& ResUse::field_name(int i) {
  if (!_names) _names = new FieldNames(this);
  return (*_names)[i];
}

inline const string& ResUse::field_name(int i) const {
  if (!_names) _names = new FieldNames(this);
  return (*_names)[i];
}

inline FieldNames& ResUse::field_names() {
  if (!_names) _names = new FieldNames(this);
  return *_names;
}

inline const FieldNames& ResUse::field_names() const {
  if (!_names) _names = new FieldNames(this);
  return *_names;
}

inline void ResUse::reset_field_names() {
  delete _names;
  _names = new FieldNames(this);
}

// field type info

inline mysql_type_info& ResUse::field_type(int i) {
  if (!_types) _types = new FieldTypes(this);
  return (*_types)[i];
}

inline const mysql_type_info& ResUse::field_type(int i) const {
  if (!_types) _types = new FieldTypes(this);
  return (*_types)[i];
}

inline FieldTypes& ResUse::field_types() {
  if (!_types) _types = new FieldTypes(this);
  return *_types;
}

inline const FieldTypes& ResUse::field_types() const {
  if (!_types) _types = new FieldTypes(this);
  return *_types;
}

inline void ResUse::reset_field_types() {
  delete _types;
  _types = new FieldTypes(this);
}

inline int               ResUse::names(const string& s) const {return field_num(s);}
inline string&           ResUse::names(int i) {return field_name(i);}
inline const string&     ResUse::names(int i) const {return field_name(i);}
inline FieldNames&       ResUse::names() {return field_names();}
inline const FieldNames& ResUse::names() const {return field_names();}
inline void              ResUse::reset_names() {reset_field_names();}

inline mysql_type_info&  ResUse::types(int i) {return field_type(i);}
inline const mysql_type_info& ResUse::types(int i) const {return field_type(i);}
inline FieldTypes&       ResUse::types() {return field_types();}
inline const FieldTypes& ResUse::types() const {return field_types();}
inline void              ResUse::reset_types() {reset_field_types();}

//

//

inline ResUse& ResUse::operator = (const ResUse &other) {
  if (this == &other) return *this;
  copy(other);  other.mysql_res=NULL;
  return *this;
}

inline ResNSel::ResNSel (Connection *q) 
  : success   (q->success()),         insert_id (q->insert_id()),
    rows      (q->affected_rows()),   info      (q->info())        {}

#endif



