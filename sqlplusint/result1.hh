#ifndef __result1_hh__
#define __result1_hh__

#ifdef __WIN32__
#include <winsock.h>
#endif

#include <mysql.h>

#include <map>
#include <set>
#include <string>

#include "define_short.hh"
#include "field_names1.hh"
#include "row1.hh"
#include "resiter1.hh"
#include "field_types1.hh"
#include "fields1.hh"
#include "exceptions.hh"

//:
class ResUse  {
  friend class Connection;
protected:
  Connection            *mysql;
  mutable MYSQL_RES     *mysql_res;
  bool                  throw_exceptions, initialized;
  mutable FieldNames    *_names;
  mutable FieldTypes    *_types;
  Fields                _fields;
  std::string                _table;
  void copy(const ResUse& other);  //: not to be used on the self. self - copy is not allowed
public:
  ResUse () : mysql(0), mysql_res(0), throw_exceptions(false),initialized(false), _names(NULL), _types(NULL), _fields(this) {}
  ResUse (MYSQL_RES *result, Connection *m = NULL, bool te = false);
  ResUse (const ResUse &other) : initialized(false) {copy(other);}
  inline ResUse& operator = (const ResUse &other);
  MYSQL_RES *mysql_result (void) {return mysql_res;}
  /* raw mysql c api functions */
  Row fetch_row()
  {
	  if (!mysql_res) {  if (throw_exceptions) throw BadQuery("Results not fetched");
    else return Row();}
    MYSQL_ROW row = mysql_fetch_row(mysql_res);
    unsigned int* length = (unsigned int*) mysql_fetch_lengths(mysql_res);
	  if (!row || !length) {  if (throw_exceptions) throw BadQuery("Bad row");
    else return Row();}
    return Row(row, this, length, throw_exceptions);
  }

  //: raw c api function
  bool          eof () const {return mysql_eof(mysql_res);}
  //: raw c api function
  long unsigned int *fetch_lengths () const {return mysql_fetch_lengths(mysql_res);}
  //: raw c api function

  /* raw mysql c api fields functions */
  Field&       fetch_field () const   {return *mysql_fetch_field(mysql_res);}
  //: raw c api field function
  void         field_seek (int field) {mysql_field_seek (mysql_res, field);}
  //: raw c api field function
  int          num_fields() const {return mysql_num_fields(mysql_res);}
  //: raw c api field function

  ~ResUse (); 

  void parent_leaving() {mysql = NULL;}
  
  void purge(void)
    { if (mysql_res) mysql_free_result(mysql_res); mysql_res=0; if (_names) delete _names; if (_types) delete _types; _names=0; _types=0; _table.erase(); }
  
  operator bool() const {if (mysql_res) return true; return false;} //:
  unsigned int columns() const {return num_fields();} //:

  std::string& table() {return _table;}
  //: table name
  const std::string& table() const {return _table;}
  //: table name

  /* methods for working with field names */
  inline int               field_num(const std::string&) const;
  //: Returns the offset of the filed which equals str.
  inline std::string&           field_name(int);
  //: Returns the field with an offset of i.
  inline const std::string&     field_name(int) const;
  //: Returns the field with an offset of i.
  inline FieldNames&       field_names();
  //: Returns a reference to the underlying FieldNames class.
  inline const FieldNames& field_names() const;
  //: Returns a const reference to the underlaying FieldNames class.
  inline void              reset_field_names();
  //: Resets the field names to there original values.

  /* methods for working with field types */
  inline mysql_type_info&  field_type(int i);
  //: Returns a reference to the  mysql field type for the field with an offset of i.
  inline const mysql_type_info& field_type(int) const;
  //: Returns a const reference to the  mysql field type for the field with an offset of i.
  inline FieldTypes&       field_types();
  //: Returns a reference to the underlying FieldTypes
  inline const FieldTypes& field_types() const;
  //: Returns a const reference to the underlying FieldTypes
  inline void              reset_field_types();
  //: Resets the field_types to their original values.

  /* short names for the above methods */
  inline int               names(const std::string& s) const;
  //: Returns the offset of the filed which equals str.
  inline std::string&           names(int i);
  //: Returns the field with an offset of i.
  inline const std::string&     names(int i) const;
  //: Returns the field with an offset of i.
  inline FieldNames&       names();
  //: Returns a reference to the underlying FieldNames class.
  inline const FieldNames& names() const;
  //: Returns a const reference to the underlying FieldNames class.
  inline void              reset_names();
  //: Resets the field names to their original values.

  inline mysql_type_info&  types(int i);
  //: Returns a reference to the  mysql field type for the field with an offset of i.
  inline const mysql_type_info& types(int i) const;
  //: Returns a const reference to the mysql field type for the field with an offset of i.
  inline FieldTypes&       types();
  //: Returns a reference to the underlying FieldTypes
  inline const FieldTypes& types() const;
  //: Returns a const reference to the underlying FieldTypes
  inline void              reset_types();
  //: Resets the field_types to their original values.
  
  /* methods for working with field info */
  const Fields&     fields() const {return _fields;}
  //: returns a reference to the underling Fields structure.
  const Field&      fields(unsigned int i) const {return _fields[i];}
  //: returns a reference to the the mysql field info for a field with an offset of i.

  bool     operator == (const ResUse &other) const
    {return mysql_res == other.mysql_res;}
  //:
  bool     operator != (const ResUse &other) const
    {return mysql_res != other.mysql_res;}
  //:
};

//: This class handles the result set. 
// It is also a Random Access Container that is not LessThanComparable
// and not Assignable. Being a Random Access Container it can
// return a Random Access Iterator or a reverse Random Access Iterator
// yet.
//
class Result : public ResUse, 
	       public const_subscript_container<Result,Row,const Row>
{
  friend class Connection;
public:
  Result () {} //:
  Result (MYSQL_RES *result, bool te = false) 
    : ResUse(result, NULL, te) {mysql = NULL;} //:
  Result (const Result &other) : ResUse(other) {mysql = NULL;} //:
  virtual ~Result() {}  
  // raw mysql c api functions
  const Row fetch_row() const
  {
	  if (!mysql_res) {  if (throw_exceptions) throw BadQuery("Results not fetched");
    else return Row();}
    MYSQL_ROW row = mysql_fetch_row(mysql_res);
    unsigned int* length = (unsigned int*) mysql_fetch_lengths(mysql_res);
	  if (!row || !length) {  if (throw_exceptions) throw BadQuery("Bad row");
    else return Row();}
    return Row(row, this, length, throw_exceptions);
  }

  //: Raw c api function
  int          num_rows() const {return mysql_num_rows(mysql_res);}
  //: Raw c api function
  void         data_seek (uint offset) const 
    {mysql_data_seek(mysql_res, offset);}
  //: Raw c api function

  size_type size() const {return num_rows();}
  //: Returns the number of rows
  size_type rows() const {return num_rows();}
  //: Returns the number of rows.
  const Row operator [] (size_type i) const {data_seek(i); return fetch_row();}
  //: Returns the row with an offset of i.
};


//! with_class = ResUSe

//:
inline void swap (ResUse &x, ResUse &y) {
  ResUse tmp = x;
  x = y;
  y = tmp;
}

//! with_class = Result

//:
inline void swap (Result &x, Result &y) {
  Result tmp = x;
  x = y;
  y = tmp;
}

//: This structure holds the information on the success of queries that
//: don't return any results.
struct ResNSel {
  bool     success;   
  int      insert_id; //: 
  int      rows;      //: Number of rows affected
  std::string   info;      //: Additional info

  ResNSel() : success(false) {};
  ResNSel (Connection *q);

  operator bool() {return success;}  //: If the query was successful
};  


#endif






