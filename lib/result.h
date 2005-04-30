#ifndef MYSQLPP_RESULT_H
#define MYSQLPP_RESULT_H

/// \file result.h
/// \brief Declares classes for holding SQL query result sets.

#include "defs.h"

#include "exceptions.h"
#include "fields.h"
#include "field_names.h"
#include "field_types.h"
#include "resiter.h"
#include "row.h"

#include <mysql.h>

#include <map>
#include <set>
#include <string>

namespace mysqlpp {

class Connection;

/// \brief A basic result set class, for use with "use" queries.
///
/// A "use" query is one where you make the query and then process just
/// one row at a time in the result instead of dealing with them all as
/// a single large chunk.  (The name comes from the MySQL C API function
/// that initiates this action, \c mysql_use_result().)  By calling
/// fetch_row() until it throws a mysqlpp::BadQuery exception (or an
/// empty row if exceptions are disabled), you can process the result
/// set one row at a time.

class ResUse  {
protected:
  Connection            *mysql;
  mutable MYSQL_RES     *mysql_res;
  bool                  throw_exceptions, initialized;
  mutable FieldNames    *_names;
  mutable FieldTypes    *_types;
  Fields                _fields;
  std::string                _table;

  /// \brief copy another ResUse object's contents into this one.
  ///
  /// Not to be used on the self. Self-copy is not allowed.
  void copy(const ResUse& other);

public:
  ResUse () : mysql(0), mysql_res(0), throw_exceptions(false),initialized(false), _names(NULL), _types(NULL), _fields(this) {}
  ResUse (MYSQL_RES *result, Connection *m = NULL, bool te = false);
  ResUse (const ResUse &other) : initialized(false) {copy(other);}
  ~ResUse (); 

  inline ResUse& operator = (const ResUse &other);

  MYSQL_RES *mysql_result (void) {return mysql_res;}

  /// \brief Wraps mysql_fetch_row() in MySQL C API.
  ///
  /// This is not a thin wrapper. It does a lot of error checking before
  /// returning the mysqlpp::Row object containing the row data.
  Row fetch_row()
  {
	  if (!mysql_res) {  if (throw_exceptions) throw BadQuery("Results not fetched");
    else return Row();}
    MYSQL_ROW row = mysql_fetch_row(mysql_res);
    unsigned long* length = mysql_fetch_lengths(mysql_res);
	  if (!row || !length) {  if (throw_exceptions) throw BadQuery("Bad row");
    else return Row();}
    return Row(row, this, length, throw_exceptions);
  }

  /// \brief Wraps mysql_eof() in MySQL C API.
  bool eof () const {return mysql_eof(mysql_res) != 0;}

  /// \brief Wraps mysql_fetch_lengths() in MySQL C API.
  unsigned long* fetch_lengths () const {return mysql_fetch_lengths(mysql_res);}

  /// \brief Wraps mysql_fetch_field() in MySQL C API.
  Field& fetch_field () const   {return *mysql_fetch_field(mysql_res);}
 
  /// \brief Wraps mysql_field_seek() in MySQL C API.
  void field_seek (int field) {mysql_field_seek (mysql_res, field);}

  /// \brief Wraps mysql_num_fields() in MySQL C API.
  int num_fields() const {return mysql_num_fields(mysql_res);}

  void parent_leaving() {mysql = NULL;}
  
  void purge(void)
    { if (mysql_res) mysql_free_result(mysql_res); mysql_res=0; if (_names) delete _names; if (_types) delete _types; _names=0; _types=0; _table.erase(); }
  
  operator bool() const {if (mysql_res) return true; return false;}
  unsigned int columns() const {return num_fields();}

  /// \brief Get the name of table that the result set comes from.
  std::string& table() {return _table;}
  const std::string& table() const {return _table;}

  /// \brief Get the index of the named field.
  ///
  /// This is the inverse of field_name().
  inline int field_num(const std::string&) const;

  /// \brief Get the name of the field at the given index.
  ///
  /// This is the inverse of field_num().
  inline std::string& field_name(int);
  inline const std::string& field_name(int) const;

  /// \brief Get the names of the fields within this result set.
  inline FieldNames& field_names();
  inline const FieldNames& field_names() const;

  /// \brief Reset the names in the field list to their original values.
  inline void reset_field_names();

  /// \brief Get the MySQL type for a field given its index.
  inline mysql_type_info& field_type(int i);
  inline const mysql_type_info& field_type(int) const;

  /// \brief Get a list of the types of the fields within this
  /// result set.
  inline FieldTypes& field_types();
  inline const FieldTypes& field_types() const;

  /// \brief Reset the field types to their original values.
  inline void reset_field_types();

  /// \brief Alias for field_num()
  inline int names(const std::string& s) const;
 
  /// \brief Alias for field_name()
  inline std::string& names(int i);
  inline const std::string& names(int i) const;
 
  /// \brief Alias for field_names()
  inline FieldNames& names();
  inline const FieldNames& names() const;

  /// \brief Alias for reset_field_names()
  inline void reset_names();

  /// \brief Alias for field_type()
  inline mysql_type_info& types(int i);
  inline const mysql_type_info& types(int i) const;

  /// \brief Alias for field_types()
  inline FieldTypes& types();
  inline const FieldTypes& types() const;

  /// \brief Alias for reset_field_types()
  inline void reset_types();

  /// \brief Get the underlying Fields structure.
  const Fields& fields() const {return _fields;}

  /// \brief Get the underlying Field structure given its index.
  const Field& fields(unsigned int i) const {return _fields[i];}

  bool     operator == (const ResUse &other) const
    {return mysql_res == other.mysql_res;}
 
  bool     operator != (const ResUse &other) const
    {return mysql_res != other.mysql_res;}
};


/// \brief This class manages SQL result sets. 
///
/// Objects of this class are created to manage the result of "store"
/// queries, where the result set is handed to the program as single
/// block of row data. (The name comes from the MySQL C API function
/// \c mysql_store_result() which creates these blocks of row data.)
///
/// This class is a random access container (in the STL sense) which
/// is neither less-than comparable nor assignable.  This container
/// provides a reverse random-access iterator in addition to the normal
/// forward one.

class Result : public ResUse, 
	       public const_subscript_container<Result,Row,const Row>
{
public:
  Result () {}
  Result (MYSQL_RES *result, bool te = false) 
    : ResUse(result, NULL, te) {mysql = NULL;}
  Result (const Result &other)
	: ResUse(other),
	const_subscript_container<Result,Row,const Row>() // no copying here
	{mysql = NULL;}
  virtual ~Result() {}  

  /// \brief Wraps mysql_fetch_row() in MySQL C API.
  ///
  /// This is simply the const version of the same function in our
  /// \link mysqlpp::ResUse parent class \endlink . Why this cannot
  /// actually \e be in our parent class is beyond me.
  const Row fetch_row() const
  {
	  if (!mysql_res) {  if (throw_exceptions) throw BadQuery("Results not fetched");
    else return Row();}
    MYSQL_ROW row = mysql_fetch_row(mysql_res);
    unsigned long* length = mysql_fetch_lengths(mysql_res);
	  if (!row || !length) {  if (throw_exceptions) throw BadQuery("Bad row");
    else return Row();}
    return Row(row, this, length, throw_exceptions);
  }

  /// \brief Wraps mysql_num_rows() in MySQL C API.
  my_ulonglong  num_rows() const {
  	if (initialized) return mysql_num_rows(mysql_res);
	else return 0;
  }

  /// \brief Wraps mysql_data_seek() in MySQL C API.
  void data_seek (uint offset) const 
  {
  	mysql_data_seek(mysql_res, offset);
  }

  /// \brief Alias for num_rows(), only with different return type.
  size_type size() const { return size_type(num_rows()); }

  /// \brief Alias for num_rows(), only with different return type.
  size_type rows() const { return size_type(num_rows()); }

  /// \brief Get the row with an offset of i.
  const Row operator [] (size_type i) const {data_seek(i); return fetch_row();}
};


inline void swap (ResUse &x, ResUse &y)
{
  ResUse tmp = x;
  x = y;
  y = tmp;
}

inline void swap (Result &x, Result &y)
{
  Result tmp = x;
  x = y;
  y = tmp;
}

/// \brief Holds the information on the success of queries that
/// don't return any results.
class ResNSel {
public:
  bool success;   
  my_ulonglong insert_id;
  my_ulonglong rows;		///< Number of rows affected
  std::string info;			///< Additional info

  ResNSel() : success(false) {};
  ResNSel (Connection *q);

  operator bool() {return success;}  ///< Returns true if the query was successful
};  


inline int ResUse::field_num(const std::string &i) const
{
  if (!_names) _names = new FieldNames(this);
  return (*_names)[i];
}

inline std::string& ResUse::field_name(int i)
{
  if (!_names) _names = new FieldNames(this);
  return (*_names)[i];
}

inline const std::string& ResUse::field_name(int i) const
{
  if (!_names) _names = new FieldNames(this);
  return (*_names)[i];
}

inline FieldNames& ResUse::field_names()
{
  if (!_names) _names = new FieldNames(this);
  return *_names;
}

inline const FieldNames& ResUse::field_names() const
{
  if (!_names) _names = new FieldNames(this);
  return *_names;
}

inline void ResUse::reset_field_names()
{
  delete _names;
  _names = new FieldNames(this);
}


inline mysql_type_info& ResUse::field_type(int i)
{
  if (!_types) _types = new FieldTypes(this);
  return (*_types)[i];
}

inline const mysql_type_info& ResUse::field_type(int i) const
{
  if (!_types) _types = new FieldTypes(this);
  return (*_types)[i];
}

inline FieldTypes& ResUse::field_types()
{
  if (!_types) _types = new FieldTypes(this);
  return *_types;
}

inline const FieldTypes& ResUse::field_types() const
{
  if (!_types) _types = new FieldTypes(this);
  return *_types;
}

inline void ResUse::reset_field_types()
{
  delete _types;
  _types = new FieldTypes(this);
}

inline int ResUse::names(const std::string& s) const {return field_num(s);}
inline std::string& ResUse::names(int i) {return field_name(i);}
inline const std::string& ResUse::names(int i) const {return field_name(i);}
inline FieldNames& ResUse::names() {return field_names();}
inline const FieldNames& ResUse::names() const {return field_names();}
inline void ResUse::reset_names() {reset_field_names();}

inline mysql_type_info& ResUse::types(int i) {return field_type(i);}
inline const mysql_type_info& ResUse::types(int i) const {return field_type(i);}
inline FieldTypes& ResUse::types() {return field_types();}
inline const FieldTypes& ResUse::types() const {return field_types();}
inline void ResUse::reset_types() {reset_field_types();}


inline ResUse& ResUse::operator = (const ResUse &other)
{
  if (this == &other) return *this;
  copy(other);  other.mysql_res=NULL;
  return *this;
}

} // end namespace mysqlpp

#endif

